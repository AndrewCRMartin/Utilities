#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
   char *devs[] = 
   {
      "/dev/hdc",
      "/dev/usb/scanner0",
      "/dev/video",
      "/dev/video0",
      "/dev/mixer",
      "/dev/dsp",
      NULL
   };

   int i;
   
   for(i=0; devs[i]; i++)
   {
      chmod(devs[i], (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH));
   }
   
   unlink("/tmp/.esd/socket");
   rmdir("/tmp/.esd");

/*   system("/sbin/service smb restart"); */
   
   return(0);
}
