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

void identifyToken(string token)
{
	regex instruction("[a-z]+ .*");
	regex label("[a-z]+: [a-z]+");
	
	if(regex_match(token.begin(), token.end(), instruction))
	{
		LOG(LEVEL_INFO) << "It is instruction";
		return;
	}
	if(regex_match(token.begin(), token.end(), label))
	{
		LOG(LEVEL_INFO) << "It is label";
		return;
	}
	LOG(LEVEL_ERROR) << "Error token doesn't exist";
	return;
}

