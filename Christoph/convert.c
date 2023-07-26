/*************************************************************************

   Program:    convert
   File:       convert.c
   
   Version:    V1.1
   Date:       05.11.93
   Function:   Convert frequency and score matrices
   
   Copyright:  (c) SciTech Software 1993
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
   This program takes a raw data file containing the following data:
   Subject number    (I2)        (Columns 1--2)
   Other number      (1X,I1,1X)  (Columns 3--5)
   Answer string     (A21)       (Columns 6--26)

   and a frequency file with the following data:
   Blank             (5X)        (Columns 1--5)
   Answer string     (A21)       (Columns 6--26)
   Frequency         (3X,I2)     (Columns 30--31)

   It creates a file containing flags for the answers given by each
   subject. For each subject there is an entry containing the subject
   number in columns 1--2 followed by a set of 0's or 1's to indicate
   whether a response has been given. This output line is split every
   80 characters.

   N.B. The answer-flag columns appear in the order used in the 
   frequency file

**************************************************************************

   Usage:
   ======
   convert [-s <n>][-m <n>][-r <c1> <c2>][-f <c1> <c2>] 
           <RawFile> <FreqFile> <OutFile>
      -s allows max # of subjects to be changed (Default = 20)
      -m allows max # of responses to be changed (Default = 200)
      -r allows columns in raw data file to be specified (Default: 6 26)
      -f allows columns in freq. file to be specified (Default: 6 26)
      <RawFile>  is the raw data file
      <FreqFile> is the frequency file
      <OutFile>  is the output file

**************************************************************************

   Notes:
   ======
   
**************************************************************************

   Revision History:
   =================
   V1.0  05.11.93 Original
   V1.1  08.11.93 Added -r and -f command line options

*************************************************************************/
/* Compile control flags
*/
/* #define DEBUG    */      /* Turn on debugging info                   */
#define NOREWIND            /* rewind() doesn't work so close and open  */

/************************************************************************/
/* Includes
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/************************************************************************/
/* Defines
*/
#define MAXSUBJECT    20 /* Max # of people questioned */
#define MAXRESPONSES 200 /* Max # of responses analysed */
#define FBSTART        5 /* Column of string in frequency file -1 */
#define FBEND         26 /* Column of end of string in frequency file */
#define RBSTART        5 /* Column of string in raw data file -1 */
#define RBEND         26 /* Column of end of string in raw data file */
/* N.B. FBEND-FBSTART must equal RBEND-RBSTART */

/************************************************************************/
/* Macros
*/
#ifdef DEBUG
#define D(BUG) printf(BUG)
#else
#define D(BUG)
#endif

/************************************************************************/
/* Globals
*/
FILE *gRawfp       = NULL,
     *gFreqfp      = NULL,
     *gOutfp       = NULL;
char gRawFile[160];
int  **gMatrix     = NULL;
int  gMaxSubject   = MAXSUBJECT,
     gMaxResponses = MAXRESPONSES,
     gFBStart      = FBSTART,
     gFBEnd        = FBEND,
     gRBStart      = RBSTART,
     gRBEnd        = RBEND;

/*************************************************************************/
/* Prototypes
*/
int  main(int argc, char **argv);
int  ReadComdLine(int argc, char **argv, char *f1, char *f2, char *f3);
int  Initialise(void);
int  OpenFiles(char *f1, char *f2, char *f3);
void WriteResults(void);
void DoProcessing(void);

