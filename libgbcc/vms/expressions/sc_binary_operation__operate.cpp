#include"libgbcc/vms/context.hpp"
#include<cmath>




namespace gbcc{




int
sc_binary_operation::functor::
convert_values() noexcept
{
  auto&  lti = m_left.type_info();
  auto&  rti = m_right.type_info();

    if(lti.is_floating())
    {
           if(rti.is_floating()        ){}
      else if(rti.is_unsigned_integer()){m_right = sc_value(static_cast<double>(m_right.unsigned_integer()));}
      else if(rti.is_integer()         ){m_right = sc_value(static_cast<double>(m_right.integer()         ));}
      else{return 0;}

      return 'd';
    }

  else
    if(rti.is_floating())
    {
           if(lti.is_unsigned_integer()){m_left = sc_value(static_cast<double>(m_left.unsigned_integer()));}
      else if(lti.is_integer()         ){m_left = sc_value(static_cast<double>(m_left.integer()         ));}
      else{return 0;}

      return 'd';
    }

  else
    if(lti.is_unsigned_integer())
    {
           if(rti.is_unsigned_integer()){}
      else if(rti.is_integer()         ){m_right = sc_value(static_cast<uint64_t>(m_right.integer()));}
      else{return 0;}

      return 'u';
    }

  else
    if(rti.is_unsigned_integer())
    {
        if(lti.is_integer()){m_left = sc_value(static_cast<uint64_t>(m_left.integer()));}
      else{return 0;}

      return 'u';
    }

  else
    if(lti.is_integer() && rti.is_integer())
    {
      return 'i';
    }


  return 0;
}




bool
sc_binary_operation::functor::
is_floating() const noexcept
{
  return m_left.type_info().is_floating() &&
         m_right.type_info().is_floating()
        ;
}


bool
sc_binary_operation::functor::
is_unsigned_integer() const noexcept
{
  return m_left.type_info().is_unsigned_integer() &&
         m_right.type_info().is_unsigned_integer()
        ;
}


bool
sc_binary_operation::functor::
is_integer() const noexcept
{
  return m_left.type_info().is_integer() &&
         m_right.type_info().is_integer()
        ;
}


bool
sc_binary_operation::functor::
is_pointer() const noexcept
{
  return m_left.type_info().is_pointer() &&
         m_right.type_info().is_pointer()
        ;
}




sc_value
sc_binary_operation::functor::
operate_add() noexcept
{
  auto  c = convert_values();

  return (c == 'd')? sc_value(m_left.floating()        +m_right.floating()        )
        :(c == 'u')? sc_value(m_left.unsigned_integer()+m_right.unsigned_integer())
        :(c == 'i')? sc_value(m_left.integer()         +m_right.integer()         )
        :            sc_value(                                                    )
        ;
}


sc_value
sc_binary_operation::functor::
operate_sub() noexcept
{
  auto  c = convert_values();

  return (c == 'd')? sc_value(m_left.floating()        -m_right.floating()        )
        :(c == 'u')? sc_value(m_left.unsigned_integer()-m_right.unsigned_integer())
        :(c == 'i')? sc_value(m_left.integer()         -m_right.integer()         )
        :            sc_value(                                                    )
        ;
}


sc_value
sc_binary_operation::functor::
operate_mul() noexcept
{
  auto  c = convert_values();

  return (c == 'd')? sc_value(m_left.floating()        *m_right.floating()        )
        :(c == 'u')? sc_value(m_left.unsigned_integer()*m_right.unsigned_integer())
        :(c == 'i')? sc_value(m_left.integer()         *m_right.integer()         )
        :            sc_value(                                                    )
        ;
}


sc_value
sc_binary_operation::functor::
operate_div() noexcept
{
  auto  c = convert_values();

  return (c == 'd')? sc_value(m_left.floating()        /m_right.floating()        )
        :(c == 'u')? sc_value(m_left.unsigned_integer()/m_right.unsigned_integer())
        :(c == 'i')? sc_value(m_left.integer()         /m_right.integer()         )
        :            sc_value(                                                    )
        ;
}


sc_value
sc_binary_operation::functor::
operate_rem() noexcept
{
  auto  c = convert_values();

  return (c == 'd')? sc_value(std::fmod(m_left.floating(),m_right.floating())     )
        :(c == 'u')? sc_value(m_left.unsigned_integer()%m_right.unsigned_integer())
        :(c == 'i')? sc_value(m_left.integer()         %m_right.integer()         )
        :            sc_value(                                                    )
        ;
}


sc_value
sc_binary_operation::functor::
operate_shl() noexcept
{
  auto  c = convert_values();

  return (c == 'u')? sc_value(m_left.unsigned_integer()<<m_right.unsigned_integer())
        :(c == 'i')? sc_value(m_left.integer()         <<m_right.integer()         )
        :            sc_value(                                                    )
        ;
}


sc_value
sc_binary_operation::functor::
operate_shr() noexcept
{
  auto  c = convert_values();

  return (c == 'u')? sc_value(m_left.unsigned_integer()>>m_right.unsigned_integer())
        :(c == 'i')? sc_value(m_left.integer()         >>m_right.integer()         )
        :            sc_value(                                                    )
        ;
}


sc_value
sc_binary_operation::functor::
operate_and() noexcept
{
  auto  c = convert_values();

  return (c == 'u')? sc_value(m_left.unsigned_integer()&m_right.unsigned_integer())
        :(c == 'i')? sc_value(m_left.integer()         &m_right.integer()         )
        :            sc_value(                                                    )
        ;
}


sc_value
sc_binary_operation::functor::
operate_or() noexcept
{
  auto  c = convert_values();

  return (c == 'u')? sc_value(m_left.unsigned_integer()|m_right.unsigned_integer())
        :(c == 'i')? sc_value(m_left.integer()         |m_right.integer()         )
        :            sc_value(                                                    )
        ;
}


sc_value
sc_binary_operation::functor::
operate_xor() noexcept
{
  auto  c = convert_values();

  return (c == 'u')? sc_value(m_left.unsigned_integer()^m_right.unsigned_integer())
        :(c == 'i')? sc_value(m_left.integer()         ^m_right.integer()         )
        :            sc_value(                                                    )
        ;
}


sc_value
sc_binary_operation::functor::
operate_eq() noexcept
{
  return is_floating()?         m_left.floating()         == m_right.floating()
        :is_unsigned_integer()? m_left.unsigned_integer() == m_right.unsigned_integer()
        :is_integer()?          m_left.integer()          == m_right.integer()
        :is_pointer()?          m_left.integer()          == m_right.integer()
        :false
        ;
}


sc_value
sc_binary_operation::functor::
operate_neq() noexcept
{
  return is_floating()?         m_left.floating()         != m_right.floating()
        :is_unsigned_integer()? m_left.unsigned_integer() != m_right.unsigned_integer()
        :is_integer()?          m_left.integer()          != m_right.integer()
        :is_pointer()?          m_left.integer()          != m_right.integer()
        :false
        ;
}


sc_value
sc_binary_operation::functor::
operate_lt() noexcept
{
  return is_floating()?         m_left.floating()         < m_right.floating()
        :is_unsigned_integer()? m_left.unsigned_integer() < m_right.unsigned_integer()
        :is_integer()?          m_left.integer()          < m_right.integer()
        :is_pointer()?          m_left.integer()          < m_right.integer()
        :false
        ;
}


sc_value
sc_binary_operation::functor::
operate_lteq() noexcept
{
  return is_floating()?         m_left.floating()         <= m_right.floating()
        :is_unsigned_integer()? m_left.unsigned_integer() <= m_right.unsigned_integer()
        :is_integer()?          m_left.integer()          <= m_right.integer()
        :is_pointer()?          m_left.integer()          <= m_right.integer()
        :false
        ;
}


sc_value
sc_binary_operation::functor::
operate_gt() noexcept
{
  return is_floating()?         m_left.floating()         > m_right.floating()
        :is_unsigned_integer()? m_left.unsigned_integer() > m_right.unsigned_integer()
        :is_integer()?          m_left.integer()          > m_right.integer()
        :is_pointer()?          m_left.integer()          > m_right.integer()
        :false
        ;
}


sc_value
sc_binary_operation::functor::
operate_gteq() noexcept
{
  return is_floating()?         m_left.floating()         >= m_right.floating()
        :is_unsigned_integer()? m_left.unsigned_integer() >= m_right.unsigned_integer()
        :is_integer()?          m_left.integer()          >= m_right.integer()
        :is_pointer()?          m_left.integer()          >= m_right.integer()
        :false
        ;
}


sc_value
sc_binary_operation::functor::
operate_logical_and() noexcept
{
  return sc_value(m_left.integer() && m_right.integer());
}


sc_value
sc_binary_operation::functor::
operate_logical_or() noexcept
{
  return sc_value(m_left.integer() || m_right.integer());
}




sc_value
sc_binary_operation::
operate_assign(const sc_value&  l, sc_value&&  r, sc_context&  ctx) noexcept
{
    if(l.type_info().is_reference())
    {
      ctx.store(l.integer(),remove_reference(l.type_info()),ctx.dereference(r));
    }


  return sc_value();
}




}




