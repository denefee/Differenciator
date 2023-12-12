#ifndef UTILITIES_HPP_INCLUDED
#define UTILITIES_HPP_INCLUDED

#include "diff_tree.hpp"
#include <math.h>


int Factorial(int number);
bool is_unary(Operator op_value);
bool is_equal(double number_1, double number_2);

const char *convert_op(Operator op_value);
const char *convert_tex_op(Operator op_value);

#endif