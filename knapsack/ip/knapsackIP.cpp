//para compilar en mac: clang++ -m64 -fPIC -fexceptions -DNDEBUG -stdlib=libc++ -I/Applications/CPLEX_Studio221/cplex/include -I/Applications/CPLEX_Studio221/concert/include  -L/Applications/CPLEX_Studio221/cplex/lib/x86-64_osx/static_pic -L/Applications/CPLEX_Studio221/concert/lib/x86-64_osx/static_pic  -o knapsackIP knapsackIP.cpp -lconcert -lilocplex -lcplex -m64 -lm -lpthread -framework CoreFoundation -framework IOKit -stdlib=libc++
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ilcplex/ilocplex.h>

using namespace std;

typedef struct instance{
  int n;
  int c;
  int* profit;
  int* weight;
} instance;


ILOSTLBEGIN

int * generateIntVector(int n) {
	int *vector;
	vector=(int* )malloc(sizeof(int)*n);
	return vector;
}

int readFile(instance* I,char* filename) {
  FILE* f;
  int uno,dos;

  f=fopen(filename,"rt");
  fscanf(f,"%d %d\n",&uno,&dos);
  I->n=uno;
  I->c=dos;
  I->profit=generateIntVector(I->n);
  I->weight=generateIntVector(I->n);
  for(int i=0;i<I->n;i++){
    fscanf(f,"%d %d\n",&(I->weight[i]),&(I->profit[i]));
  } 
  fclose(f);
  return(0);
}

int solveKnapsackIp(instance * I) {

  IloEnv env;
  IloModel model(env);
  

  IloNumVarArray x(env, I->n, 0, 1, ILOINT);

  IloExpr objective(env);
  for(int i=0;i<I->n;i++) {
    objective += I->profit[i]*x[i];
  } 
  model.add (IloMaximize(env, objective));
  objective.end();
  IloExpr expr(env);
  for(int i=0;i<I->n;i++) {
    expr += I->weight[i]*x[i];
  } 
  model.add(expr<=I->c);
  expr.end();

  IloCplex cplex(model);
  cplex.setOut(env.getNullStream());
  cplex.setWarning(env.getNullStream());
  cplex.solve();

  if (cplex.getStatus() == IloAlgorithm::Infeasible)
     printf("error\n");

  cout << "Solution status: " << cplex.getStatus() << endl;
  for(int i=0;i<I->n;i++) {
    if(cplex.getValue(x[i])>0.99) {
      cout << "Uses " << i << " profit: " << I->profit[i] << " weight " << I->weight[i] << endl;
    }
  }
  cout << "Beneficio = " << cplex.getObjValue() << endl;
  env.end();
  return(0);
}

int main(int argc, char* argv[]) {

instance I;

  if(argc<2) {
    printf("falta nombre del archivo\n");
    return(0);
  }
  //imprimimos el contenido por seguridad
  readFile(&I,argv[1]);
  printf("n: %d c: %d\n",I.n,I.c);
  solveKnapsackIp(&I);

  return(0); //todo ha salido bien
}
