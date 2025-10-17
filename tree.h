#ifndef BINARY_THREADED_TREE_H
#define BINARY_THREADED_TREE_H

#include "error_codes.h"
#include <stdbool.h>
#include <graphviz/gvc.h>

typedef enum { 
    RED = 0, 
    BLACK = 1,
} Color;

typedef struct TreeNode TreeNode;

typedef struct TreeNode {
    int key;
    char *value;
    Color color;
    TreeNode *left;
    TreeNode *right;
} TreeNode;

typedef struct Tree {
    TreeNode *root;
} Tree;

void insertWrapper(Tree *tree, int key, char *value);
bool deleteWrapper(Tree *tree, int key);
void print_tree(TreeNode *node, const char *prefix, bool is_left);
void tree_free(TreeNode *root);
TreeNode* find_lower_bound(Tree *tree, int key);
void inorder(TreeNode *root);
TreeNode* tree_search(Tree* tree, int key);
void add_to_gv(Agraph_t *graph, Agnode_t *gv_parent, TreeNode *node);
Err tree_load_from_text_file(const char *filename, Tree *tree);

#endif
