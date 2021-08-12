#ifndef libgbcc_utility_HPP
#define libgbcc_utility_HPP


#include<string>
#include<vector>




#ifndef report
#define report printf("[report %8d] %s, %s, %d\n",gbcc::g_reporting_counter++,__FILE__,__func__,__LINE__);
#endif




namespace gbcc{


extern int  g_reporting_counter;


class
utf8_decoder
{
  const char*  m_pointer=nullptr;
  const char*  m_end    =nullptr;

public:
  utf8_decoder(                    ) noexcept{}
  utf8_decoder(std::string_view  sv) noexcept{assign(sv);}

  utf8_decoder&  operator=(std::string_view  sv) noexcept{return assign(sv);}

  utf8_decoder&  assign(std::string_view  sv) noexcept;

  operator bool() const noexcept{return m_pointer < m_end;}

  char32_t  operator()() noexcept;

  const char*  get_pointer() const noexcept{return m_pointer;}

};


size_t  u8clen(unsigned char  c) noexcept;
size_t  u8slen(const char*  s) noexcept;


struct
utf8_encoder
{
  char  codes[8];

  utf8_encoder(char32_t  c=0) noexcept{(*this)(c);}

  utf8_encoder&  operator()(char32_t  c) noexcept;

};


std::string     make_string(std::u16string_view  sv) noexcept;
std::u16string  make_u16string(std::string_view  sv) noexcept;

std::string  make_string_from_file(std::string_view  path) noexcept;

void  print(std::u16string_view  sv) noexcept;


class
text
{
  using count_t = uint64_t;

  struct data;

  data*  m_data=nullptr;

  static const char16_t  m_null;

public:
  text() noexcept{}
  text(const char*  path) noexcept{load(path);}
  text(std::string_view  path) noexcept{load(path);}
 ~text(){unrefer();}

  template<class...  Args>
  explicit text(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  template<class...  Args>
  text&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  text(const text&   rhs) noexcept{assign(rhs);}
  text(      text&&  rhs) noexcept{assign(std::move(rhs));}

  text&  operator=(const text&   rhs) noexcept{return assign(rhs);}
  text&  operator=(      text&&  rhs) noexcept{return assign(std::move(rhs));}

  operator bool() const noexcept{return m_data;}

  void  unrefer() noexcept;

  text&  assign(const text&   rhs) noexcept;
  text&  assign(      text&&  rhs) noexcept;
  text&  assign(std::u16string_view  sv) noexcept;
  text&  assign(std::u16string&&  s) noexcept;

  text&  load(std::string_view  path) noexcept;

  const std::string&  path() const noexcept;

  const std::u16string&  content() const noexcept;

  count_t  reference_count() const noexcept;
  size_t  size() const noexcept;
  size_t  number_of_lines() const noexcept;

  const std::u16string_view*  begin() const noexcept;
  const std::u16string_view*    end() const noexcept;

  void  print() const noexcept;

  class iterator;

};


class
text::
iterator
{
  text  m_source;

  const char16_t*  m_pointer;

  int  m_x_position=0;
  int  m_y_position=0;

public:
  iterator() noexcept{}

  template<class...  Args>
  iterator(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  template<class...  Args>
  iterator&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  iterator&  assign(const iterator&   rhs) noexcept;
  iterator&  assign(      iterator&&  rhs) noexcept;
  iterator&  assign(const char16_t*  p=nullptr) noexcept;
  iterator&  assign(const text&  src) noexcept;

  operator bool() const noexcept{return m_pointer;}

  const text&  source() const noexcept{return m_source;}

  const char16_t&  operator[](int  i) const noexcept{return m_pointer[i];}

  const char16_t&  operator*() const noexcept{return *m_pointer;}

  bool  operator==(const iterator&  rhs) const noexcept{return m_pointer == rhs.m_pointer;}
  bool  operator!=(const iterator&  rhs) const noexcept{return m_pointer != rhs.m_pointer;}

  int  x_position() const noexcept{return m_x_position;}
  int  y_position() const noexcept{return m_y_position;}

  iterator   operator+ (int  n) const noexcept;
  iterator&  operator+=(int  n) noexcept;

  iterator&  operator++(   ) noexcept;
  iterator   operator++(int) noexcept;

  void  skip_spaces() noexcept;
  void  skip_to_newline() noexcept;
  void  skip_to_block_end() noexcept;
  void  print() const noexcept;

};




class
small_string
{
  char  m_data[4];

  static constexpr char  chr(std::string_view  sv, int  i) noexcept
  {
    return (i < sv.size())? sv[i]:0;
  }

public:
  constexpr small_string() noexcept: m_data{0,0,0,0}{}
  constexpr small_string(const char*  s) noexcept: m_data{chr(s,0),chr(s,1),chr(s,2),0}{}
  constexpr small_string(std::string_view  sv) noexcept: m_data{chr(sv,0),chr(sv,1),chr(sv,2),0}{}

  small_string&  operator=(std::string_view  sv) noexcept{return assign(sv);}

  small_string&  assign(std::string_view  sv) noexcept{
    m_data[0] = chr(sv,0);
    m_data[1] = chr(sv,1);
    m_data[2] = chr(sv,2);

    return *this;
  }

  constexpr  operator bool() const noexcept{return m_data[0];}

  constexpr const char&  operator[](int  i) const noexcept{return m_data[i];}

  constexpr const char*  data() const noexcept{return m_data;}
  constexpr int  size() const noexcept{return !m_data[0]? 0
                                             :!m_data[1]? 1
                                             :!m_data[2]? 2
                                             :            3;}

  void  clear() noexcept{reinterpret_cast<int32_t&>(m_data[0]) = 0;}

  constexpr  operator uint32_t() const noexcept{return (m_data[0]<<24)|(m_data[1]<<16)|(m_data[2]<<8);}

  constexpr bool  operator==(small_string  rhs) const noexcept
  {
    return (m_data[0] == rhs.m_data[0]) &&
           (m_data[1] == rhs.m_data[1]) &&
           (m_data[2] == rhs.m_data[2]);
  }

  constexpr bool  operator!=(small_string  rhs) const noexcept
  {
    return !(*this == rhs);
  }

  constexpr const char*  begin() const noexcept{return m_data;}
  constexpr const char*    end() const noexcept{return m_data+size();}

  void  print() const noexcept
  {
    printf("%s",m_data);
  }

};




constexpr bool
is_alphabet(char  c) noexcept
{
  return(((c >= 'a') && (c <= 'z')) ||
         ((c >= 'A') && (c <= 'Z')));
}


constexpr bool
is_number(char  c) noexcept
{
  return((c >= '0') && (c <= '9'));
}


constexpr bool
is_alphabet_or_number(char  c) noexcept
{
  return(is_alphabet(c) || is_number(c));
}




}




#endif




