#pragma once

class AParserConstant {
 public:
  // mathematical operators
  static char const PLUS[];
  static char const MINUS[];
  static char const MULTIPLY[];
  static char const DIVIDE[];
  static char const MODULO[];

  // constants
  static char const LEFT_PARENTHESIS[];
  static char const RIGHT_PARENTHESIS[];
  static char const STMT_TERMINATOR[];
  static int const MINIMUM_OPERATOR_SIZE;
};
