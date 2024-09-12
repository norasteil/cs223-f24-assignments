/***************************************************
 * acronym.c
 * Author: Nora Steil
 * Creates an acronym from user input
 */

#include <stdio.h>
#include <string.h>

int main()
{
  char phrase[1024];
  char acronym[1024] = "";

  printf("Enter a phrase: ");
  scanf("%[^\n]%*c", phrase);

  for (int i = 0; i < strlen(phrase); i++)
  {
    if (phrase[i] >= 'A' && phrase[i] <= 'Z')
    {
      if (phrase[i - 1] == ' ' || i == 0)
      {
        char temp[2] = {phrase[i], '\0'};
        strcat(acronym, temp);
      }
    }
  }

  printf("%s\n", acronym);

  return 0;
}
