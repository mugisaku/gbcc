#include"libgbcc/vms/context.hpp"




namespace gbcc{




sc_statement&
sc_statement::
assign(const sc_statement&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind = rhs.m_kind;

        switch(m_kind)
        {
      case(kind::return_   ):
      case(kind::case_     ):
      case(kind::expression): new(&m_data) sc_expression(rhs.m_data.expr);break;

      case(kind::goto_):
      case(kind::label): new(&m_data) std::u16string(rhs.m_data.s);break;

      case(kind::while_ ):
      case(kind::switch_): new(&m_data) sc_conditional_block(rhs.m_data.cblk);break;

      case(kind::if_): new(&m_data) sc_conditional_block_list(rhs.m_data.cblkls);break;

      case(kind::var   ):
      case(kind::const_): new(&m_data) sc_var(rhs.m_data.var);break;

      case(kind::for_): break;

      case(kind::block): new(&m_data) sc_block(rhs.m_data.blk);break;
        }
    }


  return *this;
}


sc_statement&
sc_statement::
assign(sc_statement&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind,rhs.m_kind);

        switch(m_kind)
        {
      case(kind::return_   ):
      case(kind::case_     ):
      case(kind::expression): new(&m_data) sc_expression(std::move(rhs.m_data.expr));break;

      case(kind::goto_):
      case(kind::label): new(&m_data) std::u16string(std::move(rhs.m_data.s));break;

      case(kind::while_ ):
      case(kind::switch_): new(&m_data) sc_conditional_block(std::move(rhs.m_data.cblk));break;

      case(kind::if_): new(&m_data) sc_conditional_block_list(std::move(rhs.m_data.cblkls));break;

      case(kind::var   ):
      case(kind::const_): new(&m_data) sc_var(std::move(rhs.m_data.var));break;

      case(kind::for_): break;

      case(kind::block): new(&m_data) sc_block(std::move(rhs.m_data.blk));break;
        }
    }


  return *this;
}


sc_statement&
sc_statement::
assign(sc_return&&  ret) noexcept
{
  clear();

  m_kind = kind::return_;

  new(&m_data) sc_expression(*ret);

  return *this;
}


sc_statement&
sc_statement::
assign(sc_break&&  brk) noexcept
{
  clear();

  m_kind = kind::break_;

  return *this;
}


sc_statement&
sc_statement::
assign(sc_continue&&  ctn) noexcept
{
  clear();

  m_kind = kind::continue_;

  return *this;
}


sc_statement&
sc_statement::
assign(sc_label&&  lb) noexcept
{
  clear();

  m_kind = kind::label;

  new(&m_data) std::u16string(*lb);

  return *this;
}


sc_statement&
sc_statement::
assign(sc_goto&&  gt) noexcept
{
  clear();

  m_kind = kind::goto_;

  new(&m_data) std::u16string(*gt);

  return *this;
}


sc_statement&
sc_statement::
assign(sc_if&&  i) noexcept
{
  clear();

  m_kind = kind::if_;

  new(&m_data) sc_conditional_block_list(*i);

  return *this;
}


sc_statement&
sc_statement::
assign(sc_while&&  wh) noexcept
{
  clear();

  m_kind = kind::while_;

  new(&m_data) sc_conditional_block(std::move(wh));

  return *this;
}


sc_statement&
sc_statement::
assign(sc_for&&  fo) noexcept
{
  clear();

  m_kind = kind::for_;

  new(&m_data) sc_conditional_block(std::move(fo));

  return *this;
}


sc_statement&
sc_statement::
assign(sc_switch&&  sw) noexcept
{
  clear();

  m_kind = kind::switch_;

  new(&m_data) sc_conditional_block(std::move(sw));

  return *this;
}


sc_statement&
sc_statement::
assign(sc_case&&  ca) noexcept
{
  clear();

  m_kind = kind::case_;

  new(&m_data) sc_expression(*ca);

  return *this;
}


sc_statement&
sc_statement::
assign(sc_default&&  de) noexcept
{
  clear();

  m_kind = kind::default_;

  return *this;
}


sc_statement&
sc_statement::
assign(sc_block&&  blk) noexcept
{
  clear();

  m_kind = kind::block;

  new(&m_data) sc_block(std::move(blk));

  return *this;
}


sc_statement&
sc_statement::
assign(sc_var&&  v) noexcept
{
  clear();

  m_kind = kind::var;

  new(&m_data) sc_var(std::move(v));

  return *this;
}


sc_statement&
sc_statement::
assign(sc_const&&  c) noexcept
{
  clear();

  m_kind = kind::const_;

  new(&m_data) sc_var(std::move(c));

  return *this;
}


sc_statement&
sc_statement::
assign(sc_expression&&  e) noexcept
{
  clear();

  m_kind = kind::expression;

  new(&m_data) sc_expression(std::move(e));

  return *this;
}




void
sc_statement::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::break_   ):
  case(kind::continue_):
  case(kind::default_ ): break;

  case(kind::return_   ):
  case(kind::case_     ):
  case(kind::expression): std::destroy_at(&m_data.expr);break;

  case(kind::goto_):
  case(kind::label): std::destroy_at(&m_data.s);break;

  case(kind::while_ ):
  case(kind::switch_): std::destroy_at(&m_data.cblk);break;

  case(kind::if_): std::destroy_at(&m_data.cblkls);break;

  case(kind::var   ):
  case(kind::const_): std::destroy_at(&m_data.var);break;

  case(kind::for_): break;

  case(kind::block): std::destroy_at(&m_data.blk);break;
    }


  m_kind = kind::null;
}


void
sc_statement::
print(int  depth) const noexcept
{
    switch(m_kind)
    {
  case(kind::break_   ): printf("break");break;
  case(kind::continue_): printf("continue");break;
  case(kind::default_ ): printf("default");break;

  case(kind::return_   ): printf("return ");  m_data.expr.print();break;
  case(kind::case_     ): printf("case ");  m_data.expr.print();printf(":");break;
  case(kind::expression): m_data.expr.print();break;

  case(kind::goto_): printf("goto ");  gbcc::print(m_data.s);break;
  case(kind::label): gbcc::print(m_data.s);  printf(":");break;

  case(kind::while_ ): printf("while");   m_data.cblk.print(depth);break;
  case(kind::switch_): printf("switch");  m_data.cblk.print(depth);break;

  case(kind::if_):
      printf("if");
        for(auto&  blk: m_data.cblkls)
        {
          printf("else");

          blk.print(depth);
        }
      break;
  case(kind::var):
      printf("var  ");
      gbcc::print(m_data.var.name());
      printf(" = ");
      m_data.var.type_info().print();
      printf("(");
      m_data.var.expression().print();
      printf(")");
      break;
  case(kind::const_):
      printf("const  ");
      gbcc::print(m_data.var.name());
      printf(" = ");
      m_data.var.type_info().print();
      printf("(");
      m_data.var.expression().print();
      printf(")");
      break;
  case(kind::for_):
      printf("for(");
      printf(")");
      break;
  case(kind::block): m_data.blk.print(); break;
    }
}




}




