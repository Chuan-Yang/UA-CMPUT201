#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int main(int argc, char * argv[])
{
  FILE *fp;
  fp = freopen(argv[1],"r",stdin);
  long int len = 10; 
  long int i = 0, j = 0;
  int *d_array = (int *) malloc(len*sizeof(int)); 
  memset (d_array,0,len*sizeof(int));
  //d_array  = (int *) malloc(10);
  char line[101];
  memset(line,0,101);
  while (!feof(fp))
    {
      scanf("%d",&d_array[i]);
      i++;
      if (i>=len-1)
	{
	   len *= 2;
	   d_array = realloc (d_array,len*sizeof(int));
	   
	}
    }

  for(j=i-2;j>=0;j--)
    printf("%d\n",d_array[j]);

  free(d_array);
  return 0;
}
