#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include "input.h"
#include "error_codes.h"
#include "tree.h"
#include <graphviz/gvc.h>

TreeNode* create_node(int key, char *value) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (!node) {
        return NULL;
    }
    node->key = key;
    node->value = strdup(value);
    if (!node->value) {
        return NULL;
    }
    node->color = RED;
    node->left = node->right = NULL;
    return node;
}

bool isRed(TreeNode* node) {
    return node != NULL && node->color == RED;
}

TreeNode* rotateLeft(TreeNode* old_parent) {
    if (old_parent == NULL || old_parent->right == NULL) {
        return old_parent;
    }
    TreeNode* new_parent = old_parent->right;
    old_parent->right = new_parent->left;
    new_parent->left = old_parent;
    new_parent->color = old_parent->color;
    old_parent->color = RED;
    return new_parent;
}

TreeNode* rotateRight(TreeNode* old_parent) {
    if (old_parent == NULL || old_parent->left == NULL) {
        return old_parent;
    }
    TreeNode* new_parent = old_parent->left;
    old_parent->left = new_parent->right;
    new_parent->right = old_parent;
    new_parent->color = old_parent->color;
    old_parent->color = RED;
    return new_parent;
}

void flipColors(TreeNode* h) {
    if (h == NULL) return;

    if (h->color == RED) {
        h->color = BLACK;
    } else {
        h->color = RED;
    }
    if (h->left->color == RED) {
        h->left->color = BLACK;
    } else {
        h->left->color = RED;
    }
    if (h->right->color == RED) {
        h->right->color = BLACK;
    } else {
        h->right->color = RED;
    }
}

TreeNode* moveRedLeft(TreeNode* h) {
    if (h == NULL || h->right == NULL) return h;
    
    flipColors(h);
    if (isRed(h->right->left)) {
        h->right = rotateRight(h->right);
        h = rotateLeft(h);
        flipColors(h);
    }
    return h;
}

TreeNode* moveRedRight(TreeNode* h) {
    if (h == NULL || h->left == NULL) return h;

    flipColors(h);
    if (isRed(h->left->left)) {
        h = rotateRight(h);
        flipColors(h);
    }
    return h;
}

TreeNode* fixUp(TreeNode* h) {
    if (h == NULL) return NULL;

    if (isRed(h->right))
        h = rotateLeft(h);
    
    if (isRed(h->left) && isRed(h->left->left))
        h = rotateRight(h);
    
    if (isRed(h->left) && isRed(h->right))
        flipColors(h);
    
    return h;
}

TreeNode* minNode(TreeNode* h) {
    if (h == NULL) return NULL;

    while (h->left != NULL)
        h = h->left;
    return h;
}

TreeNode* deleteMin(TreeNode* h) {
    if (h == NULL) return NULL;

    if (h->left == NULL) {
        if (h->value != NULL) {
            free(h->value);
        }
        free(h);
        return NULL;    
    }
    
    if (h->left != NULL && !isRed(h->left) && 
        h->left->left != NULL && !isRed(h->left->left)) {
        h = moveRedLeft(h);
    }

    h->left = deleteMin(h->left);
    return fixUp(h);
}

TreeNode* delete(TreeNode* h, int key, bool* deleted) {
    if (h == NULL) {
        *deleted = false;
        return NULL;
    }

    if (key < h->key) {
        if (!isRed(h->left) && h->left != NULL && !isRed(h->left->left))
            h = moveRedLeft(h);
        h->left = delete(h->left, key, deleted);
    } 
    else {
        if (isRed(h->left))
            h = rotateRight(h);
            
        if (key == h->key && h->right == NULL) {
            *deleted = true; 
            free(h->value);
            free(h);
            return NULL;
        }
        
        if (!isRed(h->right) && h->right != NULL && !isRed(h->right->left))
            h = moveRedRight(h);
            
        if (key == h->key) {
            *deleted = true; 
            TreeNode* min = minNode(h->right);
            h->key = min->key;
            h->right = deleteMin(h->right);
        } 
        else {
            h->right = delete(h->right, key, deleted);
        }
    }
    return fixUp(h);
}

bool deleteWrapper(Tree *tree, int key) {
    if (!tree || !tree->root) return false; 
    
    bool deleted = false;
    tree->root = delete(tree->root, key, &deleted); 
    if (tree->root != NULL)
        tree->root->color = BLACK;
    
    return deleted;
}

TreeNode* insert(TreeNode* node, int key, char *value) {
    if (node == NULL) {
        return create_node(key, value);
    }
    
    if (key < node->key) {
        node->left = insert(node->left, key, value);
    } else if (key > node->key) {
        node->right = insert(node->right, key, value);
    } else {
        free(node->value);
        node->value = strdup(value);
        return node;
    }
    
    if (isRed(node->right) && !isRed(node->left)) {
        node = rotateLeft(node);
    }
    
    if (isRed(node->left) && isRed(node->left->left)) {
        node = rotateRight(node);
    }
    
    if (isRed(node->left) && isRed(node->right)) {
        flipColors(node);
    }
    return node;
}

