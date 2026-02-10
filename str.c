#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "type.h"

char *str_concat(char *s1, char *s2)
{
   if (!s1 || !s2)
      return NULL;

   size_t len1 = strlen(s1);
   size_t len2 = strlen(s2);

   char *result = malloc(len1 + len2 + 1);
   if (!result)
      return NULL;

   memcpy(result, s1, len1);
   memcpy(result + len1, s2, len2 + 1);

   return result;
}

char *str_replicate(char *str, int times)
{
   if (times <= 0)
      return strdup("");

   size_t len = strlen(str);
   char *result = malloc(len * times + 1);
   if (!result)
      return NULL;

   for (int i = 0; i < times; i++)
      memcpy(result + i * len, str, len);

   result[len * times] = '\0';
   return result;
}

int str_is_substr(const char *subStr, const char *str)
{
   if (!subStr || !str)
      return 0;
   if (*subStr == '\0')
      return 1;
   return strstr(str, subStr) != NULL;
}

int str_lexicographical_compare(const char *str1, const char *str2)
{
   int i = 0;
   while (str1[i] != '\0' && str2[i] != '\0')
   {
      if (str1[i] != str2[i])
         return (unsigned char)str1[i] - (unsigned char)str2[i];
      i++;
   }
   return (unsigned char)str1[i] - (unsigned char)str2[i];
}

char *str_slice(const char *str, int start, int end)
{
   int len = strlen(str);

   // Handle negative indices (Python-style)
   if (start < 0)
      start = len + start;
   if (end < 0)
      end = len + end;

   // Clamp to valid ranges
   if (start < 0)
      start = 0;
   if (end > len)
      end = len;
   if (start >= end)
   {
      char *empty = malloc(1);
      empty[0] = '\0';
      return empty;
   }

   int newLen = end - start;
   char *result = malloc(newLen + 1);

   memcpy(result, str + start, newLen);
   result[newLen] = '\0';

   return result;
}

char *str_char_at(const char *s, int index)
{
   size_t len = strlen(s);

   char str[2];
   str[0] = s[index];
   str[1] = '\0';

   return str;
}

char *str_to_uppercase(const char *str)
{
   if (!str)
      return NULL;

   size_t len = strlen(str);
   char *result = malloc(len + 1);
   if (!result)
      return NULL;

   for (size_t i = 0; i < len; i++)
      result[i] = toupper((unsigned char)str[i]);

   result[len] = '\0';
   return result;
}

char *str_to_lowercase(const char *str)
{
   if (!str)
      return NULL;

   size_t len = strlen(str);
   char *result = malloc(len + 1);
   if (!result)
      return NULL;

   for (size_t i = 0; i < len; i++)
      result[i] = tolower((unsigned char)str[i]);

   result[len] = '\0';
   return result;
}

char *str_title_case(const char *str)
{
   if (!str)
      return NULL;

   size_t len = strlen(str);
   char *result = malloc(len + 1);
   if (!result)
      return NULL;

   int capitalize_next = 1;
   for (size_t i = 0; i < len; i++)
   {
      if (isspace((unsigned char)str[i]))
      {
         result[i] = str[i];
         capitalize_next = 1;
      }
      else if (capitalize_next)
      {
         result[i] = toupper((unsigned char)str[i]);
         capitalize_next = 0;
      }
      else
      {
         result[i] = tolower((unsigned char)str[i]);
      }
   }

   result[len] = '\0';
   return result;
}

char *str_capitalize(const char *str)
{
   if (!str)
      return NULL;

   size_t len = strlen(str);
   if (len == 0)
      return strdup("");

   char *result = malloc(len + 1);
   if (!result)
      return NULL;

   result[0] = toupper((unsigned char)str[0]);
   for (size_t i = 1; i < len; i++)
      result[i] = tolower((unsigned char)str[i]);

   result[len] = '\0';
   return result;
}

char *str_swap_case(const char *str)
{
   if (!str)
      return NULL;

   size_t len = strlen(str);
   char *result = malloc(len + 1);
   if (!result)
      return NULL;

   for (size_t i = 0; i < len; i++)
   {
      if (isupper((unsigned char)str[i]))
         result[i] = tolower((unsigned char)str[i]);
      else if (islower((unsigned char)str[i]))
         result[i] = toupper((unsigned char)str[i]);
      else
         result[i] = str[i];
   }

   result[len] = '\0';
   return result;
}

char *str_trim(const char *str)
{
   if (!str)
      return NULL;

   // Find first non-whitespace character
   while (*str && isspace((unsigned char)*str))
      str++;

   if (*str == '\0')
      return strdup("");

   // Find last non-whitespace character
   const char *end = str + strlen(str) - 1;
   while (end > str && isspace((unsigned char)*end))
      end--;

   size_t len = end - str + 1;
   char *result = malloc(len + 1);
   if (!result)
      return NULL;

   memcpy(result, str, len);
   result[len] = '\0';
   return result;
}

char *str_trim_start(const char *str)
{
   if (!str)
      return NULL;

   while (*str && isspace((unsigned char)*str))
      str++;

   return strdup(str);
}

char *str_trim_end(const char *str)
{
   if (!str)
      return NULL;

   if (*str == '\0')
      return strdup("");

   const char *end = str + strlen(str) - 1;
   while (end >= str && isspace((unsigned char)*end))
      end--;

   size_t len = end - str + 1;
   char *result = malloc(len + 1);
   if (!result)
      return NULL;

   memcpy(result, str, len);
   result[len] = '\0';
   return result;
}

