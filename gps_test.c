/**
 * @file   gps_test.c
 * @author Marcos Canales Mayo
 * @date   7 May 2016
 * @version 1.0
 */

#include <stdio.h>
#include "gps.h"

int main(int argc, char **argv){
	char buf[100];
	int i;

	gps_init();
	for(i=0; i<20; i++){
		gps_read(buf);
	}
	gps_exit();

	return 0;
}