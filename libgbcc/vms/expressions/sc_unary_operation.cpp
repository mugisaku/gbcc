#include"libgbcc/vms/expression.hpp"




namespace gbcc{




sc_unary_operation&
sc_unary_operation::
assign(prefix_element_list&&  prels, sc_operand&&  o, postfix_element_list&&  postls) noexcept
{
  m_prefix_elements  = std::move( prels);
  m_postfix_elements = std::move(postls);

  m_operand = std::move(o);

  return *this;
}


sc_type_info
sc_unary_operation::
type_info(const sc_context&  ctx) const noexcept
{
  auto  ti = m_operand.type_info(ctx);

    for(auto&  e: m_postfix_elements)
    {
      ti = operate_type_info(ti,e);
    }


    for(auto&  e: m_prefix_elements)
    {
      ti = operate_type_info(ti,e);
    }


  return std::move(ti);
}


sc_value
sc_unary_operation::
evaluate(sc_context&  ctx) const noexcept
{
  auto  ti = m_operand.type_info(ctx);
  auto   v = m_operand.evaluate(ctx);

    for(auto&  e: m_postfix_elements)
    {
      v  = operate_value(v,ti,e,ctx);
      ti = operate_type_info(ti,e);
    }


    for(auto&  e: m_prefix_elements)
    {
      v  = operate_value(v,ti,e,ctx);
      ti = operate_type_info(ti,e);
    }


  return std::move(v);
}


void
sc_unary_operation::
print() const noexcept
{
    for(auto&  e: m_prefix_elements)
    {
      e.print();
    }


  m_operand.print();

    for(auto&  e: m_postfix_elements)
    {
      e.print();
    }
}




}




