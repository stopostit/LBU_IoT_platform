#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<termios.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define BAUDRATE B9600
#define MODEMDEVICE "/dev/ttyATH0"
#define FALSE 0
#define TRUE 1

volatile int STOP=FALSE;

int main(int argc, char *argv[])
{
		int fd, c, res;
		struct termios oldtio, newtio;
		char buf[255];

		fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY);
		if (fd <0) {perror(MODEMDEVICE); exit(-1); }
		tcgetattr(fd,&oldtio);
		tcgetattr(fd,&oldtio);


		newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
		newtio.c_iflag = IGNPAR | ICRNL;
		newtio.c_oflag = 0;
		newtio.c_lflag = ICANON;

		tcflush(fd, TCIFLUSH);
		tcsetattr(fd,TCSANOW,&newtio);

		//int counter = 0;
		while (STOP==FALSE) {     /* loop until we have a terminating condition */
				/* read blocks program execution until a line terminating character is
				   input, even if more than 255 chars are input. If the number
				   of characters read is smaller than the number of chars available,
				   subsequent reads will return the remaining chars. res will be set
				   to the actual number of characters actually read */
				res = read(fd,buf,255);
				buf[res]=0;             /* set end of string, so we can printf */
				printf("%s\n", buf, res);
				//if (counter++>5) STOP=TRUE;
		}

		tcsetattr(fd,TCSANOW,&oldtio);
		return 0;
}