/*************************************************************************/
/*>int main(int argc, char **argv)
   -------------------------------
   Main program.

   05.11.93 Original  By: ACRM
   08.11.93 Modified to check column widths
*/
int main(int argc, char **argv)
{ 
   char FreqFile[160],
        RawFile[160],
        OutFile[160];

   if(ReadComdLine(argc, argv, RawFile, FreqFile, OutFile ))
   { 
      if((gRBEnd-gRBStart) == (gFBEnd-gFBStart))
      {
         if(Initialise())
         {
            if(OpenFiles(RawFile, FreqFile, OutFile ))
            {
               DoProcessing();
               WriteResults();
	    }
         }
      }
      else
      {
         printf("\nYou cannot specify columns of different widths!\n");
         printf("Raw data file:  %d %d (width = %d)\n",
                gRBStart,gRBEnd,gRBEnd+1-gRBStart);
         printf("Frequency file: %d %d (width = %d)\n",
                gFBStart,gFBEnd,gFBEnd+1-gFBStart);
         printf("\nProgram fails!\n");
      }
   }
   else
   {
      printf("Usage: %s [-s <n>][-m <n>][-r <s> <e>][-f <s> <e>] <RawFile> \
<FreqFile> <OutFile>\n",
            argv[0]);
      printf("-s allows max # of subjects to be changed (Default = %d)\n",
           gMaxSubject);
      printf("-m allows max # of responses to be changed (Default = %d)\n",
          gMaxResponses);
      printf("-r allows the start and end column in the raw data file \
to be specified\n   (Default = %d %d)\n",RBSTART,RBEND);
      printf("-f allows the start and end column in the frequency file \
to be specified\n   (Default = %d %d)\n",FBSTART,FBEND);
      printf("<RawFile>  is the raw data file \n");
      printf("<FreqFile> is the frequency file \n");
      printf("<OutFile>  is the output file \n");
   }
   return(0);
}

/*************************************************************************/
/*>int ReadComdLine(int argc, char **argv, char *RawFile, char *FreqFile,
                    char *Outfile)
   ----------------------------------------------------------------------
   Process the command line

   05.11.93 Original  By: ACRM
   08.11.93 Added options of -r and -f
*/
int ReadComdLine(int argc, char **argv, char *RawFile, char *FreqFile,
                 char *OutFile)
{ 
   argc--; argv++;
   while(argc >3)
   { 
      if(argv[0][0] == '-')
      {
         switch(argv[0][1])
         {
         case 's': case 'S':
            argv++; argc--;
            sscanf(argv[0], "%d", &gMaxSubject);
            break;
         case 'm': case 'M':
            argv++; argc--;
            sscanf(argv[0], "%d", &gMaxResponses);
            break;
	  case 'r': case 'R':
            argv++; argc--;
            sscanf(argv[0], "%d", &gRBStart);
            argv++; argc--;
            sscanf(argv[0], "%d", &gRBEnd);
            break;
	  case 'f': case 'F':
            argv++; argc--;
            sscanf(argv[0], "%d", &gFBStart);
            argv++; argc--;
            sscanf(argv[0], "%d", &gFBEnd);
            break;
         default:
            break;
         }
     }
     else
     { 
        return(0);
     }
     argc--; argv++;
   }
   if(argc != 3) return(0);
   strcpy(RawFile,  argv[0]);
   strcpy(gRawFile, argv[0]);
   strcpy(FreqFile, argv[1]);
   strcpy(OutFile,  argv[2]);
   return(1);
}

/*************************************************************************/
/*>int Initialise(void)
   --------------------
   Create and initialise the results matrix

   05.11.93 Original  By: ACRM
*/
int Initialise(void)
{ 
   int i,j;

   if((gMatrix = (int **)malloc(gMaxSubject * sizeof(int)))==NULL)
      return(0);

   for(i=0; i<gMaxSubject; i++)
   {
      gMatrix[i]=(int*)malloc(gMaxResponses * sizeof(int));
      if(gMatrix[i]==NULL)
         return(0);

      for(j=0; j<gMaxResponses; j++)
         gMatrix[i][j]=0;
   }
   return(1);
}

