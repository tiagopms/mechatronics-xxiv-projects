#include <Identifier.hpp>

pair<int, string>  identifyToken(string token)
{
//	regex instruction("([a-z]+ .*)|(syscall)");
//	regex label("[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: [a-z]+.*");
	pair<int, string> results;
	regex instruction("(:?[a-z]+(:?\\.[a-z])? .*)|(:?syscall)");
	regex instructionHeader("[a-z]+(:?\\.[a-z])? |(:?syscall)");
	regex label("[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: .+");
	regex labelHeader("[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*:");
	cmatch result;
	
	if(regex_match(token.begin(), token.end(), instruction))
	{
		results.first = 1;
		regex_search(token.c_str(), result, instructionHeader);
		
		token = token.substr (result.position(),result.length()-1);
		results.second = token;
		LOG(LEVEL_DEBUG) << "It is instruction " << token;
		return results;
	}
	if(regex_match(token.begin(), token.end(), label))
	{
		results.first = 2;
		regex_search(token.c_str(), result, labelHeader);
		token = token.substr (result.position(),result.length()-1);
		LOG(LEVEL_DEBUG) << "It is label " << token;
		results.second = token;
		return results;
	}
//	LOG(LEVEL_ERROR) << "Error token doesn't exist";
	results.first = 0;
	results.second = token;
	LOG(LEVEL_ERROR) << token;
	return results;
}

pair<bool, vector<string> > identifyFunction(string instruction)
{
	regex correct("(:?[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: )?(:?((:?[a-z]+(:?\\.[a-z])? .*)|(:?syscall)))");
	regex correctInstruction("(:?(:?[a-z]+(:?\\.[a-z])?)|(:?syscall))");
	regex label("[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: ");
	vector<regex> rules (1);
	vector<string> tokens (1);
	pair<bool, vector<string> > results;
	results.second = tokens;
	cmatch result;
	int i = 0;
	
	rules[0] = correctInstruction;
	if(regex_match(instruction.begin(), instruction.end(), correct))
	{
		if (regex_search(instruction.c_str(), result, label))
		{
			instruction = instruction.substr (result.position() + result.length(), instruction.length() - (result.position() + result.length()));
		}
		results.first = 1;
		while(i < 1)
		{
			regex_search(instruction.c_str(), result, rules[i]);
			tokens[i] = instruction.substr (result.position(),result.length());
			//LOG(LEVEL_INFO) << "token = " << tokens[i];
			instruction = instruction.substr (result.position() + result.length(), instruction.length() - (result.position() + result.length()));
			//LOG(LEVEL_INFO) << "instruction" << instruction;
			i++;
		}
		//LOG(LEVEL_INFO) << "It is correct, R2";
		LOG(LEVEL_DEBUG) << "Function token " << tokens[0];
		results.first = true;
		results.second = tokens;
		return results;
	}
//	LOG(LEVEL_ERROR) << "Error sintatic error function";
	results.first = false;
	results.second = tokens;
	return results;
}

pair<bool, vector<string> > identifyTypeR1(string instruction)
{
	regex correct("(:?[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: )?[a-z]+ \\$(:?(:?.{2})|(:?zero)),\\$(:?(:?.{2})|(:?zero)),\\$(:?(:?.{2})|(:?zero))");
	regex correctInstruction("[a-z]+");
	regex correctParameter1("\\$(:?(:?zero)|(:?.{2}))");
	regex correctParameter2("\\$(:?(:?zero)|(:?.{2}))");
	regex correctParameter3("\\$(:?(:?zero)|(:?.{2}))");
	regex label("[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: ");
	vector<regex> rules (4);
	vector<string> tokens (4);
	pair<bool, vector<string> > results;
	results.second = tokens;
	cmatch result;
	int i = 0;
	
	rules[0] = correctInstruction;
	rules[1] = correctParameter1;
	rules[2] = correctParameter2;
	rules[3] = correctParameter3;
	
	if(regex_match(instruction.begin(), instruction.end(), correct))
	{
		if (regex_search(instruction.c_str(), result, label))
		{
			instruction = instruction.substr (result.position() + result.length(), instruction.length() - (result.position() + result.length()));
		}
		results.first = 1;
		while(i < 4)
		{
			regex_search(instruction.c_str(), result, rules[i]);
			tokens[i] = instruction.substr (result.position(),result.length());
			//LOG(LEVEL_INFO) << "token = " << tokens[i];
			instruction = instruction.substr (result.position() + result.length(), instruction.length() - (result.position() + result.length()));
			//LOG(LEVEL_INFO) << "instruction" << instruction;
			i++;
		}
		//LOG(LEVEL_INFO) << "It is correct, R2";
		LOG(LEVEL_DEBUG) << "R1 tokens " << tokens[0] << ", " << tokens[1] << ", " << tokens[2] << ", " << tokens[3];
		results.second = tokens;
		return results;
	}
	//LOG(LEVEL_ERROR) << "Error sintatic error R2";
	results.first = 0;
	results.second = tokens;
	return results;
}

