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
  auto  sym = find_symbol(var_name);

    if(sym)
    {
      store(*sym,v,ti);
    }
}


void
sc_context::
store(const sc_symbol&  sym, sc_value  v, const sc_type_info&  ti) noexcept
{
  int64_t  base = sym.attribute().has_temporary()?  m_frame_stack.back().m_memory_size
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

  std::vector<sc_value_with_type_info>  vals;

    for(auto&  a: args)
    {
      vals.emplace_back(a.evaluate(*this),a.type_info(*this));
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
      auto  ti = sym.expression().type_info(*this);
      auto   v = sym.expression().evaluate(*this);

      store(sym,v,ti);
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


const sc_value_with_type_info&
sc_context::
run(int  depth) noexcept
{
    while(m_frame_stack.size() > depth)
    {
      step();
    }


  return m_returned_value;
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




