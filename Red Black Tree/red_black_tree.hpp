#include<iostream>
#include<bits/stdc++.h>
#include "color.hpp"
using namespace std;

class Node
{
public:
    int data;
    string value;
    Node* parent;
    Node* left;
    Node* right;
    char color;
    Node(int data, string value) : data(data), value(value), parent(nullptr), left(nullptr), right(nullptr), color('R') {}
    Node()
    {
        color = 'B';
        left = nullptr;
        right = nullptr;
    }

};

typedef Node* nodeptr;

class RotationManager
{
    nodeptr tnull;

public:
    RotationManager()
    {
        tnull = new Node();
    tnull->color = 'B';
    tnull->left = tnull;
    tnull->right = tnull;
    tnull->parent = tnull;
    }
    void leftRotate(nodeptr& root,nodeptr x)
    {
        nodeptr y=x->right;
        x->right=y->left;
        if(y->left!=tnull)
        {
            y->left->parent=x;
        }
        y->parent=x->parent;
        if(x->parent==nullptr)
        {
            root=y;
        }
        else if(x==x->parent->left)
        {
            x->parent->left=y;
        }
        else
        {
            x->parent->right=y;
        }
        y->left=x;
        x->parent=y;
    }

    void rightRotate(nodeptr& root,nodeptr y)
    {
        nodeptr x=y->left;
        y->left=x->right;
        if(x->right!=tnull)
        {
            x->right->parent=y;
        }
        x->parent=y->parent;
        if(y->parent==nullptr)
        {
            root=x;
        }
        else if(y==y->parent->left)
        {
            y->parent->left=x;
        }
        else
        {
            y->parent->right=x;
        }
        x->right=y;
        y->parent=x;
    }

};


class RedBlackTree
{
    nodeptr root;
    nodeptr tnull;
    int size;

     void leftRotate(nodeptr& root,nodeptr x)
    {
        nodeptr y=x->right;
        x->right=y->left;
        if(y->left!=tnull)
        {
            y->left->parent=x;
        }
        y->parent=x->parent;
        if(x->parent==nullptr)
        {
            root=y;
        }
        else if(x==x->parent->left)
        {
            x->parent->left=y;
        }
        else
        {
            x->parent->right=y;
        }
        y->left=x;
        x->parent=y;
    }

    void rightRotate(nodeptr& root,nodeptr y)
    {
        nodeptr x=y->left;
        y->left=x->right;
        if(x->right!=tnull)
        {
            x->right->parent=y;
        }
        x->parent=y->parent;
        if(y->parent==nullptr)
        {
            root=x;
        }
        else if(y==y->parent->left)
        {
            y->parent->left=x;
        }
        else
        {
            y->parent->right=x;
        }
        x->right=y;
        y->parent=x;
    }



    void insertFix(nodeptr& root,nodeptr k)
    {
        while (k->parent->color == 'R')
        {
            if (k->parent == k->parent->parent->left)
            {
                insertFixLeft(root, k);
            }
            else
            {
                insertFixRight(root, k);
            }
            if(k==root)
            {
                break;
            }
        }
        root->color = 'B';
    }
    void insertFixLeft(nodeptr& root, nodeptr k)
    {
        nodeptr u = k->parent->parent->right; // Uncle
        if (u != nullptr && u->color == 'R')
        {
            //  Uncle is red
            k->parent->color = 'B';
            u->color = 'B';
            k->parent->parent->color = 'R';
            k = k->parent->parent;
        }
        else
        {
            // Uncle  black
            if (k == k->parent->right)
            {
                //  Node is right child
                k = k->parent;

                leftRotate(root, k);
            }
            //  Node is left child
            k->parent->color = 'B';
            k->parent->parent->color = 'R';
            rightRotate(root, k->parent->parent);
        }
    }
    void insertFixRight(nodeptr& root, nodeptr k)
    {
        nodeptr u = k->parent->parent->left; // Uncle
        if (u != nullptr && u->color == 'R')
        {
            //  Uncle is red
            k->parent->color = 'B';
            u->color = 'B';
            k->parent->parent->color = 'R';
            k = k->parent->parent;
        }
        else
        {
            // Uncle is black
            if (k == k->parent->left)
            {
                //  Node is left child
                k = k->parent;
                rightRotate(root, k);
            }
            //  Node is right child
            k->parent->color = 'B';
            k->parent->parent->color = 'R';
            leftRotate(root, k->parent->parent);
        }
    }



