#ifndef UI_TREE_H
#define UI_TREE_H

//#include <graphviz/gvc.h>
#include "error_codes.h"
#include "tree.h"

Err ui_tree_search(Tree *tree);
Err ui_find_lower_bound(Tree *tree);
Err ui_tree_remove(Tree *tree);
Err ui_tree_insert(Tree *tree);
void save_tree_as_dot(Tree *tree);
Tree* ui_tree_load_from_file();

#endif
