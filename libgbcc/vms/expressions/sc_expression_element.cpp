#include"libgbcc/vms/expression.hpp"




namespace gbcc{




sc_expression_element&
sc_expression_element::
assign(sc_expression&&  expr) noexcept
{
  m_operator.clear();

  m_expression = std::move(expr);

  return *this;
}


sc_expression_element&
sc_expression_element::
assign(std::u16string_view  oprt) noexcept
{
  m_operator = oprt;

  m_expression.clear();

  return *this;
}




}




