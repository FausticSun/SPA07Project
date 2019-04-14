#include "DesignExtractor.h"
#include "GeneralLexer.h"
#include "PKB.h"
#include "SIMPLEParser.h"
#include "catch.hpp"
#include "fstream"

TEST_CASE("ksdbc") {
	std::ifstream file;
	file.open("C:/Users/Flora Fong/Desktop/testing.txt");
	std::istream &fileStream = file;
	auto tokens = Lexer::tokenize(fileStream);
	auto pkb = Parser::parseSIMPLE(tokens);
	DesignExtractor::populateDesigns(pkb);
	auto affectsBipTable = pkb->getAffectsBip();
	auto nextBipTable = pkb->getNextBipT();

}