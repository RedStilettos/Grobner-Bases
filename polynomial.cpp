#include <string>
#include <iostream>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "polynomial.h"

using namespace std;

int parse_polynomial(string input, Polynomial *poly){
    int num_terms;
    char *buf; 
    char *pol = strdup(input.c_str()); 

    get_num_terms(&num_terms, input);
    //poly->terms = (Term *) malloc(sizeof(Term) * num_terms);  
    
    buf = strtok(pol, "+-"); 
    while (buf != NULL){
        printf("%s\n", buf);
        buf = strtok(NULL, "+-"); 
    }  
 
    free(pol);
    return 0; 
}


int get_num_terms(int *terms, string poly){
    int len = poly.length(); 
    
    for (int i = 0; i < len; i++){
        if (ispunct(poly[i])){
            (*terms)++;
        }
    }
    return 0;
}


int parse_vars(string vars, int num_vars, Polynomial *poly){
    bool var = true; 
    poly->vars = (char *) malloc(sizeof(char) * num_vars);
    int count = 0;

    for (int i = 0; i < vars.length(); i++){
        // expecting var, not comma
        if (vars[i] == ',' && var) return 1;

        else if (vars[i] == ',' && !var) var = !var; 

        // expecting comma, not a var
        else if(isalpha(vars[i]) && !var) return 1;

        // expecting a var, make sure it's not in the list, then insert
        else if(isalpha(vars[i]) && var){
            if (count >= num_vars) return 1;
 
            for (int j = 0; j < count; j++){
                // check for duplicate vars 
                if (vars[i] == poly->vars[j]){
                    return 1; 
                }
            }
            poly->vars[count] = vars[i];
            count++;
            var = !var;  
        }
    }
    poly->num_vars = num_vars; 
    return (num_vars - count); 
}


