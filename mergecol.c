/*************************************************************************

   Program:    
   File:       
   
   Version:    
   Date:       
   Function:   
   
   Copyright:  (c) Dr. Andrew C. R. Martin 1994
   Author:     Dr. Andrew C. R. Martin
   Address:    Biomolecular Structure & Modelling Unit,
               Department of Biochemistry & Molecular Biology,
               University College,
               Gower Street,
               London.
               WC1E 6BT.
   Phone:      (Home) +44 (0372) 275775
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
#include <stdlib.h>

#include "bioplib/SysDefs.h"
#include "bioplib/macros.h"

/************************************************************************/
/* Defines and macros
*/
#define MAXBUFF 160

/************************************************************************/
/* Globals
*/

/************************************************************************/
/* Prototypes
*/

/************************************************************************/
int main(int argc, char **argv)
{
   FILE **files;
   char **buffers;
   BOOL *Ended,
        SomeEnded = FALSE,
        BreakOut  = FALSE,
        Warned    = FALSE;
   int  i;
   
   /* Check the command line                                            */
   if(argc < 2)
   {
      fprintf(stderr,"\nMergeCol V1.0 (c) 1994 Dr. Andrew C.R. Martin, \
UCL\n\n");
      fprintf(stderr,"Usage: mergecol file [file....] > file.out\n");
      fprintf(stderr,"Merges a set of 1-column files into a \
multi-column file.\n\n");
      
      return(1);
   }
   
   argc--;
   argv++;
   
   /* Allocate memory for file pointers, buffers and flags              */
   if((files=(FILE **)malloc(argc * sizeof(FILE *)))==NULL)
   {
      fprintf(stderr,"No memory for array of file pointers\n");
      return(1);
   }
   if((buffers=(char **)malloc(argc * sizeof(char *)))==NULL)
   {
      fprintf(stderr,"No memory for array of strings\n");
      return(1);
   }
   if((Ended=(BOOL *)malloc(argc * sizeof(BOOL)))==NULL)
   {
      fprintf(stderr,"No memory to store flags\n");
      return(1);
   }

   /* Clear file-ended flags                                            */
   for(i=0; i<argc; i++)
      Ended[i] = FALSE;

   /* Open files and allocate memory for buffers                        */
   for(i=0; i<argc; i++)
   {
      if((files[i] = fopen(argv[i],"r"))==NULL)
      {
         fprintf(stderr,"Unable to open input file: %s (ignored)\n",
                 argv[i]);
         Ended[i] = TRUE;
      }
      else
      {
         if((buffers[i] = (char *)malloc(MAXBUFF * sizeof(char)))==NULL)
         {
            fprintf(stderr,"No memory for buffer\n");
            return(1);
         }
      }
   }

   /* Sit in endless loop                                               */
   BreakOut = FALSE;
   for(;;)
   {
      /* Read a record from each file                                   */
      for(i=0; i<argc; i++)
      {
         if(!Ended[i])
         {
            if(!fgets(buffers[i], MAXBUFF, files[i]))
               Ended[i] = TRUE;
            else
               TERMINATE(buffers[i]);
         }
      }

      /* See if all files have ended                                    */
      for(i=0,SomeEnded=FALSE,BreakOut=TRUE; i<argc; i++)
      {
         SomeEnded |= Ended[i];
         BreakOut  &= Ended[i];
      }

      /* Issue a warning if not all files ended at once                 */
      if(SomeEnded && !BreakOut && !Warned)
      {
         fprintf(stderr,"Not all files ended at the same time\n");
         Warned = TRUE;
      }

      /* If flag is set for all files ended, break out of the loop      */
      if(BreakOut) break;

      /* Print each record out                                          */
      for(i=0; i<argc; i++)
         printf("%s ",(Ended[i] ? "????" : buffers[i]));
      printf("\n");
   }

   return(0);
}

