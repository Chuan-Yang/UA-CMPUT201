/*
 * Name: Chuan Yang
 * ID:   1421992
 * CSID: chuan1
 * Lec Section: A1
 * LAB Seciton: D05
 * TA's Name: Juehui Fan
 */


#include "lander.h"
FILE *fppo,*fp;

static struct Ship ship;

double gravity = 0.0, thrust = 0.0;
double xA=0.0, yA=0.0;
int rotated = 0, thrusted = 0;
double angle_radians=0.0;
int running;
long int i = 0, j = 0;
long int land_x[21] = {0}, land_y[21] = {0};
int index = 0;
int crashed = 0, landed = 0;

//Code from ball.c until function shutdown
void init_ncurses()
{
  int r;

  // start up the ncurses environment
  initscr(); // nothing to check, initscr exits on error

  // don't wait for enter for keyboard input
  r = cbreak(); assert( r != ERR );

  // don't echo keypresses to screen
  r = noecho(); assert( r != ERR );

  r = nonl(); assert( r != ERR );

  // turn cursor off
  r = curs_set( 0 ); assert( r != ERR );

  // don't worry about cursor position
  r = leaveok( stdscr, TRUE ); assert( r != ERR );

  // slower handling of ^C but saner output
  r = intrflush( stdscr, FALSE ); assert( r != ERR );

  // translate movement espace codes into single keys
  r = keypad( stdscr, TRUE ); assert( r != ERR );
}

void shutdown_ncurses()
{
  endwin();
}
//End of the cp

