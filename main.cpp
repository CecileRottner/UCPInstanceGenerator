#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <ctime>
#include <sstream>
#include <fstream>
#include <cmath>

#include "demand.h"
#include "corr.h"

using namespace std ;

string to_string2(int number){
    string number_string = "";
    char ones_char = '0';
    int ones = 0;
    while(true){
        ones = number % 10;
        switch(ones){
        case 0: ones_char = '0'; break;
        case 1: ones_char = '1'; break;
        case 2: ones_char = '2'; break;
        case 3: ones_char = '3'; break;
        case 4: ones_char = '4'; break;
        case 5: ones_char = '5'; break;
        case 6: ones_char = '6'; break;
        case 7: ones_char = '7'; break;
        case 8: ones_char = '8'; break;
        case 9: ones_char = '9'; break;
        default : ; //ErrorHandling("Trouble converting number to string.");
        }
        number -= ones;
        number_string = ones_char + number_string;
        if(number == 0){
            break;
        }
        number = number/10;
    }
    return number_string;
}


void setInterval(double* vect, double vmax, double vmin, int l1, int l2) {
    for (int i = l1 ; i <= l2 ; i++) {
        vect[i] = vect[i]*(vmax-vmin) + vmin ;
    }
}

void generation(int n, int T, int demande, int symetrie, int cat, int bloc, int id, int intra) {

    double pas = 1 ; //number of time steps per hour

    //Nom de l'instance

    string s_n = to_string2(n) + "_" ;

    string s_T = s_n + to_string2(T) ;
    s_T = s_T + "_" ;

    string s_bloc = s_T + to_string2(bloc);
    s_bloc = s_bloc + "_" ;

    string s_demande = s_bloc + to_string2(demande) ;
    s_demande = s_demande + "_" ;

    string  s_symetrie = s_demande + to_string2(symetrie) ;
    s_symetrie = s_symetrie + "_" ;

    string s_cat = s_symetrie + to_string2(cat) ;
    s_cat = s_cat + "_" ;

    string s_intra = s_cat + to_string2(intra) ;
    s_intra = s_intra + "_" ;

    string s_id = s_intra + to_string2(id) ;
    string file = s_id + ".txt" ;

    const char* fileChar = file.c_str() ;
    ofstream fichier(fileChar);

    fichier << "n = "<< n << " ; " << endl ;
    fichier << "T = "<< T << " ; " << endl ;

    /*fichier << "Bloc : " << bloc << endl ;
    fichier << "Demande : " << demande << endl ;
    fichier << "Symetrie : " << symetrie << endl ;
    fichier << "Categorie : " << cat << endl ;*/
    int n_sym = n ;
    int* sym = new int[n] ;
    for (int i= 0 ; (i<n) ; i++) {
        sym[i] = 1 ;
    }

    double* Pmax = new double[n];
    double* Pmin = new double[n];
    double* c0 = new double[n] ;
    double* cp = new double[n] ;
    double* cf = new double[n] ;
    double* L = new double[n] ;
    double* l = new double[n] ;
    double* S = new double[n] ;
    int* n_k = new int[n] ;

    int* First = new int[n];
    int* Last = new int[n] ;

    for (int i= 0 ; (i<n) ; i++) {
        First[i] = 0 ;
        Last[i] = 0 ;
    }

    int nS= 1 ; // number of geographical sites

    int Pmin_Pmax_dizaines=1;

    //Gestion symétries et sites
    if (symetrie && bloc <3) {

        //sym : vecteur de répétition des unités.
        //il y a des groupes de taille 1 à n/symetrie d'unités semblables
        //n_sym : nombre d'unités différentes

        int sym_factor = symetrie ;
        int sum = 0 ;
        for (int i= 0 ; (i<n) && (sum < n); i++) {
            sym[i] = (rand() % (n/sym_factor)) + 1 ;
            sum+= sym[i] ;
            if (sum >= n) {
                n_sym= i;
                sym[i] = sym[i] - (sum-n) ;
                sum = n+1 ;
            }
        }
        for (int i = n_sym+1 ; i < n ; i++) {
            sym[i] = 0 ;
        }




        //un seul groupe de symétrie : symetrie=n
        /* if (symetrie==n) {
            sym[0] = n ;
            for (int i = 1 ; i < n ; i++) {
                sym[i] = 0 ;
            }

        }*/


        //n/2 groupes de symetries
        /*      if (symetrie==n) {
            for (int i = 0 ; i < n/2 ; i++) {
                sym[i] = 2 ;
            }
            for (int i=n/2 ; i < n ; i++) {
                sym[i] =0 ;
            }

        }*/

        //passage de n_sym à nk et First / Last
        int count = 0 ;
        int i=0 ;
        while (i < n) {
            First[i] = 1 ;
            for (int j = 0 ; j < sym[count]; j++) {
                n_k[i] = count+1 ;
                i++ ;
            }
            Last[i-1] = 1 ;
            count ++ ;

        }


    }

    if (intra) {

        //S : délimiteur de sites

        if (!symetrie) { // on définit un vecteur sym juste pour l'intrasite

            int sym_factor = 6 ;
            int sum = 0 ;
            for (int i= 0 ; (i<n) && (sum < n); i++) {
                sym[i] = (rand() % sym_factor) + 1 ;
                sum+= sym[i] ;
                if (sum >= n) {
                    n_sym= i;
                    sym[i] = sym[i] - (sum-n) ;
                    sum = n+1 ;
                }
            }
            for (int i = n_sym+1 ; i < n ; i++) {
                sym[i] = 0 ;
            }

            nS = n_sym+1 ;

            for (int i = 0 ; i < n ; i++) {
                S[i] = 0 ;
            }

            int count = 0 ;
            int i=0 ;
            while (i < n) {
                S[i] = 1 ;
                for (int j = 0 ; j < sym[count]; j++) {
                    n_k[i] = count+1 ;
                    i++ ;
                }
                //Last[i-1] = 1 ;
                count ++ ;

            }
        }
//        int unit=0 ;
//        int gp_sym=0 ;
//        S[unit]=1 ;
//        int F = 1 ; // une chance sur F que les deux groupes ne soient pas sur le même site
//        while (unit<n) {
//            // on décide si le groupe de symétrie de i est sur un site différent de i-1 ou pas
//            if (unit!=0) {
//                int nombre = (rand() % F) ;
//                if (nombre == 1) {
//                    S[unit] = 1 ;
//                    nS++ ;
//                }
//                else {
//                    S[unit] = 0 ;
//                }
//            }
//            int nb_sym = sym[gp_sym] ;
//            for (int k = 0; k < nb_sym-1 ; k++ ) {
//                unit++ ;
//                S[unit] = 0 ;
//            }
//            gp_sym++ ;
//            unit++ ;
//        }
        if (!symetrie) {
            for (int i = 0 ; i < n ; i++) {
                sym[i] = 1 ;
                n_k[i] = i;
            }
            n_sym=n;
        }
    }


    //init
    fichier << "Init = [ " ;
    int count = 0 ;
    int i=0 ;
    while (i < n) {
        int init = rand() % 2  ;
        for (int j = 0 ; j < sym[count]; j++) {
            fichier << init << " " ;
            i++ ;
        }
        count++ ;
    }
    fichier << "]" << endl ;


    // Gestion corrélation type littérature
    if (bloc<=1) {

        genCorrCoef(L, cp, -0.9, n, 0, n-1) ;
        correle2a2(L, Pmax, 0.9, 0, n-1) ;
        correle2a2(Pmax, Pmin, 0.9, 0, n-1) ;
        correle2a2(Pmin, c0, 0.8, 0, n-1) ;

        //Couts décorrélés du reste
        if (cat == 2) {
            for (int i = 0 ; i < n ; i++) {
                cp[i] = (double) (rand()/(double) RAND_MAX) ;
                c0[i] = (double) (rand()/(double) RAND_MAX) ;
            }
        }

        setInterval(L, 10*pas, 2*pas, 0, n-1) ;
        setInterval(cp, 42, 8, 0, n-1) ;
        setInterval(c0, 250, 30, 0, n-1) ;
        setInterval(Pmin, 100, 8, 0, n-1) ;


        //Pmax entre 20 et 420, supérieur à Pmin
        for (int i = 0 ; i < n ; i++) {
            Pmax[i] = fmax(Pmax[i]*400 + 20, Pmin[i]);
        }

        //TPR instances
        /* double factor = 1;
        for (int i = 0 ; i < n ; i++) {
            Pmin[i] = factor*Pmax[i] ;
        }*/

        //cf aléatoire, non corrélé, entre 7 et 70
        for (int i = 0 ; i < n ; i++) {
            cf[i] = (double) (rand()/(double) RAND_MAX) ;
            cf[i] = cf[i]*63 + 7;
        }


        /*if (Pmin_Pmax_dizaines) {
            cout << "ici" << endl ;
            for (int i = 0 ; i < n ; i++) {
                int Pm = round(Pmin[i]);
                int r = Pm % 10 ;
                if ( r != 0) {
                    cout << "r : " << r << endl ;
                    if ( r >= 5) {
                        Pmin[i] = Pm + 10 - r ;
                    }
                    else {
                        Pmin[i] = Pm - r ;
                    }
                }
            }
        }*/


        //Si instances 0-1 (Pmin = Pmax)
        if (cat == 1) {
            for (int i = 0 ; i < n ; i++) {
                Pmax[i] = Pmin[i] ;
            }
        }

        //Si unit-UCP
        if (bloc==0) {
            for (int i = 0 ; i < n ; i++) {
                Pmin[i] = 1 ;
                Pmax[i] = 1 ;
            }
        }



    }

    int nc = 0 ;
    int nf = 0 ;
    int nt = 0 ;


    if (bloc==4) { // intra site sans demande

        double** c0_mat = new double*[n] ;
        double** cf_mat = new double*[n];

        for (int i = 0 ; i < n ; i++) {
            c0_mat[i] = new double[T] ;
            cf_mat[i] = new double[T] ;
        }


        double* DemandCosts = setDemand(demande, T/pas, pas);

        for (int t = 0 ; t < T ; t++) {

            if (DemandCosts[t] > 100) {
                DemandCosts[t] = 100 ;
            }

            if (DemandCosts[t] < 0) {
                DemandCosts[t] = 0 ;
            }

            DemandCosts[t]=DemandCosts[t]/100 ;
        }


        genCorrCoef(L, cp, -0.9, n, 0, n-1) ;
        correle2a2(L, Pmax, 0.9, 0, n-1) ;
        correle2a2(Pmax, Pmin, 0.9, 0, n-1) ;
        correle2a2(Pmin, c0, 0.8, 0, n-1) ;


        //cf corrélés à la demande
        for (int i=0 ; i<n ; i++) {
            correle2a2(DemandCosts, cf_mat[i], 0.8, 0, T-1) ;
        }

        //Couts décorrélés du reste
        for (int i = 0 ; i < n ; i++) {
            cp[i] = (double) (rand()/(double) RAND_MAX) ;
            for (int t=0 ; t < T ; t++) {
                c0_mat[i][t] = (double) (rand()/(double) RAND_MAX) ;
            }
        }


        //cf aléatoire, non corrélé, entre 1 et 100
        for (int i = 0 ; i < n ; i++) {
            cf[i] = (double) (rand()/(double) RAND_MAX) ;
            cf[i] = cf[i]*100 - 50;
        }

        //l aléatoire
        for (int i = 0 ; i < n ; i++) {
            l[i] = (double) (rand()/(double) RAND_MAX) ;
            l[i] = l[i]*8*pas + 2*pas;
        }

        setInterval(L, 10*pas, 2*pas, 0, n-1) ;
        setInterval(cp, 50, -50, 0, n-1) ;
        setInterval(c0, 100, -100, 0, n-1) ;

    }


    //Intra site
    /* for (int i = 0 ; i <n ; i++) {
        S[i] = i ;
    }

    if (intra==1) {
        double nombre = (double) (rand()/(double) RAND_MAX) ;
        nS = nombre*n ;

        for (int i = 0 ; i <n ; i++) {
            nombre = (double) (rand()/(double) RAND_MAX) ;
            S[i] = ceil(nombre*nS) ;
        }

    }*/

    //Demande
    double* Demand = setDemand(demande, T/pas, pas);

    double sommePmax = 0 ;
    for (int i = 0 ; i <n ; i++) {
        for (int j = 0 ; j < sym[i] ; j++) {
            sommePmax += round(Pmax[i]) ;
        }
    }

    for (int t = 0 ; t < T ; t++) {
        if (Demand[t] > 100) {
            Demand[t] = 100 ;
        }
        if (Demand[t] < 0) {
            Demand[t] = 0 ;
        }
        Demand[t] *= sommePmax/100 ;
    }





    //Sortie

    fichier << "L = [ " ;
    for (int i = 0 ; i <n ; i++) {
        for (int j = 0 ; j < sym[i]; j++) {
            fichier << round(L[i]) << " " ;
        }
    }
    fichier << "]" << endl ;

    fichier << "l = [ " ;
    for (int i = 0 ; i <n ; i++) {
        for (int j = 0 ; j < sym[i]; j++) {
            if (bloc == 1) {
                fichier << round(L[i]) << " " ;
            }
            else {
                fichier << round(l[i]) << " " ;
            }
        }
    }
    fichier << "]" << endl ;

    fichier << "Pmin = [ " ;
    for (int i = 0 ; i <n ; i++) {
        for (int j = 0 ; j < sym[i]; j++) {
            fichier << round(Pmin[i]) << " " ;
        }
    }
    fichier << "]" << endl ;

    fichier << "Pmax = [ " ;
    for (int i = 0 ; i <n ; i++) {
        for (int j = 0 ; j < sym[i]; j++) {
            fichier << round(Pmax[i]) << " " ;
        }
    }
    fichier << "]" << endl ;

    if (bloc != 4) {

        fichier << "cf = [ " ;
        for (int i = 0 ; i <n ; i++) {
            for (int j = 0 ; j < sym[i]; j++) {
                fichier << cf[i] << " " ;
            }
        }
        fichier << "]" << endl ;


        fichier << "c0 = [ " ;
        for (int i = 0 ; i <n ; i++) {
            for (int j = 0 ; j < sym[i]; j++) {
                //fichier << round(c0[i]*(1+ (j+1)*(j+2)*0.5*0.005)) << " " ;
                fichier << round(c0[i]) << " " ;
            }
        }
        fichier << "]" << endl ;
    }

    else {

    }


    fichier << "cp = [ " ;
    for (int i = 0 ; i <n ; i++) {
        for (int j = 0 ; j < sym[i]; j++) {
            fichier << cp[i] << " " ;
        }
    }
    fichier << "]" << endl ;

    fichier << "D = [ " ;
    for (int i = 0 ; i <T ; i++) {
        fichier << round(Demand[i]) << " " ;
    }
    fichier << "]" << endl ;

    fichier << "K = " << n_sym+1 << endl ;

    fichier << "nk = [ " ;
    for (int i = 0 ; i <n ; i++) {
        fichier << n_k[i] << " " ;
    }
    fichier << "]" << endl ;


    fichier << "First = [ " ;
    for (int i = 0 ; i <n ; i++) {
        fichier << First[i] << " " ;
    }
    fichier << "]" << endl ;

    fichier << "Last = [ " ;
    for (int i = 0 ; i <n ; i++) {
        fichier << Last[i] << " " ;
    }
    fichier << "]" << endl ;



//    fichier << "nS = " << nS << endl ;

//    fichier << "S = [ " ;
//    for (int i = 0 ; i <n ; i++) {
//        fichier << S[i] << " " ;
//    }
//    fichier << "]" << endl ;


    ///// SITE = GROUPE DE SYMETRIE
        fichier << "nS = " << n_sym+1 << endl ;

        fichier << "S = [ " ;
        for (int i = 0 ; i <n ; i++) {
            fichier << First[i] << " " ;
        }
        fichier << "]" << endl ;

}


