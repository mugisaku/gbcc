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

      m_kind = rhs.m_kind;
      m_id   = rhs.m_id;

        switch(m_kind)
        {
      case(kind::identifier): new(&m_data) std::u16string(rhs.m_data.s);break;
      case(kind::pointer  ): m_data.ptr = clone(rhs.m_data.ptr);break;
      case(kind::reference): m_data.ref = clone(rhs.m_data.ref);break;
      case(kind::array    ): m_data.arr = clone(rhs.m_data.arr);break;
      case(kind::struct_  ): m_data.st =  clone(rhs.m_data.st);break;
      case(kind::union_   ): m_data.un =  clone(rhs.m_data.un);break;
      case(kind::enum_    ): m_data.en =  clone(rhs.m_data.en);break;
      case(kind::function_pointer): m_data.sig = clone(rhs.m_data.sig);break;
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

      std::swap(m_kind,rhs.m_kind);
      std::swap(m_id  ,rhs.m_id  );

        switch(m_kind)
        {
      case(kind::identifier): new(&m_data) std::u16string(std::move(rhs.m_data.s));break;
      case(kind::pointer  ): m_data.ptr = rhs.m_data.ptr;break;
      case(kind::reference): m_data.ref = rhs.m_data.ref;break;
      case(kind::array    ): m_data.arr = rhs.m_data.arr;break;
      case(kind::struct_  ): m_data.st = rhs.m_data.st;break;
      case(kind::union_   ): m_data.un = rhs.m_data.un;break;
      case(kind::enum_    ): m_data.en = rhs.m_data.en;break;
      case(kind::function_pointer): m_data.sig = rhs.m_data.sig;break;
        }
    }


  return *this;
}


sc_type_info&
sc_type_info::
assign(std::u16string_view  id) noexcept
{
  clear();

  m_kind = (id == u"void")? kind::void_
          :(id == u"undefined")? kind::undefined
          :(id == u"nullptr")? kind::null_pointer
          :(id == u"bool8")? kind::bool8
          :(id == u"bool16")? kind::bool16
          :(id == u"bool32")? kind::bool32
          :(id == u"bool64")? kind::bool64
          :(id == u"char8")? kind::char8
          :(id == u"char16")? kind::char16
          :(id == u"char32")? kind::char32
          :(id == u"int8")? kind::int8
          :(id == u"int16")? kind::int16
          :(id == u"int32")? kind::int32
          :(id == u"int64")? kind::int64
          :(id == u"uint8")? kind::uint8
          :(id == u"uint16")? kind::uint16
          :(id == u"uint32")? kind::uint32
          :(id == u"uint64")? kind::uint64
          :(id == u"float32")? kind::float32
          :(id == u"float64")? kind::float64
          :(id == u"int")?     kind::int64
          :(id == u"uint")?    kind::uint64
          :(id == u"float")?   kind::float64
          : kind::identifier
          ;


  new(&m_data) std::u16string(id);

  return *this;
}


sc_type_info&
sc_type_info::
assign(sc_pointer_info&&  ptr) noexcept
{
  clear();

  m_kind = kind::pointer;

  m_data.ptr = new sc_pointer_info(std::move(ptr));

  return *this;
}


sc_type_info&
sc_type_info::
assign(sc_reference_info&&  ref) noexcept
{
  clear();

  m_kind = kind::reference;

  m_data.ref = new sc_reference_info(std::move(ref));

  return *this;
}


sc_type_info&
sc_type_info::
assign(sc_array_info&&  arr) noexcept
{
  clear();

  m_kind = kind::array;

  m_data.arr = new sc_array_info(std::move(arr));

  return *this;
}


sc_type_info&
sc_type_info::
assign(sc_struct_definition&&  st) noexcept
{
  clear();

  m_kind = kind::struct_;

  m_data.st = new sc_struct_definition(std::move(st));

  return *this;
}


sc_type_info&
sc_type_info::
assign(sc_union_definition&&  un) noexcept
{
  clear();

  m_kind = kind::union_;

  m_data.un = new sc_union_definition(std::move(un));

  return *this;
}


sc_type_info&
sc_type_info::
assign(sc_enum_definition&&  en) noexcept
{
  clear();

  m_kind = kind::enum_;

  m_data.en = new sc_enum_definition(std::move(en));

  return *this;
}


sc_type_info&
sc_type_info::
assign(sc_signature&&  sig) noexcept
{
  clear();

  m_kind = kind::function_pointer;

  m_data.sig = new sc_signature(std::move(sig));

  return *this;
}




void
sc_type_info::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::struct_  ): std::destroy_at(m_data.st);break;
  case(kind::union_   ): std::destroy_at(m_data.un);break;
  case(kind::enum_    ): std::destroy_at(m_data.en);break;
  case(kind::pointer  ): std::destroy_at(m_data.ptr);break;
  case(kind::reference): std::destroy_at(m_data.ref);break;
  case(kind::array    ): std::destroy_at(m_data.arr);break;
  case(kind::function_pointer): std::destroy_at(m_data.sig);break;
  case(kind::identifier): std::destroy_at(&m_data.s);break;
    }


  m_kind = kind::null;

  m_id.clear();
}


