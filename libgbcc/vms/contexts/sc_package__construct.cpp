#include"libgbcc/vms/context.hpp"




namespace gbcc{




sc_return
sc_package::
construct_return(const syntax_branch&  br) noexcept
{
  return (br.length() == 1)? sc_return()
        :                    sc_return(sc_expression(br[1].branch()))
        ;
}


std::unique_ptr<sc_block>
sc_package::
construct_while(const syntax_branch&  br, sc_function&  fn) noexcept
{
  auto  blk = construct_block(br[2].branch(),fn,"while");

  blk->add_condition(sc_expression(br[1].branch()));

  return std::move(blk);
}


std::unique_ptr<sc_block>
sc_package::
construct_switch(const syntax_branch&  br, sc_function&  fn) noexcept
{
  auto  blk = construct_block(br[2].branch(),fn,"switch");

  blk->add_condition(sc_expression(br[1].branch()));

  return std::move(blk);
}


sc_case
sc_package::
construct_case(const syntax_branch&  br) noexcept
{
  return sc_case(sc_expression(br[1].branch()));
}


sc_block_list
sc_package::
construct_if(const syntax_branch&  br, sc_function&  fn) noexcept
{
  sc_block_list  ls;

  auto  first_blk = construct_block(br[2].branch(),fn,"if");

  first_blk->add_condition(sc_expression(br[1].branch()));

  ls.emplace_back(std::move(first_blk));

  auto  end_it = br.end();

    for(auto  it = br.begin()+3;  it != end_it;  ++it)
    {
        if(it->is_branch(u"else_if_statement"))
        {
          auto&  brbr = it->branch();

          auto  blk = construct_block(brbr[2].branch(),fn,"if");

          blk->add_condition(sc_expression(brbr[1].branch()));

          ls.emplace_back(std::move(blk));
        }

      else
        if(it->is_branch(u"else_statement"))
        {
          auto&  brbr = it->branch();

          ls.emplace_back(construct_block(brbr[1].branch(),fn,"plain"));
        }
    }


  return std::move(ls);
}


sc_label
sc_package::
construct_label(const syntax_branch&  br) noexcept
{
  return sc_label(br[1].token().string());
}


sc_goto
sc_package::
construct_goto(const syntax_branch&  br) noexcept
{
  return sc_goto(br[1].token().string());
}


sc_variable
sc_package::
construct_variable(const syntax_branch&  br) noexcept
{
  sc_symbol  sym(br[1].token().string(),construct_type_info(br[3].branch()));

  return (br.length() == 4)? sc_variable(std::move(sym)                              )
        :                    sc_variable(std::move(sym),sc_expression(br[5].branch()))
        ;
}


std::unique_ptr<sc_block>
sc_package::
construct_block(const syntax_branch&  br, sc_function&  fn, std::string_view  k) noexcept
{
  auto  blk = std::make_unique<sc_block>();

  blk->reset(fn,k);

    for(auto&  e: br)
    {
        if(e.is_branch(u"statement"))
        {
          auto&  stbr = e.branch();

          auto&  ee = stbr[0];

               if(ee.is_branch(u"return_statement")  ){blk->emplace_back(construct_return(ee.branch()));}
          else if(ee.is_branch(u"while_statement")   ){blk->emplace_back(construct_while( ee.branch(),fn));}
          else if(ee.is_branch(u"if_statement")      ){blk->emplace_back(construct_if(    ee.branch(),fn));}
          else if(ee.is_branch(u"switch_statement")  ){blk->emplace_back(construct_switch(ee.branch(),fn));}
          else if(ee.is_branch(u"goto_statement")    ){blk->emplace_back(construct_goto(  ee.branch()));}
          else if(ee.is_branch(u"label_statement")   ){blk->emplace_back(construct_label( ee.branch()));}
          else if(ee.is_branch(u"case_statement")    ){blk->emplace_back(construct_case(  ee.branch()));}
          else if(ee.is_branch(u"block_statement")   ){blk->emplace_back(construct_block( ee.branch(),fn,"plain"));}
          else if(ee.is_branch(u"var_statement")     ){blk->emplace_back(construct_variable(   ee.branch()));}
          else if(ee.is_branch(u"const_statement")   ){blk->emplace_back(construct_variable( ee.branch()));}
          else if(ee.is_branch(u"break_statement")   ){blk->emplace_back(sc_break());}
          else if(ee.is_branch(u"default_statement") ){blk->emplace_back(sc_default());}
          else if(ee.is_branch(u"continue_statement")){blk->emplace_back(sc_continue());}
          else if(ee.is_branch(u"expression")        ){blk->emplace_back(sc_expression(ee.branch()));}
        }
    }


  return std::move(blk);
}


sc_parameter
sc_package::
construct_parameter(const syntax_branch&  br, int64_t  off) noexcept
{
  auto  ti = construct_type_info(br[2].branch());

  auto  a = ti.align();

    if(a)
    {
      off = (off+a-1)/a*a;
    }


  return sc_parameter(br[0].token().string(),std::move(ti),off);
}


sc_parameter_list
sc_package::
construct_parameter_list(const syntax_branch&  br) noexcept
{
  sc_parameter_list  ls;

  int64_t  off = 0;

    for(auto&  e: br)
    {
        if(e.is_branch(u"parameter"))
        {
          auto&  p = ls.emplace_back(construct_parameter(e.branch(),off));

          off += p.type_info().size();
        }
    }


  return std::move(ls);
}


sc_type_info
sc_package::
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

//          auto  vti = expr.type_info(*this);
//          auto    v = expr.evaluate(*this);
//v.print(vti);
          ti = make_array(ti,0);
        }
    }


  return std::move(ti);
}


std::unique_ptr<sc_function>
sc_package::
construct_function(const syntax_branch&  br, sc_package&  pk) noexcept
{
  auto  fn = std::make_unique<sc_function>();

  auto&  name = br[1].token().string();

  auto  ti = construct_type_info(br[2].branch());

  auto  parals = construct_parameter_list(br[3].branch());

  auto  blk = construct_block(br[4].branch(),*fn,"plain");

  fn->assign(pk,name,std::move(ti),std::move(parals),std::move(blk));

  return std::move(fn);
}




}




