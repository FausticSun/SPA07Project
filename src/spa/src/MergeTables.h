#pragma once

#include <PqlEvaluator.h>

class MergeTables {
public:
  MergeTables(vector<ClauseResult> &clauseResults) {
    this->tables = clauseResults;
  };
  ClauseResult getResultTables();
private:
  ClauseResult MergeTwoTables(ClauseResult &cR1, ClauseResult &cR2);
  vector<ClauseResult> tables;
};
