#include "diffirentiator.hpp"
#include "diff_tree.hpp"
#include "file_analize.hpp"
#include "utilities.hpp"
#include <assert.h>
#include <float.h>
#include <malloc.h>
#include <string.h>


extern FILE *tex_file;

//--------------------------------------------------------------------------------------------------------------------

TreeNode *Diff(const TreeNode *node, bool is_dump_to_tex,
                const char *diff_var) {
    assert(node);

    if (is_dump_to_tex) {
        Print_tex_keywords(tex_file);
        fprintf(tex_file, "$");
        Print_tex(tex_file, 1, node);
        fprintf(tex_file, " = ");
    }

    switch (node->type) {
    case NodeType::num: {
        TEX_NUM(0);
        return New_num(0);
    }
    case NodeType::var: {
        if (strcmp(node->var_value, diff_var)) {
            TEX_NUM(0);
            return New_num(0);
        }

        TEX_NUM(1);
        return New_num(1);
    }
    case NodeType::op: {
        switch (node->op_value) {
        case Operator::ADD: {
            TEX_DIFF_SUBTREE(Operator::ADD);
            return Add(dL, dR);
        }
        case Operator::SUB: {
            TEX_DIFF_SUBTREE(Operator::SUB);
            return Sub(dL, dR);
        }
        case Operator::MUL: {
            TEX_DIFF_SUBTREE(Operator::MUL);
            return Add(Mul(dL, cR), Mul(cL, dR));
        }
        case Operator::DIV: {
            TEX_DIFF_SUBTREE(Operator::DIV);
            return Div(Sub(Mul(dL, cR), Mul(cL, dR)), Mul(cR, cR));
        }
        case Operator::SIN: {
            TEX_DIFF_SUBTREE(Operator::SIN);
            return Mul(Cos(cR), dR);
        }
        case Operator::COS: {
            TEX_DIFF_SUBTREE(Operator::COS);
            return Mul(Mul(Sin(cR), New_num(-1)), dR);
        }
        case Operator::LOG: {
            TEX_DIFF_SUBTREE(Operator::LOG);
            return Div(dR, Mul(cR, Log(New_num(10))));
        }
        case Operator::PWR: {
            TEX_DIFF_SUBTREE(Operator::PWR);
            if (node->right->type == NodeType::num)
                return Mul(Mul(cR, Pwr(cL, Sub(cR, New_num(1)))), dL);

            else if (node->left->type == NodeType::num)
                return Mul(Ln(cL), Mul(Pwr(cL, cR), dR));

            return Mul(Add(Mul(dR, Ln(cL)), Mul(cR, Div(dL, cR))), Pwr(cL, cR));
        }
        case Operator::LN: {
            TEX_DIFF_SUBTREE(Operator::LN);
            return Div(dR, cR);
        }
        case Operator::TAN: {
            TEX_DIFF_SUBTREE(Operator::TAN);
            return Div(dR, Pwr(Cos(cR), New_num(2)));
        }
        case Operator::COT: {
            TEX_DIFF_SUBTREE(Operator::COT);
            return Div(dR, Pwr(Sin(cR), New_num(2)));
        }
        case Operator::NON:
            return nullptr;

        default:
            break;
        }
        break;
    }
    default:
        break;
    }

    return nullptr;
}

//--------------------------------------------------------------------------------------------------------------------

TreeNode *Constant_simplification(TreeNode *node, bool *is_modifed) {
    assert(node);
    assert(is_modifed);

    if (node->type == NodeType::op and !is_unary(node->op_value)) {
        node->left = Constant_simplification(node->left, is_modifed);
        node->right = Constant_simplification(node->right, is_modifed);

        if (node->left->type == NodeType::num and node->right->type == NodeType::num) {
            double temp_left = node->left->num_value;
            double temp_right = node->right->num_value;

            free(node->left);
            free(node->right);
            free(node);
            *is_modifed = true;

            if (node->op_value == Operator::ADD)
                node = New_num(temp_right + temp_left);

            if (node->op_value == Operator::MUL)
                node = New_num(temp_right * temp_left);

            if (node->op_value == Operator::SUB)
                node = New_num(temp_left - temp_right);

            if (node->op_value == Operator::DIV)
                node = New_num(temp_left / temp_right);
        }
    }

    return node;
}

//--------------------------------------------------------------------------------------------------------------------

