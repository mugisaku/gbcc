#ifndef gbstd_vm_expression_HPP
#define gbstd_vm_expression_HPP


#include"libgbcc/vms/typesystem.hpp"
#include"libgbcc/syntax.hpp"




namespace gbcc{




class sc_unary_operation;
class sc_binary_operation;
class sc_expression;
class sc_expression_element;
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
sc_operand
{
  enum class kind{
    null, identifier, expression, value

  } m_kind=kind::null;

  union data{
    std::u16string  s;

    sc_expression*  e;

    sc_value_with_type_info  v;

    data() noexcept{}
   ~data(){}
  } m_data;

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
  bool  is_value()      const noexcept{return m_kind == kind::value;}

  const std::u16string&  string() const noexcept{return m_data.s;}
  const sc_expression&  expression() const noexcept{return *m_data.e;}

  sc_value  value() const noexcept{return m_data.v;}

  sc_type_info  type_info(const sc_context&  ctx) const noexcept;

  sc_value  evaluate(sc_context&  ctx) const noexcept;

  void  print() const noexcept;

};


class
sc_unary_operation
{
public:
  class  prefix_element;
  class postfix_element;

  using  prefix_element_list = std::vector<prefix_element>;
  using postfix_element_list = std::vector<postfix_element>;

private:
  sc_operand  m_operand;

  postfix_element_list  m_postfix_elements;
  prefix_element_list    m_prefix_elements;

  static sc_type_info  operate_type_info(const sc_type_info&  ti, const postfix_element&  e) noexcept;
  static sc_type_info  operate_type_info(const sc_type_info&  ti, const prefix_element&   e) noexcept;

  static sc_value  operate_value(sc_value  v, const sc_type_info&  ti, const postfix_element&  e, sc_context&  ctx) noexcept;
  static sc_value  operate_value(sc_value  v, const sc_type_info&  ti, const prefix_element&   e, sc_context&  ctx) noexcept;

public:
  sc_unary_operation() noexcept{}
  sc_unary_operation(sc_value  v, const sc_type_info&  ti) noexcept: m_operand(v,ti){}
  sc_unary_operation(const sc_operand&   o) noexcept: m_operand(o){}
  sc_unary_operation(      sc_operand&&  o) noexcept: m_operand(std::move(o)){}
  sc_unary_operation(prefix_element_list&&  prels, sc_operand&&  o, postfix_element_list&&  postls) noexcept{assign(std::move(prels),std::move(o),std::move(postls));}

  sc_unary_operation&  assign(prefix_element_list&&  prels, sc_operand&&  o, postfix_element_list&&  postls) noexcept;

  const sc_operand&  operand() const noexcept{return m_operand;}

  const postfix_element_list&  postfix_elements() const noexcept{return m_postfix_elements;}
  const prefix_element_list&    prefix_elements() const noexcept{return  m_prefix_elements;}

  sc_type_info  type_info(const sc_context&  ctx) const noexcept;

  sc_value  evaluate(sc_context&  ctx) const noexcept;

  void  print() const noexcept;

};


class
sc_unary_operation::
postfix_element
{
  enum class kind{
    null, call, index, access, increment, decrement,
  } m_kind=kind::null;

  union data{
    sc_expression_list  exprls;
    sc_expression*      expr;
    std::u16string      s;

    data() noexcept{}
   ~data(){}

  } m_data;

public:
  postfix_element() noexcept{}
 ~postfix_element(){clear();}

  template<class...  Args>
  postfix_element(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  postfix_element(const postfix_element&   rhs) noexcept{assign(rhs);}
  postfix_element(      postfix_element&&  rhs) noexcept{assign(std::move(rhs));}

  template<class...  Args>
  postfix_element&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  postfix_element&  operator=(const postfix_element&   rhs) noexcept{return assign(rhs);}
  postfix_element&  operator=(      postfix_element&&  rhs) noexcept{return assign(std::move(rhs));}

  postfix_element&  assign(const postfix_element&   rhs) noexcept;
  postfix_element&  assign(      postfix_element&&  rhs) noexcept;
  postfix_element&  assign(sc_expression_list&&  exprls) noexcept;
  postfix_element&  assign(sc_expression&&  expr) noexcept;
  postfix_element&  assign(std::u16string_view  sv) noexcept;
  postfix_element&  assign(std::string_view  sv) noexcept;

  void  clear() noexcept;

  bool  is_call()      const noexcept{return m_kind == kind::call;}
  bool  is_index()     const noexcept{return m_kind == kind::index;}
  bool  is_access()    const noexcept{return m_kind == kind::access;}
  bool  is_increment() const noexcept{return m_kind == kind::increment;}
  bool  is_decrement() const noexcept{return m_kind == kind::decrement;}

  const sc_expression_list&  expression_list() const noexcept{return  m_data.exprls;}
  const sc_expression&            expression() const noexcept{return *m_data.expr;}
  const std::u16string&               string() const noexcept{return  m_data.s;}

  void  print() const noexcept;

};


class
sc_unary_operation::
prefix_element
{
  enum class kind{
    null,
    address,
    dereference,
    increment,
    decrement,
    logical_not,
    bitwise_not,
    neg,
    
  } m_kind=kind::null;

public:
  prefix_element() noexcept{}
 ~prefix_element(){clear();}

  void  clear() noexcept;

  template<class...  Args>
  prefix_element(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  prefix_element(const prefix_element&   rhs) noexcept{assign(rhs);}
  prefix_element(      prefix_element&&  rhs) noexcept{assign(std::move(rhs));}

  template<class...  Args>
  prefix_element&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  prefix_element&  operator=(const prefix_element&   rhs) noexcept{return assign(rhs);}
  prefix_element&  operator=(      prefix_element&&  rhs) noexcept{return assign(std::move(rhs));}

  prefix_element&  assign(const prefix_element&   rhs) noexcept;
  prefix_element&  assign(      prefix_element&&  rhs) noexcept;
  prefix_element&  assign(std::string_view  sv) noexcept;

  bool  is_logical_not() const noexcept{return m_kind == kind::logical_not;}
  bool  is_bitwise_not() const noexcept{return m_kind == kind::bitwise_not;}
  bool  is_neg()         const noexcept{return m_kind == kind::neg;}
  bool  is_address()     const noexcept{return m_kind == kind::address;}
  bool  is_dereference() const noexcept{return m_kind == kind::dereference;}
  bool  is_increment()   const noexcept{return m_kind == kind::increment;}
  bool  is_decrement()   const noexcept{return m_kind == kind::decrement;}

  void  print() const noexcept;

};




class
sc_expression
{
  enum class kind{
    null, unary, binary,
  } m_kind=kind::null;

  union data{
    sc_unary_operation*    unop;
    sc_binary_operation*  binop;

   data() noexcept{}
  ~data(){}

  } m_data;


  static std::vector<sc_expression_element>  make_stack(const syntax_branch_element*  it, const syntax_branch_element*  end_it) noexcept;

  static sc_unary_operation::prefix_element   construct_prefix_element(const syntax_branch&  br) noexcept;
  static sc_operand                           construct_operand(const syntax_branch&  br) noexcept;
  static sc_expression_list                   construct_argument_list(const syntax_branch&  br) noexcept;
  static sc_unary_operation::postfix_element  construct_postfix_element(const syntax_branch&  br) noexcept;
  static sc_unary_operation                   construct_unary_operation(const syntax_branch&  br) noexcept;

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

  bool  is_unary()  const noexcept{return m_kind == kind::unary;}
  bool  is_binary() const noexcept{return m_kind == kind::binary;}

  sc_type_info  type_info(const sc_context&  ctx) const noexcept;

  sc_value  evaluate(sc_context&  ctx) const noexcept;

  void  print() const noexcept;

};


class
sc_expression_element
{
  std::u16string  m_operator;

  sc_expression  m_expression;

public:
  sc_expression_element() noexcept{}

  template<class...  Args>
  sc_expression_element(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  sc_expression_element&  assign(sc_expression&&  expr) noexcept;
  sc_expression_element&  assign(std::u16string_view  oprt) noexcept;
  sc_expression_element&  assign(const std::u16string&  oprt) noexcept{return assign(std::u16string_view(oprt));}

  bool  is_operator()   const noexcept{return m_operator.size();}
  bool  is_expression() const noexcept{return !is_operator();}

  const std::u16string&  operator_() const noexcept{return m_operator;}

  sc_expression&  expression() noexcept{return m_expression;}

};




class
sc_binary_operation
{
  sc_expression  m_left;
  sc_expression  m_right;

  std::u16string  m_operator;

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

  sc_type_info  type_info(const sc_context&  ctx) const noexcept;

  sc_value  evaluate(sc_context&  ctx) const noexcept;

  void  print() const noexcept;

};




}




#endif




