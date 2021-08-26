#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <libgen.h>

#include <omp.h>


//----------------------------------------
// Complex plane
//----------------------------------------
typedef struct {
  float    *Real;
  float    *Img;
  uint16_t *Divergence;
} C;

//----------------------------------------
// PROTOTYPES
//----------------------------------------
C        ComplexPlane_creator(int); 
uint16_t Divergence_seeker(float, float);

//----------------------------------------
// MAIN
//----------------------------------------
void main()
{
  C        ComplexPlane;
  int      Number_grid, j, k, counter;
  FILE    *fp_tmp1;

  printf("Give the number of intervals for the complex plane grid \n");
  scanf("%i", &(Number_grid));

  ComplexPlane = ComplexPlane_creator(Number_grid);

  counter = 0;


  fp_tmp1 = fopen("Mandelbrot_Set.dat", "w");

#pragma omp parallel for default(none) private(j) shared(k,Number_grid,ComplexPlane, fp_tmp1) 
  for (j=0; j<Number_grid; j++)
    {
    for (k=0; k<Number_grid; k++)
      {
      if (Divergence_seeker(ComplexPlane.Real[k],ComplexPlane.Img[j]) == 0)
        {
        fprintf(fp_tmp1, " %f	%f\n", ComplexPlane.Img[j], ComplexPlane.Real[k]);
        }
      }
    }
  fclose(fp_tmp1);

  free(ComplexPlane.Divergence);


} 



//----------------------------------------
// ComplexPlane_creator()
//----------------------------------------
C  ComplexPlane_creator(int number_grid)
{
  C         ComplexPlane;
  float    *vector_real, *vector_img;
  uint16_t *divergence;
  float     max_real, min_real, max_img, min_img;
  float     interval_real, interval_img;
  int       i;
  
  vector_real = (float *) calloc(number_grid, sizeof(float));
  vector_img  = (float *) calloc(number_grid, sizeof(float));
  divergence  = (uint16_t *) calloc(number_grid*number_grid, sizeof(uint16_t));

//Give the values to the complex plane
  printf("Give the maximum value of the Real axis \n");
  scanf("%f", &(max_real));
  printf("Give the minimum value of the Real axis \n");
  scanf("%f", &(min_real));
  printf("Give the maximum value of the Img axis \n");
  scanf("%f", &(max_img));
  printf("Give the minimum value of the Img axis \n");
  scanf("%f", &(min_img));

  interval_real = (max_real - min_real)/(number_grid*1.0);
  interval_img  = (max_img  - min_img)/(number_grid*1.0);

  for (i=0; i < number_grid; i++)
    {
    vector_real[i] = min_real + i*interval_real;
    vector_img[i]  = min_img + i*interval_img;
    }

  ComplexPlane.Real       = vector_real;
  ComplexPlane.Img        = vector_img;
  ComplexPlane.Divergence = divergence;

  return(ComplexPlane);
}


//----------------------------------------
// Divergence_seeker()
//----------------------------------------
uint16_t  Divergence_seeker(float C_real, float C_img)
{
  float z_real, z_img;
  float z_real_aux, z_img_aux;
  int i;

  z_real = 0.0;
  z_img  = 0.0;

  for (i=0; i < 1000; i++)
    {
    z_real_aux = z_real*z_real - z_img*z_img + C_real;
    z_img_aux  = 2.0*z_real*z_img + C_img;
    z_real     = z_real_aux;
    z_img      = z_img_aux;
    } 
  

  if ((z_real*z_real + z_img*z_img) > 4.0 )
    {
    return(1);
    }
  else if((z_real*z_real + z_img*z_img) !=  (z_real*z_real + z_img*z_img))//for the NaN case
    {
    return(1);
    }
  else
    {
    return(0);
    }
}


