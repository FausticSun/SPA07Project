#include "DesignExtractor.h"
#include "Parser.h"
#include "catch.hpp"

SCENARIO("Design Extractor extracts design abstractions from an AST into a PKB",
         "[DesignExtractor]") {
  GIVEN("An AST of an empty program") {
    std::unique_ptr<TNode> AST =
        std::unique_ptr<TNode>(new TNode(TNodeType::Program, ""));
    DesignExtractor de(AST);
    WHEN("The PKB is retrieved") {
      std::unique_ptr<PKB> pkb = de.getPKB();
      THEN("The PKB is empty") {
        REQUIRE(pkb->getProcTable().size() == 0);
        REQUIRE(pkb->getVarTable().size() == 0);
      }
    }
  }
  GIVEN("An AST of a program with 3 empty procedures") {
    std::vector<std::unique_ptr<TNode>> childNodes;

    childNodes.emplace_back(new TNode(TNodeType::StatementList, ""));
    std::unique_ptr<TNode> proc1 = std::unique_ptr<TNode>(
        new TNode(TNodeType::Procedure, "Proc1", std::move(childNodes)));
    childNodes.clear();

    childNodes.emplace_back(new TNode(TNodeType::StatementList, ""));
    std::unique_ptr<TNode> proc2 = std::unique_ptr<TNode>(
        new TNode(TNodeType::Procedure, "Proc2", std::move(childNodes)));
    childNodes.clear();

    childNodes.emplace_back(new TNode(TNodeType::StatementList, ""));
    std::unique_ptr<TNode> proc3 = std::unique_ptr<TNode>(
        new TNode(TNodeType::Procedure, "Proc3", std::move(childNodes)));
    childNodes.clear();

    childNodes.push_back(std::move(proc1));
    childNodes.push_back(std::move(proc2));
    childNodes.push_back(std::move(proc3));
    std::unique_ptr<TNode> AST = std::unique_ptr<TNode>(
        new TNode(TNodeType::Program, std::move(childNodes)));

    DesignExtractor de(AST);
    WHEN("The PKB is retrieved") {
      std::unique_ptr<PKB> pkb = de.getPKB();
      THEN("The PKB contains the 3 procedures") {
        auto procTable = pkb->getProcTable();
        REQUIRE(procTable.size() == 3);
        REQUIRE(procTable.find("Proc1") != procTable.end());
        REQUIRE(procTable.find("Proc2") != procTable.end());
        REQUIRE(procTable.find("Proc3") != procTable.end());
      }
      THEN("The PKB does not contain variables") {
        REQUIRE(pkb->getVarTable().size() == 0);
      }
    }
  }
}

SCENARIO("Extracting Follows relations from AST with no while/if statements") {

  GIVEN("AST with two assignment statements") {
    Parser parser;
    std::queue<Token> tokenQueue;
    tokenQueue.push(Token(TokenType::Procedure, "procedure"));
    tokenQueue.push(Token(TokenType::Identifier, "Example"));
    tokenQueue.push(Token(TokenType::Separator, "{"));
    tokenQueue.push(Token(TokenType::Identifier, "x"));
    tokenQueue.push(Token(TokenType::Separator, "="));
    tokenQueue.push(Token(TokenType::Constant, "0"));
    tokenQueue.push(Token(TokenType::Separator, ";"));
    tokenQueue.push(Token(TokenType::Identifier, "y"));
    tokenQueue.push(Token(TokenType::Separator, "="));
    tokenQueue.push(Token(TokenType::Constant, "0"));
    tokenQueue.push(Token(TokenType::Separator, ";"));
    tokenQueue.push(Token(TokenType::Separator, "}"));
    auto programTNode = parser.buildAst(tokenQueue);
    DesignExtractor de(programTNode);

    WHEN("The PKB is retrieved") {
      std::unique_ptr<PKB> pkb = de.getPKB();

      THEN("Check if Follows relations are extracted correctly") {
        // Follows(1, 2), Follows*(1, 2)
        std::set<std::string> followsOne = pkb->getFollows("1");
        std::set<std::string> followsOneT = pkb->getFollowsT("1");
        REQUIRE(followsOne.find("2") != followsOne.end());
        REQUIRE(followsOneT.find("2") != followsOneT.end());
        std::set<std::string> followedByTwo = pkb->getFollowedBy("2");
        std::set<std::string> followedByTwoT = pkb->getFollowedByT("2");
        REQUIRE(followedByTwo.find("1") != followedByTwo.end());
        REQUIRE(followedByTwoT.find("1") != followedByTwoT.end());
      }
    }
  }
}

