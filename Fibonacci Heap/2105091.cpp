#include<iostream>
#include<bits/stdc++.h>
using namespace std;

class Node
{
public:
    int key;
    int value;
    int degree;
    Node *parent;
    Node *child;
    Node *left;
    Node *right;
    bool mark;
    Node(int _key, int _value) : key(_key), value(_value), degree(0), parent(nullptr), child(nullptr), left(this), right(this), mark(false) {}
};


class FibonacciHeap
{
public:
    int size;
    Node* max;
     unordered_map<int, Node*> nodeMap;
    FibonacciHeap()
    {
        make_heap();
    }

    void make_heap()
    {
        max=nullptr;
        size=0;
    }
};

class PriorityQueue
{
    FibonacciHeap heap;
public:
    FibonacciHeap insert(FibonacciHeap& heap,int key,int value)
    {
        auto it = heap.nodeMap.find(value);
    if (it != heap.nodeMap.end()) {
        Node* existingNode = it->second;
        if (key != existingNode->key) {
            existingNode->key = key;
            if (key > heap.max->key) {
                heap.max = existingNode;

            }
        }
        return heap;
    }
        Node* node=new Node(key,value);
        node->key=key;
        node->value=value;
        node->left = node;
        node->right = node ;
        node->parent = nullptr ;
        node->child = nullptr ;
        node->degree = 0 ;
        node->mark = false ;

        heap.nodeMap[value] = node;
        if(heap.max==nullptr)
        {
            heap.max=node;
            heap.size++;
            return heap;
        }
        (heap.max->left)->right=node;
        node->right=heap.max;
        node->left=heap.max->left;
        heap.max->left=node;
        if(node->key>heap.max->key)
        {
            heap.max=node;
        }
        heap.size++;
        return heap;
    }

