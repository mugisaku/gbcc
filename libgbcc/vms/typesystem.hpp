#ifndef gbstd_vm_typesystem_HPP
#define gbstd_vm_typesystem_HPP


#include<cinttypes>
#include<cstdio>
#include<string>
#include<string_view>
#include<new>
#include<list>
#include<array>
#include<memory>
#include<vector>
#include"libgbcc/utility.hpp"




namespace gbcc{




class sc_type_info;

class    sc_pointer_info;
class  sc_reference_info;
class      sc_array_info;

class  sc_struct_definition;
class   sc_union_definition;
class    sc_enum_definition;

class  sc_parameter;
class  sc_signature;
class  sc_function_pointer_info;


constexpr int  g_pointer_size = 8;


class
sc_type_info
{
  int  m_letter;
  int    m_size;
  int   m_align;

  std::string  m_id;

  union data{
    sc_pointer_info*     ptr;
    sc_reference_info*   ref;
    sc_array_info*       arr;

    sc_struct_definition*  st;
    sc_union_definition*   un;
    sc_enum_definition*    en;

    sc_signature*  sig;

    data() noexcept{}
   ~data(){}

  } m_data;

public:
  sc_type_info(char  l=0, int  sz=0) noexcept: m_letter(l), m_size(sz), m_align(sz){}
 ~sc_type_info(){clear();}

  template<class...  Args>
  sc_type_info(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  sc_type_info(const sc_type_info&   rhs) noexcept{assign(rhs);}
  sc_type_info(      sc_type_info&&  rhs) noexcept{assign(std::move(rhs));}

  template<class...  Args>
  sc_type_info&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  sc_type_info&  operator=(const sc_type_info&   rhs) noexcept{return assign(rhs);}
  sc_type_info&  operator=(      sc_type_info&&  rhs) noexcept{return assign(std::move(rhs));}

  sc_type_info&  assign(const sc_type_info&   rhs) noexcept;
  sc_type_info&  assign(      sc_type_info&&  rhs) noexcept;
  sc_type_info&  assign(sc_pointer_info&&  ptr) noexcept;
  sc_type_info&  assign(sc_reference_info&&  ref) noexcept;
  sc_type_info&  assign(sc_array_info&&  arr) noexcept;
  sc_type_info&  assign(sc_struct_definition&&  st) noexcept;
  sc_type_info&  assign(sc_union_definition&&  un) noexcept;
  sc_type_info&  assign(sc_enum_definition&&  en) noexcept;
  sc_type_info&  assign(sc_signature&&  sig) noexcept;

  void  clear() noexcept;

  operator bool() const noexcept{return m_letter;}

  bool  operator==(const sc_type_info&  rhs) const noexcept{return m_id == rhs.m_id;}
  bool  operator!=(const sc_type_info&  rhs) const noexcept{return m_id != rhs.m_id;}

  const std::string&  id() const noexcept{return m_id;}

  bool  is_void()             const noexcept{return m_letter == 'V';}
  bool  is_undefined()        const noexcept{return m_letter == 'U';}
  bool  is_null_pointer()     const noexcept{return m_letter == 'N';}

  bool  is_boolean()          const noexcept{return m_letter == 'b';}
  bool  is_character()        const noexcept{return m_letter == 'c';}
  bool  is_integer()          const noexcept{return m_letter == 'i';}
  bool  is_unsigned_integer() const noexcept{return m_letter == 'u';}
  bool  is_floating()         const noexcept{return m_letter == 'f';}

  bool  is_pointer()   const noexcept{return m_letter == 'p';}
  bool  is_reference() const noexcept{return m_letter == 'r';}
  bool  is_array()     const noexcept{return m_letter == 'a';}
  bool  is_struct()    const noexcept{return m_letter == 's';}
  bool  is_union()     const noexcept{return m_letter == 'o';}
  bool  is_enum()      const noexcept{return m_letter == 'e';}
  bool  is_function()  const noexcept{return m_letter == 'F';}

  int   size() const noexcept{return m_size;}
  int  align() const noexcept{return m_align;}

  const sc_pointer_info&       pointer_info() const noexcept{return *m_data.ptr;}
  const sc_reference_info&   reference_info() const noexcept{return *m_data.ref;}
  const sc_array_info&           array_info() const noexcept{return *m_data.arr;}

  const sc_struct_definition&  struct_definition() const noexcept{return *m_data.st;}
  const sc_union_definition&    union_definition() const noexcept{return *m_data.un;}
  const sc_enum_definition&      enum_definition() const noexcept{return *m_data.en;}

  const sc_signature&  signature() const noexcept{return *m_data.sig;}

  void  print() const noexcept;

};




class
sc_pointer_info
{
protected:
  sc_type_info  m_target;

public:
  sc_pointer_info(const sc_type_info&   target) noexcept: m_target(target){}
  sc_pointer_info(      sc_type_info&&  target) noexcept: m_target(std::move(target)){}

  const sc_type_info&  target() const noexcept{return m_target;}

  int   size() const noexcept{return g_pointer_size;}
  int  align() const noexcept{return g_pointer_size;}

  void  print() const noexcept
  {
    m_target.print();

    printf("*");
  }

};


class
sc_reference_info: public sc_pointer_info
{
public:
  sc_reference_info(const sc_type_info&   target) noexcept: sc_pointer_info(target           ){}
  sc_reference_info(      sc_type_info&&  target) noexcept: sc_pointer_info(std::move(target)){}

  void  print() const noexcept
  {
    m_target.print();

    printf("&");
  }

};




class
sc_array_info
{
  sc_type_info  m_target;

  int  m_number_of_elements;

public:
  sc_array_info(const sc_type_info&  t, int  n) noexcept:
  m_target(t), m_number_of_elements(n){}

  sc_array_info(sc_type_info&&  t, int  n) noexcept:
  m_target(std::move(t)), m_number_of_elements(n){}

  const sc_type_info&  target() const noexcept{return m_target;}

  int  number_of_elements() const noexcept{return m_number_of_elements;}

  int   size() const noexcept{return m_target.size()*m_number_of_elements;}
  int  align() const noexcept{return m_target.align();}

  void  print() const noexcept
  {
    m_target.print();

    printf("[%d]",m_number_of_elements);
  }

};




class
sc_struct_member
{
  sc_type_info  m_type_info;

  std::u16string  m_name;

  int  m_offset;

public:
  sc_struct_member(sc_type_info&&  ti, std::u16string_view  name, int  offset) noexcept:
  m_type_info(std::move(ti)), m_name(name), m_offset(offset){}

  const sc_type_info&  type_info() const noexcept{return m_type_info;}

  const std::u16string&  name() const noexcept{return m_name;}

  int  offset() const noexcept{return m_offset;}

};


class
sc_struct_definition
{
  std::vector<sc_struct_member>  m_member_list;

  int  m_size =0;
  int  m_align=0;

public:
  void  push(const sc_type_info&   ti, std::u16string_view  name) noexcept{push(sc_type_info(ti),name);}
  void  push(      sc_type_info&&  ti, std::u16string_view  name) noexcept;

  const sc_struct_member*  find(std::u16string_view  name) const noexcept;

  int   size() const noexcept{return m_size;}
  int  align() const noexcept{return m_align;}

  std::string  id() const noexcept;

  void  print() const noexcept;

};




class
sc_union_definition
{
  struct member{
    sc_type_info  m_type_info;

    std::u16string  m_name;

    member(sc_type_info&&  ti, std::u16string_view  name) noexcept: m_type_info(std::move(ti)), m_name(name){}
  };

  std::vector<member>  m_member_list;

  int  m_size =0;
  int  m_align=0;

public:
  void  push(const sc_type_info&   ti, std::u16string_view  name) noexcept{push(sc_type_info(ti),name);}
  void  push(      sc_type_info&&  ti, std::u16string_view  name) noexcept;

  const sc_type_info*  find(std::u16string_view  name) const noexcept;

  int   size() const noexcept{return m_size;}
  int  align() const noexcept{return m_align;}

  std::string  id() const noexcept;

  void  print() const noexcept;

};




class
sc_enumerator
{
  std::u16string  m_name;

  int64_t  m_value;

public:
  sc_enumerator(std::u16string_view  name, int64_t  value) noexcept:
  m_name(name), m_value(value){}

  const std::u16string&  name() const noexcept{return m_name;}

  const int64_t&  value() const noexcept{return m_value;}

  void  print() const noexcept
  {
    gbcc::print(m_name);
    printf(" = %" PRIi64,m_value);
  }

};


class
sc_enum_definition
{
  std::vector<sc_enumerator>  m_enumerator_list;

public:
  sc_enumerator&  push(std::u16string_view  name, int  value) noexcept;

  const int64_t*  find(std::u16string_view  name) const noexcept;

  std::string  id() const noexcept;

  void  print() const noexcept;

};




class
sc_parameter
{
  std::u16string  m_name;

  sc_type_info  m_type_info;

public:
  sc_parameter() noexcept{}
  sc_parameter(sc_type_info&&  ti) noexcept: m_type_info(std::move(ti)){}
  sc_parameter(std::u16string_view  name, sc_type_info&&  ti) noexcept: m_name(name), m_type_info(std::move(ti)){}

  const std::u16string&  name() const noexcept{return m_name;}

  const sc_type_info&  type_info() const noexcept{return m_type_info;}

};


using sc_parameter_list = std::vector<sc_parameter>;


class
sc_signature
{
  sc_type_info  m_type_info;

  sc_parameter_list  m_parameter_list;

public:
  sc_signature() noexcept{}

  sc_signature(sc_type_info&&  ti, sc_parameter_list&&  ls) noexcept:
  m_type_info(std::move(ti)), m_parameter_list(std::move(ls)){}

  const sc_type_info&  type_info() const noexcept{return m_type_info;}

  const sc_parameter_list&  parameter_list() const noexcept{return m_parameter_list;}

  std::string  id() const noexcept;

  void  print() const noexcept;

};




sc_type_info      make_array(const sc_type_info&  ti, int  n) noexcept;
sc_type_info    make_pointer(const sc_type_info&  ti) noexcept;
sc_type_info  make_reference(const sc_type_info&  ti) noexcept;

sc_type_info      remove_array(const sc_type_info&  ti) noexcept;
sc_type_info    remove_pointer(const sc_type_info&  ti) noexcept;
sc_type_info  remove_reference(const sc_type_info&  ti) noexcept;


extern const sc_type_info     sc_int8_ti;
extern const sc_type_info    sc_int16_ti;
extern const sc_type_info    sc_int32_ti;
extern const sc_type_info    sc_int64_ti;
extern const sc_type_info    sc_uint8_ti;
extern const sc_type_info   sc_uint16_ti;
extern const sc_type_info   sc_uint32_ti;
extern const sc_type_info   sc_uint64_ti;
extern const sc_type_info  sc_float32_ti;
extern const sc_type_info  sc_float64_ti;
extern const sc_type_info    sc_bool8_ti;
extern const sc_type_info   sc_bool16_ti;
extern const sc_type_info   sc_bool32_ti;
extern const sc_type_info   sc_bool64_ti;
extern const sc_type_info   sc_void_ti;
extern const sc_type_info   sc_undef_ti;
extern const sc_type_info   sc_nullptr_ti;


}




#endif




