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
      m_type_info = rhs.m_type_info;
      m_constant_value = rhs.m_constant_value? new sc_value(*rhs.m_constant_value):nullptr;

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

      std::swap(m_kind          ,rhs.m_kind);
      std::swap(m_type_info     ,rhs.m_type_info);
      std::swap(m_constant_value,rhs.m_constant_value);

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
assign(const sc_value&  v) noexcept
{
  clear();

  m_kind = kind::unary;

  m_data.unop = new sc_unary_operation(v);

  return *this;
}


sc_expression&
sc_expression::
assign(sc_value&&  v) noexcept
{
  clear();

  m_kind = kind::unary;

  m_data.unop = new sc_unary_operation(std::move(v));

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

  m_type_info = sc_type_info();

  delete m_constant_value          ;
         m_constant_value = nullptr;
}


sc_value
sc_expression::
evaluate(sc_context&  ctx) const noexcept
{
    if(m_constant_value)
    {
      return *m_constant_value;
    }


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




