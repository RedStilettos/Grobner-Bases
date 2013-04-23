// Ashley Kumar
// 2013

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string> 
#include <stdlib.h>
#include <stdio.h>

#include "polynomial.h"

using namespace std;

int main(int argc, char *argv[]){
    int num_polys = 0;
    int num_vars = 0;  
    int i = 0;
    Polynomial **polys;  
    string buf = ""; 

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
        cout << "separated by commas. Ex: x,y,z\n";
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

    for (int j = 0; j < num_polys; j++){
        free_polynomial(polys[j]);
        free(polys);  
    }
    return 0;   
}


