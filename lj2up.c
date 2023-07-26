/*************************************************************************

   Program:    lj2up
   File:       lj2up.c
   
   Version:    V1.1
   Date:       23.05.95
   Function:   Filter to create 2 up on a Hewlett Packard LaserJet
   
   Copyright:  (c) SciTech Software 1994
   Author:     Dr. Andrew C. R. Martin
   Address:    SciTech Software
               23, Stag Leys,
               Ashtead,
               Surrey,
               KT21 2TD.
   Phone:      +44 (0372) 275775
   EMail:      UUCP:  cbmehq!cbmuk!scitec!amartin
                      amartin@scitec.adsp.sub.org
               JANET: andrew@uk.ac.ox.biop
               
**************************************************************************

   This program is not in the public domain, but it may be freely copied
   and distributed for no charge providing this header is included.
   The code may be modified as required, but any modifications must be
   documented so that the person responsible can be identified. If someone
   else breaks this code, I don't want to be blamed for code that does not
   work! The code may not be sold commercially without prior permission 
   from the author, although it may be given away free with commercial 
   products, providing it is made clear that this program is free and that 
   the source code is provided with the program.

**************************************************************************

   Description:
   ============
   lj2up takes a file from stdin and outputs to stdout with control codes
   for creating a 2 up printout on a Hewlett Packard LaserJet.

   Designed for use as an lpr filter.

   The code for expanding tabs and creating a double page in an array
   is modified from the print program for the Amiga.

**************************************************************************

   Usage:
   ======

**************************************************************************

   Revision History:
   =================
   V1.0  25.01.94 Original    By: ACRM
   V1.1  23.05.95 Fixed format to move right hand column over 3 spaces

*************************************************************************/
/* Includes
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/************************************************************************/
/* Defines
*/
#define Tab 9
#define EOL 10
#define FF  12
#define RET 13
#define ESC 27
#define SO  14

#define MAXPAGE  2      /* Number of pages on a sheet                   */
#define MAXLINES 70     /* Max lines to a page                          */
#define MAXCHAR  90     /* Max chars per line                           */
#define COLPAGE  86     /* Columns to display per page                  */
#define LINEPAGE 58     /* Lines to display per page                    */
#define MARGIN   5      /* The left hand margin                         */

/************************************************************************/
/* Globals
*/
char gLine[MAXPAGE][MAXLINES][MAXCHAR];   /* Used to store the text     */

/************************************************************************/
/* Prototypes
*/
void  Initialise(void);
int   main(int argc, char **argv);
void  DoFilter(FILE *in, FILE *out);
void  SetLandscape(FILE *out);
void  ResetPrinter(FILE *out);
void  PrintPage(FILE *out, int NumLines, int form);

/************************************************************************/
/*>void Initialise(void)
   ---------------------
   Perform variable initialisation, etc

   25.01.94 Original    By: ACRM
*/
void Initialise(void)
{
   int j;

   memset(gLine,'\0',MAXPAGE*MAXLINES*MAXCHAR);

   for(j=0; j<MARGIN; j++)
      gLine[0][0][j] = ' ';
}

/************************************************************************/
/*>int main(int argc, char **argv)
   -------------------------------
   Main program

   25.01.94 Original    By: ACRM
   23.05.95 Added return.
*/
int main(int argc, char **argv)
{
   FILE  *in  = stdin,
         *out = stdout;

   Initialise();
   SetLandscape(out);
   DoFilter(in, out);
   PrintPage(out,LINEPAGE,0);
   ResetPrinter(out);

   return(0);
}

