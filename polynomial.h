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
    int ordering; 
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
     int (*comp)(Term t1, Term t2, int num_vars));

void sort_polynomial(Polynomial *poly); 

int sort_lexicographic(Term t1, Term t2, int num_vars); 

int sort_graded_lexicographic(Term t1, Term t2, int num_vars);

int sort_graded_reverse_lexicographic(Term t1, Term t2, int num_vars); 

void swap(Term *t1, Term *t2); 

Polynomial *copy_poly(Polynomial *p);
void double_poly_array(Polynomial **ps, int *count); 
void double_poly(Polynomial *p); 

Polynomial *empty_poly(int num_vars, int num_terms); 
Polynomial *zero_poly(Polynomial *p);

void deep_reduce_poly(Polynomial **p); 
void reduce_poly(Polynomial *p);
 
Polynomial *add_polys(Polynomial *p1, Polynomial *p2);
Polynomial *subtract_polys(Polynomial *p1, Polynomial *p2); 
Polynomial *multiply_polys(Polynomial *p1, Polynomial *p2); 
void divide_polys(Polynomial *p1, Polynomial *p2, 
                         Polynomial **quot, Polynomial **rem); 
/* void gcd_divide_polys(Polynomial *p1, Polynomial *p2, 
                         Polynomial **quot, Polynomial **rem); 
Polynomial *poly_gcd(Polynomial *p1, Polynomial *p2);  */
void scalar (Polynomial *p, Rational frac); 

void divide_terms(Term *t1, Term *t2, Term *t, int num_vars); 
// void gcd_divide_terms(Term *t1, Term *t2, Term *t, int num_vars); 
bool is_const(Term t, int num_vars); 
bool less_than(Polynomial *p1, Polynomial *p2, int ordering); 
void term_string(Term *t, int num_vars); 
Rational add_frac(Rational r1, Rational r2); 
void reduce_frac(Rational *r); 
void free_term(Term *t); 
void print_terms(Term *terms, int num_terms, int num_vars); 
#endif 
