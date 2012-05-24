#ifndef IDENTIFIER_HPP
#define IDENTIFIER_HPP

#include <ProcessLogger.h>

#include <vector>
#include <armadillo>

#include <boost/regex.hpp>

#define INPUT "input.txt"

using namespace boost;
using namespace std;

pair<bool, string> identifyToken(string token);

pair<bool, vector<string> > identifyTypeR1(string instruction);
pair<bool, vector<string> > identifyTypeR2(string instruction);
pair<bool, vector<string> > identifyTypeR3(string instruction);


#endif
