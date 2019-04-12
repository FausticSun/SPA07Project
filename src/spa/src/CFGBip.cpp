#include "CFGBip.h"

CFGBip::CFGBip() {}

CFGBip::CFGBip(Table callTable, Table nextTable, Table procTable, Table callProcNameTable, Table procExitStmtTable, int stmtCount) {
	buildCFGBip(callTable, nextTable, procTable, callProcNameTable, procExitStmtTable);
	populateTables(stmtCount);
}

void CFGBip::buildCFGBip(Table callTable, Table nextTable, Table procTable, Table callProcNameTable, Table procExitStmtTable) {

	std::map<std::string, std::vector<int>> procMap;
	for (auto data : procTable.getData()) {
		procMap.insert(std::make_pair(data[0], std::vector<int>{std::stoi(data[1]), std::stoi(data[2])}));
	}

	for (auto data : callProcNameTable.getData()) {
		callProcNameMap.insert(std::make_pair(std::stoi(data[0]), data[1]));
	}

	for (auto data : procExitStmtTable.getData()) {
		if (procExitStmtMap.count(data[0]) == 1) {
			procExitStmtMap[data[0]].push_back(std::stoi(data[1]));
		}
		else {
			procExitStmtMap.insert(std::make_pair(data[0], std::vector<int>{std::stoi(data[1])}));
		}
	}

	for (auto data : callTable.getData()) {
		callSet.insert(std::stoi(data[0]));
	}

	std::map<int, int> callNext;

	for (auto data : nextTable.getData()) {
		// not a call statement
		if (!isCallStatement(std::stoi(data[0]))) {
			controlFlowLink.push_back(std::vector<int>{std::stoi(data[0]), std::stoi(data[1])});
			nextBip.insertRow(data);
		}
		else {
			//store next for call statements
			callNext.insert(std::pair<int, int>(std::stoi(data[0]), std::stoi(data[1])));
		}
	}

	for (auto data : callProcNameTable.getData()) {
		//create branch in for all calls
		int stmt = std::stoi(data[0]);
		std::string procedure = data[1];
		nextBip.insertRow({ std::to_string(stmt), std::to_string(procMap[procedure][0]) });
		branchIn.push_back(std::vector<int> {stmt, procMap[procedure][0]});

		//create dummy node for call statements at exit stmt of procedure
		for (auto i : procExitStmtTable.getData()) {
			if (i[1] == std::to_string(stmt)) { //call is exit stmt
				callNext.insert(std::pair<int, int>(stmt, stmt * -1));
				controlFlowLink.push_back(std::vector<int> {stmt, stmt*-1});
			}
		}
	}

	std::vector<int> uncheckedExits;
	std::map<int, std::vector<std::pair<int, int>>> tempBranchbacks;

	//create branch backs
	for (auto i : callNext) {
		int curr = i.first;
		int next = i.second;
		std::string procedure = callProcNameMap[curr];
		for (int exit : procExitStmtMap[procedure]) {
			if (!isCallStatement(exit)) {
				branchBack.push_back(std::vector<int> {exit, next, curr});
			}
			else { 
				branchBack.push_back(std::vector<int> {exit*-1, next, curr});

			}
		}
	}
}

void CFGBip::populateTables(int stmtCount) {
	
	//building adjacency list (map)
	for (auto data : controlFlowLink) {
		Link l;
		l.type = LinkType::CFLink;
		l.v = data[1];
		addToAdjLst(data[0], l);
	}

	for (auto data : branchIn) {
		Link l;
		l.type = LinkType::BranchIn;
		l.v = data[1];
		addToAdjLst(data[0], l);
	}
	
	for (auto data : branchBack) {
		Link l;
		l.type = LinkType::BranchBack;
		l.v = data[1];
		l.source = data[2];
		addToAdjLst(data[0], l);
	}
	
	//populating Tables
	populateNextBip(stmtCount);
	populateNextBipT(stmtCount);
}

void CFGBip::addToAdjLst(int stmt, Link l) {
	if (adjLst.count(stmt) == 1) {
		adjLst[stmt].push_back(l);
	}
	else {
		adjLst[stmt] = std::vector<Link>{ l };
	}
}


void CFGBip::populateNextBip(int stmtCount) {
	for (int i = 1; i < stmtCount + 1; i++) {
		std::map<int, bool> visited;
		for (auto i : adjLst) {
			visited[i.first] = false;
		}
		std::queue<int> q;
		q.push(i);

		while (!q.empty()) {
			int curr = q.front();
			q.pop();
			for (Link l : adjLst[curr]) {
				int v = l.v;
				if (!visited[v]) {
					if ((v < 0) && (l.type == LinkType::BranchBack)) { //connected to dummy via branch back
						visited[v] = true;
						q.push(v);
					}
					else if (v > 0) {
						visited[v] = true;
						nextBip.insertRow({ std::to_string(i), std::to_string(v) });
					}
				}
			}
		}
	}

}


void CFGBip::populateNextBipT(int stmtCount) {

	//BFS from every statement number
	for (int i = 1; i < stmtCount + 1; i++) {

		std::map<int, bool> visited;
		for (auto i : adjLst) {
			visited[i.first] = false;
		}

		visited[0] = true;

		std::queue<int> q;
		std::stack<int> branches;
		q.push(i);

		while (!q.empty()) {
			int curr = q.front();
			q.pop();
			for (Link j : adjLst[curr]) {
				int v = j.v;
				LinkType type = j.type;

				if (type == LinkType::CFLink) {
					if (!visited[v]) {
						visited[v] = true;
						q.push(v);
						if (v > 0) {
							nextBipT.insertRow({ std::to_string(i), std::to_string(v) });
						}
					}
				}
				else if (type == LinkType::BranchIn) {
					if (!visited[v]) {
						visited[v] = true;
						q.push(v);
						for (int k = 0; k < procExitStmtMap[callProcNameMap[curr]].size(); k++) { //push one in for every exit stmt
							branches.push(curr);
						} 
						nextBipT.insertRow({ std::to_string(i), std::to_string(v) });
					}
					else { 
						for (auto k : procExitStmtMap[callProcNameMap[curr]]) { //already visited, just enqueue exit stmts
							if (isCallStatement(k)) {
								q.push(k*-1);
							}
							else {
								q.push(k);
							}
							branches.push(curr);

						}
					}
				}
				else if (type == LinkType::BranchBack) {
					if (branches.empty()) {
						visited[v] = true;
						q.push(v);
						if (v > 0) {
							nextBipT.insertRow({ std::to_string(i), std::to_string(v) });
						}
					}
					else {
						if (j.source == branches.top()) {
							branches.pop();
							visited[v] = true;
							q.push(v);
							if (v > 0) {
								nextBipT.insertRow({ std::to_string(i), std::to_string(v) });
							}
							break;
						}
					}
				}
			}
		}
	}

}

bool CFGBip::isCallStatement(int i) {
	return (callSet.count(i) == 1);
}


Table CFGBip::getNextBip() {
	return nextBip;
}

Table CFGBip::getNextBipT() {
	return nextBipT;
}
