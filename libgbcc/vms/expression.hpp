#ifndef gbstd_vm_expression_HPP
#define gbstd_vm_expression_HPP


#include"libgbcc/vms/typesystem.hpp"
#include"libgbcc/syntax.hpp"




namespace gbcc{




class sc_unary_operation;
class sc_binary_operation;
class sc_expression;
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
  sc_type_info  m_type_info;

  union data{
    int64_t  i;
   uint64_t  u;
   double    f;

    data() noexcept{}
   ~data(){}
  } m_data;

public:
  sc_value() noexcept{}
  sc_value(const sc_value&   rhs) noexcept{assign(rhs);}
  sc_value(      sc_value&&  rhs) noexcept{assign(std::move(rhs));}

  template<class...  Args>
  sc_value(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  sc_value&  operator=(const sc_value&   rhs) noexcept{return assign(rhs);}
  sc_value&  operator=(      sc_value&&  rhs) noexcept{return assign(std::move(rhs));}

  template<class...  Args>
  sc_value&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  sc_value  operator()(const sc_type_info&  ti) const noexcept;

  void  clear() noexcept;

  sc_value&  assign(const sc_value&   rhs) noexcept;
  sc_value&  assign(      sc_value&&  rhs) noexcept;
  sc_value&  assign(bool  b) noexcept;
  sc_value&  assign( int64_t  i) noexcept;
  sc_value&  assign(uint64_t  u) noexcept;
  sc_value&  assign(double  f) noexcept;
  sc_value&  assign(const sc_type_info&  ti, int64_t  i) noexcept;
  sc_value&  assign(const sc_type_info&  ti, uint64_t  u) noexcept;
  sc_value&  assign(const sc_type_info&  ti, double  f) noexcept;
  sc_value&  assign(const sc_pointer&  ptr) noexcept;
  sc_value&  assign(const sc_reference&  ref) noexcept;

  const sc_type_info&  type_info() const noexcept{return m_type_info;}

    int64_t           integer() const noexcept{return m_data.i;}
   uint64_t  unsigned_integer() const noexcept{return m_data.u;}
   double            floating() const noexcept{return m_data.f;}
   bool               boolean() const noexcept{return m_data.i;}

    int64_t           integer(const sc_context&  ctx) const noexcept;
   uint64_t  unsigned_integer(const sc_context&  ctx) const noexcept;
   double            floating(const sc_context&  ctx) const noexcept;
   bool               boolean(const sc_context&  ctx) const noexcept;

  void  print() const noexcept;

};




class
sc_operand
{
  enum class kind{
    null, identifier, integer, unsigned_integer, floating, expression,

  } m_kind=kind::null;

  union data{
    std::u16string  s;

     int64_t  i;
    uint64_t  u;
    double    f;

    sc_expression*  e;

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
  sc_operand&  assign( int64_t  i) noexcept;
  sc_operand&  assign(uint64_t  u) noexcept;
  sc_operand&  assign(double  f) noexcept;
  sc_operand&  assign(std::u16string_view  sv) noexcept;
  sc_operand&  assign(sc_expression&&  e) noexcept;

  void  clear() noexcept;

  bool  is_identifier()       const noexcept{return m_kind == kind::identifier;}
  bool  is_integer()          const noexcept{return m_kind == kind::integer;}
  bool  is_unsigned_integer() const noexcept{return m_kind == kind::unsigned_integer;}
  bool  is_floating()         const noexcept{return m_kind == kind::floating;}
  bool  is_expression()       const noexcept{return m_kind == kind::expression;}

  const std::u16string&  string() const noexcept{return m_data.s;}

   int64_t           integer() const noexcept{return m_data.i;}
  uint64_t  unsigned_integer() const noexcept{return m_data.u;}
  double   floating() const noexcept{return m_data.f;}

  const sc_expression&  expression() const noexcept{return *m_data.e;}

  sc_type_info  type_info(sc_context&  ctx) const noexcept;

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

  static sc_value  operate(const sc_value&  v, const postfix_element&  e, sc_context&  ctx) noexcept;
  static sc_value  operate(const sc_value&  v, const prefix_element&   e, sc_context&  ctx) noexcept;

public:
  sc_unary_operation() noexcept{}
  sc_unary_operation(prefix_element_list&&  prels, sc_operand&&  o, postfix_element_list&&  postls) noexcept{assign(std::move(prels),std::move(o),std::move(postls));}

  sc_unary_operation&  assign(prefix_element_list&&  prels, sc_operand&&  o, postfix_element_list&&  postls) noexcept;

  const sc_operand&  operand() const noexcept{return m_operand;}

  const postfix_element_list&  postfix_elements() const noexcept{return m_postfix_elements;}
  const prefix_element_list&    prefix_elements() const noexcept{return  m_prefix_elements;}

  sc_type_info  type_info(sc_context&  ctx) const noexcept;

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
sc_expression_element
{
  std::u16string  m_operator;

  sc_unary_operation  m_operation;

public:
  sc_expression_element() noexcept{}

  template<class...  Args>
  sc_expression_element(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  sc_expression_element(const sc_expression_element&   rhs) noexcept{assign(rhs);}
  sc_expression_element(      sc_expression_element&&  rhs) noexcept{assign(std::move(rhs));}

  sc_expression_element&  operator=(const sc_expression_element&   rhs) noexcept{return assign(rhs);}
  sc_expression_element&  operator=(      sc_expression_element&&  rhs) noexcept{return assign(std::move(rhs));}

  sc_expression_element&  assign(const sc_expression_element&   rhs) noexcept;
  sc_expression_element&  assign(      sc_expression_element&&  rhs) noexcept;
  sc_expression_element&  assign(                          sc_unary_operation&&  opti) noexcept;
  sc_expression_element&  assign(std::u16string_view  opto, sc_unary_operation&&  opti) noexcept;

  const std::u16string&  operator_() const noexcept{return m_operator;}

  const sc_unary_operation&  operation() const noexcept{return m_operation;}

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


  sc_type_info  m_type_info;

  sc_value*  m_constant_value=nullptr;

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
  sc_expression&  assign(const syntax_branch&  br) noexcept;

  void  clear() noexcept;

  const sc_type_info&  type_info() const noexcept{return m_type_info;}

  const sc_value*  constant_value() const noexcept{return m_constant_value;}

  bool  is_unary()  const noexcept{return m_kind == kind::unary;}
  bool  is_binary() const noexcept{return m_kind == kind::binary;}

  sc_value  evaluate(sc_context&  ctx) const noexcept;

  void  print() const noexcept;

};




class
sc_binary_operation
{
  std::u16string  m_operator;

  sc_expression  m_left;
  sc_expression  m_right;

  class functor{
    sc_value   m_left;
    sc_value  m_right;

    int  convert_values() noexcept;

    bool  is_floating(        ) const noexcept;
    bool  is_unsigned_integer() const noexcept;
    bool  is_integer(         ) const noexcept;
    bool  is_pointer(         ) const noexcept;

  public:
    functor(sc_value&&  l, sc_value&&  r) noexcept:
    m_left(std::move(l)), m_right(std::move(r)){}

    sc_value  operate_add(        ) noexcept;
    sc_value  operate_sub(        ) noexcept;
    sc_value  operate_mul(        ) noexcept;
    sc_value  operate_div(        ) noexcept;
    sc_value  operate_rem(        ) noexcept;
    sc_value  operate_shl(        ) noexcept;
    sc_value  operate_shr(        ) noexcept;
    sc_value  operate_and(        ) noexcept;
    sc_value  operate_or(         ) noexcept;
    sc_value  operate_xor(        ) noexcept;
    sc_value  operate_eq(         ) noexcept;
    sc_value  operate_neq(        ) noexcept;
    sc_value  operate_lt(         ) noexcept;
    sc_value  operate_lteq(       ) noexcept;
    sc_value  operate_gt(         ) noexcept;
    sc_value  operate_gteq(       ) noexcept;
    sc_value  operate_logical_and() noexcept;
    sc_value  operate_logical_or( ) noexcept;
    sc_value  operate_assign(     ) noexcept;

    sc_value  right() noexcept{return std::move(m_right);}

  };

  static sc_value  operate_assign(const sc_value&  l, sc_value&&  r, sc_context&  ctx) noexcept;

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
  sc_binary_operation&  assign(std::u16string_view  o, sc_expression&&  l, sc_expression&&  r) noexcept;

  void  clear() noexcept;

  const std::u16string&  operator_() const noexcept{return m_operator;}

  const sc_expression&   left() const noexcept{return m_left;}
  const sc_expression&  right() const noexcept{return m_right;}

  sc_type_info  type_info(sc_context&  ctx) const noexcept;

  sc_value  evaluate(sc_context&  ctx) const noexcept;

  void  print() const noexcept;

};




}




#endif




