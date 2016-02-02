#include "lang/parser.hpp"

using namespace gpse;
using namespace lang;

Parser::Parser(Lexer& lexer)
    : _m_lexer(lexer)
    , _m_current(badToken)
    , _m_next(lexer.token())
    , _m_good(true)
{
}

Parser::~Parser()
{
}

std::map<std::string, Grammar>& Parser::grammars()
{
    return _m_grammars;
}

std::map<std::string, Grammar> const& Parser::grammars() const
{
    return _m_grammars;
}

core::Scope* Parser::scope() const
{
    return _m_lexer.scope();
}

bool Parser::good() const
{
    return _m_good;
}

void Parser::reset()
{
    _m_lexer.reset();
    _m_good = true;
    _m_next = _m_lexer.token();
    _m_current = badToken;
}

bool Parser::eat(int which, Token& token)
{
    token = _M_advance();
    return token.which == which;
}

Token Parser::seek()
{
    return _m_next;
}

bool Parser::predicate(std::string const& grammar)
{
    auto it = _m_grammars.find(grammar);
    if(it == _m_grammars.end())
    {
        return false;
    }

    return it->second.predicate(this);
}

Node* Parser::parseRaw(std::string const& grammar)
{
    auto it = _m_grammars.find(grammar);
    if(it == _m_grammars.end())
    {
        return nullptr;
    }

    Node* node = it->second.get(this);
    if(node)
    {
        node->setParser(this);
        if(!node->scopeLayer())
            node->setScopeLayer(&scope()->layer());
    }

    return node;
}

void Parser::error(std::string const& message, Token const& tok) const
{
    _m_lexer.error(tok, message);
}

void Parser::error(std::string const& message) const
{
    _m_lexer.error(_m_current, message);
}

Token Parser::_M_advance()
{
    if(!_m_good)
    {
        return eofToken;
    }

    _m_current = _m_next;
    _m_next = _m_lexer.token();

    if(_m_next == eofToken || _m_next == badToken)
    {
        _m_good = false;
    }

    return _m_current;
}
