#include "lang/parser.hpp"

using namespace gpse;
using namespace lang;

Parser::Parser(Lexer& lexer)
    : m_lexer(lexer)
    , m_current(badToken)
    , m_next(lexer.token())
    , m_good(true)
{
}

Parser::~Parser()
{
}

std::map<std::string, Grammar>& Parser::grammars()
{
    return m_grammars;
}

std::map<std::string, Grammar> const& Parser::grammars() const
{
    return m_grammars;
}

bool Parser::good() const
{
    return m_good;
}

void Parser::reset()
{
    m_lexer.reset();
    m_good = true;
    m_next = m_lexer.token();
    m_current = badToken;
}

bool Parser::eat(int which, Token& token)
{
    token = m_advance();
    return token.which == which;
}

Token Parser::seek()
{
    return m_next;
}

bool Parser::predicate(std::string const& grammar)
{
    auto it = m_grammars.find(grammar);
    if(it == m_grammars.end())
    {
        return false;
    }

    return it->second.predicate(this);
}

Node* Parser::parseRaw(std::string const& grammar)
{
    auto it = m_grammars.find(grammar);
    if(it == m_grammars.end())
    {
        return nullptr;
    }

    Node* node = it->second.get(this);
    if(node)
    {
        node->setParser(this);
    }

    return node;
}

void Parser::error(std::string const& message, Token const& tok) const
{
    m_lexer.error(tok, message);
}

void Parser::error(std::string const& message) const
{
    m_lexer.error(m_current, message);
}

Token Parser::m_advance()
{
    if(!m_good)
    {
        return eofToken;
    }

    m_current = m_next;
    m_next = m_lexer.token();

    if(m_next == eofToken || m_next == badToken)
    {
        m_good = false;
    }

    return m_current;
}