SCENARIO("Extracting Parent relations from AST with one while statement") {

  GIVEN("AST with one while statement") {
    Parser parser;
    std::queue<Token> tokenQueue;
    tokenQueue.push(Token(TokenType::Procedure, "procedure"));
    tokenQueue.push(Token(TokenType::Identifier, "Example"));
    tokenQueue.push(Token(TokenType::Separator, "{"));
    tokenQueue.push(Token(TokenType::While, "while"));
    tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
    tokenQueue.push(Token(TokenType::Identifier, "x"));
    tokenQueue.push(Token(TokenType::Equal, "=="));
    tokenQueue.push(Token(TokenType::Constant, "0"));
    tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
    tokenQueue.push(Token(TokenType::Separator, "{"));
    tokenQueue.push(Token(TokenType::Read, "read"));
    tokenQueue.push(Token(TokenType::Identifier, "x"));
    tokenQueue.push(Token(TokenType::Separator, ";"));
    tokenQueue.push(Token(TokenType::Separator, "}"));
    tokenQueue.push(Token(TokenType::Separator, "}"));
    auto programTNode = parser.buildAst(tokenQueue);
    DesignExtractor de(programTNode);

    WHEN("The PKB is retrieved") {
      std::unique_ptr<PKB> pkb = de.getPKB();

      THEN("Check if Parent relations are extracted correctly") {
        // Parent(1, 2)
        std::set<std::string> parentOne = pkb->getParent("1");
        std::set<std::string> parentOneT = pkb->getParentT("1");
        REQUIRE(parentOne.find("2") != parentOne.end());
        REQUIRE(parentOneT.find("2") != parentOneT.end());
        std::set<std::string> parentOfTwo = pkb->getParentOf("2");
        std::set<std::string> parentOfTwoT = pkb->getParentOfT("2");
        REQUIRE(parentOfTwo.find("1") != parentOfTwo.end());
        REQUIRE(parentOfTwoT.find("1") != parentOfTwoT.end());
      }
    }
  }
}

SCENARIO("Extracting Modifies relations from AST with no while/if statements") {

  GIVEN("AST with one assignment and one read statement") {
    Parser parser;
    std::queue<Token> tokenQueue;
    tokenQueue.push(Token(TokenType::Procedure, "procedure"));
    tokenQueue.push(Token(TokenType::Identifier, "Example"));
    tokenQueue.push(Token(TokenType::Separator, "{"));
    tokenQueue.push(Token(TokenType::Read, "read"));
    tokenQueue.push(Token(TokenType::Identifier, "x"));
    tokenQueue.push(Token(TokenType::Separator, ";"));
    tokenQueue.push(Token(TokenType::Identifier, "y"));
    tokenQueue.push(Token(TokenType::Separator, "="));
    tokenQueue.push(Token(TokenType::Constant, "0"));
    tokenQueue.push(Token(TokenType::Separator, ";"));
    tokenQueue.push(Token(TokenType::Separator, "}"));
    auto programTNode = parser.buildAst(tokenQueue);
    DesignExtractor de(programTNode);

    WHEN("The PKB is retrieved") {
      std::unique_ptr<PKB> pkb = de.getPKB();

      THEN("Check if Modifies relations are extracted correctly") {
        std::set<std::string> procsModifiesVar = pkb->getModifies("Example");
        // Modifies("Example", "x")
        std::set<std::string> varXModifiedByProc = pkb->getModifiedBy("x");
        REQUIRE(procsModifiesVar.find("x") != procsModifiesVar.end());
        REQUIRE(varXModifiedByProc.find("Example") != varXModifiedByProc.end());
        // Modifies("Example", "y")
        std::set<std::string> varYModifiedByProc = pkb->getModifiedBy("y");
        REQUIRE(procsModifiesVar.find("y") != procsModifiesVar.end());
        REQUIRE(varYModifiedByProc.find("Example") != varYModifiedByProc.end());
        // Modifies("1", "x")
        std::set<std::string> readModifiesVar = pkb->getModifies("1");
        std::set<std::string> varModifiedByRead = pkb->getModifiedBy("x");
        REQUIRE(readModifiesVar.find("x") != readModifiesVar.end());
        REQUIRE(varModifiedByRead.find("1") != varModifiedByRead.end());
        // Modifies(2, "y")
        std::set<std::string> assignmentsModifiesVar = pkb->getModifies("2");
        std::set<std::string> varModifiedByAssignment = pkb->getModifiedBy("y");
        REQUIRE(assignmentsModifiesVar.find("y") !=
                assignmentsModifiesVar.end());
        REQUIRE(varModifiedByAssignment.find("2") !=
                varModifiedByAssignment.end());
      }
    }
  }
}

