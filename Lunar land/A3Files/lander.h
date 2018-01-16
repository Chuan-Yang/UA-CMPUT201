/*
 * Name: Chuan Yang
 * ID:   1421992
 * CSID: chuan1
 * Lec Section: A1
 * LAB Seciton: D05
 * TA's Name: Juehui Fan
 */
#define _XOPEN_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <math.h>
#include <assert.h>
#include <ncurses.h>
#include <string.h>

#define MAX_LINE_LEN 21
#define _USE_MATH_DEFINES

long int i = 0, j = 0;

extern FILE *popen (const char *command, const char *type);
extern int plcose (FILE *stream);



struct Ship
{
  double x[5],y[5];
  double center_x, center_y;
  double vel_x,vel_y;
  double degree;
};

struct op_Ship
{
  double x[5],y[5];
  double center_x, center_y;
};


void init_ncurses();
void shutdown_ncurses();
void erase_old (FILE *fppo, struct Ship ship);
void draw (FILE *fppo, struct Ship ship);
void rotate(FILE *fppo, double angle_radians, struct Ship ship);


void landscape (FILE *fppo,FILE *fp)
{
  char line [MAX_LINE_LEN];
  long int land_x[21] = {0}, land_y[21] = {0};
  int index = 0;
  while (fgets(line , MAX_LINE_LEN , fp))
    {
      int begin = 0;    //begin is to record the first char that isn't a space
      int empty_line = 1;    
      for (i = 0;i < strlen(line);i++)
	if ((line[i] != ' ')&&(line[i] != '\n'))
	  {
	    begin = i;
	    empty_line = 0;
	    break;
	  }

      if (empty_line)
	continue;
      
      if (begin != 0)
	{
	  char str[MAX_LINE_LEN];
	  strcpy (str,line);
	  memset (line, 0, MAX_LINE_LEN);
	  for (i = begin;i<strlen(str);i++)
	      line [i-begin] = str [i];
	}
      
      sscanf (line, "%ld %ld", &land_x[index],&land_y[index]);
      index++;
    }
  
  for (i = 0;i < index - 1;i++)
    fprintf(fppo, "drawSegment %ld %ld %ld %ld\n", land_x[i],land_y[i],land_x[i+1],land_y[i+1]);

  //fprintf(fppo,"pause 0.05\n");

  return;

}





