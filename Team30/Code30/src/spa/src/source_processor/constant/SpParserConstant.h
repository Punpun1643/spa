#pragma once

class SpParserConstant {
 public:
  // Procedure boundaries
  static char const START_PROCEDURE[];
  static char const END_PROCEDURE[];

  // While statment boundaries
  static char const START_WHILE_STMTLST[];
  static char const END_WHILE_STMTLST[];

  // Conditional statement boundaries
  static char const START_COND_EXPR[];
  static char const END_COND_EXPR[];

  // If statement boundaries
  static char const START_THEN_STMTLST[];
  static char const END_THEN_STMTLST[];
  static char const START_ELSE_STMTLST[];
  static char const END_ELSE_STMTLST[];

  // Assignment symbol
  static char const ASSIGN_SYMBOL[];

  static char const PROCEDURE_KEYWORD[];
  static char const PRINT_KEYWORD[];
  static char const READ_KEYWORD[];
  static char const CALL_KEYWORD[];
  static char const WHILE_KEYWORD[];
  static char const IF_KEYWORD[];
  static char const THEN_KEYWORD[];
  static char const ELSE_KEYWORD[];

  // Comparison operators for conditional expressions
  static char const EQUAL[];
  static char const NOT_EQUAL[];
  static char const LESS_THAN[];
  static char const LESS_THAN_EQUAL[];
  static char const GREATER_THAN[];
  static char const GREATER_THAN_EQUAL[];

  // Logical operators for combining conditional expressions
  static char const AND[];
  static char const OR[];
  static char const NOT[];
};
