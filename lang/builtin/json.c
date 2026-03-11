#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "./lib/json.h"

#include "../core/lib/error.h"
#include "../core/lib/type.h"
#include "../core/lib/maloc.h"
#include "../core/lib/daloc.h"
#include "../core/lib/list.h"
#include "../core/lib/dict.h"
#include "../core/lib/utils.h"

const ModuleFuncInfo JSON_MODULE[] = {
    {MODULE_JSON, JSON_PARSE, "__json_parse__", 1, 1},
    {MODULE_JSON, JSON_STRINGIFY, "__json_stringify__", 1, 2},
};

const int JSON_MODULE_COUNT = sizeof(JSON_MODULE) / sizeof(ModuleFuncInfo);

//=========================================================
//  Parser
//=========================================================

static Data *parse_value(const char **p);

static void skip_whitespace(const char **p)
{
   while (**p && isspace((unsigned char)**p))
      (*p)++;
}

static Data *parse_string(const char **p)
{
   (*p)++; // skip opening "
   const char *start = *p;
   UNUSED(start);
   int len = 0;

   // First pass — measure length handling escapes
   const char *q = *p;
   while (*q && *q != '"')
   {
      if (*q == '\\')
         q++;
      q++;
      len++;
   }

   char *result = malloc(len + 1);
   int i = 0;

   while (**p && **p != '"')
   {
      if (**p == '\\')
      {
         (*p)++;
         switch (**p)
         {
         case '"':
            result[i++] = '"';
            break;
         case '\\':
            result[i++] = '\\';
            break;
         case '/':
            result[i++] = '/';
            break;
         case 'n':
            result[i++] = '\n';
            break;
         case 't':
            result[i++] = '\t';
            break;
         case 'r':
            result[i++] = '\r';
            break;
         case 'b':
            result[i++] = '\b';
            break;
         case 'f':
            result[i++] = '\f';
            break;
         default:
            result[i++] = **p;
            break;
         }
      }
      else
         result[i++] = **p;
      (*p)++;
   }

   result[i] = '\0';
   if (**p == '"')
      (*p)++; // skip closing "
   return createData(TYPE_STR, result);
}

static Data *parse_number(const char **p)
{
   const char *start = *p;
   int is_float = 0;

   if (**p == '-')
      (*p)++;
   while (isdigit((unsigned char)**p))
      (*p)++;
   if (**p == '.')
   {
      is_float = 1;
      (*p)++;
   }
   while (isdigit((unsigned char)**p))
      (*p)++;
   if (**p == 'e' || **p == 'E')
   {
      is_float = 1;
      (*p)++;
      if (**p == '+' || **p == '-')
         (*p)++;
      while (isdigit((unsigned char)**p))
         (*p)++;
   }

   char buf[64];
   int len = *p - start;
   strncpy(buf, start, len);
   buf[len] = '\0';

   if (is_float)
   {
      double val = atof(buf);
      return createData(TYPE_FLOAT, &val);
   }
   int val = atoi(buf);
   return createData(TYPE_INT, &val);
}

static Data *parse_array(const char **p)
{
   (*p)++; // skip [
   List *list = list_create(__size__);

   skip_whitespace(p);
   if (**p == ']')
   {
      (*p)++;
      return createData(TYPE_LIST, list);
   }

   while (**p)
   {
      skip_whitespace(p);
      Data *val = parse_value(p);
      if (!val)
         return createData(TYPE_NONE, NULL);
      list_append(val, list);

      skip_whitespace(p);
      if (**p == ',')
      {
         (*p)++;
         continue;
      }
      if (**p == ']')
      {
         (*p)++;
         break;
      }

      throw_error(ERROR_VALUE, "json.parse(): expected ',' or ']'");
      return createData(TYPE_NONE, NULL);
   }

   return createData(TYPE_LIST, list);
}

