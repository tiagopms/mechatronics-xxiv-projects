#ifndef IDENTIFIER_HPP
#define IDENTIFIER_HPP

#include <ProcessLogger.h>

#include <vector>
#include <armadillo>

#include <boost/regex.hpp>

#define INPUT "input.txt"

using namespace boost;
using namespace std;

pair<int, string> identifyToken(string token);

pair<bool, vector<string> > identifyTypeR1(string instruction);
pair<bool, vector<string> > identifyTypeR2(string instruction);
pair<bool, vector<string> > identifyTypeR3(string instruction);

pair<bool, vector<string> > identifyTypeI1(string instruction);
pair<bool, vector<string> > identifyTypeI2(string instruction);
pair<bool, vector<string> > identifyTypeI3(string instruction);

pair<bool, vector<string> > identifyTypeJ(string instruction);

pair<bool, vector<string> > identifyTypeFloat(string instruction);

pair<bool, vector<string> > identifyTypePseudo1(string instruction);
pair<bool, vector<string> > identifyTypePseudo2(string instruction);
pair<bool, vector<string> > identifyTypePseudo3(string instruction);
pair<bool, vector<string> > identifyTypePseudo4(string instruction);


#endif
