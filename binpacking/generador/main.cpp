#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int seed;

double r8_uniform_01 (  ) {
  int k;
  double r;

  k = seed / 127773;

  seed = 16807 * ( seed - k * 127773 ) - k * 2836;

  if ( seed < 0 )
  {
    seed = seed + 2147483647;
  }
	/*
  Although SEED can be represented exactly as a 32 bit integer,
  it generally cannot be represented exactly as a 32 bit real number!
	*/
  r = ( double ) ( seed ) * 4.656612875E-10;
  return r;
}

int r8_int (int lower,int upper) {
  return (lower+floor(r8_uniform_01()*(double)(upper-lower+1)) );
}


//
// 1-> numero de items
// 2-> rango de pesos (entre 1 y ese valor, uniforme)
// 3-> rango de beneficios (entre 1 y ese valor, uniforme)
// 4-> capacidad
// 5-> nombre del archivo de salida
// 6-> semilla aleatoria
int main(int argc, char* argv[]) {
int n,c,minWeight,maxWeight;
FILE* f;

  if(argc<7) {
    printf("faltan parametros\n");
    printf(" 1-> numero de items\n 2-> peso minimo\n 3-> peso máximo\n 4-> capacidad\n 5-> nombre del archivo de salida\n 6-> semilla aleatoria\n"); 
    return(0);
  }
  n=atoi(argv[1]);
  minWeight=atoi(argv[2]);
  maxWeight=atoi(argv[3]);
  c=atoi(argv[4]);
  f=fopen(argv[5],"wt");
  seed=atoi(argv[6]);
  fprintf(f,"%d %d\n",n,c);
  for(int i=0;i<n;i++) { 
    fprintf(f,"%d\n",r8_int(minWeight,maxWeight));
  }
  fclose(f);
}
