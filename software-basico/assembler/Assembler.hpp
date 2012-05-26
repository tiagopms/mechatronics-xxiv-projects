#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include <Tables.hpp>
#include <Identifier.hpp>
#include <ProcessLogger.h>


#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <string>
#include <iomanip>
#include <armadillo>

#define INPUT_FILE		"input2.txt"
#define OUTPUT_FILE		"output.txt"

#define NULL_REGISTER	"00000"
#define ZERO_REGISTER	"00000"
#define AT_REGISTER		"00001"
#define NULL_SHANT		"00000"
#define ZERO_IMM		"0000000000000000"
#define SYSCALL_CODE	"00000000000000000000000000001100"

using namespace std;

int main();
bool my_itoa(int value, std::string& buf, int base, unsigned int size, int counter);

void createTables();
bool createSimbolsTable();
bool findLabel(string line, int counter1, int *counter2);

bool assemble();
int reconizeType(string line, int counter);
string assembleTypeR1(string line, int counter);
string assembleTypeR2(string line, int counter);
string assembleTypeR3(string line, int counter);
string assembleTypeI1(string line, int counter);
string assembleTypeI2(string line, int counter);
string assembleTypeI3(string line, int counter);
string assembleTypeI4(string line, int counter, int counter2);
string assembleTypeJ(string line, int counter);
string assembleTypeFloat(string line, int counter);
string assembleTypePseudo1(string line, int counter);
string assembleTypePseudo2(string line, int counter);
string assembleTypePseudo3(string line, int counter, int counter2);
string assembleTypePseudo4(string line, int counter, int counter2);
string assembleTypePseudo5(string line, int counter, int counter2);

bool checkRegister(string registerCode, int counter);

void errorSignal(int errorCode, int line);


#endif
