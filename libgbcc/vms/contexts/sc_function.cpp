#include"libgbcc/vms/context.hpp"




namespace gbcc{




void
sc_function::
scan(const sc_block&  blk) noexcept
{
    for(auto&  st: blk.statement_list())
    {
        if(st.is_var())
        {
          auto&  v = st.var();

          push(v.type_info(),v.name(),sc_symbol_attribute().add_temporary());
        }

      else
        if(st.is_const())
        {
          auto&  c = st.const_();

          push(c.type_info(),c.name(),sc_symbol_attribute().add_temporary().add_const());
        }
    }
}


void
sc_function::
push(const sc_type_info&  ti, std::u16string_view  name, sc_symbol_attribute  attr) noexcept
{
  m_symbol_list.emplace_back(name,sc_type_info(ti),stack_size(),attr);
}


void
sc_function::
update_symbol_list() noexcept
{
  m_symbol_list.clear();

    for(auto&  p: m_signature.parameter_list())
    {
      push(p.type_info(),p.name(),sc_symbol_attribute().add_temporary());
    }


  scan(m_block);
}


int
sc_function::
stack_size() const noexcept
{
  return m_symbol_list.size()? m_symbol_list.back().next_offset():0;
}


void
sc_function::
print() const
{
  printf("function ");

  gbcc::print(m_name);

  printf("\n");

  m_signature.print();

  printf("\n");

    for(auto&  sym: m_symbol_list)
    {
      sym.print();

      printf("\n");
    }


  m_block.print();
}




}




