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

#include "../include/u8string.h"
#include <stdlib.h>
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
u8str_get_utf8char_len (const char *chptr)
{
  size_t i = 0;
  
  for (i = 0; i < 3; ++i)
    if ((cpoint_len[i][0] & *chptr) == cpoint_len[i][0])
      return (size_t) cpoint_len[i][1];  

  return 1;
}

static size_t
u8str_get_cstrlen (const u8string_t *string)
{
  size_t i = 0;
  size_t len = 0;
  
  for (i = 0; i < string->length; ++i)
    len += strlen(string->string[i]);

  return len;
}

static void
u8str_copy_cstring_to_u8string (u8string_t *str, const cstr cstring)
{
  size_t i = 0;
  size_t j = 0;
  size_t k = 0;
  size_t len = strlen (cstring);

  memset (str->string, 0, sizeof (u8char_t) * str->length);
  
  for (i = 0; i < len; i += k)
    {
      k = u8str_get_utf8char_len (cstring + i);
      memcpy (str->string[j++], cstring + i, k);
    }
}

size_t
u8str_strlen (const cstr string)
{
  size_t length = 0;
  size_t i = 0;

  for (; string[i]; ++length)
    i += u8str_get_utf8char_len (string + i);

  return length;
}

u8string_t
u8string (const cstr string)
{
  u8string_t ret; 

  ret.length = u8str_strlen (string);
  ret.string = calloc (ret.length, sizeof (u8char_t));

  u8str_copy_cstring_to_u8string (&ret, string);

  return ret;
}

bool
u8string_free (u8string_t *str)
{
  if (!str)
    return false;
  
  free (str->string);
  str->length = 0;

  return true;
}

char *
u8string_to_cstr (u8string_t *string)
{
  char *ptr = NULL;
  char *ret = NULL;
  size_t i = 0;
  size_t u8charlen = 0;
  
  if (!string)
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
u8string_set (u8string_t *string, const cstr newstr)
{
  size_t newsize = u8str_strlen (newstr);

  if (!string)
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

bool
u8char_copy (u8char_t dest, const u8char_t src)
{
  if (!dest || !src)
    return false;

  memcpy (dest, src, U8CHAR_LEN);

  return true;
}

bool
u8char_compare (const void *first, const void *second)
{
  if (!first || !second)
    return first == second;

  return !strcmp (first, second);
}
