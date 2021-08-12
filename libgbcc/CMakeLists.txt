

set(SOURCES
  utility_text.cpp
  utility_text_iterator.cpp
  utility.cpp
  syntaxes/syntax_token.cpp
  syntaxes/syntax_token_string_iterator.cpp
  syntaxes/syntax_token_string.cpp
  syntaxes/syntax_token_string__read_number.cpp
  syntaxes/syntax_element.cpp
  syntaxes/syntax_formula.cpp
  syntaxes/syntax_rule.cpp
  syntaxes/syntax_branch.cpp
  syntaxes/syntax_branch_element.cpp
  syntaxes/syntax_parser.cpp
  syntaxes/syntax_parser__lower.cpp
#  vms/typesystems/struct.cpp
#  vms/typesystems/union.cpp
#  vms/typesystems/enum.cpp
#  vms/typesystems/type_info.cpp
#  vms/typesystems/function_signature.cpp
#  vms/expressions/expression.cpp
#  vms/expressions/postfix_element.cpp
#  vms/expressions/operand.cpp
#  vms/expressions/simple_value.cpp
  vms/vm_type_info.cpp
  vms/vm_opcode.cpp
  vms/vm_value.cpp
  vms/vm_memory.cpp
  vms/vm__print.cpp
  vms/vm_register.cpp
  vms/vm_line.cpp
  vms/vm_function.cpp
  vms/vm_jobfield.cpp
  vms/vm_execution.cpp
)


include_directories(./)
include_directories(../)
add_library("gbcc" STATIC ${SOURCES})




