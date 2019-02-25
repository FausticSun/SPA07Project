#include "SPA.h"
#include "DesignExtractor.h"
#include "Lexer.h"
#include "PQLEvaluator.h"
#include "PQLLexer.h"
#include "PQLParser.h"
#include "Parser.h"
#include "PqlEvaluator.h"
#include <fstream>

SPA::SPA() : pkb(new PKB()) {}

void SPA::parseSIMPLEFile(std::string filename) {
  Parser parser;
  Lexer lexer;
  std::ifstream file;
  file.open(filename);
  std::istream &fileStream = file;
  auto tokens = lexer.tokenizeFile(fileStream);
  auto ast = parser.buildAst(tokens);
  DesignExtractor designExtractor(ast);
  std::unique_ptr<PKB> newPKB = designExtractor.getPKB();
  pkb.swap(newPKB);
}
const std::list<std::string> SPA::evaluateQuery(std::string queryString) const {
	list<string> results;
  try {
	  PQLParser pqlParser;
	  auto tokens = pqlParser.parse(queryString);
	  auto query = pqlParser.buildQuery(tokens);
	  PqlEvaluator pe(*pkb);
	  auto results = pe.executeQuery(query);
  }catch (logic_error le) {
	  return results;
  }catch(invalid_argument ia) {
	  return results;
  }
  return results;
}
