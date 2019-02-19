#pragma once

#include <string>
#include "PQLParser.h"

struct QueryToken {
	TokenType type;
	std::string name;
	QueryToken() : type(TokenType::Keyword), name("") {};
	QueryToken(const TokenType type, const std::string &name)
		: type(type), name(name) {}
};
