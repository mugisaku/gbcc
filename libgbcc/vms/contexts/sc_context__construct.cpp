#include"libgbcc/vms/context.hpp"




namespace gbcc{




sc_return
sc_context::
construct_return(const syntax_branch&  br) noexcept
{
  return (br.length() == 1)? sc_return()
        :                    sc_return(sc_expression(br[1].branch()))
        ;
}


sc_for
sc_context::
construct_for(const syntax_branch&  br) noexcept
{
  return sc_for();
}


sc_while
sc_context::
construct_while(const syntax_branch&  br) noexcept
{
  return sc_while(sc_expression(br[1].branch()),construct_block(br[2].branch()));
}


sc_switch
sc_context::
construct_switch(const syntax_branch&  br) noexcept
{
  return sc_switch(sc_expression(br[1].branch()),construct_block(br[2].branch()));
}


sc_case
sc_context::
construct_case(const syntax_branch&  br) noexcept
{
  return sc_case(sc_expression(br[1].branch()));
}


sc_if
sc_context::
construct_if(const syntax_branch&  br) noexcept
{
  sc_conditional_block_list  ls;

  ls.emplace_back(sc_expression(br[1].branch()),construct_block(br[2].branch()));

  auto  end_it = br.end();

    for(auto  it = br.begin()+3;  it != end_it;  ++it)
    {
        if(it->is_branch(u"else_if_statement"))
        {
          auto&  brbr = it->branch();

          ls.emplace_back(sc_expression(brbr[2].branch()),construct_block(brbr[3].branch()));
        }

      else
        if(it->is_branch(u"else_statement"))
        {
          auto&  brbr = it->branch();

          ls.emplace_back(sc_expression(),construct_block(brbr[1].branch()));
        }
    }


  return sc_if(std::move(ls));
}


sc_label
sc_context::
construct_label(const syntax_branch&  br) noexcept
{
  return sc_label(br[1].token().string());
}


sc_goto
sc_context::
construct_goto(const syntax_branch&  br) noexcept
{
  return sc_goto(br[1].token().string());
}


sc_var
sc_context::
construct_var(const syntax_branch&  br) noexcept
{
  return (br.length() == 4)? sc_var(br[1].token().string(),construct_type_info(br[3].branch()),sc_expression())
        :(br.length() == 7)? sc_var(br[1].token().string(),construct_type_info(br[3].branch()),sc_expression())
        :                    sc_var(br[1].token().string(),construct_type_info(br[3].branch()),sc_expression(br[5].branch()))
        ;
}


sc_const
sc_context::
construct_const(const syntax_branch&  br) noexcept
{
  return static_cast<sc_const&&>(construct_var(br));
}


sc_block
sc_context::
construct_block(const syntax_branch&  br) noexcept
{
  sc_statement_list  ls;

    for(auto&  e: br)
    {
        if(e.is_branch(u"statement"))
        {
          auto&  stbr = e.branch();

          auto&  ee = stbr[0];

               if(ee.is_branch(u"return_statement")  ){ls.emplace_back(construct_return(ee.branch()));}
          else if(ee.is_branch(u"for_statement")     ){ls.emplace_back(construct_for(   ee.branch()));}
          else if(ee.is_branch(u"while_statement")   ){ls.emplace_back(construct_while( ee.branch()));}
          else if(ee.is_branch(u"if_statement")      ){ls.emplace_back(construct_if(    ee.branch()));}
          else if(ee.is_branch(u"switch_statement")  ){ls.emplace_back(construct_switch(ee.branch()));}
          else if(ee.is_branch(u"goto_statement")    ){ls.emplace_back(construct_goto(  ee.branch()));}
          else if(ee.is_branch(u"label_statement")   ){ls.emplace_back(construct_label( ee.branch()));}
          else if(ee.is_branch(u"case_statement")    ){ls.emplace_back(construct_case(  ee.branch()));}
          else if(ee.is_branch(u"block_statement")   ){ls.emplace_back(construct_block( ee.branch()));}
          else if(ee.is_branch(u"var_statement")     ){ls.emplace_back(construct_var(   ee.branch()));}
          else if(ee.is_branch(u"const_statement")   ){ls.emplace_back(construct_const( ee.branch()));}
          else if(ee.is_branch(u"break_statement")   ){ls.emplace_back(sc_break());}
          else if(ee.is_branch(u"default_statement") ){ls.emplace_back(sc_default());}
          else if(ee.is_branch(u"continue_statement")){ls.emplace_back(sc_continue());}
          else if(ee.is_branch(u"expression")        ){ls.emplace_back(sc_expression(ee.branch()));}
        }
    }


  return sc_block(std::move(ls));
}


sc_parameter
sc_context::
construct_parameter(const syntax_branch&  br) noexcept
{
    if(br[0].is_token())
    {
      return sc_parameter(br[0].token().string(),construct_type_info(br[2].branch()));
    }


  return sc_parameter(construct_type_info(br[0].branch()));
}


sc_parameter_list
sc_context::
construct_parameter_list(const syntax_branch&  br) noexcept
{
  sc_parameter_list  ls;

    for(auto&  e: br)
    {
        if(e.is_branch(u"parameter"))
        {
          ls.emplace_back(construct_parameter(e.branch()));
        }
    }


  return std::move(ls);
}


sc_type_info
sc_context::
construct_type_info(const syntax_branch&  br) noexcept
{
  sc_type_info  ti;

    for(auto&  e: br)
    {
        if(e.is_token())
        {
          auto&  tok = e.token();

            if(tok.is_identifier())
            {
              std::u16string_view  sv = tok.string();

              ti = (sv == u"nullptr_t")? sc_nullptr_ti
                  :(sv == u"undef")? sc_undef_ti
                  :(sv == u"void")? sc_void_ti
                  :(sv == u"int8")? sc_int8_ti
                  :(sv == u"int16")? sc_int16_ti
                  :(sv == u"int32")? sc_int32_ti
                  :(sv == u"int64")? sc_int64_ti
                  :(sv == u"int")? sc_int64_ti
                  :(sv == u"uint8")? sc_uint8_ti
                  :(sv == u"uint16")? sc_uint16_ti
                  :(sv == u"uint32")? sc_uint32_ti
                  :(sv == u"uint64")? sc_uint64_ti
                  :(sv == u"uint")? sc_uint64_ti
                  :(sv == u"float32")? sc_float32_ti
                  :(sv == u"float64")? sc_float64_ti
                  :(sv == u"float")? sc_float64_ti
                  :(sv == u"bool8")? sc_bool8_ti
                  :(sv == u"bool16")? sc_bool16_ti
                  :(sv == u"bool32")? sc_bool32_ti
                  :(sv == u"bool64")? sc_bool64_ti
                  :(sv == u"bool")? sc_bool64_ti
                  :sc_undef_ti
                  ;
            }
        }

      else
        if(e.is_string())
        {
          auto&  s = e.string();

            if(s == u"*")
            {
              ti = make_pointer(ti);
            }

          else
            if(s == u"&")
            {
              ti = make_reference(ti);
            }
        }

      else
        if(e.is_branch(u"index"))
        {
          sc_expression  expr(e.branch()[1].branch());

          expr.print();

          ti = make_array(ti,0);
        }
    }


  return std::move(ti);
}


sc_function
sc_context::
construct_function(const syntax_branch&  br) noexcept
{
  auto&  name = br[1].token().string();

  auto  ti = construct_type_info(br[2].branch());

  auto  parals = construct_parameter_list(br[3].branch());

  auto  blk = construct_block(br[4].branch());

  return sc_function(name,std::move(ti),std::move(parals),std::move(blk));
}




}




