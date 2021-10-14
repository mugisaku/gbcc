#include"libgbcc/vms/typesystem.hpp"




namespace gbcc{




template<class  T>
T*
clone(const T*  t) noexcept
{
  return t? new T(*t):nullptr;
}


sc_type_info&
sc_type_info::
assign(const sc_type_info&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_letter = rhs.m_letter;
      m_size   = rhs.m_size;
      m_align  = rhs.m_align;
      m_id     = rhs.m_id;

        switch(m_letter)
        {
      case('p'): m_data.ptr = clone(rhs.m_data.ptr);break;
      case('r'): m_data.ref = clone(rhs.m_data.ref);break;
      case('a'): m_data.arr = clone(rhs.m_data.arr);break;
      case('s'): m_data.st =  clone(rhs.m_data.st);break;
      case('o'): m_data.un =  clone(rhs.m_data.un);break;
      case('e'): m_data.en =  clone(rhs.m_data.en);break;
      case('F'): m_data.sig = clone(rhs.m_data.sig);break;
        }
    }


  return *this;
}


sc_type_info&
sc_type_info::
assign(sc_type_info&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_letter,rhs.m_letter);
      std::swap(m_size  ,rhs.m_size  );
      std::swap(m_align ,rhs.m_align );
      std::swap(m_id    ,rhs.m_id    );

        switch(m_letter)
        {
      case('p'): m_data.ptr = rhs.m_data.ptr;break;
      case('r'): m_data.ref = rhs.m_data.ref;break;
      case('a'): m_data.arr = rhs.m_data.arr;break;
      case('s'): m_data.st = rhs.m_data.st;break;
      case('o'): m_data.un = rhs.m_data.un;break;
      case('e'): m_data.en = rhs.m_data.en;break;
      case('F'): m_data.sig = rhs.m_data.sig;break;
        }
    }


  return *this;
}


sc_type_info&
sc_type_info::
assign(sc_pointer_info&&  ptr) noexcept
{
  clear();

  m_letter = 'p';
  m_size  = g_pointer_size;
  m_align = g_pointer_size;

  m_id = ptr.target().id()+"p";

  m_data.ptr = new sc_pointer_info(std::move(ptr));

  return *this;
}


sc_type_info&
sc_type_info::
assign(sc_reference_info&&  ref) noexcept
{
  clear();

  m_letter = 'r';
  m_size  = g_pointer_size;
  m_align = g_pointer_size;

  m_id = ref.target().id()+"r";

  m_data.ref = new sc_reference_info(std::move(ref));

  return *this;
}


sc_type_info&
sc_type_info::
assign(sc_array_info&&  arr) noexcept
{
  clear();

  m_letter = 'a';
  m_size  = arr.size();
  m_align = arr.align();

  m_id = arr.target().id()+std::to_string(arr.number_of_elements());

  m_data.arr = new sc_array_info(std::move(arr));

  return *this;
}


sc_type_info&
sc_type_info::
assign(sc_struct_definition&&  st) noexcept
{
  clear();

  m_letter = 's';
  m_size  = st.size();
  m_align = st.align();

  m_id = st.id();

  m_data.st = new sc_struct_definition(std::move(st));

  return *this;
}


sc_type_info&
sc_type_info::
assign(sc_union_definition&&  un) noexcept
{
  clear();

  m_letter = 'o';
  m_size  = un.size();
  m_align = un.align();

  m_id = un.id();

  m_data.un = new sc_union_definition(std::move(un));

  return *this;
}


sc_type_info&
sc_type_info::
assign(sc_enum_definition&&  en) noexcept
{
  clear();

  m_letter = 'e';
  m_size  = 8;
  m_align = 8;

  m_id = en.id();

  m_data.en = new sc_enum_definition(std::move(en));

  return *this;
}


