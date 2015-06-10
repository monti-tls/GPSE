#include "lang/lexer.hpp"
#include "lang/parser.hpp"
#include "sketch/rule_def.hpp"
#include "sketch/grammar_def.hpp"
#include "sketch/ast.hpp"
#include "sketch/pass_def.hpp"
#include "sketch/context.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <map>

using namespace gpse;

int main()
{
  std::string str =
  "int foo = true || false;"//-5*20+0;\n"
  "foo = 1/(4/3+1/3)*foo;\n"
  "foo = 0+foo;\n"
  "foo = foo+0*45;\n"
  "foo = 0-foo;\n"
  "foo = foo-0;\n"
  "foo = 1*foo;\n"
  "foo = foo*1;\n"
  "foo = 1/foo;\n"
  "foo = foo/1;\n"
  ;
  
  std::istringstream ss(str);
  
  sketch::Context* ctx = new sketch::Context();
  lang::Lexer lexer(ss, ctx);
  sketch::setupLexer(lexer);
  
  lang::Parser parser(lexer);
  sketch::setupParser(parser);
  
  lang::Node* root = parser.parseRaw("statement_block");
  if (!root)
    return -1;
  
  lang::TreePass print = sketch::getPrinterPass();
  lang::TreePass litred = sketch::getLiteralReductionPass();
  
  std::cout << "Parser output :" << std::endl;
  print.pass(root);
  
  litred.pass(root);
  
  std::cout << "After literal reduction pass :" << std::endl;
  print.pass(root);
  
  delete root;
  delete ctx;
  
  return 0;
}
