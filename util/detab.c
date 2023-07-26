/***************************************************************
      Andrew C.R. Martin                            05.10.88

      Laboratory of Molecular Biophysics            V1.1
      South Parks Road
      Oxford
      OX1 3QU

      This code is in the public domain. It may be copied and/or
      modified by anyone providing this notice is retained and 
      that the code is not sold for profit. Programs resulting
      from the use of this code may be sold commercially, providing
      suitable acknowledgements are included and that notice is
      given that this portion of the code is used with permission
      of the author.

****************************************************************

      detab.c

      Removes tabs from a text file and replaces with the correct
      number of spaces.

      Syntax:   detab [-n6] <input file> <output file>
      Where -n sets the number of spaces in the tab (default = 8)

      V1.1 13.10.89 Fixed a small bug in calculating value of
      NTab from a CLI switch. Only affected the outcome if you
      were expanding a tab to >9 spaces.
****************************************************************/

#include <stdio.h>
#define Tab 9        /* ASCII code for <CTRL>I (Tab) */
#define EOL 10       /* ASCII code for <linefeed>    */
#define RET 13       /* ASCII code for <return>      */

main(argc,argv)
int argc;
char **argv;
{
   int ch,i,char_pos=1, /* Was 0 in V1.0  */
       pos_in_tab,count;
   int NTab,ai,j;
   char work[10];
   FILE *in,*out;

   /* Give a message if the number of arguments is wrong */
   if(argc > 5 || argc < 3) usage();

   if(argc == 3) /* Accepting the default value for NTab */
   {
      NTab = 8;
   }
   else
   {
      copy(work,argv[1]);
      /* If the option -n is present */
      if((work[0] == '-') && ((work[1] == 'n') || (work[1] == 'N')))
      {
         if (argc == 4)    /* Parameter follows immediately e.g. -n6 */
         {
            NTab = 0;
            ai   = 2;
            for(j=0; j<2; j++) /* Assume a maximum of 3 digits */
            {
               if(work[ai+j] >= '0' && work[ai+j] <= '9')
               {
                  NTab *= 10;
                  NTab += work[ai+j] - '0';
               }
               else
               {
                  break;
               }
            }
         }
         else     /* Must be (argc == 5) i.e. Gap before parameter e.g. -n 6 */
         {
            copy(work,argv[2]);    /* Get the number into the work array */
            NTab=0;
            for(j=0; j<2; j++)
            {
               if(work[j] >= '0' && work[j] <= '9')
               {
                  NTab *= 10;
                  NTab += work[j] - '0';
               }
               else
               {
                  break;
               }
            }
         }
      }
      else
      {
         printf("Unknown option %s\n",work);
         usage();
      }
   }

   /* We've settled on a value for NTab so
      Open the files for input & output */

   if(!(in=fopen(argv[argc-2],"r")))
   {
      printf("Input file not found.\n");
      exit(1);
   }

   if(!(out=fopen(argv[argc-1],"w")))
   {
      printf("Unable to open output file.\n");
      exit(1);
   }

   /* Read each character */

   while (ch = getc(in),ch != EOF)
   {
      if(ch==EOL || ch==RET) char_pos = 0;

      if(ch==Tab)
      {
         pos_in_tab = char_pos - NTab * (int)(char_pos/NTab);
         count = 1 + NTab - pos_in_tab;
         if (count > NTab) count -= NTab;
         for(i=0; i < count; i++)
         {
            putc(' ',out);
            char_pos++;
         }
      }
      else
      {
         putc(ch,out);
         char_pos++;
      }
   }
}

usage()
{
   printf("Usage: detab [-n6] <input file> <output file>\n");
   exit(1);
}

copy(to, from)
char *to, *from;
{
   while (to && *from) *to++ = *from++;
   *to++ = '\0';
}
