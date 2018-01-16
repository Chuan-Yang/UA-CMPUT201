/*
 * Name: Chuan Yang
 * ID:   1421992
 * CSID: chuan1
 * Lec Section: A1
 * LAB Seciton: D05
 * TA's Name: Juehui Fan
 */


#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#define MAX_LINE 1000000001
#define MAX_NAMELEN 129
#define _XOPEN_SOURCE

extern FILE *popen(const char *command, const char *type);
extern int pclose(FILE *stream);

strcut Images{
  double points[MAX_LINE];  // record the points in an image
  char name[MAX_NAMELEN];   // record the name of each image
}image[MAX_LINE];

int main(int argc, char *argv[])
{
        FILE *fp, *fppo;
	char command [10][10] = {"Image", "lineTo", "End", "print", "draw", "translate", "child", "#"};
	char line[257];  	//record every line in the file
	char comments[1001][257]; //record the comments and directly print out
	int ans[10] = {0}; 	//the answer number of every command
	long int i = 0, k = 0;		//index in for loop
	
	fp = freopen(argv[1],"r",stdin);
	fppo = popen("java -jar Sketchpad.jar");
	if (fp == NULL)		//
	{
	  printf("Cannot open the input file %s!\n",argv[1]);
	  exit(-1);
	}
	else
	{
	    while(fgets(line,257,fp))	//input one line of a time
	    {
	      int len = 0;
	      while (line[len]!=' ') 	//get the length of the first word of every line
		len++; 
			
	      char str [10] = ""; 	//get the first word if every line
	      for (i=0; i<len ;i++)
		str[i] = line[i];
			
	      //printf ("%s\n",str);  for debugging			
	      //begin to do all of the commands
	      if (!strcmp(str,"Image"))
		store();
	      if (!strcmp(str,"lineTo"))
		record();
	      if (!strcmp(str,"print"))
		std_print();
	      if (!strcmp(str,"draw"))
		draw();
	      if (!strcmp(str,"translate"))
		translate();
	      if (!strcmp(str,"child"))
		child();
	      if (!strcmp(str,"#"))
		comments();
	    }
	  
	}
	fclose(stout);
	pclose(fppo);
	return 0;
}
