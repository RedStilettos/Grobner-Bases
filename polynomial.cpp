#include <string>
#include <iostream>
#include <algorithm>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "polynomial.h"

void free_polynomial(Polynomial *poly){
    for (int i = 0; i < poly->num_terms; i++){
        Term *t = &(poly->terms[i]);
        free(t->pow);
    }
    free(poly->terms);
    free(poly->vars); 
    free(poly); 
}
