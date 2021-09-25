#include"libgbcc/vms/typesystem.hpp"




namespace gbcc{




void
sc_union_definition::
push(sc_type_info&&  ti, std::u16string_view  name) noexcept
{
  m_size  = std::max(m_size ,ti.size() );
  m_align = std::max(m_align,ti.align());

  m_member_list.emplace_back(std::move(ti),name);
}


const sc_type_info*
sc_union_definition::
find(std::u16string_view  name) const noexcept
{
    for(auto&  m: m_member_list)
    {
        if(m.m_name == name)
        {
          return &m.m_type_info;
        }
    }


  return nullptr;
}


std::string
sc_union_definition::
make_id() const noexcept
{
  std::string  s("un");

    for(auto&  m: m_member_list)
    {
      s += m.m_type_info.id();
    }


  return std::move(s);
}


void
sc_union_definition::
print() const noexcept
{
  printf("{\n");

    for(auto&  m: m_member_list)
    {
      m.m_type_info.print();

      printf("  ");

      gbcc::print(m.m_name);

      printf(";\n");
    }


  printf("}\n");
}




}