static Data *parse_object(const char **p)
{
   (*p)++; // skip {
   Dict *dict = dict_create(__size__);

   skip_whitespace(p);
   if (**p == '}')
   {
      (*p)++;
      return createData(TYPE_DICT, dict);
   }

   while (**p)
   {
      skip_whitespace(p);
      if (**p != '"')
      {
         throw_error(ERROR_VALUE, "json.parse(): expected string key");
         return createData(TYPE_NONE, NULL);
      }

      Data *key = parse_string(p);
      skip_whitespace(p);

      if (**p != ':')
      {
         throw_error(ERROR_VALUE, "json.parse(): expected ':' after key");
         data_free(key);
         return createData(TYPE_NONE, NULL);
      }
      (*p)++; // skip :

      skip_whitespace(p);
      Data *val = parse_value(p);
      if (!val)
      {
         data_free(key);
         return createData(TYPE_NONE, NULL);
      }

      dict_insert(key, val, dict);

      skip_whitespace(p);
      if (**p == ',')
      {
         (*p)++;
         continue;
      }
      if (**p == '}')
      {
         (*p)++;
         break;
      }

      throw_error(ERROR_VALUE, "json.parse(): expected ',' or '}'");
      return createData(TYPE_NONE, NULL);
   }

   return createData(TYPE_DICT, dict);
}

static Data *parse_value(const char **p)
{
   skip_whitespace(p);

   switch (**p)
   {
   case '"':
      return parse_string(p);
   case '[':
      return parse_array(p);
   case '{':
      return parse_object(p);
   case 't':
      if (strncmp(*p, "true", 4) == 0)
      {
         *p += 4;
         return createData(TYPE_BOOL, &(int){1});
      }
      break;
   case 'f':
      if (strncmp(*p, "false", 5) == 0)
      {
         *p += 5;
         return createData(TYPE_BOOL, &(int){0});
      }
      break;
   case 'n':
      if (strncmp(*p, "null", 4) == 0)
      {
         *p += 4;
         return createData(TYPE_NONE, NULL);
      }
      break;
   default:
      if (**p == '-' || isdigit((unsigned char)**p))
         return parse_number(p);
      break;
   }

   throw_error(ERROR_VALUE, "json.parse(): unexpected character '%c'", **p);
   return createData(TYPE_NONE, NULL);
}

//=========================================================
//  Stringifier
//=========================================================

static void stringify_value(Data *d, int indent, int depth, char **buf, int *len, int *cap);

static void buf_append(char **buf, int *len, int *cap, const char *s, int slen)
{
   while (*len + slen + 1 > *cap)
   {
      *cap *= 2;
      *buf = realloc(*buf, *cap);
   }
   memcpy(*buf + *len, s, slen);
   *len += slen;
   (*buf)[*len] = '\0';
}

static void buf_char(char **buf, int *len, int *cap, char c)
{
   buf_append(buf, len, cap, &c, 1);
}

static void buf_str(char **buf, int *len, int *cap, const char *s)
{
   buf_append(buf, len, cap, s, strlen(s));
}

static void write_indent(char **buf, int *len, int *cap, int indent, int depth)
{
   if (indent <= 0)
      return;
   buf_char(buf, len, cap, '\n');
   for (int i = 0; i < depth * indent; i++)
      buf_char(buf, len, cap, ' ');
}

static void stringify_string(const char *s, char **buf, int *len, int *cap)
{
   buf_char(buf, len, cap, '"');
   for (int i = 0; s[i]; i++)
   {
      switch (s[i])
      {
      case '"':
         buf_str(buf, len, cap, "\\\"");
         break;
      case '\\':
         buf_str(buf, len, cap, "\\\\");
         break;
      case '\n':
         buf_str(buf, len, cap, "\\n");
         break;
      case '\t':
         buf_str(buf, len, cap, "\\t");
         break;
      case '\r':
         buf_str(buf, len, cap, "\\r");
         break;
      default:
      {
         char c = s[i];
         buf_append(buf, len, cap, &c, 1);
      }
      }
   }
   buf_char(buf, len, cap, '"');
}

