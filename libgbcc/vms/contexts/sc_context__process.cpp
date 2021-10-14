#include"libgbcc/vms/context.hpp"




namespace gbcc{




void
sc_context::
process(const sc_statement&  st) noexcept
{
       if(st.is_return()    ){process_return(st.expression().evaluate(*this),st.expression().type_info(*this));}
  else if(st.is_while()     ){process_while(st.conditional_block());}
  else if(st.is_for()       ){}
  else if(st.is_switch()    ){process_switch(st.conditional_block());}
  else if(st.is_case()      ){}
  else if(st.is_default()   ){}
  else if(st.is_break()     ){process_break();}
  else if(st.is_continue()  ){process_continue();}
  else if(st.is_if()        ){process_if(st.conditional_block_list());}
  else if(st.is_block()     ){process_block(st.block());}
  else if(st.is_expression()){st.expression().evaluate(*this);}
  else if(st.is_label()     ){}
  else if(st.is_goto()      ){process_goto(st.string());}
  else if(st.is_var()       ){process_var(st.var()   );}
  else if(st.is_const()     ){process_const(st.const_());}
}


void
sc_context::
process_return(sc_value  v, const sc_type_info&  ti) noexcept
{
  auto&  frm = m_frame_stack.back();

  auto&  ret_ti = frm.m_function->signature().type_info();

  auto  cv = v.convert(ti,ret_ti,*this);

  m_returned_value = sc_value_with_type_info(cv,ret_ti);

  m_memory.resize(frm.m_memory_size);

  m_frame_stack.pop_back();
}


void
sc_context::
process_while(const sc_conditional_block&  cblk) noexcept
{
  m_frame_stack.back().m_running_stack.emplace_back(cblk);
}


//  void  process_for() noexcept;
void
sc_context::
process_switch(const sc_conditional_block&  cblk) noexcept
{
  auto  v = cblk.expression().evaluate(*this);

    
}


void
sc_context::
process_goto(std::u16string_view  dst) noexcept
{
}


void
sc_context::
process_if(const sc_conditional_block_list&  cblkls) noexcept
{
    for(auto&  cblk: cblkls)
    {
      auto&  e = cblk.expression();

        if(e)
        {
          auto  v = e.evaluate(*this);

            if(v.integer())
            {
              process_block(cblk);
              
              break;
            }
        }

      else
        {
          process_block(cblk);
        }
   }
}


void
sc_context::
process_break() noexcept
{
  auto&  stk = m_frame_stack.back().m_running_stack;

    while(stk.size())
    {
      bool  b = stk.back().m_condition;

      stk.pop_back();

        if(b)
        {
          return;
        }
    }
}


void
sc_context::
process_continue() noexcept
{
  auto&  stk = m_frame_stack.back().m_running_stack;

    while(stk.size())
    {
        if(stk.back().m_condition)
        {
          stk.back().rewind();

          return;
        }

      else
        {
          stk.pop_back();
        }
    }
}


void
sc_context::
process_block(const sc_block&  blk) noexcept
{
  m_frame_stack.back().m_running_stack.emplace_back(blk);
}


void
sc_context::
process_var(const sc_var&  v) noexcept
{
  auto  sym = find_symbol(v.name());

    if(sym && !sym->attribute().has_const())
    {
      auto   ti = v.expression().type_info(*this);
      auto  val = v.expression().evaluate(*this);

      store(*sym,val,ti);
    }
}


void
sc_context::
process_const(const sc_const&  c) noexcept
{
}




}




