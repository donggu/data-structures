#ifndef AA_H
#define AA_H

typedef struct aa_node aa_node;

struct aa_node
{
    int level;
    int value;
    struct aa_node* left;
    struct aa_node* right;
};

aa_node* skew(aa_node* T);
aa_node* split(aa_node* T);
aa_node* insert(aa_node* T, int value);
aa_node* erase(aa_node* T, int value);
// Node* decrease_level(Node *T);

#endif