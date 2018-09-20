#include <stdlib.h>
#include <stdio.h>

int main(void)
{
  const int dimx = 1920, dimy = 1080;
  int max = 1000;
  int i, j;

        const double CxMin=-2.5;
        const double CxMax=1.5;
        const double CyMin=-2.0;
        const double CyMax=2.0;

        double PixelWidth=(CxMax-CxMin)/dimx;
        double PixelHeight=(CyMax-CyMin)/dimy;

  FILE *fp = fopen("mandel_sequential.ppm", "wb"); /* b - binary mode */
  (void) fprintf(fp, "P6\n %d\n %d\n 255\n", dimx, dimy);
  for (j = 0; j < dimy; ++j)
  {
    for (i = 0; i < dimx; ++i)
    {
                static unsigned char color[3];

                double c_re=CxMin + i*PixelWidth;
                double c_im=CyMin + j*PixelHeight;

                double x = 0, y = 0;
                int iterations = 0;
                while (x*x+y*y < 4 && iterations < max) {
                    double x_new = x*x-y*y+c_re;
                    y = 2*x*y+c_im;
                    x = x_new;
                    iterations++;
                }
                if (iterations < max)
                {
                    color[0] = 255;
                    color[1] = 255;
                    color[2] = 255;
                    (void) fwrite(color, 1, 3, fp);
                }
                else
                {
                    color[0] = 0;
                    color[1] = 0;
                    color[2] = 0;
                    (void) fwrite(color, 1, 3, fp);
                }

    }
  }
  (void) fclose(fp);
  return EXIT_SUCCESS;
}
