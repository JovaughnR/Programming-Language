#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

#include "./lib/net.h"

#include "../core/lib/error.h"
#include "../core/lib/type.h"
#include "../core/lib/maloc.h"
#include "../core/lib/daloc.h"
#include "../core/lib/list.h"
#include "../core/lib/dict.h"
#include "../core/lib/utils.h"

const ModuleFuncInfo NET_MODULE[] = {
    {MODULE_NET, NET_GET, "__net_get__", 1, 2},
    {MODULE_NET, NET_POST, "__net_post__", 1, 3},
    {MODULE_NET, NET_PUT, "__net_put__", 1, 3},
    {MODULE_NET, NET_DELETE, "__net_delete__", 1, 2},
    {MODULE_NET, NET_PATCH, "__net_patch__", 1, 3},
    {MODULE_NET, NET_HEAD, "__net_head__", 1, 2},
    {MODULE_NET, NET_REQUEST, "__net_request__", 1, 1},
};

const int NET_MODULE_COUNT = sizeof(NET_MODULE) / sizeof(ModuleFuncInfo);

//=========================================================
//  Response Buffer
//=========================================================

typedef struct
{
   char *data;
   size_t len;
   size_t cap;
} Buffer;

static void buffer_init(Buffer *b)
{
   b->data = malloc(256);
   b->len = 0;
   b->cap = 256;
   b->data[0] = '\0';
}

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
   Buffer *b = (Buffer *)userdata;
   size_t bytes = size * nmemb;

   while (b->len + bytes + 1 > b->cap)
   {
      b->cap *= 2;
      b->data = realloc(b->data, b->cap);
   }

   memcpy(b->data + b->len, ptr, bytes);
   b->len += bytes;
   b->data[b->len] = '\0';
   return bytes;
}

// Header callback — stores headers into a Buffer
static size_t header_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
   Buffer *b = (Buffer *)userdata;
   size_t bytes = size * nmemb;

   while (b->len + bytes + 1 > b->cap)
   {
      b->cap *= 2;
      b->data = realloc(b->data, b->cap);
   }

   memcpy(b->data + b->len, ptr, bytes);
   b->len += bytes;
   b->data[b->len] = '\0';
   return bytes;
}

//=========================================================
//  Helpers
//=========================================================

static const char *get_str(Data *d, const char *funcname)
{
   if (!d || d->type == TYPE_NONE || d->type != TYPE_STR)
   {
      throw_error(ERROR_TYPE, "%s() argument must be a string", funcname);
      return NULL;
   }
   return d->str;
}

// Build curl_slist from a CX dict of headers
static struct curl_slist *build_headers(Data *headers_data)
{
   if (!headers_data || headers_data->type != TYPE_DICT)
      return NULL;

   struct curl_slist *list = NULL;
   Dict *dict = DICT_PTR(headers_data);

   for (int i = 0; i < dict->size; i++)
   {
      for (Pair *p = dict->buckets[i]; p; p = p->next)
      {
         Data *key = (Data *)p->key;
         Data *val = (Data *)p->value;

         if (key->type != TYPE_STR || val->type != TYPE_STR)
            continue;

         char *header = malloc(strlen(key->str) + strlen(val->str) + 3);
         snprintf(header, strlen(key->str) + strlen(val->str) + 3, "%s: %s", key->str, val->str);
         list = curl_slist_append(list, header);
         free(header);
      }
   }

   return list;
}

// Parse raw headers string into a dict
static Data *parse_headers(const char *raw)
{
   Dict *result = dict_create(__size__);
   char *copy = strdup(raw);
   char *line = strtok(copy, "\r\n");

   while (line)
   {
      char *colon = strchr(line, ':');
      if (colon)
      {
         *colon = '\0';
         char *key = line;
         char *val = colon + 1;
         while (*val == ' ')
            val++; // strip leading space

         Data *k = createData(TYPE_STR, strdup(key));
         Data *v = createData(TYPE_STR, strdup(val));
         dict_insert(k, v, result);
      }
      line = strtok(NULL, "\r\n");
   }

   free(copy);
   return createData(TYPE_DICT, result);
}

