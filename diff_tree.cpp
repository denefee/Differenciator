#include "diff_tree.hpp"
#include "utilities.hpp"
#include <assert.h>
#include <malloc.h>
#include <math.h>
#include <string.h>

TreeNode *New_num(double number, size_t str_pos, size_t str_num) {
    TreeNode *new_node = new TreeNode{NodeType::num, nullptr, number,
                                      Operator::NON, str_pos, str_num};
    return new_node;
}

TreeNode *New_var(const char *variable) {
    assert(variable);

    TreeNode *new_node =
        new TreeNode{NodeType::var, variable, NAN, Operator::NON};
    return new_node;
}

TreeNode *New_operator(Operator op_value, TreeNode *left_son,
                       TreeNode *right_son) {

    TreeNode *new_node = new TreeNode{
        NodeType::op, nullptr, NAN, op_value, 0, 0, left_son, right_son};
    return new_node;
}

static const char *convert_graph_op(Operator op_value) {
    if (op_value == Operator::ADD)
        return " + ";
    if (op_value == Operator::SUB)
        return "-";
    if (op_value == Operator::MUL)
        return " * ";
    if (op_value == Operator::DIV)
        return " \\\\ ";
    if (op_value == Operator::SIN)
        return "sin";
    if (op_value == Operator::COS)
        return "cos";
    if (op_value == Operator::PWR)
        return "^";
    if (op_value == Operator::LN)
        return "ln";
    if (op_value == Operator::TAN)
        return "tg";
    if (op_value == Operator::COT)
        return "ctg";

    return "non operator";
}

static void Graph_print_node(FILE *graph_file, TreeNode *parent,
                             TreeNode *son) {
    assert(graph_file);

    if (son) {
        if (parent) {
            if (parent->left == son)
                fprintf(graph_file, "    node%p [label = \"op: %s L\"]\n",
                        parent, convert_graph_op(parent->op_value));
            else
                fprintf(graph_file, "    node%p [label = \"op: %s R\"]\n",
                        parent, convert_graph_op(parent->op_value));

            if (son->type == NodeType::num)
                fprintf(graph_file, "    node%p [label = \"num: %lg\"]\n", son,
                        son->num_value);
            else if (son->type == NodeType::op)
                fprintf(graph_file, "    node%p [label = \"op: %s\"]\n", son,
                        convert_graph_op(son->op_value));
            else
                fprintf(graph_file, "    node%p [label = \"var: %s\"]\n", son,
                        son->var_value);

            fprintf(graph_file, "    node%p -> node%p\n", parent, son);
        }
        Graph_print_node(graph_file, son, son->left);
        Graph_print_node(graph_file, son, son->right);
    }
}

void Graph_print_tree(Root *tree_root) {
    assert(tree_root);

    FILE *graph_file = fopen("out/input.gv", "wb");

    if (graph_file == nullptr)
        printf("open %s error", "out/input.dot");

    fprintf(graph_file, "digraph {\n");
    fprintf(graph_file, "    labelloc = \"t\"\n");
    fprintf(graph_file, "    fontsize = \"70\"\n");
    fprintf(graph_file, "    nodesep = 0.4\n");
    fprintf(graph_file, "    node [shape = record]\n");
    fprintf(graph_file, "    splines=ortho\n    rankdir=TB\n");

    Graph_print_node(graph_file, nullptr, tree_root->first_node);

    fprintf(graph_file, "}");

    if (fclose(graph_file) != 0)
        printf("graph file close error");

    system("dot out/input.gv -Tpng -o out/output.png");
}

void Free_tree(TreeNode *node) {
    if (node) {
        if (node->left)
            Free_tree(node->left);

        if (node->right)
            Free_tree(node->right);

        free(node);
    }
}