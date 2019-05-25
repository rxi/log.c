#include<stdio.h>
#include<conio.h>
#include"..\..\log.h"

void main(void)
{
	//if you want to save logs in a file
	FILE *fp;
	fp = fopen("log.bin","wb+");
	log_set_fp(fp);
	//sample log messages
	log_trace("hello %s", "world");
	log_debug("this is a debug message");
	log_info("passing information");
	log_warn("this is a warning message");
	log_error("this is an error message");
	log_fatal("this is a fatal message");
	getch();
}