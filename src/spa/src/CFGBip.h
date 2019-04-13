#pragma once
#include "Table.h"
#include <map>
#include <queue>
#include <set>
#include <stack>

class CFGBip {
  enum class LinkType { CFLink, BranchIn, BranchBack };

	struct Link {
		LinkType type;
		int v;
		int source;
		
	};

private:
	Table nextBip{2};
	Table nextBipT{2};
	Table affectsBip{2};
	Table modifiesTable{2};
	Table whileIfTable{2};
	Table modifiesAssignTable{2};
	Table usesAssignTable{2};
	std::vector<std::vector<int>> controlFlowLink;
	std::vector<std::vector<int>> branchIn;
	std::vector<std::vector<int>> branchBack;
	std::map<int, std::vector<Link>> adjLst;
	std::map<int, std::string> callProcNameMap;
	std::map<std::string, std::vector<int>> procExitStmtMap;
	std::set<int> callSet;

	void buildCFGBip(Table, Table, Table, Table, Table);
	void populateTables(int);
	void addToAdjLst(int, Link);
	void populateNextBip(int);
	void populateNextBipT(int);
	void populateAffectsBip(int);
	std::deque<int> getAffectsForward(int, std::string);
	bool isCallStatement(int);
	bool checkUsesModifies(int, std::string, std::deque<int>&);

public:
  CFGBip();
  CFGBip(Table, Table, Table, Table, Table, int, Table, Table, Table, Table);
  Table getNextBip();
  Table getNextBipT();
  Table getAffectsBip();
};