// Build and execute a response dict: {status, body, headers}
static Data *make_response(long status, Buffer *body, Buffer *headers)
{
   Dict *result = dict_create(__size__);

   Data *status_key = createData(TYPE_STR, "status");
   Data *body_key = createData(TYPE_STR, "body");
   Data *headers_key = createData(TYPE_STR, "headers");

   int s = (int)status;
   dict_insert(status_key, createData(TYPE_INT, &s), result);
   dict_insert(body_key, createData(TYPE_STR, strdup(body->data)), result);
   dict_insert(headers_key, parse_headers(headers->data), result);

   return createData(TYPE_DICT, result);
}

//=========================================================
//  Core Request Function
//=========================================================

typedef struct
{
   const char *method;
   const char *url;
   const char *body; // request body (POST/PUT/PATCH)
   Data *headers;    // dict of headers
   long timeout;     // seconds, 0 = default (30s)
   int follow_redirects;
   int verify_ssl;
} RequestOptions;

static Data *do_request(RequestOptions *opts)
{
   CURL *curl = curl_easy_init();
   if (!curl)
   {
      throw_error(ERROR_RUNTIME, "net: failed to initialize curl");
      return createData(TYPE_NONE, NULL);
   }

   Buffer body_buf, header_buf;
   buffer_init(&body_buf);
   buffer_init(&header_buf);

   // URL
   curl_easy_setopt(curl, CURLOPT_URL, opts->url);

   // Method
   if (strcmp(opts->method, "POST") == 0)
   {
      curl_easy_setopt(curl, CURLOPT_POST, 1L);
      if (opts->body)
      {
         curl_easy_setopt(curl, CURLOPT_POSTFIELDS, opts->body);
         curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(opts->body));
      }
      else
      {
         curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
         curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, 0L);
      }
   }
   else if (strcmp(opts->method, "PUT") == 0)
   {
      curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
      if (opts->body)
      {
         curl_easy_setopt(curl, CURLOPT_POSTFIELDS, opts->body);
         curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(opts->body));
      }
   }
   else if (strcmp(opts->method, "PATCH") == 0)
   {
      curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
      if (opts->body)
      {
         curl_easy_setopt(curl, CURLOPT_POSTFIELDS, opts->body);
         curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(opts->body));
      }
   }
   else if (strcmp(opts->method, "DELETE") == 0)
      curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
   else if (strcmp(opts->method, "HEAD") == 0)
      curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
   // GET is default

   // Headers
   struct curl_slist *headers = build_headers(opts->headers);

   // Always set a default User-Agent
   headers = curl_slist_append(headers, "User-Agent: cx/1.0");

   curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

   // Callbacks
   curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
   curl_easy_setopt(curl, CURLOPT_WRITEDATA, &body_buf);
   curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
   curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_buf);

   // Options
   curl_easy_setopt(curl, CURLOPT_TIMEOUT, opts->timeout > 0 ? opts->timeout : 30L);
   curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, opts->follow_redirects ? 1L : 0L);
   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, opts->verify_ssl ? 1L : 0L);
   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, opts->verify_ssl ? 2L : 0L);

   // Execute
   CURLcode res = curl_easy_perform(curl);

   Data *result;
   if (res != CURLE_OK)
   {
      throw_error(ERROR_RUNTIME, "net request failed: %s", curl_easy_strerror(res));
      result = createData(TYPE_NONE, NULL);
   }
   else
   {
      long status = 0;
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);
      result = make_response(status, &body_buf, &header_buf);
   }

   curl_slist_free_all(headers);
   curl_easy_cleanup(curl);
   free(body_buf.data);
   free(header_buf.data);

   return result;
}

//=========================================================
//  Public Functions
//=========================================================

// get(url, headers=none) -> dict
// Performs an HTTP GET request.
// Returns dict with keys: status (int), body (str), headers (dict)
// Example: res = net.get("https://api.example.com/users")
// Example: print(res["status"]);
// Example: print(res["body"]);
Data *__net_get__(Data *url, Data *headers)
{
   const char *u = get_str(url, "net.get");
   if (!u)
      return createData(TYPE_NONE, NULL);

   RequestOptions opts = {
       .method = "GET",
       .url = u,
       .body = NULL,
       .headers = headers,
       .timeout = 30,
       .follow_redirects = 1,
       .verify_ssl = 1,
   };
   return do_request(&opts);
}

