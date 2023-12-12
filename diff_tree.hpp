#pragma once
#include <stdio.h>

enum class NodeType {
    var = 0,
    num = 1,
    op = 2,
};

enum class Operator {
    NON = 0,
    ADD = 1,
    SUB = 2,
    MUL = 3,
    DIV = 4,
    SIN = 5,
    COS = 6,
    PWR = 7,
    LN = 8,
    TAN = 9,
    COT = 10,
    LOG = 11,
    LBRC = 11,
    RBRC = 12,
    END = 13,
};

using Elem_t = const char *;

struct Constant {
    char *var_name = nullptr;
    double var_value = 0;
};

struct Constants {
    size_t capacity = 0;
    Constant *list = nullptr;
};

struct TreeNode {
    NodeType type;

    const char *var_value;
    double num_value;
    Operator op_value;

    size_t str_position = 0;
    size_t str_number = 0;

    TreeNode *left = nullptr;
    TreeNode *right = nullptr;
};

struct Root {
    size_t number_of_elem = 0;
    Constants *f_constants = nullptr;
    TreeNode *first_node = nullptr;
} ;

union value {
    double num;
    const char *var;
    Operator op;
};

TreeNode *New_num(double number, size_t str_pos = 0, size_t str_num = 0);
TreeNode *New_var(const char *variable);
TreeNode *New_operator(Operator op_value, TreeNode *left, TreeNode *right);

TreeNode *Find_node_by_value(TreeNode *parent, Elem_t value);

void Print_node(const TreeNode *node);
void Free_tree(TreeNode *node);
void Graph_print_tree(Root *tree_root);
