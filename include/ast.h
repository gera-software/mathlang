#pragma once

typedef enum {
    NODE_INT, // Número inteiro
    NODE_ADD, // Soma (A+B)
    NODE_SUB, // Subtração (A-B)
    NODE_MUL, // Multiplicação (A*B)
    NODE_DIV, // Divisão (A/B)
    NODE_NEG, // Menos Unário (-A)
} NodeType;

const char* get_node_type_name(NodeType type) {
    switch(type) {
        case NODE_INT: return "INT";
        case NODE_ADD: return "ADD";
        case NODE_SUB: return "SUB";
        case NODE_MUL: return "MUL";
        case NODE_DIV: return "DIV";
        case NODE_NEG: return "NEG";
        default: return "UNKNOWN";
    }
}

typedef struct ASTNode {
    NodeType type;
    union {
        int value; // usado se node type = NODE_INT
        struct {
            struct ASTNode *left;
            struct ASTNode *right;
        } op; // Usado para operações binárias (+, -, *, /)
        struct {
            struct ASTNode *operand;
        } unary; // Usado para operações unárias (NODE_NEG)
    } data;
} ASTNode;

ASTNode* create_int_node(int val);
ASTNode* create_binary_op_node(NodeType type, ASTNode *left, ASTNode *right);
ASTNode* create_unary_op_node(NodeType type, ASTNode *operand);
void free_ast(ASTNode *node);
void print_ast(ASTNode *node, int level);