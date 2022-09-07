#include <stdio.h> //include para imprimir
#include <stdlib.h>
#define SIZE 5 //una constante que es igual a 7

int x[SIZE]; //definición de variables. (declaración como variable global)

int branch(int profundidad) {
  char letra;
  for(int i=0;i<SIZE;i++) { //miro todas las variables
    if(x[i]>=0) printf("%d ",x[i]); //si tiene valor >=0, la imprimo
    else printf(". "); //si vale -1 imprimo un punto
  }
  printf(" p: %2d\t",profundidad); //que imprima la profundidad
  if(profundidad==SIZE) {  //si he llegado a la profundidad máxima (SIZE)
    printf("FIN\n"); //imprima que ya tengo solucion completa
    scanf("%c",&letra);
    return(0); //que salga del procedimiento
  } else {
    printf("\n"); //que imprima un final de línea (para que quede bonito)
  }
  scanf("%c",&letra);
  //arco que asigna x a 1
  x[profundidad]=1; 
  branch(profundidad+1);
  printf("saliendo prof. %d val 1\n",profundidad);
  scanf("%c",&letra);
  //arco que asigna x a 0
  x[profundidad]=0;
  branch(profundidad+1);
  printf("saliendo prof. %d val 0\n",profundidad);
  scanf("%c",&letra);
  //reseando la variable x
  x[profundidad]=(-1);
  return(0);
}

int main(void) {
  for(int i=0;i<SIZE;i++) x[i]=(-1); //inicializo todas las variables a -1
  branch(0);
  return(0);
}
