#include "core/object.hpp"
#include "lang/lexer.hpp"
#include "sketch/rule_def.hpp"

#include <string>
#include <sstream>
#include <fstream>

using namespace gpse;

std::string tok2str(lang::Token tok)
{
	const char* arr[] =
	{ "<BAD>", "<EOF>", "<NONE>", "LPAR", "RPAR", "TIMES", "SLASH", "PLUS", "MINUS", "AND", "OR", "NOT", "LT", "LTE", "GT", "GTE", "EQ", "EQUALS", "SEMICOLON", "COMMA", "LCURLY", "RCURLY", "NEQ", "NUMBER", "STRING", "K_TRUE", "K_FALSE", "K_RETURN", "K_FUN", "K_IF", "K_ELIF", "K_ELSE", "K_WHILE", "SYMBOL", "IDENT" };

	return arr[tok.which];
}

int main()
{
    std::ifstream file("test");

    core::Namespace ns;
    lang::Lexer lex(file, ns);

    sketch::setupLexer(lex);

    lang::Token tok;
    do
    {
    	tok = lex.token();
    	std::cout << tok2str(tok) << std::endl;
    } while (tok != lang::Token::Bad && tok != lang::Token::Eof);

    return 0;
}