/**
 * Reverses a string.
 *
 * @param str The input string
 * @return A new reversed string (caller must free)
 */
char *str_reverse(const char *str)
{
   if (!str)
      return NULL;

   size_t len = strlen(str);
   char *result = malloc(len + 1);
   if (!result)
      return NULL;

   for (size_t i = 0; i < len; i++)
      result[i] = str[len - 1 - i];

   result[len] = '\0';
   return result;
}

char *str_replace_all(const char *str, const char *old_substr, const char *new_substr)
{
   if (!str || !old_substr || !new_substr)
      return NULL;

   size_t old_len = strlen(old_substr);
   if (old_len == 0)
      return strdup(str);

   size_t new_len = strlen(new_substr);

   // Count occurrences
   int count = 0;
   const char *tmp = str;
   while ((tmp = strstr(tmp, old_substr)) != NULL)
   {
      count++;
      tmp += old_len;
   }

   if (count == 0)
      return strdup(str);

   // Allocate result
   size_t result_len = strlen(str) + count * (new_len - old_len);
   char *result = malloc(result_len + 1);
   if (!result)
      return NULL;

   char *dest = result;
   const char *src = str;

   while (*src)
   {
      const char *match = strstr(src, old_substr);
      if (match == src)
      {
         memcpy(dest, new_substr, new_len);
         dest += new_len;
         src += old_len;
      }
      else
      {
         *dest++ = *src++;
      }
   }

   *dest = '\0';
   return result;
}

int str_starts_with(const char *str, const char *prefix)
{
   if (!str || !prefix)
      return 0;

   size_t prefix_len = strlen(prefix);
   size_t str_len = strlen(str);

   if (prefix_len > str_len)
      return 0;

   return strncmp(str, prefix, prefix_len) == 0;
}

int str_ends_with(const char *str, const char *suffix)
{
   if (!str || !suffix)
      return 0;

   size_t suffix_len = strlen(suffix);
   size_t str_len = strlen(str);

   if (suffix_len > str_len)
      return 0;

   return strcmp(str + str_len - suffix_len, suffix) == 0;
}

int str_count_substr(const char *str, const char *substr)
{
   if (!str || !substr)
      return 0;

   size_t substr_len = strlen(substr);
   if (substr_len == 0)
      return 0;

   int count = 0;
   const char *tmp = str;

   while ((tmp = strstr(tmp, substr)) != NULL)
   {
      count++;
      tmp += 1; // Move by 1 to find overlapping matches
   }

   return count;
}

int str_index_of(const char *str, const char *substr)
{
   if (!str || !substr)
      return -1;

   const char *found = strstr(str, substr);
   if (!found)
      return -1;

   return found - str;
}

int str_last_index_of(const char *str, const char *substr)
{
   if (!str || !substr)
      return -1;

   size_t substr_len = strlen(substr);
   if (substr_len == 0)
      return -1;

   const char *last = NULL;
   const char *tmp = str;

   while ((tmp = strstr(tmp, substr)) != NULL)
   {
      last = tmp;
      tmp += substr_len;
   }

   if (!last)
      return -1;

   return last - str;
}

int str_is_alpha(const char *str)
{
   if (!str || *str == '\0')
      return 0;

   while (*str)
   {
      if (!isalpha((unsigned char)*str))
         return 0;
      str++;
   }
   return 1;
}

int str_is_digit(const char *str)
{
   if (!str || *str == '\0')
      return 0;

   while (*str)
   {
      if (!isdigit((unsigned char)*str))
         return 0;
      str++;
   }
   return 1;
}

int str_is_alpha_numeric(const char *str)
{
   if (!str || *str == '\0')
      return 0;

   while (*str)
   {
      if (!isalnum((unsigned char)*str))
         return 0;
      str++;
   }
   return 1;
}

int str_is_lower(const char *str)
{
   if (!str || *str == '\0')
      return 0;

   int has_alpha = 0;
   while (*str)
   {
      if (isalpha((unsigned char)*str))
      {
         has_alpha = 1;
         if (!islower((unsigned char)*str))
            return 0;
      }
      str++;
   }
   return has_alpha;
}

int str_is_upper(const char *str)
{
   if (!str || *str == '\0')
      return 0;

   int has_alpha = 0;
   while (*str)
   {
      if (isalpha((unsigned char)*str))
      {
         has_alpha = 1;
         if (!isupper((unsigned char)*str))
            return 0;
      }
      str++;
   }
   return has_alpha;
}

char *str_pad_left(const char *str, size_t total_length, char pad_char)
{
   if (!str)
      return NULL;

   size_t len = strlen(str);
   if (len >= total_length)
      return strdup(str);

   size_t pad_count = total_length - len;
   char *result = malloc(total_length + 1);
   if (!result)
      return NULL;

   memset(result, pad_char, pad_count);
   memcpy(result + pad_count, str, len);
   result[total_length] = '\0';

   return result;
}

char *str_pad_right(const char *str, size_t total_length, char pad_char)
{
   if (!str)
      return NULL;

   size_t len = strlen(str);
   if (len >= total_length)
      return strdup(str);

   char *result = malloc(total_length + 1);
   if (!result)
      return NULL;

   memcpy(result, str, len);
   memset(result + len, pad_char, total_length - len);
   result[total_length] = '\0';

   return result;
}