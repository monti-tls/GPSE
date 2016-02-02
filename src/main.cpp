#include "lang/lexer.hpp"
#include "lang/parser.hpp"
#include "sketch/rule_def.hpp"
#include "sketch/grammar_def.hpp"
#include "sketch/ast.hpp"
#include "sketch/pass_def.hpp"
#include "sketch/runtime_def.hpp"
#include "core/scope.hpp"
#include "core/callback.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <stack>
#include <vector>
#include <tuple>
#include <cmath>

using namespace gpse;

/* TODO-list :
 *   - separate source directory into core/lang/sketch dirs
 *   - update build system with proper Makefile
 *
 *   - implement conditionals
 *   - cleanup arithmetics (to handle auto casts)
 *   - handle errors better :
 *     - no more silent errors due to nullptrs returned upon AST building
 *     - throw exception on parser / typecheck / other pass error
 */

void print_i(int value)
{
    std::cout << "print_i(" << value << ")" << std::endl;
}

void print_f(float value)
{
    std::cout << "print_f(" << value << ")" << std::endl;
}

void print_s(std::string value)
{
    std::cout << "print_s(" << value << ")" << std::endl;
}

float mypow(float a, float b)
{
    return std::pow(a, b);
}

std::vector<int>* create_vec()
{
    return new std::vector<int>({1, 2, 3, 4});
}

void print_vec(std::vector<int>* vec)
{
    for(auto i : *vec)
        std::cout << i << " ";
    std::cout << std::endl;
}

int main()
{
    std::ifstream ss("src/sample.sketch");

    core::Scope* scope = new core::Scope();
    sketch::setupScope(scope);

    sketch::expose(scope, "printi", &print_i);
    sketch::expose(scope, "printf", &print_f);
    sketch::expose(scope, "prints", &print_s);
    sketch::expose(scope, "pow", &mypow);

    sketch::expose(scope, "create_vec", &create_vec);
    sketch::expose(scope, "print_vec", &print_vec);

    lang::Lexer lexer(ss, scope);
    sketch::setupLexer(lexer);

    lang::Parser parser(lexer);
    sketch::setupParser(parser);

    lang::Node* root = parser.parseRaw("program");
    if(!root)
        return -1;

    lang::TreePass print = sketch::getPrinterPass();
    lang::TreePass typecheck = sketch::getTypecheckPass();
    lang::TreePass optimize = sketch::getOptimizePass();
    lang::TreePass run = sketch::getRunPass();

    typecheck.pass(root);

    std::cout << "Parser output :" << std::endl;
    print.pass(root);

    /*optimize.pass(root);

    std::cout << "After optimization pass :" << std::endl;
    print.pass(root);*/

    std::cout << "** Running program **" << std::endl;
    run.pass(root);

    // std::cout << scope->layer().findRef("foo")->variable().value().as<int>() << std::endl;

    delete root;
    delete scope;

    return 0;
}
