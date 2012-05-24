//label
//$zero

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
/*	string token = "add ";
	identifyToken(token);
	token = "label: askljdajks";
	identifyToken(token);
	token = "add $t0,$t0,$t0";
	identifyTypeR1(token);
	token = "sll $rd,$rs,12";
	identifyTypeR2(token);
	token = "jr $rs";
	identifyTypeR3(token);
*/
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
	int counter = 0;
	string line;
	string filename = INPUT_FILE;
	
	//open input file
	ifstream cfg(filename.c_str(), ifstream::in);
	if(cfg.fail())
		errorSignal(101, 0);
	
	//loop to go in each line
	while(cfg.good())
	{
		counter++;
		//get single line
		getline(cfg, line);
		
		//updates simbols table
		findLabel(line, counter);
	}
	
	//close file
	cfg.close();
	
	return true;
}

bool findLabel(string line, int counter)
{
	if(line.size() == 0)
		return false;
		
	pair <int, string> label = identifyToken(line);
	
	if(label.first == 2)
	{
		LOG(LEVEL_WARN) << "Label Found";
		LOG(LEVEL_INFO) << label.second << " Found in line number = " << counter;
		simbolTable[label.second] = counter;
	}
	
	return true;
}

bool assemble()
{
	int type, counter = 0;
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

	//loop to go in each line
	while(cfg.good())
	{
		counter++;
		//get single line
		getline(cfg, line);
		
		//reconize type of intruction
		type = reconizeType(line, counter);
		
//		LOG(LEVEL_INFO) << "Line number = " << counter;
//		LOG(LEVEL_INFO) << line;
		
		switch(type)
		{
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
				functionCode = assembleTypeJ(line, counter);
				break;
			case 8:
				functionCode = assembleTypeFloat(line, counter);
				break;
			case 9:
				functionCode = assembleTypePseudo1(line, counter);
				break;
			case 10:
				functionCode = assembleTypePseudo2(line, counter);
				break;
			case 11:
				functionCode = assembleTypePseudo3(line, counter);
				break;
			case 12:
				functionCode = assembleTypePseudo4(line, counter);
				break;
			case 13:
				functionCode = assembleTypePseudo5(line, counter);
				break;
			case 14:
				functionCode = SYSCALL_CODE;
				break;
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
	string instruction;
	string typeString;
	int type = 0;
	
	if(line.size() == 0)
		return 0;
	
	pair<int, string> token = identifyToken(line);
	if (token.first == 1)
	{
		instruction = token.second;
		if(instruction != "syscall")
			instruction = instruction.substr(0, instruction.length() - 1);
	}
	else
		instruction = "add";//instruction = getInstructionToken();

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
				type = 6+typeString[1]-48;
			}
			else
			{
				if (typeString[0] == 'F')
				{
					type = 7+typeString[1]-48;
				}
				else
				{
					if (typeString[0] == 'P')
					{
						type = 8+typeString[1]-48;
					}
					else
					{
						if (typeString[0] == 'S')
						{
							type = 14;
						}
					}
				}
			}
		}
	}
	
		
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
	checkRegister(rsCode, counter);
	checkRegister(rtCode, counter);
	checkRegister(rdCode, counter);
	
	//concatenate strings
	stringstream ss;
	ss << opCode << rsCode << rtCode << rdCode << NULL_SHANT << funct;
	string functionCode = ss.str();
	
	//return machine code
	return functionCode;
}

