#include <Assembler.hpp>

map<string, vector<string> > instructionTable;
map<string, string> registerTable;
map<string, string> registerFloatTable;
map<string, int> simbolTable;

int main()
{
	//create tables
	createTables();
	
	//create simbols table
	createSimbolsTable();
	
	//assemble
	assemble();
	
	return 0;
}

void createTables()
{
	//instructions
	instructionTable = createInstructionTable();
	
	//registers
	registerTable = createRegisterTable();
	registerFloatTable = createRegisterFloatTable();
}

bool createSimbolsTable()
{
	int counter1 = 0;
	int counter2 = 0;
	string line;
	string filename = INPUT_FILE;
	
	//open input file
	ifstream cfg(filename.c_str(), ifstream::in);
	if(cfg.fail())
		errorSignal(101, 0);
	
	LOG(LEVEL_WARN) << "Labels Found:";
	
	//loop to go in each line
	while(cfg.good())
	{
		counter1++;
		counter2++;
		//get single line
		getline(cfg, line);
		
		//updates simbols table
		findLabel(line, counter1, &counter2);
	}
	
	//close file
	cfg.close();
	
	return true;
}

bool findLabel(string line, int counter1, int *counter2)
{
	if(line.size() == 0)
		return false;
		
	pair<bool, vector<string> > token = identifyFunction(line);
	string instruction;
	if (token.first == true)
	{
		instruction = token.second[0];
	}
	else
	{
		errorSignal(18, counter1);
		LOG(LEVEL_INFO) << "Line = " << line;
		return 0;//instruction = getInstructionToken();
	}
	
	if(instructionTable[instruction].size() == 0)
	{
		errorSignal(14, counter1);
		LOG(LEVEL_INFO) << "Line = " << line;
		return 0;
	}
	
	string typeString = instructionTable[instruction][0];

	if (typeString[0] == 'P' && (0+typeString[1]-48)>3)
	{
		(*counter2)++;
	}
	
	
	pair <int, string> label = identifyToken(line);
	
	if(label.first == 2)
	{
		if(simbolTable[label.second])
		{
			errorSignal(17, counter1);
			LOG(LEVEL_INFO) << label.second << " already defined";
			return false;
		}
		LOG(LEVEL_INFO) << "\t" << left << setw(20) << label.second << "\tFound in line number = " << *counter2;
		simbolTable[label.second] = *counter2;
	}
	if(label.first == 0)
	{
			errorSignal(25, counter1);
			return false;
	}
	
	return true;
}

