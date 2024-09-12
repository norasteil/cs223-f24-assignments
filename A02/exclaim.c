/***************************************************
 * exclaim.c
 * Author: Nora Steil
 * Changes a word into an exclamation
 */

#include <stdio.h>
#include <stdlib.h>

int main()
{
  char buff[32];
  char symbols[4] = {'@', '!', '#', '*'};

  printf("Enter a word: ");
  scanf("%s", buff);

  for (int i = 0; i < sizeof(buff); i++)
  {
    if (buff[i] <= 'z' && buff[i] >= 'a')
    {
      int num = rand() % 4;
      buff[i] = symbols[num];
    }
  }

  printf("%s\n", buff);

  return 0;
}
