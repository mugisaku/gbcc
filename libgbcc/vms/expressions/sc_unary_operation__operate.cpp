#include"libgbcc/vms/context.hpp"




namespace gbcc{




sc_value
sc_unary_operation::
operate(const sc_value&  v, const postfix_element&  e, sc_context&  ctx) noexcept
{
    if(e.is_call())
    {
      auto  deref_v = ctx.dereference(v);

        if(deref_v.type_info().is_function_pointer())
        {
          auto  d = ctx.call(deref_v.integer(),e.expression_list());

          return ctx.run(d);
        }
    }

  else
    if(e.is_index())
    {
    }

  else
    if(e.is_access())
    {
        if(!v.type_info().is_reference())
        {
          return sc_value();
        }
    }

  else
    if(e.is_increment())
    {
        if(!v.type_info().is_reference())
        {
          printf("cannot increase because value is not reference");
        }

      else
        {
          auto  deref_v = ctx.dereference(v);

            if(deref_v.type_info().is_integer())
            {
              sc_binary_operation  bin(u"+=",sc_expression(sc_operand(v)),sc_expression(sc_operand(static_cast<int64_t>(1))));

              bin.evaluate(ctx);

              return deref_v;
            }
        }
    }

  else
    if(e.is_decrement())
    {
        if(!v.type_info().is_reference())
        {
          printf("cannot decrease because value is not reference");
        }

      else
        {
          auto  deref_v = ctx.dereference(v);

            if(deref_v.type_info().is_integer())
            {
              sc_binary_operation  bin(u"-=",sc_expression(sc_operand(v)),sc_expression(sc_operand(static_cast<int64_t>(1))));

              bin.evaluate(ctx);

              return deref_v;
            }
        }
    }


  return sc_value();
}


sc_value
sc_unary_operation::
operate(const sc_value&  v, const prefix_element&   e, sc_context&  ctx) noexcept
{
    if(e.is_logical_not())
    {
      return sc_value(!v.boolean(ctx));
    }

  else
    if(e.is_bitwise_not())
    {
      return sc_value(~v.unsigned_integer(ctx));
    }

  else
    if(e.is_neg())
    {
        if(v.type_info().is_floating()){return sc_value(-v.floating(ctx));}
      else                             {return sc_value(-v.integer(ctx));}
    }

  else
    if(e.is_address())
    {
        if(!v.type_info().is_reference())
        {
          return sc_value();
        }


      sc_pointer  ptr(remove_reference(v.type_info()),v.integer());

      return sc_value(std::move(ptr));
    }

  else
    if(e.is_dereference())
    {
        if(!v.type_info().is_pointer())
        {
          return sc_value();
        }


      sc_reference  ref(remove_pointer(v.type_info()),v.integer());

      return sc_value(std::move(ref));
    }

  else
    if(e.is_increment())
    {
        if(!v.type_info().is_reference())
        {
          printf("cannot increase because value is not reference");
        }

      else
        {
          auto  deref_v = ctx.dereference(v);

            if(deref_v.type_info().is_integer())
            {
              sc_binary_operation  bin(u"+=",sc_expression(sc_operand(v)),sc_expression(sc_operand(static_cast<int64_t>(1))));

              return bin.evaluate(ctx);
            }
        }
    }

  else
    if(e.is_decrement())
    {
        if(!v.type_info().is_reference())
        {
          printf("cannot decrease because value is not reference");
        }

      else
        {
          auto  deref_v = ctx.dereference(v);

            if(deref_v.type_info().is_integer())
            {
              sc_binary_operation  bin(u"-=",sc_expression(sc_operand(v)),sc_expression(sc_operand(static_cast<int64_t>(1))));

              return bin.evaluate(ctx);
            }
        }
    }


  return sc_value();
}



}




