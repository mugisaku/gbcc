#include"libgbcc/vms/context.hpp"




namespace gbcc{




void
sc_block::
reset(sc_function&  fn, std::string_view  k) noexcept
{
  m_function = &fn;

  m_kind = (k == "plain" )? kind::plain
          :(k == "if"    )? kind::if_
          :(k == "while" )? kind::while_
          :(k == "switch")? kind::switch_
          :kind::plain
          ;
}




const sc_statement*  sc_block::begin() const noexcept{return m_statement_list.data();}
const sc_statement*    sc_block::end() const noexcept{return begin()+m_statement_list.size();}


void
sc_block::
fix(const sc_function&  fn) noexcept
{
    for(auto&  st: m_statement_list)
    {
      st.fix(fn);
    }
}


void
sc_block::
print(int  depth) const noexcept
{
  gbcc::print_indent(depth);

       if(is_if()    ){  printf("if");  m_condition.print();}
  else if(is_while() ){  printf("while");  m_condition.print();}
  else if(is_switch()){  printf("switch");  m_condition.print();}

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




