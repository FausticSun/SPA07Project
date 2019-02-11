#include "SPA.h"
#include "PqlEvaluator.h"

void SPA::parseSIMPLEFile(std::string filename) { (void)filename; }
const std::list<std::string> SPA::evaluateQuery(std::string query) const {
  (void)query;
  PqlEvaluator pe(pkb);
  list<string> results = pe.evaluateQuery(query);
  return results;
}
