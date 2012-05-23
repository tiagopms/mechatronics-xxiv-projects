#ifndef TABLES_HPP
#define TABLES_HPP

#include <ProcessLogger.h>

#include <vector>
#include <armadillo>

#define INSTRUCTION_FILE "instruction.txt"
#define REGISTER_FILE "register.txt"

using namespace std;

map<string, vector<string> > createInstructionTable();
bool useLineInstruction(string line, map <string, vector<string> > *instructionTable);

map<string, string> createRegisterTable();
bool useLineRegister(string line, map <string, string > *registerTable);

#endif
