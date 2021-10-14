#include"libgbcc/vms/expression.hpp"




namespace gbcc{




sc_expression&
sc_expression::
assign(const sc_expression&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind = rhs.m_kind;

        switch(m_kind)
        {
      case(kind::unary ): m_data.unop  = new sc_unary_operation(*rhs.m_data.unop);break;
      case(kind::binary): m_data.binop = new sc_binary_operation(*rhs.m_data.binop);break;
        }
    }


  return *this;
}


sc_expression&
sc_expression::
assign(sc_expression&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind,rhs.m_kind);

        switch(m_kind)
        {
      case(kind::unary ): m_data.unop  = rhs.m_data.unop ;break;
      case(kind::binary): m_data.binop = rhs.m_data.binop;break;
        }
    }


  return *this;
}


sc_expression&
sc_expression::
assign(sc_unary_operation&&  unop) noexcept
{
  clear();

  m_kind = kind::unary;

  m_data.unop = new sc_unary_operation(std::move(unop));

  return *this;
}


sc_expression&
sc_expression::
assign(sc_binary_operation&&  binop) noexcept
{
  clear();

  m_kind = kind::binary;

  m_data.binop = new sc_binary_operation(std::move(binop));

  return *this;
}


sc_expression&
sc_expression::
assign(sc_value  v, const sc_type_info&  ti) noexcept
{
  clear();

  m_kind = kind::unary;

  m_data.unop = new sc_unary_operation(v,ti);

  return *this;
}


sc_expression&
sc_expression::
assign(const sc_operand&  o) noexcept
{
  clear();

  m_kind = kind::unary;

  m_data.unop = new sc_unary_operation(o);

  return *this;
}


sc_expression&
sc_expression::
assign(sc_operand&&  o) noexcept
{
  clear();

  m_kind = kind::unary;

  m_data.unop = new sc_unary_operation(std::move(o));

  return *this;
}


void
sc_expression::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::unary ): std::destroy_at(m_data.unop);break;
  case(kind::binary): std::destroy_at(m_data.binop);break;
    }


  m_kind = kind::null;
}


sc_type_info
sc_expression::
type_info(const sc_context&  ctx) const noexcept
{
    switch(m_kind)
    {
  case(kind::unary ): return m_data.unop->type_info(ctx);break;
  case(kind::binary): return m_data.binop->type_info(ctx);break;
    }


  return sc_type_info();
}


sc_value
sc_expression::
evaluate(sc_context&  ctx) const noexcept
{
    switch(m_kind)
    {
  case(kind::unary ): return m_data.unop->evaluate(ctx);break;
  case(kind::binary): return m_data.binop->evaluate(ctx);break;
    }


  return sc_value();
}


void
sc_expression::
print() const noexcept
{
    switch(m_kind)
    {
  case(kind::unary ): m_data.unop->print();break;
  case(kind::binary): m_data.binop->print();break;
    }
}




}




