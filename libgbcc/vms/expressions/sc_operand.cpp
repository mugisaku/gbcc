#include"libgbcc/vms/context.hpp"




namespace gbcc{




sc_operand&
sc_operand::
assign(const sc_operand&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind = rhs.m_kind;

        switch(m_kind)
        {
      case(kind::identifier): new(&m_data) std::u16string(rhs.m_data.s);break;
      case(kind::expression): m_data.e = new sc_expression(*rhs.m_data.e);break;
      case(kind::value     ): new(&m_data) sc_value_with_type_info(rhs.m_data.v);break;
        }
    }


  return *this;
}


sc_operand&
sc_operand::
assign(sc_operand&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind,rhs.m_kind);

        switch(m_kind)
        {
      case(kind::identifier): new(&m_data) std::u16string(std::move(rhs.m_data.s));break;
      case(kind::expression): std::swap(m_data.e,rhs.m_data.e);break;
      case(kind::value     ): new(&m_data) sc_value_with_type_info(std::move(rhs.m_data.v));break;
        }
    }


  return *this;
}


sc_operand&
sc_operand::
assign(std::u16string_view  sv) noexcept
{
  clear();

  m_kind = kind::identifier;

  new(&m_data) std::u16string(sv);

  return *this;
}


sc_operand&
sc_operand::
assign(sc_expression&&  e) noexcept
{
  clear();

  m_kind = kind::expression;

  m_data.e = new sc_expression(std::move(e));

  return *this;
}


sc_operand&
sc_operand::
assign(sc_value  v, const sc_type_info&  ti) noexcept
{
  clear();

  m_kind = kind::value;

  new(&m_data) sc_value_with_type_info(v,ti);

  return *this;
}


void
sc_operand::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::identifier): std::destroy_at(&m_data.s);break;
  case(kind::expression): std::destroy_at( m_data.e);break;
  case(kind::value     ): std::destroy_at(&m_data.v);break;
    }


  m_kind = kind::null;
}


sc_type_info
sc_operand::
type_info(const sc_context&  ctx) const noexcept
{
    switch(m_kind)
    {
  case(kind::identifier): return make_reference(ctx.find_symbol(m_data.s)->type_info());break;
  case(kind::expression): return m_data.e->type_info(ctx);break;
  case(kind::value     ): return m_data.v.type_info();break;
    }


  return sc_type_info();
}


sc_value
sc_operand::
evaluate(sc_context&  ctx) const noexcept
{
    switch(m_kind)
    {
  case(kind::identifier): return sc_value(ctx.get_reference(m_data.s).data());break;
  case(kind::expression): return m_data.e->evaluate(ctx);break;
  case(kind::value     ): return m_data.v;break;
    }


  return sc_value();
}


void
sc_operand::
print() const noexcept
{
    switch(m_kind)
    {
  case(kind::identifier): gbcc::print(m_data.s);break;
  case(kind::expression):
      printf("(");
      m_data.e->print();
      printf(")");
      break;
  case(kind::value):
      m_data.v.print();
      break;
    }
}




}




