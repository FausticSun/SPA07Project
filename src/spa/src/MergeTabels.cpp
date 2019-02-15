#include "MergeTables.h"

ClauseResult MergeTables::getResultTables()
{
	vector<ClauseResult> clauseResults = tables;

		for (int i = 0; i < clauseResults.size(); i++) {
			if (clauseResults[i].resultTable.empty()) {
				clauseResults.erase(clauseResults.begin() + i);
			}
		}
		if (clauseResults.size() == 0) {
			ClauseResult a(false, false);
			return a;
		}
		if (clauseResults.size() == 1) {
			return clauseResults[0];
		}

		while (clauseResults.size() > 1) {
			ClauseResult temp = MergeTwoTables (clauseResults[0], clauseResults[1]);
			clauseResults.erase(clauseResults.begin(), clauseResults.begin() + 2);
			clauseResults.push_back(temp);
		}

		return clauseResults[0];
};

ClauseResult MergeTables::MergeTwoTables(ClauseResult &cR1, ClauseResult &cR2) {
		map<int, int> commonCols;
		vector<int> Cols;
		vector<int> diffCols;
		vector<vector<string>> results;
		vector<QueryEntity> titles(cR1.titles.begin(), cR1.titles.end());
		for (int i = 0; i < cR1.titles.size(); i++) {
			for (int j = 0; j < cR2.titles.size(); j++) {
				if (cR1.titles[i].type == cR2.titles[j].type && cR1.titles[i].name == cR2.
					titles[j].name) {
					commonCols[i] = j;
					Cols.push_back(i);
				}
			}
		}
  for(int i=0;i<cR2.titles.size();i++)
  {
	  bool flag = true;
    for(int j = 0;j<Cols.size();j++)
    {
      if(i==commonCols[j])
      {
		  flag = false;
		  break;
      }
    }
    if(flag)
    {
		diffCols.push_back(i);
		titles.push_back(cR2.titles[i]);
    }
  }
		if (Cols.empty()) {
			//no common columns
			for (int i = 0; i < cR1.resultTable.size(); i++) {
				for (int j = 0; j < cR2.resultTable.size(); j++) {
					vector<string> temp;
					for (int m = 0; m < cR1.resultTable[i].size(); m++) {
						temp.push_back(cR1.resultTable[i][m]);

					}
					for (int n = 0; n < cR2.resultTable[j].size(); n++) {
						temp.push_back(cR2.resultTable[j][n]);
					}
					results.push_back(temp);
				}
			}
		}
		else {
			//have common columns
			bool flag;
			for (int i = 0; i < cR1.resultTable.size(); i++) {
				for (int j = 0; j < cR2.resultTable.size(); j++) {
					vector<string> temp(cR1.resultTable[i].begin(),
						cR1.resultTable[i].end());
					flag = true;
					for (int m = 0; m < Cols.size(); m++) {
						if (cR1.resultTable[i][Cols[m]] != cR2.resultTable[j][commonCols[Cols[m]]]
							) {
							flag = false;
						}

					}
					if (flag) {
						for (int n = 0; n < diffCols.size(); n++) {
							temp.push_back(cR2.resultTable[j][diffCols[n]]);
						}
						results.push_back(temp);
					}

				}
			}
			ClauseResult clauseResult(false, false, titles, results);
			return clauseResult;
		}
	
}