//shift type r functions -> sll $rd,$rs,imm
string assembleTypeR2(string line, int counter)
{
	string opCode, rsCode, rdCode, shant, funct;
	string function, rd, rs, imm;
	vector<string> parameters;
	
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
	my_itoa(atoi(imm.c_str()), shant, 2, 5, counter);
	funct = instructionTable[function][2];
	
	//identify registers
	rs = parameters[2];
	rd = parameters[1];
	
	//get register codes
	rsCode = registerTable[rs];
	rdCode = registerTable[rd];
	
	//check if valid registers
	checkRegister(rsCode, counter);
	checkRegister(rdCode, counter);
	
	//concatenate strings
	stringstream ss;
	ss << opCode << rdCode << rsCode << NULL_REGISTER << shant << funct;
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
	checkRegister(rsCode, counter);
	
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
	
	//identify function
	function = "addi";
	//identify opcode
	opCode = instructionTable[function][1];
	
	//identify registers
	rs = "$t2";
	rt = "$t5";
	//get register codes
	rsCode = registerTable[rs];
	rtCode = registerTable[rt];
	//check if valid register
	checkRegister(rsCode, counter);
	checkRegister(rtCode, counter);
	
	//identify number if label check simbols table
	imm = "3";
	//if number, transforms string decimal in string binary
	my_itoa(atoi(imm.c_str()), immCode, 2, 16, counter);
	
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
	
	//identify function
	function = "lw";
	//identify opcode
	opCode = instructionTable[function][1];
	//identify registers
	rs = "$t2";
	rt = "$t5";
	//get register codes
	rsCode = registerTable[rs];
	rtCode = registerTable[rt];
	//check if valid register
	checkRegister(rsCode, counter);
	checkRegister(rtCode, counter);
	
	//identify number if label check simbols table
	imm = "55";
	//if number, transforms string decimal in string binary
	my_itoa(atoi(imm.c_str()), immCode, 2, 16, counter);
	
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
	
	//identify function
	function = "lui";
	//identify opcode
	opCode = instructionTable[function][1];
	
	//identify registers
	rt = "$t5";
	//get register codes
	rtCode = registerTable[rt];
	//check if valid register
	checkRegister(rtCode, counter);
	
	//identify number if label check simbols table
	imm = "55";
	//if number, transforms string decimal in string binary
	my_itoa(atoi(imm.c_str()), immCode, 2, 16, counter);
	
	//concatenate strings
	stringstream ss;
	ss << opCode << NULL_REGISTER << rtCode << immCode;
	string functionCode = ss.str();
	
	//return machine code
	return functionCode;
}

//jumps -> j Label or jal Label
string assembleTypeJ(string line, int counter)
{
	string opCode, immCode;
	string function, imm;
	
	//identify function
	function = "j";
	//identify opcode
	opCode = instructionTable[function][1];
		
	//identify number if label check simbols table
	imm = "1";
	//if number, transforms string decimal in string binary
	my_itoa(atoi(imm.c_str()), immCode, 2, 26, counter);
	
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
	
	//identify function
	function = "add.d";
	
	//identify opcode, shant and funct
	opCode = instructionTable[function][1];
	type = instructionTable[function][2];
	funct = instructionTable[function][3];
	
	//identify registers
	fs = "$f2";
	ft = "$f1";
	fd = "$f0";
	
	//get register codes
	fsCode = registerFloatTable[fs];
	ftCode = registerFloatTable[ft];
	fdCode = registerFloatTable[fd];
	
	//check if valid registers
	checkRegister(fsCode, counter);
	checkRegister(ftCode, counter);
	checkRegister(fdCode, counter);
	
	//concatenate strings
	stringstream ss;
	ss << opCode << type << fsCode << ftCode << fdCode << funct;
	string functionCode = ss.str();
	
	//return machine code
	return functionCode;
}

//move -> move $rt,$rs = addi $rt,$rs,0
string assembleTypePseudo1(string line, int counter)
{
	string opCode, rsCode, rtCode;
	string function, rs, rt;
	
	//identify function
	function = "move";
	//identify opcode
	opCode = instructionTable[function][1];
	
	//identify registers
	rs = "$t2";
	rt = "$t5";
	//get register codes
	rsCode = registerTable[rs];
	rtCode = registerTable[rt];
	//check if valid register
	checkRegister(rsCode, counter);
	checkRegister(rtCode, counter);
	
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
	
	//identify function
	function = "clear";
	
	//identify opcode, shant and funct
	opCode = instructionTable[function][1];
	funct = instructionTable[function][2];
	
	//identify registers
	rt = "$t1";
	
	//get register codes
	rtCode = registerTable[rt];
	
	//check if valid registers
	checkRegister(rtCode, counter);
	
	//concatenate strings
	stringstream ss;
	ss << opCode << ZERO_REGISTER << ZERO_REGISTER << rtCode << NULL_SHANT << funct;
	string functionCode = ss.str();
	
	//return machine code
	return functionCode;
}

