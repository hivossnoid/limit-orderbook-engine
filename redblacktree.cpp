#include <iostream>
#include <chrono>
#include <string>

#include "./limit.cpp"

#pragma once

enum Color {
    BLACK,
    RED
};

struct Node{
    int data;
    int color;
    Node* left;
    Node* right;
    Node* parent;

    Limit* limit;

    Node(int val, Limit* _limit) : data(val), color(RED), left(nullptr),
        right(nullptr), parent(nullptr), limit(_limit) {}
};

class Redblacktree{
private:
    Node* root;
    Node* NIL;

    void left_rotate(Node* alpha){

        Node* beta = alpha->right;
        alpha->right = beta->left;

        if (beta->left != NIL){
            beta->left->parent = alpha;
        }
        beta->parent = alpha->parent;

        if (alpha->parent == NIL){
            root = beta;
        } else if (alpha == alpha->parent->left){
            alpha->parent->left = beta;
        } else {
            alpha->parent->right = beta;
        }

        beta->left = alpha;
        alpha->parent = beta;
    } // left_rotate-*

    void right_rotate(Node* alpha){

        Node* beta = alpha->left;

        alpha->left = beta->right;
        if (beta->right != NIL){
            beta->right->parent = alpha;
        }
        beta->parent = alpha->parent;

        if (alpha->parent == NIL){
            root = beta;
        } else if (alpha == alpha->parent->right){
            alpha->parent->right = beta;
        } else {
            alpha->parent->left = beta;
        }

        beta->right = alpha;
        alpha->parent = beta;
    } // right_roate-*

    void transplant(Node* u, Node* v){

        if (u->parent == NIL){
            root = v;
        }
        else if (u == u->parent->left){
            u->parent->left = v;
        }
        else {
            u->parent->right = v;
        }

        v->parent = u->parent;
    } // transplant-*

    void fix_insertion(Node* kappa){

        while (kappa != root && kappa->parent->color == RED){

            if (kappa->parent == kappa->parent->parent->left){
                Node* alpha = kappa->parent->parent->right;

                if (alpha->color == RED){
                    kappa->parent->color = BLACK;
                    alpha->color = BLACK;
                    kappa->parent->parent->color = RED;

                    kappa = kappa->parent->parent; // wtf is this???
                } else {
                    if (kappa == kappa->parent->right){
                        kappa = kappa->parent;
                        left_rotate(kappa);
                    }
                    kappa->parent->color = BLACK;
                    kappa->parent->parent->color = RED;
                    right_rotate(kappa->parent->parent);
                }
            }
            else {
                Node* beta = kappa->parent->parent->left;

                if (beta->color == RED){
                    kappa->parent->color = BLACK;
                    beta->color = BLACK;
                    kappa->parent->parent->color = RED;

                    kappa = kappa->parent->parent;
                } else {
                    if (kappa == kappa->parent->left){
                        kappa = kappa->parent;
                        right_rotate(kappa);
                    }
                    kappa->parent->color = BLACK;
                    kappa->parent->parent->color = RED;
                    left_rotate(kappa->parent->parent);
                }
            }
        }
       root->color = BLACK;
    } // fix_insertion-*

    void fix_deletion(Node* kappa){

        while (kappa != root && kappa->color == BLACK){

            if (kappa == kappa->parent->left){

                Node* alpha = kappa->parent->right;

                if (alpha->color == RED){

                    alpha->color = BLACK;
                    kappa->parent->color = RED;

                    left_rotate(kappa->parent);
                    alpha = kappa->parent->right;
                }

                if (alpha->left->color == BLACK &&
                    alpha->right->color == BLACK){
                        alpha->color = RED;
                        kappa = kappa->parent;
                }
                else {

                    if (alpha->right->color == BLACK){

                        Node* beta = alpha->left;

                        beta->color = BLACK;
                        alpha->color = RED;

                        right_rotate(alpha);

                        alpha = kappa->parent->right;
                    }

                    alpha->color = kappa->parent->color;
                    kappa->parent->color = BLACK;
                    alpha->right->color = BLACK;

                    left_rotate(kappa->parent);

                    kappa = root;
                }
            }

            else if (kappa == kappa->parent->right){
                Node* alpha = kappa->parent->left;

                if (alpha->color == RED){

                    alpha->color = BLACK;
                    kappa->parent->color = RED;

                    right_rotate(kappa->parent);
                    alpha = kappa->parent->left;
                }


               if (alpha->left->color == BLACK &&
                   alpha->right->color == BLACK){
                       alpha->color = RED;
                       kappa = kappa->parent;
               }
               else {

                   if (alpha->left->color == BLACK){

                       Node* beta = alpha->right;

                       beta->color = BLACK;
                       alpha->color = RED;

                       left_rotate(alpha);

                       alpha = kappa->parent->left;
                   }

                   alpha->color = kappa->parent->color;
                   kappa->parent->color = BLACK;
                   alpha->left->color = BLACK;

                   right_rotate(kappa->parent);

                   kappa = root;
               }
            }
        }
        kappa->color = BLACK;
    } // fix_deletion-*

