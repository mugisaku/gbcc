#include"libgbcc/vms/context.hpp"




namespace gbcc{




sc_reference
sc_context::
get_reference(const sc_symbol&  sym) const noexcept
{
  int64_t  base = sym.attribute().has_temporary()?  m_fef_stack.back().previous_memory_size()
                : 0
                ;

  return sc_reference(sym.type_info(),base+sym.offset());
}


sc_value
sc_context::
dereference(sc_value  v, const sc_type_info&  ti) noexcept
{
    if(ti.is_reference())
    {
      return load(v.integer(),remove_reference(ti));
    }


  return v;
}


sc_value
sc_context::
load(int64_t  address, const sc_type_info&  ti) noexcept
{
  auto  a = accessor(address);

  auto  sz = ti.size();

    if(ti.is_integer() || ti.is_pointer() || ti.is_boolean())
    {
      return (sz == 1)? sc_value(a.i8() )
            :(sz == 2)? sc_value(a.i16())
            :(sz == 4)? sc_value(a.i32())
            :(sz == 8)? sc_value(a.i64())
            :sc_value()
            ;
    }

  else
    if(ti.is_unsigned_integer())
    {
      return (sz == 1)? sc_value(a.u8() )
            :(sz == 2)? sc_value(a.u16())
            :(sz == 4)? sc_value(a.u32())
            :(sz == 8)? sc_value(a.u64())
            :sc_value()
            ;
    }

  else
    if(ti.is_floating())
    {
      return (sz == 4)? sc_value(a.f32())
            :(sz == 8)? sc_value(a.f64())
            :sc_value()
            ;
    }


  return sc_value();
}




void
sc_context::
store(std::u16string_view  var_name, sc_value  v, const sc_type_info&  ti) noexcept
{
  auto  sym = m_package->find_symbol(var_name);

    if(sym)
    {
      store(*sym,v,ti);
    }
}


void
sc_context::
store(const sc_symbol&  sym, sc_value  v, const sc_type_info&  ti) noexcept
{
  int64_t  base = sym.attribute().has_temporary()?  m_fef_stack.back().previous_memory_size()
                 :0
                 ;
 
  store(base+sym.offset(),sym.type_info(),v,ti);
}


void
sc_context::
store(int64_t  address, const sc_type_info&  dst_ti, sc_value  v, const sc_type_info&  ti) noexcept
{
  auto  a = accessor(address);

  auto  cv = v.convert(ti,dst_ti,*this);

  auto  sz = dst_ti.size();

    if(dst_ti.is_integer() || dst_ti.is_pointer() || dst_ti.is_boolean())
    {
      auto  i = cv.integer();

           if(sz == 1){a.i8()  = i;}
      else if(sz == 2){a.i16() = i;}
      else if(sz == 4){a.i32() = i;}
      else if(sz == 8){a.i64() = i;}
    }

  else
    if(dst_ti.is_unsigned_integer())
    {
      auto  u = cv.unsigned_integer();

           if(sz == 1){a.u8()  = u;}
      else if(sz == 2){a.u16() = u;}
      else if(sz == 4){a.u32() = u;}
      else if(sz == 8){a.u64() = u;}
    }

  else
    if(dst_ti.is_floating())
    {
      auto  f = cv.floating();

           if(sz == 4){a.f32() = f;}
      else if(sz == 8){a.f64() = f;}
    }
}




int
sc_context::
call(std::u16string_view  fn_name, const sc_expression_list&  args) noexcept
{
  auto  f = m_package->find_function(fn_name);

    if(f)
    {
      return call(*f,args);
    }


  return -1;
}


int
sc_context::
call(const sc_function&  fn, const sc_expression_list&  args) noexcept
{
  m_halt_flag = false;

  std::vector<sc_value_with_type_info>  vals;

    for(auto&  a: args)
    {
      vals.emplace_back(a.evaluate(*this),a.type_info());
    }


  auto  n = m_fef_stack.size();

  auto  sz = m_memory.size();

  m_fef_stack.emplace_back(fn,sz);

  auto&  syms = fn.symbol_list();

    if(syms.size())
    {
      sz += syms.back().next_offset();
    }


  m_memory.resize(sz);

  auto  it = fn.signature().parameter_list().begin();

    for(auto&  v: vals)
    {
      auto&  para = *it++;

      store(para.name(),v,v.type_info());
    }


  return n;
}


int
sc_context::
call(int  entry_number, const sc_expression_list&  args) noexcept
{
  auto&  fn = *m_package->function_list()[entry_number];

  return call(fn,args);
}


void
sc_context::
reset() noexcept
{
  m_memory.resize(m_package->stack_size());

  m_fef_stack.clear();

    for(auto&  var: m_package->variable_list())
    {
      auto&  ti = var.expression().type_info();
      auto    v = var.expression().evaluate(*this);

      store(var.symbol(),v,ti);
    }
}


void
sc_context::
step() noexcept
{
    if(m_fef_stack.size())
    {
      auto&  frm = m_fef_stack.back();

      frm(*this,m_returned_value);

        if(frm.is_returned())
        {
          m_memory.resize(frm.previous_memory_size());

          m_fef_stack.pop_back();
        }
    }

  else
    {
      halt();
    }
}


const sc_value_with_type_info&
sc_context::
run(int  depth) noexcept
{
    while(m_fef_stack.size() > depth)
    {
      step();
    }


  return m_returned_value;
}




}




