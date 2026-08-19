#include <criterion/criterion.h>
#include "ast.h"

Test(ast_suite, print_ast) {
    ASTNode *num2 = create_int_node(5);
    ASTNode *num3 = create_int_node(6);
    ASTNode *num4 = create_int_node(7);
    ASTNode *num5 = create_int_node(8);
    ASTNode *op1 = create_op_node(NODE_MUL, num2, num3);
    ASTNode *op2 = create_op_node(NODE_DIV, num4, num5);
    
    ASTNode *root = create_op_node(NODE_ADD, op1, op2);
    
    print_ast(root, 0);
    
    free_ast(root);
}

Test(ast_suite, should_create_int_node) {
    ASTNode *node = create_int_node(50);

    cr_assert_eq(node->type, NODE_INT);
    cr_assert_eq(node->data.value, 50);

    free_ast(node);

}

Test(ast_suite, should_create_op_nodes) {
    ASTNode *node1 = create_int_node(1);
    ASTNode *node2 = create_int_node(2);

    ASTNode *node_add = create_op_node(NODE_ADD, node1, node2);
    cr_assert_eq(node_add->type, NODE_ADD);
    cr_assert_eq(node_add->data.op.left, node1);
    cr_assert_eq(node_add->data.op.right, node2);
    
    ASTNode *node_sub = create_op_node(NODE_SUB, node1, node2);
    cr_assert_eq(node_sub->type, NODE_SUB);
    cr_assert_eq(node_sub->data.op.left, node1);
    cr_assert_eq(node_sub->data.op.right, node2);

    ASTNode *node_mul = create_op_node(NODE_MUL, node1, node2);
    cr_assert_eq(node_mul->type, NODE_MUL);
    cr_assert_eq(node_mul->data.op.left, node1);
    cr_assert_eq(node_mul->data.op.right, node2);

    ASTNode *node_div = create_op_node(NODE_DIV, node1, node2);
    cr_assert_eq(node_div->type, NODE_DIV);
    cr_assert_eq(node_div->data.op.left, node1);
    cr_assert_eq(node_div->data.op.right, node2);

    free(node_add);
    free(node_sub);
    free(node_mul);
    free(node_div);
}