SCENARIO("Extracting Uses relations from AST with no while/if statements") {

  GIVEN("AST with one print and one assignment statement") {
    Parser parser;
    std::queue<Token> tokenQueue;
    tokenQueue.push(Token(TokenType::Procedure, "procedure"));
    tokenQueue.push(Token(TokenType::Identifier, "Example"));
    tokenQueue.push(Token(TokenType::Separator, "{"));
    tokenQueue.push(Token(TokenType::Print, "print"));
    tokenQueue.push(Token(TokenType::Identifier, "x"));
    tokenQueue.push(Token(TokenType::Separator, ";"));
    tokenQueue.push(Token(TokenType::Identifier, "x"));
    tokenQueue.push(Token(TokenType::Separator, "="));
    tokenQueue.push(Token(TokenType::Identifier, "y"));
    tokenQueue.push(Token(TokenType::Separator, ";"));
    tokenQueue.push(Token(TokenType::Separator, "}"));
    auto programTNode = parser.buildAst(tokenQueue);
    DesignExtractor de(programTNode);

    WHEN("The PKB is retrieved") {
      std::unique_ptr<PKB> pkb = de.getPKB();

      THEN("Check if Uses relations are extracted correctly") {
        std::set<std::string> procsUsesVar = pkb->getUses("Example");
        // Uses("Example", "x")
        std::set<std::string> varXUsedByProc = pkb->getUsedBy("x");
        REQUIRE(procsUsesVar.find("x") != procsUsesVar.end());
        REQUIRE(varXUsedByProc.find("Example") != varXUsedByProc.end());
        // Uses("Example", "y")
        std::set<std::string> varYUsedByProc = pkb->getUsedBy("y");
        REQUIRE(procsUsesVar.find("y") != procsUsesVar.end());
        REQUIRE(varYUsedByProc.find("Example") != varYUsedByProc.end());
        // Uses(1, "x")
        std::set<std::string> printUsesVar = pkb->getUses("1");
        std::set<std::string> varXUsedByPrint = pkb->getUsedBy("x");
        REQUIRE(printUsesVar.find("x") != printUsesVar.end());
        REQUIRE(varXUsedByPrint.find("1") != varXUsedByPrint.end());
        // Uses(2, "y")
        std::set<std::string> assignmentsUsesVar = pkb->getUses("2");
        std::set<std::string> varYUsedByAssignment = pkb->getUsedBy("y");
        REQUIRE(assignmentsUsesVar.find("y") != assignmentsUsesVar.end());
        REQUIRE(varYUsedByAssignment.find("2") != varYUsedByAssignment.end());
      }
    }
  }
}

SCENARIO("Extracting Follows, Parent, Uses and Modifies relations from AST "
         "with one while statement") {

  GIVEN("AST with one while statement") {
    Parser parser;
    std::queue<Token> tokenQueue;
    tokenQueue.push(Token(TokenType::Procedure, "procedure"));
    tokenQueue.push(Token(TokenType::Identifier, "Example"));
    tokenQueue.push(Token(TokenType::Separator, "{"));
    tokenQueue.push(Token(TokenType::While, "while"));
    tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
    tokenQueue.push(Token(TokenType::Identifier, "x"));
    tokenQueue.push(Token(TokenType::Equal, "=="));
    tokenQueue.push(Token(TokenType::Constant, "0"));
    tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
    tokenQueue.push(Token(TokenType::Separator, "{"));
    tokenQueue.push(Token(TokenType::Read, "read"));
    tokenQueue.push(Token(TokenType::Identifier, "y"));
    tokenQueue.push(Token(TokenType::Separator, ";"));
    tokenQueue.push(Token(TokenType::Print, "print"));
    tokenQueue.push(Token(TokenType::Identifier, "z"));
    tokenQueue.push(Token(TokenType::Separator, ";"));
    tokenQueue.push(Token(TokenType::Separator, "}"));
    tokenQueue.push(Token(TokenType::Separator, "}"));
    auto programTNode = parser.buildAst(tokenQueue);
    DesignExtractor de(programTNode);

    WHEN("The PKB is retrieved") {
      std::unique_ptr<PKB> pkb = de.getPKB();

      THEN("Check if Follows, Parent, Modifies and Uses relations are "
           "extracted and derived correctly") {
        // Follow(2, 3), Follows*(2, 3)
        std::set<std::string> followsTwo = pkb->getFollows("2");
        std::set<std::string> followsTwoT = pkb->getFollowsT("2");
        REQUIRE(followsTwo.find("3") != followsTwo.end());
        REQUIRE(followsTwoT.find("3") != followsTwoT.end());
        // Parent(1, 2), Parent*(1, 2), Parent(1, 3), Parent*(1, 3)
        std::set<std::string> parentOne = pkb->getParent("1");
        std::set<std::string> parentOneT = pkb->getParentT("1");
        REQUIRE(parentOne.find("2") != parentOne.end());
        REQUIRE(parentOneT.find("2") != parentOneT.end());
        REQUIRE(parentOne.find("3") != parentOne.end());
        REQUIRE(parentOneT.find("3") != parentOneT.end());
        // Uses(1, "x"), Uses(1, "z")
        std::set<std::string> whileUsesVar = pkb->getUses("1");
        REQUIRE(whileUsesVar.find("x") != whileUsesVar.end());
        REQUIRE(whileUsesVar.find("z") != whileUsesVar.end());
        // Modifies(1, "y")
        std::set<std::string> whileModifiesVar = pkb->getModifies("1");
        REQUIRE(whileModifiesVar.find("y") != whileModifiesVar.end());
        // Modifies(2, "y")
        std::set<std::string> containerModifiesVar = pkb->getModifies("2");
        REQUIRE(containerModifiesVar.find("y") != containerModifiesVar.end());
        // Uses(3, "z")
        std::set<std::string> containerUsesVar = pkb->getUses("3");
        REQUIRE(containerUsesVar.find("z") != containerUsesVar.end());
      }
    }
  }
}

