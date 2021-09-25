#include"libgbcc/vms/expression.hpp"




namespace gbcc{




sc_value
sc_value::
operator()(const sc_type_info&  ti) const noexcept
{
  return *this;
}


void
sc_value::
clear() noexcept
{
}


sc_value&
sc_value::
assign(const sc_value&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_type_info = rhs.m_type_info;

      m_data.i = rhs.m_data.i;
    }


  return *this;
}


sc_value&
sc_value::
assign(sc_value&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_type_info,rhs.m_type_info);

      m_data.i = rhs.m_data.i;
    }


  return *this;
}


sc_value&
sc_value::
assign(bool  b) noexcept
{
  clear();

  m_type_info = sc_type_info(u"bool64");

  m_data.i = b? 1:0;

  return *this;
}


sc_value&
sc_value::
assign(int64_t  i) noexcept
{
  clear();

  m_type_info = sc_type_info(u"int64");

  m_data.i = i;

  return *this;
}


sc_value&
sc_value::
assign(uint64_t  u) noexcept
{
  clear();

  m_type_info = sc_type_info(u"uint64");

  m_data.u = u;

  return *this;
}


sc_value&
sc_value::
assign(double  f) noexcept
{
  clear();

  m_type_info = sc_type_info(u"f64");

  m_data.f = f;

  return *this;
}


sc_value&
sc_value::
assign(const sc_type_info&  ti, int64_t  i) noexcept
{
  clear();

  m_type_info = ti;

  m_data.i = i;

  return *this;
}


sc_value&
sc_value::
assign(const sc_type_info&  ti, uint64_t  u) noexcept
{
  clear();

  m_type_info = ti;

  m_data.u = u;

  return *this;
}


sc_value&
sc_value::
assign(const sc_type_info&  ti, double  f) noexcept
{
  clear();

  m_type_info = ti;

  m_data.f = f;

  return *this;
}


sc_value&
sc_value::
assign(const sc_pointer&  ptr) noexcept
{
  clear();

  m_type_info = make_pointer(ptr.type_info());

  m_data.i = ptr.data();

  return *this;
}


sc_value&
sc_value::
assign(const sc_reference&  ref) noexcept
{
  clear();

  m_type_info = make_reference(ref.type_info());

  m_data.i = ref.data();

  return *this;
}




int64_t
sc_value::
integer(const sc_context&  ctx) const noexcept
{
       if(m_type_info.is_integer()         ){return integer();}
  else if(m_type_info.is_unsigned_integer()){return unsigned_integer();}
  else if(m_type_info.is_floating()        ){return floating();}

  return 0;
}


uint64_t
sc_value::
unsigned_integer(const sc_context&  ctx) const noexcept
{
       if(m_type_info.is_integer()         ){return integer();}
  else if(m_type_info.is_unsigned_integer()){return unsigned_integer();}
  else if(m_type_info.is_floating()        ){return floating();}

  return 0;
}


double
sc_value::
floating(const sc_context&  ctx) const noexcept
{
       if(m_type_info.is_integer()         ){return integer();}
  else if(m_type_info.is_unsigned_integer()){return unsigned_integer();}
  else if(m_type_info.is_floating()        ){return floating();}

  return 0;
}


bool
sc_value::
boolean(const sc_context&  ctx) const noexcept
{
       if(m_type_info.is_integer()         ){return integer();}
  else if(m_type_info.is_unsigned_integer()){return unsigned_integer();}
  else if(m_type_info.is_pointer()         ){return unsigned_integer();}
  else if(m_type_info.is_boolean()         ){return boolean();}
  else if(m_type_info.is_null_pointer()    ){return false;}

  return false;
}




void
sc_value::
print() const noexcept
{
    if(!m_type_info)
    {
      printf("null");
    }

  else
    if(m_type_info.is_undefined())
    {
      printf("undefined");
    }

  else
    if(m_type_info.is_void())
    {
      printf("void");
    }

  else
    if(m_type_info.is_null_pointer())
    {
      printf("null_pointer");
    }

  else
    if(m_type_info.is_boolean())
    {
      printf("%s",m_data.i? "true":"false");
    }

  else
    if(m_type_info.is_integer())
    {
      printf("%" PRIi64 "i",m_data.i);
    }

  else
    if(m_type_info.is_unsigned_integer())
    {
      printf("%" PRIu64 "u",m_data.u);
    }

  else
    if(m_type_info.is_floating())
    {
      printf("%ff",m_data.f);
    }

  else
    if(m_type_info.is_pointer())
    {
      printf("%" PRIi64 "p",m_data.i);
    }

  else
    if(m_type_info.is_reference())
    {
      printf("%" PRIi64 "r",m_data.i);
    }

  else
    if(m_type_info.is_character())
    {
      printf("%" PRIi64 "c",m_data.u);
    }

  else
    if(m_type_info.is_character())
    {
      printf("%" PRIi64 "c",m_data.u);
    }

  else
//    if(m_type_info.is_character())
    {
      printf("other_type_value");
    }
}




}