void
sc_type_info::
update_id() noexcept
{
    switch(m_kind)
    {
  case(kind::bool8): m_id = "b8";break;
  case(kind::char8): m_id = "c8";break;
  case(kind::int8 ): m_id = "i8";break;
  case(kind::uint8): m_id = "u8";break;
  case(kind::bool16): m_id = "b16";break;
  case(kind::char16): m_id = "c16";break;
  case(kind::int16 ): m_id = "i16";break;
  case(kind::uint16): m_id = "u16";break;
  case(kind::bool32): m_id = "b32";break;
  case(kind::char32): m_id = "c32";break;
  case(kind::int32 ): m_id = "i32";break;
  case(kind::uint32): m_id = "u32";break;
  case(kind::float32): m_id = "f32";break;
  case(kind::bool64): m_id = "b64";break;
  case(kind::int64 ): m_id = "i64";break;
  case(kind::uint64): m_id = "u64";break;
  case(kind::float64): m_id = "f64";break;

//  case(kind::identifier): printf("%s",m_data.s.data());break;
  case(kind::pointer): m_id = "p";break;
  case(kind::reference): m_id = "r";break;
  case(kind::array): m_id = m_data.arr->make_id();break;
  case(kind::struct_): m_id = m_data.st->make_id();break;
  case(kind::union_ ): m_id = m_data.un->make_id();break;
  case(kind::enum_  ): m_id = m_data.en->make_id();break;
  case(kind::function_pointer): m_id = m_data.sig->make_id();break;
    }
}




int
sc_type_info::
size() const noexcept
{
    switch(m_kind)
    {
  case(kind::bool8):
  case(kind::char8):
  case(kind::int8 ):
  case(kind::uint8):
      return 1;
      break;
  case(kind::bool16):
  case(kind::char16):
  case(kind::int16 ):
  case(kind::uint16):
      return 2;
      break;
  case(kind::bool32):
  case(kind::char32):
  case(kind::int32 ):
  case(kind::uint32):
  case(kind::float32):
      return 4;
      break;
  case(kind::bool64):
  case(kind::int64 ):
  case(kind::uint64):
  case(kind::float64):
      return 8;
      break;
  case(kind::pointer  ): return g_pointer_size;break;
  case(kind::reference): return g_pointer_size;break;
  case(kind::array    ): return m_data.arr->size();break;
  case(kind::struct_): return m_data.st->size();break;
  case(kind::union_ ): return m_data.un->size();break;
  case(kind::enum_  ): return g_pointer_size;break;
  case(kind::function_pointer): return g_pointer_size;break;
    }


  return 0;
}


int
sc_type_info::
align() const noexcept
{
    switch(m_kind)
    {
  case(kind::bool8):
  case(kind::char8):
  case(kind::int8 ):
  case(kind::uint8):
      return 1;
      break;
  case(kind::bool16):
  case(kind::char16):
  case(kind::int16 ):
  case(kind::uint16):
      return 2;
      break;
  case(kind::bool32):
  case(kind::char32):
  case(kind::int32 ):
  case(kind::uint32):
  case(kind::float32):
      return 4;
      break;
  case(kind::bool64):
  case(kind::int64 ):
  case(kind::uint64):
  case(kind::float64):
      return 8;
      break;
  case(kind::pointer  ): return g_pointer_size;break;
  case(kind::reference): return g_pointer_size;break;
  case(kind::array    ): return m_data.arr->align();break;
  case(kind::struct_): return m_data.st->align();break;
  case(kind::union_ ): return m_data.un->align();break;
  case(kind::enum_  ): return g_pointer_size;break;
  case(kind::function_pointer): return g_pointer_size;break;
    }


  return 0;
}




void
sc_type_info::
print() const noexcept
{
    switch(m_kind)
    {
  case(kind::struct_): printf("struct ");break;
  case(kind::union_ ): printf("union ");break;
  case(kind::enum_  ): printf("enum ");break;
    }


    switch(m_kind)
    {
  case(kind::bool8): printf("bool8");break;
  case(kind::char8): printf("char8");break;
  case(kind::int8 ): printf("int8");break;
  case(kind::uint8): printf("uint8");break;
  case(kind::bool16): printf("bool16");break;
  case(kind::char16): printf("char16");break;
  case(kind::int16 ): printf("int16");break;
  case(kind::uint16): printf("uint16");break;
  case(kind::bool32): printf("bool32");break;
  case(kind::char32): printf("char32");break;
  case(kind::int32 ): printf("int32");break;
  case(kind::uint32): printf("uint32");break;
  case(kind::float32): printf("float32");break;
  case(kind::bool64): printf("bool64");break;
  case(kind::int64 ): printf("int64");break;
  case(kind::uint64): printf("uint64");break;
  case(kind::float64): printf("float64");break;

  case(kind::identifier): gbcc::print(m_data.s);break;
  case(kind::pointer): m_data.ptr->print();break;
  case(kind::reference): m_data.ref->print();break;
  case(kind::array): m_data.arr->print();break;
  case(kind::struct_): m_data.st->print();break;
  case(kind::union_ ): m_data.un->print();break;
  case(kind::enum_  ): m_data.en->print();break;
  case(kind::function_pointer): m_data.sig->print();break;
    }


//  printf("(id: %s)",m_id.data());
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




}




