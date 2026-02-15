#ifndef STR_H
#define STR_H

#include "type.h"
#include <string.h>
char *str_concat(const char *s1, const char *s2);

char *str_replicate(const char *str, int times);

int str_is_substr(const char *subStr, const char *str);

int str_lexicographical_compare(const char *str1, const char *str2);

char *str_slice(const char *str, int start, int end, int step);

char *str_char_at(const char *s, int index);

/**
 * Converts a string to uppercase.
 *
 * @param str The input string
 * @return A new uppercase string (caller must free)
 */
char *str_to_uppercase(const char *str);

/**
 * Converts a string to lowercase.
 *
 * @param str The input string
 * @return A new lowercase string (caller must free)
 */
char *str_to_lowercase(const char *str);

/**
 * Converts a string to title case (first letter of each word capitalized).
 *
 * @param str The input string
 * @return A new title-cased string (caller must free)
 */
char *str_title_case(const char *str);
/**
 * Capitalizes only the first character of the string.
 *
 * @param str The input string
 * @return A new string with first character capitalized (caller must free)
 */
char *str_capitalize(const char *str);
/**
 * Swaps the case of each character (upper to lower, lower to upper).
 *
 * @param str The input string
 * @return A new string with swapped case (caller must free)
 */
char *str_swap_case(const char *str);
/**
 * Removes leading and trailing whitespace from a string.
 *
 * @param str The input string
 * @return A new trimmed string (caller must free)
 */
char *str_trim(const char *str);

/**
 * Removes leading whitespace from a string.
 *
 * @param str The input string
 * @return A new string with leading whitespace removed (caller must free)
 */
char *str_trim_start(const char *str);

/**
 * Removes trailing whitespace from a string.
 *
 * @param str The input string
 * @return A new string with trailing whitespace removed (caller must free)
 */
char *str_trim_end(const char *str);
/**
 * Reverses a string.
 *
 * @param str The input string
 * @return A new reversed string (caller must free)
 */
char *str_reverse(const char *str);
/**
 * Replaces all occurrences of a substring with another substring.
 *
 * @param str The input string
 * @param old_substr The substring to replace
 * @param new_substr The replacement substring
 * @return A new string with replacements (caller must free)
 */
char *str_replace_all(const char *str, const char *old_substr, const char *new_substr);

/**
 * Checks if a string starts with a given prefix.
 *
 * @param str The string to check
 * @param prefix The prefix to look for
 * @return 1 if starts with prefix, 0 otherwise
 */
int str_starts_with(const char *str, const char *prefix);
/**
 * Checks if a string ends with a given suffix.
 *
 * @param str The string to check
 * @param suffix The suffix to look for
 * @return 1 if ends with suffix, 0 otherwise
 */
int str_ends_with(const char *str, const char *suffix);
/**
 * Counts the number of occurrences of a substring.
 *
 * @param str The string to search in
 * @param substr The substring to count
 * @return Number of occurrences
 */
int str_count_substr(const char *str, const char *substr);
/**
 * Finds the index of the first occurrence of a substring.
 *
 * @param str The string to search in
 * @param substr The substring to find
 * @return Index of first occurrence, or -1 if not found
 */
int str_index_of(const char *str, const char *substr);
/**
 * Finds the index of the last occurrence of a substring.
 *
 * @param str The string to search in
 * @param substr The substring to find
 * @return Index of last occurrence, or -1 if not found
 */
int str_last_index_of(const char *str, const char *substr);
/**
 * Checks if a string contains only alphabetic characters.
 *
 * @param str The string to check
 * @return 1 if all alphabetic, 0 otherwise
 */
int str_is_alpha(const char *str);

/**
 * Checks if a string contains only digits.
 *
 * @param str The string to check
 * @return 1 if all digits, 0 otherwise
 */
int str_is_digit(const char *str);
/**
 * Checks if a string contains only alphanumeric characters.
 *
 * @param str The string to check
 * @return 1 if all alphanumeric, 0 otherwise
 */
int str_is_alpha_numeric(const char *str);

/**
 * Checks if all characters in the string are lowercase.
 *
 * @param str The string to check
 * @return 1 if all lowercase, 0 otherwise
 */
int str_is_lower(const char *str);
/**
 * Checks if all characters in the string are uppercase.
 *
 * @param str The string to check
 * @return 1 if all uppercase, 0 otherwise
 */
int str_is_upper(const char *str);
/**
 * Pads a string on the left with a character to reach a specified length.
 *
 * @param str The input string
 * @param total_length The desired total length
 * @param pad_char The character to pad with
 * @return A new padded string (caller must free)
 */
char *str_pad_left(const char *str, size_t total_length, char pad_char);

/**
 * Pads a string on the right with a character to reach a specified length.
 *
 * @param str The input string
 * @param total_length The desired total length
 * @param pad_char The character to pad with
 * @return A new padded string (caller must free)
 */
char *str_pad_right(const char *str, size_t total_length, char pad_char);

#endif // STR_H