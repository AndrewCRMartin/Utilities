/***************************************************************
      tail [-l n] <fnm>
****************************************************************
      Andrew C.R. Martin                            22.05.90

      Laboratory of Mathematical Biology            V1.x
      National Institute for Medical Research,
      The Ridgeway,
      Mill Hill,
      London,
      NW7 1AA

      This code is in the public domain. It may be copied and/or
      modified by anyone providing this notice is retained and 
      that the code is not sold for profit. Programs resulting
      from the use of this code may be sold commercially, providing
      suitable acknowledgements are included and that notice is
      given that this portion of the code is used with permission
      of the author.
****************************************************************
      tail is a UNIX-like file tail reader. It allows you to read 
      just the last lines of a file, by default, 22 lines are
      displayed, but this may be changed with the -l flag


      V1.x 07.11.94 Changed to check mallocs
****************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXCHAR 6400

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

main(argc,argv)
int argc;
char **argv;
{
   int numlines = 23,
       cycled = FALSE,
       i,
       start,
        stop;
   char **buffer, *flag;
   FILE *fp;

   if((argc<2)||(argc > 4))
   {
      printf("Useage: tail [-l n] <fnm>\n");
      exit(1);
   }

   if(argc==3)    /* We have a switch with the number touching */
   {
      argv++;
      if(strncmp(argv[0],"-l",2))    /* Check it's what it should be */
      {
         printf("Error in switch %s\n",argv[0]);
         exit(1);
      }
      flag = argv[0];
      flag += 2;
      numlines = atoi(flag);         /* Read numlines from next argument */ 
      numlines++;
   }

   if(argc==4)    /* We have a switch with the number separate */
   {
      argv++;
      if(strcmp(argv[0],"-l"))    /* Check it's what it should be */
      {
         printf("Error in switch %s\n",argv[0]);
         exit(1);
      }
      argv++;
      numlines = atoi(argv[0]);      /* Read numlines from next argument */ 
      numlines++;
   }

   /* Read the filename and open the file */
   argv++;
   if((fp=fopen(argv[0],"r"))==NULL)
   {
      printf("Unable to open file %s\n",argv[0]);
      exit(1);
   }

   /* Allocate space for the string buffer */
   if((buffer = (char **)malloc(numlines * sizeof(char *)))==NULL)
   {
      fprintf(stderr,"No memory for outer buffer\n");
      return(1);
   }
   
   for(i=0; i<numlines; i++)
   {
      if((buffer[i] = (char *)malloc(MAXCHAR * sizeof(char)))==NULL)
      {
         fprintf(stderr,"No memory for inner buffer %d\n",i);
         return(1);
      }
   }
   

   /* Read the file into the circular buffer */
   i=0;
   while(!feof(fp))
   { 
      fgets(buffer[i++],MAXCHAR-1,fp);
      
      if(i>=numlines)
      {
         cycled = TRUE;
         i=0;
      }
   }

   /* Set the start and stop points in the buffer */
   if(cycled) start = i; else start = 0;
   if(start >= numlines) start = 0;
   stop=i-1;

   if(start<stop)
   {
      for(i=start; i<stop; i++)
         printf("%s",buffer[i]);
   }
   else
   { 
      for(i=start; i<numlines; i++)
         printf("%s",buffer[i]);
      for(i=0; i<stop; i++)
         printf("%s",buffer[i]);
   }
}
