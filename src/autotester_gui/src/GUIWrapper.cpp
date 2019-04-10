#include "GUIWrapper.h"

#include <iostream>

// a default constructor
GUIWrapper::GUIWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
  spa = SPA();
}

// method for parsing the SIMPLE source
void GUIWrapper::parse(std::string filename) {
  // call your parser to do the parsing
  std::cout << "parsed " << filename << std::endl;
  // ...rest of your code...
  spa.parseSIMPLEFile(filename);
}

// method to evaluating a query
void GUIWrapper::evaluate(std::string query, std::list<std::string> &results) {
  // call your evaluator to evaluate the query here
  // ...code to evaluate query...
  /*auto spaResults =*/
  spa.evaluateQuery(query, results);

  std::cout << "query=  " << query << std::endl;
  results.push_back(query + "\nQuery result: ...");
  results.assign(results.begin(), results.end());
  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
}
