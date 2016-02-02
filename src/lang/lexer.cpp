#include "lang/lexer.hpp"

#include <sstream>
#include <stdexcept>

using namespace gpse;
using namespace lang;

Lexer::Lexer(std::istream& in, core::Scope* scope)
    : _m_in(in)
    , _m_scope(scope)
    , _m_line(1)
    , _m_col(1)
    , _m_pos(_m_in.tellg())
    , _m_good(true)
{
}

Lexer::~Lexer()
{
}

Lexer::wsrule_t& Lexer::wsRule()
{
    return _m_wsRule;
}

Lexer::wsrule_t const& Lexer::wsRule() const
{
    return _m_wsRule;
}

std::vector<Rule>& Lexer::rules()
{
    return _m_rules;
}

std::vector<Rule> const& Lexer::rules() const
{
    return _m_rules;
}

core::Scope* Lexer::scope() const
{
    return _m_scope;
}

bool Lexer::good() const
{
    return _m_good;
}

void Lexer::reset()
{
    _m_in.seekg(0, std::ios::beg);
    _m_line = 1;
    _m_col = 1;
    _m_pos = _m_in.tellg();
    _m_good = true;
}

int Lexer::hint()
{
    return _m_in.peek();
}

int Lexer::get()
{
    int r = _m_in.get();
    _m_pos = _m_in.tellg();
    if(r == '\n')
    {
        ++_m_line;
        _m_col = 1;
    }
    else
    {
        ++_m_col;
    }
    return r;
}

bool Lexer::eat(char c)
{
    return get() == c;
}

bool Lexer::eat(std::string const& str)
{
    for(auto it = str.begin(); it != str.end(); ++it)
    {
        char r = get();
        if(r != *it)
        {
            return false;
        }
    }

    return true;
}

void Lexer::skipWs()
{
    while(_m_wsRule(hint()))
    {
        get();
    }
}

void Lexer::error(Token const& tok, std::string const& message) const
{
    std::ostringstream ss;

    ss << "line " << tok.debug.line << ", col " << tok.debug.col << ": error: " << message << std::endl;
    ss << "    " << _M_wholeLine(tok) << std::endl;
    ss << "    ";
    for(int i = 1; i < tok.debug.col; ++i)
    {
        ss << " ";
    }
    ss << "^";

    std::string err = ss.str();

    std::cerr << err << std::endl;
    throw std::runtime_error(err);
}

Token Lexer::token()
{
    skipWs();

    Token tok = eofToken;
    tok.debug.line = _m_line;
    tok.debug.col = _m_col;
    tok.debug.pos = _m_pos;

    if(!_m_good || _m_in.eof())
        return tok;

    for(auto it = _m_rules.begin(); it != _m_rules.end(); ++it)
        {
            if(it->predicate(hint()))
            {
                tok = it->get(this);
                tok.debug.line = _m_line;
                tok.debug.col = _m_col;
                tok.debug.pos = _m_pos;
                skipWs();
                return tok;
            }
        }

    _m_good = false;
    tok.which = Token::Bad;
    return tok;
}

std::string Lexer::_M_wholeLine(Token const& tok) const
{
    _m_in.clear();
    int saved = _m_in.tellg();
    _m_in.seekg(0, std::ios::beg);

    std::string line;
    for(int lineno = 0; lineno != tok.debug.line; ++lineno)
    {
        std::getline(_m_in, line);
    }

    _m_in.seekg(saved, std::ios::beg);

    return line;
}
