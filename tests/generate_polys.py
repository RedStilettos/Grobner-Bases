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

#test(5, 1, 5, 7, 10, 10, ['x', 'y', 'z'], "test1.txt")
test(2,1,10*1000,11*1000, 1000, 10*1000, ['a','b','c','d','e'], "add1.txt"); 
