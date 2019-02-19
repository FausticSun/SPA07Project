#include "SPA.h"
#include "DesignExtractor.h"
#include "Lexer.h"
#include "Parser.h"
#include "PqlEvaluator.h"
#include <fstream>

SPA::SPA() : pkb(new PKB()) {}

void SPA::parseSIMPLEFile(std::string filename) {
  Parser parser;
  Lexer lexer;
  auto tokens = lexer.tokenizeFile(filename);
  auto ast = parser.buildAst(tokens);
  DesignExtractor designExtractor(ast);
  std::unique_ptr<PKB> newPKB = designExtractor.getPKB();
  pkb.swap(newPKB);
}
const std::list<std::string> SPA::evaluateQuery(std::string query) const {
  // (void)query;
  // PqlEvaluator pe(pkb);
  // list<string> results = pe.evaluateQuery(query);
  //  return results;
  return {};
}
