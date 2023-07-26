/*************************************************************************

   Program:    CountChar
   File:       countchar.c
   
   Version:    V1.0
   Date:       14.12.94
   Function:   Count occurrence of one (or more) characters in a file
   
   Copyright:  (c) Dr. Andrew C. R. Martin 1994
   Author:     Dr. Andrew C. R. Martin
   Address:    Biomolecular Structure & Modelling Unit,
               Department of Biochemistry & Molecular Biology,
               University College,
               Gower Street,
               London.
               WC1E 6BT.
   Phone:      (Home) +44 (0)1372 275775
               (Work) +44 (0)171 387 7050 X 3284
   EMail:      INTERNET: martin@bsm.bioc.ucl.ac.uk
               
**************************************************************************

   This program is not in the public domain, but it may be copied
   according to the conditions laid out in the accompanying file
   COPYING.DOC

   The code may be modified as required, but any modifications must be
   documented so that the person responsible can be identified. If someone
   else breaks this code, I don't want to be blamed for code that does not
   work! 

   The code may not be sold commercially or included as part of a 
   commercial product except as described in the file COPYING.DOC.

**************************************************************************

   Description:
   ============

**************************************************************************

   Usage:
   ======

**************************************************************************

   Revision History:
   =================

*************************************************************************/
/* Includes
*/
#include <stdio.h>
#include <string.h>

/************************************************************************/
/* Defines and macros
*/

/************************************************************************/
/* Globals
*/

/************************************************************************/
/* Prototypes
*/

/************************************************************************/
/*>int main(int argc, char **argv)
   -------------------------------
   Main program for character counting. So simple that it does all the
   work.

   14.12.94 Original    By: ACRM
*/
int main(int argc, char **argv)
{
   FILE *in = stdin;
   int  ch, count = 0;

   /* Check the command line and report any error                       */
   if(argc < 2 || argc > 3)
   {
      fprintf(stderr,"CountChar V1.0 (c) 1994 Dr. Andrew C.R. Martin, \
UCL\n\n");
      
      fprintf(stderr,"Usage: countchar chars [filename]\n\n");
      
      fprintf(stderr,"Counts the occurrence of one or more characters \
(specified by chars) in\n");
      fprintf(stderr,"a file. If the filename is not specified, stdin is \
used.\n\n");

      return(1);
   }
   
   /* Open specified input file if we're not reading from stdin         */
   if(argc == 3)
   {
      if((in=fopen(argv[2],"r"))==NULL)
      {   
         fprintf(stderr,"Unable to open file: %s\n",argv[2]);
         return(1);
      }
   }
   
   /* Compare each charcater from the file with the specified set       */
   while((ch = fgetc(in)) != EOF)
   {
      if(strchr(argv[1],(char)ch) != NULL) count++;
   }
   
   /* Output the result                                                 */
   printf("%d\n",count);

   return(0);
}

      
