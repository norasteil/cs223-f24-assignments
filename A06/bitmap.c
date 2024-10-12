/*----------------------------------------------
 * Author: Nora Steil
 * Date: 10/7/24
 * Description: Read a single 64-bit unsigned integer, output it as an 8x8 1bpp sprite
 ---------------------------------------------*/
#include <stdio.h>

int main()
{
  unsigned long img;
  scanf(" %lx", &img);

  // todo: your code here
  unsigned long mask = 0x1ul << 63;

  for (int i = 0; i < 64; i++)
  {
    if (img & mask)
    {
      printf("@ ");
    }
    else
    {
      printf("  ");
    }

    mask >>= 1;

    if ((i + 1) % 8 == 0)
    {
      printf("\n");
    }
  }
  printf("\n");

  return 0;
}
