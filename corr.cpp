#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <iostream>

#include "corr.h"


using namespace std ;

double phi(double x)
{
    // constants
    double a1 =  0.254829592;
    double a2 = -0.284496736;
    double a3 =  1.421413741;
    double a4 = -1.453152027;
    double a5 =  1.061405429;
    double p  =  0.3275911;

    // Save the sign of x
    int sign = 1;
    if (x < 0)
        sign = -1;
    x = fabs(x)/sqrt(2.0);

    // A&S formula 7.1.26
    double t = 1.0/(1.0 + p*x);
    double y = 1.0 - (((((a5*t + a4)*t) + a3)*t + a2)*t + a1)*t*exp(-x*x);

    return 0.5*(1.0 + sign*y);
}

double** produitMat(double** A, double** B, int a, int b) {
    // A matrice carrée de taille a
    // B matrice de taille axb
    double** result = new double*[a] ;
    for (int j = 0 ; j < a ; j++) {
        result[j] = new double[b];
    }

    for (int i = 0 ; i < a ; i++) {
        for (int j = 0 ; j < b ; j++) {
            result[i][j] = 0 ;
            for (int k = 0 ; k < a ; k ++) {
                result[i][j] += A[i][k]* B[k][j];
            }
        }
    }

    return result ;
}

double** genCorrChol(double** C, int m, int n) {

    double** Znorm = new double*[m] ;
    for (int j = 0 ; j < m ; j++) {
        Znorm[j] = new double[n];
    }

    for (int j = 0 ; j <m ; j++) {
        for (int i = 0 ; i < n ; i++) {
            double U = (double) (rand()/(double) RAND_MAX) ;
            double V = (double) (rand()/(double) RAND_MAX);
            Znorm[j][i] = sqrt(-2*log(U))*cos(2*M_PI*V) ;
        }
    }

    double** Z = produitMat(C, Znorm, m, n) ;

    for (int j = 0 ; j <m ; j++) {
        for (int i = 0 ; i < n ; i++) {
            Z[j][i] = phi(Z[j][i]) ;
        }
    }



    return Z;


}

double** Chol(double coef) {
    double c = 2*sin(M_PI*coef/6) ;
    double** chol = new double*[2] ;
    chol[0] = new double[2];
    chol[1] = new double[2];
    chol[0][0] = 1 ;
    chol[0][1] = 0 ;
    chol[1][0] = c ;
    chol[1][1] = sqrt(1-c*c);

    return chol ;
}

void genCorrCoef(double* serie1, double* serie2, double coef, int n, int l1, int l2) {
    double** C = Chol(coef) ;
    double** result = genCorrChol(C, 2, n) ;

    for (int i = l1 ; i <= l2 ; i++) {
        serie1[i] = result[0][i] ;
        serie2[i] = result[1][i] ;
    }
}

void correle2a2(double* vect, double* result, double coef, int l1, int l2) {
    int signe = 0 ;
    int c = coef ;
    if (coef <= 0) {
        signe = 1 ;
        c = -coef ;
    }

    for (int j=l1 ; j <= l2 ; j++) {


        double r = ((double) (rand()/(double) RAND_MAX) / 10.0) ;
        r =r*(1-c) ;
        r = r - (1-c)/2 ;
        result[j] = fmin(fmax(vect[j] + r, 0), 1) ;

        if (signe==1) {
            result[j] = 1 - result[j];
        }
    }
}

void genFirst(double* serie, int n_type, int unit_type, int l1, int l2) {
    int powers ;
    if (unit_type == 0) {
        powers = n_type/10 + 2 ;
    }
    if (unit_type == 1) {
        powers = n_type - n_type/5 ;
    }
    if (unit_type == 2) {
        powers = n_type/10 + 3 ;
    }

    double* pmax = new double[powers] ;
    for (int i = 0 ; i < powers ; i++) {
        pmax[i] = (double) (rand()/(double) RAND_MAX) ;
    }

    for (int i = l1 ; i <= l2 ; i++) {
        int random = rand()%(powers) ;
        serie[i] = pmax[random] ;
    }
}

