#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tree.h"
#include "timing.h"
#include <stdbool.h>

int generate_random_key(unsigned int max) {
    return rand() % max + 1;
}

char* generate_random_value() {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    char *res = malloc(sizeof(char) * 10);
    if (!res) {
        return NULL;
    }
    for (int i = 0; i < 9; ++i) {
        int index = rand() % (sizeof(charset) - 1);
        res[i] = charset[index];
    }
    res[9] = '\0';
    return res;
}

Tree* generate_tree(int n) {
    Tree *tree = (Tree *)malloc(sizeof(Tree));
    if (!tree) {
        printf("Mem err\n");
        return NULL;
    }
    tree->root = NULL;

    for (int i = 0; i <= n; i++) {
        int key = generate_random_key(n * 2);
        char *value = generate_random_value();
        if (!value) {
            tree_free(tree->root);
            free(tree);
            return NULL;
        }
        insertWrapper(tree, key, value);
        free(value);
    }
    return tree;
}

Err time_tree_insert(FILE *file) {
    int n = 10000;
    int m = 1000;

    int step = 25000;
    int max_el = 250000;

    for (int i = n; i <= max_el; i+=step) {
        double total_time = 0;
        for (int x = 0; x < 100; x++) {
            Tree *tree = generate_tree(i);
            if (!tree) {
                printf("Err generate tree\n");
                return ERR_GENERATE_TREE;
            }

            KeyValuePair* nodes = malloc(sizeof(KeyValuePair) * m);
            if (!nodes) {
                tree_free(tree->root);
                free(tree);
                printf("Err alloc nodes\n");
                return ERR_MEM;
            }

            for (int k = 0; k < m; k++) {
                nodes[k].key = generate_random_key(i * 2);
                nodes[k].value = generate_random_value();

                if (!nodes[k].value) {
                    tree_free(tree->root);
                    free(tree);
                    free(nodes);
                    return ERR_MEM;
                }
            }
            
            clock_t start = clock();
            for (int j = 0; j < m; j++) {
                insertWrapper(tree, nodes[j].key, nodes[j].value);
            }
            clock_t end = clock();

            total_time += (double)(end - start) / CLOCKS_PER_SEC;
            
            for (int s = 0; s < m; s++) {
                free(nodes[s].value);
            }
            free(nodes);
            tree_free(tree->root);
            free(tree);
        }
        fprintf(file, "%d\t%f\n", i, total_time /100);
    }
    return ERR_OK;
}

Err time_tree_search(FILE *file) {
    int n = 10000;
    int m = 1000;

    int step = 25000;
    int max_el = 250000;

    for (int i = n; i <= max_el; i += step) {
        double total_time = 0;
        for(int x = 0; x < 100; x++) {
            Tree *tree = generate_tree(i);
            if (!tree) {
                printf("Err generate tree\n");
                return ERR_GENERATE_TREE;
            }

            int *keys = malloc(sizeof(int) * m);
            if (!keys) {
                tree_free(tree->root);
                free(tree);
                printf("Err alloc keys\n");
                return ERR_MEM;
            }

            for (int k = 0; k < m; k++) {
                int key = generate_random_key(i * 2);
                char *value = generate_random_value();

                if (!value) {
                    tree_free(tree->root);
                    free(tree);
                    free(keys);
                    return ERR_MEM;
                }

                insertWrapper(tree, key, value);

                free(value);
                keys[k] = key;
            }

            clock_t start = clock();
            for (int j = 0; j < m; j++) {
                TreeNode *node = tree_search(tree, keys[j]);
            }
            clock_t end = clock();

            total_time += (double)(end - start) / CLOCKS_PER_SEC;
            
            tree_free(tree->root);
            free(tree);
            free(keys);
        }
        fprintf(file, "%d\t%f\n", i, total_time / 100);
    }
    return ERR_OK;
}

Err time_tree_remove(FILE *file) {
    int n = 10000;
    int m = 1000;

    int step = 25000;
    int max_el = 250000;

    for (int i = n; i <= max_el; i += step) {
        double total_time = 0;
        for (int x = 0; x < 100; x++) {
            Tree *tree = generate_tree(i);
            if (!tree) {
                printf("Err generate tree\n");
                return ERR_GENERATE_TREE;
            }

            int *keys = malloc(sizeof(int) * m);
            if (!keys) {
                tree_free(tree->root);
                free(tree);
                printf("Err alloc keys\n");
                return ERR_MEM;
            }

            for (int k = 0; k < m; k++) {
                int key = generate_random_key((i + m) * 2);
                char *value = generate_random_value();

                if (!value) {
                    tree_free(tree->root);
                    free(tree);
                    free(keys);
                    return ERR_MEM;
                }

                insertWrapper(tree, key, value);

                free(value);
                keys[k] = key;
            }

            clock_t start = clock();
            for (int j = 0; j < m; j++) {
                deleteWrapper(tree, keys[j]);
            }
            clock_t end = clock();

            total_time += (double)(end - start) / CLOCKS_PER_SEC;

            tree_free(tree->root);
            free(tree);
            free(keys);
        }
        fprintf(file, "%d\t%f\n", i, total_time / 100);
    }
    return ERR_OK;
}

Err timing_tree() {
    srand(time(NULL)); 

    FILE *insert_file = fopen("insert_times.txt", "w");
    FILE *search_file = fopen("search_times.txt", "w");
    FILE *remove_file = fopen("remove_times.txt", "w");
    
    if (!insert_file || !search_file || !remove_file) {
        printf("Failed to open output files\n");
        return ERR_FILE;
    }
    fprintf(insert_file, "Elements\tTime(us)\n");
    fprintf(search_file, "Elements\tTime(us)\n");
    fprintf(remove_file, "Elements\tTime(us)\n");

    Err err = time_tree_insert(insert_file);
    if (err) {
        printf("Error time tree insert\n");
        fclose(insert_file);
        fclose(search_file);
        fclose(remove_file);
        return err;
    }
    err = time_tree_search(search_file);
    if (err) {
        printf("Error time tree search\n");
        fclose(insert_file);
        fclose(search_file);
        fclose(remove_file);
        return err;
    }
    err = time_tree_remove(remove_file);
    if (err) {
        printf("Error time tree remove\n");
        fclose(insert_file);
        fclose(search_file);
        fclose(remove_file);
        return err;
    }

    fclose(insert_file);
    fclose(search_file);
    fclose(remove_file);
    
    printf("Timing completed. Results saved to:\n");
    printf("- insert_times.txt\n- search_times.txt\n- remove_times.txt\n");
    
    return ERR_OK;
}

