/*************************************************************************

   Program:    wideprint
   File:       wideprint.c
   
   Version:    V0.1
   Date:       27.01.94
   Function:   Filter to create 2 wide pages on a Hewlett Packard LaserJet
   
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
   wideprint takes a file from stdin and outputs to stdout with control 
   codes for creating a 2 up printout on a Hewlett Packard LaserJet.

   Designed for use as an lpr filter.

   Some code based on lj2up

**************************************************************************

   Usage:
   ======

**************************************************************************

   Revision History:
   =================
   V1.0  27.01.94 Original    By: ACRM

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
#define LF  10
#define FF  12
#define RET 13
#define ESC 27
#define SO  14

#define WIDTH     129   /* Max width of page                            */
#define HEIGHT    128   /* Max height of page                           */
#define LINEPAGE   62   /* Lines to display per page                    */

/************************************************************************/
/* Prototypes
*/
int   main(int argc, char **argv);
void  DoFilter(FILE *in, FILE *out);
void  SetWide(FILE *out);
void  ResetPrinter(FILE *out);

/************************************************************************/
/*>int main(int argc, char **argv)
   -------------------------------
   Main program

   25.01.94 Original    By: ACRM
*/
int main(int argc, char **argv)
{
   FILE  *in  = stdin,
         *out = stdout;

   SetWide(out);
   DoFilter(in, out);
   ResetPrinter(out);
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
         page           = 0,
         char_pos       = 0,
         done           = 0,
         NLine          = 0,
         ch,
         pos_in_tab,
         count,
         i,
         j;
       
   /* Now copy characters from in to out and expand tabs */
   while(ch = getc(in),!feof(in))
   {
      switch(ch)
      {
      case LF:
         fputc(LF,out);
         NLine++;
         
         if(NLine==LINEPAGE)     /* Split the page                      */
         {
            fputc(LF,out);
            for(i=0; i<WIDTH; i++)
               fputc('-',out);
            fputc(LF,out);
            fputc(LF,out);
         }
         else if(NLine==2*LINEPAGE)
         {
            NLine = 0;
            fputc(FF,out);
         }
         break;
         
      case RET:
         break;
         
      case FF:
         page++;
         if(page==2)
         {
            fputc(FF,out);
            page  = 0;
            NLine = 0;
         }
         else
         {
            for(;NLine < LINEPAGE ;NLine++)
               fputc(LF,out);
            fputc(LF,out);
            for(i=0; i<WIDTH; i++)
               fputc('_',out);
            fputc(LF,out);
            fputc(LF,out);
         }
         break;

/*    NOT DONE YET...

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
*/
                       
      default:
         fputc(ch,out);
      }  /* End of character switch                                     */
   }
}

/************************************************************************/
/*>void SetWide(FILE *out)
   -----------------------
   Set wide mode on the LaserJet

   25.01.94 Original    By: ACRM
*/
void SetWide(FILE *out)
{
   char buffer[80];
   int  i;

   /* Set line spacing
      ESC &l13D is 13 lines per inch
      Commands of the same class (i.e. &l) may be combined by
      downcasing all but the last character
   */
   sprintf(buffer,"%c%s",ESC,"&l12.2D\0");
   for(i=0;buffer[i];i++) 
      putc(buffer[i],out);

   /* Set left & right margins
      ESC &a0L   Sets left margin to 0
      ESC &a132M Sets right margin to WIDTH
   */
   sprintf(buffer,"%c%s%d%s",ESC,"&a0l",WIDTH,"M\0");
   for(i=0;buffer[i];i++) 
      putc(buffer[i],out);

   /* Set top and bottom margin
      ESC &l0E   Sets top margin to 0
      ESV &l132F Sets text length to HEIGHT
   sprintf(buffer,"%c%s%d%s",ESC,"&l0e",HEIGHT,"F\0");
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

