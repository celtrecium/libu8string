#include <stdio.h>
#include <string.h>
#include <u8string.h>

void
test_u8char_compare (const u8char_t a, const u8char_t b)
{
  if (u8char_compare (a, b))
    printf ("UTF8 characters '%.*s' and '%.*s' are equal\n",
            U8CHAR_LEN, a,
            U8CHAR_LEN, b);
  else
    printf ("UTF8 characters '%.*s' and '%.*s' aren't equal\n",
            U8CHAR_LEN, a,
            U8CHAR_LEN, b);
}

int
main (void)
{
  const char  *str    = "Привет, Мир!";
  const size_t u8slen = u8str_length (str);

  char *ptr = (char*) str;
  size_t i;
  u8char_t chbuff, a, b;

  printf ("UTF8 string length: %zu\n", u8slen);
  puts ("-----------------------");

  for (i = 0; i < u8slen; ++i)
    {
      ptr = u8str_next_char (ptr, &chbuff);
      printf ("[%zu] = \t'%.*s'\n", i, U8CHAR_LEN, chbuff);
    }

  puts ("-----------------------");
  
  u8str_next_char ("Б", &a);
  u8str_next_char ("Ж", &b);

  test_u8char_compare (a, b);

  u8char_copy (b, a);
  
  test_u8char_compare (a, b);

  return 0;
}
