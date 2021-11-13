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
      case(kind::operand): new(&m_data) sc_operand(rhs.m_data.o);break;
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
      case(kind::operand): new(&m_data) sc_operand(std::move(rhs.m_data.o));break;
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

  m_data.unop = new sc_unary_operation(sc_expression(sc_operand(v,ti)));

  return *this;
}


sc_expression&
sc_expression::
assign(const sc_operand&  o) noexcept
{
  clear();

  m_kind = kind::operand;

  new(&m_data) sc_operand(o);

  return *this;
}


sc_expression&
sc_expression::
assign(sc_operand&&  o) noexcept
{
  clear();

  m_kind = kind::operand;

  new(&m_data) sc_operand(std::move(o));

  return *this;
}


void
sc_expression::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::operand): std::destroy_at(&m_data.o);break;
  case(kind::unary ): std::destroy_at(m_data.unop);break;
  case(kind::binary): std::destroy_at(m_data.binop);break;
    }


  m_kind = kind::null;
}


void
sc_expression::
fix(const sc_function&  fn) noexcept
{
    switch(m_kind)
    {
  case(kind::operand): m_data.o.fix(fn);break;
  case(kind::unary ): m_data.unop->fix(fn);break;
  case(kind::binary): m_data.binop->fix(fn);break;
    }
}


const sc_type_info&
sc_expression::
type_info() const noexcept
{
    switch(m_kind)
    {
  case(kind::operand): return m_data.o.type_info();break;
  case(kind::unary  ): return m_data.unop->type_info();break;
  case(kind::binary ): return m_data.binop->type_info();break;
    }


  return sc_undef_ti;
}


const sc_value*
sc_expression::
constant_value() const noexcept
{
    switch(m_kind)
    {
  case(kind::operand): return m_data.o.constant_value();break;
  case(kind::unary  ): return m_data.unop->constant_value();break;
  case(kind::binary ): return m_data.binop->constant_value();break;
    }


  return nullptr;
}


sc_value
sc_expression::
evaluate(sc_context&  ctx) const noexcept
{
    switch(m_kind)
    {
  case(kind::operand): return m_data.o.evaluate(ctx);break;
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
  case(kind::operand): m_data.o.print();break;
  case(kind::unary  ): m_data.unop->print();break;
  case(kind::binary ): m_data.binop->print();break;
    }
}




}




