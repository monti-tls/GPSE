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
#include <fstream>
#include <map>
#include <stack>

using namespace gpse;

/* TODO-list :
 *   - properly reformat code (w/ 4*spaces tabs)
 *   - separate source directory into core/lang/sketch dirs
 *   - update build system with proper Makefile
 *   - clean up repo from shitty project files (and setup a better gitignore)
 *
 *   - implement conditionals
 *   - cleanup arithmetics (to handle auto casts)
 */

int main()
{
    // std::istringstream ss(str);
    std::ifstream ss("src/sample.sketch");

    core::Scope* scope = new core::Scope();
    lang::Lexer lexer(ss, scope);
    sketch::setupLexer(lexer);

    lang::Parser parser(lexer);
    sketch::setupParser(parser);

    lang::Node* root = parser.parseRaw("program");
    if(!root) return -1;

    lang::TreePass print = sketch::getPrinterPass();
    lang::TreePass typecheck = sketch::getTypecheckPass();
    lang::TreePass optimize = sketch::getOptimizePass();
    lang::TreePass run = sketch::getRunPass();

    typecheck.pass(root);

    std::cout << "Parser output :" << std::endl;
    print.pass(root);

    optimize.pass(root);

    std::cout << "After optimization pass :" << std::endl;
    print.pass(root);

    std::cout << "** Running program **" << std::endl;
    run.pass(root);

    std::cout << scope->layer().findRef("foo")->variable().value().cast<int>() << std::endl;

    delete root;
    delete scope;

    return 0;
}