SCENARIO("Extracting Parent, Uses and Modifies relations from AST "
         "with one if statement") {

  GIVEN("AST with one if statement") {
    Parser parser;
    std::queue<Token> tokenQueue;
    tokenQueue.push(Token(TokenType::Procedure, "procedure"));
    tokenQueue.push(Token(TokenType::Identifier, "Example"));
    tokenQueue.push(Token(TokenType::Separator, "{"));
    tokenQueue.push(Token(TokenType::If, "if"));
    tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
    tokenQueue.push(Token(TokenType::Identifier, "x"));
    tokenQueue.push(Token(TokenType::Equal, "=="));
    tokenQueue.push(Token(TokenType::Constant, "0"));
    tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
    tokenQueue.push(Token(TokenType::Then, "then"));
    tokenQueue.push(Token(TokenType::Separator, "{"));
    tokenQueue.push(Token(TokenType::Read, "read"));
    tokenQueue.push(Token(TokenType::Identifier, "y"));
    tokenQueue.push(Token(TokenType::Separator, ";"));
    tokenQueue.push(Token(TokenType::Separator, "}"));
    tokenQueue.push(Token(TokenType::Else, "else"));
    tokenQueue.push(Token(TokenType::Separator, "{"));
    tokenQueue.push(Token(TokenType::Print, "print"));
    tokenQueue.push(Token(TokenType::Identifier, "z"));
    tokenQueue.push(Token(TokenType::Separator, ";"));
    tokenQueue.push(Token(TokenType::Separator, "}"));
    tokenQueue.push(Token(TokenType::Separator, "}"));
    auto programTNode = parser.buildAst(tokenQueue);
    DesignExtractor de(programTNode);

    WHEN("The PKB is retrieved") {
      std::unique_ptr<PKB> pkb = de.getPKB();

      THEN("Check if Parent, Modifies and Uses relations are "
           "extracted and derived correctly") {
        // Parent(1, 2), Parent*(1, 2), Parent(1, 3), Parent*(1, 3)
        std::set<std::string> parentOne = pkb->getParent("1");
        std::set<std::string> parentOneT = pkb->getParentT("1");
        REQUIRE(parentOne.find("2") != parentOne.end());
        REQUIRE(parentOneT.find("2") != parentOneT.end());
        REQUIRE(parentOne.find("3") != parentOne.end());
        REQUIRE(parentOneT.find("3") != parentOneT.end());
        // Uses(1, "x") and Uses(1, "z")
        std::set<std::string> ifUsesVar = pkb->getUses("1");
        REQUIRE(ifUsesVar.find("x") != ifUsesVar.end());
        REQUIRE(ifUsesVar.find("z") != ifUsesVar.end());
        // Modifies(1, "y")
        std::set<std::string> ifModifiesVar = pkb->getModifies("1");
        REQUIRE(ifModifiesVar.find("y") != ifModifiesVar.end());
        // Modifies(2, "y")
        std::set<std::string> containerModifiesVar = pkb->getModifies("2");
        REQUIRE(containerModifiesVar.find("y") != containerModifiesVar.end());
        // Uses(3, "z")
        std::set<std::string> containerUsesVar = pkb->getUses("3");
        REQUIRE(containerUsesVar.find("z") != containerUsesVar.end());
      }
    }
  }
}

