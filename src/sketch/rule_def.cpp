#include "sketch/rule_def.hpp"
#include "core/scope.hpp"
#include "core/type.hpp"

namespace gpse
{
    namespace sketch
    {
        void setupLexer(lang::Lexer& lexer)
        {
            lexer.rules().clear();

            lexer.wsRule() = [](int h) -> bool
            {
                return std::isspace(h) || h == '\n';
            };

            lexer.rules().push_back(lang::Rule::single(LPAR, '('));
            lexer.rules().push_back(lang::Rule::single(RPAR, ')'));
            lexer.rules().push_back(lang::Rule::single(TIMES, '*'));
            lexer.rules().push_back(lang::Rule::single(SLASH, '/'));
            lexer.rules().push_back(lang::Rule::single(PLUS, '+'));
            lexer.rules().push_back(lang::Rule::single(AND, "&&"));
            lexer.rules().push_back(lang::Rule::single(OR, "||"));
            lexer.rules().push_back(lang::Rule::single(SEMICOLON, ';'));
            lexer.rules().push_back(lang::Rule::single(COMMA, ','));
            lexer.rules().push_back(lang::Rule::single(LCURLY, '{'));
            lexer.rules().push_back(lang::Rule::single(RCURLY, '}'));

            std::map<std::string, int> keywords;
            keywords["true"] = K_TRUE;
            keywords["false"] = K_FALSE;
            keywords["return"] = K_RETURN;
            keywords["fun"] = K_FUN;
            keywords["if"] = K_IF;
            keywords["elif"] = K_ELIF;
            keywords["else"] = K_ELSE;
            keywords["while"] = K_WHILE;

            // MINUS / RETURNS
            {
                auto pred = [](int h) -> bool
                {
                    return h == '-';
                };

                auto rule = [](lang::Lexer* l) -> lang::Token
                {
                    l->eat('-');

                    if(l->hint() == '>')
                    {
                        l->eat('>');
                        return lang::Token(RETURNS);
                    }

                    return lang::Token(MINUS);
                };

                lexer.rules().push_back(lang::Rule(pred, rule));
            }

            // NEQ / NOT
            {
                auto pred = [](int h) -> bool
                {
                    return h == '!';
                };

                auto rule = [](lang::Lexer* l) -> lang::Token
                {
                    l->eat('!');

                    if(l->hint() == '=')
                    {
                        l->eat('=');
                        return lang::Token(NEQ);
                    }

                    return lang::Token(NOT);
                };

                lexer.rules().push_back(lang::Rule(pred, rule));
            }

            // LT / LTE
            {
                auto pred = [](int h) -> bool
                {
                    return h == '<';
                };

                auto rule = [](lang::Lexer* l) -> lang::Token
                {
                    l->eat('<');

                    if(l->hint() == '=')
                    {
                        l->eat('=');
                        return lang::Token(LTE);
                    }

                    return lang::Token(LT);
                };

                lexer.rules().push_back(lang::Rule(pred, rule));
            }

            // GT / GTE
            {
                auto pred = [](int h) -> bool
                {
                    return h == '>';
                };
                auto rule = [](lang::Lexer* l) -> lang::Token
                {
                    l->eat('>');

                    if(l->hint() == '=')
                    {
                        l->eat('=');
                        return lang::Token(GTE);
                    }

                    return lang::Token(GT);
                };

                lexer.rules().push_back(lang::Rule(pred, rule));
            }

            // EQ / EQUALS
            {
                auto pred = [](int h) -> bool
                {
                    return h == '=';
                };

                auto rule = [](lang::Lexer* l) -> lang::Token
                {
                    l->eat('=');

                    if(l->hint() == '=')
                    {
                        l->eat('=');
                        return lang::Token(EQ);
                    }

                    return lang::Token(EQUALS);
                };

                lexer.rules().push_back(lang::Rule(pred, rule));
            }

            // NUMBER
            {
                auto pred = [](int h) -> bool
                {
                    return std::isdigit(h);
                };

                auto rule = [](lang::Lexer* l) -> lang::Token
                {
                    std::string temp;
                    bool integer = true;

                    while(std::isdigit(l->hint()))
                    {
                        temp += l->get();
                    }

                    if(l->hint() == '.')
                    {
                        integer = false;

                        temp += l->get();

                        while(std::isdigit(l->hint()))
                        {
                            temp += l->get();
                        }
                    }

                    if (l->hint() == 'e' || l->hint() == 'E')
                    {
                        integer = false;
                        
                        temp += l->get();

                        if (l->hint() == '-')
                            temp += l->get();

                        while(std::isdigit(l->hint()))
                            temp += l->get();
                    }

                    if(integer)
                    {
                        return lang::Token(NUMBER, static_cast<int>(std::atoi(temp.c_str())));
                    }
                    else
                    {
                        return lang::Token(NUMBER, static_cast<float>(std::atof(temp.c_str())));
                    }
                };

                lexer.rules().push_back(lang::Rule(pred, rule));
            }

            // IDENT, VARIABLENAME, FUNCTIONNAME, CALLBACKNAME, TYPENAME and K_*
            {
                auto pred = [](int h) -> bool
                {
                    return std::isalpha(h) || h == '_';
                };

                auto rule = [keywords](lang::Lexer* l) -> lang::Token
                {
                    std::string temp;

                    while(std::isalpha(l->hint()) || l->hint() == '_')
                    {
                        temp += l->get();
                    }

                    while(std::isalnum(l->hint()) || l->hint() == '_')
                    {
                        temp += l->get();
                    }
                    auto kw_it = keywords.find(temp);

                    // if the identifier is a valid keyword
                    if(kw_it != keywords.end())
                    {
                        return lang::Token(kw_it->second);
                    }
                    // else, try to find out if it is a type, variable or function name
                    else
                    {
                        core::Symbol sym;
                        if(l->scope()->layer().find(temp, &sym))
                        {
                            if(sym.isVariable())
                            {
                                return lang::Token(VARIABLENAME, sym.variable());
                            }
                            else if(sym.isType())
                            {
                                return lang::Token(TYPENAME, sym.type());
                            }
                            else if(sym.isFunction())
                            {
                                return lang::Token(FUNCTIONNAME, sym.function());
                            }
                            else if(sym.isCallback())
                            {
                                return lang::Token(CALLBACKNAME, sym.callback());
                            }
                        }
                    }

                    return lang::Token(IDENT, temp);
                };

                lexer.rules().push_back(lang::Rule(pred, rule));
            }

            // STRING
            {
                auto pred = [](int h) -> bool
                {
                    return h == '"';
                };
                auto rule = [](lang::Lexer* l) -> lang::Token
                {
                    std::string temp;

                    l->eat('"');

                    for(int r = l->get(); r != '"' && r != -1; r = l->get())
                    {
                        if(r == '\\')
                        {
                            r = l->get();
                            if(r == -1)
                                break;
                            if(r == 'n')
                                temp += '\n';
                            else if(r == 'r')
                                temp += '\r';
                            else if(r == 't')
                                temp += '\t';
                            else
                                return lang::badToken;
                        }
                        else
                        {
                            temp += r;
                        }
                    }

                    // the trailing quote was already read in
                    // the loop

                    return lang::Token(STRING, temp);
                };

                lexer.rules().push_back(lang::Rule(pred, rule));
            }
        }
    }
}
