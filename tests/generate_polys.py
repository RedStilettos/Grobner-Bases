# a wonderful script to hack out some polynomials 
import random

def make_term(max_power, max_coeff, variables):
    term = ""
    include = 0

    term += str(random.randint(1, max_coeff))
 
    for i in xrange(len(variables)):
        include = random.randint(1,10)    
        # should this term be included
        if (include <= 6):
            power = random.randint(1, max_power)
            var = variables[random.randint(0, len(variables)-1)]
            term += var
            term += "^"
            term += str(power)

    return term   


def make_poly(min_terms, max_terms, max_power, max_coeff, variables):
    poly = ""
    
    num_terms = random.randint(min_terms, max_terms)
    for i in xrange(num_terms):
        poly += make_term(max_power, max_coeff, variables)
        if (random.randint(0,1) == 0):
            poly+=" +"
        else:
            poly+=" -"
    
    # slice of the last sign 
    poly = poly[0:len(poly)-1]
    return poly 


def test(num_polys, ordering, min_terms, max_terms, 
         max_power, max_coeff, variables, filename):
    output = ""
    output += str(num_polys)
    output += "\n"
    output += str(ordering)
    output += "\n"
    output += str(len(variables))
    output += "\n"
    output += ','.join(variables)
    output += "\n"
 
    for i in xrange(0,num_polys):
        output += make_poly(min_terms, max_terms, max_power, max_coeff, variables)
        output += "\n"
    
    output = output[0:len(output)-1]
    print_to_file(output, filename) 


def print_to_file(output, filename):
    f = open(filename, "w")
    f.write(output)
    f.close()

test(10*1000, 1, 1000, 1200, 10, 10, ['x','y','z'], "addmany1.txt")
#test(5, 1, 5, 7, 10, 10, ['x', 'y', 'z'], "test1.txt")
#test(3, 3, 3, 4, 4, 10, ['x', 'y', 'z'], "grobverysmall.txt")
#test(5, 3, 4, 5, 4, 20, ['x', 'y', 'z'], "grobsmall.txt")
#test(10, 3, 12, 14, 5, 20, ['a','b','c','d','e'], "grobmed.txt") 
#test(1000,1,3*1000,4*1000, 10, 100, ['a','b','c','d','e'], "reducesmall.txt"); 
#test(1000,1,10*1000,11*1000, 10, 100, ['a','b','c','d','e'], "reduce.txt"); 
#test(2,1,5,10, 100, 100, ['a','b','c','d','e'], "addsmall.txt"); 
#test(2,1,1000,1005, 100, 100, ['a','b','c','d','e'], "addmed.txt"); 
#test(2,1,100*1000,110*1000, 1000, 10*1000, ['a','b','c','d','e'], "addlarge.txt"); 
