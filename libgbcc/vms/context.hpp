#ifndef gbstd_vm_context_HPP
#define gbstd_vm_context_HPP


#include"libgbcc/vms/expression.hpp"




namespace gbcc{




class sc_statement;
class sc_package;
class sc_function;

using sc_statement_list = std::vector<sc_statement>;


class
sc_block
{
  sc_function*  m_function;

  enum class kind{
    plain, if_, while_, switch_,
  } m_kind;

  sc_expression  m_condition;

  sc_statement_list  m_statement_list;

public:
  sc_block() noexcept{}

  void  reset(sc_function&  fn, std::string_view  k) noexcept;

  const sc_function&  function() const noexcept{return *m_function;}

  template<class...  Args>
  sc_statement&  emplace_back(Args&&...  args) noexcept{return m_statement_list.emplace_back(std::forward<Args>(args)...);}

  bool  is_plain() const noexcept{return m_kind == kind::plain;}
  bool  is_if()    const noexcept{return m_kind == kind::if_;}
  bool  is_while() const noexcept{return m_kind == kind::while_;}
  bool  is_switch() const noexcept{return m_kind == kind::switch_;}

  void  add_condition(sc_expression&&  e) noexcept{m_condition = std::move(e);}

  const sc_expression&  condition() const noexcept{return m_condition;}

        sc_statement_list&  statement_list()       noexcept{return m_statement_list;}
  const sc_statement_list&  statement_list() const noexcept{return m_statement_list;}

  const sc_statement*  begin() const noexcept;
  const sc_statement*    end() const noexcept;

  void  fix(const sc_function&  fn) noexcept;

  void  print(int  depth=0) const noexcept;

};


using sc_block_list = std::vector<std::unique_ptr<sc_block>>;


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
sc_variable
{
  sc_symbol          m_symbol;
  sc_expression  m_expression;

public:
  sc_variable() noexcept{}
  sc_variable(const sc_symbol&  sym) noexcept: m_symbol(sym){}
  sc_variable(const sc_symbol&  sym, sc_expression&&  e) noexcept: m_symbol(sym), m_expression(std::move(e)){}

        sc_symbol&  symbol()       noexcept{return m_symbol;}
  const sc_symbol&  symbol() const noexcept{return m_symbol;}

        sc_expression&  expression()       noexcept{return m_expression;}
  const sc_expression&  expression() const noexcept{return m_expression;}

};


using sc_variable_list = std::vector<sc_variable>;


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
    case_,
    default_,
    var,
    const_,
    block,
    expression,
    
  } m_kind=kind::null;

  union data{
    std::u16string  s;

    std::unique_ptr<sc_block>  blk;

    sc_block_list  blkls;

    sc_expression  expr;

    sc_variable  var;

    data() noexcept{}
   ~data(){}
  } m_data;

public:
  sc_statement() noexcept{}
 ~sc_statement(){clear();}

  sc_statement(const sc_statement&   rhs) noexcept=delete;/*{assign(rhs);}*/
  sc_statement(      sc_statement&&  rhs) noexcept{assign(std::move(rhs));}

  template<class...  Args>
  sc_statement(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

//  sc_statement&  operator=(const sc_statement&   rhs) noexcept{return assign(rhs);}
  sc_statement&  operator=(      sc_statement&&  rhs) noexcept{return assign(std::move(rhs));}

  template<class...  Args>
  sc_statement&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

/*  sc_statement&  assign(const sc_statement&   rhs) noexcept;*/
  sc_statement&  assign(      sc_statement&&  rhs) noexcept;
  sc_statement&  assign(sc_return&&  ret) noexcept;
  sc_statement&  assign(sc_break&&  brk) noexcept;
  sc_statement&  assign(sc_continue&&  ctn) noexcept;
  sc_statement&  assign(sc_label&&  lb) noexcept;
  sc_statement&  assign(sc_goto&&  gt) noexcept;
  sc_statement&  assign(sc_case&&  ca) noexcept;
  sc_statement&  assign(sc_default&&  de) noexcept;
  sc_statement&  assign(std::unique_ptr<sc_block>&&  blk) noexcept;
  sc_statement&  assign(sc_block_list&&  blkls) noexcept;
  sc_statement&  assign(sc_expression&&  e) noexcept;
  sc_statement&  assign(sc_variable&&  v) noexcept;

  void  clear() noexcept;

  bool  is_return()   const noexcept{return m_kind == kind::return_;}
  bool  is_break()    const noexcept{return m_kind == kind::break_;}
  bool  is_continue() const noexcept{return m_kind == kind::continue_;}
  bool  is_goto()     const noexcept{return m_kind == kind::goto_;}
  bool  is_label()    const noexcept{return m_kind == kind::label;}
  bool  is_if()       const noexcept{return m_kind == kind::if_;}
  bool  is_var()      const noexcept{return m_kind == kind::var;}
  bool  is_case()     const noexcept{return m_kind == kind::case_;}
  bool  is_default()  const noexcept{return m_kind == kind::default_;}
  bool  is_block()      const noexcept{return m_kind == kind::block;}
  bool  is_expression() const noexcept{return m_kind == kind::expression;}

  const std::u16string&  string() const noexcept{return m_data.s;}

        sc_block&  block()       noexcept{return *m_data.blk;}
  const sc_block&  block() const noexcept{return *m_data.blk;}

        sc_block_list&  block_list()       noexcept{return m_data.blkls;}
  const sc_block_list&  block_list() const noexcept{return m_data.blkls;}

        sc_variable&  variable()       noexcept{return m_data.var;}
  const sc_variable&  variable() const noexcept{return m_data.var;}

  const sc_expression&  expression() const noexcept{return m_data.expr;}

  void  fix(const sc_function&  fn) noexcept;

  void  print(int  depth=0) const noexcept;

};


