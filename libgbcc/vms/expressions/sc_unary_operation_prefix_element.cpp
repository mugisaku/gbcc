#include"libgbcc/vms/expression.hpp"




namespace gbcc{




sc_unary_operation::prefix_element&
sc_unary_operation::prefix_element::
assign(const prefix_element&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind = rhs.m_kind;
    }


  return *this;
}


sc_unary_operation::prefix_element&
sc_unary_operation::prefix_element::
assign(prefix_element&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind,rhs.m_kind);
    }


  return *this;
}


sc_unary_operation::prefix_element&
sc_unary_operation::prefix_element::
assign(std::string_view  sv) noexcept
{
  clear();

  m_kind = (sv == "!")? kind::logical_not
          :(sv == "~")? kind::bitwise_not
          :(sv == "-")? kind::neg
          :(sv == "*")? kind::dereference
          :(sv == "&")? kind::address
          :(sv == "++")? kind::increment
          :(sv == "--")? kind::decrement
          : kind::null
          ;

  return *this;
}


void
sc_unary_operation::prefix_element::
clear() noexcept
{
  m_kind = kind::null;
}


void
sc_unary_operation::prefix_element::
print() const noexcept
{
    switch(m_kind)
    {
  case(kind::neg        ): printf("-");break;
  case(kind::logical_not): printf("!");break;
  case(kind::bitwise_not): printf("~");break;
  case(kind::address    ): printf("&");break;
  case(kind::dereference): printf("*");break;
  case(kind::increment  ): printf("++");break;
  case(kind::decrement  ): printf("--");break;
    }
}




}




