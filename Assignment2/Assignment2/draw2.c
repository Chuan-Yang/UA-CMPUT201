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
#include "draw2.h"

#define _XOPEN_SOURCE

int print_stdout(FILE *fppo,struct Images image[MAX_LINE],char *image_name);
void draw(FILE *fppo,struct Images image[MAX_LINE],char *image_name);
void rotate(FILE *fpppo,struct Images *image, char *image_name,double angle_radians);
void translate(FILE *fppo,struct Images image[MAX_LINE],char *image_name, double move_x, double move_y);

int main(int argc, char *argv[])
{
  FILE *fp;
  char line[MAX_LINE_LEN];  	//record every line in the file
  long int i = 0;		//index in for loop
  // char comments[MAX_LINE][257]; //recoed the comments and directly printf to stdin;
  // long int c_index;     //the index of comments, help to record
  struct Images *image ;
  image = malloc (MAX_LINE_LEN*sizeof (struct Images));
  memset (image,0,MAX_LINE_LEN);
  fp = freopen(argv[1],"r",stdin);
  fppo = popen("java -jar Sketchpad.jar","w");
  if (fp == NULL)		//
    {
      printf	("Cannot open the input file %s!\n",argv[1]);
    }
  else
    {
      printf("%s started on ",argv[0]);
      fflush (stdout);
      system("date\n");
      printf("inputfile: %s\n",argv[1]);
      while(fgets(line,MAX_LINE_LEN,fp))	    //input one line of a time
	{
	  int len = 0,begin = 0;    //begin is to record the first char that isn't a space
	  int empty_line = 1;       
	  for (i=0;i<strlen(line);i++)
	    if (line[i] != ' ')
	      {
		begin = i;
		empty_line = 0;
		break;
	      }

	  if (empty_line)
	    continue;

	  while (line[begin+len]!=' ') 	//get the length of the first word of every line
	    len++; 
			
	  char str [10] = ""; 	//get the first word if every line
	  for (i=0; i<len ;i++)
	    str[i] = line[i+begin];

			
	  //printf ("%s\n",str);  for debugging
			
	  //begin to handle  every command
	  if (!strcmp(str,"Image"))
	    {
	      char image_name[MAX_NAMELEN];
	      double image_x=0.0,image_y=0.0;
	      sscanf (line,"%s %s %lf %lf =",str,image_name,&image_x, &image_y);
	      strcpy(image[image_index].name, image_name);
	      image[image_index].x[image[image_index].number] = image_x;
	      image[image_index].y[image[image_index].number] = image_y;
	      image[image_index].number ++;	   
	  }

	  if (!strcmp(str,"lineTo"))
	    {
	      double add_x,add_y;
	      int index = image[image_index].number;
	      sscanf (line,"%s %lf %lf",str,&add_x, &add_y);
	      image[image_index].x[index] = image[image_index].x[index-1]+add_x;
	      image[image_index].y[index] = image[image_index].y[index-1]+add_y;
 	      image[image_index].number ++;
	    }
		    
	  if (!strcmp(str,"End"))
	    {
	      image_index++;
	      if (image_index == len_image)
		{
		  image = realloc(image,2*MAX_LINE_LEN*(sizeof(image)));
		  len_image *= 2;
		}
	    }

	  if (!strcmp(str,"print"))
	    {
	      char image_name[MAX_LINE_LEN];
	      sscanf (line,"%s %s",str,image_name);
	      if (print_stdout(fppo,image,image_name) == 0)
		printf ("There is no this image!\n");
	    }

	  if (!strcmp(str,"draw"))
	    {
	      char image_name[MAX_LINE_LEN];
	      sscanf (line,"%s %s",str,image_name);
	      draw(fppo,image,image_name);
	    }

	  if (!strcmp(str,"translate"))
	    {
	      char image_name[MAX_LINE_LEN];
	      double move_x,move_y;
	      sscanf(line,"%s %s %lf %lf",str,image_name,&move_x,&move_y);
	      translate (fppo,image,image_name,move_x,move_y);
	    }

	  if (!strcmp(str,"rotate"))
	    {
	      char image_name[MAX_LINE_LEN];
	      double angle_degrees = 0.0,angle_radians = 0.0;
	      sscanf(line,"%s %s %lf",str,image_name,&angle_degrees);
	      angle_radians = angle_degrees * M_PI / 180.0;
	      rotate (fppo,image,image_name,angle_radians);

	    }

	  if (!strcmp(str,"child"))
	    {
	      char child_process[MAX_LINE_LEN]={""};
	      for (i=6+begin;i<strlen(line);i++)
		child_process[i-6] = line [i];
	      fprintf(fppo,"%s\n",child_process);
	    }
	  if (!strcmp(str,"#"))
	    printf("%s",line);	 
	  }
    }
	
			
	
  fclose (fp);
  pclose (fppo);
  free(image);
  return 0;
}