sc_type_info&
sc_type_info::
assign(sc_signature&&  sig) noexcept
{
  clear();

  m_letter = 'F';

  m_id = sig.id();

  m_data.sig = new sc_signature(std::move(sig));

  return *this;
}




void
sc_type_info::
clear() noexcept
{
    switch(m_letter)
    {
  case('s'): std::destroy_at(m_data.st);break;
  case('o'): std::destroy_at(m_data.un);break;
  case('e'): std::destroy_at(m_data.en);break;
  case('p'): std::destroy_at(m_data.ptr);break;
  case('r'): std::destroy_at(m_data.ref);break;
  case('a'): std::destroy_at(m_data.arr);break;
  case('F'): std::destroy_at(m_data.sig);break;
    }


  m_letter = 0;
  m_size   = 0;
  m_align  = 0;

  m_id.clear();
}




void
sc_type_info::
print() const noexcept
{
    switch(m_letter)
    {
  case('s'): printf("struct ");break;
  case('o'): printf("union ");break;
  case('e'): printf("enum ");break;
    }


    switch(m_letter)
    {
  case('U'): printf("undef");break;
  case('V'): printf("void");break;
  case('N'): printf("nullptr");break;
  case('b'): printf("bool%d",8*m_size);break;
  case('c'): printf("char%d",8*m_size);break;
  case('i'): printf("int%d",8*m_size);break;
  case('u'): printf("uint%d",8*m_size);break;
  case('f'): printf("float%d",8*m_size);break;

  case('p'): m_data.ptr->print();break;
  case('r'): m_data.ref->print();break;
  case('a'): m_data.arr->print();break;
  case('s'): m_data.st->print();break;
  case('o'): m_data.un->print();break;
  case('e'): m_data.en->print();break;
  case('F'): m_data.sig->print();break;
    }
}




sc_type_info
make_array(const sc_type_info&  ti, int  n) noexcept
{
  return sc_type_info(sc_array_info(ti,n));
}


sc_type_info
make_pointer(const sc_type_info&  ti) noexcept
{
  return sc_type_info(sc_pointer_info(ti));
}


sc_type_info
make_reference(const sc_type_info&  ti) noexcept
{
  return sc_type_info(sc_reference_info(ti));
}


sc_type_info
remove_array(const sc_type_info&  ti) noexcept
{
  return ti.is_array()? ti.array_info().target()
        :ti
        ;
}


sc_type_info
remove_pointer(const sc_type_info&  ti) noexcept
{
  return ti.is_pointer()? ti.pointer_info().target()
        :ti
        ;
}


sc_type_info
remove_reference(const sc_type_info&  ti) noexcept
{
  return ti.is_reference()? ti.reference_info().target()
        :ti
        ;
}




const sc_type_info     sc_int8_ti = sc_type_info('i',1);
const sc_type_info    sc_int16_ti = sc_type_info('i',2);
const sc_type_info    sc_int32_ti = sc_type_info('i',4);
const sc_type_info    sc_int64_ti = sc_type_info('i',8);
const sc_type_info    sc_uint8_ti = sc_type_info('u',1);
const sc_type_info   sc_uint16_ti = sc_type_info('u',2);
const sc_type_info   sc_uint32_ti = sc_type_info('u',4);
const sc_type_info   sc_uint64_ti = sc_type_info('u',8);
const sc_type_info  sc_float32_ti = sc_type_info('f',4);
const sc_type_info  sc_float64_ti = sc_type_info('f',8);
const sc_type_info    sc_bool8_ti = sc_type_info('b',1);
const sc_type_info   sc_bool16_ti = sc_type_info('b',2);
const sc_type_info   sc_bool32_ti = sc_type_info('b',4);
const sc_type_info   sc_bool64_ti = sc_type_info('b',8);
const sc_type_info     sc_void_ti = sc_type_info('V',0);
const sc_type_info    sc_undef_ti = sc_type_info('U',0);
const sc_type_info  sc_nullptr_ti = sc_type_info('N',0);




}




