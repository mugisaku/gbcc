#include"libgbcc/vm.hpp"




namespace gbcc{




vm_operation
vm_function::
construct_unary_operation(const syntax_branch&  br) noexcept
{
  auto  op = construct_unary_opcode(br[0].branch());

  auto  l = construct_operand(br[1].branch());

  return vm_operation(op,std::move(l));
}


vm_operation
vm_function::
construct_binary_operation(const syntax_branch&  br) noexcept
{
  auto  op = construct_binary_opcode(br[0].branch());

  auto  l = construct_operand(br[1].branch());
  auto  r = construct_operand(br[2].branch());

  return vm_operation(op,std::move(l),std::move(r));
}


vm_opcode
vm_function::
construct_unary_opcode(const syntax_branch&  br) noexcept
{
  auto  s = br[0].string();

  return (s == u"logi_not")? vm_opcode::logi_not
        :(s == u"not")? vm_opcode::not_
        :(s == u"neg")? vm_opcode::neg
        :(s == u"fneg")? vm_opcode::fneg
        :vm_opcode::nop
        ;
}


vm_opcode
vm_function::
construct_binary_opcode(const syntax_branch&  br) noexcept
{
  auto  s = br[0].string();

  return (s == u"add")? vm_opcode::add
        :(s == u"sub")? vm_opcode::sub
        :(s == u"mul")? vm_opcode::mul
        :(s == u"div")? vm_opcode::div
        :(s == u"rem")? vm_opcode::rem
        :(s == u"fadd")? vm_opcode::fadd
        :(s == u"fsub")? vm_opcode::fsub
        :(s == u"fmul")? vm_opcode::fmul
        :(s == u"fdiv")? vm_opcode::fdiv
        :(s == u"frem")? vm_opcode::frem
        :(s == u"eq")? vm_opcode::eq
        :(s == u"neq")? vm_opcode::neq
        :(s == u"lt")? vm_opcode::lt
        :(s == u"lteq")? vm_opcode::lteq
        :(s == u"gt")? vm_opcode::gt
        :(s == u"gteq")? vm_opcode::gteq
        :(s == u"feq")? vm_opcode::feq
        :(s == u"fneq")? vm_opcode::fneq
        :(s == u"flt")? vm_opcode::flt
        :(s == u"flteq")? vm_opcode::flteq
        :(s == u"fgt")? vm_opcode::fgt
        :(s == u"fgteq")? vm_opcode::fgteq
        :(s == u"shl")? vm_opcode::shl
        :(s == u"shr")? vm_opcode::shr
        :(s == u"and")? vm_opcode::and_
        :(s == u"or")? vm_opcode::or_
        :(s == u"xor")? vm_opcode::xor_
        :(s == u"logi_and")? vm_opcode::logi_and
        :(s == u"logi_or")? vm_opcode::logi_or
        :vm_opcode::nop
        ;
}


vm_address_operation
vm_function::
construct_address_operation(const syntax_branch&  br) noexcept
{
  return vm_address_operation(br[1].string());
}


vm_load
vm_function::
construct_load(const syntax_branch&  br) noexcept
{
  auto  ti = construct_type_info(br[1].branch());

  auto  s = br[2].string();

  return vm_load(ti,s);
}


vm_register
vm_function::
construct_operations(const syntax_branch&  br, vm_type_info  ti, std::u16string_view  lb) noexcept
{
  auto&  e = br[0];

       if(e.is_branch(u"unary_operation")   ){return vm_register(ti,lb,construct_unary_operation(  e.branch()));}
  else if(e.is_branch(u"binary_operation")  ){return vm_register(ti,lb,construct_binary_operation( e.branch()));}
  else if(e.is_branch(u"address_operation") ){return vm_register(ti,lb,construct_address_operation(e.branch()));}
  else if(e.is_branch(u"load_operation")    ){return vm_register(ti,lb,construct_load(e.branch()));}
  else if(e.is_branch(u"allocate_operation")){return vm_register(ti,lb);}
  else{}

  return vm_register();
}


vm_register
vm_function::
construct_register(const syntax_branch&  br) noexcept
{
  auto  ti = construct_type_info(br[1].branch());

  auto  lb = br[2].string();

  auto&  e = br[3];

       if(e.is_branch(u"operation")     ){return construct_operations(e.branch(),ti,lb);}
  else if(e.is_branch(u"call_statement")){return vm_register(ti,lb,construct_call(e.branch()));}

  return vm_register();
}


vm_return
vm_function::
construct_return(const syntax_branch&  br) noexcept
{
    if(br.length() == 2)
    {
      return vm_return(construct_operand(br[1].branch()));
    }


  return vm_return();
}


vm_branch
vm_function::
construct_branch(const syntax_branch&  br) noexcept
{
  auto  o = construct_operand(br[1].branch());

  auto   if_true = br[2].string();
  auto  if_false = br[3].string();

  return vm_branch(std::move(o),if_true,if_false);
}


vm_jump
vm_function::
construct_jump(const syntax_branch&  br) noexcept
{
  return vm_jump(br[1].string());
}


vm_store
vm_function::
construct_store(const syntax_branch&  br) noexcept
{
  auto  o = construct_operand(br[1].branch());

  auto  ti = construct_type_info(br[2].branch());

  auto  s = br[3].string();

  return vm_store(std::move(o),ti,s);
}


vm_operand_list
vm_function::
construct_operand_list(const syntax_branch&  br) noexcept
{
  vm_operand_list  ls;

    for(auto&  e: br)
    {
        if(e.is_branch(u"operand"))
        {
          ls.emplace_back(construct_operand(e.branch()));
        }
    }


  return std::move(ls);
}


vm_call
vm_function::
construct_call(const syntax_branch&  br) noexcept
{
  return vm_call(br[1].string(),construct_operand_list(br[3].branch()));
}


vm_parameter
vm_function::
construct_parameter(const syntax_branch&  br) noexcept
{
  auto  ti = construct_type_info(br[0].branch());

  return vm_parameter(ti,br[1].string());
}


vm_parameter_list
vm_function::
construct_parameter_list(const syntax_branch&  br) noexcept
{
  vm_parameter_list  ls;

    for(auto&  e: br)
    {
        if(e.is_branch(u"parameter"))
        {
          ls.emplace_back(construct_parameter(e.branch()));
        }
    }


  return std::move(ls);
}


vm_type_info
vm_function::
construct_type_info(const syntax_branch&  br) noexcept
{
  auto  s = br[0].string();

  return (s == u"i8")?  vm_type_info('i',1)
        :(s == u"i16")? vm_type_info('i',2)
        :(s == u"i32")? vm_type_info('i',4)
        :(s == u"i64")? vm_type_info('i',8)
        :vm_type_info()
        ;
}


vm_operand
vm_function::
construct_operand(const syntax_branch&  br) noexcept
{
  auto&  tok = br[0].token();

  return tok.is_integer()?    vm_operand(tok.integer())
        :tok.is_floating()?   vm_operand(tok.floating())
        :tok.is_identifier()? vm_operand(tok.string())
        :vm_operand()
        ;
}


vm_line
vm_function::
construct_statement(const syntax_branch&  br) noexcept
{
  auto&  e = br[0];

       if(e.is_branch(u"register_statement")){return vm_line(construct_register(e.branch()));}
  else if(e.is_branch(u"return_statement")  ){return vm_line(construct_return(e.branch()));}
  else if(e.is_branch(u"branch_statement")  ){return vm_line(construct_branch(e.branch()));}
  else if(e.is_branch(u"jump_statement")    ){return vm_line(construct_jump(e.branch()));}
  else if(e.is_branch(u"store_statement")   ){return vm_line(construct_store(e.branch()));}
  else if(e.is_branch(u"call_statement")    ){return vm_line(construct_call(e.branch()));}
  else{;}

  return vm_line();
}


vm_line_list
vm_function::
construct_statement_list(const syntax_branch&  br) noexcept
{
  vm_line_list  ls;

    for(auto&  e: br)
    {
        if(e.is_branch(u"statement"))
        {
          ls.emplace_back(construct_statement(e.branch()));
        }
    }


  return std::move(ls);
}


vm_block
vm_function::
construct_block(const syntax_branch&  br) noexcept
{
  auto  lb = br[0].string();

  auto  ls = construct_statement_list(br[2].branch());

  return vm_block(lb,std::move(ls));
}


vm_block_list
vm_function::
construct_block_list(const syntax_branch&  br) noexcept
{
  vm_block_list  ls;

    for(auto&  e: br)
    {
        if(e.is_branch(u"block"))
        {
          ls.emplace_back(construct_block(e.branch()));
        }
    }


  return std::move(ls);
}




}




