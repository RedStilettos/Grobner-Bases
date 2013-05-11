#include <string>
#include <iostream>
#include <algorithm>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "polynomial.h"

using namespace std;


bool isWhite(char c) {
    return c == ' ';
}

int parse_polynomial(string input, Polynomial *poly){
    int num_terms = 0;
    int start = 0;
    int end = 0;
    int curr_term = 0; 
    bool inpoly = false; 
    bool positive = true; 

    // remove white space
    input.erase(remove_if(input.begin(), input.end(), &isWhite), input.end());
 
    get_num_terms(&num_terms, input);
    poly->terms = (Term *) malloc(sizeof(Term) * num_terms);  
    poly->num_terms = num_terms; 
 
    // parse by hand
    // keep a pointer to where you are in the char * array
    // basically check from +- by hand
    for (size_t i = 0; i < input.length(); i++){
        switch (input[i]) {
            case '+':
                if (inpoly) {
                    end = i-1;
                    inpoly = false;     
                    process_term(input.substr(start, end-start+1), curr_term, positive, poly);
                    curr_term++;
                }
                positive = true; 
                break;
            case '-':
                if (inpoly) {
                    end = i-1;
                    inpoly = false; 
                    process_term(input.substr(start, end-start+1), curr_term, positive, poly);
                    curr_term++; 
                }
                positive = false; 
                break; 
            default:
                if (!inpoly) {
                    start = i;
                    inpoly = true; 
                }
                break; 
        }        
    }
    // parase final term)
    if (inpoly){
        process_term(input.substr(start, (input.length()-start)), curr_term, positive, poly);   
        curr_term++; 
    }
    return 0; 
}


int process_term(string input, int curr_term, bool positive, Polynomial *poly){
    // we'll increment num_terms here and use it to help process the polynomial
    Term *t = &(poly->terms[curr_term]);
    string num;  
    size_t j = 0;
    int c;
    int chars_processed = 0;
    char var;
    size_t pos;
    size_t pos_len;
    t->pow = (int *) malloc(sizeof(int) * poly->num_vars);

    //process the coefficient
    if (isdigit(input[0])){
         while (isdigit(input[j]) && j < input.length()){
            j++; 
         }
         num = input.substr(0, j);
         c = atoi(num.c_str()); 
    }
    else c = 1;
    if (!positive) c = -1*c; 
    t->coeff.num = c;
    t->coeff.den = 1; 

    // look for each variable in the input string
    for (int i = 0; i < poly->num_vars; i++){
        var = poly->vars[i];
        pos = input.find(var);
        if (pos != string::npos) {
            // check to see if it has an exponent specified 
            if (pos != input.length() && input[pos+1] == '^') {
                pos_len = 0;
                while (isdigit(input[pos+2+pos_len]) &&(pos+2+pos_len) < input.length()) {
                    pos_len++; 
                }
                t->pow[i] = atoi(input.substr(pos+2, pos_len).c_str());
                chars_processed += 2+pos_len;
            }
            else {
                chars_processed += 1; 
                t->pow[i] = 1; 
            }
        }
        else {
            // this var is not in this term, i.e. has a exponent of 0
            t->pow[i] = 0;
        }
                
    } 
    return (input.length() - chars_processed); 
}


int get_num_terms(int *terms, string poly){
    // start at positiion 1, because we don't want to count a leading sign
    for (size_t i = 1; i < poly.length(); i++){
        if ((poly[i] == '+') || (poly[i] == '-')){
            (*terms)++;
        }
    }

    // num terms is one more than the number of operators
    (*terms)++;  
    return 0;
}

// MUST ENTER THIS DATA FOR ALL POLYNOMIAL TODO
int parse_vars(string vars, int num_polys, int num_vars, Polynomial **polys){
    bool var = true; 
    int count = 0;

    for (size_t i = 0; i < vars.length(); i++){
        // expecting var, not comma
        if (vars[i] == ',' && var) return 1;

        else if (vars[i] == ',' && !var) var = !var; 

        // expecting comma, not a var
        else if(isalpha(vars[i]) && !var) return 1;

        // expecting a var, make sure it's not in the list, then insert
        else if(isalpha(vars[i]) && var){
            if (count >= num_vars) return 1;
 
            for (int j = 0; j < count; j++){
                // check for duplicate vars, choose first poly since they
                // all share the same variables 
                if (vars[i] == polys[0]->vars[j]){
                    return 1; 
                }
            }
            for (int j = 0; j < num_polys; j++) {
                polys[j]->vars[count] = vars[i];
            }
            count++;
            var = !var;  
        }
    }
    for (int j = 0; j < num_polys; j++){
        polys[j]->num_vars = num_vars; 
    }
    return (num_vars - count); 
}