pair<bool, vector<string> > identifyTypeR2(string instruction)
{
	regex correct("(:?[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: )?[a-z]+ \\$(:?(:?.{2})|(:?zero)),\\$(:?(:?.{2})|(:?zero)),[0-9]+");
	regex correctInstruction("[a-z]+");
	regex correctParameter1("\\$(:?(:?zero)|(:?.{2}))");
	regex correctParameter2("\\$(:?(:?zero)|(:?.{2}))");
	regex correctImidiate("[0-9]+");
	regex label("[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: ");
	vector<regex> rules (4);
	vector<string> tokens (4);
	pair<bool, vector<string> > results;
	results.second = tokens;
	cmatch result;
	int i = 0;
	
	rules[0] = correctInstruction;
	rules[1] = correctParameter1;
	rules[2] = correctParameter2;
	rules[3] = correctImidiate;
	
	if(regex_match(instruction.begin(), instruction.end(), correct))
	{
		if (regex_search(instruction.c_str(), result, label))
		{
			instruction = instruction.substr (result.position() + result.length(), instruction.length() - (result.position() + result.length()));
		}
		results.first = 1;
		while(i < 4)
		{
			regex_search(instruction.c_str(), result, rules[i]);
			tokens[i] = instruction.substr (result.position(),result.length());
			//LOG(LEVEL_INFO) << "token = " << tokens[i];
			instruction = instruction.substr (result.position() + result.length(), instruction.length() - (result.position() + result.length()));
			//LOG(LEVEL_INFO) << "instruction" << instruction;
			i++;
		}
		LOG(LEVEL_DEBUG) << "R2 tokens " << tokens[0] << ", " << tokens[1] << ", " << tokens[2] << ", " << tokens[3];
		results.second = tokens;
		return results;
	}
	//LOG(LEVEL_ERROR) << "Error sintatic error R2";
	results.first = 0;
	results.second = tokens;
	return results;
}

pair<bool, vector<string> > identifyTypeR3(string instruction)
{
	regex correct("(:?[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: )?[a-z]+ \\$(:?(:?.{2})|(:?zero))");
	regex correctInstruction("[a-z]+");
	regex correctParameter1("\\$(:?(:?zero)|(:?.{2}))");
	regex label("[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: ");
	vector<regex> rules (2);
	vector<string> tokens (2);
	pair<bool, vector<string> > results;
	results.second = tokens;
	cmatch result;
	int i = 0;
	
	rules[0] = correctInstruction;
	rules[1] = correctParameter1;
	
	if(regex_match(instruction.begin(), instruction.end(), correct))
	{
		if (regex_search(instruction.c_str(), result, label))
		{
			instruction = instruction.substr (result.position() + result.length(), instruction.length() - (result.position() + result.length()));
		}
		results.first = 1;
		while(i < 2)
		{
			regex_search(instruction.c_str(), result, rules[i]);
			tokens[i] = instruction.substr (result.position(),result.length());
			//LOG(LEVEL_INFO) << "token = " << tokens[i];
			instruction = instruction.substr (result.position() + result.length(), instruction.length() - (result.position() + result.length()));
			//LOG(LEVEL_INFO) << "instruction" << instruction;
			i++;
		}
		LOG(LEVEL_DEBUG) << "R3 tokens " << tokens[0] << ", " << tokens[1];
		results.second = tokens;
		return results;
	}
	//LOG(LEVEL_ERROR) << "Error sintatic error R2";
	results.first = 0;
	results.second = tokens;
	return results;
}

