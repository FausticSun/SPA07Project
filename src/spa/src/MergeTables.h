#pragma once

#include "PqlEvaluator.h"

class MergeTables
{
public:
	MergeTables(const vector<ClauseResult> &clauseResults):tables(clauseResults){};
	ClauseResult getResultTables();
private:
	ClauseResult MergeTwoTables(ClauseResult &cR1, ClauseResult &cR2);
	vector<ClauseResult> tables;
};