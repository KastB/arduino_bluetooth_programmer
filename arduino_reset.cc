#include <sys/ioctl.h>
#include <termios.h>
#include <linux/serial.h> 
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stropts.h>

int main()
{
  const char *dev="/dev/rfcomm0";
  int fd=open(dev,O_RDONLY);
  if(fd<0)
  {
    fprintf(stderr,"Couldn't open %s\n",dev);
    return(1);
  }
  fprintf(stderr,"Opened\n");
  sleep(30);
  fprintf(stderr,"Clearing DTR\n");
  close(fd);
  return(0);
}
