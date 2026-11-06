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

#include <u8string.h>
#include <stdlib.h>
#include <string.h>

#define U8_CODEPT_4B 0xF0
#define U8_CODEPT_3B 0xE0
#define U8_CODEPT_2B 0xC0
#define U8_CODEPT_1B 0x7F

static inline size_t
_get_utf8char_len (const unsigned char *ptr)
{
  if      (ptr[0] <= U8_CODEPT_1B)                  return 1;
  else if ((ptr[0] & U8_CODEPT_2B) == U8_CODEPT_2B) return 2;
  else if ((ptr[0] & U8_CODEPT_3B) == U8_CODEPT_3B) return 3;
  else if ((ptr[0] & U8_CODEPT_4B) == U8_CODEPT_4B) return 4;
  
  return 0;
}

U8STR_API bool
u8char_copy (u8char_t dest, const u8char_t src)
{
  if (!dest || !src)
    return false;

  memcpy (dest, src, U8CHAR_LEN);

  return true;
}

U8STR_API bool
u8char_compare (const void *first, const void *second)
{
  if (!first || !second)
    return first == second;

  return !strncmp (first, second, U8CHAR_LEN);
}

U8STR_API size_t
u8str_length (const char *string)
{
  size_t length = 0;
  size_t i;

  for (i = 0; string[i]; ++length)
    i += _get_utf8char_len ((unsigned char*) string + i);

  return length;
}

U8STR_API char*
u8str_next_char (const char *src, u8char_t *buff)
{
  const size_t offset = _get_utf8char_len ((unsigned char*) src);

  memset (buff, 0, U8CHAR_LEN);

  if (offset == 0)
    return NULL;

  memcpy (buff, src, offset);
  
  return (char*) src + offset;
}
