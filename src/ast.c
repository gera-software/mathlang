#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

ASTNode* create_int_node(int val) {
    ASTNode *node = malloc(sizeof(ASTNode));
    if(node == NULL) { return NULL; }

    node->type = NODE_INT;
    node->data.value = val;
    return node;
}

ASTNode* create_op_node(NodeType type, ASTNode *left, ASTNode *right) {
    ASTNode *node = malloc(sizeof(ASTNode));
    if(node == NULL) { return NULL; }

    node->type = type;
    node->data.op.left = left;
    node->data.op.right = right;
    return node;
}

void print_tabs(int level) {
    for(int i = 0; i < level; i++) {
        printf("\t");
    }
}

void print_ast(ASTNode *node, int level) {
    if(node == NULL) { return; }

    print_tabs(level);
    if(node->type == NODE_INT) {
        printf("%d\n", node->data.value);
    } else {
        printf("<%d>\n", node->type);
        print_ast(node->data.op.left, level + 1);
        print_ast(node->data.op.right, level + 1);
    }
}

void free_ast(ASTNode *node) {
    if(!node) return;

    if(node->type != NODE_INT) {
        free_ast(node->data.op.left);
        free_ast(node->data.op.right);
    }
    free(node);
}