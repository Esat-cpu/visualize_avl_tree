#include <stdlib.h>


typedef struct Tree {
    int data;
    struct Tree* left;
    struct Tree* right;
} Tree;



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
            return right_rotate(root);
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

    if (new->data < root->data)
        root->left = avl_insert(root->left, new);
    else
        root->right = avl_insert(root->right, new);

    return restore_balance(root);
}



Tree* avl_remove(Tree* root, int data, int* found) {
    if (root == NULL) return NULL;

    if (root->data == data) {
        if (found != NULL) *found = 1;

        // Replace right subtree with deleted node if left subtree is empty
        if (root->left == NULL) {
            Tree* temp = root;
            root = root->right;
            free(temp);
        }

        // Replace root's value with the largest value from its left subtree
        // This block does not delete the root node
        else {
            Tree* largest_node = root->left;

            while (largest_node->right)
                largest_node = largest_node->right;

            root->data = largest_node->data;

            root->left = avl_remove(root->left, largest_node->data, NULL);
        }
    }

    else if (data < root->data)
        root->left = avl_remove(root->left, data, found);

    else
        root->right = avl_remove(root->right, data, found);


    return restore_balance(root);
}