    void deleteNodeFix(nodeptr& root, nodeptr x)
    {
        nodeptr s;
        while (x != root && x->color == 'B')
        {
            if (x == x->parent->left)
            {
                 s = x->parent->right; // Sibling node
            if (s->color == 'R')
            {
                //  Sibling s is red
                s->color = 'B';
                x->parent->color = 'R';
                leftRotate(root, x->parent);
                s = x->parent->right;
            }

            if (s->left->color == 'B' && s->right->color == 'B')
            {
                //  Both of s's children are black
                s->color = 'R';
                x = x->parent;
            }
            else
            {
                if (s->right->color == 'B')
                {
                    // Left child of s is red, right child is black
                    s->left->color = 'B';
                    s->color = 'R';
                    rightRotate(root, s);
                    s = x->parent->right;
                }
                //  Right child of s is red
                s->color = x->parent->color;
                x->parent->color = 'B';
                s->right->color = 'B';
                leftRotate(root, x->parent);
                x = root;
            }
            }
            else
            {
                s = x->parent->left; // Sibling node
            if (s->color == 'R')
            {
                //  Sibling s is red
                s->color = 'B';
                x->parent->color = 'R';
               rightRotate(root, x->parent);
                s = x->parent->left;
            }

            if (s->right->color == 'B' && s->left->color == 'B')
            {
                //  Both of s's children are black
                s->color = 'R';
                x = x->parent;
            }
            else
            {
                if (s->left->color == 'B')
                {
                    // Right child of s is red, left child is black
                    s->right->color = 'B';
                    s->color = 'R';
                    leftRotate(root, s);
                    s = x->parent->left;
                }
                //  Left child of s is red
                s->color = x->parent->color;
                x->parent->color = 'B';
                s->left->color = 'B';
                rightRotate(root, x->parent);
                x = root;
            }
            }
        }

            x->color = 'B';

    }




    void rbTransplant( nodeptr u, nodeptr v)
    {
        if (u->parent == nullptr)
        {

            root = v;
        }
        else if (u == u->parent->left)
        {

            u->parent->left = v;
        }
        else
        {

            u->parent->right = v;
        }




            v->parent = u->parent;

    }


    void deleteNodeHelper(nodeptr& root, int key)
    {
        nodeptr z = root;
        while (z != nullptr)
        {
            if (z->data == key)
            {
                break;
            }
            z = (key < z->data) ? z->left : z->right;
        }

        if (z == nullptr)
        {
            cout <<key<< " not found " << endl;
            return;
        }

        nodeptr y = z;
        nodeptr x;
        char y_original_color = y->color;

        // If z has fewer than 2 children
        if (z->left == nullptr)
        {
            x = z->right ? z->right : tnull;
            rbTransplant(z, z->right);
        }
        else if (z->right == nullptr)
        {
            x = z->left ? z->left : tnull;
            rbTransplant( z, z->left);
        }
        else
        {
            y = findmax(z->left);
            y_original_color = y->color;
            x = y->left;

            if (y->parent == z)
            {
                if (x != nullptr)
                {
                    x->parent = y;
                }
            }
            else
            {
                rbTransplant( y, y->left);
                y->left = z->left;
                y->left->parent = y;
            }

            rbTransplant( z, y);
            y->right = z->right;
            y->right->parent = y;
            y->color = z->color;
        }

        delete z;

        if (y_original_color == 'B')
        {
            deleteNodeFix(root, x);
        }
    }

   void clearHelp(nodeptr root)
    {
        if (root == tnull)
        {
            return;
        }
        clearHelp(root->left);
        clearHelp(root->right);
        delete root;

    }

    bool findHelper(nodeptr root, int element)
    {
        if (root == tnull)
            return false;
        if (element < root->data)
        {
            return findHelper(root->left, element);
        }
        else if (element > root->data)
        {
            return findHelper(root->right, element);
        }
        else
            return true;
    }

    nodeptr searchHelper(nodeptr root, int element)
    {
        if (root == tnull)
            return root;
        if (element < root->data)
        {
            return searchHelper(root->left, element);
        }
        else if (element > root->data)
        {
            return searchHelper(root->right, element);
        }
        else
            return root;
    }



    int sizeHelper(nodeptr node)
    {
        if (node == nullptr)
        {
            return 0;
        }
        return 1 + sizeHelper(node->left) + sizeHelper(node->right);
    }



    void inOrderTraversalHelper(nodeptr node)
    {
        if (node == tnull)
            return;

            inOrderTraversalHelper(node->left);
            if (node->color =='R')
        {
            cout << dye::red(node->data) << dye::red(" => ") << dye::red(node->value) << endl;
        }
        else{
            cout << node->data << " => " << node->value << endl;
        }

            inOrderTraversalHelper(node->right);

    }

