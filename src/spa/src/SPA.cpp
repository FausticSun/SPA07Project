#include "SPA.h"
#include "DesignExtractor.h"
#include "GeneralLexer.h"
#include "PQLEvaluator.h"
#include "SIMPLEParser.h"
#include "PQLParser.h"
#include <fstream>
#include <sstream>

SPA::SPA() : pkb(new PKB()) {}

void SPA::parseSIMPLEFile(std::string filename) {
  try {
    std::ifstream file;
    file.open(filename);
    std::istream &fileStream = file;
    auto tokens = Lexer::tokenize(fileStream);
    auto newPKB = Parser::parseSIMPLE(tokens);
    DesignExtractor::populateDesigns(newPKB);
    pkb.swap(newPKB);
  } catch (std::logic_error e) {
    exit(0);
  }
}
const std::list<std::string> SPA::evaluateQuery(std::string queryString) const {
  list<string> results;
  try {
    std::stringstream ss;
    ss << queryString;
    auto tokens = Lexer::tokenize(ss);
    auto query = Parser::parsePQL(tokens);
    PqlEvaluator pe(*pkb);
    auto results = pe.executeQuery(query);
    return results;
  } catch (logic_error le) {
    return results;
  } catch (invalid_argument ia) {
    return results;
  }
  return results;
}
