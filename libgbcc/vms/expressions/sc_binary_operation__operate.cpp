#include"libgbcc/vms/context.hpp"
#include<cmath>




namespace gbcc{




void
sc_binary_operation::functor::
convert_values() noexcept
{
  auto&  lti = m_l_type_info;
  auto&  rti = m_r_type_info;

  auto&  lv = m_l_value;
  auto&  rv = m_r_value;

    if(lti.is_floating())
    {
           if(rti.is_floating()        ){}
      else if(rti.is_unsigned_integer()){rv = sc_value(static_cast<double>(rv.unsigned_integer()));}
      else if(rti.is_integer()         ){rv = sc_value(static_cast<double>(rv.integer()         ));}
      else{return;}


      m_result_type_info = sc_float64_ti;
    }

  else
    if(rti.is_floating())
    {
           if(lti.is_unsigned_integer()){lv = sc_value(static_cast<double>(lv.unsigned_integer()));}
      else if(lti.is_integer()         ){lv = sc_value(static_cast<double>(lv.integer()         ));}
      else{return;}


      m_result_type_info = sc_float64_ti;
    }

  else
    if(lti.is_unsigned_integer())
    {
           if(rti.is_unsigned_integer()){}
      else if(rti.is_integer()         ){rv = sc_value(static_cast<uint64_t>(rv.integer()));}
      else{return;}


      m_result_type_info = sc_uint64_ti;
    }

  else
    if(rti.is_unsigned_integer())
    {
        if(lti.is_integer()){lv = sc_value(static_cast<uint64_t>(lv.integer()));}
      else{return;}


      m_result_type_info = sc_uint64_ti;
    }

  else
    if(lti.is_integer() && rti.is_integer())
    {
      m_result_type_info = sc_int64_ti;
    }
}




sc_binary_operation::functor&
sc_binary_operation::functor::
operate_add() noexcept
{
  convert_values();

  m_result_value = m_result_type_info.is_floating()?         sc_value(m_l_value.floating()        +m_r_value.floating()        )
                  :m_result_type_info.is_unsigned_integer()? sc_value(m_l_value.unsigned_integer()+m_r_value.unsigned_integer())
                  :m_result_type_info.is_integer()?          sc_value(m_l_value.integer()         +m_r_value.integer()         )
                  :sc_value()
                  ;

  return *this;
}


sc_binary_operation::functor&
sc_binary_operation::functor::
operate_sub() noexcept
{
  convert_values();

  m_result_value = m_result_type_info.is_floating()?          sc_value(m_l_value.floating()        -m_r_value.floating()        )
                   :m_result_type_info.is_unsigned_integer()? sc_value(m_l_value.unsigned_integer()-m_r_value.unsigned_integer())
                   :m_result_type_info.is_integer()?          sc_value(m_l_value.integer()         -m_r_value.integer()         )
                   :sc_value()
                   ;

  return *this;
}


sc_binary_operation::functor&
sc_binary_operation::functor::
operate_mul() noexcept
{
  convert_values();

  m_result_value = m_result_type_info.is_floating()?         sc_value(m_l_value.floating()        *m_r_value.floating()        )
                  :m_result_type_info.is_unsigned_integer()? sc_value(m_l_value.unsigned_integer()*m_r_value.unsigned_integer())
                  :m_result_type_info.is_integer()?          sc_value(m_l_value.integer()         *m_r_value.integer()         )
                  :sc_value()
                  ;

  return *this;
}


sc_binary_operation::functor&
sc_binary_operation::functor::
operate_div() noexcept
{
  convert_values();

  m_result_value = m_result_type_info.is_floating()?         sc_value(m_l_value.floating()        /m_r_value.floating()        )
                  :m_result_type_info.is_unsigned_integer()? sc_value(m_l_value.unsigned_integer()/m_r_value.unsigned_integer())
                  :m_result_type_info.is_integer()?          sc_value(m_l_value.integer()         /m_r_value.integer()         )
                  :sc_value()
                  ;

  return *this;
}


sc_binary_operation::functor&
sc_binary_operation::functor::
operate_rem() noexcept
{
  convert_values();

  m_result_value = m_result_type_info.is_floating()?         sc_value(std::fmod(m_l_value.floating(),m_r_value.floating())     )
                  :m_result_type_info.is_unsigned_integer()? sc_value(m_l_value.unsigned_integer()%m_r_value.unsigned_integer())
                  :m_result_type_info.is_integer()?          sc_value(m_l_value.integer()         %m_r_value.integer()         )
                  :sc_value()
                  ;

  return *this;
}


sc_binary_operation::functor&
sc_binary_operation::functor::
operate_shl() noexcept
{
  convert_values();

  m_result_value = m_result_type_info.is_unsigned_integer()? sc_value(m_l_value.unsigned_integer()<<m_r_value.unsigned_integer())
                  :m_result_type_info.is_integer()?          sc_value(m_l_value.integer()         <<m_r_value.integer()         )
                  :sc_value()
                  ;

  return *this;
}


sc_binary_operation::functor&
sc_binary_operation::functor::
operate_shr() noexcept
{
  convert_values();

  m_result_value = m_result_type_info.is_unsigned_integer()? sc_value(m_l_value.unsigned_integer()>>m_r_value.unsigned_integer())
                  :m_result_type_info.is_integer()?          sc_value(m_l_value.integer()         >>m_r_value.integer()         )
                  :sc_value()
                  ;

  return *this;
}


sc_binary_operation::functor&
sc_binary_operation::functor::
operate_and() noexcept
{
  convert_values();

  m_result_value = m_result_type_info.is_unsigned_integer()? sc_value(m_l_value.unsigned_integer()&m_r_value.unsigned_integer())
                  :m_result_type_info.is_integer()?          sc_value(m_l_value.integer()         &m_r_value.integer()         )
                  :sc_value()
                  ;

  return *this;
}


sc_binary_operation::functor&
sc_binary_operation::functor::
operate_or() noexcept
{
  convert_values();

  m_result_value = m_result_type_info.is_unsigned_integer()? sc_value(m_l_value.unsigned_integer()|m_r_value.unsigned_integer())
                  :m_result_type_info.is_integer()?          sc_value(m_l_value.integer()         |m_r_value.integer()         )
                  :sc_value()
                  ;

  return *this;
}


sc_binary_operation::functor&
sc_binary_operation::functor::
operate_xor() noexcept
{
  convert_values();

  m_result_value = m_result_type_info.is_unsigned_integer()? sc_value(m_l_value.unsigned_integer()^m_r_value.unsigned_integer())
                  :m_result_type_info.is_integer()? sc_value(m_l_value.integer()         ^m_r_value.integer()         )
                  :sc_value()
                  ;

  return *this;
}


sc_binary_operation::functor&
sc_binary_operation::functor::
operate_eq() noexcept
{
  m_result_value = m_result_type_info.is_floating()?         m_l_value.floating()         == m_r_value.floating()
                  :m_result_type_info.is_unsigned_integer()? m_l_value.unsigned_integer() == m_r_value.unsigned_integer()
                  :m_result_type_info.is_integer()?          m_l_value.integer()          == m_r_value.integer()
                  :m_result_type_info.is_pointer()?          m_l_value.integer()          == m_r_value.integer()
                  :false
                  ;

  return *this;
}


sc_binary_operation::functor&
sc_binary_operation::functor::
operate_neq() noexcept
{
  m_result_value = m_result_type_info.is_floating()?         m_l_value.floating()         != m_r_value.floating()
                  :m_result_type_info.is_unsigned_integer()? m_l_value.unsigned_integer() != m_r_value.unsigned_integer()
                  :m_result_type_info.is_integer()?          m_l_value.integer()          != m_r_value.integer()
                  :m_result_type_info.is_pointer()?          m_l_value.integer()          != m_r_value.integer()
                  :false
                  ;

  return *this;
}


sc_binary_operation::functor&
sc_binary_operation::functor::
operate_lt() noexcept
{
  m_result_value = m_result_type_info.is_floating()?         m_l_value.floating()         < m_r_value.floating()
                  :m_result_type_info.is_unsigned_integer()? m_l_value.unsigned_integer() < m_r_value.unsigned_integer()
                  :m_result_type_info.is_integer()?          m_l_value.integer()          < m_r_value.integer()
                  :m_result_type_info.is_pointer()?          m_l_value.integer()          < m_r_value.integer()
                  :false
                  ;

  return *this;
}


sc_binary_operation::functor&
sc_binary_operation::functor::
operate_lteq() noexcept
{
  m_result_value = m_result_type_info.is_floating()?         m_l_value.floating()         <= m_r_value.floating()
                  :m_result_type_info.is_unsigned_integer()? m_l_value.unsigned_integer() <= m_r_value.unsigned_integer()
                  :m_result_type_info.is_integer()?          m_l_value.integer()          <= m_r_value.integer()
                  :m_result_type_info.is_pointer()?          m_l_value.integer()          <= m_r_value.integer()
                  :false
                  ;

  return *this;
}


sc_binary_operation::functor&
sc_binary_operation::functor::
operate_gt() noexcept
{
  m_result_value = m_result_type_info.is_floating()?         m_l_value.floating()         > m_r_value.floating()
                  :m_result_type_info.is_unsigned_integer()? m_l_value.unsigned_integer() > m_r_value.unsigned_integer()
                  :m_result_type_info.is_integer()?          m_l_value.integer()          > m_r_value.integer()
                  :m_result_type_info.is_pointer()?          m_l_value.integer()          > m_r_value.integer()
                  :false
                  ;

  return *this;
}


sc_binary_operation::functor&
sc_binary_operation::functor::
operate_gteq() noexcept
{
  m_result_value = m_result_type_info.is_floating()?         m_l_value.floating()         >= m_r_value.floating()
                  :m_result_type_info.is_unsigned_integer()? m_l_value.unsigned_integer() >= m_r_value.unsigned_integer()
                  :m_result_type_info.is_integer()?          m_l_value.integer()          >= m_r_value.integer()
                  :m_result_type_info.is_pointer()?          m_l_value.integer()          >= m_r_value.integer()
                  :false
                  ;

  return *this;
}


sc_binary_operation::functor&
sc_binary_operation::functor::
operate_logical_and() noexcept
{
  m_result_value = sc_value(m_l_value && m_r_value);

  return *this;
}


sc_binary_operation::functor&
sc_binary_operation::functor::
operate_logical_or() noexcept
{
  m_result_value = sc_value(m_l_value || m_r_value);

  return *this;
}


sc_binary_operation::functor&
sc_binary_operation::functor::
operate_rv() noexcept
{
  m_result_type_info = m_r_type_info;
  m_result_value     = m_r_value;

  return *this;
}




sc_value
sc_binary_operation::
operate_assign(sc_value  l, const sc_type_info&  lti, functor&  f, sc_context&  ctx) noexcept
{
    if(lti.is_reference())
    {
      ctx.store(l.integer(),remove_reference(lti),f.result_value(),f.result_type_info());

      return l;
    }


  return sc_value();
}




}