int main(int argc, char** argv) {
    srand(time(NULL));

    int demande_type; // = 3;
    int symetrie; //=5; // facteur de symetrie  \in [1, n]: plus il est grand, moins il y a de symetries. Mais si =n, alors un seul groupe de symétrie. Si =0: pas de symétries
    int cat; // = 1;
    int bloc; // =1;
    int n; //=15 ;
    int id ; //;
    int intra; //=1;
    int T; //=48;

    if (argc>1) {
        n = atoi(argv[1]);
        T = atoi(argv[2]);
        bloc = atoi(argv[3]);
        demande_type = atoi(argv[4]);
        symetrie = atoi(argv[5]);
        cat = atoi(argv[6]);
        intra = atoi(argv[7]);
        id = atoi(argv[8]);
    }




//    n=10 ;
//    for (T=24 ; T <=48 ; T*=2) {
//        for (id=1; id <=20 ; id++) {
//            generation(n, T, demande_type, symetrie, cat, bloc, id, intra) ;
//        }
//    }

//    n=20;
//    for (T=24 ; T <=48 ; T*=2) {
//        for (id=1; id <=20 ; id++) {
//            generation(n, T, demande_type, symetrie, cat, bloc, id, intra) ;
//        }
//    }

    T=24;
        for (id=1; id <=20 ; id++) {
            generation(n, T, demande_type, symetrie, cat, bloc, id, intra) ;
        }


    return 0;
}
