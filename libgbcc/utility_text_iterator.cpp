#include"libgbcc/utility.hpp"




namespace gbcc{




text::iterator&
text::iterator::
assign(const iterator&  rhs) noexcept
{
    if(this != &rhs)
    {
      m_source  = rhs.m_source;
      m_pointer = rhs.m_pointer;

      m_x_position = rhs.m_x_position;
      m_y_position = rhs.m_y_position;
    }


  return *this;
}


text::iterator&
text::iterator::
assign(iterator&&  rhs) noexcept
{
    if(this != &rhs)
    {
      std::swap(m_source ,rhs.m_source );
      std::swap(m_pointer,rhs.m_pointer);

      std::swap(m_x_position,rhs.m_x_position);
      std::swap(m_y_position,rhs.m_y_position);
    }


  return *this;
}


text::iterator&
text::iterator::
assign(const char16_t*  p) noexcept
{
  m_source.unrefer();

  m_pointer = p;

  m_x_position = 0;
  m_y_position = 0;

  return *this;
}


text::iterator&
text::iterator::
assign(const text&  src) noexcept
{
  m_source = src;

  m_pointer = src.content().data();

  m_x_position = 0;
  m_y_position = 0;

  return *this;
}


text::iterator
text::iterator::
operator+(int  n) const noexcept
{
  auto  t = *this;

  t += n;

  return t;
}


text::iterator&
text::iterator::
operator++() noexcept
{
  auto  c = *m_pointer++;

    if(c == '\n')
    {
      m_x_position  = 0;
      m_y_position += 1;
    }

  else
    {
      ++m_x_position;
    }


  return *this;
}


text::iterator
text::iterator::
operator++(int) noexcept
{
  auto  tmp = *this;

  ++(*this);

  return tmp;
}


text::iterator&
text::iterator::
operator+=(int  n) noexcept
{
    if(n > 0)
    {
        while(n--)
        {
          ++(*this);
        }
    }


  return *this;
}




void
text::iterator::
print() const noexcept
{
  printf("{%4d行目, %4d文字目}",1+m_y_position,1+m_x_position);
}


void
text::iterator::
skip_spaces() noexcept
{
    for(;;)
    {
      auto  c = **this;

        if((c == ' ') ||
           (c == '\t'))
        {
          ++m_pointer;
          ++m_x_position;
        }

      else
        if(c == '\n')
        {
          ++m_pointer;

          m_x_position  = 0;
          m_y_position += 1;
        }

      else
        {
          break;
        }
    }
}


void
text::iterator::
skip_to_newline() noexcept
{
    for(;;)
    {
      auto  c = **this;

        if(c == '\0')
        {
          break;
        }

      else
        if(c == '\n')
        {
          ++m_pointer;

          m_x_position  = 0;
          m_y_position += 1;

          break;
        }

      else
        {
          ++m_pointer;
          ++m_x_position;
        }
    }
}


void
text::iterator::
skip_to_block_end() noexcept
{
    for(;;)
    {
      auto  c = **this;

        if(c == '\0')
        {
          report;

          break;
        }

      else
        if(c == '\n')
        {
          ++m_pointer;

          m_x_position  = 0;
          m_y_position += 1;
        }

      else
        {
          ++m_pointer;
          ++m_x_position;

            if((c == '*') && (*m_pointer == '/'))
            {
              ++m_pointer;
              ++m_x_position;

              break;
            }
        }
    }
}




}




