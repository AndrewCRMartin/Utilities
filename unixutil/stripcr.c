/* A jiffy to change CR/LF or CR to LF
   12.04.94
*/

#include <stdio.h>
#include <stdlib.h>

#ifndef TRUE
#  define TRUE  1
#  define FALSE 0
#endif

int main(int argc, char **argv)
{
   FILE *in,
        *out;
   int  ch,
        GotLF = FALSE;
   
   if(argc < 3)
   {
      printf("Usage: StripCR <infile> <outfile>\n");
      exit(0);
   }
   
   if((in=fopen(argv[1],"r"))==NULL)
   {
      printf("Unable to open input file %s\n",argv[1]);
      exit(1);
   }

   if((out=fopen(argv[2],"w"))==NULL)
   {
      printf("Unable to open output file %s\n",argv[2]);
      exit(1);
   }

   ch = getc(in);
   
   while(ch!=EOF && !feof(in))
   {
      switch(ch)
      {
      case 10:
      case 13:
         if(!GotLF)
         {
            putc((char)10, out);
            GotLF = TRUE;
         }
         break;
      default:
         putc((char)ch, out);
         GotLF = FALSE;
         break;
      }
      
      ch = getc(in);
   }
   
   return(0);
}
