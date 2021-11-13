#include"libgbcc/vms/context.hpp"




namespace gbcc{




sc_operand&
sc_operand::
assign(const sc_operand&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind           = rhs.m_kind;
      m_type_info      = rhs.m_type_info;
      m_constant_value = clone_unique(rhs.m_constant_value);

        switch(m_kind)
        {
      case(kind::identifier):
      case(kind::symbol    ): new(&m_data) sc_symbol(rhs.m_data.sym);break;
      case(kind::expression): m_data.e = new sc_expression(*rhs.m_data.e);break;
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

      std::swap(m_kind          ,rhs.m_kind);
      std::swap(m_type_info     ,rhs.m_type_info);
      std::swap(m_constant_value,rhs.m_constant_value);

        switch(m_kind)
        {
      case(kind::identifier):
      case(kind::symbol    ): new(&m_data) sc_symbol(std::move(rhs.m_data.sym));break;
      case(kind::expression): std::swap(m_data.e,rhs.m_data.e);break;
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

  new(&m_data) sc_symbol(sv);

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

  m_kind = kind::literal;

  m_type_info      = ti;
  m_constant_value = std::make_unique<sc_value>(v);

  return *this;
}


void
sc_operand::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::identifier):
  case(kind::symbol    ): std::destroy_at(&m_data.sym);break;
  case(kind::expression): std::destroy_at( m_data.e);break;
    }


  m_kind = kind::null;

  m_type_info.clear();

  m_constant_value.reset();
}


void
sc_operand::
fix_for_symbol(const sc_symbol&  sym) noexcept
{
  m_kind = kind::symbol;

  m_data.sym = sym;

  m_type_info = make_reference(sym.type_info());

    if(!sym.attribute().has_temporary())
    {
      m_constant_value = std::make_unique<sc_value>(sym.offset());
    }
}


void
sc_operand::
fix_for_function(const sc_function&  fn) noexcept
{
  m_kind = kind::symbol;

  m_constant_value = std::make_unique<sc_value>(fn.entry_number());

  sc_type_info  ti(fn.signature());

  m_type_info = make_reference(ti);
}


void
sc_operand::
fix_for_identifier(const sc_function&  fn) noexcept
{
  auto&  name = m_data.sym.name();

  auto  sym = fn.find_symbol(name);

    if(sym)
    {
      fix_for_symbol(*sym);

      return;
    }


  auto  f = fn.package().find_function(name);
  
    if(f)
    {
      fix_for_function(*f);

      return;
    }


  printf("operand fix error: \"");

  gbcc::print(name);

  printf("\" is not found.");
}


void
sc_operand::
fix(const sc_function&  fn) noexcept
{
    if(is_literal())
    {
    }

  else
    if(is_identifier())
    {
      fix_for_identifier(fn);
    }

  else
    if(is_expression())
    {
      m_data.e->fix(fn);
    }
}


const sc_type_info&
sc_operand::
type_info() const noexcept
{
    if(is_expression())
    {
      return m_data.e->type_info();
    }


  return m_type_info;
}


const sc_value*
sc_operand::
constant_value() const noexcept
{
    if(is_expression())
    {
      return m_data.e->constant_value();
    }


  return m_constant_value.get();
}


sc_value
sc_operand::
evaluate(sc_context&  ctx) const noexcept
{
    if(m_constant_value)
    {
      return *m_constant_value;
    }


    switch(m_kind)
    {
  case(kind::expression): return m_data.e->evaluate(ctx);break;
  case(kind::symbol    ): return ctx.get_reference(m_data.sym).data();break;
    }


  return sc_value();
}


void
sc_operand::
print() const noexcept
{
    switch(m_kind)
    {
  case(kind::expression):
      printf("(");
      m_data.e->print();
      printf(")");
      break;
  case(kind::identifier):
      gbcc::print(m_data.sym.name());
      break;
  case(kind::symbol    ):
      gbcc::print(m_data.sym.name());
      printf(":{off ");
      gbcc::print(m_data.sym.offset());
      printf("}");
      break;
  case(kind::literal):
      m_constant_value->print(m_type_info);
      break;
    }
}




}