SCENARIO("Extracting Parent, Uses and Modifies relations from AST with one if "
         "statement nested in one while statement") {

  GIVEN("AST with one if statement nested in one while statement") {
    Parser parser;
    std::queue<Token> tokenQueue;
    tokenQueue.push(Token(TokenType::Procedure, "procedure"));
    tokenQueue.push(Token(TokenType::Identifier, "Example"));
    tokenQueue.push(Token(TokenType::Separator, "{"));
    tokenQueue.push(Token(TokenType::While, "while"));
    tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
    tokenQueue.push(Token(TokenType::Identifier, "x"));
    tokenQueue.push(Token(TokenType::Equal, "=="));
    tokenQueue.push(Token(TokenType::Constant, "0"));
    tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
    tokenQueue.push(Token(TokenType::Separator, "{"));
    tokenQueue.push(Token(TokenType::If, "if"));
    tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
    tokenQueue.push(Token(TokenType::Identifier, "y"));
    tokenQueue.push(Token(TokenType::Equal, "=="));
    tokenQueue.push(Token(TokenType::Constant, "0"));
    tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
    tokenQueue.push(Token(TokenType::Then, "then"));
    tokenQueue.push(Token(TokenType::Separator, "{"));
    tokenQueue.push(Token(TokenType::Read, "read"));
    tokenQueue.push(Token(TokenType::Identifier, "z"));
    tokenQueue.push(Token(TokenType::Separator, ";"));
    tokenQueue.push(Token(TokenType::Separator, "}"));
    tokenQueue.push(Token(TokenType::Else, "else"));
    tokenQueue.push(Token(TokenType::Separator, "{"));
    tokenQueue.push(Token(TokenType::Print, "print"));
    tokenQueue.push(Token(TokenType::Identifier, "a"));
    tokenQueue.push(Token(TokenType::Separator, ";"));
    tokenQueue.push(Token(TokenType::Separator, "}"));
    tokenQueue.push(Token(TokenType::Separator, "}"));
    tokenQueue.push(Token(TokenType::Separator, "}"));
    auto programTNode = parser.buildAst(tokenQueue);
    DesignExtractor de(programTNode);

    WHEN("The PKB is retrieved") {
      std::unique_ptr<PKB> pkb = de.getPKB();

      THEN("Check if Parent, Modifies and Uses relations are extracted and "
           "derived correctly") {
        // Parent(1, 2), Parent*(1, 2), Parent*(1, 3), Parent*(1, 4)
        std::set<std::string> parentOne = pkb->getParent("1");
        std::set<std::string> parentOneT = pkb->getParentT("1");
        REQUIRE(parentOne.find("2") != parentOne.end());
        REQUIRE(parentOneT.find("2") != parentOneT.end());
        REQUIRE(parentOneT.find("3") != parentOneT.end());
        REQUIRE(parentOneT.find("4") != parentOneT.end());
        // Parent(2, 3), Parent(2, 4), Parent*(2, 3), Parent*(2, 4)
        std::set<std::string> parentTwo = pkb->getParent("2");
        std::set<std::string> parentTwoT = pkb->getParentT("2");
        REQUIRE(parentTwo.find("3") != parentTwo.end());
        REQUIRE(parentTwo.find("4") != parentTwo.end());
        REQUIRE(parentTwoT.find("3") != parentTwoT.end());
        REQUIRE(parentTwoT.find("4") != parentTwoT.end());
        // Uses(1, "x"), Uses(1, "y"), Uses(1, "a")
        std::set<std::string> whileUsesVar = pkb->getUses("1");
        REQUIRE(whileUsesVar.find("x") != whileUsesVar.end());
        REQUIRE(whileUsesVar.find("y") != whileUsesVar.end());
        REQUIRE(whileUsesVar.find("a") != whileUsesVar.end());
        // Modifies(1, "z")
        std::set<std::string> whileModifiesVar = pkb->getModifies("1");
        REQUIRE(whileModifiesVar.find("z") != whileModifiesVar.end());
        // Uses(2, "y"), Uses(2, "a")
        std::set<std::string> ifUsesVar = pkb->getUses("2");
        REQUIRE(ifUsesVar.find("y") != ifUsesVar.end());
        REQUIRE(ifUsesVar.find("a") != ifUsesVar.end());
        // Modifies(2, "z")
        std::set<std::string> whileContainerModifiesVar = pkb->getModifies("2");
        REQUIRE(whileContainerModifiesVar.find("z") !=
                whileContainerModifiesVar.end());
        // Modifies(3, "z")
        std::set<std::string> ifContainerModifiesVar = pkb->getModifies("3");
        REQUIRE(ifContainerModifiesVar.find("z") !=
                ifContainerModifiesVar.end());
        // Uses(4, "a")
        std::set<std::string> ifContainerUsesVar = pkb->getUses("4");
        REQUIRE(ifContainerUsesVar.find("a") != ifContainerUsesVar.end());
      }
    }
  }
}

