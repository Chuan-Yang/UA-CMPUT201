#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

extern FILE *popen(const char *command, const char *type);
extern int pclose (FILE *stream);

int main()
{
  FILE *fppo;
  long int a=10;
  long int b=10;
  long int c=60;
  long int d=10;
  fppo = popen("java -jar Sketchpad.jar","w");
  
  fprintf(fppo,"drawSegment %ld %ld %ld %ld\n",a,b,c,d);
  fprintf (fppo,"pause 60\n");
fprintf (fppo,"end\n");


 pclose(fppo);
}