/************************************************************************/
/*>void DoFilter(FILE *in, FILE *out)
   ----------------------------------
   Do the actual filtering. Expand tabs, etc.

   25.01.94 Original    By: ACRM
*/
void DoFilter(FILE *in, FILE *out)
{
   int   NTab           = 8,
         right_margin   = COLPAGE-MARGIN-1,
         page           = 0,
         char_pos       = 0,
         done           = 0,
         NLine          = 0,
         ch,
         pos_in_tab,
         count,
         j;
       
   /* Now copy characters from in to out and expand tabs */
   while(ch = getc(in),!feof(in))
   {
      switch(ch)
      {
      case EOL:
         gLine[page][NLine][char_pos + MARGIN] = '\0';
         ch = getc(in);
         if(ch != RET) ungetc(ch,in);
         char_pos = 0; done = 0; NLine++;
         for(j=0; j<MARGIN; j++)
            gLine[page][NLine][j] = ' ';
         break;
      
      case RET:
         char_pos = 0; 
         done = 0;
         break;
         
      case FF:
         page++;  NLine = 0;
         if(page==2)
         {
            PrintPage(out,LINEPAGE,0);
            page=0;
         }
         for(j=0; j<MARGIN; j++)
            gLine[page][NLine][j] = ' ';
         char_pos = 0; done=0; 
         break;
   
      case Tab:
         pos_in_tab = char_pos - (NTab * (int)(char_pos / NTab));
         count = 1 + NTab - pos_in_tab;
         if(count > NTab) count -= NTab;
         for(j=0; j<count; j++)
         {
            if(!done && (char_pos > right_margin))
            {
               gLine[page][NLine][char_pos + MARGIN] = '\0';
               done = 1; NLine++;
               for(j=0; j<MARGIN; j++)
                  gLine[page][NLine][j] = ' ';
            }
            gLine[page][NLine][char_pos + MARGIN] = ' ';
            char_pos++;
         }
         break;
                       
      default:
         if(!done && (char_pos > right_margin))
         {
            gLine[page][NLine][char_pos + MARGIN] = '\0';
            NLine++; done=1;
            for(j=0; j<MARGIN; j++)
               gLine[page][NLine][j] = ' ';
            char_pos = 0;
         }
         gLine[page][NLine][char_pos + MARGIN] = ch;
         char_pos++;
      }  /* End of character switch                                     */

      /* Handle new page                                                */
      if(NLine >= LINEPAGE)
      {
         NLine -= LINEPAGE;
         page++;
         
         switch(page)
         {
         case 1:  
            for(j=0; j<MARGIN; j++)
               gLine[1][0][j] = ' ';
            break;
         case 2:  
            page=0;
            PrintPage(out,LINEPAGE,0);
         }
      }
   }
}

/************************************************************************/
/*>void PrintPage(FILE *out, int NumLines, int form)
   -------------------------------------------------
   Print out a page to the output file

   25.01.94 Original    By: ACRM
   23.05.95 Added 3 spaces before the | to move the right page over
*/
void PrintPage(FILE *out, int NumLines, int form)
{
   int        j;
   char       format[80];
         
   sprintf(format,"%%-%2ds    | %%-%2ds\n",COLPAGE,COLPAGE);
   
   for(j=0; j<NumLines; j++)
      fprintf(out,format,gLine[0][j],gLine[1][j]);

   if(form) 
      fprintf(out,"%c",FF);

   memset(gLine,'\0',MAXPAGE*MAXLINES*MAXCHAR);
   for(j=0; j<MARGIN; j++)
   {
      gLine[0][0][j] = ' ';
      gLine[1][0][j] = ' ';
   }
}

/************************************************************************/
/*>void SetLandscape(FILE *out)
   ----------------------------
   Set landscape mode on the LaserJet

   25.01.94 Original    By: ACRM
*/
void SetLandscape(FILE *out)
{
   char buffer[80];
   int  i;

   /* Set landscape and line spacing
      ESC &l1O is landscape
      ESC &l8D is 8 lines per inch
      Commands of the same class (i.e. &l) may be combined by
      downcasing all but the last character
   */
   sprintf(buffer,"%c%s",ESC,"&l1o8D\0");
   for(i=0;buffer[i];i++) 
      putc(buffer[i],out);

   /* Set left & right margins
      ESC &a2L   Sets left margin to 2
      ESC &a170M Sets right margin to 170
   */
   sprintf(buffer,"%c%s",ESC,"&a2L\0");
   for(i=0;buffer[i];i++) 
      putc(buffer[i],out);
   sprintf(buffer,"%c%s",ESC,"&a170M\0");
   for(i=0;buffer[i];i++) 
      putc(buffer[i],out);

   /* Set secondary font to LinePrinter and select
      ESC )s16.66H   Sets 16.66 CPI
      ESC )s8.5V     Sets 8.5pt
      ESC )s0S       Sets upright
      ESC )s0B       Sets medium stroke weight
      ESC )s0T       Sets line printer font
      SO             Selects secondary font
   */
   sprintf(buffer,"%c%s",ESC,")s16.66h8.5v0s0b0T\0");
   for(i=0;buffer[i];i++) 
      putc(buffer[i],out);
   sprintf(buffer,"%c%s",SO,"\0");
   for(i=0;buffer[i];i++) 
      putc(buffer[i],out);
}

/************************************************************************/
/*>void ResetPrinter(FILE *out)
   ----------------------------
   Reset the printer

   25.01.94 Original    By: ACRM
*/
void ResetPrinter(FILE *out)
{
   char buffer[10];
   int  i;
  
   /* Reset printer                                                     */
   sprintf(buffer,"%c%s",ESC,"E\0");
   for(i=0;buffer[i];i++) 
      putc(buffer[i],out);
}