pair<bool, vector<string> > identifyTypeI1(string instruction)
{
	regex correct("(:?[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: )?[a-z]+ \\$(:?(:?.{2})|(:?zero)),\\$(:?(:?.{2})|(:?zero)),[0-9]+");
	regex correctInstruction("[a-z]+");
	regex correctParameter1("\\$(:?(:?zero)|(:?.{2}))");
	regex correctParameter2("\\$(:?(:?zero)|(:?.{2}))");
	regex correctImidiate("[0-9]+");
	regex label("[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: ");
	vector<regex> rules (4);
	vector<string> tokens (4);
	pair<bool, vector<string> > results;
	results.second = tokens;
	cmatch result;
	int i = 0;
	
	rules[0] = correctInstruction;
	rules[1] = correctParameter1;
	rules[2] = correctParameter2;
	rules[3] = correctImidiate;
	
	if(regex_match(instruction.begin(), instruction.end(), correct))
	{
		if (regex_search(instruction.c_str(), result, label))
		{
			instruction = instruction.substr (result.position() + result.length(), instruction.length() - (result.position() + result.length()));
		}
		results.first = 1;
		while(i < 4)
		{
			regex_search(instruction.c_str(), result, rules[i]);
			tokens[i] = instruction.substr (result.position(),result.length());
			//LOG(LEVEL_INFO) << "token = " << tokens[i];
			instruction = instruction.substr (result.position() + result.length(), instruction.length() - (result.position() + result.length()));
			//LOG(LEVEL_INFO) << "instruction" << instruction;
			i++;
		}
		LOG(LEVEL_DEBUG) << "I1 tokens " << tokens[0] << ", " << tokens[1] << ", " << tokens[2] << ", " << tokens[3];
		results.second = tokens;
		return results;
	}
//	LOG(LEVEL_ERROR) << "Error sintatic error I1";
	results.first = 0;
	results.second = tokens;
	return results;
}

pair<bool, vector<string> > identifyTypeI2(string instruction)
{
	regex correct("(:?[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: )?[a-z]+ \\$(:?(:?.{2})|(:?zero)),[0-9]+\\(\\$(:?(:?.{2})|(:?zero))\\)");
	regex correctInstruction("[a-z]+");
	regex correctParameter1("\\$(:?(:?zero)|(:?.{2}))");
	regex correctParameter2("[0-9]+");
	regex correctParameter3("\\(\\$(:?(:?zero)|(:?.{2}))\\)");
	regex label("[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: ");
	vector<regex> rules (4);
	vector<string> tokens (4);
	pair<bool, vector<string> > results;
	results.second = tokens;
	cmatch result;
	int i = 0;
	rules[0] = correctInstruction;
	rules[1] = correctParameter1;
	rules[2] = correctParameter2;
	rules[3] = correctParameter3;
	
	if(regex_match(instruction.begin(), instruction.end(), correct))
	{
		
		if (regex_search(instruction.c_str(), result, label))
		{
			instruction = instruction.substr (result.position() + result.length(), instruction.length() - (result.position() + result.length()));
		}
		results.first = 1;
		while(i < 4)
		{
			regex_search(instruction.c_str(), result, rules[i]);
			tokens[i] = instruction.substr (result.position(),result.length());
			//LOG(LEVEL_INFO) << "I2token = " << tokens[i];
			instruction = instruction.substr (result.position() + result.length(), instruction.length() - (result.position() + result.length()));
			//LOG(LEVEL_INFO) << "instruction" << instruction;
			i++;
		}
		tokens[3] = tokens[3].substr (2,tokens[3].length()-3);
		tokens[3].insert(0,"$");
		LOG(LEVEL_DEBUG) << "I2 tokens " << tokens[0] << ", " << tokens[1] << ", " << tokens[2] << ", " << tokens[3];
		results.second = tokens;
		return results;
	}
	LOG(LEVEL_WARN) << "Labels Found:";
//	LOG(LEVEL_ERROR) << "Error sintatic error I2";
	results.first = 0;
	results.second = tokens;
	return results;
}

pair<bool, vector<string> > identifyTypeI3(string instruction)
{
	regex correct("(:?[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: )?[a-z]+ \\$(:?(:?.{2})|(:?zero)),[0-9]+");
	regex correctInstruction("[a-z]+");
	regex correctParameter1("\\$(:?(:?zero)|(:?.{2}))");
	regex correctParameter2("[0-9]+");
	regex label("[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: ");
	vector<regex> rules (3);
	vector<string> tokens (3);
	pair<bool, vector<string> > results;
	results.second = tokens;
	cmatch result;
	int i = 0;
	rules[0] = correctInstruction;
	rules[1] = correctParameter1;
	rules[2] = correctParameter2;
	
	if(regex_match(instruction.begin(), instruction.end(), correct))
	{
		if(regex_search(instruction.c_str(), result, label))
		{
			instruction = instruction.substr (result.position() + result.length(), instruction.length() - (result.position() + result.length()));
		}
		results.first = 1;
		while(i < 3)
		{
			regex_search(instruction.c_str(), result, rules[i]);
			tokens[i] = instruction.substr (result.position(),result.length());
			//LOG(LEVEL_INFO) << "I2token = " << tokens[i];
			instruction = instruction.substr (result.position() + result.length(), instruction.length() - (result.position() + result.length()));
			//LOG(LEVEL_INFO) << "instruction" << instruction;
			i++;
		}
		LOG(LEVEL_DEBUG) << "I3 tokens " << tokens[0] << ", " << tokens[1] << ", " << tokens[2];
		results.second = tokens;
		return results;
	}
//	LOG(LEVEL_ERROR) << "Error sintatic error I3";
	results.first = 0;
	results.second = tokens;
	return results;
}