static void stringify_value(Data *d, int indent, int depth, char **buf, int *len, int *cap)
{
   if (!d || d->type == TYPE_NONE)
   {
      buf_str(buf, len, cap, "null");
      return;
   }

   char tmp[64];
   switch (d->type)
   {
   case TYPE_BOOL:
      buf_str(buf, len, cap, *(int *)d->atom ? "true" : "false");
      break;

   case TYPE_INT:
      snprintf(tmp, sizeof(tmp), "%d", *(int *)d->atom);
      buf_str(buf, len, cap, tmp);
      break;

   case TYPE_FLOAT:
      snprintf(tmp, sizeof(tmp), "%g", *(double *)d->real);
      buf_str(buf, len, cap, tmp);
      break;

   case TYPE_STR:
      stringify_string(d->str, buf, len, cap);
      break;

   case TYPE_LIST:
   {
      List *list = LIST_PTR(d);
      buf_char(buf, len, cap, '[');
      for (int i = 0; i < list->length; i++)
      {
         if (indent > 0)
            write_indent(buf, len, cap, indent, depth + 1);
         stringify_value((Data *)list->items[i], indent, depth + 1, buf, len, cap);
         if (i < list->length - 1)
            buf_char(buf, len, cap, ',');
      }
      if (indent > 0 && list->length > 0)
         write_indent(buf, len, cap, indent, depth);
      buf_char(buf, len, cap, ']');
      break;
   }

   case TYPE_DICT:
   {
      Dict *dict = DICT_PTR(d);
      buf_char(buf, len, cap, '{');
      int first = 1;
      for (int i = 0; i < dict->size; i++)
      {
         for (Pair *pair = dict->buckets[i]; pair; pair = pair->next)
         {
            Data *key = (Data *)pair->key;
            Data *val = (Data *)pair->value;

            if (!first)
               buf_char(buf, len, cap, ',');
            if (indent > 0)
               write_indent(buf, len, cap, indent, depth + 1);

            stringify_string(key->str, buf, len, cap);
            buf_char(buf, len, cap, ':');
            if (indent > 0)
               buf_char(buf, len, cap, ' ');
            stringify_value(val, indent, depth + 1, buf, len, cap);
            first = 0;
         }
      }
      if (indent > 0 && !first)
         write_indent(buf, len, cap, indent, depth);
      buf_char(buf, len, cap, '}');
      break;
   }

   default:
      buf_str(buf, len, cap, "null");
      break;
   }
}

//=========================================================
//  Public Functions
//=========================================================

// parse(s) -> dict | list | str | int | float | bool | none
// Parses a JSON string into a CX value.
// Example: data = json.parse('{"name": "cx", "version": 1}')
Data *__json_parse__(Data *s)
{
   if (!s || s->type == TYPE_NONE || s->type != TYPE_STR)
   {
      throw_error(ERROR_TYPE, "json.parse() argument must be a string");
      return createData(TYPE_NONE, NULL);
   }

   const char *p = s->str;
   Data *result = parse_value(&p);

   skip_whitespace(&p);
   if (*p != '\0')
   {
      throw_error(ERROR_VALUE, "json.parse(): unexpected trailing content");
      data_free(result);
      return createData(TYPE_NONE, NULL);
   }

   return result;
}

// stringify(value, indent=0) -> str
// Converts a CX value to a JSON string.
// Use indent > 0 for pretty printing.
// Example: json.stringify({"name": "cx"})        -> '{"name":"cx"}'
// Example: json.stringify({"name": "cx"}, 2)     -> pretty printed
Data *__json_stringify__(Data *value, Data *indent)
{
   int ind = 0;
   if (indent && indent->type == TYPE_INT)
      ind = *(int *)indent->atom;

   int cap = 256;
   int len = 0;
   char *buf = malloc(cap);
   buf[0] = '\0';

   stringify_value(value, ind, 0, &buf, &len, &cap);
   return createData(TYPE_STR, buf);
}

//=========================================================
//  Module Initializer
//=========================================================

void initializeJsonModule(Runtime *mod_rt)
{
   for (int i = 0; i < JSON_MODULE_COUNT; i++)
   {
      Data *key = createData(TYPE_STR, (void *)JSON_MODULE[i].name);
      Data *val = createData(TYPE_MODULE, (void *)&JSON_MODULE[i]);
      env_save(key, val, mod_rt->env);
      data_free(key);
   }
}

//=========================================================
//  Dispatcher
//=========================================================

Data *dispatchJsonBuiltin(JsonType type, List *args, Dict *kwargs, Runtime *rt)
{
   UNUSED(kwargs);
   UNUSED(rt);

   switch (type)
   {
   case JSON_PARSE:
      return __json_parse__(ARG(0));
   case JSON_STRINGIFY:
      return __json_stringify__(ARG(0), ARG(1));
   default:
      throw_error(ERROR_SYNTAX, "unknown json builtin type %d", type);
      return createData(TYPE_NONE, NULL);
   }
}