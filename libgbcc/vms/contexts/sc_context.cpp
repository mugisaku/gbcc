#include"libgbcc/vms/context.hpp"




namespace gbcc{




sc_context::frame::
frame(const sc_function&  fn, int64_t  memsize) noexcept
{
  m_function = &fn;

  m_memory_size = memsize;

  m_running_stack.emplace_back(fn.block());
}




sc_context&
sc_context::
assign(const syntax_branch&  br) noexcept
{
    for(auto&  e: br)
    {
        if(e.is_branch(u"top_element"))
        {
            for(auto&  ee: e.branch())
            {
                if(ee.is_branch(u"function_declaration"))
                {
                  auto  n = m_function_list.size();

                  m_function_list.emplace_back(construct_function(ee.branch())).set_entry_number(n);
                }

              else
                if(ee.is_branch(u"var_statement"))
                {
                  auto  v = construct_var(ee.branch());

                  push(v.type_info(),v.name(),sc_symbol_attribute()).add_expression(std::move(v.expression()));
                }

              else
                if(ee.is_branch(u"const_statement"))
                {
                  auto  c = construct_const(ee.branch());

                  push(c.type_info(),c.name(),sc_symbol_attribute().add_const()).add_expression(std::move(c.expression()));
                }
            }
        }
    }


  reset();

  return *this;
}


sc_symbol&
sc_context::
push(const sc_type_info&  ti, std::u16string_view  name, sc_symbol_attribute  attr) noexcept
{
  return m_symbol_list.emplace_back(name,sc_type_info(ti),stack_size(),attr);
}


int
sc_context::
stack_size() const noexcept
{
  return m_symbol_list.size()? m_symbol_list.back().next_offset():32;
}


const sc_function*
sc_context::
find_function(std::u16string_view  name) const noexcept
{
    for(auto&  c: m_function_list)
    {
        if(c.name() == name)
        {
          return &c;
        }
    }


  return nullptr;
}


const sc_constant*
sc_context::
find_constant(std::u16string_view  name) const noexcept
{
    for(auto&  c: m_constant_list)
    {
        if(c.name() == name)
        {
          return &c;
        }
    }


  return nullptr;
}


const sc_symbol*
sc_context::
find_symbol(std::u16string_view  name) const noexcept
{
    if(m_frame_stack.size())
    {
        for(auto&  sym: m_frame_stack.back().m_function->symbol_list())
        {
            if(sym.name() == name)
            {
              return &sym;
            }
        }
    }


    for(auto&  sym: m_symbol_list)
    {
        if(sym.name() == name)
        {
          return &sym;
        }
    }


  return nullptr;
}


sc_reference
sc_context::
get_reference(std::u16string_view  name) noexcept
{
  auto  sym = find_symbol(name);

    if(sym)
    {
      int64_t  base = sym->attribute().has_temporary()?  m_frame_stack.back().m_memory_size
                    : 0
                    ;

      return sc_reference(sym->type_info(),base+sym->offset());
    }


  auto  f = find_function(name);

    if(f)
    {
      sc_type_info  ti(sc_signature(f->signature()));

      return sc_reference(std::move(ti),f->entry_number());
    }


  printf("the symbol that named ");

  gbcc::print(name);

  printf(" is not found.\n");

  return sc_reference();
}


sc_value
sc_context::
dereference(const sc_value&  v) noexcept
{
  auto&  ti = v.type_info();

    if(ti.is_reference())
    {
      auto  removed_ti = remove_reference(ti);

        if(removed_ti.is_function_pointer())
        {
          return sc_value(removed_ti,v.integer());
        }


      return load(v.integer(),std::move(removed_ti));
    }


  return v;
}




sc_value
sc_context::
load(int64_t  address, const sc_type_info&  ti) noexcept
{
  auto  a = accessor(address);

       if(ti.is_int8()  || ti.is_bool8() ){return sc_value(ti,static_cast<int64_t>(a.i8()));}
  else if(ti.is_int16() || ti.is_bool16()){return sc_value(ti,static_cast<int64_t>(a.i16()));}
  else if(ti.is_int32() || ti.is_bool32()){return sc_value(ti,static_cast<int64_t>(a.i32()));}
  else if(ti.is_int64() || ti.is_bool64()){return sc_value(ti,a.i64());}
  else if(ti.is_uint8()  || ti.is_char8() ){return sc_value(ti,static_cast<uint64_t>(a.u8()));}
  else if(ti.is_uint16() || ti.is_char16()){return sc_value(ti,static_cast<uint64_t>(a.u16()));}
  else if(ti.is_uint32() || ti.is_char32()){return sc_value(ti,static_cast<uint64_t>(a.u32()));}
  else if(ti.is_uint64()                  ){return sc_value(ti,a.u64());}
  else if(ti.is_float32()){return sc_value(ti,static_cast<double>(a.f32()));}
  else if(ti.is_float64()){return sc_value(ti,a.f64());}

  return sc_value();
}




void
sc_context::
store(std::u16string_view  var_name, sc_value  v) noexcept
{
  auto  sym = find_symbol(var_name);

    if(sym)
    {
      store(*sym,std::move(v));
    }
}


void
sc_context::
store(const sc_symbol&  sym, sc_value  v) noexcept
{
  int64_t  base = sym.attribute().has_temporary()?  m_frame_stack.back().m_memory_size
                 :0
                 ;
 
  store(base+sym.offset(),sym.type_info(),std::move(v));
}


void
sc_context::
store(int64_t  address, const sc_type_info&  ti, sc_value  v) noexcept
{
  auto&  vti = v.type_info();

  auto  a = accessor(address);

       if(vti.is_int8()  || vti.is_bool8() ){a.i8()  = v.integer();}
  else if(vti.is_int16() || vti.is_bool16()){a.i16() = v.integer();}
  else if(vti.is_int32() || vti.is_bool32()){a.i32() = v.integer();}
  else if(vti.is_int64() || vti.is_bool64()){a.i64() = v.integer();}
  else if(vti.is_uint8()  || vti.is_char8() ){a.u8()  = v.unsigned_integer();}
  else if(vti.is_uint16() || vti.is_char16()){a.u16() = v.unsigned_integer();}
  else if(vti.is_uint32() || vti.is_char32()){a.u32() = v.unsigned_integer();}
  else if(vti.is_uint64()                  ){a.u64() = v.unsigned_integer();}
  else if(vti.is_float32()){a.f32() = v.floating();}
  else if(vti.is_float64()){a.f64() = v.floating();}
}




int
sc_context::
call(std::u16string_view  fn_name, const sc_expression_list&  args) noexcept
{
  auto  f = find_function(fn_name);

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

  std::vector<sc_value>  vals;

    for(auto&  a: args)
    {
      vals.emplace_back(a.evaluate(*this));
    }


  auto  n = m_frame_stack.size();

  auto  sz = m_memory.size();

  m_frame_stack.emplace_back(fn,sz);

  auto&  syms = fn.symbol_list();

    if(syms.size())
    {
      sz += syms.back().next_offset();
    }


  m_memory.resize(sz);

  auto  it = fn.signature().parameter_list().begin();

    for(auto&&  v: vals)
    {
      store(it++->name(),std::move(v));
    }


  return n;
}


int
sc_context::
call(int  entry_number, const sc_expression_list&  args) noexcept
{
  auto&  fn = m_function_list[entry_number];

  return call(fn,args);
}


void
sc_context::
exit_from_block() noexcept
{
  auto&  r_stk = m_frame_stack.back().m_running_stack;

  r_stk.pop_back();

    if(r_stk.empty())
    {
      process_return();
    }
}


void
sc_context::
reset() noexcept
{
  m_memory.resize(stack_size());

  m_frame_stack.clear();

    for(auto&  sym: m_symbol_list)
    {
      auto  v = sym.expression().evaluate(*this);

      store(sym,std::move(v));
    }
}


void
sc_context::
step() noexcept
{
    if(m_frame_stack.size())
    {
      auto&  frm = m_frame_stack.back();

      auto&  r_stk = frm.m_running_stack;

      auto&  r = r_stk.back();

        if(r.m_current == r.m_begin)
        {
          auto  cond = r.m_condition;

            if(cond && *cond)
            {
              auto  v = cond->evaluate(*this);

                if(!v.integer())
                {
                  exit_from_block();

                  return;
                }
            }
        }


        if(r.m_current < r.m_end)
        {
          process(*r.m_current++);
        }

      else
        {
            if(r.m_condition)
            {
              r.m_current = r.m_begin;
            }

          else
            {
              exit_from_block();
            }
        }
    }

  else
    {
      halt();
    }
}


sc_value
sc_context::
run(int  depth) noexcept
{
    while(m_frame_stack.size() > depth)
    {
      step();
    }


  return std::move(m_returned_value);
}


void
sc_context::
print() const noexcept
{
    for(auto&  sym: m_symbol_list)
    {
      printf("var  ");

      sym.print();

      printf("\n");
    }


  printf("\n\n");

    for(auto&  fn: m_function_list)
    {
      fn.print();

      printf("\n");
    }
}




}




