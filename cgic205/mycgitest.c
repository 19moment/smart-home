/*************************************************************************
	#	 FileName	: test.c
	#	 Author		: fengjunhui 
	#	 Email		: 18883765905@163.com 
	#	 Created	: 2018年02月28日 星期三 18时35分22秒
 ************************************************************************/

#include<stdio.h>
#include "cgic.h"
#include <string.h>
#include <stdlib.h>

int cgiMain() {
#ifdef DEBUG
	printf("Enter cgiMain() .....\n");
#endif /* DEBUG */

	cgiHeaderContentType("text/html");
	/* Top of the page */
	fprintf(cgiOut, "<HTML><HEAD>\n");
	fprintf(cgiOut, "<TITLE>My First CGI</TITLE></HEAD>\n");
	fprintf(cgiOut, "<BODY><H1>Hello CGIC......</H1>\n");
	fprintf(cgiOut, "</BODY></HTML>\n");

	return 0;
}

