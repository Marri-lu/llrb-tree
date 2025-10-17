#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "error_codes.h"
#include "input.h"
#include <readline/readline.h>
#include <graphviz/gvc.h>

Err ui_tree_insert(Tree *tree) {
    int key;
    printf("Введите ключ который нужно вставить: ");
    Err err = input_int(&key);
    
    if (err) {
        return ERR_EOF;
    }
    char *value = readline("Input elem value to insert: ");
    if (value == NULL) {
        return ERR_EOF;
    }
    insertWrapper(tree, key, value);
    free(value);

    return ERR_OK;
}

Err ui_tree_remove(Tree *tree) {
    int key;
    printf("Введите ключ который нужно удалить: ");
    Err err = input_int(&key);

    if (err) {
        return ERR_EOF;
    }

    bool deleted = deleteWrapper(tree, key);
    if (!deleted) {
        printf("No element\n");
        return ERR_NO_NODE;
    }
    printf("Delete node\n");
    return ERR_OK;
}

Err ui_find_lower_bound(Tree *tree) {
    printf("Введите ключь по которому нужно будет найти верхнюю границу: ");
    int key;
    Err err = input_int(&key);

    if (err) {
        return ERR_EOF;
    }
    TreeNode *bound = find_lower_bound(tree, key);
    if(!bound) {
        printf("Ничего не нашлось\n");
        return ERR_EMPTY_TREE;
    }
    
    if (bound->color == RED) {
        printf("Нашли элемент. ключь: %d, значение: %s, цвет: red\n", bound->key, bound->value);
    } else {
        printf("Нашли элемент. ключь: %d, значение: %s, цвет: blak\n", bound->key, bound->value);
    }
    return ERR_OK;
}

Err ui_tree_search(Tree *tree) {
    int key;
    printf("Введите ключ по которому будем искать: ");
    Err err = input_int(&key);
    
    if (err) {
        return ERR_EOF;
    }

    TreeNode *node = tree_search(tree, key);

    if (!node) {
        printf("Ничего не найдено\n");
        return ERR_NO_NODE;
    }
    printf("Найден элемент. Key: %d, value: %s\n", node->key, node->value);
    return ERR_OK;
}

void save_tree_as_dot(Tree *tree) {

    Agraph_t *graph = agopen("LLRB", Agundirected, NULL);
    add_to_gv(graph, NULL, tree->root);

    GVC_t *gvc = gvContext();
    gvLayout(gvc, graph, "dot");
    gvRenderFilename(gvc, graph, "png", "llrb.png");

    gvFreeLayout(gvc, graph);
    agclose(graph);
    gvFreeContext(gvc);
}

Tree* ui_tree_load_from_file() {
    Tree *new_tree = (Tree *)malloc(sizeof(Tree));
    new_tree->root = NULL;

    if (!new_tree) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    char *filename = readline("Input filename: ");
    if (!filename) {
        free(new_tree);
        printf("Memory allocation failed\n");
        return NULL;
    }

    Err err = tree_load_from_text_file(filename, new_tree);
    free(filename);

    if (err == ERR_OK) {
        printf("Tree successfully loaded from file\n");
        return new_tree;
    } else {
        printf("Error loading tree from file\n");
        tree_free(new_tree->root);
        free(new_tree);
        return NULL;
    }
}

