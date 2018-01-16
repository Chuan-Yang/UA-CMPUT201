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
#include "draw1b.h"
#define _XOPEN_SOURCE

int print_stdout(FILE *fppo,struct Images image[MAX_LINE],char *image_name);
void draw(FILE *fppo,struct Images image[MAX_LINE],char *image_name);
void translate(FILE *fppo,struct Images image[MAX_LINE],char *image_name, double move_x, double move_y);

int main(int argc, char *argv[])
{
  FILE *fp;
  char line[257];  	//record every line in the file
  long int i = 0;		//index in for loop
  // char comments[MAX_LINE][257]; //recoed the comments and directly printf to stdin;
  // long int c_index;     //the index of comments, help to record
  fp = freopen(argv[1],"r",stdin);
  fppo = popen("java -jar Sketchpad.jar","w");
  memset (image,0,sizeof(image));
  if (fp == NULL)		//
    {
      printf	("Cannot open the input file %s!\n",argv[1]);
    }
  else
    {
      printf("%s started on ",argv[0]);
      fflush (stdout);
      system("date\n");
      printf("inpufile: %s\n",argv[1]);
      while(fgets(line,257,fp))	//input one line of a time
	{
	  int len = 0;
	  while (line[len]!=' ') 	//get the length of the first word of every line
	    len++; 
			
	  char str [10] = ""; 	//get the first word if every line
	  for (i=0; i<len ;i++)
	    str[i] = line[i];

			
	  //printf ("%s\n",str);  for debugging
			
	  //begin to handle  every command
	  if (!strcmp(str,"Image"))
	    {
	      sscanf (line,"Image %s %lf %lf =",image[image_index].name,&image[image_index].x[image[image_index].number], 
		  &image[image_index].y[image[image_index].number]);
	      image[image_index].number ++;	   
	  }
	  if (!strcmp(str,"lineTo"))
	    {
		  double add_x,add_y;
	      sscanf (line,"lineTo %lf %lf",&add_x, &add_y);
		  image[image_index].x[image[image_index].number] = image[image_index].x[image[image_index].number-1]+add_x;
		  image[image_index].y[image[image_index].number] = image[image_index].y[image[image_index].number-1]+add_y;
 	      image[image_index].number ++;
	    }
		    
	  if (!strcmp(str,"End"))
	    image_index++;

	  if (!strcmp(str,"print"))
	    {
	      char image_name[257];
	      sscanf (line,"print %s",image_name);
	      if (print_stdout(fppo,image,image_name) == 0)
		printf ("There is no this image!\n");
	    }
	  if (!strcmp(str,"draw"))
	    {
	      char image_name[257];
	      sscanf (line,"draw %s",image_name);
	      draw(fppo,image,image_name);
	    }
	  if (!strcmp(str,"translate"))
	    {
	      char image_name[257];
	      double move_x,move_y;
	      sscanf(line,"translate %s %lf %lf",image_name,&move_x,&move_y);
	      translate (fppo,image,image_name,move_x,move_y);
	    }
	  if (!strcmp(str,"child"))
	    {
	      char child_process[257]={""};
		  for (i=6;i<strlen(line)-1;i++)
			  child_process[i-6] = line [i];
	      fprintf(fppo,"%s\n",child_process);
	    }
	  if (!strcmp(str,"#"))
	    printf("%s",line);	 
	  }
    }
	
			
	
  fclose (fp);
  pclose (fppo);
  return 0;
}