     void printhelp(nodeptr root)
    {
        if (root == tnull)
            return;
        if (root->color == 'R')
        {
            cout << dye::red(root->data) << dye::red("_") << dye::red(root->value);
        }
        else
            cout << root->data << "_" << root->value;
        if (root->left == tnull && root->right == tnull)
        {
            return;
        }
        cout << "(";
        printhelp(root->left);
        cout << ",";
        printhelp(root->right);
        cout << ")";
    }



    void eraseHelp(nodeptr& root, int key) {
    nodeptr z = root;
    nodeptr x, y;

    while (z != tnull) {
        if (z->data == key) {
            break;
        }
        if (key < z->data) {
            z = z->left;
        } else {
            z = z->right;
        }
    }

    // not found
    if (z == tnull) {
        cout << key << " not found" << endl;
        return;
    }
    size--;

    y = z;
    char y_original_color = y->color;
    if (z->left == tnull) {
        x = z->right;
        rbTransplant(z, z->right);
    } else if (z->right == tnull) {
        x = z->left;
        rbTransplant(z, z->left);
    } else {
        y = maximum(z->left);
        y_original_color = y->color;
        x = y->left;

        if (y->parent != z) {
            rbTransplant(y, y->left);
            y->left = z->left;
            y->left->parent = y;
        }

        rbTransplant(z, y);
        y->right = z->right;
        y->right->parent = y;
        y->color = z->color;
    }

    delete z;
    if (y_original_color == 'B') {
        deleteNodeFix(root,x);
    }
    printhelp(root);
        cout << endl;
}


public:

    RedBlackTree() {
    tnull = new Node();
    tnull->color = 'B';
    tnull->left = tnull;
    tnull->right = tnull;
    tnull->parent = tnull;
    root = tnull;
    size = 0;
}

    ~RedBlackTree()
    {
        clearHelp(root);
        delete tnull;
    }


    void insert(int key,string value)
    {
        nodeptr node = new Node(key,value);
        node->left=tnull;
        node->right=tnull;
        nodeptr y = nullptr;
        nodeptr x = root;
        nodeptr exist=searchHelper(root,key);
        if(exist!=tnull)
        {
            exist->value=value;
            printhelp(root);
            cout<<endl;
            return;

        }else{
            size++;
        while (x != tnull)
        {
            y = x;
            if (node->data < x->data)
            {
                x = x->left;
            }
            else if(node->data > x->data)
            {
                x = x->right;
            }
            else
            {
                x->value = value;

                    printhelp(root);
                    cout<<endl;
                    return;
            }
        }
        }

        node->parent = y;
        if (y == nullptr)
        {
            root = node;
        }
        else if (node->data < y->data)
        {
            y->left = node;
        }
        else
        {
            y->right = node;
        }

        if (node->parent == nullptr)
        {

            node->color = 'B';
             printhelp(root);
             cout<<endl;
            return;
        }

        if (node->parent->parent == nullptr)
        {
             printhelp(root);
             cout<<endl;
            return;
        }

        insertFix(root, node);
        printhelp(root);
        cout<<endl;

    }

    nodeptr findmax(nodeptr node)
    {
        if (node == nullptr)
        {
            return nullptr; //  subtree is empty
        }
        while (node->right != nullptr)
        {
            node = node->right;
        }
        return node;
    }

    nodeptr minimum(nodeptr root)
    {
        if (root->left == tnull)
        {
            return root;
        }
        else
        {
            return minimum(root->left);
        }
    }
    nodeptr maximum(nodeptr root)
    {
        while (root->right != tnull)
        {
            root=root->right;
        }
       return root;
    }
    void clear()
    {
        if (size == 0)
        {
            cout << "unsuccessful" << endl;
            return;
        }
        clearHelp(root);
        // cout << size << endl;
        root = nullptr;
        size = 0;
        cout << "successful" << endl;
    }

    void find(int key)
    {
        if (size == 0)
        {
            cout << key << " not found" << endl;
            return;
        }
        if (findHelper(root, key))
            cout << key << " found" << endl;
        else
            cout << key << " not found" << endl;
    }

    void empty()
    {
        if (size == 0)
            cout << "yes" << endl;
        else
            cout << "no" << endl;
    }

    void Size()
    {
        cout << size << endl;
    }
    void iteration()
    {
        inOrderTraversalHelper(root);
    }

    void erase(int key)
    {
        if (size == 0)
        {
            cout << key << " not found" << endl;
            return;
        }

        eraseHelp(root,key);
    }

};