// post(url, body="", headers=none) -> dict
// Performs an HTTP POST request.
// Example: res = net.post("https://api.example.com/users", json.stringify(data))
Data *__net_post__(Data *url, Data *body, Data *headers)
{
   const char *u = get_str(url, "net.post");
   if (!u)
      return createData(TYPE_NONE, NULL);

   const char *b = NULL;
   if (body && body->type == TYPE_STR)
      b = body->str;

   RequestOptions opts = {
       .method = "POST",
       .url = u,
       .body = b,
       .headers = headers,
       .timeout = 30,
       .follow_redirects = 1,
       .verify_ssl = 1,
   };
   return do_request(&opts);
}

// put(url, body="", headers=none) -> dict
// Performs an HTTP PUT request.
// Example: res = net.put("https://api.example.com/users/1", json.stringify(data))
Data *__net_put__(Data *url, Data *body, Data *headers)
{
   const char *u = get_str(url, "net.put");
   if (!u)
      return createData(TYPE_NONE, NULL);

   const char *b = NULL;
   if (body && body->type == TYPE_STR)
      b = body->str;

   RequestOptions opts = {
       .method = "PUT",
       .url = u,
       .body = b,
       .headers = headers,
       .timeout = 30,
       .follow_redirects = 1,
       .verify_ssl = 1,
   };
   return do_request(&opts);
}

// delete(url, headers=none) -> dict
// Performs an HTTP DELETE request.
// Example: res = net.delete("https://api.example.com/users/1")
Data *__net_delete__(Data *url, Data *headers)
{
   const char *u = get_str(url, "net.delete");
   if (!u)
      return createData(TYPE_NONE, NULL);

   RequestOptions opts = {
       .method = "DELETE",
       .url = u,
       .body = NULL,
       .headers = headers,
       .timeout = 30,
       .follow_redirects = 1,
       .verify_ssl = 1,
   };
   return do_request(&opts);
}

// patch(url, body="", headers=none) -> dict
// Performs an HTTP PATCH request.
// Example: res = net.patch("https://api.example.com/users/1", json.stringify(data))
Data *__net_patch__(Data *url, Data *body, Data *headers)
{
   const char *u = get_str(url, "net.patch");
   if (!u)
      return createData(TYPE_NONE, NULL);

   const char *b = NULL;
   if (body && body->type == TYPE_STR)
      b = body->str;

   RequestOptions opts = {
       .method = "PATCH",
       .url = u,
       .body = b,
       .headers = headers,
       .timeout = 30,
       .follow_redirects = 1,
       .verify_ssl = 1,
   };
   return do_request(&opts);
}

// head(url, headers=none) -> dict
// Performs an HTTP HEAD request — returns headers only, no body.
// Example: res = net.head("https://example.com")
// Example: print(res["headers"]["Content-Type"])
Data *__net_head__(Data *url, Data *headers)
{
   const char *u = get_str(url, "net.head");
   if (!u)
      return createData(TYPE_NONE, NULL);

   RequestOptions opts = {
       .method = "HEAD",
       .url = u,
       .body = NULL,
       .headers = headers,
       .timeout = 30,
       .follow_redirects = 1,
       .verify_ssl = 1,
   };
   return do_request(&opts);
}

