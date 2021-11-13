#include"libgbcc/vms/expression.hpp"
#include"libgbcc/vms/context.hpp"




namespace gbcc{




sc_unary_operation
sc_expression::
make_prefix_unary_operation(const syntax_branch&  br, sc_unary_operation&&  src) noexcept
{
  auto&  s = br[0].string();

  std::string_view  sv( (s == u"*")? "*"
                       :(s == u"&")? "&"
                       :(s == u"-")? "-"
                       :(s == u"~")? "~"
                       :(s == u"!")? "!"
                       :(s == u"++")? "++"
                       :(s == u"--")? "--"
                       :"");

  return sc_unary_operation(sc_expression(std::move(src)),sv);
}


sc_operand
sc_expression::
construct_operand(const syntax_branch&  br) noexcept
{
  auto&  e = br[0];

    if(e.is_token())
    {
      auto&  tok = e.token();

        if(tok.is_integer())
        {
          auto  i = tok.integer();

            if(i <= INT64_MAX){return sc_operand(static_cast<int64_t>(i),sc_int64_ti);}
          else                {return sc_operand(                    (i),sc_uint64_ti);}
        }
      else if(tok.is_floating()  ){return sc_operand(tok.floating(),sc_float64_ti);}
      else if(tok.is_identifier()){return sc_operand(std::u16string_view(tok.string()));}
    }

  else
    if(e.is_string())
    {
      //m_print_u16s(e.string());
    }

  else
    if(e.is_branch(u"expression_in_parentheses"))
    {
      return sc_operand(sc_expression(e.branch()[1].branch()));
    }


  return sc_operand();
}


sc_expression_list
sc_expression::
construct_argument_list(const syntax_branch&  br) noexcept
{
  sc_expression_list  ls;

    for(auto&  e: br)
    {
        if(e.is_branch(u"expression"))
        {
          ls.emplace_back(sc_expression(e.branch()));
        }
    }


  return std::move(ls);
}


sc_unary_operation
sc_expression::
make_postfix_unary_operation(const syntax_branch&  br, sc_unary_operation&&  src) noexcept
{
  sc_unary_operation  o;

  auto&  e = br[0];

    if(e.is_branch(u"access"))
    {
      o.assign(sc_expression(std::move(src)),std::u16string_view(e.branch()[1].string()));
    }

  else
    if(e.is_branch(u"index"))
    {
      o.assign(sc_expression(std::move(src)),sc_expression(e.branch()[1].branch()));
    }

  else
    if(e.is_branch(u"argument_list"))
    {
      o.assign(sc_expression(std::move(src)),construct_argument_list(e.branch()));
    }

  else
    if(e.is_string())
    {
      auto&  s = e.string();

      std::string_view  sv( (s == u"++")? "_++"
                           :(s == u"--")? "_--"
                           :"");

      o.assign(sc_expression(std::move(src)),sv);
    }


  return std::move(o);
}


sc_unary_operation
sc_expression::
construct_unary_operation(const syntax_branch&  br) noexcept
{
  sc_unary_operation  o;

  std::vector<const syntax_branch*>  prefixes;

    for(auto&  e: br)
    {
        if(e.is_branch(u"prefix_unary_operator"))
        {
          prefixes.emplace_back(&e.branch());
        }

      else
        if(e.is_branch(u"operand"))
        {
          o.assign(sc_expression(construct_operand(e.branch())));
        }

      else
        if(e.is_branch(u"postfix_unary_operator"))
        {
          o.assign(make_postfix_unary_operation(e.branch(),std::move(o)));
        }
    }


    while(prefixes.size())
    {
      o.assign(make_prefix_unary_operation(*prefixes.back(),std::move(o)));

      prefixes.pop_back();
    }


  return std::move(o);
}




namespace{
bool
is_assignment(std::u16string_view  sv) noexcept
{
  return (sv == u"="  ) ||
         (sv == u"+=" ) ||
         (sv == u"-=" ) ||
         (sv == u"*=" ) ||
         (sv == u"/=" ) ||
         (sv == u"%=" ) ||
         (sv == u"<<=") ||
         (sv == u">>=") ||
         (sv == u"|=" ) ||
         (sv == u"&=" ) ||
         (sv == u"^=" );
}
}




class
sc_expression::
element
{
  std::u16string  m_operator;

  sc_expression  m_expression;

public:
  element() noexcept{}

  template<class...  Args>
  element(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  element&  assign(sc_expression&&  expr) noexcept;
  element&  assign(std::u16string_view  oprt) noexcept;
  element&  assign(const std::u16string&  oprt) noexcept{return assign(std::u16string_view(oprt));}

  bool  is_operator()   const noexcept{return m_operator.size();}
  bool  is_expression() const noexcept{return !is_operator();}

  const std::u16string&  operator_() const noexcept{return m_operator;}

  sc_expression&  expression() noexcept{return m_expression;}

};


sc_expression::element&
sc_expression::element::
assign(sc_expression&&  expr) noexcept
{
  m_operator.clear();

  m_expression = std::move(expr);

  return *this;
}


sc_expression::element&
sc_expression::element::
assign(std::u16string_view  oprt) noexcept
{
  m_operator = oprt;

  m_expression.clear();

  return *this;
}


void
sc_expression::
process(const syntax_branch_element*&  it, std::vector<element>&  stk, std::u16string_view&  assop, std::u16string_view&  opbuf) noexcept
{
  std::u16string_view  oprt = it++->branch()[0].string();

    if(is_assignment(oprt))
    {
        if(assop.size())
        {
          printf("multi assign operator is found\n");

          return;
        }


      assop = oprt;
    }

  else
    {
        if(opbuf.size())
        {
          stk.emplace_back(opbuf);
        }


      opbuf = oprt;
    }


    if(it->is_branch(u"unary_operation"))
    {
      stk.emplace_back(sc_expression(construct_unary_operation(it++->branch())));
    }
}


std::vector<sc_expression::element>
sc_expression::
make_stack(const syntax_branch_element*  it, const syntax_branch_element*  end_it) noexcept
{
  std::vector<element>  stack;

  std::u16string_view  assign_operator;
  std::u16string_view  operator_buffer;

    if(it != end_it)
    {
        if(it->is_branch(u"unary_operation"))
        {
          stack.emplace_back(sc_expression(construct_unary_operation(it++->branch())));

            while(it != end_it)
            {
                if(it->is_branch(u"binary_operator"))
                {
                  process(it,stack,assign_operator,operator_buffer);
                }
            }
        }
    }


    if(operator_buffer.size())
    {
      stack.emplace_back(operator_buffer);
    }


    if(assign_operator.size())
    {
      stack.emplace_back(assign_operator);
    }


  return std::move(stack);
}




sc_expression&
sc_expression::
assign(const syntax_branch&  br) noexcept
{
  clear();

  auto  stack = make_stack(br.begin(),br.end());

  std::vector<sc_expression>  buffer;

    for(auto&  e: stack)
    {
        if(e.is_operator())
        {
            if(buffer.size() < 2)
            {
              printf("expression construction error: number of operands is not enough");

              return *this;
            }


          auto  r = std::move(buffer.back());

          buffer.pop_back();

          auto  l = std::move(buffer.back());

          buffer.back() = sc_binary_operation(std::move(l),std::move(r),e.operator_());
        }

      else
        {
          buffer.emplace_back(std::move(e.expression()));
        }
    }


    if(buffer.empty())
    {
    }

  else
    if(buffer.size() > 1)
    {
      printf("expression construction error: invalid result");
    }

  else
    {
      *this = std::move(buffer.front());
    }


  return *this;
}




}




