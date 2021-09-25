#include"libgbcc/vms/expression.hpp"




namespace gbcc{




sc_expression_element&
sc_expression_element::
assign(const sc_expression_element&  rhs) noexcept
{
    if(this != &rhs)
    {
      m_operator  = rhs.m_operator ;
      m_operation = rhs.m_operation;
    }


  return *this;
}


sc_expression_element&
sc_expression_element::
assign(sc_expression_element&&  rhs) noexcept
{
    if(this != &rhs)
    {
      m_operator  = std::move(rhs.m_operator );
      m_operation = std::move(rhs.m_operation);
    }


  return *this;
}


sc_expression_element&
sc_expression_element::
assign(sc_unary_operation&&  opti) noexcept
{
  m_operator.clear();

  m_operation = std::move(opti);

  return *this;
}


sc_expression_element&
sc_expression_element::
assign(std::u16string_view  opto, sc_unary_operation&&  opti) noexcept
{
  m_operator = opto;

  m_operation = std::move(opti);

  return *this;
}


void
sc_expression_element::
print() const noexcept
{
  gbcc::print(m_operator);

  m_operation.print();
}




}