   Node* extractMax(FibonacciHeap& heap) {
       Node* temp=heap.max;
    if (heap.max == nullptr) return temp;
    heap.nodeMap.erase(heap.max->value);

    Node* z = heap.max;
    Node* x = z->child;

    // Add children of z to the root list
    if (x != nullptr) {
        do {
            Node* next = x->right;
             heap.max->right->left = x;
            x->right = heap.max->right;
             heap.max->right = x;
              x->left = heap.max;
            x->parent = nullptr; // Remove parent
            x = next;
        } while (x != heap.max->child); // Iterate all children
    }

    // Remove z from the root list
    z->left->right = z->right;
    z->right->left = z->left;

    if (z == z->right && z->child==nullptr) {
        heap.max = nullptr;
    } else {
        heap.max = z->right;
        Consolidate(heap);
    }

    heap.size--;

    return z;
}

void Consolidate(FibonacciHeap& heap) {
    if (heap.max == nullptr) return;

    int maxDegree = log(heap.size) / log(2) + 1;
    vector<Node*> degreeRoots(maxDegree , nullptr);

    vector<Node*> roots;
    Node* current = heap.max;
    do {
        roots.push_back(current);
        current = current->right;
    } while (current != heap.max);

    for (Node* node : roots) {
        int degree = node->degree;
        while (degreeRoots[degree] != nullptr) {
            Node* sameDegreeNode = degreeRoots[degree];
            if (node->key < sameDegreeNode->key) {
                swap(node, sameDegreeNode);
            }
            if (sameDegreeNode == heap.max) {
                heap.max = node;
            }

            // Link sameDegreeNode as a child of node
            sameDegreeNode->left->right = sameDegreeNode->right;
            sameDegreeNode->right->left = sameDegreeNode->left;

            if (node->child == nullptr) {
                node->child = sameDegreeNode;
                sameDegreeNode->right = sameDegreeNode;
                sameDegreeNode->left = sameDegreeNode;
            } else {
                sameDegreeNode->left = node->child;
                sameDegreeNode->right = node->child->right;
                node->child->right->left = sameDegreeNode;
                node->child->right = sameDegreeNode;
            }
            sameDegreeNode->parent = node;
            node->degree++;
            sameDegreeNode->mark = false;
            degreeRoots[degree] = nullptr;
            degree++;
        }
        degreeRoots[degree] = node;
    }

    // Reconstructing root list and finding the new max node
    heap.max = nullptr;
    for (int i = 0; i < maxDegree; i++) {
        if (degreeRoots[i] != nullptr) {
            if (heap.max == nullptr) {
                heap.max = degreeRoots[i]->left = degreeRoots[i]->right = degreeRoots[i];
            } else {
                degreeRoots[i]->right = heap.max;
                degreeRoots[i]->left = heap.max->left;
                heap.max->left->right = degreeRoots[i];
                heap.max->left = degreeRoots[i];
                if (degreeRoots[i]->key > heap.max->key) {
                    heap.max = degreeRoots[i];
                }
            }
        }
    }
}
bool is_empty(FibonacciHeap& heap)
{
    if(heap.max==nullptr){
        cout<<"True"<<endl;
        return true;
    }
    else{
        cout<<"False"<<endl;
        return false;
    }
}

Node* findMax(FibonacciHeap& heap)
{
    return heap.max;
}

Node* find(FibonacciHeap& heap, int value) {
   auto it = heap.nodeMap.find(value);
        if (it != heap.nodeMap.end()) {
            return it->second;
        }
        return nullptr;
}


void Cut(FibonacciHeap& heap,Node* child,Node* parent)
{
    if (child->right == child) {
        parent->child = nullptr; // child was the only child
    } else {
        child->left->right = child->right;
        child->right->left = child->left;
        if (parent->child == child) {
            parent->child = child->right;
        }
    }
    parent->degree--;
    // Add child to the root list
    child->left = heap.max;
    child->right = heap.max->right;
    heap.max->right->left = child;
    heap.max->right = child;
    child->parent = nullptr;
    child->mark = false;
}

void Cascading_Cut(FibonacciHeap& heap,Node* node)
{
    Node* parent=node->parent;
    if(parent!=nullptr)
    {
        if(node->mark==false)
        {
            node->mark=true;
        }else{
            Cut(heap,node,parent);
            Cascading_Cut(heap,parent);
        }
    }
}
FibonacciHeap increaseKey(FibonacciHeap& heap,int value,int newKey)
{
    if (heap.max == nullptr) {
        cout << "The Heap is Empty" << endl;
        return heap;
    }
    Node* node=find(heap,value);
    if (node == nullptr) {
        cout << "Node not found in the Heap" << endl;
        return heap;
    }
    if(node->key>newKey)
    {
        cout << "New key is smaller than current key, operation not allowed in increaseKey." << endl;
        return heap;
    }
    node->key=newKey;
    Node* parent=node->parent;
    if(parent!=nullptr && node->key>parent->key)
    {
        Cut(heap,node,parent);
        Cascading_Cut(heap,parent);
    }
    if(node->key>heap.max->key)
    {
        heap.max=node;
    }
    return heap;

}

FibonacciHeap deleteNode(FibonacciHeap& heap, int value) {
    Node* nodeToDelete = find(heap, value);

    if (nodeToDelete == nullptr) {
        return heap;
    }


    increaseKey(heap, nodeToDelete->value, INT_MAX);
    extractMax(heap);

    return heap;
}


FibonacciHeap meld(FibonacciHeap& heap1, FibonacciHeap& heap2) {
    FibonacciHeap mergedHeap; // Create a new heap to hold the merged values

    if (heap1.max) {
        // Copy heap1
        mergedHeap.max = heap1.max;
        mergedHeap.size = heap1.size;
        mergedHeap.nodeMap = heap1.nodeMap;
    }

    if (heap2.max) {
        // Add heap2
        if (mergedHeap.max) {
            // Link
            Node* temp = mergedHeap.max->right;
            mergedHeap.max->right = heap2.max->right;
            heap2.max->right->left = mergedHeap.max;
            temp->left = heap2.max;
            heap2.max->right = temp;

            // Update max
            if (heap2.max->key > mergedHeap.max->key) {
                mergedHeap.max = heap2.max;
            }

            mergedHeap.size += heap2.size;

            for (auto& pair : heap2.nodeMap) {
                mergedHeap.nodeMap.insert(pair);
            }
        } else {
            mergedHeap.max = heap2.max;
            mergedHeap.size = heap2.size;
            mergedHeap.nodeMap = heap2.nodeMap;
        }
    }
    heap1.make_heap();
    heap2.make_heap();

    return mergedHeap;
}



};

