#include"libgbcc/vms/context.hpp"




namespace gbcc{




sc_binary_operation&
sc_binary_operation::
assign(const sc_binary_operation&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_left  = rhs.m_left ;
      m_right = rhs.m_right;

      m_operator = rhs.m_operator;
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

      std::swap(m_left ,rhs.m_left );
      std::swap(m_right,rhs.m_right);

      std::swap(m_operator,rhs.m_operator);
    }


  return *this;
}


sc_binary_operation&
sc_binary_operation::
assign(sc_expression&&  l, sc_expression&&  r, std::u16string_view  o) noexcept
{
  m_left  = std::move(l);
  m_right = std::move(r);

  m_operator = o;

  return *this;
}


void
sc_binary_operation::
clear() noexcept
{
  m_left.clear();
  m_right.clear();

  m_operator.clear();
}




sc_binary_operation::functor::
functor(sc_value  l, const sc_type_info&  lti, sc_value  r, const sc_type_info&  rti, sc_context&  ctx) noexcept:
m_l_value(ctx.dereference(l,lti)), m_l_type_info(remove_reference(lti)),
m_r_value(ctx.dereference(r,rti)), m_r_type_info(remove_reference(rti))
{
}


sc_value
sc_binary_operation::
evaluate(sc_context&  ctx) const noexcept
{
  auto  lti =  m_left.type_info(ctx);
  auto  rti = m_right.type_info(ctx);

  auto  l = m_left.evaluate(ctx);
  auto  r = m_right.evaluate(ctx);

  functor  f(l,lti,r,rti,ctx);

       if(m_operator == u"=" ){return operate_assign(l,lti,f.operate_rv(),ctx);}
  else if(m_operator == u"+="){return operate_assign(l,lti,f.operate_add(),ctx);}
  else if(m_operator == u"-="){return operate_assign(l,lti,f.operate_sub(),ctx);}
  else if(m_operator == u"*="){return operate_assign(l,lti,f.operate_mul(),ctx);}
  else if(m_operator == u"/="){return operate_assign(l,lti,f.operate_div(),ctx);}
  else if(m_operator == u"%="){return operate_assign(l,lti,f.operate_rem(),ctx);}
  else if(m_operator == u"<<="){return operate_assign(l,lti,f.operate_shl(),ctx);}
  else if(m_operator == u">>="){return operate_assign(l,lti,f.operate_shr(),ctx);}
  else if(m_operator == u"&="){return operate_assign(l,lti,f.operate_and(),ctx);}
  else if(m_operator == u"|="){return operate_assign(l,lti,f.operate_or(),ctx);}
  else if(m_operator == u"^="){return operate_assign(l,lti,f.operate_xor(),ctx);}
  else if(m_operator == u"+"){return f.operate_add().result_value();}
  else if(m_operator == u"-"){return f.operate_sub().result_value();}
  else if(m_operator == u"*"){return f.operate_mul().result_value();}
  else if(m_operator == u"/"){return f.operate_div().result_value();}
  else if(m_operator == u"%"){return f.operate_rem().result_value();}
  else if(m_operator == u"<<"){return f.operate_shl().result_value();}
  else if(m_operator == u">>"){return f.operate_shr().result_value();}
  else if(m_operator == u"|"){return f.operate_or().result_value();}
  else if(m_operator == u"&"){return f.operate_and().result_value();}
  else if(m_operator == u"^"){return f.operate_xor().result_value();}
  else if(m_operator == u"&&"){return f.operate_logical_and().result_value();}
  else if(m_operator == u"||"){return f.operate_logical_or().result_value();}
  else if(m_operator == u"=="){return f.operate_eq().result_value();}
  else if(m_operator == u"!="){return f.operate_neq().result_value();}
  else if(m_operator == u"<" ){return f.operate_lt().result_value();}
  else if(m_operator == u"<="){return f.operate_lteq().result_value();}
  else if(m_operator == u">" ){return f.operate_gt().result_value();}
  else if(m_operator == u">="){return f.operate_gteq().result_value();}


  return sc_value();
}


namespace{
sc_type_info
result(const sc_type_info&  l, const sc_type_info&  r) noexcept
{
  auto  deref_l = remove_reference(l);
  auto  deref_r = remove_reference(r);

       if(deref_l.is_floating() || deref_r.is_floating()){return sc_float64_ti;}
  else if(deref_l.is_pointer()){return deref_l;}
  else if(deref_r.is_pointer()){return deref_r;}
  else if(deref_l.is_unsigned_integer() || deref_r.is_unsigned_integer()){return sc_uint64_ti;}

  return sc_int64_ti;
}
}


sc_type_info
sc_binary_operation::
type_info(const sc_context&  ctx) const noexcept
{
  auto  l =  m_left.type_info(ctx);
  auto  r = m_right.type_info(ctx);

       if(m_operator == u"=" ){return l;}
  else if(m_operator == u"+="){return l;}
  else if(m_operator == u"-="){return l;}
  else if(m_operator == u"*="){return l;}
  else if(m_operator == u"/="){return l;}
  else if(m_operator == u"%="){return l;}
  else if(m_operator == u"<<="){return l;}
  else if(m_operator == u">>="){return l;}
  else if(m_operator == u"&="){return l;}
  else if(m_operator == u"|="){return l;}
  else if(m_operator == u"^="){return l;}
  else if(m_operator == u"+"){return result(l,r);}
  else if(m_operator == u"-"){return result(l,r);}
  else if(m_operator == u"*"){return result(l,r);}
  else if(m_operator == u"/"){return result(l,r);}
  else if(m_operator == u"%"){return result(l,r);}
  else if(m_operator == u"<<"){return result(l,r);}
  else if(m_operator == u">>"){return result(l,r);}
  else if(m_operator == u"|"){return result(l,r);}
  else if(m_operator == u"&"){return result(l,r);}
  else if(m_operator == u"^"){return result(l,r);}
  else if(m_operator == u"&&"){return sc_bool64_ti;}
  else if(m_operator == u"||"){return sc_bool64_ti;}
  else if(m_operator == u"=="){return sc_bool64_ti;}
  else if(m_operator == u"!="){return sc_bool64_ti;}
  else if(m_operator == u"<" ){return sc_bool64_ti;}
  else if(m_operator == u"<="){return sc_bool64_ti;}
  else if(m_operator == u">" ){return sc_bool64_ti;}
  else if(m_operator == u">="){return sc_bool64_ti;}

  return sc_type_info();
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




