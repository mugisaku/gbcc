#include"libgbcc/vms/context.hpp"




namespace gbcc{




const sc_statement*  sc_block::begin() const noexcept{return m_statement_list.data();}
const sc_statement*    sc_block::end() const noexcept{return begin()+m_statement_list.size();}


void
sc_block::
print(int  depth) const noexcept
{
  gbcc::print_indent(depth);

  printf("{\n");

    for(auto&  st: m_statement_list)
    {
      gbcc::print_indent(depth+1);

      st.print(depth+1);

      printf("\n");
    }


  gbcc::print_indent(depth);

  printf("}");
}




}




