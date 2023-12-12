#pragma once

#define dL Diff(node->left, is_dump_to_tex, diff_var)
#define dR Diff(node->right, is_dump_to_tex, diff_var)
#define cL Copy(node->left)
#define cR Copy(node->right)
#define Add(L, R) New_operator(Operator::ADD, L, R)
#define Sub(L, R) New_operator(Operator::SUB, L, R)
#define Mul(L, R) New_operator(Operator::MUL, L, R)
#define Div(L, R) New_operator(Operator::DIV, L, R)
#define Sin(R) New_operator(Operator::SIN, nullptr, R)
#define Cos(R) New_operator(Operator::COS, nullptr, R)
#define Pwr(L, R) New_operator(Operator::PWR, L, R)
#define Ln(R) New_operator(Operator::LN, nullptr, R)
#define Log(R) New_operator(Operator::LOG, nullptr, R)

#define TEX_DIFF_SUBTREE(type)                                                 \
    if (is_dump_to_tex)                                                        \
    Tex_subtree(tex_file, type, node)

#define TEX_NUM(value)                                                         \
    if (is_dump_to_tex)                                                        \
    fprintf(tex_file, "%d$\n\n", value)

#define TYPE_IS(son, node_type) node->son->type == node_type
#define OP_IS(type) node->op_value == type
#define IS_VAL(son, val) is_equal(node->son->num_value, val)

#define FREE_AND_MODIFICATE(free_son, swap_son)                                \
    {                                                                          \
        TreeNode *temp = node->swap_son;                                      \
        delete node->free_son;                                                 \
        delete node;                                                           \
        node = temp;                                                           \
        *is_modifed = true;                                                    \
    }

#define CALC_L Calculate_func(node->left, x_value, f_constants)
#define CALC_R Calculate_func(node->right, x_value, f_constants)