bool assemble()
{
	int type, counter = 0, counter2 = 0;
	string line;
	string functionCode;
	string inputFilename = INPUT_FILE;
	string outputFilename = OUTPUT_FILE;
	
	//open input and output file
	ifstream cfg(inputFilename.c_str(), ifstream::in);
	if(cfg.fail())
		errorSignal(102, 0);
	ofstream output(outputFilename.c_str(), ofstream::out);
	if(!output.is_open())
		errorSignal(103, 0);

	LOG(LEVEL_WARN) << "Instructions Found";
	
	//loop to go in each line
	while(cfg.good())
	{
		counter++;
		counter2++;
		//get single line
		getline(cfg, line);
		
		//reconize type of intruction
		type = reconizeType(line, counter);
		
//		LOG(LEVEL_INFO) << "Line number = " << counter;
//		LOG(LEVEL_INFO) << line;
		
		switch(type)
		{
			case 0:
				functionCode = "Error";
				break;
			case 1:
				functionCode = assembleTypeR1(line, counter);
				break;
			case 2:
				functionCode = assembleTypeR2(line, counter);
				break;
			case 3:
				functionCode = assembleTypeR3(line, counter);
				break;
			case 4:
				functionCode = assembleTypeI1(line, counter);
				break;
			case 5:
				functionCode = assembleTypeI2(line, counter);
				break;
			case 6:
				functionCode = assembleTypeI3(line, counter);
				break;
			case 7:
				functionCode = assembleTypeI4(line, counter, counter2);
				break;
			case 8:
				functionCode = assembleTypeJ(line, counter);
				break;
			case 9:
				functionCode = assembleTypeFloat(line, counter);
				break;
			case 10:
				functionCode = assembleTypePseudo1(line, counter);
				break;
			case 11:
				functionCode = assembleTypePseudo2(line, counter);
				break;
			case 12:
				functionCode = assembleTypePseudo3(line, counter, counter2);
				break;
			case 13:
				counter2++;
				functionCode = assembleTypePseudo4(line, counter, counter2);
				break;
			case 14:
				counter2++;
				functionCode = assembleTypePseudo5(line, counter, counter2);
				break;
			case 15:
				functionCode = SYSCALL_CODE;
				break;
			case 30:
				return true;
		}
		
		if(functionCode == "Error")
		{
			if(type == 13 || type == 14)
			{
				LOG(LEVEL_INFO) << left << setw(30) << line << left << setw(10) << functionCode << "\t\t\t\t\tAssemble line = " << counter2-1 << left << setw(79) << "\n" << left << setw(30) << functionCode << setw(10) << "\tLine = " << counter << "\tAssemble line = " << counter2;
			}
			else
			{
				LOG(LEVEL_INFO) << left << setw(30) << line << left << setw(10) << functionCode << setw(10) << "\t\t\tLine = " << counter << "\tAssemble line = " << counter2;
			}
		}
		else
		{
			if(functionCode.size() > 32)
			{
				string function1, function2;
				function1 = functionCode.substr(0,32);
				function2 = functionCode.substr(33,functionCode.length());
				LOG(LEVEL_INFO) << left << setw(30) << line << left << setw(10) << function1 << "\t\t\tAssemble line = " << counter2-1 << left << setw(79) << "\n" << left << setw(30) << function2 << setw(10) << "\tLine = " << counter << "\tAssemble line = " << counter2;
			}
			else
			{
				LOG(LEVEL_INFO) << left << setw(30) << line << left << setw(10) << functionCode << setw(10) << "\tLine = " << counter << "\tAssemble line = " << counter2;
			}
		}
//		LOG(LEVEL_INFO) << "Line code = " << functionCode;
		
		output << functionCode << "\n";
	}
		
	
	//close input and output file
	cfg.close();
	
	return true;
}

int reconizeType(string line, int counter)
{
	if(line.size() == 0)
		return 30;
	
	string instruction;
	string typeString;
	int type = 0;
	
	if(line.size() == 0)
		return 0;
	
	pair<bool, vector<string> > token = identifyFunction(line);
	if (token.first == true)
	{
		instruction = token.second[0];
	}
	else
	{
		errorSignal(18, counter);
		LOG(LEVEL_INFO) << "Line = " << line;
		return 0;;//instruction = getInstructionToken();
	}
	
//	LOG(LEVEL_INFO) << "\t" << instruction << "\t\tFound in line number = " << counter;
	
	if(instructionTable[instruction].size() == 0)
	{
		errorSignal(14, counter);
		LOG(LEVEL_INFO) << "Line = " << line;
		return 0;
	}
	
	typeString = instructionTable[instruction][0];

	if (typeString[0] == 'R')
	{
		type = 0+typeString[1]-48;
	}
	else
	{
		if (typeString[0] == 'I')
		{
			type = 3+typeString[1]-48;
		}
		else
		{
			if (typeString[0] == 'J')
			{
				type = 8;
			}
			else
			{
				if (typeString[0] == 'F')
				{
					type = 9;
				}
				else
				{
					if (typeString[0] == 'P')
					{
						type = 9+typeString[1]-48;
					}
					else
					{
						if (typeString[0] == 'S')
						{
							type = 15;
						}
					}
				}
			}
		}
	}
	
	LOG(LEVEL_DEBUG) << "Tipo de Funcao = " << typeString;
	LOG(LEVEL_DEBUG) << "Tipo de Funcao = " << type;
		
	//if not found send error signal
	if(type == 0)
		errorSignal(2, counter);
	
//	LOG(LEVEL_WARN) << type;
	
	
	return type;
}

