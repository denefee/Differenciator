#pragma once

#include "diff_tree.hpp"

#define IS_TYPE(type_val) tokens->array[tokens->size]->type == type_val
#define IS_OP(op_val)     tokens->array[tokens->size]->op_value == op_val
#define IS_UNARY()        IS_OP (Operator::SIN) or IS_OP (Operator::COS) or IS_OP (Operator::LN) or IS_OP (Operator::TAN)

struct Tree_tokens {
    size_t size = 0;
    size_t capacity = 0;
    TreeNode **array = nullptr;
    TreeNode *current = nullptr;
};

TreeNode* Get_G (Tree_tokens *tokens);
TreeNode* Get_E (Tree_tokens *tokens);
TreeNode* Get_T (Tree_tokens *tokens);
TreeNode* Get_P (Tree_tokens *tokens);
TreeNode* Get_N (Tree_tokens *tokens);
TreeNode* Get_W (Tree_tokens *tokens);
TreeNode* Get_V (Tree_tokens *tokens);
TreeNode* Get_F (Tree_tokens *tokens);

Tree_tokens *Tokenizer (const char **str);
TreeNode *Is_num      (const char **str);
TreeNode *Is_variable (const char **str);
TreeNode *Is_operator (const char **str);