pair<bool, vector<string> > identifyTypeJ(string instruction)
{
	regex correct("(:?[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: )?[a-z]+ (:?[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*)");
	regex correctInstruction("[a-z]+");
	regex correctParameter1("(:?[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*)");
	regex label("[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: ");
	vector<regex> rules (2);
	vector<string> tokens (2);
	pair<bool, vector<string> > results;
	results.second = tokens;
	cmatch result;
	int i = 0;
	rules[0] = correctInstruction;
	rules[1] = correctParameter1;
	
	if(regex_match(instruction.begin(), instruction.end(), correct))
	{
		if(regex_search(instruction.c_str(), result, label))
		{
			instruction = instruction.substr (result.position() + result.length(), instruction.length() - (result.position() + result.length()));
		}
		results.first = 1;
		while(i < 2)
		{
			regex_search(instruction.c_str(), result, rules[i]);
			tokens[i] = instruction.substr (result.position(),result.length());
			//LOG(LEVEL_INFO) << "I2token = " << tokens[i];
			instruction = instruction.substr (result.position() + result.length(), instruction.length() - (result.position() + result.length()));
			//LOG(LEVEL_INFO) << "instruction" << instruction;
			i++;
		}
		LOG(LEVEL_DEBUG) << "J tokens " << tokens[0] << ", " << tokens[1];
		results.second = tokens;
		return results;
	}
//	LOG(LEVEL_ERROR) << "Error sintatic error J";
	results.first = 0;
	results.second = tokens;
	return results;
}

pair<bool, vector<string> > identifyTypeFloat(string instruction)
{
	regex correct("(:?[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: )?[a-z]+\\.[a-z] \\$(:?(:?.{2,3})|(:?zero)),\\$(:?(:?.{2,3})|(:?zero)),\\$(:?(:?.{2,3})|(:?zero))");
	regex correctInstruction("[a-z]+\\.[a-z]");
	regex correctParameter1("\\$(:?(:?zero)|(:?[a-z]|[0-9]){2,3})");
	regex correctParameter2("\\$(:?(:?zero)|(:?[a-z]|[0-9]){2,3})");
	regex correctParameter3("\\$(:?(:?zero)|(:?[a-z]|[0-9]){2,3})");
	regex label("[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: ");
	vector<regex> rules (4);
	vector<string> tokens (4);
	pair<bool, vector<string> > results;
	results.second = tokens;
	cmatch result;
	int i = 0;
	
	rules[0] = correctInstruction;
	rules[1] = correctParameter1;
	rules[2] = correctParameter2;
	rules[3] = correctParameter3;
	
	if(regex_match(instruction.begin(), instruction.end(), correct))
	{
		if (regex_search(instruction.c_str(), result, label))
		{
			instruction = instruction.substr (result.position() + result.length(), instruction.length() - (result.position() + result.length()));
		}
		results.first = 1;
		while(i < 4)
		{
			regex_search(instruction.c_str(), result, rules[i]);
			tokens[i] = instruction.substr (result.position(),result.length());
			//LOG(LEVEL_INFO) << "token = " << tokens[i];
			instruction = instruction.substr (result.position() + result.length(), instruction.length() - (result.position() + result.length()));
			//LOG(LEVEL_INFO) << "instruction" << instruction;
			i++;
		}
		//LOG(LEVEL_INFO) << "It is correct, R2";
		LOG(LEVEL_DEBUG) << "Float tokens " << tokens[0] << ", " << tokens[1] << ", " << tokens[2] << ", " << tokens[3];
		results.second = tokens;
		return results;
	}
	//LOG(LEVEL_ERROR) << "Error sintatic error R2";
	results.first = 0;
	results.second = tokens;
	return results;
}