//normal type r functions -> add $rd,$rs,$rt
string assembleTypeR1(string line, int counter)
{
	string opCode, rsCode, rtCode, rdCode, funct;
	string function, rd, rs, rt;
	vector<string> parameters;
	bool noErrors = true;
	
	pair<bool, vector<string> > encapsuledParameters = identifyTypeR1(line);
	
	if(encapsuledParameters.first == false)
	{
		LOG(LEVEL_INFO) << "Line = " << line;
		errorSignal(7, counter);
		return "Error";
	}
	
	parameters = encapsuledParameters.second;
	
	//identify function
	function = parameters[0];
	
	//identify opcode, shant and funct
	opCode = instructionTable[function][1];
	funct = instructionTable[function][2];
	
	//identify registers
	rs = parameters[2];
	rt = parameters[3];
	rd = parameters[1];
	
	//get register codes
	rsCode = registerTable[rs];
	rtCode = registerTable[rt];
	rdCode = registerTable[rd];
	
	//check if valid registers
	noErrors *= checkRegister(rsCode, counter);
	noErrors *= checkRegister(rtCode, counter);
	noErrors *= checkRegister(rdCode, counter);
	
	//check for register or imediate errors
	if (noErrors == false)
	{
		return "Error";
	}
	
	//concatenate strings
	stringstream ss;
	ss << opCode << rsCode << rtCode << rdCode << NULL_SHANT << funct;
	string functionCode = ss.str();
	
	//return machine code
	return functionCode;
}

//shift type r functions -> sll $rd,$rt,imm
string assembleTypeR2(string line, int counter)
{
	string opCode, rtCode, rdCode, shant, funct;
	string function, rd, rt, imm;
	vector<string> parameters;
	bool noErrors = true;
	
	pair<bool, vector<string> > encapsuledParameters = identifyTypeR2(line);
	
	if(encapsuledParameters.first == false)
	{
		LOG(LEVEL_INFO) << "Line = " << line;
		errorSignal(7, counter);
		return "Error";
	}
	
	parameters = encapsuledParameters.second;
	
	//identify function
	function = parameters[0];
	imm = parameters[3];
	
	//identify opcode, shant and funct
	opCode = instructionTable[function][1];
	noErrors *= my_itoa(atoi(imm.c_str()), shant, 2, 5, counter);
	funct = instructionTable[function][2];
	
	//identify registers
	rt = parameters[2];
	rd = parameters[1];
	
	//get register codes
	rtCode = registerTable[rt];
	rdCode = registerTable[rd];
	
	//check if valid registers
	noErrors *= checkRegister(rtCode, counter);
	noErrors *= checkRegister(rdCode, counter);
	
	//check for register or imediate errors
	if (noErrors == false)
	{
		return "Error";
	}
	
	//concatenate strings
	stringstream ss;
	ss << opCode << NULL_REGISTER << rtCode << rdCode << shant << funct;
	string functionCode = ss.str();
	
	//return machine code
	return functionCode;
}

//jr function -> jr $rs
string assembleTypeR3(string line, int counter)
{
	string opCode, rsCode, funct;
	string function, rs;
	vector<string> parameters;
	bool noErrors = true;
	
	pair<bool, vector<string> > encapsuledParameters = identifyTypeR3(line);
	
	if(encapsuledParameters.first == false)
	{
		LOG(LEVEL_INFO) << "Line = " << line;
		errorSignal(7, counter);
		return "Error";
	}
	
	parameters = encapsuledParameters.second;
	
	//identify function
	function = parameters[0];
	
	//identify opcode, shant and funct
	opCode = instructionTable[function][1];
	funct = instructionTable[function][2];
	
	//identify registers
	rs = parameters[1];
	
	//get register codes
	rsCode = registerTable[rs];
	
	//check if valid register
	noErrors *= checkRegister(rsCode, counter);
	
	//check for register or imediate errors
	if (noErrors == false)
	{
		return "Error";
	}
	
	//concatenate strings
	stringstream ss;
	ss << opCode << rsCode << NULL_REGISTER << NULL_REGISTER << NULL_SHANT << funct;
	string functionCode = ss.str();
	
	//return machine code
	return functionCode;
}

