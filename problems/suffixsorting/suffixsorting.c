#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    int num_queries;
    int str_len;
    int *queries;
    char *str;
} input_sample;

input_sample sample;

typedef struct tree_node {
    struct tree_node *parent;
    struct tree_node *left;
    struct tree_node *right;
    char *suffix;
    int suffix_pos;
    int size;
    char red;
} tree_node;

typedef struct rb_tree {
    tree_node *root;
    tree_node *nil;
} rb_tree;

int read_input()
{
    // Make room for null character
    sample.str = malloc(100001 * sizeof(*sample.str));
    char ch;
    int i = 0;
    while (ch = getchar())
    {
        if (ch == '\n')
        {
            sample.str[i] = '\0';
            break;
        }
        if (ch == EOF)
        {
            return 0;
        }
        sample.str[i] = ch;
        i++;
    }

    sample.str_len = strlen(sample.str);

    scanf("%i ", &sample.num_queries);
    sample.queries = malloc(sample.num_queries * sizeof(*sample.queries));

    for (i = 0; i < sample.num_queries; i++)
    {
        int query;
        char c;
        scanf("%i%c", &query, &c);
        sample.queries[i] = query;
    }

    return 1;
}

tree_node *create_tree_node(char *suffix)
{
    tree_node *node = malloc(sizeof(*node));
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    node->suffix = suffix;
    node->suffix_pos = 0;
    node->size = 0;
    node->red = 0;

    return node;
}

void free_tree_node(rb_tree *tree, tree_node *node)
{
    if (node->left != tree->nil)
        free_tree_node(tree, node->left);
    if (node->right != tree->nil)
        free_tree_node(tree, node->right);
    free(node);
}

rb_tree *create_suffix_tree()
{
    rb_tree *tree = malloc(sizeof(*tree));
    tree->nil = create_tree_node(NULL);
    tree->root = tree->nil;

    return tree;
}

void free_suffix_tree(rb_tree *tree)
{
    if (tree->root)
        free_tree_node(tree, tree->root);
    free(tree->nil);
    free(tree);
}

void suffix_tree_left_rotate(rb_tree *tree, tree_node *node)
{
    tree_node *y = node->right;
    node->right = y->left;
    if (y->left != tree->nil)
        y->left->parent = node;
    y->parent = node->parent;
    if (node->parent == tree->nil)
        tree->root = y;
    else if (node->parent->right == node)
        node->parent->right = y;
    else
        node->parent->left = y;
    node->parent = y;
    y->left = node;

    y->size = node->size;
    node->size = node->left->size + node->right->size + 1;
}

void suffix_tree_right_rotate(rb_tree *tree, tree_node *node)
{
    tree_node *y = node->left;
    node->left = y->right;
    if (y->right != tree->nil)
        y->right->parent = node;
    y->parent = node->parent;
    if (node->parent == tree->nil)
        tree->root = y;
    else if (node->parent->right == node)
        node->parent->right = y;
    else
        node->parent->left = y;
    node->parent = y;
    y->right = node;

    y->size = node->size;
    node->size = node->left->size + node->right->size + 1;
}

void suffix_tree_insert_fixup(rb_tree *tree, tree_node *node)
{
    while (node->parent->red)
    {
        tree_node *x = node->parent->parent; // Grandfather
        if (node->parent == x->left) // Parent is a left node
        {
            if (x->right->red) // Uncle is red
            {
                node->parent->red = 0;
                x->right->red = 0;
                x->red = 1;
                node = x;
            }
            else
            {
                if (node->parent->right == node)
                {
                    node = node->parent;
                    suffix_tree_left_rotate(tree, node);
                }
                node->parent->red = 0;
                x->red = 1;
                suffix_tree_right_rotate(tree, x);
            }
        }
        else // Parent is a right node
        {
            if (x->left->red)
            {
                node->parent->red = 0;
                x->left->red = 0;
                x->red = 1;
                node = x;
            }
            else
            {
                if (node->parent->left == node)
                {
                    node = node->parent;
                    suffix_tree_right_rotate(tree, node);
                }
                node->parent->red = 0;
                x->red = 1;
                suffix_tree_left_rotate(tree, x);
            }
        }
    }
    tree->root->red = 0;
}

void suffix_tree_insert(rb_tree *tree, tree_node *node)
{
    tree_node *x = tree->root;
    tree_node *y = tree->nil;
    while (x != tree->nil)
    {
        y = x;
        x->size++;
        if (strcmp(node->suffix, x->suffix) < 0)
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }
    }

    node->parent = y;
    if (y == tree->nil)
    {
        tree->root = node;
    }
    else if (strcmp(node->suffix, y->suffix) < 0)
    {
        y->left = node;
    }
    else
    {
        y->right = node;
    }
    node->left = tree->nil;
    node->right = tree->nil;
    node->size = 1;
    node->red = 1;
    suffix_tree_insert_fixup(tree, node);
}

tree_node *suffix_tree_select(tree_node *node, int rank)
{
    int r = node->left->size;
    if (rank == r)
    {
        return node;
    }
    else if (rank < r)
    {
        return suffix_tree_select(node->left, rank);
    }
    else
    {
        return suffix_tree_select(node->right, rank - r - 1);
    }
}

int main()
{
    sample.str = NULL;
    sample.queries = NULL;
    while (read_input())
    {
        rb_tree *tree = create_suffix_tree();

        for (int i = 0; i < sample.str_len; i++)
        {
            tree_node *node = create_tree_node(sample.str + i);
            node->suffix_pos = i;
            suffix_tree_insert(tree, node);
        }

        for (int i = 0; i < sample.num_queries; i++)
        {
            int rank = sample.queries[i];
            tree_node *node = suffix_tree_select(tree->root, rank);
            if (i > 0)
                printf(" ");
            printf("%i", node->suffix_pos);
        }

        printf("\n");

        free_suffix_tree(tree);
        free(sample.str);
        free(sample.queries);
    }

    return 0;
}
