#include <Assembler.hpp>

map<string, vector<string> > instructionTable;
map<string, string> registerTable;
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

void findLabel(string line, int counter)
{
	if(identifyToken(line))
	{
		LOG(LEVEL_WARN) << "Token Found";
		LOG(LEVEL_INFO) << line;
		LOG(LEVEL_INFO) << "Line number = " << counter;
	}
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
		type = reconizeType(line);
		
		//if not found send error signal
		if(type == 0)
			errorSignal(2, counter);
		
		LOG(LEVEL_INFO) << "Line number = " << counter;
		LOG(LEVEL_INFO) << line;
		
		switch(type)
		{
			case 1:
				functionCode = assembleTypeR1(line, counter);
				break;
			case 2:
				functionCode = assembleTypeR2(line, counter);
				break;
			case 3:
				functionCode = assembleTypeI1(line, counter);
				break;
			case 4:
				functionCode = assembleTypeI2(line, counter);
				break;
			case 5:
				functionCode = assembleTypeJ(line, counter);
				break;
			case 6:
				functionCode = assembleTypeFloat(line, counter);
				break;
			case 7:
				functionCode = assembleTypePseudo(line, counter);
				break;
		}
		
		LOG(LEVEL_INFO) << "Line code = " << functionCode;
		
		output << functionCode << "\n";
	}
		
	
	//close input and output file
	cfg.close();
	
	return true;
}

int reconizeType(string line)
{
	
	return 2;
}

//normal type r functions -> add $rd,$rs,$rt
string assembleTypeR1(string line, int counter)
{
	string opCode, rsCode, rtCode, rdCode, shant, funct;
	string function, rd, rs, rt;
	
	//identify function
	function = "add";
	
	//identify opcode, shant and funct
	opCode = instructionTable[function][1];
	shant = instructionTable[function][1];
	funct = instructionTable[function][2];
	
	//identify registers
	rs = "$t2";
	rt = "$t1";
	rd = "$t0";
	
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
	ss << opCode << rdCode << rsCode << rtCode << shant << funct;
	string functionCode = ss.str();
	
	//make translation to machine code
	return functionCode;
}

//shift type r functions -> sll $rd,$rs,imm
string assembleTypeR2(string line, int counter)
{
	string opCode, rsCode, rtCode, rdCode, shant, funct;
	string function, rd, rs, rt;
	
	//identify function
	function = "add";
	
	//identify opcode, shant and funct
	opCode = instructionTable[function][1];
	shant = instructionTable[function][1];
	funct = instructionTable[function][2];
	
	//identify registers
	rs = "$t2";
	rt = "$t1";
	rd = "$t0";
	
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
	ss << opCode << rdCode << rsCode << rtCode << shant << funct;
	string functionCode = ss.str();
	
	//make translation to machine code
	return functionCode;
}

//jr function -> jr $rs
string assembleTypeR3(string line, int counter)
{
	string opCode, rd, rs, rt, shant, funct;
	//identify opcode, shant and funct
	
	//identify registers
	//registerCode(line);
	
	//concatenate strings
	stringstream ss;
	ss << opCode << rd << rs << rt << shant << funct;
	string functionCode = ss.str();
	
	//make translation to machine code
	return functionCode;
}

//normal type I functions -> addi $rt, $rs, imm
string assembleTypeI1(string line, int counter)
{
	string opCode, rs, rt, imm;
	//identify opcode
	
	//identify registers
	
	//identify number if label check simbols table
	
	//concatenate strings
	stringstream ss;
	ss << opCode << rs << rt << imm;
	string functionCode = ss.str();
	
	//make translation to machine code
	return functionCode;
}

//loads and saves -> lw $rt,58($rs)
string assembleTypeI2(string line, int counter)
{
	string opCode, rs, rt, imm;
	//identify opcode
	
	//identify registers
	
	//identify number if label check simbols table
	
	//concatenate strings
	stringstream ss;
	ss << opCode << rs << rt << imm;
	string functionCode = ss.str();
	
	//make translation to machine code
	return functionCode;
}

