#include <string>
#include <iostream>
#include <algorithm>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>
#include <time.h> 

#include "polynomial.h"

void print_time(clock_t start, clock_t end){
    printf("done in %.4fs\n", ((float)(end-start))/CLOCKS_PER_SEC); 
}

void do_stuff(){
    int count = 0;
    for (int i = 0; i < 100*100; i++){
        count += i;
    }
}

void do_pointless(Polynomial **ps, int num_polys){
    clock_t start, end;
    start = clock();
    #pragma omp parallel for schedule(static, 1) 
    for (int i = 0; i < num_polys; i++){
        printf("you suck\n"); 
    }
    end = clock(); 
    print_time(start, end); 
}

void par_test(){
    clock_t start, end;
    printf("par test\n");
    start = clock();
    #pragma omp parallel for schedule(dynamic)  
    for (int i = 0; i < 10000; i++){
        do_stuff();
    } 
    end = clock(); 
    print_time(start, end); 
}

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
    Polynomial *copy = empty_poly(p->num_vars, p->num_terms, p->vars); 
    copy->ordering = p->ordering; 
    memcpy(copy->vars, p->vars, sizeof(char) * p->num_vars); 
    for (int i = 0; i < p->num_terms; i++) {
        copy->terms[i].coeff.num = p->terms[i].coeff.num; 
        copy->terms[i].coeff.den = p->terms[i].coeff.den;
        memcpy(copy->terms[i].pow, p->terms[i].pow, sizeof(int) * p->num_vars);
    }
    return copy; 
}


// returns a copy of an array of polynomials
Polynomial **copy_poly_array(Polynomial **ps, int count){
    Polynomial **newps;

    newps = (Polynomial **) malloc(sizeof(Polynomial *) * count);
    for (int i = 0; i < count; i++){
        newps[i] = copy_poly(ps[i]); 
    }
    return newps;  
}


// resize array to hold twice as many Polynomial *'s
Polynomial **double_poly_array(Polynomial **ps, int *count) {
    Polynomial **newps; 
    int old_count; 

    old_count = *count; 
    *count = 2 * old_count;

    newps = (Polynomial **) malloc(sizeof(Polynomial *) * *count); 
    for (int i = 0; i < old_count; i++) {
        newps[i] = ps[i]; 
    } 
    
    //free the old list
    free(ps);
    return newps;
}


