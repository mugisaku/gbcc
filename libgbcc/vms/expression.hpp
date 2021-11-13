#ifndef gbstd_vm_expression_HPP
#define gbstd_vm_expression_HPP


#include"libgbcc/vms/typesystem.hpp"
#include"libgbcc/syntax.hpp"




namespace gbcc{




class sc_unary_operation;
class sc_binary_operation;
class sc_expression;
class sc_symbol;
class sc_function;
class sc_context;

using sc_expression_list = std::vector<sc_expression>;




class
sc_pointer
{
  sc_type_info  m_type_info;

  int64_t  m_data=0;

public:
  sc_pointer() noexcept{}
  sc_pointer(const sc_type_info&  ti, int64_t  dat=0) noexcept: m_type_info(ti), m_data(dat){}

  const sc_type_info&  type_info() const noexcept{return m_type_info;}

  int64_t  data() const noexcept{return m_data;}

};


class
sc_reference
{
  sc_type_info  m_type_info;

  int64_t  m_data=0;

public:
  sc_reference() noexcept{}
  sc_reference(const sc_type_info&  ti, int64_t  dat=0) noexcept: m_type_info(ti), m_data(dat){}

  const sc_type_info&  type_info() const noexcept{return m_type_info;}

  int64_t  data() const noexcept{return m_data;}

};


class
sc_value
{
  union{
    void*     p;
    int64_t   i;
    uint64_t  u;
    double    f;
  };

public:
  constexpr sc_value() noexcept: i(0){}
  constexpr sc_value(bool  b) noexcept: i(b? 1:0){}
  constexpr sc_value(int8_t    i_) noexcept: i(i_){}
  constexpr sc_value(int16_t   i_) noexcept: i(i_){}
  constexpr sc_value(int32_t   i_) noexcept: i(i_){}
  constexpr sc_value(int64_t   i_) noexcept: i(i_){}
  constexpr sc_value(uint8_t   u_) noexcept: u(u_){}
  constexpr sc_value(uint16_t  u_) noexcept: u(u_){}
  constexpr sc_value(uint32_t  u_) noexcept: u(u_){}
  constexpr sc_value(uint64_t  u_) noexcept: u(u_){}
  constexpr sc_value(float     f_) noexcept: f(f_){}
  constexpr sc_value(double    f_) noexcept: f(f_){}
  constexpr sc_value(void*     p_) noexcept: p(p_){}

  constexpr operator bool() const noexcept{return i;}

  constexpr int64_t            integer() const noexcept{return i;}
  constexpr uint64_t  unsigned_integer() const noexcept{return u;}
  constexpr double            floating() const noexcept{return f;}
  constexpr void*              pointer() const noexcept{return p;}

  sc_value  convert(const sc_type_info&  source, const sc_type_info&  target, sc_context&  ctx, bool  forced=false) const noexcept;

  void  print(const sc_type_info&  ti) const noexcept;

};


class
sc_value_with_type_info: public sc_value
{
  sc_type_info  m_type_info;

public:
  sc_value_with_type_info() noexcept: m_type_info(sc_undef_ti){}
  sc_value_with_type_info(sc_value  v, const sc_type_info&  ti) noexcept: sc_value(v), m_type_info(ti){}

  const sc_type_info&  type_info() const noexcept{return m_type_info;}

  void  print() const noexcept{sc_value::print(m_type_info);}

};




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
  sc_symbol(std::u16string_view  name) noexcept: m_name(name){}
  sc_symbol(std::u16string_view  name, sc_type_info&&  ti) noexcept:
  m_name(name), m_type_info(std::move(ti)){}

  sc_symbol(std::u16string_view  name, sc_type_info&&  ti, sc_symbol_attribute  attr, int64_t  off) noexcept:
  m_name(name), m_type_info(std::move(ti)), m_attribute(attr), m_offset(off){}

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

    printf(":{offset %6" PRIi64 "} ",m_offset);
  }

};


using sc_symbol_list = std::vector<sc_symbol>;




