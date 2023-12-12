#pragma once

#include "diff_dsl.hpp"
#include "diff_tree.hpp"

enum CmdString : int {
    derivative_n_str = 1,
    func_calc_str = 2,
    derivative_calc_str = 3,
    taylor_series_str = 4,
    tangent_point_str = 5,
    graph_range_str = 6,
};

TreeNode *Diff(const TreeNode *node, bool is_dump_to_tex,
                const char *diff_var = "x");
TreeNode *Copy(TreeNode *node);
TreeNode *Simpler(TreeNode *node);

TreeNode *Constant_simplification(TreeNode *node, bool *is_modifed);

double Calculate_func(TreeNode *node, double x_value, Constants *f_constants);
double Find_const_by_name(const char *var_name, Constants *f_constants);