pair<bool, vector<string> > identifyTypePseudo1(string instruction)
{
	regex correct("(:?[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: )?[a-z]+ \\$(:?(:?.{2})|(:?zero)),\\$(:?(:?.{2})|(:?zero))");
	regex correctInstruction("[a-z]+");
	regex correctParameter1("\\$(:?(:?zero)|(:?.{2}))");
	regex correctParameter2("\\$(:?(:?zero)|(:?.{2}))");
	regex label("[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: ");
	vector<regex> rules (3);
	vector<string> tokens (3);
	pair<bool, vector<string> > results;
	results.second = tokens;
	cmatch result;
	int i = 0;
	
	rules[0] = correctInstruction;
	rules[1] = correctParameter1;
	rules[2] = correctParameter2;
	
	if(regex_match(instruction.begin(), instruction.end(), correct))
	{
		if (regex_search(instruction.c_str(), result, label))
		{
			instruction = instruction.substr (result.position() + result.length(), instruction.length() - (result.position() + result.length()));
		}
		results.first = 1;
		while(i < 3)
		{
			regex_search(instruction.c_str(), result, rules[i]);
			tokens[i] = instruction.substr (result.position(),result.length());
			//LOG(LEVEL_INFO) << "token = " << tokens[i];
			instruction = instruction.substr (result.position() + result.length(), instruction.length() - (result.position() + result.length()));
			//LOG(LEVEL_INFO) << "instruction" << instruction;
			i++;
		}
		LOG(LEVEL_DEBUG) << "Pseudo1 tokens " << tokens[0] << ", " << tokens[1] << ", " << tokens[2];
		results.second = tokens;
		return results;
	}
	//LOG(LEVEL_ERROR) << "Error sintatic error R2";
	results.first = 0;
	results.second = tokens;
	return results;
}

pair<bool, vector<string> > identifyTypePseudo2(string instruction)
{
	regex correct("(:?[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: )?[a-z]+ \\$(:?(:?.{2})|(:?zero))");
	regex correctInstruction("[a-z]+");
	regex correctParameter1("\\$(:?(:?zero)|(:?.{2}))");
	regex label("[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: ");
	vector<regex> rules (2);
	vector<string> tokens (2);
	pair<bool, vector<string> > results;
	results.second = tokens;
	cmatch result;
	int i = 0;
	
	rules[0] = correctInstruction;
	rules[1] = correctParameter1;
	
	if(regex_match(instruction.begin(), instruction.end(), correct))
	{
		if (regex_search(instruction.c_str(), result, label))
		{
			instruction = instruction.substr (result.position() + result.length(), instruction.length() - (result.position() + result.length()));
		}
		results.first = 1;
		while(i < 2)
		{
			regex_search(instruction.c_str(), result, rules[i]);
			tokens[i] = instruction.substr (result.position(),result.length());
			//LOG(LEVEL_INFO) << "token = " << tokens[i];
			instruction = instruction.substr (result.position() + result.length(), instruction.length() - (result.position() + result.length()));
			//LOG(LEVEL_INFO) << "instruction" << instruction;
			i++;
		}
		LOG(LEVEL_DEBUG) << "Pseudo2 tokens " << tokens[0] << ", " << tokens[1];
		results.second = tokens;
		return results;
	}
	//LOG(LEVEL_ERROR) << "Error sintatic error R2";
	results.first = 0;
	results.second = tokens;
	return results;
}

pair<bool, vector<string> > identifyTypePseudo3(string instruction)
{
	regex correct("(:?[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: )?[a-z]+ (:?[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*)");
	regex correctInstruction("[a-z]+");
	regex correctParameter1("(:?[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*)");
	regex label("[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: ");
	vector<regex> rules (2);
	vector<string> tokens (2);
	pair<bool, vector<string> > results;
	results.second = tokens;
	cmatch result;
	int i = 0;
	rules[0] = correctInstruction;
	rules[1] = correctParameter1;
	
	if(regex_match(instruction.begin(), instruction.end(), correct))
	{
		if(regex_search(instruction.c_str(), result, label))
		{
			instruction = instruction.substr (result.position() + result.length(), instruction.length() - (result.position() + result.length()));
		}
		results.first = 1;
		while(i < 2)
		{
			regex_search(instruction.c_str(), result, rules[i]);
			tokens[i] = instruction.substr (result.position(),result.length());
			//LOG(LEVEL_INFO) << "I2token = " << tokens[i];
			instruction = instruction.substr (result.position() + result.length(), instruction.length() - (result.position() + result.length()));
			//LOG(LEVEL_INFO) << "instruction" << instruction;
			i++;
		}
		LOG(LEVEL_DEBUG) << "Pseudo3 tokens " << tokens[0] << ", " << tokens[1];
		results.second = tokens;
		return results;
	}
//	LOG(LEVEL_ERROR) << "Error sintatic error J";
	results.first = 0;
	results.second = tokens;
	return results;
}

