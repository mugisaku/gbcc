#include"libgbcc/vms/typesystem.hpp"



namespace gbcc{




std::string
sc_signature::
make_id() const noexcept
{
  std::string  s("fp");

  s += m_type_info.id();

    for(auto&  p: m_parameter_list)
    {
      s += p.type_info().id();
    }


  return std::move(s);
}


void
sc_signature::
print() const noexcept
{
  m_type_info.print();

  printf("(");

    for(auto&  p: m_parameter_list)
    {
        if(p.name().size())
        {
          gbcc::print(p.name());

          printf("=");
        }


      p.type_info().print();

      printf(", ");
    }


  printf(")");
}




}




