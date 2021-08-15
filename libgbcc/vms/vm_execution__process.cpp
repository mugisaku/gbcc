#include"libgbcc/vm.hpp"




namespace gbcc{




void
vm_execution::
process(const vm_return&  ret) noexcept
{
    if(m_function_stack.empty())
    {
      printf("function stack is empty.\n");

      halt();

      return;
    }


  m_sp = m_memory.load64(m_bp+ 8);
  m_pc = m_memory.load64(m_bp+16);

  auto  previous_bp = m_memory.load64(m_bp+ 0);
  auto  retval_addr = m_memory.load64(m_bp+24);

    if(ret.operand())
    {
      store(retval_addr,m_function_stack.back()->type_info(),get_value_by_operand(ret.operand()));
    }


  m_bp = previous_bp;

  m_memory.resize(m_sp);

  m_function_stack.pop_back();

    if(m_function_stack.empty())
    {
      halt();
    }
}


void
vm_execution::
process(const vm_branch&  br) noexcept
{
  bool  b = integer(br.operand());

  auto  dst = b?  br.if_true()
             :   br.if_false();

printf("branch to ");
gbcc::print(dst);
printf("\n");

  jump(dst);
}


void
vm_execution::
process(const vm_call&  cal, std::u16string_view  dst) noexcept
{
  auto&  ols = cal.operand_list();

  vm_address  retval_addr = 0;

    if(dst.size())
    {
      auto  sym = find_symbol(dst);

        if(!sym)
        {
          printf("destination ");

          gbcc::print(dst);

          printf(" is not found.\n");

          halt();

          return;
        }


      retval_addr = m_bp+sym->offset();
    }


  call(cal.target(),retval_addr,ols.data(),ols.size());
}


void
vm_execution::
process(const vm_store&  st) noexcept
{
  auto  src_v = get_value_by_operand(st.operand());

  auto  ptr_v = get_value_by_name(st.dst_label());

  store(ptr_v.integer(),st.dst_type_info(),src_v);
}


vm_value
vm_execution::
process(const vm_load&  ld) noexcept
{
  auto  v = get_value_by_name(ld.label());

  auto  ti = ld.type_info();
  auto  sz = ti.size();

    if(v.type_info().is_integer())
    {
      auto  ptr = v.integer();

      return (sz == 1)? vm_value(m_memory.load8( ptr))
            :(sz == 2)? vm_value(m_memory.load16(ptr))
            :(sz == 4)? vm_value(m_memory.load32(ptr))
            :(sz == 8)? vm_value(m_memory.load64(ptr))
            :vm_value()
            ;
    }


  return vm_value();
}


vm_value
vm_execution::
process(const vm_operation&  op) noexcept
{
  int64_t  l =                     integer(op.left_operand() );
  int64_t  r = op.right_operand()? integer(op.right_operand()):0;

    switch(op.opcode())
    {
  case(vm_opcode::add):
      return vm_value(l+r);
      break;
  case(vm_opcode::sub):
      return vm_value(l-r);
      break;
  case(vm_opcode::mul):
      return vm_value(l*r);
      break;
  case(vm_opcode::div):
      return vm_value(l/r);
      break;
  case(vm_opcode::rem):
      return vm_value(l%r);
      break;
  case(vm_opcode::shl):
      return vm_value(l<<r);
      break;
  case(vm_opcode::shr):
      return vm_value(l>>r);
      break;
  case(vm_opcode::and_):
      return vm_value(l&r);
      break;
  case(vm_opcode::or_):
      return vm_value(l|r);
      break;
  case(vm_opcode::xor_):
      return vm_value(l^r);
      break;
  case(vm_opcode::eq):
      return vm_value(l == r? 1:0);
      break;
  case(vm_opcode::neq):
      return vm_value(l != r? 1:0);
      break;
  case(vm_opcode::lt):
printf("%" PRIi64 " < %" PRIi64 "\n",l,r);
      return vm_value(l < r? 1:0);
      break;
  case(vm_opcode::lteq):
      return vm_value(l <= r? 1:0);
      break;
  case(vm_opcode::gt):
      return vm_value(l > r? 1:0);
      break;
  case(vm_opcode::gteq):
      return vm_value(l >= r? 1:0);
      break;
  case(vm_opcode::logi_and):
      return vm_value(l && r? 1:0);
      break;
  case(vm_opcode::logi_or):
      return vm_value(l || r? 1:0);
      break;
  case(vm_opcode::neg):
      return vm_value(-l);
      break;
  case(vm_opcode::logi_not):
      return vm_value(l? 0:1);
      break;
  case(vm_opcode::not_):
      return vm_value(~l);
      break;
    }


  return vm_value();
}


vm_value
vm_execution::
process(const vm_address_operation&  addr) noexcept
{
  auto  sym = find_symbol(addr.label());

    if(sym)
    {
      return vm_value(m_bp+sym->offset());
    }


  return vm_value();
}


void
vm_execution::
process(const vm_register&  reg) noexcept
{
    if(reg.is_allocate())
    {
      return;
    }


    if(reg.is_call())
    {
      process(reg.call(),reg.label());

      return;
    }


  auto  sym = find_symbol(reg.label());

    if(sym)
    {
      auto  v = reg.is_load()?       process(reg.load())
               :reg.is_operation()? process(reg.operation())
               :reg.is_address_operation()? process(reg.address_operation())
               :vm_value()
               ;

      store(m_bp+sym->offset(),sym->type_info(),v);
    }
}


void
vm_execution::
process(const vm_line&  ln) noexcept
{
       if(ln.is_return()  ){process(ln.get_return());}
  else if(ln.is_branch()  ){process(ln.get_branch());}
  else if(ln.is_jump()    ){jump(ln.get_jump().label());}
  else if(ln.is_call()    ){process(ln.get_call(),u"");}
  else if(ln.is_store()   ){process(ln.get_store());}
  else if(ln.is_register()){process(ln.get_register());}
}




}




