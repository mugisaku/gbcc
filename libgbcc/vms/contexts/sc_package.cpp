#include"libgbcc/vms/context.hpp"




namespace gbcc{




sc_package&
sc_package::
assign(const syntax_branch&  br) noexcept
{
  clear();

    for(auto&  e: br)
    {
        if(e.is_branch(u"top_element"))
        {
            for(auto&  ee: e.branch())
            {
                if(ee.is_branch(u"function_declaration"))
                {
                  m_function_list.emplace_back(construct_function(ee.branch(),*this));
                }

              else
                if(ee.is_branch(u"var_statement"))
                {
                  auto  v = construct_variable(ee.branch());

                  m_variable_list.emplace_back(std::move(v));
                }

              else
                if(ee.is_branch(u"const_statement"))
                {
                  auto  v = construct_variable(ee.branch());

                  m_variable_list.emplace_back(std::move(v));
                }
            }
        }
    }


    for(auto&  fn: m_function_list)
    {
      fn->fix();
    }


  return *this;
}


void
sc_package::
clear() noexcept
{
  m_function_list.clear();
  m_variable_list.clear();
}


int
sc_package::
stack_size() const noexcept
{
  return m_variable_list.size()? m_variable_list.back().symbol().next_offset():32;
}


const sc_function*
sc_package::
find_function(std::u16string_view  name) const noexcept
{
    for(auto&  c: m_function_list)
    {
        if(c->name() == name)
        {
          return &*c;
        }
    }


  return nullptr;
}


const sc_symbol*
sc_package::
find_symbol(std::u16string_view  name) const noexcept
{
    for(auto&  v: m_variable_list)
    {
        if(v.symbol().name() == name)
        {
          return &v.symbol();
        }
    }


  return nullptr;
}


void
sc_package::
print() const noexcept
{
    for(auto&  v: m_variable_list)
    {
      printf("var  ");

      v.symbol().print();

      printf("\n");
    }


  printf("\n\n");

    for(auto&  fn: m_function_list)
    {
      fn->print();

      printf("\n");
    }
}




}




