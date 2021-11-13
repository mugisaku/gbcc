#include"libgbcc/vms/context.hpp"




namespace gbcc{




sc_function_execution_frame::
sc_function_execution_frame(const sc_function&  fn, int64_t  memsize) noexcept
{
  m_function = &fn;

  m_memory_size = memsize;

  m_bef_stack.emplace_back(fn.block());
}




void
sc_function_execution_frame::
process(const sc_statement&  st, sc_context&  ctx, sc_value_with_type_info&  ret_v) noexcept
{
       if(st.is_return()    ){process_return(st.expression().evaluate(ctx),st.expression().type_info(),ret_v,ctx);}
  else if(st.is_case()      ){}
  else if(st.is_default()   ){}
  else if(st.is_break()     ){process_break();}
  else if(st.is_continue()  ){process_continue();}
  else if(st.is_if()        ){process_if(st.block_list(),ctx);}
  else if(st.is_block()     ){process_block(st.block());}
  else if(st.is_expression()){st.expression().evaluate(ctx);}
  else if(st.is_label()     ){}
  else if(st.is_goto()      ){process_goto(st.string());}
  else if(st.is_var()       ){process_variable(st.variable(),ctx);}
}


void
sc_function_execution_frame::
process_return(sc_value  v, const sc_type_info&  ti, sc_value_with_type_info&  ret_v, sc_context&  ctx) noexcept
{
  auto&  ret_ti = m_function->signature().type_info();

  auto  cv = v.convert(ti,ret_ti,ctx);

  ret_v = sc_value_with_type_info(cv,ret_ti);

  m_status.set(flag::returned);
}


void
sc_function_execution_frame::
process_while(const sc_block&  blk, sc_context&  ctx) noexcept
{
    if(blk.condition().evaluate(ctx).integer())
    {
      m_bef_stack.emplace_back(blk);
    }
}


void
sc_function_execution_frame::
process_switch(const sc_block&  blk) noexcept
{
//  auto  v = cblk.expression().evaluate(*this);

    
}


void
sc_function_execution_frame::
process_goto(std::u16string_view  dst) noexcept
{
}


void
sc_function_execution_frame::
process_if(const sc_block_list&  blkls, sc_context&  ctx) noexcept
{
    for(auto&  blk: blkls)
    {
        if(blk->is_if())
        {
          auto  v = blk->condition().evaluate(ctx);

            if(v.integer())
            {
              process_block(*blk);
              
              break;
            }
        }

      else
        {
          process_block(*blk);
        }
   }
}


void
sc_function_execution_frame::
process_break() noexcept
{
    while(m_bef_stack.size())
    {
      bool  b = m_bef_stack.back().has_condition();

      m_bef_stack.pop_back();

        if(b)
        {
          return;
        }
    }
}


void
sc_function_execution_frame::
process_continue() noexcept
{
    while(m_bef_stack.size())
    {
        if(m_bef_stack.back().has_condition())
        {
          m_bef_stack.back().rewind();

          return;
        }

      else
        {
          m_bef_stack.pop_back();
        }
    }
}


void
sc_function_execution_frame::
process_block(const sc_block&  blk) noexcept
{
  m_bef_stack.emplace_back(blk);
}


void
sc_function_execution_frame::
process_variable(const sc_variable&  v, sc_context&  ctx) noexcept
{
    if(!v.symbol().attribute().has_const())
    {
      auto&  ti = v.expression().type_info();
      auto  val = v.expression().evaluate(ctx);

      ctx.store(v.symbol(),val,ti);
    }
}


void
sc_function_execution_frame::
operator()(sc_context&  ctx, sc_value_with_type_info&  ret_v) noexcept
{
    if(m_bef_stack.size())
    {
      auto&  frm = m_bef_stack.back();

        if(frm)
        {
          process(*frm,ctx,ret_v);
        }

      else
        {
            if(frm.test_condition(ctx))
            {
              frm.rewind();

              return;
            }


          m_bef_stack.pop_back();
        }
    }

  else
    {
      m_status.set(flag::returned);
    }
}




}




