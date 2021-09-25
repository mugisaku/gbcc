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
      case(kind::identifier      ): new(&m_data) std::u16string(rhs.m_data.s);break;
      case(kind::integer         ): m_data.i = rhs.m_data.i;break;
      case(kind::unsigned_integer): m_data.u = rhs.m_data.u;break;
      case(kind::floating        ): m_data.f = rhs.m_data.f;break;
      case(kind::expression      ): m_data.e = new sc_expression(*rhs.m_data.e);break;
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
      case(kind::identifier      ): new(&m_data) std::u16string(std::move(rhs.m_data.s));break;
      case(kind::integer         ): m_data.i = rhs.m_data.i;break;
      case(kind::unsigned_integer): m_data.u = rhs.m_data.u;break;
      case(kind::floating        ): m_data.f = rhs.m_data.f;break;
      case(kind::expression      ): std::swap(m_data.e,rhs.m_data.e);break;
        }
    }


  return *this;
}


sc_operand&
sc_operand::
assign(int64_t  i) noexcept
{
  clear();

  m_kind = kind::integer;

  m_data.i = i;

  return *this;
}


sc_operand&
sc_operand::
assign(uint64_t  u) noexcept
{
  clear();

  m_kind = kind::unsigned_integer;

  m_data.u = u;

  return *this;
}


sc_operand&
sc_operand::
assign(double  f) noexcept
{
  clear();

  m_kind = kind::floating;

  m_data.f = f;

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


void
sc_operand::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::identifier): std::destroy_at(&m_data.s);break;
  case(kind::expression): std::destroy_at( m_data.e);break;
    }


  m_kind = kind::null;
}


sc_type_info
sc_operand::
type_info(sc_context&  ctx) const noexcept
{
  auto  v = evaluate(ctx);

  return v.type_info();
}


sc_value
sc_operand::
evaluate(sc_context&  ctx) const noexcept
{
    switch(m_kind)
    {
  case(kind::identifier      ): return sc_value(ctx.get_reference(m_data.s));break;
  case(kind::integer         ): return sc_value(m_data.i);break;
  case(kind::unsigned_integer): return sc_value(m_data.u);break;
  case(kind::floating        ): return sc_value(m_data.f);break;
  case(kind::expression      ): return m_data.e->evaluate(ctx);break;
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
  case(kind::integer         ): printf("%" PRIi64,m_data.i);break;
  case(kind::unsigned_integer): printf("%" PRIu64,m_data.u);break;
  case(kind::floating): printf("%f",m_data.f);break;
  case(kind::expression):
      printf("(");
      m_data.e->print();
      printf(")");
      break;
    }
}




}




