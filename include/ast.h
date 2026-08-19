#pragma once

typedef enum {
    NODE_INT,
    NODE_ADD,
    NODE_SUB,
    NODE_MUL,
    NODE_DIV,
} NodeType;

typedef struct ASTNode {
    NodeType type;
    union {
        int value; // usado se node type = NODE_INT
        struct {
            struct ASTNode *left;
            struct ASTNode *right;
        } op; // Usado para operações binárias (+, -, *, /)
    } data;
} ASTNode;

ASTNode* create_int_node(int val);
ASTNode* create_op_node(NodeType type, ASTNode *left, ASTNode *right);
void free_ast(ASTNode *node);
void print_ast(ASTNode *node, int level);