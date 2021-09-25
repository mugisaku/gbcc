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

          push(v.type_info(),v.name());
        }
    }
}


void
sc_function::
push(const sc_type_info&  ti, std::u16string_view  name) noexcept
{
  m_symbol_list.emplace_back(name,sc_type_info(ti),stack_size(),sc_symbol_attribute().add_temporary());
}


void
sc_function::
update_symbol_list() noexcept
{
  m_symbol_list.clear();

    for(auto&  p: m_signature.parameter_list())
    {
      push(p.type_info(),p.name());
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




