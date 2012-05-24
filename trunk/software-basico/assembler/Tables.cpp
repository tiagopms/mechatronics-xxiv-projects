#include <Assembler.hpp>

map<string, vector<string> > createInstructionTable()
{
	//create instruction table
	map<string, vector<string> > instructionTable;
	
	//open instructions file
	string filename = INSTRUCTION_FILE;
	ifstream cfg(filename.c_str(), ifstream::in);
	if(cfg.fail())
	{
		LOG(LEVEL_FATAL) << "Error found - Error number " << 104;
		LOG(LEVEL_INFO) << "Unable to open instructions file " << INSTRUCTION_FILE;
	}
	
	//loop to go in each line
	string line;
	while(cfg.good())
	{
		getline(cfg, line);
		//add line to instructions table
		useLineInstruction(line, &instructionTable);
	}
	
	//close file
	cfg.close();

	return instructionTable;
}

bool useLineInstruction(string line, map <string, vector<string> > *instructionTable)
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
	
	//uses first token as key and adds others to map
	string key(tokens[0]);
	tokens.erase(tokens.begin());
	(*instructionTable)[key] = tokens;
	
	LOG(LEVEL_VERBOSE) << "instructionTable[ " << key
					   << "] size: " << (*instructionTable)[key].size() << endl;
	
	return true;
}

map<string, string> createRegisterTable()
{
	//create register table
	map<string, string> registerTable;
	
	//open registers file
	string filename = REGISTER_FILE;
	ifstream cfg(filename.c_str(), ifstream::in);
	if(cfg.fail())
	{
		LOG(LEVEL_FATAL) << "Error found - Error number " << 105;
		LOG(LEVEL_INFO) << "Unable to open register file " << REGISTER_FILE;
	}
	
	//loop to go in each line
	string line;
	while(cfg.good())
	{
		getline(cfg, line);
		//add line to registers table
		useLineRegister(line, &registerTable);
	}

	//close file
	cfg.close();
	
	return registerTable;
}

bool useLineRegister(string line, map <string, string > *registerTable)
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
	
	//uses first token as key and adds the other to the map
	string key(tokens[0]);
	(*registerTable)[key] = tokens[1];
	
	LOG(LEVEL_VERBOSE) << "registerTable[ " << key
					   << "] size: " << (*registerTable)[key].size() << endl;
	
	return true;
}

map<string, string> createRegisterFloatTable()
{
	//create register table
	map<string, string> registerFloatTable;
	
	//open registers file
	string filename = REGISTER_FLOAT_FILE;
	ifstream cfg(filename.c_str(), ifstream::in);
	if(cfg.fail())
	{
		LOG(LEVEL_FATAL) << "Error found - Error number " << 106;
		LOG(LEVEL_INFO) << "Unable to open register file " << REGISTER_FLOAT_FILE;
	}
	
	//loop to go in each line
	string line;
	while(cfg.good())
	{
		getline(cfg, line);
		//add line to registers table
		useLineRegisterFloat(line, &registerFloatTable);
	}

	//close file
	cfg.close();
	
	return registerFloatTable;
}

bool useLineRegisterFloat(string line, map <string, string > *registerFloatTable)
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
	
	//uses first token as key and adds the other to the map
	string key(tokens[0]);
	(*registerFloatTable)[key] = tokens[1];
	
	LOG(LEVEL_VERBOSE) << "registerTable[ " << key
					   << "] size: " << (*registerFloatTable)[key].size() << endl;
	
	return true;
}



