#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(int argc, char *argv[])
{
  FILE *fp;
  char line[100001];
  int array [101]={0};
  int i = 0,len=0;
  fp = fopen(argv[1],"r");
if (fp==NULL)
  while (fgets(line,101,fp)!=NULL)
    {
      sscanf (line,"%d",&array[len]);
      len++;
    }
  for (i=len-2;i>=0;i--)
    printf("%d\n",array[i]);
fclose(fp);  
return 0;
}
