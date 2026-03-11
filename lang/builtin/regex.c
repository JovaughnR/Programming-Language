#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>

#include "./lib/regex.h"

#include "../core/lib/error.h"
#include "../core/lib/type.h"
#include "../core/lib/maloc.h"
#include "../core/lib/daloc.h"
#include "../core/lib/list.h"
#include "../core/lib/utils.h"

const ModuleFuncInfo RE_MODULE[] = {
    {MODULE_REGEX, RE_MATCH, "__re_match__", 2, 2},
    {MODULE_REGEX, RE_SEARCH, "__re_search__", 2, 2},
    {MODULE_REGEX, RE_FINDALL, "__re_findall__", 2, 2},
    {MODULE_REGEX, RE_REPLACE, "__re_replace__", 3, 3},
    {MODULE_REGEX, RE_SPLIT, "__re_split__", 2, 2},
    {MODULE_REGEX, RE_TEST, "__re_test__", 2, 2},
};

const int RE_MODULE_COUNT = sizeof(RE_MODULE) / sizeof(ModuleFuncInfo);

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

// Translate Perl-style shortcuts to POSIX ERE equivalents
static char *translate_pattern(const char *pattern)
{
   int len = strlen(pattern);
   int cap = len * 8 + 1; // generous cap to handle expansions
   char *result = malloc(cap);
   int i = 0, j = 0;

   while (i < len)
   {
      if (pattern[i] == '\\' && i + 1 < len)
      {
         i++;
         const char *r = NULL;
         switch (pattern[i])
         {
         case 'd':
            r = "[0-9]";
            break;
         case 'D':
            r = "[^0-9]";
            break;
         case 'w':
            r = "[a-zA-Z0-9_]";
            break;
         case 'W':
            r = "[^a-zA-Z0-9_]";
            break;
         case 's':
            r = "[ \t\n\r\f\v]";
            break;
         case 'S':
            r = "[^ \t\n\r\f\v]";
            break;
         default:
            result[j++] = '\\';
            result[j++] = pattern[i];
            break;
         }

         if (r)
         {
            int rlen = strlen(r);
            memcpy(result + j, r, rlen);
            j += rlen;
         }
      }
      else
         result[j++] = pattern[i];
      i++;
   }

   result[j] = '\0';
   return result;
}

static int compile_regex(regex_t *re, const char *pattern, const char *funcname)
{
   char *translated = translate_pattern(pattern);
   int flags = REG_EXTENDED;
   int err = regcomp(re, translated, flags);
   free(translated);

   if (err)
   {
      char errbuf[256];
      regerror(err, re, errbuf, sizeof(errbuf));
      throw_error(ERROR_VALUE, "%s() invalid pattern: %s", funcname, errbuf);
      return 0;
   }
   return 1;
}

//=========================================================
//  Implementations
//=========================================================

// test(pattern, string) -> bool
// Returns true if the pattern matches anywhere in the string.
// Example: re.test("\\d+", "abc123") -> true
// Example: re.test("^\\d+$", "abc")  -> false
Data *__re_test__(Data *pattern, Data *string)
{
   const char *pat = get_str(pattern, "re.test");
   const char *str = get_str(string, "re.test");
   if (!pat || !str)
      return createData(TYPE_BOOL, &(int){0});

   regex_t re;
   if (!compile_regex(&re, pat, "re.test"))
      return createData(TYPE_BOOL, &(int){0});

   int result = regexec(&re, str, 0, NULL, 0) == 0;
   regfree(&re);
   return createData(TYPE_BOOL, &result);
}

// match(pattern, string) -> str | none
// Returns the first match or none if no match found.
// Anchored to the start of the string.
// Example: re.match("\\d+", "123abc") -> "123"
// Example: re.match("\\d+", "abc123") -> none
Data *__re_match__(Data *pattern, Data *string)
{
   const char *pat = get_str(pattern, "re.match");
   const char *str = get_str(string, "re.match");
   if (!pat || !str)
      return createData(TYPE_NONE, NULL);

   char *translated = translate_pattern(pat);
   char *anchored = malloc(strlen(translated) + 2);
   anchored[0] = '^';
   strcpy(anchored + 1, translated);
   free(translated);

   regex_t re;
   regmatch_t match;

   int flags = REG_EXTENDED;
   int err = regcomp(&re, anchored, flags);
   free(anchored);

   if (err)
   {
      char errbuf[256];
      regerror(err, &re, errbuf, sizeof(errbuf));
      throw_error(ERROR_VALUE, "re.match() invalid pattern: %s", errbuf);
      return createData(TYPE_NONE, NULL);
   }

   if (regexec(&re, str, 1, &match, 0) != 0)
   {
      regfree(&re);
      return createData(TYPE_NONE, NULL);
   }

   int len = match.rm_eo - match.rm_so;
   char *result = malloc(len + 1);
   strncpy(result, str + match.rm_so, len);
   result[len] = '\0';

   regfree(&re);
   return createData(TYPE_STR, result);
}

// search(pattern, string) -> str | none
// Returns the first match found anywhere in the string, or none.
// Example: re.search("\\d+", "abc123def") -> "123"
Data *__re_search__(Data *pattern, Data *string)
{
   const char *pat = get_str(pattern, "re.search");
   const char *str = get_str(string, "re.search");
   if (!pat || !str)
      return createData(TYPE_NONE, NULL);

   regex_t re;
   regmatch_t match;

   if (!compile_regex(&re, pat, "re.search"))
      return createData(TYPE_NONE, NULL);

   if (regexec(&re, str, 1, &match, 0) != 0)
   {
      regfree(&re);
      return createData(TYPE_NONE, NULL);
   }

   int len = match.rm_eo - match.rm_so;
   char *result = malloc(len + 1);
   strncpy(result, str + match.rm_so, len);
   result[len] = '\0';

   regfree(&re);
   return createData(TYPE_STR, result);
}

