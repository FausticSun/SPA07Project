//#include "PQLLexer.h"
////#include "Relation.h"
//#include <algorithm>
//#include <queue>
//#include <string.h>
//#include <string>
//#include <vector>
//
//// modifies and uses(left emplty), pattern a is not a actual assignemnt, type,
//// how to check errors
//using namespace std;
//bool BothAreSpaces(char lhs, char rhs) { return (lhs == rhs) && (lhs == ' '); }
//string standerize(string str) {
//  string new_str;
//  for (int i = 0; i < str.length(); i++) {
//    if (str[i] == '\t' || str[i] == '\n') {
//      str[i] = ' ';
//    }
//  }
//  for (int i = 0; i < str.length() - 5; i++) {
//	  if (str.substr(i, 4) == "with") {
//		  if (str[i + 4] != ' ') {
//			  throw("wrong format with clause");
//		  }
//	  }
//  }
//  std::string::iterator new_end =
//      std::unique(str.begin(), str.end(), BothAreSpaces);
//  str.erase(new_end, str.end());
//  if (str.front() == ' ')
//    str = str.substr(1, str.size());
//  if (str.back() == ' ')
//    str = str.substr(0, str.size() - 1);
//  for (std::string::size_type i = 0; i < str.size(); ++i) {
//    if (str[i] == ',') { // remove space in front and add space at back
//      if (new_str.back() != string::npos && new_str.back() == ' ') {
//        new_str = new_str.substr(0, new_str.size() - 1);
//        new_str = new_str + str[i];
//      } else {
//        new_str = new_str + str[i];
//      }
//      if (str[i + 1] != string::npos && str[i + 1] != ' ') {
//        new_str = new_str + ' ';
//      }
//    } else if (str[i] == ';') {
//      if (new_str.back() != string::npos && new_str.back() == ' ') {
//        new_str = new_str.substr(0, new_str.size() - 1);
//        new_str = new_str + str[i];
//      } else {
//        new_str = new_str + str[i];
//      }
//      if (str[i + 1] != string::npos && str[i + 1] != ' ') {
//        new_str = new_str + ' ';
//      }
//    } else if (str[i] == ')') {
//      if (new_str.back() != string::npos && new_str.back() == ' ') {
//        new_str = new_str.substr(0, new_str.size() - 1);
//        new_str = new_str + str[i];
//      } else {
//        new_str = new_str + str[i];
//      }
//      if (str[i + 1] != string::npos && str[i + 1] != ' ') {
//        new_str = new_str + ' ';
//      }
//    } else if (str[i] == '\"') {
//      if (new_str[new_str.size() - 1] != string::npos &&
//          new_str[new_str.size() - 2] != string::npos &&
//          new_str[new_str.size() - 1] == ' ' &&
//          new_str[new_str.size() - 2] != ',') {
//        new_str = new_str.substr(0, new_str.size() - 1);
//        new_str = new_str + str[i];
//      } else {
//        new_str = new_str + str[i];
//      }
//    } else if (str[i] == '(') {
//      if (new_str.back() != string::npos && new_str.back() != ' ') {
//        new_str = new_str + ' ';
//        new_str = new_str + str[i];
//      } else {
//        new_str = new_str + str[i];
//      }
//    } else if (str[i] == ')' || str[i] == '+' || str[i] == '-' ||
//               str[i] == '*' || str[i] == '/' ||
//               str[i] == '%') { // remove space in front
//      if (new_str.back() != string::npos && new_str.back() == ' ') {
//        new_str = new_str.substr(0, new_str.size() - 1);
//        new_str = new_str + str[i];
//      } else {
//        new_str = new_str + str[i];
//      }
//    } else if (str[i] == ' ') { // remove space at back
//      if (new_str.back() != string::npos &&
//          (new_str.back() == '(' || new_str.back() == '+' ||
//           new_str.back() == '-' || new_str.back() == '*' ||
//           new_str.back() == '/' || new_str.back() == '%' ||
//           new_str.back() == '\"')) {
//
//      } else {
//        new_str = new_str + str[i];
//      }
//    } else {
//      new_str = new_str + str[i];
//    }
//  }
//  return new_str;
//}
//PQLLexer::PQLLexer(string input) {
//  string standard_input = standerize(input);
//  Tokenize(standard_input);
//  selectEnd = false;
//}
//
//vector<string> PQLLexer::vectorize(string input) {
//  vector<string> tokens;
//  char *p;
//  char *temp = (char *)input.c_str();
//  p = strtok(temp, " ");
//  while (p) {
//    tokens.push_back(p);
//    p = strtok(NULL, " ");
//  }
//  return tokens;
//}
//
//void PQLLexer::Tokenize(string input) {
//  vector<string> token = vectorize(input);
//  string last_keyword;
//  while (!token.empty()) {
//    if (token[0] == "procedure") {
//      token = tokenizeProcedure(token);
//
//    } else if (token[0] == "variable") {
//      token = tokenizeVariable(token);
//
//    } else if (token[0] == "stmt") {
//      token = tokenizeStmt(token);
//
//    } else if (token[0] == "read") {
//      token = tokenizeRead(token);
//
//    } else if (token[0] == "print") {
//      token = tokenizePrint(token);
//
//    } else if (token[0] == "while") {
//      token = tokenizeWhile(token);
//
//    } else if (token[0] == "if") {
//      token = tokenizeIf(token);
//
//    } else if (token[0] == "assign") {
//      token = tokenizeAssign(token);
//
//    } else if (token[0] == "constant") {
//      token = tokenizeConstant(token);
//    } else if (token[0] == "prog_line") {
//      token = tokenizeProgLine(token);
//    } else if (token[0] == "call") {
//      token = tokenizeCall(token);
//    } else
//        // if (find(token.begin(), token.end(), "select") != token.end()) {
//		if (!token.empty() && token[0] == "Select") {
//			token = tokenizeSelect(token);
//			// deal with "," and ";". If ";", go to the outer loop.
//			// token.erase(token.begin());
//			// token.erase(token.begin());
//			while (!token.empty()) {
//				if (token[0] == "such" && token[1] == "that") {
//					last_keyword = "such that";
//					tokenQueue.push(make_pair(TokenType::Keyword, "such that"));
//					token.erase(token.begin());
//					token.erase(token.begin());
//					if (token[0] == "Modifies" ||
//						(token[0].find("Modifies") != token[0].npos &&
//							token[0][8] == '(')) {
//						token = tokenizeModifies(token);
//					}
//					else if (token[0] == "Uses" ||
//						(token[0].find("Uses") != token[0].npos &&
//							token[0][4] == '(')) {
//						token = tokenizeUses(token);
//
//					}
//					else if (token[0] == "Parent*" ||
//						(token[0].find("Parent*") != token[0].npos &&
//							token[0][7] == '(')) {
//						token = tokenizeParentT(token);
//
//					}
//					else if (token[0] == "Parent" ||
//						(token[0].find("Parent") != token[0].npos &&
//							token[0][6] == '(')) {
//						token = tokenizeParent(token);
//					}
//					else if (token[0] == "Follows*" ||
//						(token[0].find("Follows*") != token[0].npos &&
//							token[0][8] == '(')) {
//						token = tokenizeFollowsT(token);
//					}
//					else if (token[0] == "Follows" ||
//						(token[0].find("Follows") != token[0].npos &&
//							token[0][7] == '(')) {
//						token = tokenizeFollows(token);
//					}
//					else if (token[0] == "Next") {
//						token = tokenizeNext(token);
//					}
//					else if (token[0] == "Next*") {
//						token = tokenizeNextT(token);
//					}
//					else if (token[0] == "Calls") {
//						token = tokenizeCalls(token);
//					}
//					else if (token[0] == "Calls*") {
//						token = tokenizeCallsT(token);
//					}
//					else {
//						throw("wrong format of selection!");
//					}
//
//				}
//				else if (token[0] == "pattern" ||
//					(token[0].find("pattern") != token[0].npos &&
//						token[0][7] == '(')) {
//					last_keyword = "pattern";
//					token = tokenizePattern(token);
//				}
//				else if (token[0].find("with") != token[0].npos) {
//					last_keyword = "with";
//					token = tokenizeWith(token);
//				}
//				else if (token[0] == "and") {
//					token.erase(token.begin());
//					tokenQueue.push(make_pair(TokenType::Keyword, "and"));
//					if (last_keyword == "such that") {
//						if (token[0] == "Modifies" ||
//							(token[0].find("Modifies") != token[0].npos &&
//								token[0][8] == '(')) {
//							token = tokenizeModifies(token);
//						}
//						else if (token[0] == "Uses" ||
//							(token[0].find("Uses") != token[0].npos &&
//								token[0][4] == '(')) {
//							token = tokenizeUses(token);
//
//						}
//						else if (token[0] == "Parent*" ||
//							(token[0].find("Parent*") != token[0].npos &&
//								token[0][7] == '(')) {
//							token = tokenizeParentT(token);
//
//						}
//						else if (token[0] == "Parent" ||
//							(token[0].find("Parent") != token[0].npos &&
//								token[0][6] == '(')) {
//							token = tokenizeParent(token);
//						}
//						else if (token[0] == "Follows*" ||
//							(token[0].find("Follows*") != token[0].npos &&
//								token[0][8] == '(')) {
//							token = tokenizeFollowsT(token);
//						}
//						else if (token[0] == "Follows" ||
//							(token[0].find("Follows") != token[0].npos &&
//								token[0][7] == '(')) {
//							token = tokenizeFollows(token);
//						}
//						else if (token[0] == "Next") {
//							token = tokenizeNext(token);
//						}
//						else if (token[0] == "Next*") {
//							token = tokenizeNextT(token);
//						}
//						else if (token[0] == "Calls") {
//							token = tokenizeCalls(token);
//						}
//						else if (token[0] == "Calls*") {
//							token = tokenizeCallsT(token);
//						}
//						else if (token[0] == "pattern" ||
//							(token[0].find("pattern") != token[0].npos &&
//								token[0][7] == '(')) {
//							last_keyword = "pattern";
//							token = tokenizePattern(token);
//						}
//					
//						else {
//							throw("wrong format of selection!");
//						}
//					}
//					else if (last_keyword == "pattern") {
//						token = tokenizePattern(token);
//					}
//					else if (last_keyword == "with") {
//						token = tokenizeWith(token);
//					}
//
//				}
//			}
//		}
//		else {
//			throw invalid_argument("wrong keyword");
//		}
//  }
//}
//
//bool isInt(string s) {
//  if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+')))
//    return false;
//
//  char *p;
//  strtol(s.c_str(), &p, 10);
//
//  return (*p == 0);
//}
//
//queue<pair<TokenType, string>> PQLLexer::getTokenQueue() { return tokenQueue; }
//
//queue<string> PQLLexer::getTarget() { return target; }
//
//string PQLLexer::SplitCommas(string s) {
//  s = s.substr(0, s.length() - 1);
//  return s;
//}
//
//bool PQLLexer::existSemi(string s) {
//  int check = 0;
//  for (int i = 0; i < s.length(); i++) {
//    if (s[i] == ';') {
//      check = 1;
//      break;
//    }
//  }
//  if (check == 0) {
//    return false;
//  } else {
//    return true;
//  }
//}
//
//vector<string> PQLLexer::tokenizeCall(vector<string> token) {
//  tokenQueue.push(make_pair(TokenType::Keyword, "call"));
//  token.erase(token.begin());
//  bool end = false;
//  while (!end) {
//    if (token[0].find(",") != token[0].npos) // first situation
//    {
//      while (!token.empty() && token[0].find(',') != token[0].npos) {
//        for (int i = 0; i < token[0].length(); i++) {
//          if (token[0][i] == ',') {
//            if (token[0].length() == 1) {
//              // tokenQueue.push(make_pair(TokenType::Separator, ","));
//              // token.erase(token.begin());
//            } else if (i == token[0].length() - 1) {
//
//              tokenQueue.push(
//                  make_pair(TokenType::Identifier,
//                            token[0].substr(0, token[0].length() - 1)));
//              tokenQueue.push(make_pair(TokenType::Separator, ","));
//              token.erase(token.begin());
//              break;
//            } else {
//              // tokenQueue.push(make_pair(TokenType::Identifier,
//              // token[0].substr(0, i)));
//              // tokenQueue.push(make_pair(TokenType::Separator, ","));
//              // token[0] = token[0].substr(i + 1, token[0].length() - i - 1);
//              // break;
//            }
//          }
//        }
//      }
//    } else if (existSemi(token[0])) // second situation
//    {
//      for (int i = 0; i < token[0].length(); i++) {
//        if (token[0][i] == ';') {
//          if (token[0].length() == 1) {
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token.erase(token.begin());
//          } else if (i == token[0].length() - 1) {
//
//            tokenQueue.push(
//                make_pair(TokenType::Identifier, token[0].substr(0, i)));
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token.erase(token.begin());
//          } else {
//            if (i == 0) {
//
//            } else {
//              tokenQueue.push(
//                  make_pair(TokenType::Identifier, token[0].substr(0, i)));
//            }
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token[0] = token[0].substr(i + 1, token[0].length() - i - 1);
//          }
//        }
//      }
//
//      end = true;
//    } else {
//      throw("missing semicollumn in declaration!");
//    }
//  }
//  if (end) {
//    expectionOfDeclaration(token);
//  }
//
//  return token;
//}
//
//vector<string> PQLLexer::tokenizeVariable(vector<string> token) {
//  tokenQueue.push(make_pair(TokenType::Keyword, "variable"));
//  token.erase(token.begin());
//  bool end = false;
//  while (!end) {
//    if (token[0].find(",") != token[0].npos) // first situation
//    {
//      while (!token.empty() && token[0].find(',') != token[0].npos) {
//        for (int i = 0; i < token[0].length(); i++) {
//          if (token[0][i] == ',') {
//            if (token[0].length() == 1) {
//              // tokenQueue.push(make_pair(TokenType::Separator, ","));
//              // token.erase(token.begin());
//            } else if (i == token[0].length() - 1) {
//
//              tokenQueue.push(
//                  make_pair(TokenType::Identifier,
//                            token[0].substr(0, token[0].length() - 1)));
//              tokenQueue.push(make_pair(TokenType::Separator, ","));
//              token.erase(token.begin());
//              break;
//            } else {
//              // tokenQueue.push(make_pair(TokenType::Identifier,
//              // token[0].substr(0, i)));
//              // tokenQueue.push(make_pair(TokenType::Separator, ","));
//              // token[0] = token[0].substr(i + 1, token[0].length() - i - 1);
//              // break;
//            }
//          }
//        }
//      }
//    } else if (existSemi(token[0])) // second situation
//    {
//      for (int i = 0; i < token[0].length(); i++) {
//        if (token[0][i] == ';') {
//          if (token[0].length() == 1) {
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token.erase(token.begin());
//          } else if (i == token[0].length() - 1) {
//
//            tokenQueue.push(
//                make_pair(TokenType::Identifier, token[0].substr(0, i)));
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token.erase(token.begin());
//          } else {
//            if (i == 0) {
//
//            } else {
//              tokenQueue.push(
//                  make_pair(TokenType::Identifier, token[0].substr(0, i)));
//            }
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token[0] = token[0].substr(i + 1, token[0].length() - i - 1);
//          }
//        }
//      }
//
//      end = true;
//    } else {
//      throw("missing semicollumn in declaration!");
//    }
//  }
//  if (end) {
//    expectionOfDeclaration(token);
//  }
//
//  return token;
//}
//
//vector<string> PQLLexer::tokenizeProcedure(vector<string> token) {
//  tokenQueue.push(make_pair(TokenType::Keyword, "procedure"));
//  token.erase(token.begin());
//  bool end = false;
//  while (!end) {
//    if (token[0].find(",") != token[0].npos) // first situation
//    {
//      while (!token.empty() && token[0].find(',') != token[0].npos) {
//        for (int i = 0; i < token[0].length(); i++) {
//          if (token[0][i] == ',') {
//            if (token[0].length() == 1) {
//              // tokenQueue.push(make_pair(TokenType::Separator, ","));
//              // token.erase(token.begin());
//            } else if (i == token[0].length() - 1) {
//
//              tokenQueue.push(
//                  make_pair(TokenType::Identifier,
//                            token[0].substr(0, token[0].length() - 1)));
//              tokenQueue.push(make_pair(TokenType::Separator, ","));
//              token.erase(token.begin());
//              break;
//            } else {
//              // tokenQueue.push(make_pair(TokenType::Identifier,
//              // token[0].substr(0, i)));
//              // tokenQueue.push(make_pair(TokenType::Separator, ","));
//              // token[0] = token[0].substr(i + 1, token[0].length() - i - 1);
//              // break;
//            }
//          }
//        }
//      }
//    } else if (existSemi(token[0])) // second situation
//    {
//      for (int i = 0; i < token[0].length(); i++) {
//        if (token[0][i] == ';') {
//          if (token[0].length() == 1) {
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token.erase(token.begin());
//          } else if (i == token[0].length() - 1) {
//
//            tokenQueue.push(
//                make_pair(TokenType::Identifier, token[0].substr(0, i)));
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token.erase(token.begin());
//          } else {
//            if (i == 0) {
//
//            } else {
//              tokenQueue.push(
//                  make_pair(TokenType::Identifier, token[0].substr(0, i)));
//            }
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token[0] = token[0].substr(i + 1, token[0].length() - i - 1);
//          }
//        }
//      }
//
//      end = true;
//    } else {
//      throw("missing semicollumn in declaration!");
//    }
//  }
//  if (end) {
//    expectionOfDeclaration(token);
//  }
//
//  return token;
//}
//
//vector<string> PQLLexer::tokenizeProgLine(vector<string> token) {
//  tokenQueue.push(make_pair(TokenType::Keyword, "prog_line"));
//  token.erase(token.begin());
//  bool end = false;
//  while (!end) {
//    if (token[0].find(",") != token[0].npos) // first situation
//    {
//      while (!token.empty() && token[0].find(',') != token[0].npos) {
//        for (int i = 0; i < token[0].length(); i++) {
//          if (token[0][i] == ',') {
//            if (token[0].length() == 1) {
//              // tokenQueue.push(make_pair(TokenType::Separator, ","));
//              // token.erase(token.begin());
//            } else if (i == token[0].length() - 1) {
//
//              tokenQueue.push(
//                  make_pair(TokenType::Identifier,
//                            token[0].substr(0, token[0].length() - 1)));
//              tokenQueue.push(make_pair(TokenType::Separator, ","));
//              token.erase(token.begin());
//              break;
//            } else {
//              // tokenQueue.push(make_pair(TokenType::Identifier,
//              // token[0].substr(0, i)));
//              // tokenQueue.push(make_pair(TokenType::Separator, ","));
//              // token[0] = token[0].substr(i + 1, token[0].length() - i - 1);
//              // break;
//            }
//          }
//        }
//      }
//    } else if (existSemi(token[0])) // second situation
//    {
//      for (int i = 0; i < token[0].length(); i++) {
//        if (token[0][i] == ';') {
//          if (token[0].length() == 1) {
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token.erase(token.begin());
//          } else if (i == token[0].length() - 1) {
//
//            tokenQueue.push(
//                make_pair(TokenType::Identifier, token[0].substr(0, i)));
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token.erase(token.begin());
//          } else {
//            if (i == 0) {
//
//            } else {
//              tokenQueue.push(
//                  make_pair(TokenType::Identifier, token[0].substr(0, i)));
//            }
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token[0] = token[0].substr(i + 1, token[0].length() - i - 1);
//          }
//        }
//      }
//
//      end = true;
//    } else {
//      throw("missing semicollumn in declaration!");
//    }
//  }
//  if (end) {
//    expectionOfDeclaration(token);
//  }
//
//  return token;
//}
//
//vector<string> PQLLexer::tokenizeRead(vector<string> token) {
//  tokenQueue.push(make_pair(TokenType::Keyword, "read"));
//  token.erase(token.begin());
//  bool end = false;
//  while (!end) {
//    if (token[0].find(",") != token[0].npos) // first situation
//    {
//      while (!token.empty() && token[0].find(',') != token[0].npos) {
//        for (int i = 0; i < token[0].length(); i++) {
//          if (token[0][i] == ',') {
//            if (token[0].length() == 1) {
//              // tokenQueue.push(make_pair(TokenType::Separator, ","));
//              // token.erase(token.begin());
//            } else if (i == token[0].length() - 1) {
//
//              tokenQueue.push(
//                  make_pair(TokenType::Identifier,
//                            token[0].substr(0, token[0].length() - 1)));
//              tokenQueue.push(make_pair(TokenType::Separator, ","));
//              token.erase(token.begin());
//              break;
//            } else {
//              // tokenQueue.push(make_pair(TokenType::Identifier,
//              // token[0].substr(0, i)));
//              // tokenQueue.push(make_pair(TokenType::Separator, ","));
//              // token[0] = token[0].substr(i + 1, token[0].length() - i - 1);
//              // break;
//            }
//          }
//        }
//      }
//    } else if (existSemi(token[0])) // second situation
//    {
//      for (int i = 0; i < token[0].length(); i++) {
//        if (token[0][i] == ';') {
//          if (token[0].length() == 1) {
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token.erase(token.begin());
//          } else if (i == token[0].length() - 1) {
//
//            tokenQueue.push(
//                make_pair(TokenType::Identifier, token[0].substr(0, i)));
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token.erase(token.begin());
//          } else {
//            if (i == 0) {
//
//            } else {
//              tokenQueue.push(
//                  make_pair(TokenType::Identifier, token[0].substr(0, i)));
//            }
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token[0] = token[0].substr(i + 1, token[0].length() - i - 1);
//          }
//        }
//      }
//
//      end = true;
//    } else {
//      throw("missing semicollumn in declaration!");
//    }
//  }
//  if (end) {
//    expectionOfDeclaration(token);
//  }
//
//  return token;
//}
//
//vector<string> PQLLexer::tokenizePrint(vector<string> token) {
//  tokenQueue.push(make_pair(TokenType::Keyword, "print"));
//  token.erase(token.begin());
//  bool end = false;
//  while (!end) {
//    if (token[0].find(",") != token[0].npos) // first situation
//    {
//      while (!token.empty() && token[0].find(',') != token[0].npos) {
//        for (int i = 0; i < token[0].length(); i++) {
//          if (token[0][i] == ',') {
//            if (token[0].length() == 1) {
//              // tokenQueue.push(make_pair(TokenType::Separator, ","));
//              // token.erase(token.begin());
//            } else if (i == token[0].length() - 1) {
//
//              tokenQueue.push(
//                  make_pair(TokenType::Identifier,
//                            token[0].substr(0, token[0].length() - 1)));
//              tokenQueue.push(make_pair(TokenType::Separator, ","));
//              token.erase(token.begin());
//              break;
//            } else {
//              // tokenQueue.push(make_pair(TokenType::Identifier,
//              // token[0].substr(0, i)));
//              // tokenQueue.push(make_pair(TokenType::Separator, ","));
//              // token[0] = token[0].substr(i + 1, token[0].length() - i - 1);
//              // break;
//            }
//          }
//        }
//      }
//    } else if (existSemi(token[0])) // second situation
//    {
//      for (int i = 0; i < token[0].length(); i++) {
//        if (token[0][i] == ';') {
//          if (token[0].length() == 1) {
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token.erase(token.begin());
//          } else if (i == token[0].length() - 1) {
//
//            tokenQueue.push(
//                make_pair(TokenType::Identifier, token[0].substr(0, i)));
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token.erase(token.begin());
//          } else {
//            if (i == 0) {
//
//            } else {
//              tokenQueue.push(
//                  make_pair(TokenType::Identifier, token[0].substr(0, i)));
//            }
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token[0] = token[0].substr(i + 1, token[0].length() - i - 1);
//          }
//        }
//      }
//
//      end = true;
//    } else {
//      throw("missing semicollumn in declaration!");
//    }
//  }
//  if (end) {
//    expectionOfDeclaration(token);
//  }
//
//  return token;
//}
//
//vector<string> PQLLexer::tokenizeWhile(vector<string> token) {
//  tokenQueue.push(make_pair(TokenType::Keyword, "while"));
//  token.erase(token.begin());
//  bool end = false;
//  while (!end) {
//    if (token[0].find(",") != token[0].npos) // first situation
//    {
//      while (!token.empty() && token[0].find(',') != token[0].npos) {
//        for (int i = 0; i < token[0].length(); i++) {
//          if (token[0][i] == ',') {
//            if (token[0].length() == 1) {
//              // tokenQueue.push(make_pair(TokenType::Separator, ","));
//              // token.erase(token.begin());
//            } else if (i == token[0].length() - 1) {
//
//              tokenQueue.push(
//                  make_pair(TokenType::Identifier,
//                            token[0].substr(0, token[0].length() - 1)));
//              tokenQueue.push(make_pair(TokenType::Separator, ","));
//              token.erase(token.begin());
//              break;
//            } else {
//              // tokenQueue.push(make_pair(TokenType::Identifier,
//              // token[0].substr(0, i)));
//              // tokenQueue.push(make_pair(TokenType::Separator, ","));
//              // token[0] = token[0].substr(i + 1, token[0].length() - i - 1);
//              // break;
//            }
//          }
//        }
//      }
//    } else if (existSemi(token[0])) // second situation
//    {
//      for (int i = 0; i < token[0].length(); i++) {
//        if (token[0][i] == ';') {
//          if (token[0].length() == 1) {
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token.erase(token.begin());
//            break;
//          } else if (i == token[0].length() - 1) {
//
//            tokenQueue.push(
//                make_pair(TokenType::Identifier, token[0].substr(0, i)));
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token.erase(token.begin());
//            break;
//          } else {
//            if (i == 0) {
//
//            } else {
//              tokenQueue.push(
//                  make_pair(TokenType::Identifier, token[0].substr(0, i)));
//            }
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token[0] = token[0].substr(i + 1, token[0].length() - i - 1);
//            break;
//          }
//        }
//      }
//
//      end = true;
//    } else {
//      throw("missing semicollumn in declaration!");
//    }
//  }
//  if (end) {
//    expectionOfDeclaration(token);
//  }
//
//  return token;
//}
//
//vector<string> PQLLexer::tokenizeIf(vector<string> token) {
//  tokenQueue.push(make_pair(TokenType::Keyword, "if"));
//  token.erase(token.begin());
//  bool end = false;
//  while (!end) {
//    if (token[0].find(",") != token[0].npos) // first situation
//    {
//      while (!token.empty() && token[0].find(',') != token[0].npos) {
//        for (int i = 0; i < token[0].length(); i++) {
//          if (token[0][i] == ',') {
//            if (token[0].length() == 1) {
//              // tokenQueue.push(make_pair(TokenType::Separator, ","));
//              // token.erase(token.begin());
//            } else if (i == token[0].length() - 1) {
//
//              tokenQueue.push(
//                  make_pair(TokenType::Identifier,
//                            token[0].substr(0, token[0].length() - 1)));
//              tokenQueue.push(make_pair(TokenType::Separator, ","));
//              token.erase(token.begin());
//              break;
//            } else {
//              // tokenQueue.push(make_pair(TokenType::Identifier,
//              // token[0].substr(0, i)));
//              // tokenQueue.push(make_pair(TokenType::Separator, ","));
//              // token[0] = token[0].substr(i + 1, token[0].length() - i - 1);
//              // break;
//            }
//          }
//        }
//      }
//    } else if (existSemi(token[0])) // second situation
//    {
//      for (int i = 0; i < token[0].length(); i++) {
//        if (token[0][i] == ';') {
//          if (token[0].length() == 1) {
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token.erase(token.begin());
//          } else if (i == token[0].length() - 1) {
//
//            tokenQueue.push(
//                make_pair(TokenType::Identifier, token[0].substr(0, i)));
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token.erase(token.begin());
//          } else {
//            if (i == 0) {
//
//            } else {
//              tokenQueue.push(
//                  make_pair(TokenType::Identifier, token[0].substr(0, i)));
//            }
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token[0] = token[0].substr(i + 1, token[0].length() - i - 1);
//          }
//        }
//      }
//
//      end = true;
//    } else {
//      throw("missing semicollumn in declaration!");
//    }
//  }
//  if (end) {
//expectionOfDeclaration(token);
//  }
//
//  return token;
//}
//
//vector<string> PQLLexer::tokenizeAssign(vector<string> token) {
//  tokenQueue.push(make_pair(TokenType::Keyword, "assign"));
//  token.erase(token.begin());
//  bool end = false;
//  while (!end) {
//    if (token[0].find(",") != token[0].npos) // first situation
//    {
//      while (!token.empty() && token[0].find(',') != token[0].npos) {
//        for (int i = 0; i < token[0].length(); i++) {
//          if (token[0][i] == ',') {
//            if (token[0].length() == 1) {
//              // tokenQueue.push(make_pair(TokenType::Separator, ","));
//              // token.erase(token.begin());
//            } else if (i == token[0].length() - 1) {
//
//              tokenQueue.push(
//                  make_pair(TokenType::Identifier,
//                            token[0].substr(0, token[0].length() - 1)));
//              tokenQueue.push(make_pair(TokenType::Separator, ","));
//              token.erase(token.begin());
//              break;
//            } else {
//              // tokenQueue.push(make_pair(TokenType::Identifier,
//              // token[0].substr(0, i)));
//              // tokenQueue.push(make_pair(TokenType::Separator, ","));
//              // token[0] = token[0].substr(i + 1, token[0].length() - i - 1);
//              // break;
//            }
//          }
//        }
//      }
//    } else if (existSemi(token[0])) // second situation
//    {
//      for (int i = 0; i < token[0].length(); i++) {
//        if (token[0][i] == ';') {
//          if (token[0].length() == 1) {
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token.erase(token.begin());
//          } else if (i == token[0].length() - 1) {
//
//            tokenQueue.push(
//                make_pair(TokenType::Identifier, token[0].substr(0, i)));
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token.erase(token.begin());
//          } else {
//            if (i == 0) {
//
//            } else {
//              tokenQueue.push(
//                  make_pair(TokenType::Identifier, token[0].substr(0, i)));
//            }
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token[0] = token[0].substr(i + 1, token[0].length() - i - 1);
//          }
//        }
//      }
//
//      end = true;
//    } else {
//      throw("missing semicollumn in declaration!");
//    }
//  }
//  if (end) {
//    expectionOfDeclaration(token);
//  }
//
//  return token;
//}
//
//vector<string> PQLLexer::tokenizeStmt(vector<string> token) {
//  tokenQueue.push(make_pair(TokenType::Keyword, "stmt"));
//  token.erase(token.begin());
//  bool end = false;
//  while (!end) {
//    if (token[0].find(",") != token[0].npos) // first situation
//    {
//      while (!token.empty() && token[0].find(',') != token[0].npos) {
//        for (int i = 0; i < token[0].length(); i++) {
//          if (token[0][i] == ',') {
//            if (token[0].length() == 1) {
//              // tokenQueue.push(make_pair(TokenType::Separator, ","));
//              // token.erase(token.begin());
//            } else if (i == token[0].length() - 1) {
//
//              tokenQueue.push(
//                  make_pair(TokenType::Identifier,
//                            token[0].substr(0, token[0].length() - 1)));
//              tokenQueue.push(make_pair(TokenType::Separator, ","));
//              token.erase(token.begin());
//              break;
//            } else {
//              // tokenQueue.push(make_pair(TokenType::Identifier,
//              // token[0].substr(0, i)));
//              // tokenQueue.push(make_pair(TokenType::Separator, ","));
//              // token[0] = token[0].substr(i + 1, token[0].length() - i - 1);
//              // break;
//            }
//          }
//        }
//      }
//    } else if (existSemi(token[0])) // second situation
//    {
//      for (int i = 0; i < token[0].length(); i++) {
//        if (token[0][i] == ';') {
//          if (token[0].length() == 1) {
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token.erase(token.begin());
//          } else if (i == token[0].length() - 1) {
//
//            tokenQueue.push(
//                make_pair(TokenType::Identifier, token[0].substr(0, i)));
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token.erase(token.begin());
//          } else {
//            if (i == 0) {
//
//            } else {
//              tokenQueue.push(
//                  make_pair(TokenType::Identifier, token[0].substr(0, i)));
//            }
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token[0] = token[0].substr(i + 1, token[0].length() - i - 1);
//          }
//        }
//      }
//
//      end = true;
//
//    } else {
//      throw("missing semicollumn in declaration!");
//    }
//  }
//  if (end) {
//    expectionOfDeclaration(token);
//  }
//
//  return token;
//}
//
//vector<string> PQLLexer::tokenizeConstant(vector<string> token) {
//  tokenQueue.push(make_pair(TokenType::Keyword, "constant"));
//  token.erase(token.begin());
//  bool end = false;
//  while (!end) {
//    if (token[0].find(",") != token[0].npos) // first situation
//    {
//      while (!token.empty() && token[0].find(',') != token[0].npos) {
//        for (int i = 0; i < token[0].length(); i++) {
//          if (token[0][i] == ',') {
//            if (token[0].length() == 1) {
//              // tokenQueue.push(make_pair(TokenType::Separator, ","));
//              // token.erase(token.begin());
//            } else if (i == token[0].length() - 1) {
//
//              tokenQueue.push(
//                  make_pair(TokenType::Identifier,
//                            token[0].substr(0, token[0].length() - 1)));
//              tokenQueue.push(make_pair(TokenType::Separator, ","));
//              token.erase(token.begin());
//              break;
//            } else {
//              // tokenQueue.push(make_pair(TokenType::Identifier,
//              // token[0].substr(0, i)));
//              // tokenQueue.push(make_pair(TokenType::Separator, ","));
//              // token[0] = token[0].substr(i + 1, token[0].length() - i - 1);
//              // break;
//            }
//          }
//        }
//      }
//    } else if (existSemi(token[0])) // second situation
//    {
//      for (int i = 0; i < token[0].length(); i++) {
//        if (token[0][i] == ';') {
//          if (token[0].length() == 1) {
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token.erase(token.begin());
//          } else if (i == token[0].length() - 1) {
//
//            tokenQueue.push(
//                make_pair(TokenType::Identifier, token[0].substr(0, i)));
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token.erase(token.begin());
//          } else {
//            if (i == 0) {
//
//            } else {
//              tokenQueue.push(
//                  make_pair(TokenType::Identifier, token[0].substr(0, i)));
//            }
//            tokenQueue.push(make_pair(TokenType::Separator, ";"));
//            token[0] = token[0].substr(i + 1, token[0].length() - i - 1);
//          }
//        }
//      }
//
//      end = true;
//    } else {
//      throw("missing semicollumn in declaration!");
//    }
//  }
//  if (end) {
//    expectionOfDeclaration(token);
//  }
//
//  return token;
//}
//
//bool PQLLexer::tokenContainCommas(string token) {
//  return (token.find(",") != token.npos);
//}
//
//void PQLLexer::expectionOfSelect(vector<string> token) {
//  if (token[0] == "such" && token[1] == "that") {
//
//  } else if (token[0] == "pattern") {
//
//  }
//  else if (token[0].find("with") != token[0].npos) {
//
//  }
//  else {
//    throw invalid_argument("expect such that or pattern");
//  }
//}
//
//void PQLLexer::expectionOfDeclaration(vector<string> token) {
//  if (token.empty()) {
//    throw("no selction after the declaration.");
//  } else if (token[0] == "Select" || token[0] == "stmt" || token[0] == "read" ||
//             token[0] == "print" || token[0] == "while" || token[0] == "if" ||
//             token[0] == "assign" || token[0] == "variable" ||
//             token[0] == "constant" || token[0] == "procedure" ||
//             token[0] == "call" || token[0] == "prog_line") {
//
//  } else {
//    throw invalid_argument("should follows a new declaration or selection");
//  }
//}
//
//vector<string> PQLLexer::tokenizeSelect(vector<string> token) {
//
//  tokenQueue.push(make_pair(TokenType::Keyword, "Select"));
//  token.erase(token.begin());
//  // push the select, then for three situations: 1. "a, b". 2. "a;" 3. "a"
//  bool end = false;
//  while (!end) {
//	if (token[0].find("<") != token[0].npos) {
//		int angle_appear;
//		string whole = "";
//		tokenQueue.push(make_pair(TokenType::Separator, "<"));
//		for (int i = 0; i < token.size(); i++) {
//			if (token[i].find(">") != token[i].npos) {
//				angle_appear = i;
//			}
//		}
//		for (int j = 0; j <= angle_appear; j++) {
//			whole.append(token[j]);
//			//whole.append(whole, token[j]);
//		}
//		for (int m = 0; m <= angle_appear; m++) {
//			token.erase(token.begin());
//		}
//		int string_begin = 1;;
//		int string_end;
//		int index = 0;
//		while (index < whole.length()) {
//			if (whole[index] == ',') {
//				if (whole.substr(string_begin, index - string_begin).find(".") != whole.substr(string_begin, index - string_begin).npos)
//				{
//					string before_dot = "";
//					string after_dot = "";
//					int dot_appear = 0;
//					for (int i = string_begin; i < whole.length(); i++) {
//						if (whole[i] == '.') {
//							dot_appear = i;
//							break;
//						}
//					}
//					before_dot = whole.substr(string_begin, dot_appear - string_begin);
//					after_dot = whole.substr(dot_appear + 1, index - dot_appear - 1);
//					tokenQueue.push(make_pair(TokenType::Identifier, before_dot));
//					tokenQueue.push(make_pair(TokenType::Separator, "."));
//					tokenQueue.push(make_pair(TokenType::Identifier, after_dot));
//				}
//				else {
//					tokenQueue.push(make_pair(TokenType::Identifier, whole.substr(string_begin, index - string_begin)));
//				}
//				string_begin = index + 1;
//				tokenQueue.push(make_pair(TokenType::Separator, ","));
//			}
//			else if (whole[index] == '>') {
//				if (whole.substr(string_begin, index - string_begin).find(".") != whole.substr(string_begin, index - string_begin).npos)
//				{
//					string before_dot = "";
//					string after_dot = "";
//					int dot_appear = 0;
//					for (int i = string_begin; i < whole.length(); i++) {
//						if (whole[i] == '.') {
//							dot_appear = i;
//						}
//					}
//					before_dot = whole.substr(string_begin, dot_appear - string_begin);
//					after_dot = whole.substr(dot_appear + 1, whole.length() - dot_appear - 2);
//					tokenQueue.push(make_pair(TokenType::Identifier, before_dot));
//					tokenQueue.push(make_pair(TokenType::Separator, "."));
//					tokenQueue.push(make_pair(TokenType::Identifier, after_dot));
//				}
//				else {
//					tokenQueue.push(make_pair(TokenType::Identifier, whole.substr(string_begin, index - string_begin)));
//				}
//				tokenQueue.push(make_pair(TokenType::Separator, ">"));
//			}
//			
//			index++;
//		}
//		end = true;
//
//	} else if (token[0].find(".") != token[0].npos) { // a.procName
//		string before_dot = "";
//		string after_dot = "";
//		int dot_appear = 0;
//		for (int i = 0; i < token[0].length(); i++) {
//			if (token[0][i] == '.') {
//				dot_appear = i;
//			}
//		}
//		before_dot = token[0].substr(0, dot_appear);
//		after_dot = token[0].substr(dot_appear + 1, token[0].length() - dot_appear - 1);
//		tokenQueue.push(make_pair(TokenType::Identifier, before_dot));
//		tokenQueue.push(make_pair(TokenType::Separator, "."));
//		tokenQueue.push(make_pair(TokenType::Identifier, after_dot));
//		token.erase(token.begin());
//		end = true;
//	}
//	
//	else if (token[0].find(';') != token[0].npos) // second situation
//    {
//
//      throw invalid_argument("no semicollumn");
//    } else {
//      tokenQueue.push(make_pair(TokenType::Identifier, token[0]));
//      token.erase(token.begin());
//
//      if (!token.empty() && token[0].find(";") != token[0].npos &&
//          token[0][0] == ';') {
//        throw invalid_argument("no semicollumn");
//      } else {
//      }
//      end = true;
//    }
//  }
//  if (!token.empty())
//    expectionOfSelect(token);
//  return token;
//}
//
//vector<string> PQLLexer::tokenizePattern(vector<string> token) {
//	int check = 1;
//	int n0 = -1, n1 = -1, n2 = -1, n3, iter = 0;
//	string s;
//	string first_s, second_s;
//	bool appearQuo;
//	bool appearCom;
//	bool findFirstBra;
//	int count;
//	int endQuo;
//	endQuo = 0;
//	count = 0;
//	appearQuo = false;
//	appearCom = false;
//	findFirstBra = false;
//	// KEYWORD,"a" "(" (3 situations: v, "v", _) "," "_" or _" adfsf"_ ")" ";"
//
//	while (iter != token.size() && check == 1) {
//
//		if ((endQuo == 2 || endQuo == 0) &&
//			token[iter].find(")") != token[iter].npos) {
//			check = 0;
//			break;
//		}
//		if (token[iter].find("\"") != token[iter].npos && appearCom) {
//			endQuo++;
//			if (endQuo == 2)
//				break;
//		}
//		if (token[iter].find(",") != token[iter].npos) {
//			appearCom = true;
//		}
//		iter++;
//	}
//	appearCom = false;
//	endQuo = 0;
//	if (iter == token.size()) {
//		iter = iter - 1;
//	}
//	s = token[0];
//	for (int i = 1; i <= iter; i++) {
//
//		if (count == 0) {
//			s = s + token[i];
//			if (token[i].find("\"") != token[i].npos) {
//				for (int j = 0; j < token[i].length(); j++) {
//					if (token[i] == "\"") {
//						if (count == 0) {
//							count = 1;
//						}
//						else {
//							count = 0;
//						}
//					}
//				}
//			}
//		}
//		else {
//			s = s + " " + token[i];
//			if (token[i].find("\"") != token[i].npos) {
//				for (int j = 0; j < token[i].length(); j++) {
//					if (token[i] == "\"") {
//						if (count == 0) {
//							count = 1;
//						}
//						else {
//							count = 0;
//						}
//					}
//				}
//			}
//		}
//	}
//	for (int i = 0; i < iter + 1; i++) {
//		token.erase(token.begin());
//	}
//	int count_for_commas = 0;
//	for (int i = 0; i < s.length(); i++) {
//		if (s[i] == ',') {
//			count_for_commas++;
//		}
//	}
//	if (count_for_commas == 2) {
//		if (s.find("pattern") != s.npos) {
//			tokenQueue.push(make_pair(TokenType::Keyword, "pattern"));
//			s = s.substr(7, s.length() - 7);
//		}
//		string first_string;
//		int string_begin = 0;
//		for (int i = 0; i < s.length(); i++) {
//			if (s[i] == '(') {
//				tokenQueue.push(make_pair(TokenType::Identifier, s.substr(0, i)));
//				tokenQueue.push(make_pair(TokenType::Separator, "("));
//				string_begin = i + 1;
//			}
//			else if (s[i] == ',') {
//				first_string = s.substr(string_begin, i - string_begin);
//				break;
//			}
//		}
//		if (first_string.find("\"") != first_string.npos) {
//			tokenQueue.push(make_pair(TokenType::Separator, "\""));
//			tokenQueue.push(make_pair(TokenType::Identifier, first_string.substr(1, first_string.length() - 2)));
//			tokenQueue.push(make_pair(TokenType::Separator, "\""));
//		}
//		else {
//			tokenQueue.push(make_pair(TokenType::Identifier, first_string));
//		}
//		tokenQueue.push(make_pair(TokenType::Separator, ","));
//		tokenQueue.push(make_pair(TokenType::Identifier, "_"));
//		tokenQueue.push(make_pair(TokenType::Separator, ","));
//		tokenQueue.push(make_pair(TokenType::Identifier, "_"));
//		tokenQueue.push(make_pair(TokenType::Separator, ")")); //shijin version2.0
//	}
//	else {
//		for (int i = 0; i < s.length(); i++) {
//			// no space
//			if (s[i] == '(' && findFirstBra == false) {
//				n0 = i;
//				findFirstBra = true;
//			}
//			if (s[i] == ')' && appearCom && (endQuo == 2 || endQuo == 0)) {
//				n2 = i;
//			}
//			if (s[i] == '\"' && appearCom) {
//				endQuo++;
//			}
//			if (s[i] == ',') {
//				n1 = i;
//				appearCom = true;
//			}
//		}
//		if (n0 != -1 && n1 != -1 && n2 != -1) {
//			if (s.find("pattern") != s.npos) {
//				tokenQueue.push(make_pair(TokenType::Keyword, "pattern"));
//				tokenQueue.push(make_pair(TokenType::Identifier, s.substr(7, n0 - 7)));
//			}
//			else {
//				tokenQueue.push(make_pair(TokenType::Identifier, s.substr(0, n0)));
//			}
//			tokenQueue.push(make_pair(TokenType::Separator, "("));
//			first_s = s.substr(n0 + 1, n1 - 1 - n0);
//			if (first_s.find("\"") == first_s.npos) {
//				if (first_s[0] == '_') {
//					tokenQueue.push(make_pair(TokenType::Identifier, "_"));
//
//				}
//				else {
//					tokenQueue.push(make_pair(TokenType::Identifier, first_s.substr(0, 1)));
//				}
//			}
//			else {
//				if (first_s[0] == '\"' && first_s[first_s.length() - 1] == '\"') {
//					tokenQueue.push(make_pair(TokenType::Separator, "\""));
//					tokenQueue.push(make_pair(TokenType::Identifier,
//						first_s.substr(1, first_s.length() - 2)));
//					tokenQueue.push(make_pair(TokenType::Separator, "\""));
//				}
//				else {
//					//throw invalid_argument("first parameter format error");
//					tokenQueue.push(make_pair(TokenType::Identifier, first_s));
//				}
//			}
//			tokenQueue.push(make_pair(TokenType::Separator, ","));
//			second_s = s.substr(n1 + 1, n2 - 1 - n1);
//			if (second_s.find("\"") == second_s.npos) {
//				if (second_s[0] == '_') {
//					tokenQueue.push(make_pair(TokenType::Identifier, "_"));
//				}
//				else {
//					throw invalid_argument("cannot have a variable on the right");
//				}
//			}
//			else {
//				if (second_s.find('_') != second_s.npos) {
//					if (second_s[0] == '_' && second_s[second_s.length() - 1] == '_' &&
//						second_s[1] == '\"' && second_s[second_s.length() - 2] == '\"') {
//						tokenQueue.push(make_pair(TokenType::Identifier, "_"));
//						tokenQueue.push(make_pair(TokenType::Separator, "\""));
//						tokenQueue.push(make_pair(TokenType::Identifier,
//							second_s.substr(2, second_s.length() - 4)));
//						tokenQueue.push(make_pair(TokenType::Separator, "\""));
//						tokenQueue.push(make_pair(TokenType::Identifier, "_"));
//					}
//				}
//				else if (second_s[0] == '\"' &&
//					second_s[second_s.length() - 1] == '\"') {
//					tokenQueue.push(make_pair(TokenType::Separator, "\""));
//					tokenQueue.push(make_pair(TokenType::Identifier,
//						second_s.substr(1, second_s.length() - 2)));
//					tokenQueue.push(make_pair(TokenType::Separator, "\""));
//				}
//			}
//			tokenQueue.push(make_pair(TokenType::Separator, ")"));
//		}
//		else {
//			throw "lack , or ( or )";
//		}
//	}
//
//  if (s.find(';') != s.npos) {
//
//    throw invalid_argument("no semicollumn");
//  } else if (!token.empty() && token[0] == "such" &&
//             token[1].find("that") != token[1].npos) {
//
//  }
//  else if (!token.empty() && token[0] == "and") {
//
//  }
//  else if (!token.empty() && token[0] == "with") {
//
//  }
//  else if (!token.empty() && token[0] == "pattern") {
//	
//    //token = tokenizePattern(token);
//  } else {
//    if (!token.empty()) {
//      throw invalid_argument("should be pattern or such that or and");
//    }
//  }
//  return token;
//}
//
//vector<string> PQLLexer::tokenizeFollows(vector<string> token) {
//  int check = 1;
//  int n0 = -1, n1 = -1, n2 = -1, n3, iter = 0;
//  string s;
//  bool appearQuo;
//  appearQuo = false;
//  // KEYWORD, "(" "," ")" ";"
//
//  while (!token.empty() && check == 1) {
//    if (token[iter].find(")") != token[iter].npos) {
//      check = 0;
//    }
//    iter++;
//  }
//  iter = iter - 1;
//  s = token[0];
//  for (int i = 1; i <= iter; i++) {
//
//    s = s + token[i];
//  }
//  for (int i = 0; i < iter + 1; i++) {
//    token.erase(token.begin());
//  }
//  for (int i = 0; i < s.length(); i++) {
//    // no space
//    if (s[i] == '(') {
//      n0 = i;
//    }
//    if (s[i] == ',') {
//      n1 = i;
//    }
//    if (s[i] == ')') {
//      n2 = i;
//    }
//  }
//  if (n0 != -1 && n1 != -1 && n2 != -1) {
//    tokenQueue.push(make_pair(TokenType::Keyword, "Follows"));
//    tokenQueue.push(make_pair(TokenType::Separator, "("));
//    tokenQueue.push(
//        make_pair(TokenType::Identifier, s.substr(n0 + 1, n1 - 1 - n0)));
//    tokenQueue.push(make_pair(TokenType::Separator, ","));
//    tokenQueue.push(
//        make_pair(TokenType::Identifier, s.substr(n1 + 1, n2 - 1 - n1)));
//    tokenQueue.push(make_pair(TokenType::Separator, ")"));
//  } else {
//    throw invalid_argument("lack , or ; or ( or )");
//  }
//
//  if (s.find(";") != s.npos) {
//    throw invalid_argument("no semicollumn");
//  } else if (!token.empty() && token[0] == "such" &&
//             token[1].find("that") != token[1].npos) {
//    /*tokenQueue.push(make_pair(TokenType::Separator, ";"));*/
//    tokenQueue.push(make_pair(TokenType::Keyword, "such that"));
//    //if (token[1].length() != 4) {
//    //  token[1] = token[1].substr(4, token[1].length() - 4);
//    //  token.erase(token.begin());
//    //} else {
//    //  token.erase(token.begin());
//    //  token.erase(token.begin());
//    //}
//  }
//  else if (!token.empty() && token[0] == "and") {
//
//  }
//  else if (!token.empty() && token[0] == "with") {
//
//  }
//  else if (!token.empty() && token[0] == "pattern") {
//    //token = tokenizePattern(token);
//  } else {
//    if (!token.empty()) {
//      throw invalid_argument("should be pattern or such that or and");
//    }
//  }
//  return token;
//}
//
//vector<string> PQLLexer::tokenizeFollowsT(vector<string> token) {
//  int check = 1;
//  int n0 = -1, n1 = -1, n2 = -1, n3, iter = 0;
//  string s;
//  bool appearQuo;
//  appearQuo = false;
//  // KEYWORD, "(" "," ")" ";"
//
//  while (!token.empty() && check == 1) {
//    if (token[iter].find(")") != token[iter].npos) {
//      check = 0;
//    }
//    iter++;
//  }
//  iter = iter - 1;
//  s = token[0];
//  for (int i = 1; i <= iter; i++) {
//
//    s = s + token[i];
//  }
//  for (int i = 0; i < iter + 1; i++) {
//    token.erase(token.begin());
//  }
//  for (int i = 0; i < s.length(); i++) {
//    // no space
//    if (s[i] == '(') {
//      n0 = i;
//    }
//    if (s[i] == ',') {
//      n1 = i;
//    }
//    if (s[i] == ')') {
//      n2 = i;
//    }
//  }
//  if (n0 != -1 && n1 != -1 && n2 != -1) {
//    tokenQueue.push(make_pair(TokenType::Keyword, "Follows*"));
//    tokenQueue.push(make_pair(TokenType::Separator, "("));
//    tokenQueue.push(
//        make_pair(TokenType::Identifier, s.substr(n0 + 1, n1 - 1 - n0)));
//    tokenQueue.push(make_pair(TokenType::Separator, ","));
//    tokenQueue.push(
//        make_pair(TokenType::Identifier, s.substr(n1 + 1, n2 - 1 - n1)));
//    tokenQueue.push(make_pair(TokenType::Separator, ")"));
//  } else {
//    throw invalid_argument("lack , or ; or ( or )");
//  }
//
//  if (s.find(";") != s.npos) {
//    throw invalid_argument("no semicollumn");
//  }
//
//  else if (!token.empty() && token[0] == "such" &&
//           token[1].find("that") != token[1].npos) {
//    /*tokenQueue.push(make_pair(TokenType::Separator, ";"));*/
//    tokenQueue.push(make_pair(TokenType::Keyword, "such that"));
//    //if (token[1].length() != 4) {
//    //  token[1] = token[1].substr(4, token[1].length() - 4);
//    //  token.erase(token.begin());
//    //} else {
//    //  token.erase(token.begin());
//    //  token.erase(token.begin());
//    //}
//  }
//  else if (!token.empty() && token[0] == "and") {
//
//  }
//  else if (!token.empty() && token[0] == "with") {
//
//  }
//  else if (!token.empty() && token[0] == "pattern") {
//    //token = tokenizePattern(token);
//  } else {
//    if (!token.empty()) {
//      throw invalid_argument("should be pattern or such that or and");
//    }
//  }
//  return token;
//}
//
//vector<string> PQLLexer::tokenizeParent(vector<string> token) {
//  int check = 1;
//  int n0 = -1, n1 = -1, n2 = -1, n3, iter = 0;
//  string s;
//  bool appearQuo;
//  appearQuo = false;
//  // KEYWORD, "(" "," ")" ";"
//
//  while (!token.empty() && check == 1) {
//    if (token[iter].find(")") != token[iter].npos) {
//      check = 0;
//    }
//    iter++;
//  }
//  iter = iter - 1;
//  s = token[0];
//  for (int i = 1; i <= iter; i++) {
//
//    s = s + token[i];
//  }
//  for (int i = 0; i < iter + 1; i++) {
//    token.erase(token.begin());
//  }
//  for (int i = 0; i < s.length(); i++) {
//    // no space
//    if (s[i] == '(') {
//      n0 = i;
//    }
//    if (s[i] == ',') {
//      n1 = i;
//    }
//    if (s[i] == ')') {
//      n2 = i;
//    }
//  }
//  if (n0 != -1 && n1 != -1 && n2 != -1) {
//    tokenQueue.push(make_pair(TokenType::Keyword, "Parent"));
//    tokenQueue.push(make_pair(TokenType::Separator, "("));
//    tokenQueue.push(
//        make_pair(TokenType::Identifier, s.substr(n0 + 1, n1 - 1 - n0)));
//    tokenQueue.push(make_pair(TokenType::Separator, ","));
//    tokenQueue.push(
//        make_pair(TokenType::Identifier, s.substr(n1 + 1, n2 - 1 - n1)));
//    tokenQueue.push(make_pair(TokenType::Separator, ")"));
//  } else {
//    throw invalid_argument("lack , or ; or ( or )");
//  }
//
//  if (s.find(";") != s.npos) {
//    throw invalid_argument("no semicollumn");
//  }
//
//  else if (!token.empty() && token[0] == "such" &&
//           token[1].find("that") != token[1].npos) {
//
//    //tokenQueue.push(make_pair(TokenType::Keyword, "such that"));
//    //if (token[1].length() != 4) {
//    //  token[1] = token[1].substr(4, token[1].length() - 4);
//    //  token.erase(token.begin());
//    //} else {
//    //  token.erase(token.begin());
//    //  token.erase(token.begin());
//    //}
//
//  }
//  else if (!token.empty() && token[0] == "and") {
//
//  }
//  else if (!token.empty() && token[0] == "with") {
//
//  }
//  else if (!token.empty() && token[0] == "pattern") {
//    //token = tokenizePattern(token);
//  } else {
//    if (!token.empty()) {
//      throw invalid_argument("should be pattern or such that or and");
//    }
//  }
//  return token;
//}
//
//vector<string> PQLLexer::tokenizeParentT(vector<string> token) {
//  int check = 1;
//  int n0 = -1, n1 = -1, n2 = -1, n3, iter = 0;
//  string s;
//  bool appearQuo;
//  appearQuo = false;
//  // KEYWORD, "(" "," ")" ";"
//
//  while (!token.empty() && check == 1) {
//    if (token[iter].find(")") != token[iter].npos) {
//      check = 0;
//    }
//    iter++;
//  }
//  iter = iter - 1;
//  s = token[0];
//  for (int i = 1; i <= iter; i++) {
//
//    s = s + token[i];
//  }
//  for (int i = 0; i < iter + 1; i++) {
//    token.erase(token.begin());
//  }
//  for (int i = 0; i < s.length(); i++) {
//    // no space
//    if (s[i] == '(') {
//      n0 = i;
//    }
//    if (s[i] == ',') {
//      n1 = i;
//    }
//    if (s[i] == ')') {
//      n2 = i;
//    }
//  }
//  if (n0 != -1 && n1 != -1 && n2 != -1) {
//    tokenQueue.push(make_pair(TokenType::Keyword, "Parent*"));
//    tokenQueue.push(make_pair(TokenType::Separator, "("));
//    tokenQueue.push(
//        make_pair(TokenType::Identifier, s.substr(n0 + 1, n1 - 1 - n0)));
//    tokenQueue.push(make_pair(TokenType::Separator, ","));
//    tokenQueue.push(
//        make_pair(TokenType::Identifier, s.substr(n1 + 1, n2 - 1 - n1)));
//    tokenQueue.push(make_pair(TokenType::Separator, ")"));
//  } else {
//    throw invalid_argument("lack , or ; or ( or )");
//  }
//
//  if (s.find(";") != s.npos) {
//    throw invalid_argument("no semicollumn");
//  }
//
//  else if (!token.empty() && token[0] == "such" &&
//           token[1].find("that") != token[1].npos) {
//    //tokenQueue.push(make_pair(TokenType::Keyword, "such that"));
//    //if (token[1].length() != 4) {
//    //  token[1] = token[1].substr(4, token[1].length() - 4);
//    //  token.erase(token.begin());
//    //} else {
//    //  token.erase(token.begin());
//    //  token.erase(token.begin());
//    //}
//
//  }
//  else if (!token.empty() && token[0] == "and") {
//
//  }
//  else if (!token.empty() && token[0] == "with") {
//
//  }
//  else if (!token.empty() && token[0] == "pattern") {
//    //token = tokenizePattern(token);
//  } else {
//    if (!token.empty()) {
//      throw invalid_argument("should be pattern or such that or and");
//    }
//  }
//  return token;
//}
//
//vector<string> PQLLexer::tokenizeUses(vector<string> token) {
//  int check = 1;
//  int n0 = -1, n1 = -1, n2 = -1, n3, iter = 0;
//  string s;
//  string first_s;
//  string second_s;
//  bool findFirstBra;
//  int endQuo;
//  // KEYWORD, "(" "," ")" ";"
//  bool appearQuo;
//  bool appearCom;
//  int count;
//  appearQuo = false;
//  endQuo = 0;
//  count = 0;
//  appearCom = false;
//  findFirstBra = false;
//  while (iter != token.size() && check == 1) {
//
//    if ((endQuo == 2 || endQuo == 0) &&
//        token[iter].find(")") != token[iter].npos && appearCom) {
//      check = 0;
//      break;
//    }
//    if (token[iter].find("\"") != token[iter].npos && appearCom) {
//      endQuo++;
//    }
//    if (token[iter].find(",") != token[iter].npos) {
//      appearCom = true;
//    }
//    iter++;
//  }
//  appearCom = false;
//  endQuo = 0;
//  if (iter == token.size()) {
//    iter = iter - 1;
//  }
//  s = token[0];
//  for (int i = 1; i <= iter; i++) {
//
//    if (count == 0) {
//      s = s + token[i];
//      if (token[i].find("\"") != token[i].npos) {
//        for (int j = 0; j < token[i].length(); j++) {
//          if (token[i] == "\"") {
//            if (count == 0) {
//              count = 1;
//            } else {
//              count = 0;
//            }
//          }
//        }
//      }
//    } else {
//      s = s + " " + token[i];
//      if (token[i].find("\"") != token[i].npos) {
//        for (int j = 0; j < token[i].length(); j++) {
//          if (token[i] == "\"") {
//            if (count == 0) {
//              count = 1;
//            } else {
//              count = 0;
//            }
//          }
//        }
//      }
//    }
//  }
//  for (int i = 0; i < iter + 1; i++) {
//    token.erase(token.begin());
//  }
//  for (int i = 0; i < s.length(); i++) {
//    // no space
//    if (s[i] == '(' && findFirstBra == false) {
//      n0 = i;
//      findFirstBra = true;
//    }
//    if (s[i] == ')' && appearCom && (endQuo == 2 || endQuo == 0)) {
//      n2 = i;
//    }
//    if (s[i] == '\"' && appearCom) {
//      endQuo++;
//    }
//    if (s[i] == ',') {
//      n1 = i;
//      appearCom = true;
//    }
//  }
//  if (n0 != -1 && n1 != -1 && n2 != -1) {
//    tokenQueue.push(make_pair(TokenType::Keyword, "Uses"));
//    tokenQueue.push(make_pair(TokenType::Separator, "("));
//    first_s = s.substr(n0 + 1, n1 - 1 - n0);
//    if (first_s.find("\"") == first_s.npos) {
//
//      tokenQueue.push(make_pair(TokenType::Identifier, first_s));
//
//    } else {
//      if (first_s[0] == '\"' && first_s[first_s.length() - 1] == '\"') {
//        tokenQueue.push(make_pair(TokenType::Separator, "\""));
//        tokenQueue.push(make_pair(TokenType::Identifier,
//                                  first_s.substr(1, first_s.length() - 2)));
//        tokenQueue.push(make_pair(TokenType::Separator, "\""));
//      } else {
//        tokenQueue.push(make_pair(TokenType::Identifier, first_s));
//      }
//    }
//    tokenQueue.push(make_pair(TokenType::Separator, ","));
//    second_s = s.substr(n1 + 1, n2 - 1 - n1);
//    if (second_s.find("\"") == second_s.npos) {
//
//      tokenQueue.push(make_pair(TokenType::Identifier, second_s));
//
//    } else {
//      if (second_s[0] == '\"' && second_s[second_s.length() - 1] == '\"') {
//        tokenQueue.push(make_pair(TokenType::Separator, "\""));
//        tokenQueue.push(make_pair(TokenType::Identifier,
//                                  second_s.substr(1, second_s.length() - 2)));
//        tokenQueue.push(make_pair(TokenType::Separator, "\""));
//      }
//    }
//    tokenQueue.push(make_pair(TokenType::Separator, ")"));
//  } else {
//    throw invalid_argument("lack , or ; or ( or )");
//  }
//
//  if (s.find(";") != s.npos) {
//    throw invalid_argument("no semicollumn");
//  }
//  else if (!token.empty() && token[0] == "such" && token[1] == "that") {
//
//  }
//
//  else if (!token.empty() && token[0] == "pattern") {
//    //token = tokenizePattern(token);
//  }
//  else if (!token.empty() && token[0] == "and") {
//
//
//  }
//  else if (!token.empty() && token[0] == "with") {
//
//  }
//  else {
//    if (!token.empty()) {
//      throw invalid_argument("should be pattern or such that or and");
//    }
//  }
//  return token;
//}
//
//vector<string> PQLLexer::tokenizeModifies(vector<string> token) {
//  int check = 1;
//  int n0 = -1, n1 = -1, n2 = -1, n3, iter = 0;
//  string s;
//  string first_s;
//  string second_s;
//  bool appearQuo;
//  bool findFirstBra;
//  bool appearCom;
//  int endQuo;
//  int count;
//  endQuo = 0;
//  count = 0;
//  appearQuo = false;
//  appearCom = false;
//  findFirstBra = false;
//  // KEYWORD, "(" "," ")" ";"
//
//  while (iter != token.size() && check == 1) {
//
//    if ((endQuo == 2 || endQuo == 0) &&
//        token[iter].find(")") != token[iter].npos) {
//      check = 0;
//      break;
//    }
//    if (token[iter].find("\"") != token[iter].npos && appearCom) {
//      endQuo++;
//    }
//    if (token[iter].find(",") != token[iter].npos) {
//      appearCom = true;
//    }
//    iter++;
//  }
//  appearCom = false;
//  endQuo = 0;
//  if (iter == token.size()) {
//    iter = iter - 1;
//  }
//  s = token[0];
//  for (int i = 1; i <= iter; i++) {
//
//    if (count == 0) {
//      s = s + token[i];
//      if (token[i].find("\"") != token[i].npos) {
//        for (int j = 0; j < token[i].length(); j++) {
//          if (token[i] == "\"") {
//            if (count == 0) {
//              count = 1;
//            } else {
//              count = 0;
//            }
//          }
//        }
//      }
//    } else {
//      s = s + " " + token[i];
//      if (token[i].find("\"") != token[i].npos) {
//        for (int j = 0; j < token[i].length(); j++) {
//          if (token[i] == "\"") {
//            if (count == 0) {
//              count = 1;
//            } else {
//              count = 0;
//            }
//          }
//        }
//      }
//    }
//  }
//  for (int i = 0; i < iter + 1; i++) {
//    token.erase(token.begin());
//  }
//  for (int i = 0; i < s.length(); i++) {
//    // no space
//    if (s[i] == '(' && findFirstBra == false) {
//      n0 = i;
//      findFirstBra = true;
//    }
//    if (s[i] == ')' && appearCom && (endQuo == 2 || endQuo == 0)) {
//      n2 = i;
//    }
//    if (s[i] == '\"' && appearCom) {
//      endQuo++;
//    }
//    if (s[i] == ',') {
//      n1 = i;
//      appearCom = true;
//    }
//  }
//  if (n0 != -1 && n1 != -1 && n2 != -1) {
//    tokenQueue.push(make_pair(TokenType::Keyword, "Modifies"));
//    tokenQueue.push(make_pair(TokenType::Separator, "("));
//    first_s = s.substr(n0 + 1, n1 - 1 - n0);
//    if (first_s.find("\"") == first_s.npos) {
//
//      tokenQueue.push(make_pair(TokenType::Identifier, first_s));
//
//    } else {
//      if (first_s[0] == '\"' && first_s[first_s.length() - 1] == '\"') {
//        tokenQueue.push(make_pair(TokenType::Separator, "\""));
//        tokenQueue.push(make_pair(TokenType::Identifier,
//                                  first_s.substr(1, first_s.length() - 2)));
//        tokenQueue.push(make_pair(TokenType::Separator, "\""));
//      } else {
//        tokenQueue.push(make_pair(TokenType::Identifier, first_s));
//      }
//    }
//    tokenQueue.push(make_pair(TokenType::Separator, ","));
//    second_s = s.substr(n1 + 1, n2 - 1 - n1);
//    if (second_s.find("\"") == second_s.npos) {
//
//      tokenQueue.push(make_pair(TokenType::Identifier, second_s));
//
//    } else {
//      if (second_s[0] == '\"' && second_s[second_s.length() - 1] == '\"') {
//        tokenQueue.push(make_pair(TokenType::Separator, "\""));
//        tokenQueue.push(make_pair(TokenType::Identifier,
//                                  second_s.substr(1, second_s.length() - 2)));
//        tokenQueue.push(make_pair(TokenType::Separator, "\""));
//      }
//    }
//    tokenQueue.push(make_pair(TokenType::Separator, ")"));
//  } else {
//    throw invalid_argument("lack , or ; or ( or )");
//  }
//
//  if (s.find(";") != s.npos) {
//	  throw invalid_argument("no semicollumn");
//  }
//  else if (!token.empty() && token[0] == "such" && token[1] == "that") {
//
//  }
//
//  else if (!token.empty() && token[0] == "pattern") {
//	  //token = tokenizePattern(token);
//  }
//  else if (!token.empty() && token[0] == "and") {
//
//  }
//  else if (!token.empty() && token[0] == "with") {
//
//  }
//  else {
//	  if (!token.empty()) {
//		  throw invalid_argument("should be pattern or such that or and");
//	  }
//  }
//  return token;
//}
//
//vector<string> PQLLexer::tokenizeWith(vector<string> token) {
//	if (token[0].find("with") != token[0].npos) {
//		tokenQueue.push(make_pair(TokenType::Keyword, "with"));
//		token[0] = token[0].substr(4, token[0].length() - 4);
//		if (token[0] == "") {
//			token.erase(token.begin());
//		}
//	}
//	string whole = "";
//	bool appear_equal = false;
//	int end = 0;
//	for (int i = 0; i < token.size(); i++) {
//		if (token[i].find("=") != token[i].npos) {
//			appear_equal = true;
//			if (token[i][token[i].length() - 1] == '=') {
//				end = i + 1;
//			}
//			else {
//				end = i;
//			}
//			break;
//		}
//	}
//	for (int j = 0; j <= end; j++) {
//		//whole.append(whole, token[j]);
//		whole.append(token[j]);
//	}
//	int check_for = 0;
//	int count = 0;
//	for (int n = 0; n <= end; n++) {
//		if (token[0].find("\"") != token[0].npos) {
//			for (int i = 0; i < token[0].length(); i++) {
//				if (token[0][i] == '\"') {
//					check_for = i;
//					count++;
//				}
//				if (count == 2) {
//					break;
//				}
//			}
//			whole = whole.substr(0, whole.length() - token[0].length() + check_for + 1);
//			token[0] = token[0].substr(check_for + 1, token[0].length() - check_for - 1);
//		}
//		else {
//			token.erase(token.begin());
//		}
//		
//	}
//	for (int m = 0; m <= whole.length(); m++) {
//		if (whole[m] == '=') {
//			//tokenQueue.push(TokenType::Identifier, whole.substr(0, m - 1));
//			if (whole.substr(0, m).find(".") != whole.substr(0, m).npos)
//			{
//				string before_dot = "";
//				string after_dot = "";
//				int dot_appear = 0;
//				for (int i = 0; i < m; i++) {
//					if (whole[i] == '.') {
//						dot_appear = i;
//						break;
//					}
//				}
//				before_dot = whole.substr(0, dot_appear);
//				after_dot = whole.substr(dot_appear + 1, m - dot_appear - 1);
//				tokenQueue.push(make_pair(TokenType::Identifier, before_dot));
//				tokenQueue.push(make_pair(TokenType::Separator, "."));
//				tokenQueue.push(make_pair(TokenType::Identifier, after_dot));
//			}
//			else if (whole.substr(0, m).find("\"") != whole.substr(0, m).npos) {
//				tokenQueue.push(make_pair(TokenType::Separator, "\""));
//				tokenQueue.push(make_pair(TokenType::Identifier, whole.substr(1, m - 2)));
//				tokenQueue.push(make_pair(TokenType::Separator, "\""));
//			}
//			else {
//				tokenQueue.push(make_pair(TokenType::Identifier, whole.substr(0, m)));
//			}
//			tokenQueue.push(make_pair(TokenType::Separator, "="));
//			if (whole.substr(m + 1, whole.length() - m - 1).find(".") != whole.substr(m + 1, whole.length() - m - 1).npos)
//			{
//				string before_dot = "";
//				string after_dot = "";
//				int dot_appear = 0;
//				for (int i = m + 1; i < whole.length(); i++) {
//					if (whole[i] == '.') {
//						dot_appear = i;
//					}
//				}
//				before_dot = whole.substr(m + 1, dot_appear - m - 1);
//				after_dot = whole.substr(dot_appear + 1, whole.length() - dot_appear - 1);
//				tokenQueue.push(make_pair(TokenType::Identifier, before_dot));
//				tokenQueue.push(make_pair(TokenType::Separator, "."));
//				tokenQueue.push(make_pair(TokenType::Identifier, after_dot));
//			}
//			else if (whole.substr(m + 1, whole.length() - m - 1).find("\"") != whole.substr(m + 1, whole.length() - m - 1).npos) {
//				tokenQueue.push(make_pair(TokenType::Separator, "\""));
//				tokenQueue.push(make_pair(TokenType::Identifier, whole.substr(m + 2, whole.length() - m - 3)));
//				tokenQueue.push(make_pair(TokenType::Separator, "\""));
//			}
//			else {
//				tokenQueue.push(make_pair(TokenType::Identifier, whole.substr(m + 1, whole.length() - m - 1)));
//			}
//			//tokenQueue.push(TokenType::Identifier, whole.substr(m + 1, whole.length() - m - 1));
//		}
//	}
//	if (!token.empty() && token[0] == "such" && token[1] == "that") {
//
//	}
//
//	else if (!token.empty() && token[0] == "pattern") {
//		//token = tokenizePattern(token);
//	}
//	else if (!token.empty() && token[0] == "and") {
//
//	}
//	else if (!token.empty() && token[0] == "with") {
//
//	}
//	else {
//		if (token[0] == "") {
//			token.erase(token.begin());
//		}
//		if (!token.empty()) {
//			throw invalid_argument("should be pattern or such that or and");
//		}
//	}
//	return token;
//}
//
//vector<string> PQLLexer::tokenizeNext(vector<string> token) {
//	int check = 1;
//	int n0 = -1, n1 = -1, n2 = -1, n3, iter = 0;
//	string s;
//	bool appearQuo;
//	appearQuo = false;
//	// KEYWORD, "(" "," ")" ";"
//
//	while (!token.empty() && check == 1) {
//		if (token[iter].find(")") != token[iter].npos) {
//			check = 0;
//		}
//		iter++;
//	}
//	iter = iter - 1;
//	s = token[0];
//	for (int i = 1; i <= iter; i++) {
//
//		s = s + token[i];
//	}
//	for (int i = 0; i < iter + 1; i++) {
//		token.erase(token.begin());
//	}
//	for (int i = 0; i < s.length(); i++) {
//		// no space
//		if (s[i] == '(') {
//			n0 = i;
//		}
//		if (s[i] == ',') {
//			n1 = i;
//		}
//		if (s[i] == ')') {
//			n2 = i;
//		}
//	}
//	if (n0 != -1 && n1 != -1 && n2 != -1) {
//		tokenQueue.push(make_pair(TokenType::Keyword, "Next"));
//		tokenQueue.push(make_pair(TokenType::Separator, "("));
//		tokenQueue.push(
//			make_pair(TokenType::Identifier, s.substr(n0 + 1, n1 - 1 - n0)));
//		tokenQueue.push(make_pair(TokenType::Separator, ","));
//		tokenQueue.push(
//			make_pair(TokenType::Identifier, s.substr(n1 + 1, n2 - 1 - n1)));
//		tokenQueue.push(make_pair(TokenType::Separator, ")"));
//	}
//	else {
//		throw invalid_argument("lack , or ; or ( or )");
//	}
//
//	if (s.find(";") != s.npos) {
//		throw invalid_argument("no semicollumn");
//	}
//
//	else if (!token.empty() && token[0] == "such" &&
//		token[1].find("that") != token[1].npos) {
//
//		//tokenQueue.push(make_pair(TokenType::Keyword, "such that"));
//		//if (token[1].length() != 4) {
//		//  token[1] = token[1].substr(4, token[1].length() - 4);
//		//  token.erase(token.begin());
//		//} else {
//		//  token.erase(token.begin());
//		//  token.erase(token.begin());
//		//}
//
//	}
//	else if (!token.empty() && token[0] == "and") {
//
//	}
//	else if (!token.empty() && token[0] == "pattern") {
//		
//	}
//	else if (!token.empty() && token[0] == "with") {
//
//	}
//	else {
//		if (!token.empty()) {
//			throw invalid_argument("should be pattern or such that or and");
//		}
//	}
//	return token;
//}
//
//vector<string> PQLLexer::tokenizeNextT(vector<string> token) {
//	int check = 1;
//	int n0 = -1, n1 = -1, n2 = -1, n3, iter = 0;
//	string s;
//	bool appearQuo;
//	appearQuo = false;
//	// KEYWORD, "(" "," ")" ";"
//
//	while (!token.empty() && check == 1) {
//		if (token[iter].find(")") != token[iter].npos) {
//			check = 0;
//		}
//		iter++;
//	}
//	iter = iter - 1;
//	s = token[0];
//	for (int i = 1; i <= iter; i++) {
//
//		s = s + token[i];
//	}
//	for (int i = 0; i < iter + 1; i++) {
//		token.erase(token.begin());
//	}
//	for (int i = 0; i < s.length(); i++) {
//		// no space
//		if (s[i] == '(') {
//			n0 = i;
//		}
//		if (s[i] == ',') {
//			n1 = i;
//		}
//		if (s[i] == ')') {
//			n2 = i;
//		}
//	}
//	if (n0 != -1 && n1 != -1 && n2 != -1) {
//		tokenQueue.push(make_pair(TokenType::Keyword, "Next*"));
//		tokenQueue.push(make_pair(TokenType::Separator, "("));
//		tokenQueue.push(
//			make_pair(TokenType::Identifier, s.substr(n0 + 1, n1 - 1 - n0)));
//		tokenQueue.push(make_pair(TokenType::Separator, ","));
//		tokenQueue.push(
//			make_pair(TokenType::Identifier, s.substr(n1 + 1, n2 - 1 - n1)));
//		tokenQueue.push(make_pair(TokenType::Separator, ")"));
//	}
//	else {
//		throw invalid_argument("lack , or ; or ( or )");
//	}
//
//	if (s.find(";") != s.npos) {
//		throw invalid_argument("no semicollumn");
//	}
//
//	else if (!token.empty() && token[0] == "such" &&
//		token[1].find("that") != token[1].npos) {
//		//tokenQueue.push(make_pair(TokenType::Keyword, "such that"));
//		//if (token[1].length() != 4) {
//		//	token[1] = token[1].substr(4, token[1].length() - 4);
//		//	token.erase(token.begin());
//		//}
//		//else {
//		//	token.erase(token.begin());
//		//	token.erase(token.begin());
//		//}
//
//	}
//	else if (!token.empty() && token[0] == "and") {
//
//	}
//	else if (!token.empty() && token[0] == "with") {
//
//	}
//	else if (!token.empty() && token[0] == "pattern") {
//		//token = tokenizePattern(token);
//	}
//	else {
//		if (!token.empty()) {
//			throw invalid_argument("should be pattern or such that or and");
//		}
//	}
//	return token;
//}
//
//vector<string> PQLLexer::tokenizeCalls(vector<string> token) {
//	int check = 1;
//	int n0 = -1, n1 = -1, n2 = -1, n3, iter = 0;
//	string s;
//	string first_s;
//	string second_s;
//	bool findFirstBra;
//	int endQuo;
//	// KEYWORD, "(" "," ")" ";"
//	bool appearQuo;
//	bool appearCom;
//	int count;
//	appearQuo = false;
//	endQuo = 0;
//	count = 0;
//	appearCom = false;
//	findFirstBra = false;
//	while (iter != token.size() && check == 1) {
//
//		if ((endQuo == 2 || endQuo == 0) &&
//			token[iter].find(")") != token[iter].npos && appearCom) {
//			check = 0;
//			break;
//		}
//		if (token[iter].find("\"") != token[iter].npos && appearCom) {
//			endQuo++;
//		}
//		if (token[iter].find(",") != token[iter].npos) {
//			appearCom = true;
//		}
//		iter++;
//	}
//	appearCom = false;
//	endQuo = 0;
//	if (iter == token.size()) {
//		iter = iter - 1;
//	}
//	s = token[0];
//	for (int i = 1; i <= iter; i++) {
//
//		if (count == 0) {
//			s = s + token[i];
//			if (token[i].find("\"") != token[i].npos) {
//				for (int j = 0; j < token[i].length(); j++) {
//					if (token[i] == "\"") {
//						if (count == 0) {
//							count = 1;
//						}
//						else {
//							count = 0;
//						}
//					}
//				}
//			}
//		}
//		else {
//			s = s + " " + token[i];
//			if (token[i].find("\"") != token[i].npos) {
//				for (int j = 0; j < token[i].length(); j++) {
//					if (token[i] == "\"") {
//						if (count == 0) {
//							count = 1;
//						}
//						else {
//							count = 0;
//						}
//					}
//				}
//			}
//		}
//	}
//	for (int i = 0; i < iter + 1; i++) {
//		token.erase(token.begin());
//	}
//	for (int i = 0; i < s.length(); i++) {
//		// no space
//		if (s[i] == '(' && findFirstBra == false) {
//			n0 = i;
//			findFirstBra = true;
//		}
//		if (s[i] == ')' && appearCom && (endQuo == 2 || endQuo == 0)) {
//			n2 = i;
//		}
//		if (s[i] == '\"' && appearCom) {
//			endQuo++;
//		}
//		if (s[i] == ',') {
//			n1 = i;
//			appearCom = true;
//		}
//	}
//	if (n0 != -1 && n1 != -1 && n2 != -1) {
//		tokenQueue.push(make_pair(TokenType::Keyword, "Calls"));
//		tokenQueue.push(make_pair(TokenType::Separator, "("));
//		first_s = s.substr(n0 + 1, n1 - 1 - n0);
//		if (first_s.find("\"") == second_s.npos) {
//
//			tokenQueue.push(make_pair(TokenType::Identifier, first_s));
//
//		}
//		else {
//			if (first_s[0] == '\"' && first_s[first_s.length() - 1] == '\"') {
//				tokenQueue.push(make_pair(TokenType::Separator, "\""));
//				tokenQueue.push(make_pair(TokenType::Identifier,
//					first_s.substr(1, first_s.length() - 2)));
//				tokenQueue.push(make_pair(TokenType::Separator, "\""));
//			}
//			else {
//				tokenQueue.push(make_pair(TokenType::Identifier, first_s));
//			}
//		}
//		tokenQueue.push(make_pair(TokenType::Separator, ","));
//		second_s = s.substr(n1 + 1, n2 - 1 - n1);
//		if (second_s.find("\"") == second_s.npos) {
//
//			tokenQueue.push(make_pair(TokenType::Identifier, second_s));
//
//		}
//		else {
//			if (second_s[0] == '\"' && second_s[second_s.length() - 1] == '\"') {
//				tokenQueue.push(make_pair(TokenType::Separator, "\""));
//				tokenQueue.push(make_pair(TokenType::Identifier,
//					second_s.substr(1, second_s.length() - 2)));
//				tokenQueue.push(make_pair(TokenType::Separator, "\""));
//			}
//		}
//		tokenQueue.push(make_pair(TokenType::Separator, ")"));
//	}
//	else {
//		throw invalid_argument("lack , or ; or ( or )");
//	}
//
//	if (s.find(";") != s.npos) {
//		throw invalid_argument("no semicollumn");
//	}
//	else if (!token.empty() && token[0] == "such" && token[1] == "that") {
//
//	}
//	else if (!token.empty() && token[0] == "and") {
//
//	}
//	else if (!token.empty() && token[0] == "with") {
//
//	}
//	else if (!token.empty() && token[0] == "pattern") {
//		//token = tokenizePattern(token);
//	}
//	else {
//		if (!token.empty()) {
//			throw invalid_argument("should be pattern or such that or and");
//		}
//	}
//	return token;
//}
//
//
//vector<string> PQLLexer::tokenizeCallsT(vector<string> token) {
//	int check = 1;
//	int n0 = -1, n1 = -1, n2 = -1, n3, iter = 0;
//	string s;
//	string first_s;
//	string second_s;
//	bool findFirstBra;
//	int endQuo;
//	// KEYWORD, "(" "," ")" ";"
//	bool appearQuo;
//	bool appearCom;
//	int count;
//	appearQuo = false;
//	endQuo = 0;
//	count = 0;
//	appearCom = false;
//	findFirstBra = false;
//	while (iter != token.size() && check == 1) {
//
//		if ((endQuo == 2 || endQuo == 0) &&
//			token[iter].find(")") != token[iter].npos && appearCom) {
//			check = 0;
//			break;
//		}
//		if (token[iter].find("\"") != token[iter].npos && appearCom) {
//			endQuo++;
//		}
//		if (token[iter].find(",") != token[iter].npos) {
//			appearCom = true;
//		}
//		iter++;
//	}
//	appearCom = false;
//	endQuo = 0;
//	if (iter == token.size()) {
//		iter = iter - 1;
//	}
//	s = token[0];
//	for (int i = 1; i <= iter; i++) {
//
//		if (count == 0) {
//			s = s + token[i];
//			if (token[i].find("\"") != token[i].npos) {
//				for (int j = 0; j < token[i].length(); j++) {
//					if (token[i] == "\"") {
//						if (count == 0) {
//							count = 1;
//						}
//						else {
//							count = 0;
//						}
//					}
//				}
//			}
//		}
//		else {
//			s = s + " " + token[i];
//			if (token[i].find("\"") != token[i].npos) {
//				for (int j = 0; j < token[i].length(); j++) {
//					if (token[i] == "\"") {
//						if (count == 0) {
//							count = 1;
//						}
//						else {
//							count = 0;
//						}
//					}
//				}
//			}
//		}
//	}
//	for (int i = 0; i < iter + 1; i++) {
//		token.erase(token.begin());
//	}
//	for (int i = 0; i < s.length(); i++) {
//		// no space
//		if (s[i] == '(' && findFirstBra == false) {
//			n0 = i;
//			findFirstBra = true;
//		}
//		if (s[i] == ')' && appearCom && (endQuo == 2 || endQuo == 0)) {
//			n2 = i;
//		}
//		if (s[i] == '\"' && appearCom) {
//			endQuo++;
//		}
//		if (s[i] == ',') {
//			n1 = i;
//			appearCom = true;
//		}
//	}
//	if (n0 != -1 && n1 != -1 && n2 != -1) {
//		tokenQueue.push(make_pair(TokenType::Keyword, "Calls*"));
//		tokenQueue.push(make_pair(TokenType::Separator, "("));
//		first_s = s.substr(n0 + 1, n1 - 1 - n0);
//		if (first_s.find("\"") == first_s.npos) {
//
//			tokenQueue.push(make_pair(TokenType::Identifier, first_s));
//
//		}
//		else {
//			if (first_s[0] == '\"' && first_s[first_s.length() - 1] == '\"') {
//				tokenQueue.push(make_pair(TokenType::Separator, "\""));
//				tokenQueue.push(make_pair(TokenType::Identifier,
//					first_s.substr(1, first_s.length() - 2)));
//				tokenQueue.push(make_pair(TokenType::Separator, "\""));
//			}
//			else {
//				tokenQueue.push(make_pair(TokenType::Identifier, first_s));
//			}
//		}
//		tokenQueue.push(make_pair(TokenType::Separator, ","));
//		second_s = s.substr(n1 + 1, n2 - 1 - n1);
//		if (second_s.find("\"") == second_s.npos) {
//
//			tokenQueue.push(make_pair(TokenType::Identifier, second_s));
//
//		}
//		else {
//			if (second_s[0] == '\"' && second_s[second_s.length() - 1] == '\"') {
//				tokenQueue.push(make_pair(TokenType::Separator, "\""));
//				tokenQueue.push(make_pair(TokenType::Identifier,
//					second_s.substr(1, second_s.length() - 2)));
//				tokenQueue.push(make_pair(TokenType::Separator, "\""));
//			}
//		}
//		tokenQueue.push(make_pair(TokenType::Separator, ")"));
//	}
//	else {
//		throw invalid_argument("lack , or ; or ( or )");
//	}
//
//	if (s.find(";") != s.npos) {
//		throw invalid_argument("no semicollumn");
//	}
//	else if (!token.empty() && token[0] == "such" && token[1] == "that") {
//
//	}
//	else if (!token.empty() && token[0] == "and") {
//
//	}
//	else if (!token.empty() && token[0] == "with") {
//
//	}
//	else if (!token.empty() && token[0] == "pattern") {
//		//token = tokenizePattern(token);
//	}
//	else {
//		if (!token.empty()) {
//			throw invalid_argument("should be pattern or such that or and");
//		}
//	}
//	return token;
//}
