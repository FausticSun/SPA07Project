#include <PqlEvaluator.h>

PqlEvaluator::PqlEvaluator(const PKB& pkb) { this->mypkb = pkb; }

PqlEvaluator::~PqlEvaluator() = default;

list<string> PqlEvaluator::evaluateQuery(string query) {
  string target;
  


  return;
}

struct find_by_EntityType {
  find_by_EntityType(const RelationType & rt, const EntityType & f, const EntityType & s)
  {
    relType = rt;
    first = f;
    second = s;
  }
  bool operator()(const Relation &rel) {
    return rel.type == relType && rel.leftEntity.type == first && rel.rightEntity.type == second;
  }
private:
  RelationType relType;
  EntityType first;
  EntityType second;
};

struct remove_by_first
{
public:
  remove_by_first(const string & n)
  {
    name = n;
  }
  bool operator()(const Relation& rel)
  {
    return rel.leftEntity.name == name;
  }
private:
  string name;
};

struct remove_by_second
{
public:
	remove_by_second(const string & n)
	{
		name = n;
	}
	bool operator()(const Relation& rel)
	{
		return rel.rightEntity.name == name;
	}
private:
	string name;
};

list<Relation> PqlEvaluator::getUses(Entity first, Entity second)
{
  set<Relation> relations = mypkb.getRelTable();
  set<Relation>::iterator iter = find_if(relations.begin(), relations.end(),
	  find_by_EntityType(RelationType::USES, first.type, second.type));
  list<Relation> results;

  while(true)
  {
    if (iter != relations.end()) {
      results.push_back(*iter);
      iter++;
    }
    else {
	    break;
    }
  }

  if(first.type == EntityType::CONSTANT)
  {
    results.remove_if(remove_by_first(first.name));
  }

  if(second.type == EntityType::CONSTANT)
  {
    results.remove_if(remove_by_second(second.name));
  }

  return results;
	  
}

list<Relation> PqlEvaluator::getModifies(Entity first, Entity second)
{
  set<Relation> relations = mypkb.getRelTable();
  set<Relation>::iterator iter = find_if(relations.begin(), relations.end(),
	  find_by_EntityType(RelationType::MODIFIES, first.type, second.type));
  list<Relation> results;

  while (true)
  {
	  if (iter != relations.end()) {
		  results.push_back(*iter);
		  iter++;
	  }
	  else {
		  break;
	  }
  }

  if (first.type == EntityType::CONSTANT)
  {
    results.remove_if(remove_by_first(first.name));
  }

  if (second.type == EntityType::CONSTANT)
  {
    results.remove_if(remove_by_second(second.name));
  }

  return results;

}

list<Relation> PqlEvaluator::getFollows (Entity first, Entity second)
{
  set<Relation> relations = mypkb.getRelTable();
  set<Relation>::iterator iter = find_if(relations.begin(), relations.end(),
	  find_by_EntityType(RelationType::FOLLOWS, first.type, second.type));
  list<Relation> results;

  while (true)
  {
	  if (iter != relations.end()) {
		  results.push_back(*iter);
		  iter++;
	  }
	  else {
		  break;
	  }
  }

  if (first.type == EntityType::CONSTANT)
  {
    results.remove_if(remove_by_first(first.name));
  }

  if (second.type == EntityType::CONSTANT)
  {
    results.remove_if(remove_by_second(second.name));
  }

  return results;

}

list<Relation> PqlEvaluator::getParents(Entity first, Entity second)
{
  set<Relation> relations = mypkb.getRelTable();
  set<Relation>::iterator iter = find_if(relations.begin(), relations.end(),
	  find_by_EntityType(RelationType::PARENT, first.type, second.type));
  list<Relation> results;

  while (true)
  {
	  if (iter != relations.end()) {
		  results.push_back(*iter);
		  iter++;
	  }
	  else {
		  break;
	  }
  }

  if (first.type == EntityType::CONSTANT)
  {
    results.remove_if(remove_by_first(first.name));
  }

  if (second.type == EntityType::CONSTANT)
  {
    results.remove_if(remove_by_second(second.name));
  }

  return results;

}




