#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include <Tables.hpp>
#include <Identifier.hpp>
#include <ProcessLogger.h>

#include <vector>
#include <sstream>
#include <string>
#include <armadillo>

#define INPUT_FILE "input.txt"
#define OUTPUT_FILE "output.txt"

using namespace std;

int main();

void createTables();
bool createSimbolsTable();
void findLabel(string line, int counter);

bool assemble();
int reconizeType(string line);
string assembleTypeR1(string line);
string assembleTypeR2(string line, int counter);
string assembleTypeR3(string line);
string assembleTypeI1(string line);
string assembleTypeI2(string line);
string assembleTypeJ(string line);
string assembleTypeFloat(string line);
string assembleTypePseudo(string line);

bool checkRegister(string registerCode, int counter);

void errorSignal(int errorCode, int line);


#endif
