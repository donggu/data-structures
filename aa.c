#include <stdlib.h>
#include "aa.h"

aa_node* aa_skew(aa_node* T)
{
    if(!T) return 0;
    if(!T->left) return T;
    if(T->left->level != T->level) return T;

    aa_node* L = T->left;
    T->left = L->right;
    L->right = T;

    return L;
}

aa_node* aa_split(aa_node* T)
{
    if(!T) return 0;
    if(!T->right || !T->right->right) return T;
    if(T->level != T->right->right->level) return T;

    aa_node *R = T->right;
    T->right = R->left;
    R->left = T;
    R->level++;

    return R;
}

aa_node* aa_find(aa_node* T, int value)
{
    if(!T) return 0;

    if(value < T->value) return aa_find(T->left, value);
    if(value > T->value) return aa_find(T->right, value);
    return T;
}

aa_node* aa_insert(aa_node* T, int value)
{
    if(!T)
    {
        aa_node *node = (aa_node*) malloc( sizeof(aa_node) );
        node->level = 1;
        node->value = value;
        node->left = 0;
        node->right= 0;
        return node;
    }
    else if(value < T->value)
        T->left = aa_insert(T->left, value);
    else if(value > T->value)
        T->right = aa_insert(T->right, value);
    else
        return T;  // same element? do nothing.

    T = aa_skew(T);
    T = aa_split(T);
    return T;
}

aa_node* aa_remove(aa_node* T, int value)
{
    if(!T) return 0;
    if(value < T->value)
        T->left = aa_remove(T->left, value);
    else if(value > T->value)
        T->right = aa_remove(T->right, value);
    else
    {
        // leaf?
        if(!T->right) { return 0; }

        aa_node *L;

        if(!T->left)
        {
            // L = successor(T)
            for(L = T->right;
                L->left;
                L = L->left);
            T->right = erase(T->right, L->value);
            T->value = L->value;
        }
        else
        {
            // L = predecessor(T)
            for(L = T->left;
                L->right;
                L = L->right);
            T->left = erase(T->left, L->value);
            T->value = L->value;
        }
    }

    T = aa_decrease_level(T);
    T = aa_skew(T);

    if(T->right)
    {
        T->right = aa_skew(T->right);
        T->right->right = aa_skew(T->right->right); // nullable
    }

    T = aa_split(T);
    T->right = aa_split(T->right); // nullable
    return T;
}

aa_node* aa_decrease_level(aa_node *T) // T is non-null
{
    // int should_be = 1;
    // if(T->left && should_be < T->left->level+1) should_be = T->left->level+1;
    // if(T->right && should_be < T->right->level+1) should_be = T->right->level+1;

    if ((T->left  && T->left ->level+1 < T->level)
    ||  (T->right && T->right->level+1 < T->level)
    || ((!T->left || !T->right)  && 1 < T->level) )
    {
        T->level--;
        if(T->right && T->level < T->right->level)
            T->right->level = T->level;
    }
    return T;
}
