#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "parse.h"

char program_name[PROG_LEN + 1];
const char exec_name[] = "java -jar Sketchpad.jar -d";
int shiftX=0; 
int shiftY=0;

int main(int argc, char* argv[])
{
  FILE* input;
  FILE* executable;
  
  memset(program_name, 0, PROG_LEN + 1);
  strncpy(program_name, argv[0], PROG_LEN);

  if (argc != 4) {
    fprintf(stderr, "Usage: %s input-file shiftX shiftY\n", program_name);
    exit(1);
  }

  input = fopen(argv[1], "r");

  if (input == NULL) {
    fprintf(stderr, "Could not open input file %s\n", argv[1]);
    exit(1);
  }

  /*** Add code to extract the shift values from the command line ***/
  /*** arguments and set the shift values shiftX and shiftY ***/
  int i =0;
  for (i=0;i<strlen(argv[2]);i++)
    shiftX = shiftX*10 + (argv[2][i]-'0');

  for (i=0;i<strlen(argv[3]);i++)
    shiftY = shiftY*10 + (argv[3][i]-'0');

  executable = popen(exec_name, "w");
  if (executable == NULL)
    fprintf(stderr, "Could not open pipe %s\n", exec_name);

  convert(input, executable);

  pclose(executable);
  fclose(input);

  return EXIT_SUCCESS;
}

// Read from input, convert and send the rows to executable 
void convert(FILE* input, FILE* executable)
{
  char command[LINE_LEN + 1];
  char line[LINE_LEN + 1];
  int x1=0,x2=0,y1=0,y2=0;
  memset(line, 0, LINE_LEN + 1);
  memset(command, 0, LINE_LEN + 1);
  while (fgets(line, LINE_LEN + 1, input) != NULL)
  {
    if (strncmp(line, "drawSegment", 11) == 0) {
      fprintf(executable, "%s", line);  // print the original line

      /*** Add code to shift the X and Y coordinates respectively by shiftX
           and shiftY (i.e. add shiftX to X and shiftY to Y) and draw 
           the shifted line ***/
      sscanf(line,"drawSegment %d %d %d %d",&x1,&y1,&x2,&y2);
      x1 += shiftX;
      y1 += shiftY;
      x2 += shiftX;
      y2 += shiftY;
      fprintf(executable,"drawSegment %d %d %d %d\n",x1,y1,x2,y2);
    }
    else 
      fprintf(executable, "%s", line);

    memset(line, 0, LINE_LEN + 1);
    memset(command, 0, LINE_LEN + 1);
  }
}