//normal type I functions -> addi $rt, $rs, imm
string assembleTypeI1(string line, int counter)
{
	string opCode, rsCode, rtCode, immCode;
	string function, rs, rt, imm;
	vector<string> parameters;
	bool noErrors = true;
	
	pair<bool, vector<string> > encapsuledParameters = identifyTypeI1(line);
	
	if(encapsuledParameters.first == false)
	{
		errorSignal(7, counter);
		LOG(LEVEL_INFO) << "Line = " << line;
		return "Error";
	}
	
	parameters = encapsuledParameters.second;
	
	//identify function
	function = parameters[0];
	//identify opcode
	opCode = instructionTable[function][1];
	
	//identify registers
	rs = parameters[2];
	rt = parameters[1];
	//get register codes
	rsCode = registerTable[rs];
	rtCode = registerTable[rt];
	//check if valid register
	noErrors *= checkRegister(rsCode, counter);
	noErrors *= checkRegister(rtCode, counter);
	
	//identify number if label check simbols table
	imm = parameters[3];
	//if number, transforms string decimal in string binary
	noErrors *= my_itoa(atoi(imm.c_str()), immCode, 2, 16, counter);
	
	//check for register or imediate errors
	if (noErrors == false)
	{
		return "Error";
	}
	
	//concatenate strings
	stringstream ss;
	ss << opCode << rsCode << rtCode << immCode;
	string functionCode = ss.str();
	
	//return machine code
	return functionCode;
}

//loads and saves -> lw $rt,58($rs)
string assembleTypeI2(string line, int counter)
{
	string opCode, rsCode, rtCode, immCode;
	string function, rs, rt, imm;
	vector<string> parameters;
	bool noErrors = true;
	
	pair<bool, vector<string> > encapsuledParameters = identifyTypeI2(line);
	
	if(encapsuledParameters.first == false)
	{
		errorSignal(7, counter);
		LOG(LEVEL_INFO) << "Line = " << line;
		return "Error";
	}
	
	parameters = encapsuledParameters.second;
	
	//identify function
	function = parameters[0];
	//identify opcode
	opCode = instructionTable[function][1];
	//identify registers
	rs = parameters[3];
	rt = parameters[1];
	//get register codes
	rsCode = registerTable[rs];
	rtCode = registerTable[rt];
	//check if valid register
	noErrors *= checkRegister(rsCode, counter);
	noErrors *= checkRegister(rtCode, counter);
	
	//identify number if label check simbols table
	imm = parameters[2];
	//if number, transforms string decimal in string binary
	noErrors *= my_itoa(atoi(imm.c_str()), immCode, 2, 16, counter);
	
	//check for register or imediate errors
	if (noErrors == false)
	{
		return "Error";
	}
	
	//concatenate strings
	stringstream ss;
	ss << opCode << rsCode << rtCode << immCode;
	
	string functionCode = ss.str();
	
	//return machine code
	return functionCode;
}

//loads and saves -> lui $rt,58 --- lui $rt,Label
string assembleTypeI3(string line, int counter)
{
	string opCode, rtCode, immCode;
	string function, rt, imm;
	vector<string> parameters;
	bool noErrors = true;
	
	pair<bool, vector<string> > encapsuledParameters = identifyTypeI3(line);
	
	if(encapsuledParameters.first == false)
	{
		errorSignal(7, counter);
		LOG(LEVEL_INFO) << "Line = " << line;
		return "Error";
	}
	
	parameters = encapsuledParameters.second;
	
	//identify function
	function = parameters[0];
	//identify opcode
	opCode = instructionTable[function][1];
	
	//identify registers
	rt = parameters[1];
	//get register codes
	rtCode = registerTable[rt];
	//check if valid register
	noErrors *= checkRegister(rtCode, counter);
	
	//identify number if label check simbols table
	imm = parameters[2];
	//if number, transforms string decimal in string binary
	noErrors *= my_itoa(atoi(imm.c_str()), immCode, 2, 16, counter);
	
	//check for register or imediate errors
	if (noErrors == false)
	{
		return "Error";
	}
	
	//concatenate strings
	stringstream ss;
	ss << opCode << NULL_REGISTER << rtCode << immCode;
	string functionCode = ss.str();
	
	//return machine code
	return functionCode;
}