// findall(pattern, string) -> list
// Returns a list of all non-overlapping matches.
// Example: re.findall("\\d+", "a1b22c333") -> ["1", "22", "333"]
Data *__re_findall__(Data *pattern, Data *string)
{
   const char *pat = get_str(pattern, "re.findall");
   const char *str = get_str(string, "re.findall");
   if (!pat || !str)
      return createData(TYPE_NONE, NULL);

   regex_t re;
   regmatch_t match;

   if (!compile_regex(&re, pat, "re.findall"))
      return createData(TYPE_NONE, NULL);

   List *result = list_create(__size__);
   const char *cursor = str;

   while (regexec(&re, cursor, 1, &match, 0) == 0)
   {
      int len = match.rm_eo - match.rm_so;
      char *item = malloc(len + 1);
      strncpy(item, cursor + match.rm_so, len);
      item[len] = '\0';
      list_append(createData(TYPE_STR, item), result);

      cursor += match.rm_eo;
      if (match.rm_eo == 0)
         cursor++; // avoid infinite loop on empty match
   }

   regfree(&re);
   return createData(TYPE_LIST, result);
}

// replace(pattern, replacement, string) -> str
// Replaces all matches of pattern with replacement.
// Example: re.replace("\\d+", "#", "a1b22c333") -> "a#b#c#"
Data *__re_replace__(Data *pattern, Data *replacement, Data *string)
{
   const char *pat = get_str(pattern, "re.replace");
   const char *repl = get_str(replacement, "re.replace");
   const char *str = get_str(string, "re.replace");
   if (!pat || !repl || !str)
      return createData(TYPE_NONE, NULL);

   regex_t re;
   regmatch_t match;

   if (!compile_regex(&re, pat, "re.replace"))
      return createData(TYPE_NONE, NULL);

   int rlen = strlen(repl);
   int cap = strlen(str) * 2 + 64;
   char *result = malloc(cap);
   char *w = result;
   const char *cursor = str;

   while (regexec(&re, cursor, 1, &match, 0) == 0)
   {
      int before = match.rm_so;
      int needed = (w - result) + before + rlen + strlen(cursor) + 1;
      if (needed > cap)
      {
         int offset = w - result;
         cap = needed * 2;
         result = realloc(result, cap);
         w = result + offset;
      }

      memcpy(w, cursor, before);
      w += before;
      memcpy(w, repl, rlen);
      w += rlen;

      cursor += match.rm_eo;
      if (match.rm_eo == 0)
      {
         *w++ = *cursor++;
      } // avoid infinite loop
   }

   strcpy(w, cursor); // append remainder
   regfree(&re);
   return createData(TYPE_STR, result);
}

// split(pattern, string) -> list
// Splits string by all matches of pattern.
// Example: re.split("\\s+", "hello   world  cx") -> ["hello", "world", "cx"]
Data *__re_split__(Data *pattern, Data *string)
{
   const char *pat = get_str(pattern, "re.split");
   const char *str = get_str(string, "re.split");
   if (!pat || !str)
      return createData(TYPE_NONE, NULL);

   regex_t re;
   regmatch_t match;

   if (!compile_regex(&re, pat, "re.split"))
      return createData(TYPE_NONE, NULL);

   List *result = list_create(__size__);
   const char *cursor = str;

   while (regexec(&re, cursor, 1, &match, 0) == 0)
   {
      int len = match.rm_so;
      char *item = malloc(len + 1);
      strncpy(item, cursor, len);
      item[len] = '\0';
      list_append(createData(TYPE_STR, item), result);

      cursor += match.rm_eo;
      if (match.rm_eo == 0)
         cursor++;
   }

   // Append the remainder
   list_append(createData(TYPE_STR, strdup(cursor)), result);

   regfree(&re);
   return createData(TYPE_LIST, result);
}

//=========================================================
//  Module Initializer
//=========================================================

void initializeReModule(Runtime *mod_rt)
{
   for (int i = 0; i < RE_MODULE_COUNT; i++)
   {
      Data *key = createData(TYPE_STR, (void *)RE_MODULE[i].name);
      Data *val = createData(TYPE_MODULE, (void *)&RE_MODULE[i]);
      env_save(key, val, mod_rt->env);
      data_free(key);
   }
}

//=========================================================
//  Dispatcher
//=========================================================

Data *dispatchReBuiltin(ReType type, List *args, Dict *kwargs, Runtime *rt)
{
   UNUSED(kwargs);
   UNUSED(rt);

   switch (type)
   {
   case RE_TEST:
      return __re_test__(ARG(0), ARG(1));
   case RE_MATCH:
      return __re_match__(ARG(0), ARG(1));
   case RE_SEARCH:
      return __re_search__(ARG(0), ARG(1));
   case RE_FINDALL:
      return __re_findall__(ARG(0), ARG(1));
   case RE_REPLACE:
      return __re_replace__(ARG(0), ARG(1), ARG(2));
   case RE_SPLIT:
      return __re_split__(ARG(0), ARG(1));
   default:
      throw_error(ERROR_SYNTAX, "unknown re builtin type %d", type);
      return createData(TYPE_NONE, NULL);
   }
}