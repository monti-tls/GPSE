#include "core/core.hpp"
#include "lang/lang.hpp"
#include "sketch/sketch.hpp"

#include <lconf/cli.h>

#include <stdexcept>
#include <iostream>
#include <string>
#include <fstream>

void __trace__(std::string str)
{
    std::cout << str;
}

int main(int argc, char** argv)
{
    using namespace gpse;

    /****************************************/
    /*** Command-line options definitions ***/
    /****************************************/

    lconf::cli::Parser options(argc, argv);

    options.setProgramDescription("The GPSE Sketch interpreter.");

    options.addSwitch('h', "help").setStop().setDescription("Print this help");

    options.addSwitch('x', "no-optimize").setDescription("Don't optimize the script before running");

    options.addSwitch('p', "print-ast").setDescription("Print the generated AST");

    options.addSwitch('d', "dry-run").setDescription("Don't run the script");

    /************************************/
    /*** Options parsing and checking ***/
    /************************************/

    try
    {
        options.parse();
    }
    catch(std::exception const& exc)
    {
        std::cerr << "Error: " << exc.what() << std::endl;
        return -1;
    }

    if(options.has("help"))
    {
        options.showHelp();
        return 0;
    }

    if(options.arguments().size() != 1)
    {
        std::cerr << "Error: " << argv[0] << " expects exactly one argument." << std::endl;
        std::cerr << "       Try " << argv[0] << " --help" << std::endl;
        return -1;
    }

    /***************************/
    /*** Actual useful stuff ***/
    /***************************/

    try
    {
        std::ifstream ss(options.arguments()[0]);
        if(!ss)
        {
            std::cerr << "Error: unable to open \"" << options.arguments()[0] << "\"" << std::endl;
            return -1;
        }

        core::Scope* scope = new core::Scope();
        sketch::setupScope(scope);

        sketch::expose(scope, "__trace__", &__trace__);

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

        if(!options.has("no-optimize"))
        {
            optimize.pass(root);
        }

        if(options.has("print-ast"))
        {
            print.pass(root);
        }

        if(!options.has("dry-run"))
        {
            run.pass(root);
        }

        delete root;
        delete scope;
    }
    catch(std::exception const& exc)
    {
        std::cerr << exc.what() << std::endl;
    }

    return 0;
}
