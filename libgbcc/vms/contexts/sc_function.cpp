#include"libgbcc/vms/context.hpp"




namespace gbcc{




sc_function&
sc_function::
assign(sc_package&  pk, std::u16string_view  name, sc_type_info&&  ti, sc_parameter_list&&  parals, std::unique_ptr<sc_block>&&  blk) noexcept
{
  m_package = &pk;
  m_entry_number = pk.function_list().size();
  m_name = name;
  m_signature = sc_signature(std::move(ti),std::move(parals));
  m_block = std::move(blk);

  m_symbol_list.clear();

  return *this;
}


void
sc_function::
scan(sc_block&  blk) noexcept
{
    for(auto&  st: blk.statement_list())
    {
        if(st.is_block())
        {
          scan(st.block());
        }

      else
        if(st.is_if())
        {
            for(auto&  coblk: st.block_list())
            {
              scan(*coblk);
            }
        }

      else
        if(st.is_var())
        {
          auto&  v = st.variable();

          sc_symbol  sym(v.symbol().name(),sc_type_info(v.symbol().type_info()),sc_symbol_attribute().add_temporary(),stack_size());

          m_symbol_list.emplace_back(sym);
          v.symbol() = sym;
        }
    }
}


const sc_symbol*
sc_function::
find_symbol(std::u16string_view  name) const noexcept
{
    for(auto&  sym: m_symbol_list)
    {
        if(sym.name() == name)
        {
          return &sym;
        }
    }


  return m_package->find_symbol(name);
}


void
sc_function::
fix() noexcept
{
  m_symbol_list.clear();

    for(auto&  p: m_signature.parameter_list())
    {
      m_symbol_list.emplace_back(p.name(),sc_type_info(p.type_info()),sc_symbol_attribute().add_temporary(),p.offset());
    }


  scan(*m_block);

  m_block->fix(*this);
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


  m_block->print();
}




}




