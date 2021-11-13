#include"libgbcc/vms/expression.hpp"
#include"libgbcc/vms/context.hpp"




namespace gbcc{




sc_unary_operation&
sc_unary_operation::
assign(const sc_unary_operation&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind           = rhs.m_kind;
      m_expression     = rhs.m_expression;
      m_type_info      = rhs.m_type_info;
      m_constant_value = clone_unique(rhs.m_constant_value);

        switch(m_kind)
        {
      case(kind::access): new(&m_data) std::u16string(rhs.m_data.s);break;
      case(kind::call  ): new(&m_data) sc_expression_list(rhs.m_data.exprls);break;
      case(kind::index ): m_data.expr = new sc_expression(*rhs.m_data.expr);break;
        }
    }


  return *this;
}


sc_unary_operation&
sc_unary_operation::
assign(sc_unary_operation&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind          ,rhs.m_kind);
      std::swap(m_expression    ,rhs.m_expression);
      std::swap(m_type_info     ,rhs.m_type_info);
      std::swap(m_constant_value,rhs.m_constant_value);

        switch(m_kind)
        {
      case(kind::access): new(&m_data) std::u16string(std::move(rhs.m_data.s));break;
      case(kind::call  ): new(&m_data) sc_expression_list(std::move(rhs.m_data.exprls));break;
      case(kind::index ): std::swap(m_data.expr,rhs.m_data.expr);break;
        }
    }


  return *this;
}


sc_unary_operation&
sc_unary_operation::
assign(sc_expression&&  e) noexcept
{
  clear();

  m_kind = kind::nop;

  m_expression = std::move(e);

  return *this;
}


sc_unary_operation&
sc_unary_operation::
assign(sc_expression&&  e, sc_expression_list&&  exprls) noexcept
{
  clear();

  m_kind = kind::call;

  m_expression = std::move(e);

  new(&m_data) sc_expression_list(std::move(exprls));

  return *this;
}


sc_unary_operation&
sc_unary_operation::
assign(sc_expression&&  e, sc_expression&&  expr) noexcept
{
  clear();

  m_kind = kind::index;

  m_expression = std::move(e);

  m_data.expr = new sc_expression(std::move(expr));

  return *this;
}


sc_unary_operation&
sc_unary_operation::
assign(sc_expression&&  e, std::u16string_view  sv) noexcept
{
  clear();

  m_kind = kind::access;

  m_expression = std::move(e);

  new(&m_data) std::u16string(sv);

  return *this;
}


sc_unary_operation&
sc_unary_operation::
assign(sc_expression&&  e, std::string_view  sv) noexcept
{
  clear();

  m_kind = (sv == "!")? kind::logical_not
          :(sv == "~")? kind::bitwise_not
          :(sv == "-")? kind::neg
          :(sv == "*")? kind::dereference
          :(sv == "&")? kind::address
          :(sv == "_++")? kind::postfix_increment
          :(sv == "_--")? kind::postfix_decrement
          :(sv == "++")? kind::prefix_increment
          :(sv == "--")? kind::prefix_decrement
          :(sv == "")? kind::nop
          : kind::null
          ;


  m_expression = std::move(e);

  return *this;
}




void
sc_unary_operation::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::access): std::destroy_at(&m_data.s);break;
  case(kind::index ): std::destroy_at( m_data.expr);break;
  case(kind::call  ): std::destroy_at(&m_data.exprls);break;
    }


  m_kind = kind::null;

  m_expression.clear();
  m_type_info.clear();

  m_constant_value.reset();
}


void
sc_unary_operation::
fix(const sc_function&  fn) noexcept
{
  m_expression.fix(fn);

  m_type_info = m_expression.type_info();

    if(m_expression.constant_value())
    {
      sc_context  ctx(fn.package());

      m_constant_value = std::make_unique<sc_value>(evaluate(ctx));
    }


    switch(m_kind)
    {
  case(kind::index):
      m_data.expr->fix(fn);
      break;
  case(kind::call):
        for(auto&  e: m_data.exprls)
        {
          e.fix(fn);
        }
      break;
    }
}


void
sc_unary_operation::
print() const noexcept
{
    switch(m_kind)
    {
  case(kind::nop):
      m_expression.print();
      break;
  case(kind::access):
      m_expression.print();
      printf(".");
      gbcc::print(m_data.s);
      break;
  case(kind::index):
      m_expression.print();
      printf("[");
      m_data.expr->print();
      printf("]");
      break;
  case(kind::postfix_increment):
      m_expression.print();
      printf("++");
      break;
  case(kind::postfix_decrement):
      m_expression.print();
      printf("--");
      break;
  case(kind::call):
      m_expression.print();
      printf("(");

        for(auto&  e: m_data.exprls)
        {
          e.print();

          printf(",");
        }

      printf(")");
      break;
  case(kind::neg):
      printf("-");
      m_expression.print();
      break;
  case(kind::logical_not):
      printf("!");
      m_expression.print();
      break;
  case(kind::bitwise_not):
      printf("~");
      m_expression.print();
      break;
  case(kind::address):
      printf("&");
      m_expression.print();
      break;
  case(kind::dereference):
      printf("*");
      m_expression.print();
      break;
  case(kind::prefix_increment):
      printf("++");
      m_expression.print();
      break;
  case(kind::prefix_decrement):
      printf("--");
      m_expression.print();
      break;
  default: printf("null unary");
    }
}




}