using sc_statement_list = std::vector<sc_statement>;




class
sc_function
{
  sc_package*  m_package=nullptr;

  std::u16string  m_name;

  sc_signature  m_signature;

  std::unique_ptr<sc_block>  m_block;

  sc_symbol_list  m_symbol_list;

  int  m_entry_number=-1;

  void  scan(sc_block&  blk) noexcept;

public:
  sc_function() noexcept{}

  sc_function&  assign(sc_package&  pk, std::u16string_view  name, sc_type_info&&  ti, sc_parameter_list&&  parals, std::unique_ptr<sc_block>&&  blk) noexcept;

  const sc_package&  package() const noexcept{return *m_package;}

  const std::u16string&  name() const noexcept{return m_name;}

  const sc_signature&  signature() const noexcept{return m_signature;}

  const sc_block&  block() const noexcept{return *m_block;}

  const sc_symbol_list&  symbol_list() const noexcept{return m_symbol_list;}

  const sc_symbol*  find_symbol(std::u16string_view  name) const noexcept;

  int  entry_number() const noexcept{return m_entry_number;}

  void  fix() noexcept;

  int  stack_size() const noexcept;

  void  print() const;

};


using sc_function_list = std::vector<std::unique_ptr<sc_function>>;




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
sc_package
{
  sc_function_list  m_function_list;
  sc_variable_list  m_variable_list;

  static sc_return                  construct_return(const syntax_branch&  br) noexcept;
  static std::unique_ptr<sc_block>   construct_while(const syntax_branch&  br, sc_function&  fn) noexcept;
  static std::unique_ptr<sc_block>  construct_switch(const syntax_branch&  br, sc_function&  fn) noexcept;
  static sc_case                      construct_case(const syntax_branch&  br) noexcept;
  static sc_block_list                  construct_if(const syntax_branch&  br, sc_function&  fn) noexcept;
  static sc_label                    construct_label(const syntax_branch&  br) noexcept;
  static sc_goto                      construct_goto(const syntax_branch&  br) noexcept;
  static sc_variable              construct_variable(const syntax_branch&  br) noexcept;
  static std::unique_ptr<sc_block>   construct_block(const syntax_branch&  br, sc_function&  fn, std::string_view  k) noexcept;
  static sc_parameter            construct_parameter(const syntax_branch&  br, int64_t  off) noexcept;
  static sc_parameter_list  construct_parameter_list(const syntax_branch&  br) noexcept;
  static sc_type_info            construct_type_info(const syntax_branch&  br) noexcept;

  static std::unique_ptr<sc_function>  construct_function(const syntax_branch&  br, sc_package&  pk) noexcept;

public:
  sc_package() noexcept{}
  sc_package(const syntax_branch&  br) noexcept{assign(br);}

  sc_package&  operator=(const syntax_branch&  br) noexcept{return assign(br);}

  sc_package&  assign(const syntax_branch&  br) noexcept;

  void  clear() noexcept;

  int  stack_size() const noexcept;

  const sc_function*  find_function(std::u16string_view  name) const noexcept;
  const sc_symbol*      find_symbol(std::u16string_view  name) const noexcept;

  const sc_function_list&  function_list() const noexcept{return m_function_list;}
  const sc_variable_list&  variable_list() const noexcept{return m_variable_list;}

  void  print() const noexcept;

};




