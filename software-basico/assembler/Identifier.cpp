#include <Identifier.hpp>

void identifyTypeR(string token)
{
	regex correct("[a-z]+ \\$.+, \\$.+, \\$.{2}");
	if(regex_match(token.begin(), token.end(), correct))
	{
		LOG(LEVEL_INFO) << "It is correct";
		return;
	}
	LOG(LEVEL_ERROR) << "Error sintatic error";
	return;
}

bool identifyToken(string token)
{
	regex instruction("[a-z]+ .*");
	regex label("[a-z]+: [a-z]+");
	
	if(regex_match(token.begin(), token.end(), instruction))
	{
		//LOG(LEVEL_INFO) << "It is instruction";
		return false;
	}
	if(regex_match(token.begin(), token.end(), label))
	{
		//LOG(LEVEL_INFO) << "It is label";
		return true;
	}
	LOG(LEVEL_ERROR) << "Error token doesn't exist";
	LOG(LEVEL_ERROR) << token;
	return false;
}