    Node* helper_min(Node* curr){
        while (curr != NIL && curr->left != NIL){
            curr = curr->left;
        }
        return curr;
    }

    void inorder_traversal(Node* current){
        if (current == NIL){
            return;
        }

        inorder_traversal(current->left);

        // Visit the current node
        delete current->limit;
        current->limit = nullptr;

        inorder_traversal(current->right);
    }

    void destroy_nodes(Node* current){
        if (current == NIL){
            return;
        }

        destroy_nodes(current->left);
        destroy_nodes(current->right);
        delete current;
    }

public:
    Redblacktree(){
        NIL = new Node(0, nullptr);
        NIL->color = BLACK;
        NIL->left = NIL;
        NIL->right = NIL;
        NIL->parent = NIL;

        root = NIL;
    } // RBT constructor-*


    void insert_value(int val, Limit* _limit){
        Node* new_node = new Node(val, _limit);
        new_node->left = NIL;
        new_node->right = NIL;
        new_node->color = RED;

        Node* parent = NIL;
        Node* current = root;

        while (current != NIL){
            parent = current;
            if (new_node->data < current->data){
                current = current->left;
            } else {
                current = current->right;
            }
        }

        new_node->parent = parent;

        if (parent == NIL){
            root = new_node;
        } else if (new_node->data < parent->data){
            parent->left = new_node;
        } else {
            parent->right = new_node;
        }

        fix_insertion(new_node);
    } // insert_value-*

    bool search_value(int val){
        Node* current = root;

        if (current == NIL){
            return false;
        }
        if (val == current->data){
            return true;
        }
        while (current != NIL){
            if (val < current->data){
                current = current->left;
            }
            else if (val > current->data) {
                current = current->right;
            }
            else if (val == current->data){
                return true;
            }
        }
        return false;
    } // search_value-*

    Limit* search_limit(int val){
        Node* current = root;

        while (current != NIL){
            if (val < current->data){
                current = current->left;
            }
            else if (val > current->data){
                current = current->right;
            }
            else {
                return current->limit;
            }
        }

        return nullptr;
    }

    int max_val(){
        if (root == NIL){
            std::cout << "[TREE EMPTY]" << std::endl;
            return root->data;
        }

        Node* curr = root;
        while (curr && curr->right != NIL){
            curr = curr->right;
        }
        return curr->data;
    } // max_val-*

    int min_val(){
        if (root == NIL){
            std::cout << "[TREE EMPTY]" << std::endl;
            return root->data;
        }

        Node* curr = root;
        while (curr && curr->left != NIL){
            curr = curr->left;
        }
        return curr->data;
    } // min_val-*

    void delete_value(int val){

        Node* kappa = root;

        while (kappa != NIL && kappa->data != val){
            if (val < kappa->data){
                kappa = kappa->left;
            } else {
                kappa = kappa->right;
            }
        }

        if (kappa == NIL){
            return;
        }

        Node* alpha = kappa;
        int orig_color = alpha->color;
        Node* beta;

        if (kappa->left == NIL){

            beta = kappa->right;
            transplant(kappa, beta);

        } else if (kappa->right == NIL){

            beta = kappa->left;
            transplant(kappa, beta);
        }

        else {
            alpha = helper_min(kappa->right);
            orig_color = alpha->color;
            beta = alpha->right;

            if (alpha->parent == kappa){
                beta->parent = alpha;
            }
            else {
                transplant(alpha, beta);

                alpha->right = kappa->right;
                alpha->right->parent = alpha;
            }

            transplant(kappa, alpha);

            alpha->left = kappa->left;
            alpha->left->parent = alpha;

            alpha->color = kappa->color;
        }
        delete kappa;

        if (orig_color == BLACK){
            fix_deletion(beta);
        }
    } // delete_val-*

    void destroy_limits(){
        inorder_traversal(root);
    } // destroy_limits-* safely remove limit objects.

    ~Redblacktree(){
            destroy_nodes(root);
            delete NIL;
        }
};