//branch instructions --> beq $rs,$rt,Label
string assembleTypeI4(string line, int counter, int counter2)
{
	string opCode, rsCode, rtCode, immCode;
	string function, rs, rt, label;
	int imm;
	vector<string> parameters;
	bool noErrors = true;
	
	pair<bool, vector<string> > encapsuledParameters = identifyTypePseudo4(line);
	
	if(encapsuledParameters.first == false)
	{
		errorSignal(7, counter);
		LOG(LEVEL_INFO) << "Line = " << line;
		return "Error";
	}
	
	parameters = encapsuledParameters.second;
	
	//identify function
	function = parameters[0];
	//identify opcode
	opCode = instructionTable[function][1];
	
	//identify registers
	rs = parameters[1];
	rt = parameters[2];
	//get register codes
	rsCode = registerTable[rs];
	rtCode = registerTable[rt];
	//check if valid register
	noErrors *= checkRegister(rsCode, counter);
	noErrors *= checkRegister(rtCode, counter);
	
	//identify number if label check simbols table
	label = parameters[3];
	imm = simbolTable[label]-counter2;
	
	if(simbolTable[label] == 0)
	{
		errorSignal(13, counter);
		LOG(LEVEL_INFO) << "Line = " << line;
		return "Error";
	}
	
	//transforms decimal in string binary
	noErrors *= my_itoa(imm, immCode, 2, 16, counter);
	
	//check for register or imediate errors
	if (noErrors == false)
	{
		return "Error";
	}
	
	//concatenate strings
	stringstream ss;
	ss << opCode << rsCode << rtCode << immCode;
	string functionCode = ss.str();
	
	//return machine code
	return functionCode;
}

//jumps -> j Label or jal Label
string assembleTypeJ(string line, int counter)
{
	string opCode, immCode;
	string function, label;
	int imm;
	vector<string> parameters;
	bool noErrors = true;
	
	pair<bool, vector<string> > encapsuledParameters = identifyTypeJ(line);
	
	if(encapsuledParameters.first == false)
	{
		errorSignal(7, counter);
		LOG(LEVEL_INFO) << "Line = " << line;
		return "Error";
	}
	
	parameters = encapsuledParameters.second;
	
	//identify function
	function = parameters[0];
	//identify opcode
	opCode = instructionTable[function][1];
	
	//identify number if label check simbols table
	label = parameters[1];
	imm = simbolTable[label];
	
	if(imm == 0)
	{
		errorSignal(13, counter);
		LOG(LEVEL_INFO) << "Line = " << line;
		return "Error";
	}
	
	//if number, transforms string decimal in string binary
	noErrors *= my_itoa(imm, immCode, 2, 26, counter);
	
	//check for register or imediate errors
	if (noErrors == false)
	{
		return "Error";
	}
		
	//concatenate strings
	stringstream ss;
	ss << opCode << immCode;
	string functionCode = ss.str();
	
	//return machine code
	return functionCode;
}

