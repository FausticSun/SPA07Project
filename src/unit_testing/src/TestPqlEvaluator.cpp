
#include "PqlEvaluator.h"
#include  "MergeTables.h"
#include "catch.hpp"
#include <algorithm>
using namespace std;


SCENARIO("merging test") {
	
	vector<QueryEntity> titleA{ QueryEntity(QueryEntityType::Stmt,"s")
	,QueryEntity(QueryEntityType::Variable,"a") };
	vector<QueryEntity> titleB{ QueryEntity(QueryEntityType::Stmt,"s")
	  ,QueryEntity(QueryEntityType::Stmt,"s1") };
	vector<vector<string>> tableA{ {"1","a"},{"1","b"},{"1","c"},{"1","d"}, {"2","a"},{"3","b"}};
	vector<vector<string>> tableB{ {"1","2"},{"1","4"},{"10","5"},{"1","7"}, {"5","9"},{"4","10"}};
	ClauseResult a(false, false,titleA,tableA);
	ClauseResult b(false, false,titleB,tableB);
	vector<ClauseResult> clauseResults{ a,b };
	MergeTables mt(clauseResults);
	ClauseResult tem = mt.getResultTables();
  SECTION("output result")
  {
    for(int i=0;i<tem.titles.size();i++)
    {
		cout << tem.titles[i].name << " ";
    }
	cout << endl;
	  for (int i = 0; i < tem.resultTable.size(); i++)
	  {
		  for (int j = 0; j < tem.resultTable[i].size(); j++)
		  {
			  cout << tem.resultTable[i][j] << " ";
		  }
		  cout << endl;
	  }
  }
  
}
