//para compilar en mac: clang++ -m64 -fPIC -fexceptions -DNDEBUG -stdlib=libc++ -I/Applications/CPLEX_Studio221/cplex/include -I/Applications/CPLEX_Studio221/concert/include  -L/Applications/CPLEX_Studio221/cplex/lib/x86-64_osx/static_pic -L/Applications/CPLEX_Studio221/concert/lib/x86-64_osx/static_pic  -o bppIP bppIP.cpp -lconcert -lilocplex -lcplex -m64 -lm -lpthread -framework CoreFoundation -framework IOKit -stdlib=libc++
//para compilar en linux:
//g++ -m64 -fPIC -fno-strict-aliasing -fexceptions -DNDEBUG -I/opt/ibm/ILOG/CPLEX_Studio221/cplex/include -I/opt/ibm/ILOG/CPLEX_Studio221/concert/include  -L/opt/ibm/ILOG/CPLEX_Studio221/cplex/lib/x86-64_linux/static_pic -L/opt/ibm/ILOG/CPLEX_Studio221/concert/lib/x86-64_linux/static_pic  bppIP.cpp -lconcert -lilocplex -lcplex -lm -lpthread -ldl

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ilcplex/ilocplex.h>

using namespace std;

typedef struct instance{
  int n;
  int c;
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

  f=fopen(filename,"rt");
  fscanf(f,"%d %d\n",&(I->n),&(I->c));
  I->weight=generateIntVector(I->n);
  for(int i=0;i<I->n;i++){
    fscanf(f,"%d\n",&(I->weight[i]));
  } 
  fclose(f);
  return(0);
}

int solveBppIp(instance * I) {

  IloEnv env;
  IloModel model(env);
  

  IloNumVarArray y(env, I->n, 0, 1, ILOINT);
  //https://www.ibm.com/support/pages/sample-create-and-use-multi-dimensional-ilonumvararray
  IloArray<IloNumVarArray> x(env,I->n);
  for (int i = 0; i < I->n; i++)
    x[i] = IloNumVarArray(env, I->n, 0, 1, ILOINT);

  IloExpr objective(env);
  for(int i=0;i<I->n;i++) {
    objective += y[i];
  } 
  model.add (IloMinimize(env, objective));
  objective.end();
  for(int j=0;j<I->n;j++) { //una restricción para cada bin
    IloExpr expr(env);
    for(int i=0;i<I->n;i++) {
      expr += I->weight[i]*x[i][j];
    } 
    model.add(expr<=I->c*y[j]);
    expr.end();
  }
  for(int i=0;i<I->n;i++) {
    IloExpr expr(env);
    for(int j=0;j<I->n;j++) {
      expr += x[i][j];
    }
    model.add(expr==1);
    expr.end();
  }

  IloCplex cplex(model);
  //cplex.setOut(env.getNullStream());
  //cplex.setWarning(env.getNullStream());
  cplex.solve();

  if (cplex.getStatus() == IloAlgorithm::Infeasible)
     printf("error\n");

  cout << "Solution status: " << cplex.getStatus() << endl;
  for(int i=0;i<I->n;i++) {
    for(int j=0;j<I->n;j++) {
      if(cplex.getValue(x[i][j])>0.99) {
        cout << "Item " << i << " bin " << j << endl;
      }
    }
  }
  cout << "numBins = " << cplex.getObjValue() << endl;
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
  solveBppIp(&I);
  //liberar memoria
  free(I.weight);

  return(0); //todo ha salido bien
}
