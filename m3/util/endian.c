#include <stdio.h>

void main(void)
{
   if(((unsigned short)(0xF0) >> 8 ) == 0x0F)
      printf("Little Endian\n");
   else if(((unsigned short)(0xF0) >> 8 ) == 0x00)
      printf("Big Endian\n");
   else
      printf("Can't determine endedness\n");
}
