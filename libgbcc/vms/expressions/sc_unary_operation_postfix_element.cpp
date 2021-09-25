#include"libgbcc/vms/expression.hpp"




namespace gbcc{




sc_unary_operation::postfix_element&
sc_unary_operation::postfix_element::
assign(const postfix_element&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind = rhs.m_kind;

        switch(m_kind)
        {
      case(kind::access): new(&m_data) std::u16string(rhs.m_data.s);break;
      case(kind::call  ): new(&m_data) sc_expression_list(rhs.m_data.exprls);break;
      case(kind::index ): m_data.expr = new sc_expression(*rhs.m_data.expr);break;
        }
    }


  return *this;
}


sc_unary_operation::postfix_element&
sc_unary_operation::postfix_element::
assign(postfix_element&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind,rhs.m_kind);

        switch(m_kind)
        {
      case(kind::access): new(&m_data) std::u16string(std::move(rhs.m_data.s));break;
      case(kind::call  ): new(&m_data) sc_expression_list(std::move(rhs.m_data.exprls));break;
      case(kind::index ): std::swap(m_data.expr,rhs.m_data.expr);break;
        }
    }


  return *this;
}


sc_unary_operation::postfix_element&
sc_unary_operation::postfix_element::
assign(sc_expression_list&&  exprls) noexcept
{
  clear();

  m_kind = kind::call;

  new(&m_data) sc_expression_list(std::move(exprls));

  return *this;
}


sc_unary_operation::postfix_element&
sc_unary_operation::postfix_element::
assign(sc_expression&&  expr) noexcept
{
  clear();

  m_kind = kind::index;

  m_data.expr = new sc_expression(std::move(expr));

  return *this;
}


sc_unary_operation::postfix_element&
sc_unary_operation::postfix_element::
assign(std::u16string_view  sv) noexcept
{
  clear();

  m_kind = kind::access;

  new(&m_data) std::u16string(sv);

  return *this;
}


sc_unary_operation::postfix_element&
sc_unary_operation::postfix_element::
assign(std::string_view  sv) noexcept
{
  clear();

  m_kind = (sv == "++")? kind::increment
          :(sv == "--")? kind::decrement
          :kind::null
          ;

  return *this;
}


void
sc_unary_operation::postfix_element::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::access): std::destroy_at(&m_data.s);break;
  case(kind::index ): std::destroy_at( m_data.expr);break;
  case(kind::call  ): std::destroy_at(&m_data.exprls);break;
    }


  m_kind = kind::null;
}


void
sc_unary_operation::postfix_element::
print() const noexcept
{
    switch(m_kind)
    {
  case(kind::access):
      printf(".");
      gbcc::print(m_data.s);
      break;
  case(kind::index):
      printf("[");
      m_data.expr->print();
      printf("]");
      break;
  case(kind::increment): printf("++");break;
  case(kind::decrement): printf("--");break;
  case(kind::call):
      printf("(");

        for(auto&  e: m_data.exprls)
        {
          e.print();

          printf(",");
        }

      printf(")");
      break;
    }
}




}