class
sc_status
{
  uint64_t  m_data;

public:
  sc_status(uint64_t  d=0) noexcept: m_data(d){}

  uint64_t  test(uint64_t  flags) const noexcept{return m_data&flags;}
  uint64_t  data(               ) const noexcept{return m_data;}

  sc_status&    set(uint64_t  flags) noexcept{  m_data |=  flags;  return *this;}
  sc_status&  unset(uint64_t  flags) noexcept{  m_data &= ~flags;  return *this;}

  void  reset(uint64_t  flags=0) noexcept{m_data = flags;}

};


class
sc_block_execution_frame
{
  const sc_statement*  m_begin;
  const sc_statement*  m_current;
  const sc_statement*  m_end;

  const sc_expression*  m_condition=nullptr;

public:
  sc_block_execution_frame(const sc_block&  blk) noexcept:
  m_begin(blk.begin()), m_current(blk.begin()), m_end(blk.end()){}

  operator bool() const noexcept{return m_current < m_end;}

  const sc_statement&  operator*() noexcept{return *m_current++;}

  bool  test_condition(sc_context&  ctx) const noexcept{return m_condition && m_condition->evaluate(ctx).integer();}
  bool   has_condition(                ) const noexcept{return m_condition;}

  void  rewind() noexcept{m_current = m_begin;}

};


class
sc_function_execution_frame
{
  int64_t  m_memory_size;

  const sc_function*  m_function;

  std::vector<sc_block_execution_frame>  m_bef_stack;

  struct flag{
    static constexpr uint64_t  returned = 1;
  };

  sc_status  m_status;

  void  process(const sc_statement&  st, sc_context&  ctx, sc_value_with_type_info&  ret_v) noexcept;

  void  process_return(sc_value  v, const sc_type_info&  ti, sc_value_with_type_info&  ret_v, sc_context&  ctx) noexcept;
  void  process_while(const sc_block&  blk, sc_context&  ctx) noexcept;
//  void  process_for() noexcept;
  void  process_switch(const sc_block&  blk) noexcept;
  void  process_goto(std::u16string_view  dst) noexcept;
  void  process_if(const sc_block_list&  blkls, sc_context&  ctx) noexcept;
  void  process_break() noexcept;
  void  process_continue() noexcept;
  void  process_block(const sc_block&  blk) noexcept;
  void  process_variable(const sc_variable&  v, sc_context&  ctx) noexcept;

  void  exit_from_block() noexcept;

public:
  sc_function_execution_frame(const sc_function&  fn, int64_t  memsize) noexcept;

  void  operator()(sc_context&  ctx, sc_value_with_type_info&  ret_v) noexcept;

  int64_t  previous_memory_size() const noexcept{return m_memory_size;}

  bool  is_returned() const noexcept{return m_status.test(flag::returned);;}

  void  print() const noexcept;

};




class
sc_context
{
  const sc_package*  m_package=nullptr;

  std::vector<uint8_t>  m_memory;

  int64_t  m_memory_size=0;

  std::vector<sc_function_execution_frame>  m_fef_stack;

  sc_value_with_type_info  m_returned_value;

  bool  m_halt_flag;

public:
  sc_context(const sc_package&  pk) noexcept: m_package(&pk){}

  const sc_package&  package() const noexcept{return *m_package;}

  sc_reference  get_reference(const sc_symbol&  sym) const noexcept;

  sc_accessor  accessor(int64_t  address) noexcept{return sc_accessor(&m_memory[address]);}

  sc_value  load(int64_t  address, const sc_type_info&  ti) noexcept;
  sc_value  dereference(sc_value  v, const sc_type_info&  ti) noexcept;

  void  store(std::u16string_view  var_name, sc_value  v, const sc_type_info&  ti) noexcept;
  void  store(const sc_symbol&  sym, sc_value  v, const sc_type_info&  ti) noexcept;
  void  store(int64_t  address, const sc_type_info&  dst_ti, sc_value  v, const sc_type_info&  ti) noexcept;

  int  call(std::u16string_view  fn_name, const sc_expression_list&  args) noexcept;
  int  call(const sc_function&  fn, const sc_expression_list&  args) noexcept;
  int  call(int  entry_number, const sc_expression_list&  args) noexcept;

  void  reset() noexcept;
  void  step() noexcept;

  const sc_value_with_type_info&  run(int  depth) noexcept;

  void  halt() noexcept{m_halt_flag = true;}
  bool  is_halted() const noexcept{return m_halt_flag;}


};




}




#endif




