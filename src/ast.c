#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "arena.h"
#include "str_buf.h"

ASTNode* create_int_node(Arena *arena, int val) {
    ASTNode *node = arena_push(arena, sizeof(ASTNode));
    if(node == NULL) { return NULL; }

    node->type = NODE_INT;
    node->data.value = val;
    return node;
}

ASTNode* create_binary_op_node(Arena *arena, NodeType type, ASTNode *left, ASTNode *right) {
    ASTNode *node = arena_push(arena, sizeof(ASTNode));
    if(node == NULL) { return NULL; }

    node->type = type;
    node->data.op.left = left;
    node->data.op.right = right;
    return node;
}

ASTNode* create_unary_op_node(Arena *arena, NodeType type, ASTNode *operand) {
    ASTNode* node = arena_push(arena, sizeof(ASTNode));
    if(node == NULL) { return NULL; }

    node->type = type;
    node->data.unary.operand = operand;
    return node;
}

void print_tabs(StrBuf *string_buf, int level) {
    // sb_append_int(string_buf, level);
    for(int i = 0; i < level; i++) {
        sb_append_char(string_buf, '\t');
        // printf("\t");
    }
}

// FIX pode retornar string de uma função sem alocação dinamica da string na heap?
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

void print_ast(StrBuf *string_buf, ASTNode *node, int level) {
    if(node == NULL) { return; }

    print_tabs(string_buf, level);
    if(node->type == NODE_INT) {
        sb_append_int(string_buf, node->data.value);
        sb_append_char(string_buf, '\n');
        // printf("%d\n", node->data.value);
    } else {
        sb_append_char(string_buf, '<');
        sb_append(string_buf, get_node_type_name(node->type));
        sb_append(string_buf, ">\n");
        // printf("<%s>\n", get_node_type_name(node->type));
        if(node->type == NODE_NEG) {
            print_ast(string_buf, node->data.unary.operand, level + 1);
        } else {
            print_ast(string_buf, node->data.op.left, level + 1);
            print_ast(string_buf, node->data.op.right, level + 1);
        }
    }
}

void ast_to_string(StrBuf *string_buf, ASTNode *node) {
    if(node == NULL) { return; }

    if(node->type == NODE_INT) {
        sb_append_int(string_buf, node->data.value);
    } else if(node->type == NODE_NEG) {
        sb_append(string_buf, "(");
        sb_append(string_buf, " ");
        sb_append(string_buf, get_node_type_name(node->type));
        sb_append(string_buf, " ");
        ast_to_string(string_buf, node->data.unary.operand);
        sb_append(string_buf, ")");
    } else {
        sb_append(string_buf, "(");
        ast_to_string(string_buf, node->data.op.left);
        sb_append(string_buf, " ");
        sb_append(string_buf, get_node_type_name(node->type));
        sb_append(string_buf, " ");
        ast_to_string(string_buf, node->data.op.right);
        sb_append(string_buf, ")");
    }
}

// void free_ast(ASTNode *node) {
//     if(!node) return;

//     if(node->type == NODE_NEG) {
//         free_ast(node->data.unary.operand);
//     } else if(node->type != NODE_INT) {
//         free_ast(node->data.op.left);
//         free_ast(node->data.op.right);
//     }
//     free(node);
// }