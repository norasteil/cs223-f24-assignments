/*----------------------------------------------
 * Author: Nora Steil
 * Date: 9/16/24
 * Description: Take in a string from user input and add a certain number of pauses between each character
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  int pause;
  char text[32];
  char *output;

  printf("Pause length: ");
  scanf("%d", &pause);

  printf("Text: ");
  scanf("%s", text);

  output = malloc(sizeof(char) * (strlen(text) * pause + 1) + 1);

  int idx = 0;
  for (int i = 0; i < strlen(text); i++)
  {
    output[idx++] = text[i];
    for (int j = 0; j < pause; j++)
    {
      output[idx++] = '.';
    }
  }
  output[idx] = '\0';

  printf("%s\n", output);

  free(output);
  return 0;
}
