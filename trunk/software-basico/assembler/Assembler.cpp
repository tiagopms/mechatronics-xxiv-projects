#include <Assembler.hpp>

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
	//open instructions file
	
	//loop to go in each line
	
		//create instructions table
	
	//close file
	
	//registers
	//open registers file
	
	//loop to go in each line
	
		//create registers table
	
	//close file
}

void createSimbolsTable()
{
	//open file
	
	
	//loop to go in each line
	
		//create simbols table
	
	//close file
	
}

bool assemble()
{
	int type, counter = 0;
	string line;
	string filename = INPUT;
	//open input and output file
	ifstream cfg(filename.c_str(), ifstream::in);
	if(cfg.fail())
		errorSignal(100000);//change this ------------------------------------------



	//loop to go in each line
	while(cfg.good())
	{
		counter++;
		//get single line
		getline(cfg, line);
		
		//reconize type of intruction
		type = reconizeType(line);
		
		//if not found send error signal
		errorSignal(2, counter);
		
		switch(type)
		{
			case 1:
				assembleTypeR(line);
				break;
			case 2:
				assembleTypeI(line);
				break;
			case 3:
				assembleTypeJ(line);
				break;
		}
	}
		
	
	//close input and output file
	cfg.close();
	
	return true;
}

int reconizeType(string line)
{
	
	return 0;
}

void assembleTypeR(string line)
{
	//identify opcode, shant and funct
	
	//identify registers
	registerCode(line);
	
	//make translation to machine code
	
}

void assembleTypeI(string line)
{
	//identify opcode
	
	//identify registers
	
	//identify number if label check simbols table
	
	//make translation to machine code
	
}

void assembleTypeJ(string line)
{
	//identify opcode
	
	//identify number if label check simbols table
	
	//make translation to machine code
	
}

string registerCode(string registerUsed)
{
	//compare resgisterUsed to registers table
	
	//get register's code
	
	//return code found in string format
	return "01010"; //change this -----------------------------------------
}

void errorSignal(int errorCode, int counter)
{
	LOG(LEVEL_FATAL) << "Error found - Error number " << errorCode;
	
	switch(errorCode)
	{
		case 1:
			LOG(LEVEL_ERROR) << "Lexic Error\nError due to already existent token used as label";
			break;
		case 2:
			LOG(LEVEL_ERROR) << "Error due to non existent function used";
		break;
		case 3:
			LOG(LEVEL_ERROR) << "Error due to register with no function";
		break;
		case 4:
			LOG(LEVEL_ERROR) << "Error due to missing parameter in function";
		break;
		case 5:
			LOG(LEVEL_ERROR) << "Error due to non existent register used";
		break;
		case 6:
			LOG(LEVEL_ERROR) << "Error due to no $ before register";
		break;
		case 7:
			LOG(LEVEL_ERROR) << "Error due to wrong format function";
		break;
		case 8:
			LOG(LEVEL_ERROR) << "Error due to existence of extra parameters";
		break;
	}
	
	
}

/*
bool Config::read(string filename)
{
	ifstream cfg(filename.c_str(), ifstream::in);

	if(cfg.fail())
		return false;

	string line;

	while(cfg.good())
	{
		getline(cfg, line);
		useLine(line);
	}

	cfg.close();

	return true;
}

bool Config::parseLine(string line)
{
	if((line[0] == '#') || (line.size() == 0))
		return false;

	// construct a stream from the string
	stringstream ss(line);

	// use stream iterators to copy the stream to the vector as whitespace separated strings
	istream_iterator<string> it(ss);
	istream_iterator<string> end;
	vector<string> tokens(it, end);

	LOG(LEVEL_VERBOSE) << "tokens size: " << tokens.size();

	string key(tokens[0]);
	tokens.erase(tokens.begin());
	config[key] = tokens;

	LOG(LEVEL_VERBOSE) << "config[ " << key
					   << "] size: " << config[key].size() << endl;

	return true;
}
*/
