// Ashley Kumar
// 2013

#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <string> 

using namespace std;

// Fraction struct to represent rational coefficients 
struct Rational {
    int num;
    int den; 
}; 


// one term in the polynomial 
struct Term {
    Rational coeff;
    int *pow;  
};


struct Polynomial {
    char *vars;
    int num_terms; 
    int num_vars;
    Term *terms; 
};


/**
 * @brief frees the polynomial and all internal structures
 *
 * @param poly pointer to the polynomial
 *
 * @return nothing
 **/
void free_polynomial(Polynomial *poly);

void to_string(Polynomial *poly); 
#endif 
