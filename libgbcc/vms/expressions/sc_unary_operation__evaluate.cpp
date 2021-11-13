#include"libgbcc/vms/expression.hpp"
#include"libgbcc/vms/context.hpp"




namespace gbcc{




sc_value
sc_unary_operation::
evaluate(sc_context&  ctx) const noexcept
{
    if(m_constant_value)
    {
      return *m_constant_value;
    }


  auto  v = m_expression.evaluate(ctx);

    if(is_nop())
    {
      return v;
    }

  else
    if(is_call())
    {
      auto  d = ctx.call(v.integer(),expression_list());

      return ctx.run(d);
    }

  else
    if(is_index())
    {
      
    }

  else
    if(is_access())
    {
        if(!m_type_info.is_reference())
        {
          return sc_value();
        }
    }

  else
    if(is_postfix_increment())
    {
        if(!m_type_info.is_reference())
        {
          printf("cannot increase because value is not reference");
        }

      else
        {
          auto  deref_ti = remove_reference(m_type_info);
          auto  deref_v  = ctx.load(v.integer(),deref_ti);

            if(deref_ti.is_integer())
            {
              sc_binary_operation  bin(sc_expression(sc_operand(v,m_type_info)),sc_expression(sc_operand(sc_value(1),sc_int64_ti)),u"+=");

              bin.evaluate(ctx);

              return deref_v;
            }
        }
    }

  else
    if(is_postfix_decrement())
    {
        if(!m_type_info.is_reference())
        {
          printf("cannot decrease because value is not reference");
        }

      else
        {
          auto  deref_ti = remove_reference(m_type_info);
          auto  deref_v  = ctx.load(v.integer(),deref_ti);

            if(deref_ti.is_integer())
            {
              sc_binary_operation  bin(sc_expression(sc_operand(v,m_type_info)),sc_expression(sc_operand(sc_value(1),sc_int64_ti)),u"-=");

              bin.evaluate(ctx);

              return deref_v;
            }
        }
    }

  else
    if(is_logical_not())
    {
      return sc_value(!v.integer());
    }

  else
    if(is_bitwise_not())
    {
      return sc_value(~v.unsigned_integer());
    }

  else
    if(is_neg())
    {
        if(m_type_info.is_floating()){return sc_value(-v.floating());}
      else                           {return sc_value(-v.integer());}
    }

  else
    if(is_address())
    {
        if(!m_type_info.is_reference())
        {
          return sc_value();
        }


      return sc_value(v.integer());
    }

  else
    if(is_dereference())
    {
        if(!m_type_info.is_pointer())
        {
          return sc_value();
        }


      return sc_value(v.integer());
    }

  else
    if(is_prefix_increment())
    {
        if(!m_type_info.is_reference())
        {
          printf("cannot increase because value is not reference");
        }

      else
        {
          sc_binary_operation  bin(sc_expression(sc_operand(v,m_type_info)),sc_expression(sc_operand(sc_value(1),sc_int64_ti)),u"+=");

          return bin.evaluate(ctx);
        }
    }

  else
    if(is_prefix_decrement())
    {
        if(!m_type_info.is_reference())
        {
          printf("cannot decrease because value is not reference");
        }

      else
        {
          sc_binary_operation  bin(sc_expression(sc_operand(v,m_type_info)),sc_expression(sc_operand(sc_value(1),sc_int64_ti)),u"-=");

          return bin.evaluate(ctx);
        }
    }


  return sc_value();
}




}




