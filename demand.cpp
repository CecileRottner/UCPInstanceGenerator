#include "demand.h"

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <iostream>


using namespace std ;


//met à jour les paramètres de la saison ;
void setParam(int saison, double & b1, double & b2, double & b3, double & h1, double & h2) {

    double min1 ;
    double min11 ;

    double min2 ;
    double min22 ;

    if (saison == 0) {


        b1 = (double) (rand()/(double) RAND_MAX)*8 + 1 ;
        b2 = (double) (rand()/ (double) RAND_MAX)*17 + 4 ;
        b3 = (double) (rand()/(double) RAND_MAX)*15 + 13 ;

        min1 = fmax(b1, b2) ;
        min11 = fmax(min1, 14) ;

        h1 = (double) (rand()/(double) RAND_MAX)*(33 - min11) + min11 ;

        min2 = fmax(b3, b2) ;
        min22 = fmax(min2, 25) ;

        h2 = (double) (rand()/(double) RAND_MAX)*(44 - min22) + min22 ;


    }

    //paramètres mi-saison
    if (saison == 1) {


        b1 = (double) (rand()/(double) RAND_MAX)*12 + 1 ;
        b2 = (double) (rand()/(double) RAND_MAX)*4 + 6 ;
        b3 = (double) (rand()/(double) RAND_MAX)*4 + 10 ;

        min1 = fmax(b1, b2) ;
        min11 = fmax(min1, 11) ;

        h1 = (double) (rand()/(double) RAND_MAX)*(17 - min11) + min11 ;

        min2 = fmax(b3, b2) ;
        min22 = fmax(min2, 11) ;

        h2 = (double) (rand()/(double) RAND_MAX)*(18 - min22) + min22 ;

    }

    //paramètres et calcul été
    if (saison == 2) {


        b1 = (double) (rand()/(double) RAND_MAX)*7 + 1 ;
        b2 = (double) (rand()/(double) RAND_MAX)*7 + 1 ;

        min1 = fmax(b1, b2) ;
        min11 = fmax(min1, 3) ;

        h1 = (double) (rand()/(double) RAND_MAX)*(13 - min11) + min11 ;

    }


    //paramètres et calcul demande littérature
    if (saison == 3) {


        b1 = (double) (rand()/(double) RAND_MAX)*10 + 20 ;
        b2 = (double) (rand()/(double) RAND_MAX)*6 + 47 ;
        b3 = (double) (rand()/(double) RAND_MAX)*10 + 20 ;

        h1 = (double) (rand()/(double) RAND_MAX)*(10) + 80 ;
        h2 = (double) (rand()/(double) RAND_MAX)*(10) + 70 ;


    }
}


