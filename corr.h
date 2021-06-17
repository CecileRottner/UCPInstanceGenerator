#ifndef CORRH
#define CORRH


using namespace std ;

double** Chol(double coef) ;

//Génère m séquences corrélées de taille n, selon la corrélation induite par la matrice T = C'C
double** genCorrChol(double** C, int m, int n) ;

//Génère 2 séquences de taille n, de coefficient  de corrélation coef
void genCorrCoef(double* serie1, double* serie2, double coef, int n, int l1, int l2) ;
//mat : matrice de Choleski de la matrice de corrélations
// m : taille de mat


void correle2a2(double* vect, double* result, double coef, int l1, int l2) ;

void genFirst(double* serie, int n_type, int unit_type, int l1, int l2) ;
//Génère Pmax entre 0 et 1
//unit_type : type d'unité'
//0 : charbon
// 1 : Fioul
//2 : TAC
//ntype : nombre d'unités du type

#endif
