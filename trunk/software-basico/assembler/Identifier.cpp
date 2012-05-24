#include <Identifier.hpp>

bool identifyToken(string token)
{
	regex instruction("[a-z]+ .*");
	regex label("[a-zA-Z]+(?:[0-9]*[a-zA-Z]*)*: [a-z]+");
	
	if(regex_match(token.begin(), token.end(), instruction))
	{
		LOG(LEVEL_INFO) << "It is instruction";
		return false;
	}
	if(regex_match(token.begin(), token.end(), label))
	{
		LOG(LEVEL_INFO) << "It is label";
		return true;
	}
	LOG(LEVEL_ERROR) << "Error token doesn't exist";
	LOG(LEVEL_ERROR) << token;
	return false;
}

void identifyTypeR1(string instruction)
{
	regex correct("[a-z]+ \\$(:?(:?.{2})|(:?zero)),\\$(:?(:?.{2})|(:?zero)),\\$(:?(:?.{2})|(:?zero))");
	if(regex_match(instruction.begin(), instruction.end(), correct))
	{
		LOG(LEVEL_INFO) << "It is correct, R1";
		return;
	}
	LOG(LEVEL_ERROR) << "Error sintatic error R1";
	return;
}

void identifyTypeR2(string instruction)
{
	regex correct("[a-z]+ \\$(:?(:?.{2})|(:?zero)),\\$(:?(:?.{2})|(:?zero)),[0-9]+");
	regex correctInstruction("[a-z]+");
	vector<string> tokens (4);
	cmatch result;
	const string a = "add";
	
	if(regex_match(instruction.begin(), instruction.end(), correct))
	{
		//regex_search(instruction, result, correctInstruction); NÃO FUNCIONA
		regex_search("add", result, correct);//FUNCIONA
		//LOG(LEVEL_INFO) << "instruction str:" << result.str; //<< "' at "result.position <" with length: " << result.length;
		LOG(LEVEL_INFO) << "It is correct, R2";
		return;
	}
	LOG(LEVEL_ERROR) << "Error sintatic error R2";
	return;
}


