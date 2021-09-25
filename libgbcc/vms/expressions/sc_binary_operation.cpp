#include"libgbcc/vms/context.hpp"




namespace gbcc{




sc_binary_operation&
sc_binary_operation::
assign(const sc_binary_operation&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_operator = rhs.m_operator;

      m_left  = rhs.m_left ;
      m_right = rhs.m_right;
    }


  return *this;
}


sc_binary_operation&
sc_binary_operation::
assign(sc_binary_operation&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_operator,rhs.m_operator);

      std::swap(m_left ,rhs.m_left );
      std::swap(m_right,rhs.m_right);
    }


  return *this;
}


sc_binary_operation&
sc_binary_operation::
assign(std::u16string_view  o, sc_expression&&  l, sc_expression&&  r) noexcept
{
  m_operator = o;

  m_left  = std::move(l);
  m_right = std::move(r);

  return *this;
}


void
sc_binary_operation::
clear() noexcept
{
  m_operator.clear();

  m_left.clear();
  m_right.clear();
}




sc_value
sc_binary_operation::
evaluate(sc_context&  ctx) const noexcept
{
  auto  l = m_left.evaluate(ctx);
  auto  r = m_right.evaluate(ctx);

  functor  f(ctx.dereference(l),ctx.dereference(r));

       if(m_operator == u"="){return operate_assign(l,f.right(),ctx);}
  else if(m_operator == u"+="){return operate_assign(l,f.operate_add(),ctx);}
  else if(m_operator == u"-="){return operate_assign(l,f.operate_sub(),ctx);}
  else if(m_operator == u"*="){return operate_assign(l,f.operate_mul(),ctx);}
  else if(m_operator == u"/="){return operate_assign(l,f.operate_div(),ctx);}
  else if(m_operator == u"%="){return operate_assign(l,f.operate_rem(),ctx);}
  else if(m_operator == u"<<="){return operate_assign(l,f.operate_shl(),ctx);}
  else if(m_operator == u">>="){return operate_assign(l,f.operate_shr(),ctx);}
  else if(m_operator == u"&="){return operate_assign(l,f.operate_and(),ctx);}
  else if(m_operator == u"|="){return operate_assign(l,f.operate_or(),ctx);}
  else if(m_operator == u"^="){return operate_assign(l,f.operate_xor(),ctx);}
  else if(m_operator == u"+"){return f.operate_add();}
  else if(m_operator == u"-"){return f.operate_sub();}
  else if(m_operator == u"*"){return f.operate_mul();}
  else if(m_operator == u"/"){return f.operate_div();}
  else if(m_operator == u"%"){return f.operate_rem();}
  else if(m_operator == u"<<"){return f.operate_shl();}
  else if(m_operator == u">>"){return f.operate_shr();}
  else if(m_operator == u"|"){return f.operate_or();}
  else if(m_operator == u"&"){return f.operate_and();}
  else if(m_operator == u"^"){return f.operate_xor();}
  else if(m_operator == u"&&"){return f.operate_logical_and();}
  else if(m_operator == u"||"){return f.operate_logical_or();}
  else if(m_operator == u"=="){return f.operate_eq();}
  else if(m_operator == u"!="){return f.operate_neq();}
  else if(m_operator == u"<" ){return f.operate_lt();}
  else if(m_operator == u"<="){return f.operate_lteq();}
  else if(m_operator == u">" ){return f.operate_gt();}
  else if(m_operator == u">="){return f.operate_gteq();}


  return sc_value();
}


sc_type_info
sc_binary_operation::
type_info(sc_context&  ctx) const noexcept
{
  auto  v = evaluate(ctx);

  return v.type_info();
}


void
sc_binary_operation::
print() const noexcept
{
  m_left.print();

  gbcc::print(m_operator);

  m_right.print();
}




}




