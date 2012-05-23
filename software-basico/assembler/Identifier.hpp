#ifndef IDENTIFIER_HPP
#define IDENTIFIER_HPP

#include <ProcessLogger.h>

#include <vector>
#include <armadillo>

#include <boost/regex.hpp>

#define INPUT "input.txt"

using namespace boost;
using namespace std;

void identifyTypeR(string token);

void identifyToken(string token);


#endif
