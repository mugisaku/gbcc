

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
  vms/expressions/sc_value.cpp
  vms/expressions/sc_operand.cpp
  vms/expressions/sc_unary_operation_prefix_element.cpp
  vms/expressions/sc_unary_operation_postfix_element.cpp
  vms/expressions/sc_unary_operation.cpp
  vms/expressions/sc_unary_operation__operate.cpp
  vms/expressions/sc_binary_operation.cpp
  vms/expressions/sc_binary_operation__operate.cpp
  vms/expressions/sc_expression.cpp
  vms/expressions/sc_expression__assign.cpp
  vms/expressions/sc_expression_element.cpp
  vms/typesystems/sc_struct_definition.cpp
  vms/typesystems/sc_union_definition.cpp
  vms/typesystems/sc_enum_definition.cpp
  vms/typesystems/sc_type_info.cpp
  vms/typesystems/sc_signature.cpp
  vms/contexts/sc_statement.cpp
  vms/contexts/sc_block.cpp
  vms/contexts/sc_function.cpp
  vms/contexts/sc_context.cpp
  vms/contexts/sc_context__construct.cpp
  vms/contexts/sc_context__process.cpp
  vms/vm_type_info.cpp
  vms/vm_opcode.cpp
  vms/vm_value.cpp
  vms/vm_memory.cpp
  vms/vm__print.cpp
  vms/vm_register.cpp
  vms/vm_line.cpp
  vms/vm_function.cpp
  vms/vm_function__construct.cpp
  vms/vm_jobfield.cpp
  vms/vm_execution.cpp
  vms/vm_execution__process.cpp
)


include_directories(./)
include_directories(../)
add_library("gbcc" STATIC ${SOURCES})




