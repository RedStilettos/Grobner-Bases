// Ashley Kumar
// 2013

#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#define LEX 1
#define GRLEX 2
#define GREVLEX 3

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

void polynomial_quicksort(Term *terms, int num_terms, int num_vars, 
     int (*comp)(Term *t1, Term *t2, int num_vars));

void sort_polynomial(Polynomial *poly, int ordering); 

int sort_lexicographic(Term *t1, Term *t2, int num_vars); 

int sort_graded_lexicographic(Term *t1, Term *t2, int num_vars);

int sort_graded_reverse_lexicographic(Term *t1, Term *t2, int num_vars); 

void swap(Term *t1, Term *t2); 
#endif 