double* demand24(int pas, int saison, double b1, double b2, double b3, double h1, double h2) {
    //simule demande sur 24 heures

    int T = 24 ;
    int amplit ;

    double c ;
    double c2 ;


    int s1 ;
    int t1 ;
    int s2 ;
    int t2 ;
    int s3 ;
    int t3 ;
    int s4 ;
    int t4 ;
    int s5 ;
    int t5 ;



    double* points = new double[T*pas] ;
    for (int i=0 ; i < T*pas ; i++) {
        points[i] = 0 ;
    }

    if (saison == 0) {
        c= 4 ;
        c2=0.5 ;

        s1 = 0 ;
        t1 = 5 ;

        s2 = 5 ;
        t2 = 10 ;

        s3 = 10 ;
        t3 = 13 ;

        s4 = 13;
        t4 = 16 ;

        s5 = 16 ;
        t5 = 21 ;

        amplit = 10 ;
    }

    if (saison == 1) {
        c = 4 ;
        c2 = 0.5 ;

        s1 = 0 ;
        t1 = 4 ;

        s2 = 4 ;
        t2 = 10 ;

        s3 = 10 ;
        t3 = 14 ;

        s4 = 14;
        t4 = 17 ;

        s5 = 17 ;
        t5 = 22 ;

        amplit = 5 ;
    }

    //Calcul et paramètres été
    if (saison == 2) {
        c = 4 ;
        c2 = 0.5 ;

        s1 = 0 ;
        t1 = 4 ;

        s2 = 4 ;
        t2 = 10 ;

        s3 = 10 ;
        t3 = 14 ;


        for (int i = 0 ; i < t1*pas ; i++) {
            double x = (double) i / pas ;
            double y_rand = (double) (rand()/ (double) RAND_MAX) ;
            points[i] = b1 + (h1 - b1)*exp(-(x-t1)*(x-t1)/(c))  + y_rand*2 - 1 ;
        }

        for (int i = s2*pas ; i < t2*pas ; i++) {
            double x = (double) i / pas ;
            double y_rand = (double) (rand()/ (double) RAND_MAX) ;
            points[i] = h1 + y_rand*2 - 1 ;
        }

        for (int i = s3*pas ; i < T*pas ; i++) {
            double x = (double) i / pas ;
            double y_rand = (double) (rand()/ (double) RAND_MAX) ;
            points[i] = b2 + (h1 - b2)*exp(-(x-s3)*(x-s3)/(c)) + y_rand*2 - 1;
        }

    }

        //paramètres et calcul demande littérature
    if (saison == 3) {
        s1 = 0 ;
        t1 = 12 ;

        s2 = 12 ;
        t2 = 17 ;

        s3 = 17 ;
        t3 = 20 ;

        s4 = 20;
        t4 = 24 ;

        for (int i = s1 ; i < t1*pas ; i++) {
            double x = (double) i / pas ;
            double y_rand = (double) (rand()/ (double) RAND_MAX) ;
            double terme = ((h1 - b1)*x/ t1 ) ;
            points[i] = b1 + terme + y_rand*4 - 2 ;
        }

        for (int i = s2*pas ; i < t2*pas ; i++) {
            double x = (double) i / pas ;
            double y_rand = (double) (rand()/ (double) RAND_MAX) ;
            points[i] = h1 - (double) ((b2-h1)/(double) (t2-s2))*s2 + (double) ((b2-h1)/(double) (t2-s2))*x  + y_rand*4 - 2 ;
        }

        for (int i = s3*pas ; i < t3*pas ; i++) {
            double x = (double) i / pas ;
            double y_rand = (double) (rand()/ (double) RAND_MAX) ;
            double a = ((h2-b2)/(t3-s3)) ;
            points[i] = b2 - a*s3 + a*x + y_rand*4 - 2 ;
        }

        for (int i = s4*pas ; i < t4*pas ; i++) {
            double x = (double) i / pas ;
            double y_rand = (double) (rand()/ (double) RAND_MAX) ;
            double a = ((b3-h2)/(t4-s4)) ;
            points[i] = b3 - a*t4 + a*x + y_rand*4 - 2 ;
        }


    }

        // Calcul demande hiver et mi-saison
    if (saison <= 1) {

        for (int i = 0 ; i < t1*pas ; i++) {
            double x = (double) i / pas ;
            double y_rand = (double) (rand()/ (double) RAND_MAX) ;
            points[i] = b1 + (h1 - b1)*exp(-(x-t1)*(x-t1)/(c)) + y_rand*2 - 1 ;
        }


        for (int i = s2*pas ; i < t2*pas ; i++) {
            double x = (double) i / pas ;
            double y_rand = (double) (rand()/ (double) RAND_MAX) ;
            points[i] = h1 + y_rand*2 - 1 ;
            if ((x >= 9) && (x <= 10)) {
                y_rand = (double) (rand()/(double) RAND_MAX);
                double z_rand = (double) (rand()/(double) RAND_MAX) ;
                double pic = amplit*y_rand*z_rand ;
                points[i] += pic ;
            }
        }

        for (int i = (s3*pas) ; i < t3*pas ; i++) {
            double x = (double) i / pas ;
            double y_rand = (double) (rand()/(double) RAND_MAX) ;
            points[i] = b2 + (h1 - b2)*exp(-(x-s3)*(x-s3)/(c)) + y_rand*4 - 2;
        }

        for (int i = (s4*pas) ; i < t4*pas ; i++) {
            double x = (double) i / pas ;
            double y_rand = (double) (rand()/(double) RAND_MAX) ;
            points[i] = b2 + (h2 - b2)*exp(-(x-t4)*(x-t4)/(c2))  + y_rand*2 - 1;

        }

        for (int i = (s5*pas) ; i < t5*pas ; i++) {
            double x = (double) i / pas ;
            double y_rand = (double) (rand()/(double) RAND_MAX) ;
            points[i] = h2  + y_rand*2 ;
        }

        for (int i = (t5*pas) ; i < T*pas ; i++) {
            double x = (double) i / pas ;
            double y_rand = (double) (rand()/(double) RAND_MAX) ;
            points[i] = b3 + (h2 - b3)*exp(-(x-t5)*(x-t5)/(c))  + y_rand*4 - 2 ;
        }
    }

    if (saison == 4) {
        for (int i=0 ; i < T*pas ; i++) {
            double y_rand = (double) (rand()/ (double) RAND_MAX) ;
            points[i] = y_rand*100 ;
        }
    }

    return points ;

}


double* setDemand(int saison, int heures, int pas) {
    //0 : hiver
    // 1 : mi-saison
    // 2 : été
    // 3 : théorique
    // 4 : aléatoire


    double b1 ;
    double b2 ;
    double b3 ;

    double h1 ;
    double h2 ;


    double* points = new double[heures*pas] ;
    for (int i=0 ; i < heures*pas ; i++) {
        points[i] = 0 ;
    }

    setParam(saison, b1, b2, b3, h1, h2) ;

    int jours = heures/24 ;

    for (int i = 0 ; i < jours ; i++) {
        double* journee = demand24(pas, saison, b1, b2, b3, h1, h2) ;
        for (int j=0 ; j < 24*pas ; j++) {
            points[i*24*pas + j] = journee[j];
        }
        double b = b3 ;
        if (saison == 2) {
            b=b2;
        }
        setParam(saison, b1, b2, b3, h1, h2) ;
        b1 = b ;
    }


    return points ;

}
