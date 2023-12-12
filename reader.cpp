#include "reader.hpp"
#include "utilities.hpp"
#include <assert.h>
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

const int op_number = 12;

const char *valid_operators[op_number] = {"sin", "cos", "+", "-",  "*",   "/",
                                          "^",   "(",   ")", "ln", "log", "tg"};
Operator valid_enum_op[op_number] = {Operator::SIN,  Operator::COS, Operator::ADD, Operator::SUB,
                                      Operator::MUL,  Operator::DIV, Operator::PWR, Operator::LBRC,
                                      Operator::RBRC, Operator::LN,  Operator::LOG, Operator::TAN};

static Tree_tokens *Tokens_ctor(size_t size) {
    Tree_tokens *tokens = new Tree_tokens();
    tokens->capacity = 0;
    tokens->size = 0;
    tokens->array = new TreeNode *[size];

    return tokens;
}

static void Skip_spaces(const char **str) {
    while (**str == ' ')
        *str += 1;
}

Tree_tokens *Tokenizer(const char **str) {

    Tree_tokens *tokens = Tokens_ctor(strlen(*str));
    TreeNode *node = nullptr;

    while (**str != '\0') {
        Skip_spaces(str);

        if ((node = Is_operator(str)))
            tokens->array[tokens->capacity++] = node;

        else if ((node = Is_num(str)))
            tokens->array[tokens->capacity++] = node;

        else if ((node = Is_variable(str)))
            tokens->array[tokens->capacity++] = node;

        else if (**str != '\0')
            printf("lex error\n");
    }

    tokens->array[tokens->capacity++] = New_operator(Operator::END, nullptr, nullptr);

    return tokens;
}

TreeNode *Is_num(const char **str) {
    double number = 0;
    int lenght = 0;

    if (sscanf(*str, "%lg%n", &number, &lenght) == 1) {
        *str += lenght;
        return New_num(number);
    }
    return nullptr;
}

TreeNode *Is_variable(const char **str) {
    char *var_value = new char[strlen(*str)];

    if (sscanf(*str, " %[\\a-z]", var_value) == 1) {
        *str += strlen(var_value);
        return New_var(var_value);
    }

    free(var_value);

    return nullptr;
}
// strdl
//

TreeNode *Is_operator(const char **str) {
    for (int index = 0; index < op_number; index++) {
        size_t size = strlen(valid_operators[index]);

        if (strncmp(*str, valid_operators[index], size) == 0) {
            *str += size;
            return New_operator(valid_enum_op[index], nullptr, nullptr);
        }
    }
    return nullptr;
}

TreeNode *Get_G(Tree_tokens *tokens) {
    assert(tokens);

    TreeNode *first_node = Get_E(tokens);

    if (IS_TYPE(NodeType::op) and IS_OP(Operator::END))
        printf("succes syntax analize\n");

    return first_node;
}

TreeNode *Get_E(Tree_tokens *tokens) {
    TreeNode *node_left = Get_T(tokens);

    while (IS_TYPE(NodeType::op) and (IS_OP(Operator::ADD) or IS_OP(Operator::SUB))) {
        Operator temp_op = tokens->array[tokens->size]->op_value;
        tokens->size += 1;

        TreeNode *node_right = Get_T(tokens);

        node_left = New_operator(temp_op, node_left, node_right);
    }
    return node_left;
}

TreeNode *Get_T(Tree_tokens *tokens) {
    TreeNode *node_left = Get_W(tokens);

    while (IS_TYPE(NodeType::op) and (IS_OP(Operator::MUL) or IS_OP(Operator::DIV))) {
        Operator temp_op = tokens->array[tokens->size]->op_value;
        tokens->size += 1;
        TreeNode *node_right = Get_W(tokens);

        node_left = New_operator(temp_op, node_left, node_right);
    }
    return node_left;
}

TreeNode *Get_P(Tree_tokens *tokens) {
    TreeNode *val = nullptr;

    if (IS_TYPE(NodeType::op) and IS_OP(Operator::LBRC)) {
        tokens->size += 1;
        val = Get_E(tokens);

        if (!(IS_TYPE(NodeType::op) and IS_OP(Operator::RBRC)))
            printf("syntax error");
        else
            tokens->size += 1;
    } else if (IS_TYPE(NodeType::num))
        val = Get_N(tokens);
    else if (IS_TYPE(NodeType::var))
        val = Get_V(tokens);
    else
        printf("get P error\n");

    return val;
}

TreeNode *Get_N(Tree_tokens *tokens) {
    if (IS_TYPE(NodeType::num))
        return tokens->array[tokens->size++];
    
    printf("get number error");

    return nullptr;
}

TreeNode *Get_V(Tree_tokens *tokens) {
    if (IS_TYPE(NodeType::var))
        return tokens->array[tokens->size++];
    printf("get variable error");

    return nullptr;
}

TreeNode *Get_W(Tree_tokens *tokens) {
    TreeNode *node_left = Get_F(tokens);

    if (IS_TYPE(NodeType::op) and IS_OP(Operator::PWR)) {
        tokens->size += 1;

        TreeNode *node_right = Get_F(tokens);

        return New_operator(Operator::PWR, node_left, node_right);
    }
    return node_left;
}

TreeNode *Get_F(Tree_tokens *tokens) {
    TreeNode *node_right = nullptr;

    if (IS_TYPE(NodeType::op) and (IS_UNARY())) {
        Operator temp_op = tokens->array[tokens->size]->op_value;
        tokens->size += 1;
        if (!(IS_TYPE(NodeType::op) and IS_OP(Operator::LBRC)))
            printf("syntax error");
        else
            tokens->size += 1;

        node_right = New_operator(temp_op, nullptr, Get_E(tokens));
        tokens->size += 1;
    } else
        node_right = Get_P(tokens);

    return node_right;
}