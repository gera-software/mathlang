#include <criterion/criterion.h>
#include "ast.h"
#include <stdio.h>
#include "arena.h"

Test(ast_suite, should_create_int_node) {
    Arena *a = arena_alloc(1024);

    ASTNode *node = create_int_node(a, 50);

    cr_assert_eq(node->type, NODE_INT);
    cr_assert_eq(node->data.value, 50);

    arena_release(a);
}

Test(ast_suite, should_create_binary_op_nodes) {
    Arena *a = arena_alloc(1024);

    ASTNode *node1 = create_int_node(a, 1);
    ASTNode *node2 = create_int_node(a, 2);

    ASTNode *node_add = create_binary_op_node(a, NODE_ADD, node1, node2);
    cr_assert_eq(node_add->type, NODE_ADD);
    cr_assert_eq(node_add->data.op.left, node1);
    cr_assert_eq(node_add->data.op.right, node2);
    
    ASTNode *node_sub = create_binary_op_node(a, NODE_SUB, node1, node2);
    cr_assert_eq(node_sub->type, NODE_SUB);
    cr_assert_eq(node_sub->data.op.left, node1);
    cr_assert_eq(node_sub->data.op.right, node2);

    ASTNode *node_mul = create_binary_op_node(a, NODE_MUL, node1, node2);
    cr_assert_eq(node_mul->type, NODE_MUL);
    cr_assert_eq(node_mul->data.op.left, node1);
    cr_assert_eq(node_mul->data.op.right, node2);

    ASTNode *node_div = create_binary_op_node(a, NODE_DIV, node1, node2);
    cr_assert_eq(node_div->type, NODE_DIV);
    cr_assert_eq(node_div->data.op.left, node1);
    cr_assert_eq(node_div->data.op.right, node2);

    arena_release(a);
}

Test(ast_suite, should_create_unary_op_nodes) {
    Arena *a = arena_alloc(1024);

    ASTNode *node1 = create_int_node(a, 1);

    ASTNode *node_neg = create_unary_op_node(a, NODE_NEG, node1);
    cr_assert_eq(node_neg->type, NODE_NEG);
    cr_assert_eq(node_neg->data.unary.operand, node1);

    arena_release(a);
}

Test(ast_suite, should_print_ast) {
    Arena *a = arena_alloc(1024);

    ASTNode *num1 = create_int_node(a, 5);
    ASTNode *num2 = create_int_node(a, 6);
    ASTNode *num3 = create_int_node(a, 7);
    ASTNode *num4 = create_int_node(a, 8);
    ASTNode *op1 = create_unary_op_node(a, NODE_NEG, num1);
    ASTNode *op2 = create_binary_op_node(a, NODE_MUL, op1, num2);
    ASTNode *op3 = create_binary_op_node(a, NODE_DIV, num3, num4);
    
    ASTNode *root = create_binary_op_node(a, NODE_ADD, op2, op3);
    
    print_ast(root, 0);
    
    arena_release(a);
}

Test(ast_suite, should_pretty_print_ast) {
    Arena *a = arena_alloc(1024);

    ASTNode *num1 = create_int_node(a, 5);
    ASTNode *num2 = create_int_node(a, 6);
    ASTNode *num3 = create_int_node(a, 7);
    ASTNode *num4 = create_int_node(a, 8);
    ASTNode *op1 = create_unary_op_node(a, NODE_NEG, num1);
    ASTNode *op2 = create_binary_op_node(a, NODE_MUL, op1, num2);
    ASTNode *op3 = create_binary_op_node(a, NODE_DIV, num3, num4);
    
    ASTNode *root = create_binary_op_node(a, NODE_ADD, op2, op3);
    
    ast_to_string(root);
    printf("\n");
    
    arena_release(a);
}
