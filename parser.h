// Ashley Kumar
// 2013

#ifndef PARSER_H
#define PARSER_H

#include <string> 
#include "polynomial.h"

using namespace std;


/**
 * @brief: given the raw input string, parse it out 
 * into the polyomial structure
 *
 * @param input raw input string
 * @param poly empty polynomial struct; fields need to be allocated
 *
 * @return: 0 on success, 1 on format error
 **/
int parse_polynomial(string input, Polynomial *poly);


/**
 * @brief: given the raw polynomial string, determines the number of terms
 *
 * @param terms count for the number of temrs
 * @param poly the raw string polynomial input
 *
 * @return 0 on success 1 on parse error
 **/ 
int get_num_terms(int *terms, string poly);


/**
 * @brief: parse out the variables being used 
 * 
 * @param vars raw input of varibles, comma separated
 * @param num_polys number of polynomials 
 * @param num_vars number of unique variables
 * @param polys list of pointers to polynomials;
 *
 * @return: 0 on success, 1 on format error
 **/
int parse_vars(string vars, int num_polys, int num_vars, Polynomial **poly);

/** 
 * @brief: processes one term of the polynomial and creates internal structure
 *
 * @param input the string of the term to be parsed
 * @curr_term index of current term being processed
 * @positive the sign of the term
 * @poly pointer to the polynomial being built 
 *
 * @return: 0 on success, 1 on error
 **/
int process_term(string input, int curr_term, bool positive, Polynomial *poly);

#endif 
