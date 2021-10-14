#include"libgbcc/vms/typesystem.hpp"




namespace gbcc{




sc_enumerator&
sc_enum_definition::
push(std::u16string_view  name, int  value) noexcept
{
  return m_enumerator_list.emplace_back(name,value);
}


const int64_t*
sc_enum_definition::
find(std::u16string_view  name) const noexcept
{
    for(auto&  en: m_enumerator_list)
    {
        if(en.name() == name)
        {
          return &en.value();
        }
    }


  return nullptr;
}


std::string
sc_enum_definition::
id() const noexcept
{
  std::string  s("en");

    for(auto&  en: m_enumerator_list)
    {
      s += std::to_string(en.value());
    }


  return std::move(s);
}


void
sc_enum_definition::
print() const noexcept
{
  printf("{\n");

    for(auto&  en: m_enumerator_list)
    {
      en.print();

      printf("\n");
    }


  printf("}\n");
}




}




