#include"libgbcc/vm.hpp"




namespace gbcc{




vm_type_info&
vm_type_info::
assign(int  l, int  sz) noexcept
{
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
  else {printf("unknown");}
}




}