//float or double operations
//float operations -> add.i $fd,$fs,$ft
//double operations -> add.d $fd,$fs,$ft
//this two have to be with register f
string assembleTypeFloat(string line, int counter)
{
	string opCode, type, fsCode, ftCode, fdCode, funct;
	string function, fs, ft, fd;
	vector<string> parameters;
	bool noErrors = true;
	
	pair<bool, vector<string> > encapsuledParameters = identifyTypeFloat(line);
	
	if(encapsuledParameters.first == false)
	{
		errorSignal(7, counter);
		LOG(LEVEL_INFO) << "Line = " << line;
		return "Error";
	}
	
	parameters = encapsuledParameters.second;
	
	//identify function
	function = parameters[0];
	
	//identify opcode, shant and funct
	opCode = instructionTable[function][1];
	type = instructionTable[function][2];
	funct = instructionTable[function][3];
	
	//identify registers
	fs = parameters[2];
	ft = parameters[3];
	fd = parameters[1];
	
	//get register codes
	fsCode = registerFloatTable[fs];
	ftCode = registerFloatTable[ft];
	fdCode = registerFloatTable[fd];
	
	//check if valid registers
	noErrors *= checkRegister(fsCode, counter);
	noErrors *= checkRegister(ftCode, counter);
	noErrors *= checkRegister(fdCode, counter);
	
	//check for register or imediate errors
	if (noErrors == false)
	{
		return "Error";
	}
	
	//concatenate strings
	stringstream ss;
	ss << opCode << type << ftCode << fsCode << fdCode << funct;
	string functionCode = ss.str();
	
	//return machine code
	return functionCode;
}

//move -> move $rt,$rs = addi $rt,$rs,0
string assembleTypePseudo1(string line, int counter)
{
	string opCode, rsCode, rtCode;
	string function, rs, rt;
	vector<string> parameters;
	bool noErrors = true;
	
	pair<bool, vector<string> > encapsuledParameters = identifyTypePseudo1(line);
	
	if(encapsuledParameters.first == false)
	{
		errorSignal(7, counter);
		LOG(LEVEL_INFO) << "Line = " << line;
		return "Error";
	}
	
	parameters = encapsuledParameters.second;
	
	//identify function
	function = parameters[0];
	//identify opcode
	opCode = instructionTable[function][1];
	
	//identify registers
	rs = parameters[2];
	rt = parameters[1];
	//get register codes
	rsCode = registerTable[rs];
	rtCode = registerTable[rt];
	//check if valid register
	noErrors *= checkRegister(rsCode, counter);
	noErrors *= checkRegister(rtCode, counter);
	
	//check for register or imediate errors
	if (noErrors == false)
	{
		return "Error";
	}
	
	//concatenate strings
	stringstream ss;
	ss << opCode << rsCode << rtCode << ZERO_IMM;
	string functionCode = ss.str();
	
	//return machine code
	return functionCode;
}

//clear -> clear $rt = add $rt,$zero,$zero
string assembleTypePseudo2(string line, int counter)
{
	string opCode, rtCode, funct;
	string function, rt;
	vector<string> parameters;
	bool noErrors = true;
	
	pair<bool, vector<string> > encapsuledParameters = identifyTypePseudo2(line);
	
	if(encapsuledParameters.first == false)
	{
		errorSignal(7, counter);
		LOG(LEVEL_INFO) << "Line = " << line;
		return "Error";
	}
	
	parameters = encapsuledParameters.second;
	
	//identify function
	function = parameters[0];
	
	//identify opcode, shant and funct
	opCode = instructionTable[function][1];
	funct = instructionTable[function][2];
	
	//identify registers
	rt = parameters[1];
	
	//get register codes
	rtCode = registerTable[rt];
	
	//check if valid registers
	noErrors *= checkRegister(rtCode, counter);
	
	//check for register or imediate errors
	if (noErrors == false)
	{
		return "Error";
	}
	
	//concatenate strings
	stringstream ss;
	ss << opCode << ZERO_REGISTER << ZERO_REGISTER << rtCode << NULL_SHANT << funct;
	string functionCode = ss.str();
	
	//return machine code
	return functionCode;
}