void printTree(FibonacciHeap &heap,Node* temp) {
     if (temp == nullptr)
            return;
        Node *child = temp->child;
        if (child == nullptr && temp->parent != nullptr)
        {
            return;
        }
        if (child == nullptr)
        {
            cout << "    (" << temp->key << ", " << temp->value << ")" << endl;
            return;
        }

        cout << "    (" << temp->key << ", " << temp->value << ") -> ";

        if (temp->child != nullptr)
        {
            cout << " (" << temp->child->key << ", " << temp->child->value << ")";
        }

        while (child->left != temp->child)
        {
            child = child->left;
            cout << " ";
            cout << "(" << child->key << ", " << child->value << ")";
        }
        cout << endl;
        child = temp->child;
        do
        {
            printTree(heap, child);
            child = child->left;
        } while (child != temp->child);
    }

void printHeap(FibonacciHeap& heap) {
    if (heap.max == nullptr) {
        cout << "The heap is empty." << endl;
        return;
    }

    Node* curr = heap.max;
    int treeCount = 1;
     cout << "tree " << treeCount << ":";
     cout<<endl;
     printTree(heap,curr);
    while (curr->left != heap.max){
       treeCount++;
       cout<<"tree "<<treeCount<<endl;
        curr = curr->left;

         printTree(heap,curr);
    }
    cout<<endl;
}

void test()
{
      FibonacciHeap heap,heap1,heap2;
    PriorityQueue pq;
    pq.insert(heap,31,1);
        pq.insert(heap,-19,2);
        pq.insert(heap,42,3);
        pq.insert(heap,2,4);
        pq.insert(heap,-4,5);
        pq.insert(heap,7,6);
        pq.insert(heap,-17,7);
        pq.insert(heap,28,8);
        pq.insert(heap,14,9);
        pq.insert(heap,-1,10);
        pq.insert(heap,36,11);
        pq.insert(heap,10,12);
        pq.insert(heap,4,13);
         printHeap(heap);
        pq.extractMax(heap);
        printHeap(heap);
        pq.increaseKey(heap,6,29);
        printHeap(heap);
        pq.insert(heap,12,14);
        pq.insert(heap,19,15);
        pq.insert(heap,-8,16);
        pq.insert(heap,-23,17);
        pq.insert(heap,35,18);
        pq.insert(heap,41,19);
        pq.insert(heap,-3,20);
        printHeap(heap);
        pq.increaseKey(heap,5,1);
        printHeap(heap);
        pq.deleteNode(heap,9);
        printHeap(heap);
        pq.extractMax(heap);
        printHeap(heap);

        pq.insert(heap1,101,21);
        pq.insert(heap1,200,22);
        pq.insert(heap1,300,23);
        pq.insert(heap1,400,24);
        pq.insert(heap1,500,25);
        pq.insert(heap1,600,26);
        pq.insert(heap1,700,27);
        pq.insert(heap1,800,28);
        pq.insert(heap1,900,29);
        pq.insert(heap1,1000,30);
        pq.extractMax(heap1);
        printHeap(heap1);
        heap2=pq.meld(heap,heap1);
        printHeap(heap2);
        pq.deleteNode(heap2,28);
        pq.extractMax(heap2);
        pq.extractMax(heap2);
        pq.extractMax(heap2);
        pq.extractMax(heap2);
        pq.extractMax(heap2);
        printHeap(heap2);
        cout<<"Max:("<<pq.findMax(heap2)->key<<","<<pq.findMax(heap2)->value<<")"<<endl;
        printHeap(heap1);
        cout<<"Passed"<<endl;
}


int main()
{
    test();
    return 0;

}


