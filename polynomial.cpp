#include <string>
#include <iostream>
#include <algorithm>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "polynomial.h"


void to_string(Polynomial *poly){
    Term *term;

    for (int i = 0; i < poly->num_terms; i++){
        term = &(poly->terms[i]);
        if (term->coeff.den == 1){
            if (term->coeff.num != 1){
                printf("%+d", term->coeff.num);
            }
        }
        else {
            printf("%+d/%d", term->coeff.num, term->coeff.den);
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


void free_polynomial(Polynomial *poly){
    for (int i = 0; i < poly->num_terms; i++){
        Term *t = &(poly->terms[i]);
        free(t->pow);
    }
    free(poly->terms);
    free(poly->vars); 
    free(poly); 
}
