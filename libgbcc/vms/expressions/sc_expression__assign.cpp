#include"libgbcc/vms/expression.hpp"




namespace gbcc{




sc_unary_operation::prefix_element
sc_expression::
construct_prefix_element(const syntax_branch&  br) noexcept
{
  using element = sc_unary_operation::prefix_element;

  auto&  s = br[0].string();

  std::string_view  sv( (s == u"*")? "*"
                       :(s == u"&")? "&"
                       :(s == u"-")? "-"
                       :(s == u"~")? "~"
                       :(s == u"!")? "!"
                       :(s == u"++")? "++"
                       :(s == u"--")? "--"
                       :"");

  return element(sv);
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


sc_unary_operation::postfix_element
sc_expression::
construct_postfix_element(const syntax_branch&  br) noexcept
{
  using element = sc_unary_operation::postfix_element;

  auto&  e = br[0];

    if(e.is_branch(u"access"))
    {
      return element(std::u16string_view(e.branch()[1].string()));
    }

  else
    if(e.is_branch(u"index"))
    {
      return element(sc_expression(e.branch()[1].branch()));
    }

  else
    if(e.is_branch(u"argument_list"))
    {
      return element(construct_argument_list(e.branch()));
    }

  else
    if(e.is_string())
    {
      auto&  s = e.string();

      std::string_view  sv( (s == u"++")? "++"
                           :(s == u"--")? "--"
                           :"");

      return element(sv);
    }


  return element();
}


sc_unary_operation
sc_expression::
construct_unary_operation(const syntax_branch&  br) noexcept
{
  sc_operand  o;
  sc_unary_operation::prefix_element_list    prels;
  sc_unary_operation::postfix_element_list  postls;

    for(auto&  e: br)
    {
        if(e.is_branch(u"prefix_unary_operator"))
        {
          prels.emplace_back(construct_prefix_element(e.branch()));
        }

      else
        if(e.is_branch(u"operand"))
        {
          o.assign(construct_operand(e.branch()));
        }

      else
        if(e.is_branch(u"postfix_unary_operator"))
        {
          postls.emplace_back(construct_postfix_element(e.branch()));
        }
    }


  return sc_unary_operation(std::move(prels),std::move(o),std::move(postls));
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


std::vector<sc_expression_element>
sc_expression::
make_stack(const syntax_branch_element*  it, const syntax_branch_element*  end_it) noexcept
{
  std::vector<sc_expression_element>  stack;

  std::u16string_view  assign_operator;
  std::u16string_view  operator_buffer;

    if(it != end_it)
    {
        if(it->is_branch(u"unary_operation"))
        {
          stack.emplace_back(sc_expression(construct_unary_operation(it++->branch())));

            while(it != end_it)
            {
              auto&  e = *it++;

                if(e.is_branch(u"binary_operator"))
                {
                  std::u16string_view  oprt = e.branch()[0].string();

                    if(is_assignment(oprt))
                    {
                        if(assign_operator.size())
                        {
                          printf("multi assign operator is found\n");

                          return {};
                        }


                      assign_operator = oprt;
                    }

                  else
                    {
                        if(operator_buffer.size())
                        {
                          stack.emplace_back(operator_buffer);
                        }


                      operator_buffer = oprt;
                    }


                    if(it->is_branch(u"unary_operation"))
                    {
                      stack.emplace_back(sc_expression(construct_unary_operation(it++->branch())));
                    }
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


    if(buffer.size() != 1)
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