// request(options) -> dict
// Performs a fully customized HTTP request via an options dict.
// Options keys:
//   "method"           -> str  (required) "GET", "POST", "PUT", etc.
//   "url"              -> str  (required)
//   "body"             -> str  (optional) request body
//   "headers"          -> dict (optional) request headers
//   "timeout"          -> int  (optional, default 30)
//   "follow_redirects" -> bool (optional, default true)
//   "verify_ssl"       -> bool (optional, default true)
// Example:
//   opts = dict();
//   opts["method"]  = "POST";
//   opts["url"]     = "https://api.example.com/data";
//   opts["body"]    = json.stringify(payload);
//   opts["headers"] = {"Content-Type": "application/json"};
//   res = net.request(opts);
Data *__net_request__(Data *options)
{
   if (!options || options->type != TYPE_DICT)
   {
      throw_error(ERROR_TYPE, "net.request() argument must be a dict");
      return createData(TYPE_NONE, NULL);
   }

   Dict *opts = DICT_PTR(options);

   // Required: method
   Data *method_key = createData(TYPE_STR, "method");
   Data *method_val = dict_get(method_key, opts);
   data_free(method_key);
   if (!method_val || method_val->type != TYPE_STR)
   {
      throw_error(ERROR_VALUE, "net.request() options must include 'method'");
      return createData(TYPE_NONE, NULL);
   }

   // Required: url
   Data *url_key = createData(TYPE_STR, "url");
   Data *url_val = dict_get(url_key, opts);
   data_free(url_key);
   if (!url_val || url_val->type != TYPE_STR)
   {
      throw_error(ERROR_VALUE, "net.request() options must include 'url'");
      return createData(TYPE_NONE, NULL);
   }

   // Optional: body
   Data *body_key = createData(TYPE_STR, "body");
   Data *body_val = dict_get(body_key, opts);
   data_free(body_key);
   const char *body = body_val && body_val->type == TYPE_STR ? body_val->str : NULL;

   // Optional: headers
   Data *headers_key = createData(TYPE_STR, "headers");
   Data *headers_val = dict_get(headers_key, opts);
   data_free(headers_key);

   // Optional: timeout
   Data *timeout_key = createData(TYPE_STR, "timeout");
   Data *timeout_val = dict_get(timeout_key, opts);
   data_free(timeout_key);
   long timeout = timeout_val && timeout_val->type == TYPE_INT
                      ? (long)*(int *)timeout_val->atom
                      : 30L;

   // Optional: follow_redirects
   Data *redir_key = createData(TYPE_STR, "follow_redirects");
   Data *redir_val = dict_get(redir_key, opts);
   data_free(redir_key);
   int follow = redir_val && redir_val->type == TYPE_BOOL
                    ? *(int *)redir_val->atom
                    : 1;

   // Optional: verify_ssl
   Data *ssl_key = createData(TYPE_STR, "verify_ssl");
   Data *ssl_val = dict_get(ssl_key, opts);
   data_free(ssl_key);
   int verify = ssl_val && ssl_val->type == TYPE_BOOL
                    ? *(int *)ssl_val->atom
                    : 1;

   RequestOptions req = {
       .method = method_val->str,
       .url = url_val->str,
       .body = body,
       .headers = headers_val,
       .timeout = timeout,
       .follow_redirects = follow,
       .verify_ssl = verify,
   };

   return do_request(&req);
}

//=========================================================
//  Module Initializer
//=========================================================

void initializeNetModule(Runtime *mod_rt)
{
   curl_global_init(CURL_GLOBAL_DEFAULT);

   for (int i = 0; i < NET_MODULE_COUNT; i++)
   {
      Data *key = createData(TYPE_STR, (void *)NET_MODULE[i].name);
      Data *val = createData(TYPE_MODULE, (void *)&NET_MODULE[i]);
      env_save(key, val, mod_rt->env);
      data_free(key);
   }
}

//=========================================================
//  Dispatcher
//=========================================================

Data *dispatchNetBuiltin(NetType type, List *args, Dict *kwargs, Runtime *rt)
{
   UNUSED(kwargs);
   UNUSED(rt);

   switch (type)
   {
   case NET_GET:
      return __net_get__(ARG(0), ARG(1));
   case NET_POST:
      return __net_post__(ARG(0), ARG(1), ARG(2));
   case NET_PUT:
      return __net_put__(ARG(0), ARG(1), ARG(2));
   case NET_DELETE:
      return __net_delete__(ARG(0), ARG(1));
   case NET_PATCH:
      return __net_patch__(ARG(0), ARG(1), ARG(2));
   case NET_HEAD:
      return __net_head__(ARG(0), ARG(1));
   case NET_REQUEST:
      return __net_request__(ARG(0));
   default:
      throw_error(ERROR_SYNTAX, "unknown net builtin type %d", type);
      return createData(TYPE_NONE, NULL);
   }
}