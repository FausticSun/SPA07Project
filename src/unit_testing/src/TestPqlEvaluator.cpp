
#include "PqlEvaluator.h"
#include <algorithm>
#include "catch.hpp"
using namespace std;

//SCENARIO("Selecting variable command") {
//  string PQL = "variable v; select v;";
//  PqlEvaluator pe;
//  WHEN("PKB is contructed") {}
//}

SCENARIO("test") {
	set<Relation> relations;
	Relation a;
	a.type = RelationType::FOLLOWS;
	a.leftEntity = Entity();
	a.leftEntity.type = EntityType::CONSTANT;
	a.leftEntity.name = "1";
	a.rightEntity.type = EntityType::CONSTANT;
	a.rightEntity.name = "2";
	Relation b;
	b.type = RelationType::FOLLOWS;
	b.leftEntity = Entity();
	b.leftEntity.type = EntityType::CONSTANT;
	b.leftEntity.name = "a";
	b.rightEntity.type = EntityType::CONSTANT;
	b.rightEntity.name = "2";
	Relation c;
	c.type = RelationType::FOLLOWS;
	c.leftEntity = Entity();
	c.leftEntity.type = EntityType::VARIABLE;
	c.leftEntity.name = "b";
	c.rightEntity.type = EntityType::PROCEDURE;
	c.rightEntity.name = "c";
	relations.insert(a);
	relations.insert(b);
	relations.insert(c);
	set<string> varTables{"alice","bob","cs"};
	set<string> proTables{"main","readp","writep"};
	PKB pkb(varTables,proTables,relations);
	Entity a1;
	a1.type = EntityType::CONSTANT;
	a1.name = "1";
	Entity a2;
	a2.type = EntityType::CONSTANT;
	a2.name = "2";
	Entity a3;
	a3.type = EntityType::CONSTANT;
	a3.name = "a";
	Entity a4;
	a4.type = EntityType::VARIABLE;
	a4.name = "b";
	Entity a5;
	a5.type = EntityType::PROCEDURE;
	a5.name = "c";
	PqlEvaluator pe(pkb);
  
	SECTION("test")
	{
		list<Relation> r1 = pe.getFollows(a1, a2);
		cout << r1.size()<<endl;
		cout << r1.front().leftEntity.name << endl;
		cout << r1.front().rightEntity.name << endl;
		list<Relation> r2 = pe.getFollows(a3, a2);
		cout << r2.size() << endl;
		cout << r2.front().leftEntity.name << endl;
		cout << r2.front().rightEntity.name << endl;
		list<Relation> r3 = pe.getFollows(a4, a5);
		cout << r3.size() << endl;
		cout << r3.front().leftEntity.name << endl;
		cout << r3.front().rightEntity.name << endl;
	
	}
}
