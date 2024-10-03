/*----------------------------------------------
 * Author: Nora Steil
 * Date: 10/2/24
 * Description: Generate a crossword from user input
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    printf("Usage: %s <word1> <word2>\n", argv[0]);
    exit(0);
  }

  char word1[64];
  char word2[64];

  strcpy(word1, argv[1]);
  strcpy(word2, argv[2]);

  char **grid = (char **)malloc(sizeof(char *) * strlen(word1));

  for (int i = 0; i < strlen(word1); i++)
  {
    grid[i] = (char *)malloc(strlen(word2) * sizeof(char));
    for (int j = 0; j < strlen(word2); j++)
    {
      grid[i][j] = '.';
    }
  }

  // find common letter or print no common letter
  char common = '\0';
  int commonIdx1 = -1;
  int commonIdx2 = -1;
  for (int i = 0; i < strlen(word1); i++)
  {
    for (int j = 0; j < strlen(word2); j++)
    {
      if (word1[i] == word2[j])
      {
        common = word1[i];
        commonIdx1 = i;
        commonIdx2 = j;
        break;
      }
    }
  }
  if (common == '\0' || commonIdx1 == -1)
  {
    printf("No common letter\n");
  }
  else
  {
    for (int i = 0; i < strlen(word1); i++)
    {
      for (int j = 0; j < strlen(word2); j++)
      {
        if (i == commonIdx1 && j == commonIdx2) // common letter
        {
          grid[i][j] = word1[i];
        }
        else if (i == commonIdx1) // print word 2 vertically
        {
          grid[i][j] = word2[j];
        }
        else if (j == commonIdx2) // print word 1 horizontally
        {
          grid[i][j] = word1[i];
        }
      }
    }

    // print grid
    for (int i = 0; i < strlen(word1); i++)
    {
      for (int j = 0; j < strlen(word2); j++)
      {
        printf("%c ", grid[i][j]);
      }
      printf("\n");
    }
  }

  // free grid
  for (int i = 0; i < strlen(word1); i++)
  {
    free(grid[i]);
  }
  free(grid);

  return 0;
}
