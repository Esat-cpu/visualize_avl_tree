#include <stdlib.h>


typedef struct Tree {
    int data;
    struct Tree* left;
    struct Tree* right;
} Tree;



Tree* init_tree_node (int data) {
    Tree* node = malloc(sizeof (Tree));
    node->left = NULL;
    node->right = NULL;
    node->data = data;
    return node;
}



static int height(Tree* node) {
    if (node == NULL) return -1;

    int left = height(node->left);
    int right = height(node->right);

    return ((left > right) ? left : right) + 1;
}



static int avl_value(Tree* node) {
    if (node == NULL) return 0;

    return height(node->left) - height(node->right);
}



static Tree* right_rotate(Tree* x) {
    Tree* y = x->left;
    Tree* T = y->right;

    y->right = x;
    x->left = T;
    return y;
}



static Tree* left_rotate(Tree* x) {
    Tree* y = x->right;
    Tree* T = y->left;

    y->left = x;
    x->right = T;
    return y;
}



static Tree*
restore_balance (Tree* root)
{
    int balance = avl_value(root);

    // Check if the left tree is unbalanced
    if (balance > 1) {
        // LL
        if (avl_value(root->left) >= 0) {
            return right_rotate(root);
        }
        // LR
        else {
            root->left = left_rotate(root->left);
            return right_rotate(root->left);
        }
    }

    // Check if the right tree is unbalanced
    else if (balance < -1) {
        // RR
        if (avl_value(root->right) < 0) {
            return left_rotate(root);
        }
        // RL
        else {
            root->right = right_rotate(root->right);
            return left_rotate(root);
        }
    }

    // Return root if node is balanced
    else {
        return root;
    }
}



Tree* avl_insert(Tree* root, Tree* new) {
    if (root == NULL) return new;

    if (root->data < new->data)
        root->left = avl_insert(root->left, new);
    else
        root->right = avl_insert(root->right, new);

    return restore_balance(root);
}



Tree* avl_remove(Tree* root, int data, int* found) {
    if (root == NULL) return NULL;

    if (root->data == data) {
        if (found != NULL) *found = 1;
        Tree* new_root;

        // Replace right subtree with deleted node if left subtree is empty
        if (root->left == NULL) {
            new_root = root->right;
        }

        // Replace largest node of left subtree with deleted node
        else {
            Tree* subtree = root->left;
            Tree* largest_node = subtree->right; // initialize largest_node

            if (largest_node == NULL) {
                new_root = subtree;
                new_root->right = root->right;
            }

            else {
                while (largest_node->right != NULL) {
                    largest_node = largest_node->right;
                    subtree = subtree->right;
                }

                subtree->right = largest_node->left;

                new_root = largest_node;
                new_root->right = root->right;
                new_root->left = root->left;
            }
        }

        free(root);
        return new_root;
    }

    else if (data < root->data)
        root->left = avl_remove(root->left, data, NULL);

    else
        root->right = avl_remove(root->right, data, NULL);


    return restore_balance(root);
}


