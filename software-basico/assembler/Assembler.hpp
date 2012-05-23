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
string assembleTypeR1(string line, int counter);
string assembleTypeR2(string line, int counter);
string assembleTypeR3(string line, int counter);
string assembleTypeI1(string line, int counter);
string assembleTypeI2(string line, int counter);
string assembleTypeJ(string line, int counter);
string assembleTypeFloat(string line, int counter);
string assembleTypePseudo1(string line, int counter);
string assembleTypePseudo2(string line, int counter);
string assembleTypePseudo3(string line, int counter);
string assembleTypePseudo4(string line, int counter);
string assembleTypePseudo5(string line, int counter);

bool checkRegister(string registerCode, int counter);

void errorSignal(int errorCode, int line);


#endif