pair<bool, vector<string> > identifyTypePseudo4(string instruction)
{
	regex correct("(:?[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: )?[a-z]+ \\$(:?(:?.{2})|(:?zero)),\\$(:?(:?.{2})|(:?zero)),(:?[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*)?");
	regex correctInstruction("[a-z]+");
	regex correctParameter1("\\$(:?(:?zero)|(:?.{2}))");
	regex correctParameter2("\\$(:?(:?zero)|(:?.{2}))");
	regex correctParameter3("(:?[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*)");
	regex label("[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: ");
	vector<regex> rules (4);
	vector<string> tokens (4);
	pair<bool, vector<string> > results;
	results.second = tokens;
	cmatch result;
	int i = 0;
	
	rules[0] = correctInstruction;
	rules[1] = correctParameter1;
	rules[2] = correctParameter2;
	rules[3] = correctParameter3;
	
	if(regex_match(instruction.begin(), instruction.end(), correct))
	{
		if (regex_search(instruction.c_str(), result, label))
		{
			instruction = instruction.substr (result.position() + result.length(), instruction.length() - (result.position() + result.length()));
		}
		results.first = 1;
		while(i < 4)
		{
			regex_search(instruction.c_str(), result, rules[i]);
			tokens[i] = instruction.substr (result.position(),result.length());
			//LOG(LEVEL_INFO) << "token = " << tokens[i];
			instruction = instruction.substr (result.position() + result.length(), instruction.length() - (result.position() + result.length()));
			//LOG(LEVEL_INFO) << "instruction" << instruction;
			i++;
		}
		//LOG(LEVEL_INFO) << "It is correct, R2";
		LOG(LEVEL_DEBUG) << "Pseudo4 tokens " << tokens[0] << ", " << tokens[1] << ", " << tokens[2] << ", " << tokens[3];
		results.second = tokens;
		return results;
	}
	//LOG(LEVEL_ERROR) << "Error sintatic error R2";
	results.first = 0;
	results.second = tokens;
	return results;
}

pair<bool, vector<string> > identifyTypePseudo5(string instruction)
{
	regex correct("(:?[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: )?[a-z]+ \\$(:?(:?.{2})|(:?zero)),\\$(:?(:?.{2})|(:?zero)),(:?[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*)?");
	regex correctInstruction("[a-z]+");
	regex correctParameter1("\\$(:?(:?zero)|(:?.{2}))");
	regex correctParameter2("\\$(:?(:?zero)|(:?.{2}))");
	regex correctParameter3("(:?[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*)");
	regex label("[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: ");
	vector<regex> rules (4);
	vector<string> tokens (4);
	pair<bool, vector<string> > results;
	results.second = tokens;
	cmatch result;
	int i = 0;
	
	rules[0] = correctInstruction;
	rules[1] = correctParameter1;
	rules[2] = correctParameter2;
	rules[3] = correctParameter3;
	
	if(regex_match(instruction.begin(), instruction.end(), correct))
	{
		if (regex_search(instruction.c_str(), result, label))
		{
			instruction = instruction.substr (result.position() + result.length(), instruction.length() - (result.position() + result.length()));
		}
		results.first = 1;
		while(i < 4)
		{
			regex_search(instruction.c_str(), result, rules[i]);
			tokens[i] = instruction.substr (result.position(),result.length());
			//LOG(LEVEL_INFO) << "token = " << tokens[i];
			instruction = instruction.substr (result.position() + result.length(), instruction.length() - (result.position() + result.length()));
			//LOG(LEVEL_INFO) << "instruction" << instruction;
			i++;
		}
		LOG(LEVEL_DEBUG) << "Pseudo5 tokens " << tokens[0] << ", " << tokens[1] << ", " << tokens[2] << ", " << tokens[3];
		results.second = tokens;
		return results;
	}
//	LOG(LEVEL_ERROR) << "Error sintatic error P5";
	results.first = 0;
	results.second = tokens;
	return results;
}
