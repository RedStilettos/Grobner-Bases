// Ashley Kumar
// 2013

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
 * @param num_vars number of unique variables
 * @param poly empty polynomial struct;
 *
 * @return: 0 on success, 1 on format error
 **/
int parse_vars(string vars, int num_vars, Polynomial *poly);