void landscape (FILE *fppo,FILE *fp)
{
  char line [MAX_LINE_LEN];
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

void erase_old (FILE *fppo, struct Ship ship)
{
  long int x1,x2,y1,y2;
  x1 = lround (ship.x[0]);
  x2 = lround (ship.x[2]);
  y1 = lround (ship.y[0]);
  y2 = lround (ship.y[2]);
  fprintf(fppo,"eraseSegment %ld %ld %ld %ld\n",x1, y1, x2, y2);
  for (i = 0; i < 3; i++)
    {
      if (i == 0 || i ==2)
	{
	  x1 = lround (ship.x[i]);
	  x2 = lround (ship.x[i+1]);
	  y1 = lround (ship.y[i]);
	  y2 = lround (ship.y[i+1]);
	  fprintf(fppo,"eraseSegment %ld %ld %ld %ld\n",x1, y1, x2, y2);
	}
      else
	{
	  x1 = lround (ship.x[i]);
	  x2 = lround (ship.x[i+2]);
	  y1 = lround (ship.y[i]);
	  y2 = lround (ship.y[i+2]);
	  fprintf(fppo,"eraseSegment %ld %ld %ld %ld\n",x1, y1, x2, y2);
	}
    }
  long int x_t,y_t;
  x1 = lround (ship.x[2]);
  x2 = lround (ship.x[3]);
  y1 = lround (ship.y[2]);
  y2 = lround (ship.y[3]);
  x_t = lround(ship.thrust_x);
  y_t = lround (ship.thrust_y);
  fprintf (fppo, "eraseSegment %ld %ld %ld %ld\n" , x1, y1, x_t, y_t);
  fprintf (fppo, "eraseSegment %ld %ld %ld %ld\n" , x2, y2, x_t, y_t );
}

void draw (FILE *fppo, struct Ship ship)
{
  long int x1,x2,y1,y2;
  x1 = lround (ship.x[0]);
  x2 = lround (ship.x[2]);
  y1 = lround (ship.y[0]);
  y2 = lround (ship.y[2]);
  fprintf(fppo,"drawSegment %ld %ld %ld %ld\n",x1, y1, x2, y2);
  for (i = 0; i < 3; i++)
    {
      if (i == 0 || i ==2)
	{
	  x1 = lround (ship.x[i]);
	  x2 = lround (ship.x[i+1]);
	  y1 = lround (ship.y[i]);
	  y2 = lround (ship.y[i+1]);
	  fprintf(fppo,"drawSegment %ld %ld %ld %ld\n",x1, y1, x2, y2);
	}
      else
	{
	  x1 = lround (ship.x[i]);
	  x2 = lround (ship.x[i+2]);
	  y1 = lround (ship.y[i]);
	  y2 = lround (ship.y[i+2]);
	  fprintf(fppo,"drawSegment %ld %ld %ld %ld\n",x1, y1, x2, y2);
	}
    }

    if (thrusted == 1)
    {
      long int x_t,y_t;
      x1 = lround (ship.x[2]);
      x2 = lround (ship.x[3]);
      y1 = lround (ship.y[2]);
      y2 = lround (ship.y[3]);
      x_t = lround(ship.thrust_x);
      y_t = lround (ship.thrust_y);
      fprintf(fppo, "drawSegment %ld %ld %ld %ld\n", x1, y1, x_t, y_t);
      fprintf(fppo, "drawSegment %ld %ld %ld %ld\n" , x2, y2, x_t, y_t );
      thrusted = 0;
    }
	//fprintf (fppo, "pause 1\n");
   //fprintf(fppo,"end\n");
}

//copy from the GIVEN WEBSITE
int lineSegmentIntersection(double Ax, double Ay,double Bx, double By,double Cx, double Cy,double Dx, double Dy)
{
  double  distAB, theCos, theSin, newX, ABpos ;

  //  Fail if either line segment is zero-length.
  if ((Ax==Bx && Ay==By) || (Cx==Dx && Cy==Dy)) return 0;

  //  Fail if the segments share an end-point.
  if ((Ax==Cx && Ay==Cy) || (Bx==Cx && By==Cy)
      ||  (Ax==Dx && Ay==Dy) || (Bx==Dx && By==Dy)) {
    return 0; }

  //  (1) Translate the system so that point A is on the origin.
  Bx-=Ax; By-=Ay;
  Cx-=Ax; Cy-=Ay;
  Dx-=Ax; Dy-=Ay;

  //  Discover the length of segment A-B.
  distAB=sqrt(Bx*Bx+By*By);

  //  (2) Rotate the system so that point B is on the positive X axis.
  theCos=Bx/distAB;
  theSin=By/distAB;
  newX=Cx*theCos+Cy*theSin;
  Cy  =Cy*theCos-Cx*theSin; Cx=newX;
  newX=Dx*theCos+Dy*theSin;
  Dy  =Dy*theCos-Dx*theSin; Dx=newX;

  //  Fail if segment C-D doesn't cross line A-B.
  if ((Cy<0. && Dy<0.) || (Cy>=0. && Dy>=0.)) return 0;

  //  (3) Discover the position of the intersection point along line A-B.
  ABpos=Dx+(Cx-Dx)*Dy/(Dy-Cy);

  //  Fail if segment C-D crosses line A-B outside of segment A-B.
  if (ABpos<0. || ABpos>distAB) return 0;

  //  (4) Apply the discovered position to line A-B in the original coordinate system.

  //  Success.
  return 1; 
} 

void handle_signal(int signal)
{
  erase_old(fppo,ship);

  double max_xy[2]= {0.0,0.0}, min_xy[2]={0.0,0.0};
  double maxx = 0, maxy=0;
  for (i = 0; i < 4; i++)
    {
      if (ship.x[i]>maxx)
	maxx = ship.x[i];
      if (ship.y[i]>maxy)
	maxy = ship.y[i];
    }
  max_xy[0] = maxx;
  max_xy[1] = maxy;
  
  double minx = 1000000.0, miny=100000.0;
  for (i = 0; i < 4; i++)
    {
      if (ship.x[i] < minx)
	minx = ship.x[i];
      if (ship.y[i] < miny)
	miny = ship.y[i];
    }
  min_xy[0] = minx;
  min_xy[1] = miny;

  ship.center_x = (max_xy[0] + min_xy[0]) /2;
  ship.center_y = (max_xy[1] + min_xy[1]) /2;
  //printf ("%f %f\n",ship.center_x,ship.center_y);

 // printf ("%d\n",rotated);
  if (rotated!=0)
    {
      angle_radians = rotated * 10 * M_PI / 180.0;
      for (i = 0; i < 4; i++)
	 {
	   double x=0.0, y=0.0;   
	   double x_o = ship.x[i] - ship.center_x;
	   double y_o = ship.y[i] - ship.center_y;
	   x = x_o * cos(angle_radians) - y_o * sin(angle_radians);
	   y = x_o * sin(angle_radians) + y_o * cos(angle_radians);
	   ship.x[i] = x + ship.center_x;
	   ship.y[i] = y + ship.center_y;
	 }

      double x=0.0, y=0.0;   
      double x_o = ship.thrust_x - ship.center_x;
      double y_o = ship.thrust_y - ship.center_y;
      x = x_o * cos(angle_radians) - y_o * sin(angle_radians);
      y = x_o * sin(angle_radians) + y_o * cos(angle_radians);
      ship.thrust_x = x + ship.center_x;
      ship.thrust_y = y + ship.center_y;
      // printf ("%f %f\n",ship.center_x, ship.center_y);
      rotated = 0;
    }

  ship.vel_y += gravity * 0.05;
  for (i = 0; i < 4 ;i++)
    {
      ship.x[i] += (ship.vel_x * 0.05 + xA * 0.05*0.05*0.5);
      ship.y[i] += (ship.vel_y * 0.05 + yA * 0.5*0.05*0.05);
    }
  ship.thrust_x += (ship.vel_x * 0.05 + xA * 0.05*0.05*0.5);
  ship.thrust_y += (ship.vel_y * 0.05 + yA * 0.5*0.05*0.05);

  xA = 0.0; yA = 0.0;

  // check if crashed or landed
  for ( i = 0 ; i < index - 1 ;i++)
    {
      for (j = 0 ; j < 3 ; j++)
	{
	  int intersect =  lineSegmentIntersection(land_x[i],land_y[i],land_x[i+1],land_y[i+1],
						     ship.x[j], ship.y[j], ship.x[j+1], ship.y[j+1]);
	  if (intersect == 1)
	    {
	      running = 0;
	      if (ship.y[2] != ship.y[3])
		crashed = 1;
	      if (land_y[i] != land_y[i+1])
		crashed = 1;
	      if (ship.vel_y > 20)
		crashed = 1;
	      if (crashed == 0)
		landed = 1;
	      struct itimerval timer;
	      // turn the timer off by setting the values to 0/0
	      timer.it_interval.tv_sec = 0;
	      timer.it_interval.tv_usec = 0;
	      timer.it_value.tv_sec = 0;
	      timer.it_value.tv_usec = 0;
	      if( setitimer( ITIMER_REAL, &timer, NULL ) < 0 ) {
		exit( EXIT_FAILURE );
	      }
	     
	    }
	}
    }

  //handle_signal(xA,yA,ship);
  draw(fppo,ship);
  fflush(fppo);
}



int main(int argc, char *argv[])
{
  struct sigaction handler;
  struct itimerval timer;

  int c;
  // handle the input problems
  if (argc < 7)
    {
      printf ("need flags -g gravity, -f landscape.txt, -t thrust\n");
      fprintf (fppo,"end\n");
      exit (EXIT_FAILURE);
    }
  else if (argc > 7)
    { 
      printf ("no non-option arguements allowed\n");
      fprintf (fppo,"end\n");      
      exit (EXIT_FAILURE);
    }
  else
    {
      for (i = 1; i < 7; i++)
	{
	  if (argv[i][0] == '-' && argv[i][1] == 't')
	    {
	      i++;
	      thrust = atof (argv[i]);
	      if ((thrust <= 0) && (thrust >= -20))
		continue;
	      else
		{
		  printf("thrust > 0, < -20 not allowed\n");
		  exit (EXIT_FAILURE);
		}
	    }
	  else if (argv[i][0] == '-' && argv[i][1] == 'g')
	    {
	      i++;
	      gravity = atof (argv[i]);
	      if ((gravity >= 0) && (gravity <= 20))
		continue;
	      else 
		{
		  printf("gravity < 0, > 20 not allowed\n");
		  fprintf (fppo,"end\n");
		  exit (EXIT_FAILURE);
		}
	    } 
	  else if (argv[i][0] == '-' && argv[i][1] == 'f')
	    {
	      i++;
	      fp = fopen (argv[i], "r");
	      if (fp == NULL)
		{
		  printf("could not open %s\n", argv[i]);
		  fclose (fp);
		  exit (EXIT_FAILURE);
		}
	    }
	  else 
	    {
	      printf ("no non-option arguements allowed\n");
	      exit (EXIT_FAILURE);
	    }
	}
    }
  //Start

  ship.x[0]=300;ship.y[0]=10;
  ship.x[1]=310;ship.y[1]=10;
  ship.x[2]=296;ship.y[2]=28;
  ship.x[3]=314;ship.y[3]=28;
  ship.center_x = 305; ship.center_y = 19;
  ship.vel_y = 0; ship.vel_x = 0;
  ship.degree = M_PI/2;
  ship.thrust_x = 305; ship.thrust_y = 34;
  
  // start ncurses
 
  
  init_ncurses();
 
  fppo = popen("java -jar Sketchpad.jar","w");

  mvprintw( 5, 10, "Press any key to start.\n" );
  refresh(); // printed output doesn't appear until we refresh
  c = getch();
  
  nodelay( stdscr, TRUE ); // have getch() return ERR if input isn't ready

  mvprintw( 6, 10, "Press arrow keys, 'q' to quit.\n" );
  refresh();
  

  handler.sa_handler = handle_signal;
  sigemptyset( &handler.sa_mask );
  handler.sa_flags = 0;
  if( sigaction( SIGALRM, &handler, NULL ) < 0 ) {
    exit( EXIT_FAILURE );
  }

  // set up alarm timer
  timer.it_value.tv_sec = 0;
  timer.it_value.tv_usec = 50000;
  timer.it_interval.tv_sec = 0;
  timer.it_interval.tv_usec = 50000;
  if( setitimer( ITIMER_REAL, &timer, NULL ) < 0 ) {
    exit( EXIT_FAILURE );
  }


  landscape(fppo,fp);
  draw(fppo,ship);
  running = 1;
  while( running ) {

    c = getch(); // pause, waiting for a character
    //printf("%f\n",gravity);
    // if( c == ERR ) { continue; } // pause might have been interrupted by alarm
    // set up set of signals to block: start with empty set, add SIGALRM
    
    /*
    sigset_t block_mask;
    sigset_t old_mask;
    sigemptyset( &block_mask );
    sigaddset( &block_mask, SIGALRM );
    if( sigprocmask( SIG_BLOCK, &block_mask, &old_mask ) < 0 ) {
    exit( EXIT_FAILURE );
    }
    */
    // CRITICAL CODE GOES HERE
    // can call other functions here, and they will not be interrupted
    // ...
    // handle the input

    switch( c ) {
    case KEY_LEFT:
      rotated = -1;
      ship.degree -= 10 * M_PI / 180.0;
      break;

    case KEY_RIGHT:
      rotated = 1;
      ship.degree += 10 * M_PI / 180.0;
      break;

    case ' ':
      thrusted = 1;
      xA = thrust * cos(ship.degree);
      yA = thrust * sin(ship.degree);
      ship.vel_x += xA * 0.05;
      ship.vel_y += yA * 0.05;
      break;

    case 'q':
      running = 0;
      break;

    default:
      break;
     
    }
    
    //handle_signal();
    
    /*
    if( sigprocmask( SIG_SETMASK, &old_mask, NULL ) < 0 ) {
      exit( EXIT_FAILURE );
    }
    */
  }

  if (landed == 1)
    mvprintw (7,13,"LANDED !!!!\n");
  if (crashed == 1)
    mvprintw (7,13,"CRASHED !!!!\n");
  
  int end = 0;
  while (!end)
    {
      c = getch();
      switch (c){
      case 'q':
	end = 1;
	break;
	}
    }

  fprintf(fppo,"end\n");
  fclose (fp);
  pclose (fppo);
  shutdown_ncurses();
  return EXIT_SUCCESS;
}
