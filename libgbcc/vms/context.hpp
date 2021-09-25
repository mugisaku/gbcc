#ifndef gbstd_vm_context_HPP
#define gbstd_vm_context_HPP


#include"libgbcc/vms/expression.hpp"




namespace gbcc{




class sc_statement;

using sc_statement_list = std::vector<sc_statement>;


class
sc_block
{
  sc_statement_list  m_statement_list;

public:
  sc_block() noexcept{}
  sc_block(sc_statement_list&&  ls) noexcept: m_statement_list(std::move(ls)){}

  const sc_statement_list&  statement_list() const noexcept{return m_statement_list;}

  const sc_statement*  begin() const noexcept;
  const sc_statement*    end() const noexcept;

  void  print(int  depth=0) const noexcept;

};


class
sc_conditional_block: public sc_block
{
  sc_expression  m_expression;

public:
  sc_conditional_block() noexcept{}
  sc_conditional_block(sc_expression&&  e, sc_block&&  blk) noexcept: m_expression(std::move(e)), sc_block(std::move(blk)){}

  const sc_expression&  expression() const noexcept{return m_expression;}

  void  print(int  depth=0) const noexcept
  {
    printf("(");

    m_expression.print();

    printf(")\n");

    sc_block::print(depth);
  }

};


using sc_conditional_block_list = std::vector<sc_conditional_block>;


class
sc_if
{
  sc_conditional_block_list  m_block_list;

public:
  sc_if() noexcept{}
  sc_if(sc_conditional_block_list&&  ls) noexcept: m_block_list(std::move(ls)){}

  sc_conditional_block_list  operator*() noexcept{return std::move(m_block_list);}

};


class
sc_while: public sc_conditional_block
{
public:
  using sc_conditional_block::sc_conditional_block;

};


class
sc_for: public sc_conditional_block
{
public:
  using sc_conditional_block::sc_conditional_block;

};


class
sc_switch: public sc_conditional_block
{
public:
  using sc_conditional_block::sc_conditional_block;

};


struct sc_break{};
struct sc_continue{};
struct sc_default{};


class
sc_expression_wrapper
{
  sc_expression  m_expression;

public:
  sc_expression_wrapper() noexcept{}
  sc_expression_wrapper(sc_expression&&  e) noexcept: m_expression(std::move(e)){}

  sc_expression  operator*() noexcept{return std::move(m_expression);}

};


class
sc_label
{
  std::u16string  m_string;

public:
  sc_label() noexcept;
  sc_label(std::u16string_view  sv) noexcept: m_string(sv){}

  std::u16string  operator*() noexcept{return std::move(m_string);}

};


class
sc_goto: public sc_label
{
public:
  using sc_label::sc_label;

};


class
sc_return: public sc_expression_wrapper
{
public:
  using sc_expression_wrapper::sc_expression_wrapper;

};


class
sc_case: public sc_expression_wrapper
{
public:
  using sc_expression_wrapper::sc_expression_wrapper;

};


class
sc_var
{
  std::u16string   m_name;

  sc_type_info  m_type_info;

  sc_expression  m_expression;

public:
  sc_var() noexcept{}
  sc_var(std::u16string_view  name, sc_type_info&&  ti, sc_expression&&  e) noexcept: m_type_info(std::move(ti)), m_name(name), m_expression(std::move(e)){}

  const std::u16string&  name() const noexcept{return m_name;}

  const sc_type_info&  type_info() const noexcept{return m_type_info;}

  const sc_expression&  expression() const noexcept{return m_expression;}

};


class
sc_const: public sc_var
{
public:
  using sc_var::sc_var;

};


class
sc_statement
{
  enum class kind{
    null,
    return_,
    break_,
    continue_,
    goto_,
    label,
    if_,
    for_,
    while_,
    switch_,
    case_,
    default_,
    var,
    const_,
    block,
    expression,
    
  } m_kind=kind::null;

  union data{
    std::u16string  s;

    sc_block                      blk;
    sc_conditional_block         cblk;
    sc_conditional_block_list  cblkls;

    sc_expression  expr;

    sc_var  var;

    data() noexcept{}
   ~data(){}
  } m_data;

public:
  sc_statement() noexcept{}
 ~sc_statement(){clear();}

  sc_statement(const sc_statement&   rhs) noexcept{assign(rhs);}
  sc_statement(      sc_statement&&  rhs) noexcept{assign(std::move(rhs));}

  template<class...  Args>
  sc_statement(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  sc_statement&  operator=(const sc_statement&   rhs) noexcept{return assign(rhs);}
  sc_statement&  operator=(      sc_statement&&  rhs) noexcept{return assign(std::move(rhs));}

  template<class...  Args>
  sc_statement&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  sc_statement&  assign(const sc_statement&   rhs) noexcept;
  sc_statement&  assign(      sc_statement&&  rhs) noexcept;
  sc_statement&  assign(sc_return&&  ret) noexcept;
  sc_statement&  assign(sc_break&&  brk) noexcept;
  sc_statement&  assign(sc_continue&&  ctn) noexcept;
  sc_statement&  assign(sc_label&&  lb) noexcept;
  sc_statement&  assign(sc_goto&&  gt) noexcept;
  sc_statement&  assign(sc_if&&  i) noexcept;
  sc_statement&  assign(sc_while&&  wh) noexcept;
  sc_statement&  assign(sc_for&&  fo) noexcept;
  sc_statement&  assign(sc_switch&&  sw) noexcept;
  sc_statement&  assign(sc_case&&  ca) noexcept;
  sc_statement&  assign(sc_default&&  de) noexcept;
  sc_statement&  assign(sc_block&&  blk) noexcept;
  sc_statement&  assign(sc_expression&&  e) noexcept;
  sc_statement&  assign(sc_var&&  v) noexcept;
  sc_statement&  assign(sc_const&&  c) noexcept;

  void  clear() noexcept;

  bool  is_return()   const noexcept{return m_kind == kind::return_;}
  bool  is_break()    const noexcept{return m_kind == kind::break_;}
  bool  is_continue() const noexcept{return m_kind == kind::continue_;}
  bool  is_goto()     const noexcept{return m_kind == kind::goto_;}
  bool  is_label()    const noexcept{return m_kind == kind::label;}
  bool  is_if()       const noexcept{return m_kind == kind::if_;}
  bool  is_while()    const noexcept{return m_kind == kind::while_;}
  bool  is_for()      const noexcept{return m_kind == kind::for_;}
  bool  is_var()      const noexcept{return m_kind == kind::var;}
  bool  is_const()    const noexcept{return m_kind == kind::const_;}
  bool  is_switch()   const noexcept{return m_kind == kind::switch_;}
  bool  is_case()     const noexcept{return m_kind == kind::case_;}
  bool  is_default()  const noexcept{return m_kind == kind::default_;}
  bool  is_block()      const noexcept{return m_kind == kind::block;}
  bool  is_expression() const noexcept{return m_kind == kind::expression;}

  const std::u16string&  string() const noexcept{return m_data.s;}

  const sc_block&                                    block() const noexcept{return m_data.blk;}
  const sc_conditional_block&            conditional_block() const noexcept{return m_data.cblk;}
  const sc_conditional_block_list&  conditional_block_list() const noexcept{return m_data.cblkls;}

  const sc_var&       var() const noexcept{return m_data.var;}
  const sc_const&  const_() const noexcept{return static_cast<const sc_const&>(m_data.var);}

  const sc_expression&  expression() const noexcept{return m_data.expr;}

  void  print(int  depth=0) const noexcept;

};


using sc_statement_list = std::vector<sc_statement>;




class
sc_constant
{
  std::u16string  m_name;

  sc_value  m_value;

public:
  sc_constant() noexcept{}
  sc_constant(std::u16string_view  name, sc_value&&  v) noexcept: m_name(name), m_value(std::move(v)){}

  sc_constant&  assign(std::u16string_view  name, sc_value&&  v) noexcept{
    m_name = name;
    m_value = std::move(v);
    return *this;
  }

  const std::u16string&  name() const noexcept{return m_name;}

  const sc_value&  value() const noexcept{return m_value;}

  void  print() const noexcept{
    gbcc::print(m_name);
    printf("(");
    m_value.print();
    printf(")");
  }

};


using sc_constant_list = std::vector<sc_constant>;


class
sc_symbol_attribute
{
  int  m_data=0;

  static constexpr int  m_temporary_flag = 1;
  static constexpr int  m_const_flag     = 2;

public:
  constexpr sc_symbol_attribute(int  dat=0) noexcept: m_data(dat){}

  constexpr sc_symbol_attribute  add_temporary() const noexcept{return sc_symbol_attribute(m_data|m_temporary_flag);}
  constexpr sc_symbol_attribute  add_const()  const noexcept{return sc_symbol_attribute(m_data|m_const_flag);}

  constexpr bool  has_temporary() const noexcept{return (m_data&m_temporary_flag);}
  constexpr bool  has_const()  const noexcept{return (m_data&m_const_flag);}

};


class
sc_symbol
{
  std::u16string  m_name;

  sc_type_info  m_type_info;

  int64_t  m_offset=0;

  sc_symbol_attribute  m_attribute;

  static constexpr int  m_asize = sizeof(int64_t);

  static constexpr int64_t  aligned(int64_t  off) noexcept{return (off+(m_asize-1))/m_asize*m_asize;}

public:
  sc_symbol() noexcept{}
  sc_symbol(std::u16string_view  name, sc_type_info&&  ti, int64_t  off, sc_symbol_attribute  attr) noexcept:
  m_name(name), m_type_info(std::move(ti)), m_offset(off), m_attribute(attr){}

  const std::u16string&  name() const noexcept{return m_name;}

  const sc_type_info&  type_info() const noexcept{return m_type_info;}

  int64_t       offset() const noexcept{return m_offset;}
  int64_t  next_offset() const noexcept{return aligned(m_offset+m_type_info.size());}

  sc_symbol_attribute  attribute() const noexcept{return m_attribute;}

  void  print() const noexcept
  {
    gbcc::print(m_name);

    printf(" ");

    m_type_info.print();

    printf("(%6" PRIi64 ")",m_offset);
  }

};


using sc_symbol_list = std::vector<sc_symbol>;




class
sc_function
{
  std::u16string  m_name;

  sc_signature  m_signature;

  sc_block  m_block;

  sc_symbol_list  m_symbol_list;

  int  m_entry_number=-1;

  void  scan(const sc_block&  blk) noexcept;

  void  push(const sc_type_info&  ti, std::u16string_view  name) noexcept;

public:
  sc_function() noexcept{}
  sc_function(std::u16string_view  name, sc_type_info&&  ti, sc_parameter_list&&  parals, sc_block&&  blk) noexcept:
  m_name(name), m_signature(std::move(ti),std::move(parals)), m_block(std::move(blk)){update_symbol_list();}

  const std::u16string&  name() const noexcept{return m_name;}

  const sc_signature&  signature() const noexcept{return m_signature;}

  const sc_block&  block() const noexcept{return m_block;}

  const sc_symbol_list&  symbol_list() const noexcept{return m_symbol_list;}

  void  set_entry_number(int  n) noexcept{m_entry_number = n;}

  int  entry_number() const noexcept{return m_entry_number;}

  void  update_symbol_list() noexcept;

  int  stack_size() const noexcept;

  void  print() const;

};


using sc_function_list = std::vector<sc_function>;




class
sc_accessor
{
  void*  m_pointer;

public:
  constexpr sc_accessor(void*  ptr) noexcept: m_pointer(ptr){}

  int8_t&   i8()  const noexcept{return *static_cast<int8_t*>(m_pointer);}
  int16_t&  i16() const noexcept{return *static_cast<int16_t*>(m_pointer);}
  int32_t&  i32() const noexcept{return *static_cast<int32_t*>(m_pointer);}
  int64_t&  i64() const noexcept{return *static_cast<int64_t*>(m_pointer);}

  uint8_t&   u8()  const noexcept{return *static_cast<uint8_t*>(m_pointer);}
  uint16_t&  u16() const noexcept{return *static_cast<uint16_t*>(m_pointer);}
  uint32_t&  u32() const noexcept{return *static_cast<uint32_t*>(m_pointer);}
  uint64_t&  u64() const noexcept{return *static_cast<uint64_t*>(m_pointer);}

  float&   f32() const noexcept{return *static_cast<float*>(m_pointer);}
  double&  f64() const noexcept{return *static_cast<double*>(m_pointer);}

};



class
sc_context
{
  std::vector<uint8_t>  m_memory;

  int64_t  m_memory_size=0;

  sc_function_list  m_function_list;
  sc_constant_list  m_constant_list;

  sc_symbol_list  m_symbol_list;

  struct running{
    const sc_statement*  m_begin;
    const sc_statement*  m_current;
    const sc_statement*  m_end;

    const sc_expression*  m_condition=nullptr;
    const sc_expression*  m_step=nullptr;

    running(const sc_block&  blk) noexcept:
    m_begin(blk.begin()), m_current(blk.begin()), m_end(blk.end()){}

    running(const sc_conditional_block&  cblk) noexcept:
    m_begin(cblk.begin()), m_current(cblk.begin()), m_end(cblk.end()), m_condition(&cblk.expression()){}

    void  rewind() noexcept{m_current = m_begin;}

  };

  struct frame{
    int64_t  m_memory_size;

    const sc_function*  m_function;

    std::vector<running>  m_running_stack;

    frame(const sc_function&  fn, int64_t  memsize) noexcept;

  };


  std::vector<frame>  m_frame_stack;

  sc_value  m_returned_value;

  bool  m_halt_flag;

  static sc_return                  construct_return(const syntax_branch&  br) noexcept;
  static sc_for                        construct_for(const syntax_branch&  br) noexcept;
  static sc_while                    construct_while(const syntax_branch&  br) noexcept;
  static sc_switch                  construct_switch(const syntax_branch&  br) noexcept;
  static sc_case                      construct_case(const syntax_branch&  br) noexcept;
  static sc_if                          construct_if(const syntax_branch&  br) noexcept;
  static sc_label                    construct_label(const syntax_branch&  br) noexcept;
  static sc_goto                      construct_goto(const syntax_branch&  br) noexcept;
  static sc_var                        construct_var(const syntax_branch&  br) noexcept;
  static sc_const                    construct_const(const syntax_branch&  br) noexcept;
  static sc_block                    construct_block(const syntax_branch&  br) noexcept;
  static sc_parameter            construct_parameter(const syntax_branch&  br) noexcept;
  static sc_parameter_list  construct_parameter_list(const syntax_branch&  br) noexcept;
  static sc_type_info            construct_type_info(const syntax_branch&  br) noexcept;
  static sc_function              construct_function(const syntax_branch&  br) noexcept;

  void  process(const sc_statement&  st) noexcept;

  void  process_return(sc_value  v=sc_value()) noexcept;
  void  process_while(const sc_conditional_block&  cblk) noexcept;
//  void  process_for() noexcept;
  void  process_switch(const sc_conditional_block&  cblk) noexcept;
  void  process_goto(std::u16string_view  dst) noexcept;
  void  process_if(const sc_conditional_block_list&  cblkls) noexcept;
  void  process_break() noexcept;
  void  process_continue() noexcept;
  void  process_block(const sc_block&  blk) noexcept;
  void  process_var(const sc_var&  v) noexcept;
  void  process_const(const sc_const&  c) noexcept;

  void  exit_from_block() noexcept;

public:
  sc_context() noexcept{}

  sc_context(const syntax_branch&  br) noexcept{assign(br);}
  sc_context&  operator=(const syntax_branch&  br) noexcept{return assign(br);}

  sc_context&  assign(const syntax_branch&  br) noexcept;

  const sc_function*  find_function(std::u16string_view  name) const noexcept;
  const sc_constant*  find_constant(std::u16string_view  name) const noexcept;
  const sc_symbol*      find_symbol(std::u16string_view  name) const noexcept;

  sc_reference  get_reference(std::u16string_view  name) noexcept;

  sc_value  dereference(const sc_value&  v) noexcept;

  void  add_constant(std::u16string_view  name, sc_value&&  v) noexcept{m_constant_list.emplace_back(name,std::move(v));}

  sc_accessor  accessor(int64_t  address) noexcept{return sc_accessor(&m_memory[address]);}

  sc_value  load(int64_t  address, const sc_type_info&  ti) noexcept;

  void  store(int64_t  address, sc_value  v) noexcept;

  int  call(std::u16string_view  fn_name, const sc_expression_list&  args) noexcept;
  int  call(const sc_function&  fn, const sc_expression_list&  args) noexcept;
  int  call(int  entry_number, const sc_expression_list&  args) noexcept;

  void  step() noexcept;

  sc_value  run(int  depth) noexcept;

  void  halt() noexcept{m_halt_flag = true;}
  bool  is_halted() const noexcept{return m_halt_flag;}

  void  print() const noexcept;

};




}




#endif