SCENARIO("Extracting Parent, Uses and Modifies relations from AST "
         "with one while statement nested in one if statement") {

  GIVEN("AST with one while statement nested in one if statement") {
    Parser parser;
    std::queue<Token> tokenQueue;
    tokenQueue.push(Token(TokenType::Procedure, "procedure"));
    tokenQueue.push(Token(TokenType::Identifier, "Example"));
    tokenQueue.push(Token(TokenType::Separator, "{"));
    tokenQueue.push(Token(TokenType::If, "if"));
    tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
    tokenQueue.push(Token(TokenType::Identifier, "x"));
    tokenQueue.push(Token(TokenType::Equal, "=="));
    tokenQueue.push(Token(TokenType::Constant, "0"));
    tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
    tokenQueue.push(Token(TokenType::Then, "then"));
    tokenQueue.push(Token(TokenType::Separator, "{"));
    tokenQueue.push(Token(TokenType::While, "while"));
    tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
    tokenQueue.push(Token(TokenType::Identifier, "y"));
    tokenQueue.push(Token(TokenType::Equal, "=="));
    tokenQueue.push(Token(TokenType::Constant, "0"));
    tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
    tokenQueue.push(Token(TokenType::Separator, "{"));
    tokenQueue.push(Token(TokenType::Read, "read"));
    tokenQueue.push(Token(TokenType::Identifier, "z"));
    tokenQueue.push(Token(TokenType::Separator, ";"));
    tokenQueue.push(Token(TokenType::Separator, "}"));
    tokenQueue.push(Token(TokenType::Separator, "}"));
    tokenQueue.push(Token(TokenType::Else, "else"));
    tokenQueue.push(Token(TokenType::Separator, "{"));
    tokenQueue.push(Token(TokenType::Print, "print"));
    tokenQueue.push(Token(TokenType::Identifier, "a"));
    tokenQueue.push(Token(TokenType::Separator, ";"));
    tokenQueue.push(Token(TokenType::Separator, "}"));
    tokenQueue.push(Token(TokenType::Separator, "}"));
    auto programTNode = parser.buildAst(tokenQueue);
    DesignExtractor de(programTNode);

    WHEN("The PKB is retrieved") {
      std::unique_ptr<PKB> pkb = de.getPKB();

      THEN("Check if Parent, Modifies and Uses relations are extracted and "
           "derived correctly") {
        // Parent(1, 2), Parent(1, 4), Parent*(1, 2), Parent*(1, 3),
        // Parent*(1, 4)
        std::set<std::string> parentOne = pkb->getParent("1");
        std::set<std::string> parentOneT = pkb->getParentT("1");
        REQUIRE(parentOne.find("2") != parentOne.end());
        REQUIRE(parentOne.find("4") != parentOne.end());
        REQUIRE(parentOneT.find("2") != parentOneT.end());
        REQUIRE(parentOneT.find("3") != parentOneT.end());
        REQUIRE(parentOneT.find("4") != parentOneT.end());
        // Parent(2, 3), Parent*(2, 3)
        std::set<std::string> parentTwo = pkb->getParent("2");
        std::set<std::string> parentTwoT = pkb->getParentT("2");
        REQUIRE(parentTwo.find("3") != parentTwo.end());
        REQUIRE(parentTwoT.find("3") != parentTwoT.end());
        // Uses(1, "x"), Uses(1, "y"), Uses(1, "a")
        std::set<std::string> ifUsesVar = pkb->getUses("1");
        REQUIRE(ifUsesVar.find("x") != ifUsesVar.end());
        REQUIRE(ifUsesVar.find("y") != ifUsesVar.end());
        REQUIRE(ifUsesVar.find("a") != ifUsesVar.end());
        // Modifies(1, "z")
        std::set<std::string> ifModifiesVar = pkb->getModifies("1");
        REQUIRE(ifModifiesVar.find("z") != ifModifiesVar.end());
        // Uses(2, "y")
        std::set<std::string> whileUsesVar = pkb->getUses("2");
        REQUIRE(whileUsesVar.find("y") != whileUsesVar.end());
        // Modifies(2, "z")
        std::set<std::string> whileModifiesVar = pkb->getModifies("2");
        REQUIRE(whileModifiesVar.find("z") != whileModifiesVar.end());
        // Modifies(3, "z")
        std::set<std::string> whileContainerModifiesVar = pkb->getModifies("3");
        REQUIRE(whileContainerModifiesVar.find("z") !=
                whileContainerModifiesVar.end());
        // Uses(4, "a")
        std::set<std::string> ifContainerUsesVar = pkb->getUses("4");
        REQUIRE(ifContainerUsesVar.find("a") != ifContainerUsesVar.end());
      }
    }
  }
}

