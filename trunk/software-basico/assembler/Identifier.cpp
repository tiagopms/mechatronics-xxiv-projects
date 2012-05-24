#include <Identifier.hpp>

bool identifyToken(string token)
{
//	regex instruction("([a-z]+ .*)|(syscall)");
//	regex label("[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: [a-z]+.*");

	regex instruction("[a-z]+ .*");
	regex instructionHeader("[a-z]+ ");
	regex label("[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: .+");
	regex labelHeader("[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*:");
	cmatch result;
	
	if(regex_match(token.begin(), token.end(), instruction))
	{
//		LOG(LEVEL_INFO) << "It is instruction";
		regex_search(token.c_str(), result, instructionHeader);
		token = instruction.substr (result.position(),result.length());
		return false;
	}
	if(regex_match(token.begin(), token.end(), label))
	{
//		LOG(LEVEL_INFO) << "It is label";
		regex_search(token.c_str(), result, labelHeader);
		token = instruction.substr (result.position(),result.length());
		return true;
	}
	LOG(LEVEL_ERROR) << "Error token doesn't exist";
	LOG(LEVEL_ERROR) << token;
	return false;
}

pair<bool, vector<string> > identifyTypeR1(string instruction)
{
	regex correct("[a-z]+ \\$(:?(:?.{2})|(:?zero)),\\$(:?(:?.{2})|(:?zero)),\\$(:?(:?.{2})|(:?zero))");
	regex correctInstruction("[a-z]+");
	regex correctParameter1("\\$(:?(:?.{2})|(:?zero))");
	regex correctParameter2("\\$(:?(:?.{2})|(:?zero))");
	regex correctParameter3("\\$(:?(:?.{2})|(:?zero))");
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
		return tokens;
	}
	//LOG(LEVEL_ERROR) << "Error sintatic error R2";
	results.first = 0;
	return tokens;
}

pair<bool, vector<string> > identifyTypeR2(string instruction)
{
	regex correct("[a-z]+ \\$(:?(:?.{2})|(:?zero)),\\$(:?(:?.{2})|(:?zero)),[0-9]+");
	regex correctInstruction("[a-z]+");
	regex correctParameter1("\\$(:?(:?.{2})|(:?zero))");
	regex correctParameter2("\\$(:?(:?.{2})|(:?zero))");
	regex correctImidiate("[0-9]+");
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
		results.second = tokens;
		return results;
	}
	//LOG(LEVEL_ERROR) << "Error sintatic error R2";
	results.first = 0;
	results.second = tokens;
	return results;
}