//jumps -> j Label or jal Label
string assembleTypeJ(string line, int counter)
{
	string opCode, imm;
	//identify opcode
	
	//identify number if label check simbols table
	
	//concatenate strings
	stringstream ss;
	ss << opCode << imm;
	string functionCode = ss.str();
	
	//make translation to machine code
	return functionCode;
}

//float or double operations
//float operations -> add.i $fd,$fs,$ft
//double operations -> add.d $fd,$fs,$ft
//this two have to be with register f
string assembleTypeFloat(string line, int counter)
{
	string opCode, type, fs, ft, fd, funct;
	//identify opcode
	
	//identify registers
	
	//identify number if label check simbols table
	
	//concatenate strings
	stringstream ss;
	ss << opCode << type << fs << ft << fd << funct;
	string functionCode = ss.str();
	
	//make translation to machine code
	return functionCode;
}

//move -> move $rt,$rs = addi $rt,$rs,0
string assembleTypePseudo1(string line, int counter)
{
	string opCode, rd, rs, rt, shant, funct;
	//identify both opcodes
	
	//identify registers
	
	//identify numbers if label check simbols table
	
	//concatenate strings
	stringstream ss;
	ss << opCode << rd << rs << rt << shant << funct;
	string functionCode = ss.str();
	
	//make translation to machine code
	return functionCode;
}

//clear -> clear $rt = add $rt,$zero,$zero
string assembleTypePseudo2(string line, int counter)
{
	string opCode, rd, rs, rt, shant, funct;
	//identify both opcodes
	
	//identify registers
	
	//identify numbers if label check simbols table
	
	//concatenate strings
	stringstream ss;
	ss << opCode << rd << rs << rt << shant << funct;
	string functionCode = ss.str();
	
	//make translation to machine code
	return functionCode;
}

//Branch Unconditionally -> b Label = beq $zero,$zero, Label
string assembleTypePseudo3(string line, int counter)
{
	string opCode, rd, rs, rt, shant, funct;
	//identify both opcodes
	
	//identify registers
	
	//identify numbers if label check simbols table
	
	//concatenate strings
	stringstream ss;
	ss << opCode << rd << rs << rt << shant << funct;
	string functionCode = ss.str();
	
	//make translation to machine code
	return functionCode;
}

//Branch if greater than -> bgt $rs,$rt,Label = slt $at,$rt,$rs + bne $at,$zero,Label
string assembleTypePseudo4(string line, int counter)
{
	string opCode, rd, rs, rt, shant, funct;
	//identify both opcodes
	
	//identify registers
	
	//identify numbers if label check simbols table
	
	//concatenate strings
	stringstream ss;
	ss << opCode << rd << rs << rt << shant << funct;
	string functionCode = ss.str();
	
	//make translation to machine code
	return functionCode;
}

//Branch if less than -> blt $rs,$rt,Label = slt $at,$rs,$rt + bne $at,$zero,Label
//Branch if greater than or equal -> bge $rs,$rt,Label = slt $at,$rs,$rt + beq $at,$zero,Label
string assembleTypePseudo5(string line, int counter)
{
	string opCode, rd, rs, rt, shant, funct;
	//identify both opcodes
	
	//identify registers
	
	//identify numbers if label check simbols table
	
	//concatenate strings
	stringstream ss;
	ss << opCode << rd << rs << rt << shant << funct;
	string functionCode = ss.str();
	
	//make translation to machine code
	return functionCode;
}

bool checkRegister(string registerCode, int counter)
{
	if(registerCode.size() == 0)
		errorSignal(15, counter);
	
	//return code found in string format
	return true;
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
		case 101:
			LOG(LEVEL_INFO) << "Unable to open file " << INPUT_FILE;
		break;
	}
	
	
}
