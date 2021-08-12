#include"libgbcc/vm.hpp"




namespace gbcc{




void
vm_type_info::
clear() noexcept
{
  m_letter = 'v';
  m_size   =   0;

  delete m_base_type_info          ;
         m_base_type_info = nullptr;
}


vm_type_info&
vm_type_info::
assign(const vm_type_info&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_letter = rhs.m_letter;
      m_size   = rhs.m_size;

        if(rhs.m_base_type_info)
        {
          m_base_type_info = new vm_type_info(*rhs.m_base_type_info);
        }
    }


  return *this;
}


vm_type_info&
vm_type_info::
assign(vm_type_info&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_letter        ,rhs.m_letter);
      std::swap(m_size          ,rhs.m_size);
      std::swap(m_base_type_info,rhs.m_base_type_info);
    }


  return *this;
}


vm_type_info&
vm_type_info::
assign(int  l, int  sz) noexcept
{
  clear();

  m_letter =  l;
  m_size   = sz;

  return *this;
}


void
vm_type_info::
print() const noexcept
{
       if(m_letter == 'n'){printf("undefined");}
  else if(m_letter == 'v'){printf("void");}
  else if(m_letter == 'i'){printf("i%d",8*m_size);}
  else if(m_letter == 'u'){printf("u%d",8*m_size);}
  else if(m_letter == 'f'){printf("f%d",8*m_size);}
  else if(m_letter == 'o'){printf("o%d",m_size);}
  else if(m_letter == 'p')
    {
      m_base_type_info->print();
      printf("*");
    }

  else {printf("unknown");}
}




}




