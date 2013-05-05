#include <string>
#include <iostream>
#include <algorithm>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "polynomial.h"


void sort_polynomial(Polynomial *poly) {
    int (*comparator) (Term t1, Term t2, int num_vars);
    int ordering = poly->ordering; 
    // use grevlex by default 
    if (ordering == 0) {
        ordering = 3; 
    }

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
        if (comp(terms[i], pivot, num_vars) == 1) {
            swap(&terms[i], &terms[store]); 
            store += 1;
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


// compares polynomials term by term, higher power is larger
int sort_lexicographic(Term t1, Term t2, int num_vars){
    for (int i = 0; i < num_vars; i++){
        if (t1.pow[i] > t2.pow[i]) return 1;
        if (t1.pow[i] < t2.pow[i]) return -1;  
    }
    return 0; 
}

// compares total powers, then term by term 
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

// compares total powers, then term by term in reverse order 
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


// return a copy of the given polynomial 
Polynomial *copy_poly(Polynomial *p){
    Polynomial *copy = empty_poly(p->num_vars, p->num_terms); 
    copy->ordering = p->ordering; 
    memcpy(copy->vars, p->vars, sizeof(char) * p->num_vars); 
    for (int i = 0; i < p->num_terms; i++) {
        copy->terms[i].coeff.num = p->terms[i].coeff.num; 
        copy->terms[i].coeff.den = p->terms[i].coeff.den;
        memcpy(copy->terms[i].pow, p->terms[i].pow, sizeof(int) * p->num_vars);
    }
    return copy; 
}


// resize array to hold twice as many Polynomial *'s
void double_poly_array(Polynomial **ps, int *count) {
    Polynomial **newps; 
    int old_count; 

    old_count = *count; 
    *count = 2 * *count;

    newps = (Polynomial **) malloc(sizeof(Polynomial *) * *count); 
    for (int i = 0; i < old_count; i++) {
        newps[i] = ps[i]; 
    } 
    
    //free the old list
    free(ps);
    ps = newps;
}


// resizes the existing polynomial to hold twice the terms 
void double_poly(Polynomial *p) {
    Polynomial *newp;

    newp = empty_poly(p->num_vars, 2*p->num_terms); 
    newp->ordering = p->ordering;
    memcpy(newp->vars, p->vars, sizeof(char) * p->num_vars); 
    
    for (int i = 0; i < p->num_terms; i++){
        newp->terms[i].coeff.num = p->terms[i].coeff.num;
        newp->terms[i].coeff.den = p->terms[i].coeff.den;
        memcpy(newp->terms[i].pow, p->terms[i].pow, sizeof(int) * p->num_vars); 
    }

    free_polynomial(p);
    p = newp; 
}  


// create empty polynomial of the right dimensions 
Polynomial *empty_poly(int num_vars, int num_terms) {
    Polynomial *p = (Polynomial *) malloc(sizeof(Polynomial)); 
    p->num_vars = num_vars;
    p->num_terms = num_terms;
    p->vars = (char *) malloc(sizeof(char) * num_vars);
    p->terms = (Term *) malloc(sizeof(Term) * num_terms); 
    
    for (int i = 0; i < num_terms; i++) {
        p->terms[i].pow = (int *) malloc(sizeof(int) * num_vars); 
    }
    return p;
}


// returns the zero polynomial
Polynomial *zero_poly(Polynomial *p) {
    Polynomial *zero = empty_poly(p->num_vars, 1);
    zero->ordering = p->ordering;
    memcpy(zero->vars, p->vars, sizeof(int) * p->num_vars);
    zero->terms[0].coeff.num = 0;
    zero->terms[0].coeff.den = 1;
    for (int i = 0; i < p->num_vars; i++){
        zero->terms[0].pow[i] = 0; 
    }
    return zero;  
}


// combines terms, gets ried of any 0 terms
// actually recreates the polynomial with accurate space
void deep_reduce_poly(Polynomial **orig) {
    int orig_num; 
    Polynomial *newp; 
    Polynomial *p = *orig;
    orig_num = p->num_terms;

    // just lazy reduce, and then resize if necessary
    reduce_poly(p); 
    // no reduction needed
    if (p->num_terms == orig_num) return;

    newp = (Polynomial *) malloc(sizeof(Polynomial));
    newp->vars = (char *) malloc(sizeof(char) * p->num_vars);
    memcpy(newp->vars, p->vars, sizeof(char) * p->num_vars); 
    newp->num_vars = p->num_vars;
    newp->num_terms = p->num_terms;
    newp->ordering = p->ordering;     
    newp->terms = (Term *) malloc(sizeof(Term) * p->num_terms);
   
    for (int i = 0; i < p->num_terms; i++) {
        newp->terms[i].coeff.num = p->terms[i].coeff.num;
        newp->terms[i].coeff.den = p->terms[i].coeff.den;
        newp->terms[i].pow = (int *) malloc(sizeof(int) * p->num_vars);
        memcpy(newp->terms[i].pow, p->terms[i].pow, sizeof(int) * p->num_vars);  
    }
    p->num_terms = orig_num;  
    free_polynomial(p);
    *orig = newp;
}


// combines terms, gets rid of any 0 terms 
// does a lazy reduction and just uses current term space
// simple changes term count at the end, so term array may have unused space
void reduce_poly(Polynomial *p) {
    int count = 0; 
    int index = 0; 

    // if there's only one term, there's really nothing to do 
    // just check to make sure it's not zero
    if (p->num_terms == 1) {
        if (p->terms[0].coeff.num == 0) {
            for (int i = 0; i < p->num_vars; i++) {
                p->terms[0].pow[i] = 0; 
            }
        } 
        return; 
    }

    // make sure its sorted to help with reduction 
    sort_polynomial(p);

    // reduction must be done in two phases
    // 1. combine like terms
    // 2. check for 0 terms, some of which may have been created in step 1
   
    // reuse the same array of terms and just count how many
    // unique terms there are 
    // first term will already be in it's correct place
    count = 1;  
    for (int j = 1; j < p->num_terms; j++) {
        // not the same, copy it over
        if (memcmp(p->terms[j].pow, p->terms[j-1].pow, sizeof(int)*p->num_vars)) {
            p->terms[count].coeff.num = p->terms[j].coeff.num;
            p->terms[count].coeff.den = p->terms[j].coeff.den;
            memcpy(p->terms[count].pow, p->terms[j].pow, sizeof(int)*p->num_vars);  
            count++; 
        }
        // it is the same, copy it over because it must already have a slot
        else {
            Rational r = add_frac(p->terms[count-1].coeff, p->terms[j].coeff);
            p->terms[count-1].coeff.num = r.num;
            p->terms[count-1].coeff.den = r.den; 
        } 
    }

    // now check for any terms that went to 0 by just copying over
    // all the terms that are not 0
    for (int j = 0; j < count; j++){
        if (p->terms[j].coeff.num != 0) {
            p->terms[index].coeff.num = p->terms[j].coeff.num;
            p->terms[index].coeff.den = p->terms[j].coeff.den;
            memcpy(p->terms[index].pow, p->terms[j].pow, sizeof(int) * p->num_vars);
            index++;   
        }
    }
    
    // check for just the zero polynomial 
    // and just set all powers to the constant 0
    if (index == 0) {
        for (int i = 0; i < p->num_vars; i++) {
            p->terms[0].pow[i] = 0;
            p->num_terms = 1;
        }
        return;
    }
    p->num_terms = index;
}


// finds sum of two polynomials, returns a totally new one  
Polynomial *add_polys(Polynomial *p1, Polynomial *p2) {  
    Polynomial *sum; 
    int index; 

    // conservatively allocate enough space, then just call reduce 
    sum = empty_poly(p1->num_vars, p1->num_terms + p2->num_terms); 

    sum->ordering = p1->ordering;
    memcpy(sum->vars, p1->vars, sizeof(char) * p1->num_vars); 
 
    // copy over p1 and p2
    for (int i = 0; i < p1->num_terms; i++) {
        sum->terms[i].coeff.num = p1->terms[i].coeff.num;
        sum->terms[i].coeff.den = p1->terms[i].coeff.den;
        memcpy(sum->terms[i].pow, p1->terms[i].pow, sizeof(int)*p1->num_vars); 
    }
    for (int i = 0; i < p2->num_terms; i++){
        index = i + p1->num_terms; 
        sum->terms[index].coeff.num = p2->terms[i].coeff.num;
        sum->terms[index].coeff.den = p2->terms[i].coeff.den;
        memcpy(sum->terms[index].pow, p2->terms[i].pow, sizeof(int)*p2->num_vars);
    }
    deep_reduce_poly(&sum);
    return sum;  
}


// subtracts p2 from p1
Polynomial *subtract_polys(Polynomial *p1, Polynomial *p2) {
    // just make everything in the p2 negative, then call add 
    for (int i = 0; i < p2->num_terms; i++){
        p2->terms[i].coeff.num *= -1; 
    }
    return add_polys(p1, p2); 
}


// multiples p1 and p2
Polynomial *multiply_polys(Polynomial *p1, Polynomial *p2) {
    Polynomial *prod;
    Term t1, t2; 
    int index = 0;

    //conservatively allocate product space 
    prod = empty_poly(p1->num_vars, p1->num_terms * p2->num_terms);
    memcpy(prod->vars, p1->vars, sizeof(char)*p1->num_vars);
    prod->ordering = p1->ordering;

    // runs in O(n^2) time...could probably be better 
    for (int i = 0; i < p1->num_terms; i++){
        t1 = p1->terms[i];         
        for (int j = 0; j < p2->num_terms; j++){
            t2 = p2->terms[j]; 
            index = i * (p2->num_terms) + j;
            prod->terms[index].coeff.num = t1.coeff.num * t2.coeff.num;
            prod->terms[index].coeff.den = t1.coeff.den * t2.coeff.den;
            for (int k = 0; k < p2->num_vars; k++){
                prod->terms[index].pow[k] = t1.pow[k] + t2.pow[k];
            }
        }
    }
    deep_reduce_poly(&prod); 
    return prod;   
}

// does polynomial long division, p1 divided by p2 
void divide_polys(Polynomial *p1, Polynomial *p2, 
                         Polynomial **quot, Polynomial **rem) {
    int max = 2 * p2->num_terms;
    int count = 0; 
    Polynomial *res;
    Polynomial *old_temp, *new_temp; 
    Polynomial *old_div, *new_div; 
    Polynomial **quots; 

    
    // need to allocate some polynomails, let's just assume we need
    // double the space of the dividend and then adjust if it goes over
    *quot = empty_poly(p1->num_vars, 1);
    divide_terms(&p1->terms[0], &p2->terms[0], &((*quot)->terms[0]), p1->num_vars);
    
    // not divisible from the get go, so the quot is 0
    // and the remainder is all of p1 
    if ((*quot)->terms[0].coeff.num == 0) {
        *rem = copy_poly(p1);  
        return; 
    }
    
    // the first term was not 0, so we need to set this polynomial in our list of
    // partial quotients. don't forget to malloc space for the list
    quots = (Polynomial **) malloc(sizeof(Polynomial *) * max);
    quots[count] = *quot; 
    count++; 

    // copy the divisor 
    old_div = copy_poly(p1); 
    while(true) {
        res = multiply_polys(quots[count-1], p2);
        
        // get the new dividend
        new_div = subtract_polys(old_div, res); 
        sort_polynomial(new_div);  
        free(old_div);
        
        // see if it can be divided
        quots[count] = empty_poly(p1->num_vars, 1); 
        divide_terms(&new_div->terms[0], &p2->terms[0], 
                     &quots[count]->terms[0], p1->num_vars);  
        if (quots[count]->terms[0].coeff.num == 0) {
            res = new_div;
            break;  
        }
       
        // it divided, time to loop again
        count += 1;
        free(res);
        old_div = new_div;   

        // increase solutions array ?
        if (count == max-1){
            double_poly_array(quots, &max); 
        } 
    }   
    // sum everything in the quotes list
    old_temp = zero_poly(p1); 
    for (int i = 0; i < count; i++) {
        new_temp = add_polys(old_temp, quots[i]);
        free(old_temp);
        free(quots[i]); 
        old_temp = new_temp;  
    }  
    free(quots); 
 
    // set the quotient and remainder 
    *quot = new_temp; 
    *rem = res; 
}


// does polynomial long division, p1 divided by p2 
/* void gcd_divide_polys(Polynomial *p1, Polynomial *p2, 
                         Polynomial **quot, Polynomial **rem) {
    int max = 2 * p2->num_terms;
    int count = 0; 
    Polynomial *res;
    Polynomial *old_temp, *new_temp; 
    Polynomial *old_div, *new_div; 
    Polynomial **quots; 

    
    // need to allocate some polynomails, let's just assume we need
    // double the space of the dividend and then adjust if it goes over
    *quot = empty_poly(p1->num_vars, 1);
    gcd_divide_terms(&p1->terms[0], &p2->terms[0], &((*quot)->terms[0]), p1->num_vars);
    
    // not divisible from the get go, so the quot is 0
    // and the remainder is all of p1 
    if ((*quot)->terms[0].coeff.num == 0) {
        *rem = copy_poly(p1);  
        return; 
    }
    
    // the first term was not 0, so we need to set this polynomial in our list of
    // partial quotients. don't forget to malloc space for the list
    quots = (Polynomial **) malloc(sizeof(Polynomial *) * max);
    quots[count] = *quot; 
    count++; 

    // copy the divisor 
    old_div = copy_poly(p1); 
    while(true) {
        res = multiply_polys(quots[count-1], p2);
        
        // get the new dividend
        new_div = subtract_polys(old_div, res); 
        sort_polynomial(new_div);  
        free(old_div);
        
        // see if it can be divided
        quots[count] = empty_poly(p1->num_vars, 1); 
        gcd_divide_terms(&new_div->terms[0], &p2->terms[0], 
                     &quots[count]->terms[0], p1->num_vars);  
        if (quots[count]->terms[0].coeff.num == 0) {
            res = new_div;
            break;  
        }
       
        // it divided, time to loop again
        count += 1;
        free(res);
        old_div = new_div;   

        // increase solutions array ?
        if (count == max-1){
            double_poly_array(quots, &max); 
        } 
    }   
    // sum everything in the quotes list
    old_temp = zero_poly(p1); 
    for (int i = 0; i < count; i++) {
        new_temp = add_polys(old_temp, quots[i]);
        free(old_temp);
        free(quots[i]); 
        old_temp = new_temp;  
    }  
    free(quots); 
 
    // set the quotient and remainder 
    *quot = new_temp; 
    *rem = res; 
}
*/

// finds the GCD of polynomials p1 and p2 
// TODO what happens if p2 is zero? can this happen?
/* Polynomial *poly_gcd(Polynomial *p1, Polynomial *p2) {
    Polynomial *f, *g, *q, *r;    
    f = copy_poly(p1);
    g = copy_poly(p2);
    r = NULL;
    q = NULL; 

    while(true) {
        divide_polys(f, g, &q, &r);
        
        if (r->terms[0].coeff.num == 0){
            free(f);
            free(q);
            free(r);
            return g; 
        } 

        // need to loop again
        free(f);
        f = g; 
        g = r;
        free(q); 
    }
    // should never get to this return 
    return g;   
} */



/* Polynomial poly_gcd(Polynomial *p1, Polynomial *p2) {
    Polynomial *g, *quot, *rem;
    printf("p1 is\n"); to_string(p1);
    printf("p2 is \n"); to_string(p2); 

    if (p2->terms[0].coeff.num == 0) {
        return copy_poly(p1); 
    }

    divide_polys(p1, p2, &quot, &rem);
    printf("quot and rem\n");
    to_string(quot);     
    to_string(rem); 
    
    if (!less_than(rem, p1, p1->ordering)) {
        printf( "wahhhh");
    }

    free(quot);  
    g = poly_gcd(p2, rem); 
    free(rem);
    return g; 
}
*/

// returns true if degree leading term p1 < degree leading term p2
bool less_than(Polynomial *p1, Polynomial *p2, int ordering) {

    if (ordering == LEX){
        for (int i = 0; i < p1->num_vars; i++){
            if (p1->terms[0].pow[i] < p2->terms[0].pow[i]) return true;
            else if (p1->terms[0].pow[i] > p2->terms[0].pow[i]) return false;
        } 
        return false;
    }

    else if (ordering == GRLEX) {
        int sum1=0, sum2=0; 
        for (int i = 0; i < p1->num_vars; i++){
            sum1 += p1->terms[0].pow[i];
            sum2 += p2->terms[0].pow[i];
        }
        if (sum1 < sum2) return true;
        return less_than(p1, p2, LEX); 
    }

    else if (ordering == GREVLEX) {
        int sum1=0, sum2=0; 
        for (int i = 0; i < p1->num_vars; i++){
            sum1 += p1->terms[0].pow[i];
            sum2 += p2->terms[0].pow[i];
        }
        if (sum1 < sum2) return true; 
        for (int i = p1->num_vars -1; i >= 0; i--){
            if (p1->terms[0].pow[i] > p2->terms[0].pow[i]) return true; 
            else if (p1->terms[0].pow[i] < p2->terms[0].pow[i]) return false;
        } 
        return false; 
    }
    return true;  
}


// multiply the existing polynomial by a scalar
void scalar(Polynomial *p, Rational frac){
    for (int i = 0; i < p->num_terms; i++){
        Rational r = p->terms[i].coeff; 
        r.num = r.num * frac.num;
        r.den = r.den * frac.den;
        reduce_frac(&r); 
        p->terms[i].coeff.num = r.num;
        p->terms[i].coeff.den = r.den; 
    }    
}


void to_string(Polynomial *poly){
    Term *term;
    for (int i = 0; i < poly->num_terms; i++){
        term = &(poly->terms[i]);
        if (term->coeff.den == 1){
            if (is_const(*term, poly->num_vars)) {
                printf("%+d", term->coeff.num); 
            }
            else if (i == 0 && term->coeff.num != 1) {
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


// computes t1 divided by t2
// dont allow negative exponents  
void divide_terms(Term *t1, Term *t2, Term *t, int num_vars) {
    int power; 

    t->pow = (int *) malloc(sizeof(int) * num_vars);

    t->coeff.num = (t1->coeff.num / t2->coeff.num);
    t->coeff.den = t1->coeff.den * t2->coeff.den; 
    for (int i = 0; i < num_vars; i++) {
        power = t1->pow[i] - t2->pow[i];

        // don't allow negative terms  
        if (power < 0) {
            t->coeff.num = 0;
            return;     
        }
        t->pow[i] = power;   
    }
}


/* void gcd_divide_terms(Term *t1, Term *t2, Term *t, int num_vars) {
    int power; 

    t->pow = (int *) malloc(sizeof(int) * num_vars);

    t->coeff.num = t1->coeff.num * t2->coeff.den;
    t->coeff.den = t1->coeff.den * t2->coeff.num;
    reduce_frac(&t->coeff); 
    for (int i = 0; i < num_vars; i++) {
        power = t1->pow[i] - t2->pow[i];

        // don't allow negative terms  
        if (power < 0) {
            t->coeff.num = 0;
            return;     
        }
        t->pow[i] = power;   
    }
} */



bool is_const(Term t, int num_vars){
    for (int i = 0; i < num_vars; i++){
        if (t.pow[i] != 0) return false;
    }
    return true; 
}

Rational add_frac(Rational r1, Rational r2) {
    Rational r; 
    r.den = r1.den * r2.den;
    r.num = r1.num * r2.den + r2.num * r1.den; 
    reduce_frac(&r);
    return r; 
} 


void reduce_frac(Rational *r){
    int a = r->num;
    int b = r->den; 
    int temp = 0;
    
    if (a == 0) return;
  
    while (b != 0) {
        temp = b;
        b = a % temp;
        a = temp;         
    }
    r->num = r->num / a;
    r->den = r->den / a; 
}


void free_term(Term *t) {
    free(t->pow);
    free(t); 
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
