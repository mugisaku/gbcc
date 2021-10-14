#include"libgbcc/vms/context.hpp"




namespace gbcc{




sc_value
sc_value::
convert(const sc_type_info&  source, const sc_type_info&  target, sc_context&  ctx, bool  forced) const noexcept
{
    if(source == target)
    {
      return *this;
    }


    if(source.is_reference())
    {
      auto  deref_ti = remove_reference(source);

      auto  v = ctx.load(i,deref_ti);

      return v.convert(deref_ti,target,ctx);
    }

  else
    {
        if(target.is_integer())
        {
               if(source.is_unsigned_integer()){return static_cast<uint64_t>(u);}
          else if(source.is_floating()        ){return static_cast<uint64_t>(f);}
          else if(source.is_pointer()         ){return static_cast<uint64_t>(i);}
        }

      else
        if(target.is_unsigned_integer())
        {
               if(source.is_integer() ){return static_cast<uint64_t>(i);}
          else if(source.is_floating()){return static_cast<uint64_t>(f);}
          else if(source.is_pointer() ){return static_cast<uint64_t>(i);}
        }

      else
        if(target.is_floating())
        {
               if(source.is_integer()         ){return static_cast<double>(i);}
          else if(source.is_unsigned_integer()){return static_cast<double>(u);}
        }

      else
        if(target.is_pointer())
        {
            if(source.is_integer() ||
               source.is_unsigned_integer() ||
               source.is_pointer() ||
               source.is_null_pointer())
            {
              return i;
            }
        }

      else
        if(target.is_boolean())
        {
            if(source.is_integer() ||
               source.is_unsigned_integer() ||
               source.is_pointer() ||
               source.is_null_pointer())
            {
              return i;
            }
        }
    }


  return sc_value();
}


void
sc_value::
print(const sc_type_info&  ti) const noexcept
{
    if(!ti)
    {
      printf("null");
    }

  else
    if(ti.is_undefined())
    {
      printf("undefined");
    }

  else
    if(ti.is_void())
    {
      printf("void");
    }

  else
    if(ti.is_null_pointer())
    {
      printf("null_pointer");
    }

  else
    if(ti.is_boolean())
    {
      printf("%s",i? "true":"false");
    }

  else
    if(ti.is_integer())
    {
      printf("%" PRIi64 "i",i);
    }

  else
    if(ti.is_unsigned_integer())
    {
      printf("%" PRIu64 "u",u);
    }

  else
    if(ti.is_floating())
    {
      printf("%ff",f);
    }

  else
    if(ti.is_pointer())
    {
      printf("%" PRIi64 "p",i);
    }

  else
    if(ti.is_reference())
    {
      printf("%" PRIi64 "r",i);
    }

  else
    if(ti.is_character())
    {
      printf("%" PRIi64 "c",u);
    }

  else
    {
      printf("other_type_value");
    }
}




}