SCENARIO("Extracting Follows, Parent, Uses and Modifies relations from AST "
         "with one while statement nested in another while statement") {

  GIVEN("AST with one while statement nested in another while statement") {
    Parser parser;
    std::queue<Token> tokenQueue;
    tokenQueue.push(Token(TokenType::Procedure, "procedure"));
    tokenQueue.push(Token(TokenType::Identifier, "Example"));
    tokenQueue.push(Token(TokenType::Separator, "{"));
    tokenQueue.push(Token(TokenType::While, "while"));
    tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
    tokenQueue.push(Token(TokenType::Identifier, "x"));
    tokenQueue.push(Token(TokenType::Equal, "=="));
    tokenQueue.push(Token(TokenType::Constant, "0"));
    tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
    tokenQueue.push(Token(TokenType::Separator, "{"));
    tokenQueue.push(Token(TokenType::While, "while"));
    tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
    tokenQueue.push(Token(TokenType::Identifier, "y"));
    tokenQueue.push(Token(TokenType::Equal, "=="));
    tokenQueue.push(Token(TokenType::Constant, "0"));
    tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
    tokenQueue.push(Token(TokenType::Separator, "{"));
    tokenQueue.push(Token(TokenType::Read, "read"));
    tokenQueue.push(Token(TokenType::Identifier, "z"));
    tokenQueue.push(Token(TokenType::Separator, ";"));
    tokenQueue.push(Token(TokenType::Separator, "}"));
    tokenQueue.push(Token(TokenType::Print, "print"));
    tokenQueue.push(Token(TokenType::Identifier, "a"));
    tokenQueue.push(Token(TokenType::Separator, ";"));
    tokenQueue.push(Token(TokenType::Separator, "}"));
    tokenQueue.push(Token(TokenType::Separator, "}"));
    auto programTNode = parser.buildAst(tokenQueue);
    DesignExtractor de(programTNode);

    WHEN("The PKB is retrieved") {
      std::unique_ptr<PKB> pkb = de.getPKB();

      THEN("Check if Follows, Parent, Modifies and Uses relations are "
           "extracted and derived correctly") {
        // Follows(2, 4), Follows*(2, 4)
        std::set<std::string> followsTwo = pkb->getFollows("2");
        std::set<std::string> followsTwoT = pkb->getFollowsT("2");
        REQUIRE(followsTwo.find("4") != followsTwo.end());
        REQUIRE(followsTwoT.find("4") != followsTwoT.end());
        // Parent(1, 2), Parent(1, 4), Parent*(1, 2), Parent*(1, 3),
        // Parent*(1, 4)
        std::set<std::string> parentOne = pkb->getParent("1");
        std::set<std::string> parentOneT = pkb->getParentT("1");
        REQUIRE(parentOne.find("2") != parentOne.end());
        REQUIRE(parentOne.find("4") != parentOne.end());
        REQUIRE(parentOneT.find("2") != parentOneT.end());
        REQUIRE(parentOneT.find("3") != parentOneT.end());
        REQUIRE(parentOneT.find("4") != parentOneT.end());
        // Parent(2, 3), Parent*(2, 3)
        std::set<std::string> parentTwo = pkb->getParent("2");
        std::set<std::string> parentTwoT = pkb->getParentT("2");
        REQUIRE(parentTwo.find("3") != parentTwo.end());
        REQUIRE(parentTwoT.find("3") != parentTwoT.end());
        // Uses(1, "x"), Uses(1, "y"), Uses(1, "a")
        std::set<std::string> outerWhileUsesVar = pkb->getUses("1");
        REQUIRE(outerWhileUsesVar.find("x") != outerWhileUsesVar.end());
        REQUIRE(outerWhileUsesVar.find("y") != outerWhileUsesVar.end());
        REQUIRE(outerWhileUsesVar.find("a") != outerWhileUsesVar.end());
        // Modifies(1, "z")
        std::set<std::string> outerWhileModifiesVar = pkb->getModifies("1");
        REQUIRE(outerWhileModifiesVar.find("z") != outerWhileModifiesVar.end());
        // Uses(2, "y")
        std::set<std::string> innerWhileUsesVar = pkb->getUses("2");
        REQUIRE(innerWhileUsesVar.find("y") != innerWhileUsesVar.end());
        // Modifies(2, "z")
        std::set<std::string> innerWhileModifiesVar = pkb->getModifies("2");
        REQUIRE(innerWhileModifiesVar.find("z") != innerWhileModifiesVar.end());
        // Modifies(3, "z")
        std::set<std::string> innerWhileContainerModifiesVar =
            pkb->getModifies("3");
        REQUIRE(innerWhileContainerModifiesVar.find("z") !=
                innerWhileContainerModifiesVar.end());
        // Uses(4, "a")
        std::set<std::string> outerWhileContainerUsesVar = pkb->getUses("4");
        REQUIRE(outerWhileContainerUsesVar.find("a") !=
                outerWhileContainerUsesVar.end());
      }
    }
  }
}

