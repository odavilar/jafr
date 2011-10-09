#include <iostream>
#include "main.h"
extern "C" {
#include "serial.h"
}

using namespace std;

int main(int argc, char *argv[]){

	int fd = 0;
	char serialport[256];
	int baudrate = B9600;  // default
	char buf[256];

	if (argc==1) {
		usage();
		exit(EXIT_SUCCESS);
	}

	strcpy(serialport,argv[1]);

	fd = serialport_init(serialport, baudrate);
	if(fd==-1) return -1;
	printf("Serial Port %s was initialized succesfully", serialport);

	/*
	   int rc = serialport_write(fd, buf);
	   if(rc==-1) return -1;

	   serialport_read_until(fd, buf, '\n');
	   printf("read: %s\n",buf);
	   */

	return 0;
}


