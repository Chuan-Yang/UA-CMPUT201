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
int rotated = 0;
double angle_radians=0.0;

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
	//fprintf (fppo, "pause 1\n");
   //fprintf(fppo,"end\n");
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
  for (i = 0; i < 4 ;i++)
    {
      ship.x[i] += (ship.vel_x * 0.05 + xA * 0.05*0.05*0.5);
      ship.y[i] += (ship.vel_y * 0.05 - yA * 0.5*0.05*0.05);
    }
  xA = 0.0; yA = 0.0;

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
      rotated = 0;
    }
  ship.vel_y += gravity * 0.05;
  //handle_signal(xA,yA,ship);
  draw(fppo,ship);
  fflush(fppo);
}



int main(int argc, char *argv[])
{
  struct sigaction handler;
  struct itimerval timer;

  int running, c;
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

  
  // start ncurses
 
  
  init_ncurses();
 
  fppo = popen("java -jar Sketchpad.jar","w");

  mvprintw( 5, 10, "Press any key to start." );
  refresh(); // printed output doesn't appear until we refresh
  c = getch();
  
  nodelay( stdscr, TRUE ); // have getch() return ERR if input isn't ready

  mvprintw( 6, 10, "Press arrow keys, 'q' to quit." );
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

  /*
  handler.sa_handler = handle_signal;
  sigemptyset( &handler.sa_mask );
  handler.sa_flags = 0;
  if( sigaction( SIGALRM, &handler, NULL ) < 0 ) {
    exit( EXIT_FAILURE );
  }
  */
  


  thrust = -thrust;

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
      ship.degree -= 10 * M_PI / 180.;
      break;

    case KEY_RIGHT:
      rotated = 1;
      ship.degree += 10 * M_PI / 180.;
      break;

    case ' ':
      xA = thrust * cos(ship.degree);
      yA = thrust * sin(ship.degree);
      ship.vel_x += xA * 0.05;
      ship.vel_y -= yA * 0.05;
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



  fprintf(fppo,"end\n");
  fclose (fp);
  pclose (fppo);
  shutdown_ncurses();
  return EXIT_SUCCESS;
}
