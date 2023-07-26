#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "bioplib/macros.h"

typedef struct _memlist
{
   struct _memlist *next;
   unsigned long   *mem;
   unsigned long   size;
}  MEMLIST;


#define NTEST 1000
#define MAXMEM 128000000
/* #define VERBOSE */

int main(int argc, char **argv)
{
   unsigned long size = 1,
                 i,
                 j,
                 totmem = 0,
                 *mem;
   MEMLIST       *memlist = NULL, 
                 *m;
   int           n = 0;
   
   
   /* Keep doubling the size we try to malloc()                              */
   do
   {
      if(memlist==NULL)
      {
         INIT(memlist, MEMLIST);
         m=memlist;
      }
      else
      {
         ALLOCNEXT(m, MEMLIST);
      }

      if(m==NULL)
         break;
      
      mem=(unsigned long *)malloc(size * sizeof(unsigned long));
      if(mem!=NULL)
      {
         m->mem  = mem;
         m->size = size;
         printf("Allocated %lu\n", size);
         totmem += size;
         n++;

         if(totmem*sizeof(unsigned long) >= MAXMEM)
            break;
      }

      size *= 2;
   }  while(mem!=NULL);

   if(memlist==NULL)
   {
      fprintf(stderr,"No memory allocated\n");
      return(0);
   }
   
   /* Now keep trying smaller chunks                                         */

   while((size > 1) && (totmem*sizeof(unsigned long) < MAXMEM))
   {
      mem=(unsigned long *)malloc(size * sizeof(unsigned long));
      if(mem!=NULL)
      {
         ALLOCNEXT(m, MEMLIST);
         if(m==NULL)
         {
            /* We didn't manage a space in our linked list as well, so free
               this one and forget about it
            */
            free(mem);
         }
         else
         {
            m->mem  = mem;
            m->size = size;
            printf("Allocated %lu\n", size);
            totmem += size;
            n++;
         }
      }
      else
      {
         size /= 2;
      }
   };

   printf("Total memory allocated = %lu in %d blocks\n",
          totmem*sizeof(unsigned long),n);
   
   /* Now thrash the memory                                                  */
   for(i=0; i<NTEST; i++)
   {
      printf("Test %lu. ",i);
      fflush(stdout);
      
      printf("Setting %lu...", ((i%2)?ULONG_MAX:0));
      fflush(stdout);
      
      for(m=memlist, n=0; m!=NULL; NEXT(m))
      {
#ifdef VERBOSE
         printf("Setting values for block %d\n", n++);
#endif
         for(j=0; j<m->size; j++)
         {
            if(i%2)
            {
               m->mem[j] = ULONG_MAX;
            }
            else
            {
               m->mem[j] = 0;
            }
         }
      }

      printf("testing...");
      fflush(stdout);

      for(m=memlist, n=0; m!=NULL; NEXT(m))
      {
#ifdef VERBOSE
         printf("Testing values for block %d\n", n++);
#endif

         for(j=0; j<m->size; j++)
         {
            if(((i%2) && (m->mem[j] != ULONG_MAX)) ||
               ((!i%2) && (m->mem[j] != 0)))
            {
               printf("%lu is %lu - should be %lu\n",
                      (unsigned long int)m->mem+j, 
                      m->mem[j], 
                      j);
            }
         }
      }

      printf("done\n");

   }

   return(0);
}
