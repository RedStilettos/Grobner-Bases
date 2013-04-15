// Ashley Kumar
// 2013

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string> 
#include <stdlib.h>

#include "polynomial.h"

using namespace std;

int main(int argc, char *argv[]){
    int num_polys = 0;
    int num_vars = 0;  
    int i = 0;
    Polynomial poly;  
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

    // get the exact variables used in the polynomials 
    while (true){
        cout << "Please enter the variables in the polynomials, ";
        cout << "separated by commas. Ex: x,y,z\n";
        getline(cin, buf);
        if (parse_vars(buf, num_vars, &poly) == 0){
            break;
        }
    
        // there was a problem parsing the vars
        // be sure to free the allocated vars list 
        cout << "Invalid vars, try again.\n";
        free(poly.vars); 
    }

    cout << "Please enter " << num_polys << " polynomials.\n";

    while (i < num_polys){
        cout << "polynomial " << (i+1) << " ";  
        getline(cin, buf); 
        if (parse_polynomial(buf, &poly) == 0){
            i++;
        }
        else{
            cout << "Error parsing polynomial. Please re-enter.\n";   
        }
    }

    return 1;   
}



