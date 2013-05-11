// Ashley Kumar
// 2013
// OMP_NUM_THREADS=12

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string> 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h> 

#include "polynomial.h"
#include "parser.h"

using namespace std;

int main(int argc, char *argv[]){
    int num_polys = 0;
    int num_vars = 0;  
    int i = 0, count=0, ordering=0;
    int c, line_count=0;
    bool show_help = false; 
    time_t start, end;  
    timeval s, e;  
    ifstream infile;  
    Polynomial **polys, **basis, **reduced;  
    string buf = "";
    string input;  
    Polynomial *res; 
    int size=0, red_size = 0;  

    // parse out command line options 
    while((c = getopt(argc, argv, ":v:f:h")) != EOF) {
        switch(c) {
            case 'f':
                infile.open(optarg);
                break;
            case 'h':
                show_help = true;
                break; 
            case ':':
                cout << "bad input, son\n";
                break;  
        }
    }
    
    if (show_help) {
        printf("should probably say something helpful here\n");
        return 0;  
    }

    // read from a file, if one is open; 
    if (infile.is_open()){
        printf("read input\n"); 
        while(!infile.eof()){
            getline(infile, input); 
            stringstream str(input); 
            switch(line_count) {
                case 0:
                    if (str >> num_polys) line_count++;
                    else { printf("invalid number of polynomial\n"); return 1; }
                    break;
                case 1:
                    if (str >> ordering) line_count++;
                    else { printf("invalid ordering\n"); return 1; }
                    break;
                case 2:
                    if (str >> num_vars) {
                        line_count++;
                        polys = (Polynomial **) malloc(sizeof(Polynomial *) * num_polys);
                        for (int j = 0; j < num_polys; j++) {
                            polys[j] = (Polynomial *) malloc(sizeof(Polynomial));
                            polys[j]->ordering = ordering; 
                            polys[j]->vars = (char *) malloc(sizeof(char) * num_vars); 
                        }
                    }
                    else { printf("invalid number of vars\n"); return 1; }
                    break;
                case 3:
                    if (parse_vars(input, num_polys, num_vars, polys) == 0){
                        line_count++;
                    }
                    else {
                        printf("problem parsing variables\n"); 
                        return 1; 
                    }
                    break; 
                default:
                    if (count < num_polys) {
                        if (parse_polynomial(input, polys[count]) != 0){
                            printf("error parsing polynomial\n");
                            return 1; 
                        }
                        count++; 
                    }
                    break; 
            }
        }
        infile.close();  
        for(int i = 0; i < num_polys; i++){
            sort_polynomial(polys[i]); 
            //to_string(polys[i]); 
        }
        printf("we have %d polynomials in %d variables\n", num_polys, num_vars); 
        //do_pointless(polys, num_polys);
        //basis = grobner_basis(polys, num_polys, &size);  
        //reduced = reduce_basis(basis, size, &red_size); 
        //res = subtract_polys(polys[1],polys[0]);
        start = time(&start);
        gettimeofday(&s, NULL); 
        res = add_polys(polys[0], polys[1]); 
        end = time(&end); 
        gettimeofday(&e, NULL);  
        //to_string(res);
        printf("took %f seconnds\n", difftime(end, start));  
        free_polynomial(res);  
        printf("back from operation\n");     
        //to_string(res); 
        //free_polynomial(res); 
        for (int i = 0; i < num_polys; i++){
            free_polynomial(polys[i]);
        }
        free(polys);
       /* printf("basis is:\n"); 
        for (int i = 0; i < size; i++){
           to_string(basis[i]); 
           free_polynomial(basis[i]);
        }
        free(basis);
        printf("reduced basis\n");  
        for (int i = 0; i < red_size; i++){
            to_string(reduced[i]);
            free_polynomial(reduced[i]); 
        }   
        free(reduced);*/
        //par_test(); 
    } 

    else{
        cout << "Hello! This program will compute the Grobner Basis ";
        cout << "for a set of polynomials.\n";
       
        // for ease of later parsing, get the total number of polynomials
        while (true){
            cout << "Please enter the number of polynomials ";
            getline(cin, buf); 
            stringstream str(buf); 
       
            if (str >> num_polys){
                break; 
            }
            cout << "Invalid number, try again.\n"; 
        }

        // for ease of later parsing, get the total number of variables in the set
        while (true){
            cout << "Please enter the number of variables ";
            getline(cin, buf); 
            stringstream str(buf); 
       
            if (str >> num_vars){
                break; 
            }
            cout << "Invalid number, try again.\n"; 
        }

        polys = (Polynomial **) malloc(sizeof(Polynomial *) * num_polys);
        for (int j = 0; j < num_polys; j++) {
            polys[j] = (Polynomial *) malloc(sizeof(Polynomial));
            polys[j]->vars = (char *) malloc(sizeof(char) * num_vars); 
        }

        // get the exact variables used in the polynomials 
        while (true){
            cout << "Please enter the variables in the polynomials, ";
            cout << "separated by commas. They should be in decreasing ";
            cout << "order as will be used for the monomial orderings. Ex: x,y,z\n";
            getline(cin, buf);
            if (parse_vars(buf, num_polys, num_vars, polys) == 0){
                break;
            }
        
            // there was a problem parsing the vars
            cout << "Invalid vars, try again.\n";
        }

        cout << "Please enter " << num_polys << " polynomials.\n";

        while (i < num_polys){
            cout << "polynomial " << (i+1) << " ";  
            getline(cin, buf); 
            if (parse_polynomial(buf, polys[i]) == 0){
                i++;
            }
            else{
                cout << "Error parsing polynomial. Please re-enter.\n";   
            }
        }

        // set ordering 
        for (int j = 0; j < num_polys; j++) {
            polys[j]->ordering =1;
            sort_polynomial(polys[j]); 
        }

        printf("computing...\n"); 
        int basis_size = num_polys; 
        int reduced_size; 
        Polynomial **basis, **reduced;  
        /*divide_polys(polys[0], polys[1], &quot, &rem); 
        printf("quot is \n");
        to_string(quot); 
        printf("rem is\n"); 
        to_string(rem);
        free(quot);
        free(rem); */
        /*s = s_poly(polys[0], polys[1]); 
        printf("s poly is\n");
        to_string(s);
        free(s);*/
        basis = grobner_basis(polys, 2, &basis_size); 
        reduced = reduce_basis(basis, basis_size, &reduced_size);
        printf("lets print the basis\n");  
        for (int j = 0; j < num_polys; j++){
            free_polynomial(polys[j]);
        }
        free(polys); 
        
        for (int j = 0; j < basis_size; j++){
            to_string(basis[j]); 
            free_polynomial(basis[j]);
        }
        free(basis);
        printf("REDUCED BASIS\n"); 
        for (int j = 0; j < reduced_size; j++){
            to_string(reduced[j]);
            free_polynomial(reduced[j]);
        }
        free(reduced);
    }

    return 0;   
}


