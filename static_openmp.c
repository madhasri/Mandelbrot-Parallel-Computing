/* Program for Mandelbrot Set - Static Assignment (in openmp) */

# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <time.h>
# include <omp.h>



/* function declaration */

int main ( void );
int min ( int i1, int i2 );



/* Main Mandelbrot Code */


int main ( void )

{
  
  int m = 500;
  int n = 500;
  int count_max = 256;
  double x_max =  1.25;
  double x_min = -2.25;
  double y_max =  1.75;
  double y_min = -1.75;
  char *output_filename = "mandelbrot_set.ppm";
  FILE *file;

  int count[m][n], r[m][n], g[m][n], b[m][n];
  int c,thread_num,num_threads,start,end;
  int i,j,k,j1,j2;
  int ierror;
  double x,y;
  double x1,y1;
  double x2,y2;
  
 
  printf (" \n\n For each complex point C = X + i*Y , with X range [%g,%g] and Y range [%g,%g] carry out %d iterations of the map\n", x_min,    x_max,  y_min, y_max, count_max );
  
  printf (" M = %d pixels in the X direction and N = %d pixels in the Y direction.\n", m ,n);





# pragma omp parallel \
  shared ( b, count, count_max, g, r, x_max, x_min, y_max, y_min ) \
  private ( thread_num,num_threads,start,end, i, j, k, x, x1, x2, y, y1, y2 )
{
   thread_num = omp_get_thread_num();
    num_threads = omp_get_num_threads();
    start = thread_num * m / num_threads;
    end = (thread_num + 1) * m / num_threads;


  for (i = start; i != end; ++i) 
     
  {

    printf("%d %d\n", thread_num, i);
    for ( j = 0; j < n; j++ )
    {
      x = x_min + ( (double) j * ( (x_max - x_min) / (m) ));

     y = y_min + ((double) i * ( (y_max - y_min) / (n) ));

      count[i][j] = 0;

      x1 = x;
      y1 = y;

      for ( k = 1; k <= count_max; k++ )
      {
        x2 = x1 * x1 - y1 * y1 + x;
        y2 = 2 * x1 * y1 + y;

        if ( x2 < -2.0 || 2.0 < x2 || y2 < -2.0 || 2.0 < y2 )
        {
          count[i][j] = k;
          break;
        }
        x1 = x2;
        y1 = y2;
      }



      if ( ( count[i][j] % 2 ) == 1 )
      {
        r[i][j] = 255;
        g[i][j] = 255;
        b[i][j] = 255;
      }
      else
      {
        c = ( int ) ( 255.0 * sqrt ( sqrt ( sqrt ( 
          ( ( double ) ( count[i][j] ) / ( double ) ( count_max ) ) ) ) ) );
        r[i][j] = 3 * c / 5;
        g[i][j] = 3 * c / 5;
        b[i][j] = c;
      }
    }
  }

}

//the array - count

for(i=0;i<m;i++)
 {
  for(j=0;j<n;j++)
      printf(" %d ", count[i][j]);

printf("\n");
}


/*
  Write data to an ASCII PPM file.
*/

  file = fopen ( output_filename, "wt" );

  fprintf ( file, "P3\n" );
  fprintf ( file, "%d  %d\n", n, m );
  fprintf ( file, "%d\n", 255 );
  for ( i = 0; i < m; i++ )
  {
    for ( j1 = 0; j1 < n; j1 = j1 + 4 )
    {
      j2 = min ( j1 + 4, n );
      for ( k = j1; k < j2; k++ )
      {
        fprintf ( file, "  %d  %d  %d", r[i][j], g[i][j], b[i][j] );
      }
      fprintf ( file, "\n" );
    }
  }

  fclose ( file );
 
  return 0;
}



int min ( int a, int b )

{
  int min;

  if ( a < b )
  {
    min = a;
  }
  else
  {
    min = b;
  }
  return min;
}