/*************************************************************************/
/*>void WriteResults(void)
   -----------------------
   Write the results file with a max of 80 colums per row 

   05.11.93 Original  By: ACRM
*/
void WriteResults(void)
{ 
   int i,
       j, 
       column;

   D("Writing Results\n");

   printf("Highest subject number giving response listed in the frequency \
file was: %d\n",gMaxSubject);
   printf("A total of %d responses was found\n",gMaxResponses);

   for(i=0; i<gMaxSubject; i++)
   { 
      column=2;
      fprintf(gOutfp, "%2d", i+1);
      for(j=0; j<gMaxResponses; j++)
      { 
         if(gMatrix[i][j]) fprintf(gOutfp, "1");
         else              fprintf(gOutfp, "0");

         if(++column>=80)
         {
            fprintf(gOutfp, "\n");
            column=0;
	 }
      }
      if(column) fprintf(gOutfp, "\n");
   }
}

/*************************************************************************/
/*>void DoProcessing(void)
   -----------------------
   Do the actual processing. This is somewhat disk intensive to cut down
   on memory usage.

   05.11.93 Original  By: ACRM
   08.11.93 Use global variables gFBStart, gFBEnd, etc., rather than
            constants
*/
void DoProcessing(void)
{
   char FreqBuffer[160],
        RawBuffer[160],
        *fbptr,
        *rbptr;
   int  subject,
        RespCount=0,
        MaxSubject=0;

   rewind(gFreqfp);
   rewind(gRawfp);

   while(fgets(FreqBuffer, 160, gFreqfp)) /*step through freq.file */
   { 
      D("Read line from freq. file\n");

      FreqBuffer[gFBEnd] = '\0';
      fbptr=FreqBuffer + gFBStart;

      /* step through raw data */
      while(fgets(RawBuffer, 160, gRawfp))
      { 
         RawBuffer[gRBEnd] = '\0';
         rbptr =RawBuffer + gRBStart;
         /* See if the strings mach. This could be replaced by a
            more complex routine if we want to do subclass pattern matching 
         */
         if(!strcmp(fbptr, rbptr))
         { 
            /* yes, we have a match */

            D("Match found in raw data file\n");

            *rbptr ='\0';

            /* Read the subect number */
            if(sscanf(RawBuffer, "%d", &subject))
            { 
               if(subject>gMaxSubject)
               { 
                  printf("Subject number %d exceeds max. allowed; \
Use -s option \n",subject);
                  exit(1);
               }
               if(subject > MaxSubject) MaxSubject = subject;
               gMatrix[subject-1][RespCount] = 1;
            }
         }
      } /* End of raw data file */

      /* Increment response count */
      if(++RespCount > gMaxResponses)
      { 
         printf("Max. # of responses exceeded; use -m option \n");
         exit(1);
      }
      /* Rewind the raw data file.
         Some C compilers aren`t very good at rewind() with a big file 
         so we give a compile time option to close and re-open instead.
         No error checking since we know the file can be opened. 
      */
      D("Rewinding...\n");
#ifdef NOREWIND
      fclose(gRawfp);
      gRawfp = fopen(gRawFile, "r");
#else
      rewind(gRawfp);
#endif
   } /* End of frequency data file */

   /* update these globals for WriteResults() to use */
   gMaxResponses = RespCount;
   gMaxSubject   = MaxSubject;
}


/*************************************************************************/
/*>int OpenFiles(char *RawFile, char *FreqFile, char *OutFile)
   -----------------------------------------------------------
   Open the 3 files, reporting errors 

   05.11.93 Original  By: ACRM
*/
int OpenFiles(char *RawFile, char *FreqFile, char *OutFile)
{ 
   if((gRawfp = fopen(RawFile, "r"))==NULL)
   { 
      printf("Error opening file: %s \n", RawFile);
      return(0);
   }

   if((gFreqfp = fopen(FreqFile, "r"))==NULL)
   {  
      printf("Error opening file: %s \n", FreqFile);
      return(0);
   }

   if((gOutfp=fopen(OutFile, "w"))==NULL)
   {
      printf("Error opening file: %s (for output) \n", OutFile);
      return(0);
   }
   return(1);
}