//Branch Unconditionally -> b Label = beq $zero,$zero, Label
string assembleTypePseudo3(string line, int counter)
{
	string opCode, immCode;
	string function, imm;
	
	//identify function
	function = "b";
	//identify opcode
	opCode = instructionTable[function][1];
	
	//identify number if label check simbols table
	imm = "3";
	//if number, transforms string decimal in string binary
	my_itoa(atoi(imm.c_str()), immCode, 2, 16, counter);
	
	//concatenate strings
	stringstream ss;
	ss << opCode << ZERO_REGISTER << ZERO_REGISTER << immCode;
	string functionCode = ss.str();
	
	//return machine code
	return functionCode;
}

//Branch if greater than -> bgt $rs,$rt,Label = slt $at,$rt,$rs + bne $at,$zero,Label
string assembleTypePseudo4(string line, int counter)
{
	string opCode1, rsCode, rtCode, funct, opCode2, immCode;
	string function, rs, rt, imm;
	
	//identify function
	function = "bgt";
	//identify opcode
	opCode1 = instructionTable[function][1];
	funct = instructionTable[function][2];
	opCode2 = instructionTable[function][3];
	
	//identify registers
	rs = "$t2";
	rt = "$t5";
	//get register codes
	rsCode = registerTable[rs];
	rtCode = registerTable[rt];
	//check if valid register
	checkRegister(rsCode, counter);
	checkRegister(rtCode, counter);
	
	//identify number if label check simbols table
	imm = "3";
	//if number, transforms string decimal in string binary
	my_itoa(atoi(imm.c_str()), immCode, 2, 16, counter);
	
	//concatenate strings
	stringstream ss;
	ss << opCode1 << rtCode << rsCode << AT_REGISTER << NULL_SHANT << funct << "\n" << opCode2 << AT_REGISTER << ZERO_REGISTER << immCode;
	string functionCode = ss.str();
	
	//return machine code
	return functionCode;
}

//Branch if less than -> blt $rs,$rt,Label = slt $at,$rs,$rt + bne $at,$zero,Label
//Branch if greater than or equal -> bge $rs,$rt,Label = slt $at,$rs,$rt + beq $at,$zero,Label
string assembleTypePseudo5(string line, int counter)
{
	string opCode1, rsCode, rtCode, funct, opCode2, immCode;
	string function, rs, rt, imm;
	
	//identify function
	function = "blt";
	//identify opcode
	opCode1 = instructionTable[function][1];
	funct = instructionTable[function][2];
	opCode2 = instructionTable[function][3];
	
	//identify registers
	rs = "$t2";
	rt = "$t5";
	//get register codes
	rsCode = registerTable[rs];
	rtCode = registerTable[rt];
	//check if valid register
	checkRegister(rsCode, counter);
	checkRegister(rtCode, counter);
	
	//identify number if label check simbols table
	imm = "3";
	//if number, transforms string decimal in string binary
	my_itoa(atoi(imm.c_str()), immCode, 2, 16, counter);
	
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
		errorSignal(15, counter);
	
	//return code found in string format
	return true;
}

void my_itoa(int value, std::string& buf, int base, unsigned int size, int counter)
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
	}
	else
	{
		if (value<0 && value > -pow(2, size))
		{
			errorSignal(10, counter);
		}
		else
		{
			errorSignal(9, counter);
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
		case 101:
			LOG(LEVEL_INFO) << "Unable to open file " << INPUT_FILE;
		break;
	}
	
	
}
