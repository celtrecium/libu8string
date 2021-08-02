/*
 * This file is part of libu8string.
 *
 * libu8string is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * libu8string is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libu8string. If not, see
 * <https://www.gnu.org/licenses/>.
 */

#ifndef U8STRING_H
#define U8STRING_H

#include <stdlib.h>
#include <stdbool.h>

#if defined(_WIN32) || defined(__CYGWIN__)
# if defined(_MSC_VER)
#  define U8STR_API __declspec(dllexport)
# elif (defined(__GNUC__) && (__GNUC__ >= 4))
#  define U8STR_API __attribute__((dllexport))
# else
#  define U8STR_API
# endif /* _MSC_VER */
#elif defined(__unix__)
# if (defined(__GNUC__) && (__GNUC__ >= 4))
#  define U8STR_API __attribute__((visibility("default")))
# else
#  define U8STR_API
# endif
#else
# define U8STR_API
#endif /* _WIN32 || __CYGWIN__ */

#define U8CHAR_LEN 5

typedef char u8char_t[U8CHAR_LEN];

typedef struct u8string
{
  u8char_t *string;
  size_t length;
} u8string_t;

U8STR_API size_t u8str_strlen (char *string);
U8STR_API u8string_t *u8string (char *string);
U8STR_API int u8string_free (u8string_t *str);
U8STR_API u8string_t *u8string_set (u8string_t *string, char *newstr);
U8STR_API char *u8string_to_cstr (u8string_t *string);
U8STR_API int u8char_copy (u8char_t dest, u8char_t src);
U8STR_API bool u8char_compare (void *first, void *second);

#endif  /* U8STRING_H */
