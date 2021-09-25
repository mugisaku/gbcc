#include"libgbcc/syntax.hpp"




namespace gbcc{




syntax_branch_element&
syntax_branch_element::
assign(const syntax_branch_element&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind  = rhs.m_kind ;
      m_token = rhs.m_token;

        switch(m_kind)
        {
      case(kind::string): new(&m_data) std::u16string(rhs.m_data.str);break;
      case(kind::branch): new(&m_data)  syntax_branch(rhs.m_data.br );break;
        }
    }


  return *this;
}


syntax_branch_element&
syntax_branch_element::
assign(syntax_branch_element&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind ,rhs.m_kind );
      std::swap(m_token,rhs.m_token);

        switch(m_kind)
        {
      case(kind::string): new(&m_data) std::u16string(std::move(rhs.m_data.str));break;
      case(kind::branch): new(&m_data)  syntax_branch(std::move(rhs.m_data.br ));break;
        }
    }


  return *this;
}


syntax_branch_element&
syntax_branch_element::
assign(const syntax_token&  tok, bool  k) noexcept
{
  clear();

  m_token = &tok;

  m_kind = k? kind::keyword:kind::token;

  return *this;
}


syntax_branch_element&
syntax_branch_element::
assign(const syntax_token&  tok, std::u16string_view  sv) noexcept
{
  clear();

  m_token = &tok;

  m_kind = kind::string;

  new(&m_data) std::u16string(sv);

  return *this;
}


syntax_branch_element&
syntax_branch_element::
assign(syntax_branch&&  bra) noexcept
{
  clear();

  m_kind = kind::branch;

  new(&m_data) syntax_branch(std::move(bra));

  return *this;
}




void
syntax_branch_element::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::string): std::destroy_at(&m_data.str);break;
  case(kind::branch): std::destroy_at(&m_data.br );break;
    }


  m_kind = kind::null;

  m_token = nullptr;
}


void
syntax_branch_element::
print(int  indent) const noexcept
{
    for(int  n = 0;  n < indent;  ++n)
    {
      printf("|__");
    }


    if(is_token())
    {
        if(is_keyword())
        {
          printf("KEYWORD,");
        }


      m_token->print();

      printf("\n");
    }

  else
    if(is_branch())
    {
      m_data.br.print(indent+1);
    }

  else
    if(is_string())
    {
      gbcc::print(m_data.str);
    }
}



}




