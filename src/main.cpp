#include "lang/lexer.hpp"
#include "lang/parser.hpp"
#include "sketch/rule_def.hpp"
#include "sketch/grammar_def.hpp"
#include "sketch/ast.hpp"
#include "sketch/pass_def.hpp"
#include "core/scope.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <map>

using namespace gpse;

int main()
{
  std::string str =
  "int function(int n)\n"
  "{\n"
  "  int fee = (int) 1.0/2.0 + 3.0/4.0 + 5.0/6.0;\n"
  "  int foo = fee*function(n-1);\n"
  "}\n"
  ;
  
  std::istringstream ss(str);
  
  core::Scope* scope = new core::Scope();
  lang::Lexer lexer(ss, scope);
  sketch::setupLexer(lexer);
  
  lang::Parser parser(lexer);
  sketch::setupParser(parser);
  
  lang::Node* root = parser.parseRaw("function_declaration");
  if (!root)
    return -1;
  
  lang::TreePass print = sketch::getPrinterPass();
  lang::TreePass litred = sketch::getLiteralReductionPass();
  lang::TreePass typecheck = sketch::getTypecheckPass();
  
  std::cout << "Parser output :" << std::endl;
  print.pass(root);
  typecheck.pass(root);
  
  litred.pass(root);
  
  std::cout << "After literal reduction pass :" << std::endl;
  print.pass(root);
  
  delete root;
  delete scope;
  
  return 0;
}