// resizes the existing polynomial to hold twice the terms 
void double_poly(Polynomial *p) {
    Polynomial *newp;

    newp = empty_poly(p->num_vars, 2*p->num_terms, p->vars); 
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
Polynomial *empty_poly(int num_vars, int num_terms, char *vars) {
    Polynomial *p = (Polynomial *) malloc(sizeof(Polynomial)); 
    p->num_vars = num_vars;
    p->num_terms = num_terms;
    p->vars = (char *) malloc(sizeof(char) * num_vars);
    p->terms = (Term *) malloc(sizeof(Term) * num_terms); 
    
    memcpy(p->vars, vars, sizeof(char) * num_vars); 
    for (int i = 0; i < num_terms; i++) {
        p->terms[i].pow = (int *) malloc(sizeof(int) * num_vars); 
    }
    return p;
}


// returns the zero polynomial
Polynomial *zero_poly(Polynomial *p) {
    Polynomial *zero = empty_poly(p->num_vars, 1, p->vars);
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
    printf("deep reduce time\n"); 
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
            if (count != j){
                p->terms[count].coeff.num = p->terms[j].coeff.num;
                p->terms[count].coeff.den = p->terms[j].coeff.den;
                memcpy(p->terms[count].pow, p->terms[j].pow, sizeof(int)*p->num_vars);
            }  
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
            if (index != j){
                p->terms[index].coeff.num = p->terms[j].coeff.num;
                p->terms[index].coeff.den = p->terms[j].coeff.den;
                memcpy(p->terms[index].pow, p->terms[j].pow, sizeof(int) * p->num_vars);
            }
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


// adds a list of polynomials
Polynomial *add_all_polys(Polynomial **polys, int num_polys, bool doit){
    Polynomial *old, *res;
    int threads = omp_get_max_threads();  
    int chunk = num_polys / threads;
    int start, end;
    //printf("threads %d\n", threads);  
    if (threads == 1 || num_polys < threads){
        res = zero_poly(polys[0]); 
        for (int i = 0; i < num_polys; i++){
            old = res;
            res = add_polys(old, polys[i]);
            free_polynomial(old); 
        }
    }

    else{
        Polynomial ** ps; 
        ps = (Polynomial **) malloc(sizeof(Polynomial *) * threads); 
        #pragma omp parallel for schedule(static, 1) 
        for (int i = 0; i < threads; i++){
            start = i * chunk;
            end = (i+1) * chunk;
            if (i == threads-1){
                ps[i] = add_all_polys(&(polys[i]), num_polys-start, doit); 
            }
            else {
                ps[i] = add_all_polys(&(polys[i]), chunk, doit); 
            }
        }    
        #pragma omp barrier
        res = copy_poly(ps[0]); 
        free_polynomial(ps[0]); 
        for (int i = 1; i < threads; i++){
            old = res; 
            res = add_polys(old, ps[i]);
            free_polynomial(old); 
            free_polynomial(ps[i]); 
        }
        free(ps);     
    }
    return res; 
}


// finds sum of two polynomials, returns a totally new one  
Polynomial *add_polys(Polynomial *p1, Polynomial *p2) {  
    Polynomial *sum; 
    int index;
    clock_t start, end;
    start = clock();   

    // conservatively allocate enough space, then just call reduce 
    sum = empty_poly(p1->num_vars, p1->num_terms + p2->num_terms, p1->vars); 

    sum->ordering = p1->ordering;
    memcpy(sum->vars, p1->vars, sizeof(char) * p1->num_vars); 
 
    // copy over p1 and p2
    #pragma omp parallel for schedule(static, 1)  
    for (int i = 0; i < p1->num_terms; i++) {
        sum->terms[i].coeff.num = p1->terms[i].coeff.num;
        sum->terms[i].coeff.den = p1->terms[i].coeff.den;
        memcpy(sum->terms[i].pow, p1->terms[i].pow, sizeof(int)*p1->num_vars); 
    }
    #pragma omp parallel for schedule(static, (p2->num_terms+6)/6)
    for (int i = 0; i < p2->num_terms; i++){
        index = i + p1->num_terms; 
        sum->terms[index].coeff.num = p2->terms[i].coeff.num;
        sum->terms[index].coeff.den = p2->terms[i].coeff.den;
        memcpy(sum->terms[index].pow, p2->terms[i].pow, sizeof(int)*p2->num_vars);
    }
    deep_reduce_poly(&sum);
    end = clock(); 
    //printf("done in %.6fs\n", ((float)(end-start))/CLOCKS_PER_SEC); 
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


// multiply the polynomial by a particular term 
Polynomial *term_multiply_poly(Polynomial *p, Term *t) {
    Polynomial *prod;
    Term oldt, newt;    

    prod = empty_poly(p->num_vars, p->num_terms, p->vars);
    memcpy(prod->vars, p->vars, sizeof(char)*p->num_vars);
    prod->ordering = p->ordering;
    
    for (int i = 0; i < p->num_terms; i++){
        oldt = p->terms[i];
        newt = prod->terms[i];
        prod->terms[i].coeff.num = oldt.coeff.num * t->coeff.num;
        prod->terms[i].coeff.den = oldt.coeff.den * t->coeff.den;
        reduce_frac(&(t->coeff)); 
        for (int j = 0; j < p->num_vars; j++){
            prod->terms[i].pow[j] = t->pow[j] + oldt.pow[j];
        } 
    }
    return prod; 
}


// multiples p1 and p2
Polynomial *multiply_polys(Polynomial *p1, Polynomial *p2) {
    Polynomial *prod;
    Term t1, t2; 
    int index = 0;

    //conservatively allocate product space 
    prod = empty_poly(p1->num_vars, p1->num_terms * p2->num_terms, p1->vars);
    memcpy(prod->vars, p1->vars, sizeof(char)*p1->num_vars);
    prod->ordering = p1->ordering;

    // runs in O(n^2) time...could probably be better 
    //#pragma omp parallel for schedule(static, 1) 
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
        printf("term done %d\n", i); 
    }
    //#pragma omp barrier 
    //printf("IM DONE WITH THE LOOP\n"); 
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
    *quot = empty_poly(p1->num_vars, 1, p1->vars);
    
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
        quots[count] = empty_poly(p1->num_vars, 1, p1->vars); 
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
            quots = double_poly_array(quots, &max); 
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


//computes the grobner basis for a given set of polynomials
Polynomial **grobner_basis(Polynomial **orig_basis, int num_polys, int *basis_size){
    bool not_grobner = true; 
    int space, bound, try_reduce, temp=0;
    int start_index = 1;
    Polynomial *s;
    Polynomial *reduced;    
    Polynomial **basis; 
    
    space = num_polys; 
    *basis_size = num_polys;  

    basis = copy_poly_array(orig_basis, num_polys); 

    // while we do not have a grobner basis, keeping on computing s polynomials
    while(not_grobner){
        not_grobner = false;
        //printf("\nwhile loop time\n"); 
        bound = *basis_size; 
        for (int i = 0; i < bound; i++){
            for (int j = start_index; j < bound; j++){
                if (i == j) continue; // dont check poly with itself
                printf("found s poly for terms f%d and f%d ", i+1, j+1); fflush(stdout);
                s = s_poly(basis[i], basis[j]);
                to_string(s);
        
                // try to reduce it with normal form
                reduced = normal_form(basis, s, *basis_size, *basis_size, &try_reduce);
                //printf("reduced is\n");
                //to_string(reduced); 
                free(s); 

                // check to see how var it was reduced  
                if (reduced->terms[0].coeff.num != 0){
                    if (*basis_size == space) {
                        basis = double_poly_array(basis, &space); 
                    }
                    basis[*basis_size] = reduced;
                    (*basis_size)++;
                    not_grobner = true;  
                }
                else {
                    free(reduced); 
                }
            }
        }
        temp++;
        start_index = bound;
    }
    return basis; 
}


// reduces a polynomial with respect to a set of polynomials F
Polynomial *normal_form(Polynomial **set, Polynomial *orig, 
                        int index, int num_polys, int *reduced){
    Polynomial *res, *temp; 

    // we must try all quotient and remainders
    Polynomial **quots = (Polynomial **) malloc(sizeof(Polynomial *) * num_polys);
    Polynomial **rems = (Polynomial **) malloc(sizeof(Polynomial *) * num_polys);
    printf("reducing... ");
    to_string(orig); 
    res = copy_poly(orig);
    *reduced = 0;
  
    //printf("computing some reduction\n");  
    // try to divide using all things in the set 
    for (int i = 0; i < num_polys; i++){
        if (i == index) continue;
        divide_polys(orig, set[i], &(quots[i]), &(rems[i]));
    }

    //printf("wtf tried to divide everything\n"); 
    // nothing divided out completely the first time
    // keep trying to divide out stuff recursively
    for (int i = 0; i < num_polys; i++){
        if (i == index) continue;
        
        // remainder is 0, return this right away
        if (rems[i]->terms[0].coeff.num == 0){
            free(res);  
            res = copy_poly(rems[i]);
            break;
        }

        // try to divide recursively
        else if (quots[i]->terms[0].coeff.num != 0){
            temp = normal_form(set, rems[i], index, num_polys, reduced);
            free(res); 
            // it reduced all the way
            if (temp->terms[0].coeff.num == 0){
                res = copy_poly(temp);
                break; 
            }
            else{
                res = copy_poly(rems[i]); 
            }
            free(temp); 
        }
    }

    // free all this stuff
    for (int i = 0; i < num_polys; i++) {
        if (i == index) continue; 
        free(quots[i]);
        free(rems[i]); 
    }

    free(quots);
    free(rems); 
    printf("end of normal"); 
    return res; 
}


// reduces the entire set of polynomials, using normal_form to help
Polynomial **reduce_basis(Polynomial **set, int num_polys, int *basis_size){
    int reduced = 0;
    Polynomial *temp;
    Polynomial **basis;
    int *to_remove; 
   
    
    // flag for removal at the end
    to_remove = (int *) malloc(sizeof(int) * num_polys); 
 
    // create empty basis and build it up lazily 
    *basis_size = 0;
    basis = (Polynomial **) malloc(sizeof(Polynomial *) * num_polys);
 
    //printf("let's reduce!!\n");  
    for (int i = 0; i < num_polys; i++){
        //printf("iteration %d\n", i);
        to_string(set[i]);  
        temp = normal_form(set, set[i], i, num_polys, &reduced);
        
        // it was reduced; dont copy over
        if (temp->terms[0].coeff.num == 0) {
            to_remove[i] = 1;
            free_polynomial(temp); 
        }
        else {
            to_remove[i] = 0;
            basis[*basis_size] = temp;
            (*basis_size)++; 
        }

        //printf("back from normal_form\n");  
    }
    return basis; 
}


// returns the S-polynomial 
Polynomial *s_poly(Polynomial *p1, Polynomial *p2){
    Term *t1, *t2, *m1, *m2; 
    Term *lcm, *f1, *f2;

    Polynomial *new1, *new2, *res;

    t1 = leading_term(p1);
    t2 = leading_term(p2);
    m1 = leading_monomial(p1);
    m2 = leading_monomial(p2); 
  
    lcm = term_lcm(m1, m2, p1->num_vars);

    f1 = (Term *) malloc(sizeof(Term));
    f2 = (Term *) malloc(sizeof(Term));
    f1->pow = (int *) malloc(sizeof(int) * p1->num_vars); 
    f2->pow = (int *) malloc(sizeof(int) * p1->num_vars); 

    divide_terms(lcm, m1, f1, p1->num_vars);
    divide_terms(lcm, m2, f2, p1->num_vars); 
    
    // mutiply factors by opposite leading coefficients
    f1->coeff.num = f1->coeff.num * t2->coeff.num;
    f1->coeff.den = f1->coeff.den * t2->coeff.den;
    f2->coeff.num = f2->coeff.num * t1->coeff.num;
    f2->coeff.den = f2->coeff.den * t1->coeff.den;  

    new1 = term_multiply_poly(p1, f1); 
    new2 = term_multiply_poly(p2, f2);

    res = subtract_polys(new1, new2);  

    // free all this intermediate stuff
    free_term(t1); free_term(t2);
    free_term(m1); free_term(m2);
    free_term(f1); free_term(f2);
    free_term(lcm); 
    free_polynomial(new1); free_polynomial(new2);

    sort_polynomial(res); 

    return res;  
}


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
    printf("%d/%d", t->coeff.num, t->coeff.den); 
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


// fins the least common multiple of 2 terms 
Term *term_lcm(Term *a, Term *b, int num_vars){
    Term *lcm;
    lcm = (Term *) malloc(sizeof(Term)); 
    lcm->pow = (int *) malloc(sizeof(int));
    lcm->coeff.num = 1;
    lcm->coeff.den = 1; 

    for (int i = 0; i < num_vars; i++){
        if (a->pow[i] > b->pow[i]) lcm->pow[i] = a->pow[i];
        else lcm->pow[i] = b->pow[i]; 
    } 
    return lcm; 
}


//finds the leading term of a Polynomial 
Term *leading_term(Polynomial *p) {
    Term *t, *l;
    t = (Term *) malloc(sizeof(Term)); 
    t->pow = (int *) malloc(sizeof(int) * p->num_vars);  
    
    // sort just to be sure
    sort_polynomial(p);
    l = &(p->terms[0]); 
    t->coeff.num = l->coeff.num;
    t->coeff.den = l->coeff.den;
    
    memcpy(t->pow, l->pow, sizeof(int) * p->num_vars);
    return t; 
}


// finds the leading monomial of a Polynomial 
Term *leading_monomial(Polynomial *p) {
    Term *t;
    t = leading_term(p);
    t->coeff.num = 1;
    t->coeff.den = 1; 
    return t; 
}


// computes t1 divided by t2, to the nearest whole number coefficient
// dont allow negative exponents  
void whole_divide_terms(Term *t1, Term *t2, Term *t, int num_vars) {
    int power; 

    t->coeff.num = (t1->coeff.num / t2->coeff.num);
    t->coeff.den = t1->coeff.den * t2->coeff.den; 
    for (int i = 0; i < num_vars; i++) {
        power = t1->pow[i] - t2->pow[i];

        // don't allow negative terms  
        if (power < 0) {
            t->coeff.num = 0;
            return; //TODO should really set all exponents to 0  
        }
        t->pow[i] = power;   
    }
}


// computes t1 divided by t2, with fractional coefficients 
void divide_terms(Term *t1, Term *t2, Term *t, int num_vars) {
    int power; 

    t->coeff.num = t1->coeff.num * t2->coeff.den;
    t->coeff.den = t2->coeff.num * t1->coeff.den;
    reduce_frac(&(t->coeff));  
   
     for (int i = 0; i < num_vars; i++) {
        power = t1->pow[i] - t2->pow[i];
        // don't allow negative terms; set to 0 and return
        if (power < 0) {
            t->coeff.num = 0;
            for (int j = 0; j < num_vars; j++){
                t->pow[j] = 0;
            }   
            return; 
        }
        t->pow[i] = power;   
    }
}


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

    //negatives should be in the numerator
    if (r->den < 0) {
        r->num *= -1;
        r->den *= -1; 
    } 
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
