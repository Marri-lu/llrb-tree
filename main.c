#include <stdio.h>
#include <stdlib.h>
#include "error_codes.h"
#include "ui_tree.h"
#include "input.h"
#include <readline/readline.h>
#include "timing.h"

void print_menu() {
    printf("\n--- LLRB Tree Operations ---\n");
    printf("1. Insert element\n");
    printf("2. Remove element\n");
    printf("3. Search element\n");
    printf("4. Find lower bound\n");
    printf("5. Inorder traversal not in range\n");
    printf("6. Save tree visualization (PNG)\n");
    printf("7. Print tree\n");
    printf("8. Load tree from txt file\n");
    printf("9. Timing tree\n");
    printf("10. Exit\n");
    printf("Enter your choice: ");
}

Err main() {
    Tree *tree = (Tree *)malloc(sizeof(Tree));

    if (!tree) {
        printf("Memory allocation failed\n");
        return ERR_MEM;
    }
    tree->root = NULL;
    int choice = 0;
    Err err = ERR_OK;

    while (err != ERR_EOF && choice != 10) {
        print_menu();

        if (input_int(&choice) != 0) {
            printf("Bue\n");
            tree_free(tree->root);
            free(tree);
            return ERR_EOF;
        }
        switch (choice) {
            case 1:
                err = ui_tree_insert(tree);
                break;
            case 2:
                err = ui_tree_remove(tree);
                break;
            case 3:
                err = ui_tree_search(tree);
                break;
            case 4:
                err = ui_find_lower_bound(tree);
                break;
            case 5:
                inorder(tree->root);
                break;
            case 6:
                save_tree_as_dot(tree);
                printf("Tree visualization saved as 'llrb.png'\n");
                break;
            case 7:
                print_tree(tree->root, "", false);
                break;
            case 8:
                Tree *new_tree = ui_tree_load_from_file(tree);
                if (new_tree) {
                    tree_free(tree->root);
                    free(tree);
                    tree = new_tree;
                }
                break;
            case 9:
                printf("Timing...\n");
                err = timing_tree();
                break;
            case 10:
                printf("Exiting program...\n");
                tree_free(tree->root);
                free(tree);
                return ERR_OK;
            default:
                printf("Invalid choice. Please try again.\n");
        }

        if (err == ERR_EOF) {
            tree_free(tree->root);
            free(tree);
            return ERR_EOF;
       }
    }
}