void insertWrapper(Tree* tree, int key, char *value) {
    tree->root = insert(tree->root, key, value);
    tree->root->color = BLACK;
}

void tree_free(TreeNode* node) {
    if (node == NULL) return;
    
    tree_free(node->left);
    tree_free(node->right);
    
    free(node->value);
    free(node);
}

void print_tree(TreeNode *node, const char *prefix, bool is_left) {
    if (node == NULL) return;

    printf("%s", prefix);

    if (is_left) {
        printf("├──");
    } else {
        printf("└──");
    }

    if (node->color == RED) {
        printf("%d,R\n", node->key);
    } else {
        printf("%d,B\n", node->key);
    }

    char new_prefix[256];

    if (is_left) {
        strcpy(new_prefix, prefix);
        strcat(new_prefix, "|   ");
    } else {
        strcpy(new_prefix, prefix);
        strcat(new_prefix, "    ");
    }
    if (node->left != NULL) {
        print_tree(node->left, new_prefix, true);
    }

    if (node->right != NULL) {
        print_tree(node->right, new_prefix, false);
    }
}

TreeNode *tree_search(Tree *tree, int key) {
    if (tree == NULL || tree->root == NULL) return NULL;

    TreeNode *current = tree->root;
    int count = 0;

    while (current) {
        if (current->key < key) {
            current = current->right;
        }
        else if (current->key > key) {
            current = current->left;
        }
        else {
            return current;
        }
    }
    return NULL;
}

void inorder(TreeNode *root) {
    if (!root) return;

    inorder(root->right);

    char color_char = 'B';
    if (root->color == RED) {
        color_char = 'R';
    }
    printf("%d(%c) ", root->key, color_char);

    inorder(root->left);
}

TreeNode* find_lower_bound(Tree* tree, int key) {
    if (!tree || !tree->root) return NULL;
    TreeNode* result = NULL;
    TreeNode* current = tree->root;

    while (current != NULL) {
        if (current->key == key) {
            return current;
        }

        if (current->key < key) {
            result = current;
            current = current->right;
        } else {
            current = current->left;
        }
    }
    return result;
}

void add_to_gv(Agraph_t *graph, Agnode_t *gv_parent, TreeNode *node) {
    if (!node) return;
    
    char name[64];
    sprintf(name, "%d_%p", node->key, (void *)node);
    Agnode_t *gv_node = agnode(graph, name, 1);

    char label[128];
    if (node->value) {
        sprintf(label, "%d: %s", node->key, node->value);
    } else {
        sprintf(label, "%d", node->key);
    }
    agsafeset(gv_node, "label", label, "");

    if (node->color == RED) {
        agsafeset(gv_node, "color", "red", "");
        agsafeset(gv_node, "fontcolor", "red", "");
        agsafeset(gv_node, "fillcolor", "#FFDDDD", "");
        agsafeset(gv_node, "style", "filled,rounded", "");
    } else {
        agsafeset(gv_node, "color", "black", "");
        agsafeset(gv_node, "fontcolor", "black", "");
        agsafeset(gv_node, "fillcolor", "#DDDDDD", "");
        agsafeset(gv_node, "style", "filled,rounded", "");
    }

    if (gv_parent) {
        Agedge_t *edge = agedge(graph, gv_parent, gv_node, NULL, 1);
        agsafeset(edge, "color", node->color == RED ? "red" : "black", "");
        agsafeset(edge, "penwidth", "1.5", "");
    }

    add_to_gv(graph, gv_node, node->left);
    add_to_gv(graph, gv_node, node->right);
}

char *read_line_from_file(FILE *file) {
    char buf[81] = {0};
    char *res = NULL;
    int len = 0;
    int n = 0;

    do {
        n = fscanf(file, "%80[^\n]", buf);
        if (n == EOF) {
            if (!res) return NULL;
            break;
        }
        if (n < 0) {
            if (!res) return NULL;
        } else if (n > 0) {
            int chunk_len = strlen(buf);
            char *new_res = realloc(res, len + chunk_len + 1);
            if (!new_res) {
                free(res);
                return NULL;
            }
            res = new_res;
            strcpy(res + len, buf);
            len += chunk_len;
        } else {
            fscanf(file, "%*c");
        }
    } while (n > 0);

    if (len > 0) {
        res[len] = '\0';
    } else {
        res = calloc(1, sizeof(char));
    }
    return res;
}

Err tree_load_from_text_file(const char *filename, Tree *tree) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: failed to open file '%s'\n", filename);
        return ERR_FILE;
    }

    while (1) {
        char *key_str = read_line_from_file(file);
        if (!key_str) {
            break;
        }

        char *endptr1;
        int key = strtoul(key_str, &endptr1, 10);

        if (*endptr1 != '\0') {
            printf("Error: invalid key '%s'\n", key_str);
            free(key_str);
            fclose(file);
            return ERR_PARSE;
        }
        free(key_str);

        char *value_str = read_line_from_file(file);
        if (!value_str) {
            printf("Error reading value for key %d\n", key);
            fclose(file);
            return ERR_READ;
        }
        insertWrapper(tree, key, value_str);
        free(value_str);
    }
    fclose(file);
    return ERR_OK;
}
