//para compilar en mac: 
//clang++ -m64 -fPIC -fexceptions -DNDEBUG -stdlib=libc++ -I/Applications/CPLEX_Studio221/cplex/include -I/Applications/CPLEX_Studio221/concert/include  -L/Applications/CPLEX_Studio221/cplex/lib/x86-64_osx/static_pic -L/Applications/CPLEX_Studio221/concert/lib/x86-64_osx/static_pic  -o knapsackIP knapsackIP.cpp -lconcert -lilocplex -lcplex -m64 -lm -lpthread -framework CoreFoundation -framework IOKit -stdlib=libc++
//para compilar en linux:
//g++ -m64 -fPIC -fno-strict-aliasing -fexceptions -DNDEBUG -I/opt/ibm/ILOG/CPLEX_Studio221/cplex/include -I/opt/ibm/ILOG/CPLEX_Studio221/concert/include  -L/opt/ibm/ILOG/CPLEX_Studio221/cplex/lib/x86-64_linux/static_pic -L/opt/ibm/ILOG/CPLEX_Studio221/concert/lib/x86-64_linux/static_pic  knapsackIP.cpp -lconcert -lilocplex -lcplex -lm -lpthread -ldl

//definir los archivos que se van a utilizar (equivalente en python a import)
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ilcplex/ilocplex.h> //esta línea sólo si uso cplex
                              
using namespace std;

//Definir las estructuras de datos que se van a utilizar y aquellas "utilidades"
//Propias de las librerías
// Voy a crear una estructura para guardar todos los datos asociados a una instancia
typedef struct instance{ //me voy a crear un nuevo tipo de datos llamado instancia
  int n; //número entero que es el número de items
  int c; //número entero que es la capacidad
  int* profit; //int* significa puntero --> vector (beneficio)
  int* weight; //int* significa puntero --> vector (peso)
} instance;

ILOSTLBEGIN //requerido por cplex, si no, no funciona

//Definir funciones
int * generateIntVector(int n) {
	int *vector;
	vector=(int* )malloc(sizeof(int)*n);
	return vector;
}

//Función 1.-> LEER DATOS
int readFile(instance* I,char* filename) {
  FILE* f;
  int uno;
  int dos;
  int i; //quiero una variable que será de tipo entero y que llamaré i.

  printf("voy a leer archivo %s\n",filename);
  f=fopen(filename,"rt");
  fscanf(f,"%d %d\n",&uno,&dos); //%d -> número entero
  I->n=uno;
  I->c=dos;
  printf("numero de objetos: %d\n",I->n);
  printf("capacidad: %d\n",I->c);
  //como ya sé el número de items, puedo pedir al programa que genere un vector
  //de dimensión n para profit y para weight
  I->profit=generateIntVector(I->n); //va a avisar que tengo n items para gestionar profit
  I->weight=generateIntVector(I->n); //va a avisar que tengo n items para gestionar weight
  for(i=0;i<I->n;i++) { //voy a leer los dos numeritos
    fscanf(f,"%d %d\n",&uno,&dos);
    I->weight[i]=uno;
    I->profit[i]=dos;
  }
  //for(i=0;i<I->n;i++) {
  //  printf("el item %d tiene peso %d y beneficio %d\n",i,I->weight[i],I->profit[i]);
  //}
  fclose(f);
  return(0);
}

//Función 2.-> Construir el programa lineal entero, resolverlo y mostrar la solución
int solveKnapsackIP(instance* I) {
  printf("dentro de la funcion solve, numero de items es %d\n",I->n);
  //cplex requiere de un entorno, de un modelo y de un solver
  IloEnv env; //construye en entorno
  IloModel model(env); //construye el modelo, le llama model y lo liga al entorno

  //definir variables
  IloNumVarArray x(env, I->n, 0, 1, ILOINT); //quiero un vector de variables 
                                             //enteras que pueden tomar valor 0 o 1

  //definir función objetivo
  IloExpr objective(env); //creo una expresión que se llama objective
  for(int i=0;i<I->n;i++) { //paso por todas las variables
    objective += I->profit[i]*x[i]; //y añado a la expresión profit*x
  } 
  //al salir del for objective tiene la función desarrollada y completa
  model.add (IloMaximize(env, objective)); //añade esa función al modelo (como max)
  objective.end(); //la borramos
  
  //definir restricciones
  //a) IloExpr de restricción
  IloExpr expr(env);
  //b) for donde se construye el lado izquierdo weight[i]*x[i]
  for(int i=0;i<I->n;i++) { //es equivalente a un sumatorio
    expr += I->weight[i]*x[i]; // lo que hace que sea equivalente a un sumatorio +=
  } 
  //c) model.add ( restriccion <= I->c)
  model.add(expr<=I->c);
  //d) end de IloExpr de restricción
  expr.end();
  //solver y resolver
  IloCplex cplex(model); //quiero un solver al que llamo cplex que resuelve model
  cplex.setOut(env.getNullStream());
  cplex.setWarning(env.getNullStream());
  cplex.solve(); //resolver el modelo
  //vamos a pedir a cplex que nos imprima el valor de la solución y de las variables
  cout << "Beneficio = " << cplex.getObjValue() << endl;
  for(int i=0;i<I->n;i++) { //vamos a mirar todas las variables
    if(cplex.getValue(x[i])>0.99) { //aquellas con valor igual a 1 son solución.
      cout << "Uses " << i << " profit: " << I->profit[i] << " weight " << I->weight[i] << endl;
    }
  }

  env.end(); //decirle a cplex que no quiero usar más el modelo ni el entorno
  return(0);
}

//La función de entrada en el programa (función main)
//argc -> número de parámetros de entrada
//argv[] -> cada parámetro
int main(int argc, char* argv[]) { //entra en el programa y pásale los datos
  instance I;
  printf("Entro con nombre de fichero: %s\n",argv[1]); //parámetro 1: Nombre archivo
  //C pasa todos los datos a función como un valor, implica que no los cambia una
  //vez que sale
  //Para pasar una variable y que se modifique dentro de la función, hay que pasar
  //la variable por referencia (la forma de hacerlo es usar la pareja & y *)
  readFile(&I,argv[1]);
  solveKnapsackIP(&I);
  free(I.profit);
  free(I.weight);
  return(0);
}