SCENARIO("Extracting Parent, Uses and Modifies relations from AST with one if "
         "statement nested in another if statement") {

  GIVEN("AST with one if statement nested in another if statement") {
    Parser parser;
    std::queue<Token> tokenQueue;
    tokenQueue.push(Token(TokenType::Procedure, "procedure"));
    tokenQueue.push(Token(TokenType::Identifier, "Example"));
    tokenQueue.push(Token(TokenType::Separator, "{"));
    tokenQueue.push(Token(TokenType::If, "if"));
    tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
    tokenQueue.push(Token(TokenType::Identifier, "x"));
    tokenQueue.push(Token(TokenType::Equal, "=="));
    tokenQueue.push(Token(TokenType::Constant, "0"));
    tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
    tokenQueue.push(Token(TokenType::Then, "then"));
    tokenQueue.push(Token(TokenType::Separator, "{"));
    tokenQueue.push(Token(TokenType::If, "if"));
    tokenQueue.push(Token(TokenType::OpenParenthesis, "("));
    tokenQueue.push(Token(TokenType::Identifier, "y"));
    tokenQueue.push(Token(TokenType::Equal, "=="));
    tokenQueue.push(Token(TokenType::Constant, "0"));
    tokenQueue.push(Token(TokenType::CloseParenthesis, ")"));
    tokenQueue.push(Token(TokenType::Then, "then"));
    tokenQueue.push(Token(TokenType::Separator, "{"));
    tokenQueue.push(Token(TokenType::Read, "read"));
    tokenQueue.push(Token(TokenType::Identifier, "z"));
    tokenQueue.push(Token(TokenType::Separator, ";"));
    tokenQueue.push(Token(TokenType::Separator, "}"));
    tokenQueue.push(Token(TokenType::Else, "else"));
    tokenQueue.push(Token(TokenType::Separator, "{"));
    tokenQueue.push(Token(TokenType::Print, "print"));
    tokenQueue.push(Token(TokenType::Identifier, "a"));
    tokenQueue.push(Token(TokenType::Separator, ";"));
    tokenQueue.push(Token(TokenType::Separator, "}"));
    tokenQueue.push(Token(TokenType::Separator, "}"));
    tokenQueue.push(Token(TokenType::Else, "else"));
    tokenQueue.push(Token(TokenType::Separator, "{"));
    tokenQueue.push(Token(TokenType::Print, "print"));
    tokenQueue.push(Token(TokenType::Identifier, "b"));
    tokenQueue.push(Token(TokenType::Separator, ";"));
    tokenQueue.push(Token(TokenType::Separator, "}"));
    tokenQueue.push(Token(TokenType::Separator, "}"));
    auto programTNode = parser.buildAst(tokenQueue);
    DesignExtractor de(programTNode);

    WHEN("The PKB is retrieved") {
      std::unique_ptr<PKB> pkb = de.getPKB();

      THEN("Check if Parent, Modifies and Uses relations are extracted and "
           "derived correctly") {
        // Parent(1, 2), Parent(1, 5), Parent*(1, 2), Parent*(1, 3),
        // Parent*(1, 4)
        std::set<std::string> parentOne = pkb->getParent("1");
        std::set<std::string> parentOneT = pkb->getParentT("1");
        REQUIRE(parentOne.find("2") != parentOne.end());
        REQUIRE(parentOne.find("5") != parentOne.end());
        REQUIRE(parentOneT.find("2") != parentOneT.end());
        REQUIRE(parentOneT.find("3") != parentOneT.end());
        REQUIRE(parentOneT.find("4") != parentOneT.end());
        // Parent(2, 3), Parent(2, 4), Parent*(2, 3), Parent*(2, 4)
        std::set<std::string> parentTwo = pkb->getParent("2");
        std::set<std::string> parentTwoT = pkb->getParentT("2");
        REQUIRE(parentTwo.find("3") != parentTwo.end());
        REQUIRE(parentTwo.find("4") != parentTwo.end());
        REQUIRE(parentTwoT.find("3") != parentTwoT.end());
        REQUIRE(parentTwoT.find("4") != parentTwoT.end());
        // Uses(1, "x"), Uses(1, "y"), Uses(1, "a"), Uses(1, "b")
        std::set<std::string> outerIfUsesVar = pkb->getUses("1");
        REQUIRE(outerIfUsesVar.find("x") != outerIfUsesVar.end());
        REQUIRE(outerIfUsesVar.find("y") != outerIfUsesVar.end());
        REQUIRE(outerIfUsesVar.find("a") != outerIfUsesVar.end());
        REQUIRE(outerIfUsesVar.find("b") != outerIfUsesVar.end());
        // Modifies(1, "z")
        std::set<std::string> outerIfModifiesVar = pkb->getModifies("1");
        REQUIRE(outerIfModifiesVar.find("z") != outerIfModifiesVar.end());
        // Uses(2, "y"), Uses(2, "a")
        std::set<std::string> innerIfUsesVar = pkb->getUses("2");
        REQUIRE(innerIfUsesVar.find("y") != innerIfUsesVar.end());
        REQUIRE(innerIfUsesVar.find("a") != innerIfUsesVar.end());
        // Modifies(2, "z")
        std::set<std::string> innerIfModifiesVar = pkb->getModifies("2");
        REQUIRE(innerIfModifiesVar.find("z") != innerIfModifiesVar.end());
        // Modifies(3, "z")
        std::set<std::string> innerIfContainerModifiesVar =
            pkb->getModifies("3");
        REQUIRE(innerIfContainerModifiesVar.find("z") !=
                innerIfContainerModifiesVar.end());
        // Uses(4, "a")
        std::set<std::string> innerIfContainerUsesVar = pkb->getUses("4");
        REQUIRE(innerIfContainerUsesVar.find("a") !=
                innerIfContainerUsesVar.end());
        // Uses(5, "b")
        std::set<std::string> outerIfContainerUsesVar = pkb->getUses("5");
        REQUIRE(outerIfContainerUsesVar.find("b") !=
                outerIfContainerUsesVar.end());
      }
    }
  }
}
