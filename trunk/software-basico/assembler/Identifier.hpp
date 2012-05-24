#ifndef IDENTIFIER_HPP
#define IDENTIFIER_HPP

#include <ProcessLogger.h>

#include <vector>
#include <armadillo>

#include <boost/regex.hpp>

#define INPUT "input.txt"

using namespace boost;
using namespace std;

bool identifyToken(string token);

void identifyTypeR1(string token);
void identifyTypeR2(string token);

#endif
