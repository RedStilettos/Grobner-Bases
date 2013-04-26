#include <string>
#include <iostream>
#include <algorithm>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "polynomial.h"


void sort_polynomial(Polynomial *poly, int ordering) {
    int (*comparator) (Term t1, Term t2, int num_vars);

    // default comparator to grevlex
    comparator = &sort_graded_reverse_lexicographic; 

    if (ordering == LEX){
        comparator = &sort_lexicographic; 
    }
    else if (ordering == GRLEX) {
        comparator = &sort_graded_lexicographic; 
    }
    else if (ordering == GREVLEX) {
        comparator = &sort_graded_reverse_lexicographic; 
    }
    polynomial_quicksort(poly->terms, poly->num_terms, poly->num_vars, comparator);  
}


void polynomial_quicksort(Term *terms, int num_terms, int num_vars, int (*comp)(Term t1, Term t2, int num_vars)){
    int pivot_index = num_terms/2;
    Term pivot = terms[pivot_index]; 
    int store = 0;
    
    if (num_terms < 2) return; 
    swap(&terms[pivot_index], &terms[num_terms-1]);

    for (int i = 0; i < num_terms; i++) {
        if (comp(terms[i], pivot, num_vars) == -1) {
            swap(&terms[i], &terms[store]); 
            store += 1;
            print_terms(terms, num_terms, num_vars);  
        }
    }
    swap(&terms[store], &terms[num_terms-1]);

    // sort left half, right half
    polynomial_quicksort(terms, store, num_vars, comp);
    if (store < num_terms-1) {
        polynomial_quicksort(&(terms[store+1]), num_terms-store-1, num_vars, comp); 
    }
}


void swap(Term *t1, Term *t2) {
    Term temp;
    temp = *t1; 
    *t1 = *t2;
    *t2 = temp; 
}


int sort_lexicographic(Term t1, Term t2, int num_vars){
    for (int i = 0; i < num_vars; i++){
        if (t1.pow[i] > t2.pow[i]) return 1;
        if (t1.pow[i] < t2.pow[i]) return -1;  
    }
    return 0; 
}


int sort_graded_lexicographic(Term t1, Term t2, int num_vars) {
    int total_1 = 0;
    int total_2 = 0; 
    
    for (int i = 0; i < num_vars; i++) {
        total_1 += t1.pow[i];
        total_2 += t2.pow[i]; 
    }

    if (total_1 > total_2) return 1;
    if (total_1 < total_2) return -1; 

    return sort_lexicographic(t1, t2, num_vars); 
}


int sort_graded_reverse_lexicographic(Term t1, Term t2, int num_vars){
    int total_1 = 0;
    int total_2 = 0; 
    
    for (int i = 0; i < num_vars; i++) {
        total_1 += t1.pow[i];
        total_2 += t2.pow[i]; 
    }

    if (total_1 > total_2) return 1;
    if (total_1 < total_2) return -1; 

    for (int i = num_vars -1; i >=0; i--){
        if (t1.pow[i] > t2.pow[i]) return -1;
        if (t1.pow[i] < t2.pow[i]) return 1; 
    }
    return 0; 
}


void add(Polynomial *p1, Polynomial *p2) { 
    return; 
}


void to_string(Polynomial *poly){
    Term *term;
    for (int i = 0; i < poly->num_terms; i++){
        term = &(poly->terms[i]);
        if (term->coeff.den == 1){
            if (i == 0 && term->coeff.num != 1) {
                printf("%d", term->coeff.num); 
            }
            else if (i != 0 && term->coeff.num != 1){
                printf("%+d", term->coeff.num);
            }
            else if (i != 0 && term->coeff.num == 1) {
                printf("+"); 
            }
        }
        else if (i != 0){
            printf("%+d/%d", term->coeff.num, term->coeff.den);
        }
        else {
            printf("%d/%d", term->coeff.num, term->coeff.den);
        }   
        for (int j = 0; j < poly->num_vars; j++){
            if (term->pow[j] != 0) {
                if (term->pow[j] == 1) {
                    printf("%c", poly->vars[j]);
                }
                else {
                    printf("%c^%d", poly->vars[j], term->pow[j]); 
                }
            }
        }
        printf(" "); 
    }
    printf("\n"); 
}


void term_string(Term *t, int num_vars){
    for (int i = 0; i < num_vars; i++){
        printf(" x^%d ", t->pow[i]); 
    }
}


void print_terms(Term *terms, int num_terms, int num_vars){
    for (int i = 0; i < num_terms; i++){
        term_string(&terms[i], num_vars); 
    }
    printf("\n");
}


void free_polynomial(Polynomial *poly){
    for (int i = 0; i < poly->num_terms; i++){
        Term *t = &(poly->terms[i]);
        free(t->pow);
    }
    free(poly->terms);
    free(poly->vars); 
    free(poly); 
}
