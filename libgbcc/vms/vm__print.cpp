#include"libgbcc/vm.hpp"
#include<cinttypes>




namespace gbcc{




namespace{
void  print_type(const syntax_branch&  br) noexcept;


void
print_operand(const syntax_branch&  br) noexcept
{
  gbcc::print(br[0].string());
}


void
print_parameter(const syntax_branch&  br) noexcept
{
  print_type(br[0].branch());

  printf("  ");

  gbcc::print(br[1].string());
}


void
print_parameter_list(const syntax_branch&  br) noexcept
{
  printf("(");

    for(auto&  e: br)
    {
        if(e.is_branch(u"parameter"))
        {
          print_parameter(e.branch());

          printf(", ");
        }
    }


  printf(")");
}


void
print_define_operation(const syntax_branch&  br) noexcept
{
  printf("def ");

  auto&  tok = br[1].token();

    if(tok.is_integer())
    {
      printf("%" PRIu64,tok.integer());
    }

  else
    {
//      tok.floating();
    }
}


void
print_unary_operation(const syntax_branch&  br) noexcept
{
}


void
print_binary_instruction(const syntax_branch&  br) noexcept
{
  gbcc::print(br[0].string());
}


void
print_binary_operation(const syntax_branch&  br) noexcept
{
  print_binary_instruction(br[0].branch());

  printf(" ");

  print_operand(br[1].branch());

  printf(" ");

  print_operand(br[2].branch());
}


void
print_address_operation(const syntax_branch&  br) noexcept
{
}


void
print_call_operation(const syntax_branch&  br) noexcept
{
}


void
print_phi_operation(const syntax_branch&  br) noexcept
{
}


void
print_operation(const syntax_branch&  br) noexcept
{
  auto&  e = br[0];

       if(e.is_branch(u"define_operation") ){print_define_operation(e.branch());}
  else if(e.is_branch(u"unary_operation")  ){print_unary_operation(e.branch());}
  else if(e.is_branch(u"binary_operation") ){print_binary_operation(e.branch());}
  else if(e.is_branch(u"address_operation")){print_address_operation(e.branch());}
  else if(e.is_branch(u"call_operation")   ){print_call_operation(e.branch());}
  else if(e.is_branch(u"phi_operation")    ){print_phi_operation(e.branch());}
  else{}
}


void
print_register_statement(const syntax_branch&  br) noexcept
{
  printf("reg ");

  print_type(br[1].branch());

  printf(" ");

  gbcc::print(br[2].string());

  printf(" ");

  print_operation(br[3].branch());
}


void
print_return_statement(const syntax_branch&  br) noexcept
{
  printf("ret ");

    if(br.length() == 2)
    {
      print_operand(br[1].branch());
    }
}


void
print_branch_statement(const syntax_branch&  br) noexcept
{
  gbcc::print(br[1].string());
  gbcc::print(br[2].string());
}


void
print_label_statement(const syntax_branch&  br) noexcept
{
  gbcc::print(br[1].string());
}


void
print_store_statement(const syntax_branch&  br) noexcept
{
  print_type(br[1].branch());

  gbcc::print(br[2].string());
  gbcc::print(br[3].string());
}


void
print_statement(const syntax_branch&  br) noexcept
{
  auto&  e = br[0];

       if(e.is_branch(u"register_statement")){print_register_statement(e.branch());}
  else if(e.is_branch(u"return_statement")  ){print_return_statement(e.branch());}
  else if(e.is_branch(u"branch_statement")  ){print_branch_statement(e.branch());}
  else if(e.is_branch(u"label_statement")   ){print_label_statement(e.branch());}
  else if(e.is_branch(u"store_statement")   ){print_store_statement(e.branch());}
  else{;}
}


void
print_statement_list(const syntax_branch&  br) noexcept
{
    for(auto&  e: br)
    {
        if(e.is_branch(u"statement"))
        {
          printf("  ");
  
          print_statement(e.branch());

          printf("\n");
        }
    }
}


void
print_type(const syntax_branch&  br) noexcept
{
  gbcc::print(br[0].string());
}


void
print_function(const syntax_branch&  br) noexcept
{
  printf("function\n");

  print_type(br[1].branch());

  print_parameter_list(br[2].branch());

  printf("\n");

  gbcc::print(br[3].string());

  printf("\n{\n");

  print_statement_list(br[4].branch());

  printf("}\n");
}
}




void
print_branch_as_ir(const syntax_branch&  br)
{
    for(auto&  e: br)
    {
        if(e.is_branch(u"element"))
        {
            for(auto&  ee: e.branch())
            {
                if(ee.is_branch(u"function_declaration"))
                {
                  print_function(ee.branch());
                }
            }
        }
    }
}




void
vm_value::
print() const noexcept
{
  printf("value: ");

    if(m_type_info.is_integer())
    {
      printf("%" PRIi64,m_data.i);
    }

  else
    {
      printf("null");
    }
}


void
vm_operand::
print() const noexcept
{
       if(is_integer()         ){printf("%" PRIi64,m_data.i);}
  else if(is_unsigned_integer()){printf("%" PRIu64,m_data.u);}
  else if(is_floating()        ){printf("%f",m_data.f);}
  else if(is_identifier()      ){gbcc::print(m_data.s);}
  else{printf("NULL");}
}


void
vm_load::
print() const noexcept
{
  printf("ld ");

  m_type_info.print();

  printf(" ");

  gbcc::print(m_label);
}


void
vm_store::
print() const noexcept
{
  printf("st ");

  m_operand.print();

  printf(" ");

  m_dst_type_info.print();

  printf(" ");

  gbcc::print(m_dst_label);
}


void
vm_call::
print() const noexcept
{
  printf("cal ");

  gbcc::print(m_target);

  printf("(");

    for(auto&  o: m_operand_list)
    {
      o.print();

      printf(",");
    }


  printf(")");
}


void
vm_return::
print() const noexcept
{
  printf("ret ");

  m_operand.print();
}


void
vm_jump::
print() const noexcept
{
  printf("jmp ");

  gbcc::print(m_label);
}


void
vm_address_operation::
print() const noexcept
{
  printf("addr ");

  gbcc::print(m_label);
}


void
vm_branch::
print() const noexcept
{
  printf("br ");

  m_operand.print();

  printf(" ");

  gbcc::print(m_if_true);

  printf(" ");

  gbcc::print(m_if_false);
}


void
vm_operation::
print() const noexcept
{
  gbcc::print(get_string(m_opcode));

  printf(" ");

  m_left_operand.print();

  printf(" ");

  m_right_operand.print();
}


void
vm_symbol::
print() const noexcept
{
  m_type_info.print();

  printf("  ");

  gbcc::print(m_name);

  printf("(%6" PRIi64 ")",m_offset);
}



}




