#include "utilities.hpp"
#include <assert.h>
#include <float.h>

bool is_equal(double number_1, double number_2) {
    assert(isfinite(number_1));
    assert(isfinite(number_2));

    return fabs(number_1 - number_2) < DBL_EPSILON;
}

bool is_unary(Operator op_value) {
    return (op_value == Operator::SIN or op_value == Operator::COS or static_cast<int>(op_value) >= 8);
}

const char *convert_tex_op(Operator op_value) {
    switch (op_value) {
    case Operator::ADD:
        return " + ";
    case Operator::SUB:
        return " - ";
    case Operator::MUL:
        return " \\cdot ";
    case Operator::DIV:
        return " \\over ";
    case Operator::SIN:
        return "\\sin";
    case Operator::COS:
        return "\\cos";
    case Operator::PWR:
        return "^";
    case Operator::LN:
        return "\\ln";
    case Operator::TAN:
        return "\\tg";
    case Operator::COT:
        return "\\ctg";
    case Operator::NON:
        return "non operator";
    default:
        break;
    }
    return "convert error";
}

const char *convert_op(Operator op_value) {
    switch (op_value) {
    case Operator::ADD:
        return " + ";
    case Operator::SUB:
        return " - ";
    case Operator::MUL:
        return " * ";
    case Operator::DIV:
        return " / ";
    case Operator::SIN:
        return "sin";
    case Operator::COS:
        return "cos";
    case Operator::PWR:
        return "**";
    case Operator::LN:
        return "log";
    case Operator::TAN:
        return "tan";
    case Operator::COT:
        return "\\ tan";
    case Operator::NON:
        return "non operator";
    default:
        break;
    }

    return "convert error";
}

int Factorial(int number) {
    if (number > 0)
        return (number * Factorial(number - 1));

    return 1;
}
