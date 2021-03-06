#include"libgbcc/syntax.hpp"




namespace gbcc{




const syntax_token
syntax_token_string::iterator::
m_null;


syntax_token_string::iterator&
syntax_token_string::iterator::
assign(const syntax_token_string&  s) noexcept
{
  m_current = s.data();
  m_end     = s.data()+s.size();

  return *this;
}


syntax_token_string::iterator&
syntax_token_string::iterator::
operator++() noexcept
{
  ++m_current;

  return *this;
}


syntax_token_string::iterator
syntax_token_string::iterator::
operator++(int) noexcept
{
  auto  tmp = *this;

  ++(*this);

  return tmp;
}




syntax_token_string::iterator
syntax_token_string::iterator::
operator+(int  n) const noexcept
{
  auto  t = *this;

  t += n;

  return t;
}


syntax_token_string::iterator&
syntax_token_string::iterator::
operator+=(int  n) noexcept
{
    if(n > 0)
    {
        while(n--)
        {
          ++(*this);
        }
    }


  return *this;
}


bool
syntax_token_string::iterator::
test(std::u16string_view  sv) const noexcept
{
  auto  p = sv.data();
  auto  n = sv.size();

  return (n == 1)? ((*this)[0].is_others(p[0]))
        :(n == 2)? ((*this)[0].is_others(p[0])) && ((*this)[1].is_others(p[1]))
        :(n == 3)? ((*this)[0].is_others(p[0])) && ((*this)[1].is_others(p[1])) && ((*this)[2].is_others(p[2]))
        :false;
}


void
syntax_token_string::iterator::
skip() noexcept
{
    while((*this)->is_others(u' ') ||
          (*this)->is_others(u'\n'))
    {
      ++(*this);
    }
}




}




