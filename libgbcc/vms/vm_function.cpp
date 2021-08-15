#include"libgbcc/vm.hpp"




namespace gbcc{




vm_function&
vm_function::
assign(const syntax_branch&  br) noexcept
{
  m_block_list.clear();

  m_symbol_table.clear();

  m_type_info = construct_type_info(br[1].branch());

  auto  parals = construct_parameter_list(br[2].branch());

  m_argument_size         = 0;
  m_stack_allocation_size = 0;

    for(auto&  para: parals)
    {
      auto&  sym = m_symbol_table.emplace_back(para.type_info(),para.name(),'l');

      sym.set_offset(m_argument_size);

      m_argument_size += para.type_info().max_size();
    }

    
  m_symbol_table.emplace_back(vm_type_info('i',8),u"__previous_bp__",'l').set_offset(m_argument_size);
  m_symbol_table.emplace_back(vm_type_info('i',8),u"__current_sp__",'l').set_offset(m_argument_size+8);
  m_symbol_table.emplace_back(vm_type_info('i',8),u"__return_address__",'l').set_offset(m_argument_size+16);
  m_symbol_table.emplace_back(vm_type_info('i',8),u"__return_value_address__",'l').set_offset(m_argument_size+24);

  m_stack_allocation_size = m_argument_size+32;

  m_name = br[3].string();

    if(br[4].is_branch(u"block_list"))
    {
      m_block_list = construct_block_list(br[4].branch());
    }


  return *this;
}




const vm_block*
vm_function::
find_block(std::u16string_view  label) const noexcept
{
    for(auto&  blk: m_block_list)
    {
        if(blk.label() == label)
        {
          return &blk;
        }
    }


  return nullptr;
}


int
vm_function::
finalize(int  pos) noexcept
{
    for(auto&  blk: m_block_list)
    {
        for(auto&  ln: blk.line_list())
        {
          ln.set_position(pos++);

            if(ln.is_register())
            {
              auto&  reg = ln.get_register();

              m_symbol_table.emplace_back(reg.type_info(),reg.label(),'l').set_offset(m_stack_allocation_size);

              m_stack_allocation_size += reg.type_info().max_size();
            }
        }
    }


    for(auto&  sym: m_symbol_table)
    {
      sym.add_offset(-m_argument_size);
    }


  return pos;
}


void
vm_function::
print() const noexcept
{
  printf("function ");

  m_type_info.print();

  printf(" ");

  gbcc::print(m_name);

  printf("\n");

    for(auto&  sym: m_symbol_table)
    {
      sym.print();

      printf("\n");
    }


  printf("\n");

    for(auto&  blk: m_block_list)
    {
      gbcc::print(blk.label());

      printf(":\n");

        for(auto&  ln: blk.line_list())
        {
          ln.print();

          printf("\n");
        }
    }
}




}




