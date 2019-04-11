#include "Table.h"
#include "catch.hpp"

SCENARIO("Table") {
  GIVEN("a table has yet to be created") {
    WHEN("a table is created with 3 columns") {
      Table t(3);
      THEN("it is empty") { REQUIRE(t.empty()); }
      THEN("it has headers labeled 0-2") {
        REQUIRE(t.getHeader() == std::vector<std::string>{"0", "1", "2"});
      }
    }
    WHEN("a table is created with headers") {
      std::vector<std::string> headers = {"h1", "h2", "h3"};
      Table t(headers);
      THEN("it is empty") { REQUIRE(t.empty()); }
      THEN("it has headers are set correctly") {
        REQUIRE(t.getHeader() == headers);
      }
    }
    WHEN("a table is created with -1 columns") {
      THEN("an expection is thrown") { REQUIRE_THROWS(Table(-1)); }
    }
  }
  GIVEN("an empty table with 3 columns labeled 0-2") {
    Table t(3);
    WHEN("a data row with 3 columns is inserted") {
      std::vector<std::string> data = {"a", "b", "c"};
      t.insertRow(data);
      THEN("the table is not empty") { REQUIRE(!t.empty()); }
      THEN("the table contains the inserted data") {
        REQUIRE(t.getData().count(data) == 1);
      }
      THEN("retrieving data with specificed columns contains the inserted "
           "data") {
        REQUIRE(t.getData({"0", "2"}).count({"a", "c"}) == 1);
      }
    }
    WHEN("a data row with 1 column is inserted") {
      std::vector<std::string> data = {"a"};
      THEN("an exception is throw") { REQUIRE_THROWS(t.insertRow(data)); }
    }
    WHEN("the header is set with a header with 3 columns") {
      std::vector<std::string> header = {"h1", "h2", "h3"};
      t.setHeader(header);
      THEN("the header is set") { REQUIRE(t.getHeader() == header); }
    }
    WHEN("the header is set with a header with 1 columns") {
      std::vector<std::string> header = {"h1"};
      THEN("an exception is throw") { REQUIRE_THROWS(t.setHeader(header)); }
    }
  }
  GIVEN("a table with 3 columns containing some data") {
    Table t({"h1", "h2", "h3"});
    t.insertRow({"1", "2", "3"});
    t.insertRow({"a", "b", "c"});
    WHEN("a column is dropped") {
      t.dropColumn("h2");
      THEN("the column and its data is dropped") {
        std::vector<std::string> header{"h1", "h3"};
        REQUIRE(t.getHeader() == header);
        REQUIRE(t.contains({"1", "3"}));
        REQUIRE(t.contains({"a", "c"}));
      }
    }
  }
  GIVEN("2 tables with common columns") {
    Table t1({"h1", "h2", "h3", "h4"});
    t1.insertRow({"1", "a", "2", "a"});
    t1.insertRow({"1", "a", "2", "b"});
    t1.insertRow({"1", "c", "2", "c"});
    t1.insertRow({"3", "a", "4", "a"});
    Table t2({"h2", "h4", "h5", "h6"});
    t2.insertRow({"a", "a", "1", "2"});
    t2.insertRow({"a", "b", "1", "2"});
    t2.insertRow({"a", "b", "3", "4"});
    t2.insertRow({"d", "d", "1", "2"});
    WHEN("t1 is merged with t2") {
      t1.mergeWith(t2);
      THEN("t1 contains the natural union of t1 and t2") {
        std::vector<std::string> header{"h1", "h2", "h3", "h4", "h5", "h6"};
        REQUIRE(t1.getHeader() == header);
        auto data = t1.getData();
        REQUIRE(data.size() == 4);
        REQUIRE(data.count({"1", "a", "2", "a", "1", "2"}) == 1);
        REQUIRE(data.count({"3", "a", "4", "a", "1", "2"}) == 1);
        REQUIRE(data.count({"1", "a", "2", "b", "1", "2"}) == 1);
        REQUIRE(data.count({"1", "a", "2", "b", "3", "4"}) == 1);
      }
    }
  }
  GIVEN("2 tables without common columns") {
    Table t1({"h1", "h2"});
    t1.insertRow({"1", "2"});
    t1.insertRow({"3", "4"});
    Table t2({"h3", "h4"});
    t2.insertRow({"a", "b"});
    t2.insertRow({"c", "d"});
    WHEN("t1 is merged with t2") {
      t1.mergeWith(t2);
      THEN("t1 contains the cross product of t1 and t2") {
        std::vector<std::string> header{"h1", "h2", "h3", "h4"};
        REQUIRE(t1.getHeader() == header);
        auto data = t1.getData();
        REQUIRE(data.size() == 4);
        REQUIRE(data.count({"1", "2", "a", "b"}) == 1);
        REQUIRE(data.count({"1", "2", "c", "d"}) == 1);
        REQUIRE(data.count({"3", "4", "a", "b"}) == 1);
        REQUIRE(data.count({"3", "4", "c", "d"}) == 1);
      }
    }
  }
  GIVEN("2 table with common columns where 1 is empty") {
    Table t1({"h1", "h2"});
    Table t2({"h1", "h2"});
    t2.insertRow({"a", "b"});
    t2.insertRow({"c", "d"});
    WHEN("t1 is merged with t2") {
      t1.mergeWith(t2);
      THEN("t1 is empty") {
        std::vector<std::string> header{"h1", "h2"};
        REQUIRE(t1.getHeader() == header);
        REQUIRE(t1.empty());
      }
    }
  }

  GIVEN("2 table without common columns where 1 is empty") {
    Table t1({"h1", "h2"});
    Table t2({"h3", "h4"});
    t2.insertRow({"a", "b"});
    t2.insertRow({"c", "d"});
    WHEN("t1 is merged with t2") {
      t1.mergeWith(t2);
      THEN("t1 is empty") {
        std::vector<std::string> header{"h1", "h2", "h3", "h4"};
        REQUIRE(t1.getHeader() == header);
        REQUIRE(t1.empty());
      }
    }
  }

  GIVEN("2 table with the same headers") {
    Table t1({"h1", "h2"});
    t1.insertRow({"a", "b"});
    t1.insertRow({"c", "d"});
    Table t2({"h1", "h2"});
    t2.insertRow({"a", "b"});
    t2.insertRow({"e", "f"});
    t2.insertRow({"g", "h"});
    WHEN("t1 is concatenated with t2") {
      t1.concatenate(t2);
      THEN("t1 contains elements of t1 and t2") {
        std::vector<std::string> header{"h1", "h2"};
        REQUIRE(t1.getHeader() == header);
        auto data = t1.getData();
        REQUIRE(data.size() == 4);
        REQUIRE(data.count({"a", "b"}) == 1);
        REQUIRE(data.count({"c", "d"}) == 1);
        REQUIRE(data.count({"e", "f"}) == 1);
        REQUIRE(data.count({"g", "h"}) == 1);
      }
    }
    WHEN("t1 is set differenced with t2") {
      t1.setDifference(t2);
      THEN("t1 contains elements of t1 \ t2") {
        std::vector<std::string> header{"h1", "h2"};
        REQUIRE(t1.getHeader() == header);
        auto data = t1.getData();
        REQUIRE(data.size() == 1);
        REQUIRE(data.count({"c", "d"}) == 1);
      }
    }
  }
  GIVEN("2 table without the same headers") {
    Table t1({"h1", "h2"});
    t1.insertRow({"a", "b"});
    t1.insertRow({"c", "d"});
    Table t2({"h3", "h4"});
    t2.insertRow({"e", "f"});
    t2.insertRow({"g", "h"});
    WHEN("t1 is concatenated with t2") {
      THEN("an exception is throw") { REQUIRE_THROWS(t1.concatenate(t2)); }
    }
    WHEN("t1 is set differenced with t2") {
      THEN("an exception is throw") { REQUIRE_THROWS(t1.setDifference(t2)); }
    }
  }
  GIVEN("2 table without the same headers") {
    Table t1({"h1", "h2"});
    t1.insertRow({"a", "b"});
    t1.insertRow({"c", "d"});
    Table t2({"h3", "h4"});
    t2.insertRow({"e", "f"});
    t2.insertRow({"g", "h"});
    WHEN("t1 is concatenated with t2") {
      THEN("an exception is throw") { REQUIRE_THROWS(t1.concatenate(t2)); }
    }
    WHEN("t1 is set differenced with t2") {
      THEN("an exception is throw") { REQUIRE_THROWS(t1.setDifference(t2)); }
    }
  }
  GIVEN("a table with 2 columns") {
    Table t1({"h1", "h2"});
    t1.insertRow({"a", "b"});
    t1.insertRow({"b", "c"});
    t1.insertRow({"c", "d"});
    t1.insertRow({"d", "e"});

    WHEN("transitive closure of t1 is called") {
      t1.transitiveClosure();
      THEN("t1 contains the transitive closure of what was in t1") {
        std::vector<std::string> header{"h1", "h2"};
        REQUIRE(t1.getHeader() == header);
        auto data = t1.getData();
        REQUIRE(data.size() == 10);
        REQUIRE(data.count({"a", "b"}) == 1);
        REQUIRE(data.count({"a", "c"}) == 1);
        REQUIRE(data.count({"a", "d"}) == 1);
        REQUIRE(data.count({"a", "e"}) == 1);
        REQUIRE(data.count({"b", "c"}) == 1);
        REQUIRE(data.count({"b", "d"}) == 1);
        REQUIRE(data.count({"b", "e"}) == 1);
        REQUIRE(data.count({"c", "d"}) == 1);
        REQUIRE(data.count({"c", "e"}) == 1);
        REQUIRE(data.count({"d", "e"}) == 1);
      }
    }
  }
}