class
sc_operand
{
  enum class kind{
    null, identifier, expression, literal, symbol

  } m_kind=kind::null;

  union data{
    sc_expression*  e;

    sc_symbol  sym;

    data() noexcept{}
   ~data(){}
  } m_data;

  sc_type_info  m_type_info;
  
  std::unique_ptr<sc_value>  m_constant_value;

  void  fix_for_symbol(const sc_symbol&  sym) noexcept;
  void  fix_for_function(const sc_function&  fn) noexcept;
  void  fix_for_identifier(const sc_function&  fn) noexcept;

public:
  sc_operand() noexcept{}
 ~sc_operand(){clear();}

  sc_operand(const sc_operand&   rhs) noexcept{assign(rhs);}
  sc_operand(      sc_operand&&  rhs) noexcept{assign(std::move(rhs));}

  template<class...  Args>
  sc_operand(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  sc_operand&  operator=(const sc_operand&   rhs) noexcept{return assign(rhs);}
  sc_operand&  operator=(      sc_operand&&  rhs) noexcept{return assign(std::move(rhs));}

  template<class...  Args>
  sc_operand&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  operator bool() const noexcept{return m_kind != kind::null;}

  sc_operand&  assign(const sc_operand&   rhs) noexcept;
  sc_operand&  assign(      sc_operand&&  rhs) noexcept;
  sc_operand&  assign(std::u16string_view  sv) noexcept;
  sc_operand&  assign(sc_expression&&  e) noexcept;
  sc_operand&  assign(sc_value  v, const sc_type_info&  ti) noexcept;

  void  clear() noexcept;

  bool  is_identifier() const noexcept{return m_kind == kind::identifier;}
  bool  is_expression() const noexcept{return m_kind == kind::expression;}
  bool  is_symbol()     const noexcept{return m_kind == kind::symbol;}
  bool  is_literal()    const noexcept{return m_kind == kind::literal;}

  const sc_expression&  expression() const noexcept{return *m_data.e;}

  const sc_symbol&  symbol() const noexcept{return m_data.sym;}

  void  fix(const sc_function&  fn) noexcept;

  const sc_type_info&  type_info() const noexcept;

  const sc_value*  constant_value() const noexcept;

  sc_value  evaluate(sc_context&  ctx) const noexcept;

  void  print() const noexcept;

};




class
sc_expression
{
  enum class kind{
    null, operand, unary, binary,
  } m_kind=kind::null;

  union data{
    sc_operand  o;

    sc_unary_operation*    unop;
    sc_binary_operation*  binop;

   data() noexcept{}
  ~data(){}

  } m_data;


  static sc_operand          construct_operand(const syntax_branch&  br) noexcept;
  static sc_expression_list  construct_argument_list(const syntax_branch&  br) noexcept;
  static sc_unary_operation  construct_unary_operation(const syntax_branch&  br) noexcept;

  static sc_unary_operation  make_prefix_unary_operation(const syntax_branch&  br, sc_unary_operation&&  src) noexcept;
  static sc_unary_operation  make_postfix_unary_operation(const syntax_branch&  br, sc_unary_operation&&  src) noexcept;

  class element;

  static void  process(const syntax_branch_element*&  it, std::vector<element>&  stk, std::u16string_view&  assop, std::u16string_view&  opbuf) noexcept;

  static std::vector<element>  make_stack(const syntax_branch_element*  it, const syntax_branch_element*  end_it) noexcept;

public:
  sc_expression() noexcept{}
  sc_expression(const syntax_branch&  br) noexcept{assign(br);}
 ~sc_expression(){clear();}

  sc_expression(const sc_expression&   rhs) noexcept{assign(rhs);}
  sc_expression(      sc_expression&&  rhs) noexcept{assign(std::move(rhs));}

  template<class...  Args>
  sc_expression(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  sc_expression&  operator=(const sc_expression&   rhs) noexcept{return assign(rhs);}
  sc_expression&  operator=(      sc_expression&&  rhs) noexcept{return assign(std::move(rhs));}

  template<class...  Args>
  sc_expression&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  operator bool() const noexcept{return m_kind != kind::null;}

  sc_expression&  assign(const sc_expression&   rhs) noexcept;
  sc_expression&  assign(      sc_expression&&  rhs) noexcept;
  sc_expression&  assign(sc_unary_operation&&   unop) noexcept;
  sc_expression&  assign(sc_binary_operation&&  binop) noexcept;
  sc_expression&  assign(sc_value  v, const sc_type_info&  ti) noexcept;
  sc_expression&  assign(const sc_operand&   o) noexcept;
  sc_expression&  assign(      sc_operand&&  o) noexcept;
  sc_expression&  assign(const syntax_branch&  br) noexcept;

  void  clear() noexcept;

  bool  is_operand() const noexcept{return m_kind == kind::operand;}
  bool  is_unary()   const noexcept{return m_kind == kind::unary;}
  bool  is_binary()  const noexcept{return m_kind == kind::binary;}

  void  fix(const sc_function&  fn) noexcept;

  const sc_type_info&  type_info() const noexcept;

  const sc_value*  constant_value() const noexcept;

  sc_value  evaluate(sc_context&  ctx) const noexcept;

  void  print() const noexcept;

};




class
sc_unary_operation
{
  enum class kind{
    null,
    nop,
    call,
    index,
    access,
    postfix_increment,
    postfix_decrement,
    address,
    dereference,
    prefix_increment,
    prefix_decrement,
    logical_not,
    bitwise_not,
    neg,

  } m_kind=kind::null;

  union data{
    sc_expression_list  exprls;
    sc_expression*      expr;
    std::u16string      s;

    data() noexcept{}
   ~data(){}

  } m_data;


  sc_expression  m_expression;

  sc_type_info  m_type_info;

  std::unique_ptr<sc_value>  m_constant_value;

public:
  sc_unary_operation() noexcept{}
  sc_unary_operation(sc_value  v, const sc_type_info&  ti) noexcept: m_expression(sc_operand(v,ti)){}
  sc_unary_operation(const sc_unary_operation&   rhs) noexcept{assign(rhs);}
  sc_unary_operation(      sc_unary_operation&&  rhs) noexcept{assign(std::move(rhs));}
 ~sc_unary_operation(){clear();}

  template<class...  Args>
  sc_unary_operation(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  void  clear() noexcept;

  sc_unary_operation&  assign(const sc_unary_operation&   rhs) noexcept;
  sc_unary_operation&  assign(      sc_unary_operation&&  rhs) noexcept;
  sc_unary_operation&  assign(sc_expression&&  e) noexcept;
  sc_unary_operation&  assign(sc_expression&&  e, sc_expression_list&&  exprls) noexcept;
  sc_unary_operation&  assign(sc_expression&&  e, sc_expression&&  expr) noexcept;
  sc_unary_operation&  assign(sc_expression&&  e, std::u16string_view  sv) noexcept;
  sc_unary_operation&  assign(sc_expression&&  e, std::string_view  sv) noexcept;

  bool  is_nop()               const noexcept{return m_kind == kind::nop;}
  bool  is_call()              const noexcept{return m_kind == kind::call;}
  bool  is_index()             const noexcept{return m_kind == kind::index;}
  bool  is_access()            const noexcept{return m_kind == kind::access;}
  bool  is_postfix_increment() const noexcept{return m_kind == kind::postfix_increment;}
  bool  is_postfix_decrement() const noexcept{return m_kind == kind::postfix_decrement;}
  bool  is_logical_not()       const noexcept{return m_kind == kind::logical_not;}
  bool  is_bitwise_not()       const noexcept{return m_kind == kind::bitwise_not;}
  bool  is_neg()               const noexcept{return m_kind == kind::neg;}
  bool  is_address()           const noexcept{return m_kind == kind::address;}
  bool  is_dereference()       const noexcept{return m_kind == kind::dereference;}
  bool  is_prefix_increment()  const noexcept{return m_kind == kind::prefix_increment;}
  bool  is_prefix_decrement()  const noexcept{return m_kind == kind::prefix_decrement;}


  const sc_expression_list&  expression_list() const noexcept{return  m_data.exprls;}
  const sc_expression&  expression_for_index() const noexcept{return *m_data.expr;}
  const std::u16string&               string() const noexcept{return  m_data.s;}

  const sc_expression&  expression() const noexcept{return m_expression;}

  void  fix(const sc_function&  fn) noexcept;

  const sc_type_info&  type_info() const noexcept{return m_type_info;}

  const sc_value*  constant_value() const noexcept{return m_constant_value.get();}

  sc_value  evaluate(sc_context&  ctx) const noexcept;

  void  print() const noexcept;

};




class
sc_binary_operation
{
  sc_expression  m_left;
  sc_expression  m_right;

  std::u16string  m_operator;

  sc_type_info  m_type_info;

  std::unique_ptr<sc_value>  m_constant_value;

  class functor{
    sc_value  m_l_value;
    sc_value  m_r_value;

    sc_type_info  m_l_type_info;
    sc_type_info  m_r_type_info;

    sc_value          m_result_value;
    sc_type_info  m_result_type_info;

    void  convert_values() noexcept;

  public:
    functor(sc_value  l, const sc_type_info&  lti, sc_value  r, const sc_type_info&  rti, sc_context&  ctx) noexcept;

    functor&  operate_add(        ) noexcept;
    functor&  operate_sub(        ) noexcept;
    functor&  operate_mul(        ) noexcept;
    functor&  operate_div(        ) noexcept;
    functor&  operate_rem(        ) noexcept;
    functor&  operate_shl(        ) noexcept;
    functor&  operate_shr(        ) noexcept;
    functor&  operate_and(        ) noexcept;
    functor&  operate_or(         ) noexcept;
    functor&  operate_xor(        ) noexcept;
    functor&  operate_eq(         ) noexcept;
    functor&  operate_neq(        ) noexcept;
    functor&  operate_lt(         ) noexcept;
    functor&  operate_lteq(       ) noexcept;
    functor&  operate_gt(         ) noexcept;
    functor&  operate_gteq(       ) noexcept;
    functor&  operate_logical_and() noexcept;
    functor&  operate_logical_or( ) noexcept;
    functor&  operate_rv(         ) noexcept;

    const sc_type_info&  result_type_info() const noexcept{return m_result_type_info;}

    sc_value  result_value() const noexcept{return m_result_value;}

  };

  static sc_value  operate_assign(sc_value  l, const sc_type_info&  lti, functor&  f, sc_context&  ctx) noexcept;

public:
  sc_binary_operation() noexcept{}
 ~sc_binary_operation(){clear();}

  sc_binary_operation(const sc_binary_operation&   rhs) noexcept{assign(rhs);}
  sc_binary_operation(      sc_binary_operation&&  rhs) noexcept{assign(std::move(rhs));}

  template<class...  Args>
  sc_binary_operation(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  sc_binary_operation&  operator=(const sc_binary_operation&   rhs) noexcept{return assign(rhs);}
  sc_binary_operation&  operator=(      sc_binary_operation&&  rhs) noexcept{return assign(std::move(rhs));}

  sc_binary_operation&  assign(const sc_binary_operation&   rhs) noexcept;
  sc_binary_operation&  assign(      sc_binary_operation&&  rhs) noexcept;
  sc_binary_operation&  assign(sc_expression&&  l, sc_expression&&  r, std::u16string_view  o) noexcept;

  void  clear() noexcept;

  const sc_expression&   left() const noexcept{return m_left;}
  const sc_expression&  right() const noexcept{return m_right;}

  const std::u16string&  operator_() const noexcept{return m_operator;}

  void  fix(const sc_function&  fn) noexcept;

  const sc_type_info&  type_info() const noexcept{return m_type_info;}

  const sc_value*  constant_value() const noexcept{return m_constant_value.get();}

  sc_value  evaluate(sc_context&  ctx) const noexcept;

  void  print() const noexcept;

};



}




#endif




