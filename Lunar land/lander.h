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



extern FILE *popen (const char *command, const char *type);
extern int plcose (FILE *stream);



struct Ship
{
  double x[5],y[5];
  double center_x, center_y;
  double vel_x,vel_y;
  double degree;
  double thrust_x, thrust_y;
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
void landscape (FILE *fppo,FILE *fp);









