#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include <ProcessLogger.h>

#include <vector>
#include <armadillo>

#define INPUT "input.txt"

using namespace std;

int main();

void createTables();
void createSimbolsTable();

bool assemble();
int reconizeType(string line);
void assembleTypeR(string line);
void assembleTypeI(string line);
void assembleTypeJ(string line);

string registerCode(string registerUsed);

void errorSignal(int errorCode, int counter);


#endif
