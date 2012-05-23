#include <Identifier.hpp>

void identifyTypeR(string token)
{
	regex correct("[a-z]+ \\$(?:zero)|(?:at)|(?:v[0-1])|(?:a[0-3])|(?:t[0-9])|(?:s[0-7])|(?:k[0-1])|(?:gp)|(?:sp)|(?:ra)");
	//regex correct("[a-z]+ \\$(?:t[0-9])");
	//regex correct("add \\$");
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