//Branch Unconditionally -> b Label = beq $zero,$zero, Label
string assembleTypePseudo3(string line, int counter, int counter2)
{
	string opCode, immCode;
	string function, label;
	int imm;
	vector<string> parameters;
	bool noErrors = true;
	
	pair<bool, vector<string> > encapsuledParameters = identifyTypePseudo3(line);
	
	if(encapsuledParameters.first == false)
	{
		errorSignal(7, counter);
		LOG(LEVEL_INFO) << "Line = " << line;
		return "Error";
	}
	
	parameters = encapsuledParameters.second;
	
	//identify function
	function = parameters[0];
	//identify opcode
	opCode = instructionTable[function][1];
	
	//identify number if label check simbols table
	label = parameters[1];
	imm = simbolTable[label]-counter2;
	
	if(simbolTable[label] == 0)
	{
		errorSignal(13, counter);
		LOG(LEVEL_INFO) << "Line = " << line;
		return "Error";
	}
	
	//if number, transforms string decimal in string binary
	noErrors *= my_itoa(imm, immCode, 2, 16, counter);
	
	//check for register or imediate errors
	if (noErrors == false)
	{
		return "Error";
	}
	
	//concatenate strings
	stringstream ss;
	ss << opCode << ZERO_REGISTER << ZERO_REGISTER << immCode;
	string functionCode = ss.str();
	
	//return machine code
	return functionCode;
}

//Branch if greater than -> bgt $rs,$rt,Label = slt $at,$rt,$rs + bne $at,$zero,Label
string assembleTypePseudo4(string line, int counter, int counter2)
{
	string opCode1, rsCode, rtCode, funct, opCode2, immCode;
	string function, rs, rt, label;
	int imm;
	vector<string> parameters;
	bool noErrors = true;
	
	pair<bool, vector<string> > encapsuledParameters = identifyTypePseudo4(line);
	
	if(encapsuledParameters.first == false)
	{
		errorSignal(7, counter);
		LOG(LEVEL_INFO) << "Line = " << line;
		return "Error";
	}
	
	parameters = encapsuledParameters.second;
	
	//identify function
	function = parameters[0];
	//identify opcode
	opCode1 = instructionTable[function][1];
	funct = instructionTable[function][2];
	opCode2 = instructionTable[function][3];
	
	//identify registers
	rs = parameters[1];
	rt = parameters[2];
	//get register codes
	rsCode = registerTable[rs];
	rtCode = registerTable[rt];
	//check if valid register
	noErrors *= checkRegister(rsCode, counter);
	noErrors *= checkRegister(rtCode, counter);
	
	//identify number if label check simbols table
	label = parameters[3];
	imm = simbolTable[label]-counter2;
	
	if(simbolTable[label] == 0)
	{
		errorSignal(13, counter);
		LOG(LEVEL_INFO) << "Line = " << line;
		return "Error";
	}
	
	//if number, transforms string decimal in string binary
	noErrors *= my_itoa(imm, immCode, 2, 16, counter);
	
	//check for register or imediate errors
	if (noErrors == false)
	{
		return "Error";
	}
	
	//concatenate strings
	stringstream ss;
	ss << opCode1 << rtCode << rsCode << AT_REGISTER << NULL_SHANT << funct << "\n" << opCode2 << AT_REGISTER << ZERO_REGISTER << immCode;
	string functionCode = ss.str();
	
	//return machine code
	return functionCode;
}

//Branch if less than -> blt $rs,$rt,Label = slt $at,$rs,$rt + bne $at,$zero,Label
//Branch if greater than or equal -> bge $rs,$rt,Label = slt $at,$rs,$rt + beq $at,$zero,Label
string assembleTypePseudo5(string line, int counter, int counter2)
{
	string opCode1, rsCode, rtCode, funct, opCode2, immCode;
	string function, rs, rt, label;
	int imm;
	vector<string> parameters;
	bool noErrors = true;
	
	pair<bool, vector<string> > encapsuledParameters = identifyTypePseudo5(line);
	
	if(encapsuledParameters.first == false)
	{
		errorSignal(7, counter);
		LOG(LEVEL_INFO) << "Line = " << line;
		return "Error";
	}
	
	parameters = encapsuledParameters.second;
	
	//identify function
	function = parameters[0];
	//identify opcode
	opCode1 = instructionTable[function][1];
	funct = instructionTable[function][2];
	opCode2 = instructionTable[function][3];
	
	//identify registers
	rs = parameters[1];
	rt = parameters[2];
	//get register codes
	rsCode = registerTable[rs];
	rtCode = registerTable[rt];
	//check if valid register
	noErrors *= checkRegister(rsCode, counter);
	noErrors *= checkRegister(rtCode, counter);
	
	//identify number if label check simbols table
	label = parameters[3];
	imm = simbolTable[label]-counter2;
	
	if(simbolTable[label] == 0)
	{
		errorSignal(13, counter);
		LOG(LEVEL_INFO) << "Line = " << line;
		return "Error";
	}
	
	//if number, transforms string decimal in string binary
	noErrors *= my_itoa(imm, immCode, 2, 16, counter);
	
	//check for register or imediate errors
	if (noErrors == false)
	{
		return "Error";
	}
	
	//concatenate strings
	stringstream ss;
	ss << opCode1 << rsCode << rtCode << AT_REGISTER << NULL_SHANT << funct << "\n" << opCode2 << AT_REGISTER << ZERO_REGISTER << immCode;
	string functionCode = ss.str();
	
	//return machine code
	return functionCode;
}

