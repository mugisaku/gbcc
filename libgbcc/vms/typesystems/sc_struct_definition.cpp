#include"libgbcc/vms/typesystem.hpp"




namespace gbcc{




constexpr
int
get_aligned_offset(int  offset, int  align) noexcept
{
    if(align)
    {
      offset = (offset+(align-1))/align*align;
    }


  return offset;
}


void
sc_struct_definition::
push(sc_type_info&&  ti, std::u16string_view  name) noexcept
{
  int  offset = m_size;

  const auto     sz = ti.size();
  const auto  align = ti.align();

  offset = get_aligned_offset(offset,align);

  m_size  = offset+sz;
  m_align = std::max(m_align,align);

  m_member_list.emplace_back(std::move(ti),name,offset);
}


const sc_struct_member*
sc_struct_definition::
find(std::u16string_view  name) const noexcept
{
    for(auto&  m: m_member_list)
    {
        if(m.name() == name)
        {
          return &m;
        }
    }


  return nullptr;
}


std::string
sc_struct_definition::
make_id() const noexcept
{
  std::string  s("st");

    for(auto&  m: m_member_list)
    {
      s += m.type_info().id();
    }


  return std::move(s);
}


void
sc_struct_definition::
print() const noexcept
{
  printf("{\n");

    for(auto&  m: m_member_list)
    {
      m.type_info().print();

      printf("  ");

      gbcc::print(m.name());

      printf("(offset:%d);\n",m.offset());
    }


  printf("}\n");
}




}




