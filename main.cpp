#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <math.h>
#include <ctime>
#include <sstream>
#include <fstream>

using namespace std ;

#define nbGen 8 
#define nbInst 25
#define nbData 10
#define T 24
#define ratio_reserve 0.03

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


int main() {

    double** nb_of_gen = new double*[nbInst] ;
    for (int i=0 ; i < nbInst ; i++) {
        nb_of_gen[i] = new double[nbGen] ;
    }

    double* Pmax = new double[nbGen] ;
    double* Pmin = new double[nbGen] ;
    double* L = new double[nbGen] ;
    double* SU = new double[nbGen] ;
    double* RU = new double[nbGen] ;
    double* Tcold = new double[nbGen] ;
    double* CostLow = new double[nbGen] ;
    double* CostHigh = new double[nbGen] ;
    double* hc = new double[nbGen] ;
    double* cc = new double[nbGen] ;

    double* demand = new double[T] ;

    ifstream f_data("gen_data.txt", ios::in);
    ifstream f_nb("gen_nb.txt", ios::in);
    ifstream f_demand("demand.txt", ios::in);
    //demand.txt est la demande du dataset de Carrion et al. Dans l'article de Knueven et Ostrowski sur l'aggrégation, ils donnent la demande en pourcentage de Pmax, et on obtient des pourcentages différents.

    string ligne;
    int guff ;

    ////////////////////////// Récupération des données ////////////////////////

    int sommePmax = 0 ;

    // récupération des données de gen_data
    getline(f_data, ligne);
    for (int i=0 ; i < nbGen ; i++) {
        f_data >> guff;
        f_data >> Pmax[i] ;
        f_data >> Pmin[i] ;
        f_data >> L[i] ;
        f_data >> RU[i] ;
        f_data >> Tcold[i] ;
        getline(f_data, ligne);

        sommePmax+= Pmax[i] ;
    }

    getline(f_data, ligne);
    getline(f_data, ligne);
    for (int i=0 ; i < nbGen ; i++) {
        f_data >> guff;
        f_data >> CostLow[i] ;
        f_data >> CostHigh[i] ;
        f_data >> hc[i] ;
        f_data >> cc[i] ;
        getline(f_data, ligne);
    }

    // récupération des données de gen_nb
    getline(f_nb, ligne);
    for (int i=0 ; i < nbInst ; i++) {
        f_nb >> guff ;
        f_nb >> guff ;
        for (int gen = 0 ; gen < nbGen ; gen++) {
            f_nb >> nb_of_gen[i][gen] ;
        }
        getline(f_nb, ligne);
    }

    //récupération demande
    for (int t=0 ; t < T ; t++) {
        f_demand >> demand[t] ;
        demand[t] /= sommePmax ;
        cout << demand[t] << "; " ;
    }

    cout << endl ;
    ////////////////////////////////////////////////////////////////////////////


    //////// Création instances ///////


    for (int inst = 0 ; inst < nbInst ; inst++) {

        string name = "ost_"+ to_string2(inst)+".txt" ;
        const char* fileChar = name.c_str() ;
        ofstream fichier(fileChar) ;

        int n=0 ;
        for (int i=0 ; i < nbGen ; i++) {
            n+= nb_of_gen[i];
        }

        fichier << "n = "<< n << " ; " << endl ;
        fichier << "T = "<< T << " ; " << endl ;

        //pas d'initialisation=pas de contraintes dynamiques à l'instant t=1
        fichier << "Init = [ " ;
        int i=0 ;
        while (i < n) {
            fichier << -1 << " " ;
            i++ ;
        }
        fichier << "]" << endl ;


        fichier << "L = [ " ;
        for (int i = 0 ; i < nbGen ; i++) {
            for (int j = 0 ; j < nb_of_gen[i]; j++) {
                fichier << L[i] << " " ;
            }
        }
        fichier << "]" << endl ;

        fichier << "l = [ " ;
        for (int i = 0 ; i < nbGen ; i++) {
            for (int j = 0 ; j < nb_of_gen[i]; j++) {
                fichier << L[i] << " " ;
            }
        }
        fichier << "]" << endl ;

        fichier << "Pmin = [ " ;
        for (int i = 0 ; i < nbGen ; i++) {
            for (int j = 0 ; j < nb_of_gen[i]; j++) {
                fichier << Pmin[i] << " " ;
            }
        }
        fichier << "]" << endl ;


        fichier << "Pmax = [ " ;
        for (int i = 0 ; i < nbGen ; i++) {
            for (int j = 0 ; j < nb_of_gen[i]; j++) {
                fichier << Pmax[i] << " " ;
            }
        }
        fichier << "]" << endl ;


        fichier << "CostLow = [ " ;
        for (int i = 0 ; i <n ; i++) {
            for (int j = 0 ; j < sym[i]; j++) {
                fichier << CostLow[i] << " " ;
            }
        }
        fichier << "]" << endl ;

        fichier << "c0 = [ " ;
        for (int i = 0 ; i <n ; i++) {
            for (int j = 0 ; j < sym[i]; j++) {
                fichier << c0[i] << " " ;
            }
        }
        fichier << "]" << endl ;

        fichier << "CostLow = [ " ;
        for (int i = 0 ; i <n ; i++) {
            for (int j = 0 ; j < sym[i]; j++) {
                fichier << CostLow[i] << " " ;
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


        fichier << "nS = " << nS << endl ;

        fichier << "S = [ " ;
        for (int i = 0 ; i <n ; i++) {
            fichier << S[i] << " " ;
        }
        fichier << "]" << endl ;

    }


} // fin for inst


return 0;
}

/*

        ifstream file2("result_trait.txt", ios::in);

        for (int k = 0 ; k < 4 ; k++) {

                fichier << "\\hline" << endl ;
                fichier << "(" << N[k] << "," << T[k] << ") " ;

                int sym_start=4 ;
                if (N[k] == 15) {
                        sym_start=3 ;
                }
                for (int sym=sym_start ; sym >= 2 ; sym--) {

                        fichier << " & F = " << sym ;

                        double guff_double ;
                        int guff_int ;
                        char guff_char ;

                        // Sorties et indicateurs pour le lot de 20 instances

                        int nb_solved[nbMethods] ;

                        double gap_impr_dof[2] = {0,0} ; // default Cplex et Callback Cplex vs DOF
                        double gap_impr_mob[2] = {0,0} ; // default Cplex et Callback Cplex vs DOF

                        double cpu_impr_dof[2] = {0,0} ; // default Cplex et Callback Cplex vs DOF
                        double cpu_impr_mob[2] = {0,0} ; // default Cplex et Callback Cplex vs DOF

                        double node_impr_dof[2] = {0,0} ; // default Cplex et Callback Cplex vs DOF
                        double node_impr_mob[2] = {0,0} ; // default Cplex et Callback Cplex vs DOF

                        int nb_one_ended_dof[2] = {0,0} ; // default Cplex et Callback Cplex vs DOF
                        double nb_one_ended_mob[2] = {0,0} ; // default Cplex et Callback Cplex vs DOF

                        int nb_both_ended_dof[2] = {0,0} ; // default Cplex et Callback Cplex vs DOF
                        double nb_both_ended_mob[2] = {0,0} ; // default Cplex et Callback Cplex vs DOF

                        string ligne;
                        int n;
                        int T;
                        int sym ;

                        int id;
                        double opt ;
                        int fix ;

                        double fixTime ;
                        int met ;

                        double meanFixs[nbMethods]  ;
                        double meanCPU[nbMethods] ;
                        double meanNodes[nbMethods]

                        for (int i=0 ; i < nbMethods ; i ++) {
                                meanCPU[i] = 0 ;
                                meanFixs[i] = 0 ;
                                nb_solved[i] = 0 ;
                                meanNodes[i] = 0 ;
                        }

                        ///////// Extraction des données ///////

                        int test = 0;
                        while (test < nTests) {

                                double gap[nbMethods]  ;
                                double nodes[nbMethods] ;
                                double cpu[nbMethods] ;

                                for (int i=0 ; i < nbMethods ; i ++) {
                                        gap[i] = 0;
                                        nodes[i] = 0;
                                        cpu[i] = 0;
                                }

                                for (int m = 0 ; m < nbMethods ; m++) {

                                        file2 >> met ;
                                        file2 >> guff_char ;

                                        file2 >> n ;
                                        file2 >> guff_char ;

                                        file2 >> T ;
                                        file2 >> guff_char ;

                                        file2 >> sym ;
                                        file2 >> guff_char ;

                                        file2 >> guff_int ;
                                        file2 >> guff_char ;

                                        file2 >> guff_int ;
                                        file2 >> guff_char ;

                                        file2 >> guff_double ;
                                        file2 >> guff_char ;

                                        file2 >> id ;
                                        file2 >> guff_char ;

                                        file2 >> opt ;
                                        file2 >> guff_char ;

                                        file2 >> gap[m];
                                        file2 >> guff_char ;
                                        file2 >> guff_char ;
                                        file2 >> guff_char ;

                                        file2 >> nodes[m] ;
                                        file2 >> guff_char ;

                                        file2 >> fix ;
                                        file2 >> guff_char ;

                                        file2 >> fixTime;
                                        file2 >> guff_char ;

                                        file2 >> cpu[m] ;
                                        file2 >> guff_char ;

                                        getline(file2, ligne);


                                        //affichage de controle
                                        fichier2 << met <<  " & " << n << " & " << T  << " & " << sym << " & " << id ;
                                        fichier2 << " & " << opt ;
                                        fichier2.precision(3);
                                        fichier2 << " & " << gap[m] ;
                                        fichier2.precision(6);
                                        fichier2 << " & " << nodes[m] ;
                                        fichier2 << " & " << fix ;
                                        fichier2 <<  " & " << fixTime ;
                                        fichier2 << " & " << cpu[m] ;
                                        fichier2 <<" \\\\ " << endl ;

                                        meanFixs[m] += fix ;
                                        meanNodes[m] += nodes[m] ;
                                }

                                fichier2 << " \\hline" << endl ;
                                //Ligne vide
                                getline(file2, ligne);

                                test++ ;

                                /// Mise à jour des indicateurs pour le test précédent ///

                                for (int i = 0 ; i <= 4 ; i++) {
                                        if (cpu[i] < 3600) {
                                                nb_solved[i] ++ ;
                                                meanCPU[i] += cpu[i] ;
                                        }
                                }

                                cout << "Instance id : " << test << endl ;

                                for (int i = 0 ; i <= 1 ; i++) {

                                        // instances qui terminent
                                        if ((cpu[i] < 3600) && (cpu[2] < 3600)) {
                                                nb_both_ended_mob[i] ++ ;
                                                nb_one_ended_mob[i] ++ ;
                                        }
                                        else if ((cpu[i] < 3600) || (cpu[2] < 3600)) {
                                                nb_one_ended_mob[i] ++ ;
                                        }
                                        if ((cpu[i] < 3600) && (cpu[3] < 3600)) {
                                                nb_both_ended_dof[i] ++ ;
                                                nb_one_ended_dof[i] ++ ;
                                        }
                                        else if ((cpu[i] < 3600) || (cpu[3] < 3600)) {
                                                nb_one_ended_dof[i] ++ ;
                                        }

                                        //gap improvement
                                        if ((cpu[i] > 3600) && (cpu[2] > 3600)) {
                                                gap_impr_mob[i] += 2*(gap[i] - gap[2]) / (gap[i] + gap[2]) ;
                                        }
                                        if ((cpu[i] > 3600) && (cpu[3] > 3600)) {
                                                gap_impr_dof[i] += 2*(gap[i] - gap[3]) / (gap[i] + gap[3]) ;
                                        }

                                        //node improvement
                                        if ((cpu[i] < 3600) && (cpu[2] < 3600) && (nodes[i] + nodes[2] > 0) ) {
                                                node_impr_mob[i] += 2*(nodes[i] - nodes[2]) / (nodes[i] + nodes[2]) ;
                                        }
                                        if ((cpu[i] < 3600) && (cpu[3] < 3600) && (nodes[i] + nodes[3] > 0) ) {
                                                node_impr_dof[i] += 2*(nodes[i] - nodes[3]) / (nodes[i] + nodes[3]) ;
                                        }


                                        //cpu improvement
                                        if ((cpu[i] < 3600)  || (cpu[2] < 3600)) {
                                                if (i==1) {
                                                cout << "cpu impr mob : " << 2*(cpu[i] - cpu[2]) / (cpu[i] + cpu[2]) << endl ;
                                                }
                                                cpu_impr_mob[i] += 2*(cpu[i] - cpu[2]) / (cpu[i] + cpu[2]) ;

                                        }

                                        if (i==1) {
                                        cout << "cpu callback : " << cpu[1] << endl ;
                                        cout << "cpu dof : " << cpu[3] << endl ;
                                        }
                                        if ( (cpu[i] < 3600) || (cpu[3] < 3600)) {
                                                cpu_impr_dof[i] += 2*(cpu[i] - cpu[3]) / (cpu[i] + cpu[3]) ;

                                                if (i==1) {

                                                double impr = 2*(cpu[i] - cpu[3]) / (cpu[i] + cpu[3]) ;
                                                cout << "cpu impr dof : " <<  impr << endl ;

                                                }

                                        }
                                }

                                cout << endl ;

                        }


                                //////// Affichage sorties pour ce lot (n,T), F //////////

                        for (int i=0 ; i <= 1 ; i++) {
                                if (nb_one_ended_mob[i] < 20) {
                                        gap_impr_mob[i] /= 20 - nb_one_ended_mob[i] ;
                                }
                                if (nb_one_ended_dof[i] < 20) {
                                        gap_impr_dof[i] /= 20 - nb_one_ended_dof[i] ;
                                }

                                if (nb_both_ended_mob[i] > 0) {
                                        node_impr_mob[i] /= nb_both_ended_mob[i] ;
                                }
                                if (nb_both_ended_dof[i] > 0) {
                                        node_impr_dof[i] /= nb_both_ended_dof[i] ;
                                }

                                // Pour l'amélioration du CPU; les instances non terminées comptent pour 0%

                                cpu_impr_mob[i] /=20;

                                cpu_impr_dof[i] /= 20;

                        }

                        for (int m=0 ; m < 4 ; m++) {
                                meanNodes[m] /= 20 ;
                                meanCPU[m] /= 20 ;
                                meanFixs[m] /= 20 ;
                        }
                        fichier.precision(3);


                        for (int m=0 ; m < 4 ; m++) {

                                if (m==0) {
                                        fichier << " & DC & " ;
                                }
                                if (m==1) {
                                        fichier << " & & CC & " ;
                                }
                                if (m==2) {
                                        fichier << " & & MOB & " ;
                                }
                                if (m==3) {
                                        fichier << " & & DOF & " ;
                                }
                        }

                        fichier << " & MOB & " << nb_solved[2] ;
                        for (int i=0 ; i <= 1 ; i++) {
                        fichier << " & " << nb_solved[2] - nb_solved[i] <<  " & " << 100*gap_impr_mob[i] <<  " \\% & " << 100*node_impr_mob[i] << " \\% & " << 100* cpu_impr_mob[i]  << " \\%";
                        }
                        fichier << " \\\\" << endl ;

                        fichier << "\\hhline{~~----------}" << endl ;

                        fichier << " & & DOF & " << nb_solved[3] ;
                        for (int i=0 ; i <= 1 ; i++) {
                        fichier << " & " << nb_solved[3] - nb_solved[i] <<  " & " << 100*gap_impr_dof[i] <<  " \\% & " << 100*node_impr_dof[i] << " \\% & " << 100*cpu_impr_dof[i] << " \\%";
                        }
                        fichier <<  " \\\\" << endl ;

                        if (sym != 2) {
                                        fichier << "\\hhline{~===========}" << endl ;
                        }
                        else {
                                        fichier << "\\hline" << endl ;

                        }
                }
        }*/





