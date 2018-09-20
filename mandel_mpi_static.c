#include <mpi.h>
#include<math.h>
#include <stdio.h>
#include<stdlib.h>

#define WORK_TAG 1
int max_iter = 10;


struct complex{
float real;
float imag;
}c;




int cal_pixel( struct complex c)
{

int count;
struct complex z;
float temp,lengthsq;

z.real = 0.0;
z. imag =0.0;
count= 0;
do
   {
     temp = ((z.real)*(z.real))-((z.imag)*(z.imag)) + (c.real);
     z.imag = 2*(z.real)*(z.imag) + (c.imag);
     z.real = temp;
     lengthsq = (z.real)*(z.real) + (z.imag)*(z.imag);
     count++;
    } while((lengthsq < 4.0) && (count < max_iter));
//printf("\n %d" , count);
  return count;

}

int i4_min ( int i1, int i2 )

{
  int value;

  if ( i1 < i2 )
  {
    value = i1;
  }
  else
  {
    value = i2;
  }
  return value;
}

 
int main (int argc, char* argv[])
{
  
  
  //long** color = NULL;
  int i,j;
  int real_max = 2;
  int imag_max = 2;
int real_min = -2;
int imag_min = -2;
 int disp_width =500;
 int disp_height = 500;
  //int matrix[20][20];
  int matrix[500][500],red[500][500],green[500][500],blue[500][500];
  int rank,size,k,count,row,x,y,slave,r,source_tag;
  int *color = malloc((disp_width+1) * sizeof(int));
  float scale_real = ((float)(real_max-real_min))/((float)disp_width);
  float scale_imag = ((float)(imag_max-imag_min))/((float)disp_height);  
  
   /*for(i=0;i<disp_width;i++)
   printf("%d " , color[i]);*/
FILE* output_unit;
 FILE* fp = fopen("output_file2" , "w");
 
  MPI_Init (&argc, &argv);      /* starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);        /* get current process id */
  MPI_Comm_size (MPI_COMM_WORLD, &size);        /* get number of processes */
  MPI_Status status;

//master
if(rank==0)
{
   for(i=0,row=0;i<size-1;i++, row=row+5) /* for each process */
     MPI_Send(&row,1,MPI_INT,i+1,WORK_TAG,MPI_COMM_WORLD);             /*  send(&row,Pi); send row number */
   /*for(i=0;i<(480*640);i++)  */  
           /* from processes, any order */

     for(j=0;j<disp_height;j++)
      { 
           MPI_Recv(color,disp_width+1,MPI_INT,MPI_ANY_SOURCE,WORK_TAG,MPI_COMM_WORLD,&status);
           //printf("\n");
           int row_num= color[0];
           for(i=0;i<disp_width;i++) 
            { 
                  /*  recv(&c,&color,Pany);  recieve coordinates/colors */
        
                  /* display pixel on screen */
                  matrix[row_num][i] = color[i+1]-1;
                 //printf("%d  " , color[i]);
             
            }
      }
   
}

//slave

else
 {
    MPI_Recv(&row,1,MPI_INT,0,WORK_TAG,MPI_COMM_WORLD,&status); /*recv(&row,Pmaster)*/
    
     
    for(y=row;y<(row+5);y++)
       {  
          color[0]=y;
          for(x=1; x<=disp_width;x++)
               
        
             {
              
              c.real = real_min  + ((float)x * scale_real);
              c.imag = imag_min + ((float)y * scale_imag);
              color[x] =cal_pixel(c);
               /*send(&c,&color,Pmaster);*/
             }
          MPI_Send(color, disp_width+1, MPI_INT, 0,WORK_TAG,MPI_COMM_WORLD);
       }    
}


if(rank==0)
{
int c_1;






for(i=0;i<500;i++)
{
fprintf(fp,"\n");
for(j=0;j<500;j++)
{
fprintf(fp, "%d " ,  matrix[i][j]);
if ( ( matrix[i][j+1] % 2 ) == 1 )
      {
        red[i][j] = 255;
        green[i][j] = 255;
        blue[i][j] = 255;
      }
      else
      {
        c_1 = ( int ) ( 255.0 * sqrt ( sqrt ( sqrt ( 
          ( ( double ) ( matrix[i][j] ) / ( double ) ( max_iter ) ) ) ) ) );
        red[i][j] = 3 * c_1 / 5;
        green[i][j] = 3 * c_1 / 5;
        blue[i][j] = c_1;
      }
}
}

int n = 500;
int m = 500;
int jlo;
int jhi;
output_unit = fopen("image_op" , "w");
fprintf ( output_unit, "P3\n" );
  fprintf ( output_unit, "%d  %d\n", n, m );
  fprintf ( output_unit, "%d\n", 255 );
  for ( i = 0; i < n; i++ )
  {
    for ( jlo = 0; jlo < m; jlo = jlo + 4 )
    {
      jhi = i4_min ( jlo + 4, n );
      for ( j = jlo; j < jhi; j++ )
      {
        fprintf ( output_unit, "  %d  %d  %d", red[i][j], green[i][j], blue[i][j] );
      }
      fprintf ( output_unit, "\n" );
    }
  }

}

MPI_Finalize();
fclose(fp);
fclose(output_unit);

/*
for(i=0;i<20;i++)
{
fprintf(fp,"\n");
for(j=0;j<20;j++)
{
fprintf(fp, "%d " ,  matrix[i][j]);
}
}

}
MPI_Finalize();
fclose(fp);*/

  return 0;
}






