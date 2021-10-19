#include"libgbcc/vms/context.hpp"




namespace gbcc{




sc_type_info
sc_unary_operation::
operate_type_info(const sc_type_info&  ti, const postfix_element&  e) noexcept
{
    if(e.is_call())
    {
        if(ti.is_function())
        {
          return ti.signature().type_info();
        }
    }

  else
    if(e.is_index())
    {
    }

  else
    if(e.is_access())
    {
        if(!ti.is_reference())
        {
          return sc_type_info();
        }
    }

  else
    if(e.is_increment() ||
       e.is_decrement())
    {
        if(!ti.is_reference())
        {
        }


      return remove_reference(ti);
    }


  return sc_type_info();
}


sc_type_info
sc_unary_operation::
operate_type_info(const sc_type_info&  ti, const prefix_element&  e) noexcept
{
    if(e.is_logical_not())
    {
      return sc_bool64_ti;
    }

  else
    if(e.is_bitwise_not())
    {
      return sc_uint64_ti;
    }

  else
    if(e.is_neg())
    {
        if(ti.is_floating()){return sc_float64_ti;}
      else                  {return   sc_int64_ti;}
    }

  else
    if(e.is_address())
    {
        if(!ti.is_reference())
        {
          return sc_type_info();
        }


      return make_pointer(remove_reference(ti));
    }

  else
    if(e.is_dereference())
    {
        if(!ti.is_pointer())
        {
          return sc_type_info();
        }


      return make_reference(remove_pointer(ti));
    }

  else
    if(e.is_increment() ||
       e.is_decrement())
    {
        if(!ti.is_reference())
        {
        }


      return ti;
    }


  return sc_type_info();
}




sc_value
sc_unary_operation::
operate_value(sc_value  v, const sc_type_info&  ti, const postfix_element&  e, sc_context&  ctx) noexcept
{
    if(e.is_call())
    {
      auto  d = ctx.call(v.integer(),e.expression_list());

      return ctx.run(d);
    }

  else
    if(e.is_index())
    {
    }

  else
    if(e.is_access())
    {
        if(!ti.is_reference())
        {
          return sc_value();
        }
    }

  else
    if(e.is_increment())
    {
        if(!ti.is_reference())
        {
          printf("cannot increase because value is not reference");
        }

      else
        {
          auto  deref_ti = remove_reference(ti);
          auto  deref_v  = ctx.load(v.integer(),deref_ti);

            if(deref_ti.is_integer())
            {
              sc_binary_operation  bin(sc_expression(sc_operand(v,ti)),sc_expression(sc_operand(sc_value(1),sc_int64_ti)),u"+=");

              bin.evaluate(ctx);

              return deref_v;
            }
        }
    }

  else
    if(e.is_decrement())
    {
        if(!ti.is_reference())
        {
          printf("cannot decrease because value is not reference");
        }

      else
        {
          auto  deref_ti = remove_reference(ti);
          auto  deref_v  = ctx.load(v.integer(),deref_ti);

            if(deref_ti.is_integer())
            {
              sc_binary_operation  bin(sc_expression(sc_operand(v,ti)),sc_expression(sc_operand(sc_value(1),sc_int64_ti)),u"-=");

              bin.evaluate(ctx);

              return deref_v;
            }
        }
    }


  return sc_value();
}


sc_value
sc_unary_operation::
operate_value(sc_value  v, const sc_type_info&  ti, const prefix_element&   e, sc_context&  ctx) noexcept
{
    if(e.is_logical_not())
    {
      return sc_value(!v.integer());
    }

  else
    if(e.is_bitwise_not())
    {
      return sc_value(~v.unsigned_integer());
    }

  else
    if(e.is_neg())
    {
        if(ti.is_floating()){return sc_value(-v.floating());}
      else                  {return sc_value(-v.integer());}
    }

  else
    if(e.is_address())
    {
        if(!ti.is_reference())
        {
          return sc_value();
        }


      return sc_value(v.integer());
    }

  else
    if(e.is_dereference())
    {
        if(!ti.is_pointer())
        {
          return sc_value();
        }


      return sc_value(v.integer());
    }

  else
    if(e.is_increment())
    {
        if(!ti.is_reference())
        {
          printf("cannot increase because value is not reference");
        }

      else
        {
          sc_binary_operation  bin(sc_expression(sc_operand(v,ti)),sc_expression(sc_operand(sc_value(1),sc_int64_ti)),u"+=");

          return bin.evaluate(ctx);
        }
    }

  else
    if(e.is_decrement())
    {
        if(!ti.is_reference())
        {
          printf("cannot decrease because value is not reference");
        }

      else
        {
          sc_binary_operation  bin(sc_expression(sc_operand(v,ti)),sc_expression(sc_operand(sc_value(1),sc_int64_ti)),u"-=");

          return bin.evaluate(ctx);
        }
    }


  return sc_value();
}




}




