#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<termios.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#define DEST_URL "https://iot-project-1e8b6.firebaseio.com/items.json"
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

		CURL *curl;

		struct curl_slist *headers = NULL;
		headers=curl_slist_append(headers, "Accept: application/json");
		headers=curl_slist_append(headers, "Content-Type: application/json");
		headers=curl_slist_append(headers, "charsets: utf-8");
		CURLcode res;
		curl_global_init(CURL_GLOBAL_DEFAULT);
		curl = curl_easy_init();
		if(curl == NULL){
				perror("curl init error");
				exit(1);
		}

		curl_easy_setopt(curl, CURLOPT_URL, DEST_URL );
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 

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
				printf("%s\n", buf);

				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buf);
				rescurl = curl_easy_perform(curl);
				if (rescurl != CURLE_OK) {
						fprintf(stderr, "curl_easy_perform() failded: %s\n", curl_easy_strerror(rescurl));
				}
				//if (counter++>5) STOP=TRUE;
		}

		curl_easy_cleanup(curl);
		curl_global_cleanup();
		tcsetattr(fd,TCSANOW,&oldtio);
		return 0;
}include "serial.h"
