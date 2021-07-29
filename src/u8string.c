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

#include "u8string.h"
#include <string.h>

#define UTF8_CODEPOINT_4B 0xF0
#define UTF8_CODEPOINT_3B 0xE0
#define UTF8_CODEPOINT_2B 0xC0

static const int cpoint_len[3][2] =
  {
    { UTF8_CODEPOINT_2B, 2 },
    { UTF8_CODEPOINT_3B, 3 },
    { UTF8_CODEPOINT_4B, 4 }
  };

static size_t
u8str_get_utf8char_len (char *chptr)
{
  size_t i = 0;
  
  for (i = 0; i < 3; ++i)
    if ((cpoint_len[i][0] & *chptr) == cpoint_len[i][0])
      return (size_t) cpoint_len[i][1];  
    else if (i == 2)
      return 1;

  return 1;
}

static size_t
u8str_get_cstrlen (u8string_t *string)
{
  size_t i = 0;
  size_t len = 0;
  
  for (i = 0; i < string->length; ++i)
    len += strlen(string->string[i]);

  return len;
}

static void
u8str_copy_cstring_to_u8string (u8string_t *str, char *cstr)
{
  size_t i = 0;
  size_t j = 0;
  size_t k = 0;
  size_t len = strlen (cstr);

  memset (str->string, 0, sizeof (u8char_t) * str->length);
  
  for (i = 0; i < len; i += k)
    {
      k = u8str_get_utf8char_len (cstr + i);
      memcpy (str->string[j++], cstr + i, k);
    }
}

size_t
u8str_strlen (char *string)
{
  size_t length = 0;
  size_t i = 0;

  for (; string[i]; ++length)
    i += u8str_get_utf8char_len (string + i);

  return length;
}

u8string_t *
u8string (char *string)
{
  u8string_t *ret = calloc (1, sizeof (u8string_t));

  ret->length += u8str_strlen (string);
  ret->string = malloc (ret->length * sizeof (u8char_t));

  u8str_copy_cstring_to_u8string (ret, string);

  return ret;
}

int
u8string_free (u8string_t *str)
{
  if (str == NULL)
    return EXIT_FAILURE;
  
  free (str->string);
  free (str);

  return EXIT_SUCCESS;
}

char *
u8string_to_cstr (u8string_t *string)
{
  char *ptr = NULL;
  char *ret = NULL;
  size_t i = 0;
  size_t u8charlen = 0;
  
  if (string == NULL)
    return NULL;

  ret = ptr = calloc (u8str_get_cstrlen (string) + 1, sizeof (char));
  
  for (i = 0; i < string->length; ++i)
    {
      u8charlen = strlen (string->string[i]);
      memcpy (ptr, string->string[i], u8charlen);
      ptr += u8charlen;
    }

  return ret;
}

u8string_t *
u8string_set (u8string_t *string, char *newstr)
{
  size_t newsize = u8str_strlen (newstr);

  if (string == NULL)
    return NULL;
  
  if (newsize <= string->length)
    {
    if (newsize <= string->length / 2)
      {
        string->string = realloc(string->string, (newsize + 1) *
                                 sizeof (u8char_t));

        string->length = newsize;
        u8str_copy_cstring_to_u8string (string, newstr);
      }
    else
      u8str_copy_cstring_to_u8string (string, newstr);
    }
  else
    {
      string->string = realloc (string->string, (newsize + 1) *
                                sizeof (u8char_t));

      string->length = newsize;
      u8str_copy_cstring_to_u8string (string, newstr);
    }
  return string;
}
