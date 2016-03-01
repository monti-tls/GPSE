#include "lang/lexer.hpp"

#include <sstream>
#include <stdexcept>

using namespace gpse;
using namespace lang;

Lexer::Lexer(std::istream& in, core::Namespace& ns)
    : m_in(in)
    , m_ns(ns)
    , m_line(1)
    , m_col(1)
    , m_pos(m_in.tellg())
    , m_good(true)
{
}

Lexer::~Lexer()
{
}

core::Namespace& Lexer::ns() const
{ return m_ns; }

Lexer::wsrule_t& Lexer::wsRule()
{
    return m_wsRule;
}

Lexer::wsrule_t const& Lexer::wsRule() const
{
    return m_wsRule;
}

std::vector<Rule>& Lexer::rules()
{
    return m_rules;
}

std::vector<Rule> const& Lexer::rules() const
{
    return m_rules;
}

bool Lexer::good() const
{
    return m_good;
}

void Lexer::reset()
{
    m_in.seekg(0, std::ios::beg);
    m_line = 1;
    m_col = 1;
    m_pos = m_in.tellg();
    m_good = true;
}

int Lexer::hint()
{
    return m_in.peek();
}

int Lexer::get()
{
    int r = m_in.get();
    m_pos = m_in.tellg();
    if(r == '\n')
    {
        ++m_line;
        m_col = 1;
    }
    else
    {
        ++m_col;
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
    while(m_wsRule(hint()))
    {
        get();
    }
}

void Lexer::error(Token const& tok, std::string const& message) const
{
    std::ostringstream ss;

    ss << "line " << tok.debug.line << ", col " << tok.debug.col << ": error: " << message << std::endl;
    ss << "    " << M_wholeLine(tok) << std::endl;
    ss << "    ";
    for(int i = 1; i < tok.debug.col; ++i)
    {
        ss << " ";
    }
    ss << "^";

    std::string err = ss.str();
    throw std::runtime_error(err);
}

Token Lexer::token()
{
    skipWs();

    Token tok = eofToken;
    tok.debug.line = m_line;
    tok.debug.col = m_col;
    tok.debug.pos = m_pos;

    if(!m_good || m_in.eof())
        return tok;

    for(auto it = m_rules.begin(); it != m_rules.end(); ++it)
    {
        if(it->predicate(hint()))
        {
            tok = it->get(this);
            tok.debug.line = m_line;
            tok.debug.col = m_col;
            tok.debug.pos = m_pos;
            skipWs();
            return tok;
        }
    }

    m_good = false;
    tok.which = Token::Bad;
    return tok;
}

std::string Lexer::M_wholeLine(Token const& tok) const
{
    m_in.clear();
    int saved = m_in.tellg();
    m_in.seekg(0, std::ios::beg);

    std::string line;
    for(int lineno = 0; lineno != tok.debug.line; ++lineno)
    {
        std::getline(m_in, line);
    }

    m_in.seekg(saved, std::ios::beg);

    return line;
}
