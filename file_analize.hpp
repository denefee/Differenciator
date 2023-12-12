#pragma once

#include "diff_tree.hpp"
#include <stdio.h>
#include <string>

const int max_cmd_size = 128;
const int keywords_number = 8;

struct String {
    char *string_point = nullptr; // pointer to a start of string
    size_t string_len = 0;     // len of string
} ;

struct Text {
    char *source = nullptr;         // pointers massive
    size_t count_of_symbols = 0; // count of symbols in source file
    size_t count_of_strings = 0; // count of strings in source file
    String *meta_string = nullptr;
};

enum Error : int {
    FALL = 0,            // abnormal program termination
    SUCCESS = 1,         // succes
    ERRORS = 2,          // typical error
    INPUT_ERROR = 3,     // incorrect comand line input
    OPEN_FILE_ERROR = 4, // file cannot open
    NO_MEM = 5,
};

void create_pointers(Text *text);
void counting_strings(Text *text);
void save_tree(const char *file_name, Root *tree_root);
void save_node(FILE *output_file, TreeNode *node, TreeNode *parent,
               Constants *f_constants);
void read_t_file(Text *text, const char *t_file_name, Root *tree_root,
                 Constants *constants);
void Tex_subtree(FILE *tex_file, Operator op_value, const TreeNode *node);
void Print_tex(FILE *tex_file, bool is_diff, const TreeNode *node,
               int diff_power = 1);
void Print_tex_keywords(FILE *tex_file);
void Print_tex_title(FILE *tex_file, Root *tree_root, Constants *f_constants);
void Print_tex_end(FILE *tex_file);

Error count_and_read(const char *file_name, Text *text);