static TreeNode *Removal_neutral_elements(TreeNode *node, bool *is_modifed) {
    assert(node);
    assert(is_modifed);

    if (node->type == NodeType::op and !is_unary(node->op_value)) {
        node->left = Removal_neutral_elements(node->left, is_modifed);
        node->right = Removal_neutral_elements(node->right, is_modifed);

        if (TYPE_IS(left, NodeType::num) and OP_IS(Operator::MUL) and IS_VAL(left, 0))
            FREE_AND_MODIFICATE(right, left)

        else if (TYPE_IS(right, NodeType::num) and OP_IS(Operator::MUL) and IS_VAL(right, 0))
            FREE_AND_MODIFICATE(left, right)

        else if (TYPE_IS(right, NodeType::num) and OP_IS(Operator::ADD) and IS_VAL(right, 0))
            FREE_AND_MODIFICATE(right, left)

        else if (TYPE_IS(left, NodeType::num) and OP_IS(Operator::ADD) and IS_VAL(left, 0))
            FREE_AND_MODIFICATE(left, right)

        else if (TYPE_IS(left, NodeType::num) and OP_IS(Operator::MUL) and IS_VAL(left, 1))
            FREE_AND_MODIFICATE(left, right)

        else if (TYPE_IS(right, NodeType::num) and OP_IS(Operator::MUL) and IS_VAL(right, 1))
            FREE_AND_MODIFICATE(right, left)

        else if (TYPE_IS(right, NodeType::num) and OP_IS(Operator::PWR) and IS_VAL(right, 1))
            FREE_AND_MODIFICATE(right, left)

        else if (TYPE_IS(left, NodeType::num) and OP_IS(Operator::DIV) and IS_VAL(left, 0))
            FREE_AND_MODIFICATE(right, left)

        else if (TYPE_IS(right, NodeType::num) and OP_IS(Operator::MUL) and
                 !(TYPE_IS(left, NodeType::num))) {
            TreeNode *temp = node->left;
            node->left = node->right;
            node->right = temp;
        }
    }

    return node;
}

//--------------------------------------------------------------------------------------------------------------------

TreeNode *Simpler(TreeNode *node) {
    assert(node);

    bool is_modifed = true;

    while (is_modifed) {
        is_modifed = false;
        node = Constant_simplification(node, &is_modifed);
        node = Removal_neutral_elements(node, &is_modifed);
    }
    return node;
}

//--------------------------------------------------------------------------------------------------------------------

TreeNode *Copy(TreeNode *node) {
    if (node) {
        TreeNode *copy_node = new TreeNode();

        copy_node->type = node->type;

        if (node->type == NodeType::var) {
            char *temp_var = new char[max_cmd_size];
            temp_var = strcpy(temp_var, node->var_value);
            copy_node->var_value = temp_var;
        }
        if (node->type == NodeType::num)
            copy_node->num_value = node->num_value;

        if (node->type == NodeType::op)
            copy_node->op_value = node->op_value;

        copy_node->right = Copy(node->right);
        copy_node->left = Copy(node->left);

        return copy_node;
    }

    return nullptr;
}

//--------------------------------------------------------------------------------------------------------------------

double Calculate_func(TreeNode *node, double x_value, Constants *f_constants) {
    assert(node);

    switch (node->type) {
    case NodeType::num:
        return node->num_value;
    case NodeType::var: {
        if (*node->var_value == 'x')
            return x_value;
        else
            return Find_const_by_name(node->var_value, f_constants);
    }

    case NodeType::op: {
        switch (node->op_value) {
        case Operator::ADD:
            return (CALC_L + CALC_R);
        case Operator::MUL:
            return (CALC_L * CALC_R);
        case Operator::SUB:
            return (CALC_L - CALC_R);
        case Operator::DIV:
            return (CALC_L / CALC_R);

        case Operator::SIN:
            return sin(CALC_R);
        case Operator::COS:
            return cos(CALC_R);
        case Operator::PWR:
            return pow(CALC_L, CALC_R);
        case Operator::LN:
            return log(CALC_R);
        case Operator::TAN:
            return tan(CALC_R);
        case Operator::COT:
            return tan(M_PI / 2 - CALC_R);

        case Operator::NON:
            printf("invalid operator");
        default:
            break;
        }
        break;
    }
    default:
        break;
    }

    return NAN;
}

double Find_const_by_name(const char *var_name, Constants *f_constants) {
    for (size_t index = 0; index < f_constants->capacity; index++) {
        if (strcmp(var_name, f_constants->list[index].var_name) == 0)
            return f_constants->list[index].var_value;
    }
    return NAN;
}