bool checkRegister(string registerCode, int counter)
{
	if(registerCode.size() == 0)
	{
		errorSignal(15, counter);
		return false;
	}
	
	//return code found in string format
	return true;
}

bool my_itoa(int value, std::string& buf, int base, unsigned int size, int counter)
{
	if(value >= 0 && value < pow(2, size))
	{
		int i = 30;
		buf = "";
		
		for(; value && i ; --i, value /= base) buf = "0123456789abcdef"[value % base] + buf;
		
		while(buf.size() < size)
		{
			buf = "0" + buf;
		}
		
		return true;
	}
	else
	{
		if (value<0 && value > -pow(2, size))
		{
			value = -value - 1;
			int i = 30;
			buf = "";
			
			for(; value && i ; --i, value /= base) buf = "1023456789abcdef"[value % base] + buf;
			
			while(buf.size() < size)
			{
				buf = "1" + buf;
			}
			
			return true;
		}
		else
		{
			errorSignal(9, counter);
			return false;
		}
	}
}

void errorSignal(int errorCode, int line)
{
	if (errorCode > 100)
		LOG(LEVEL_FATAL) << "Error found - Error number " << errorCode;
	else
		LOG(LEVEL_ERROR) << "Error found - Error number " << errorCode;
	
	if (line > 0)
	{
		LOG(LEVEL_INFO) << "Error in line " << line;
	}
	
	switch(errorCode)
	{
		case 1:
			LOG(LEVEL_INFO) << "Lexic Error\nError due to already existent token used as label";
			break;
		case 2:
			LOG(LEVEL_INFO) << "Error due to non existent function used";
		break;
		case 3:
			LOG(LEVEL_INFO) << "Error due to register with no function";
		break;
		case 4:
			LOG(LEVEL_INFO) << "Error due to missing parameter in function";
		break;
		case 5:
			LOG(LEVEL_INFO) << "Error due to non existent register used";
		break;
		case 6:
			LOG(LEVEL_INFO) << "Error due to no $ before register";
		break;
		case 7:
			LOG(LEVEL_INFO) << "Error due to wrong format function";
		break;
		case 8:
			LOG(LEVEL_INFO) << "Error due to existence of extra parameters";
		break;
		case 9:
			LOG(LEVEL_INFO) << "Trying to use to large number in immediate";
		break;
		case 10:
			LOG(LEVEL_INFO) << "Can't convert negative number to binary";
		break;
		case 13:
			LOG(LEVEL_INFO) << "Trying to access non declared label";
		break;
		case 14:
			LOG(LEVEL_INFO) << "Trying to access non existent function";
		break;
		case 15:
			LOG(LEVEL_INFO) << "Trying to access non existent register";
		break;
		case 17:
			LOG(LEVEL_INFO) << "Label registered twice";
		break;
		case 18:
			LOG(LEVEL_INFO) << "Didn't detect line";
		break;
		case 25:
			LOG(LEVEL_INFO) << "First token of line isn't label or instruction";
		break;
		case 101:
			LOG(LEVEL_INFO) << "Unable to open file " << INPUT_FILE;
		break;
	}
	
	
}
