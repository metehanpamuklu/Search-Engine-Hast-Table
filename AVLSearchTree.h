#include <string>
#include <iostream>
using namespace std;
string not_exist = "not here";
template <class Key, class Value>
struct AvlNode
{
    Key key;
    Value value;
    AvlNode* left;
    AvlNode* right;
    int        		height;

    AvlNode(const Key& theKey, const Value& theValue,
        AvlNode* lt, AvlNode* rt, int h = 0)
        : key(theKey), value(theValue), left(lt), right(rt), height(h) { }

};
//from this line all of the functions taken from lecture
template <class Key, class Value>
class AVLSearchTree {
private:
    const Key& elementAt(AvlNode<Key, Value>* t) const;
    AvlNode<Key, Value>* root;
    void insert(const Key& x, const Value& y, AvlNode<Key, Value>*& t) const;
    void remove(const Key& x, AvlNode<Key, Value>*& t) const;
    AvlNode<Key, Value>* findMin(AvlNode<Key, Value>* t) const;
    AvlNode<Key, Value>* findMax(AvlNode<Key, Value>* t) const;
    AvlNode<Key, Value>* find(const Key& x, AvlNode<Key, Value>* t) const;
    void makeEmpty(AvlNode<Key, Value>*& t) const;
    // Avl manipulations
    int height(AvlNode<Key, Value>* t) const;
    int max(int lhs, int rhs) const;
    void rotateWithLeftChild(AvlNode<Key, Value>*& k2) const;
    void rotateWithRightChild(AvlNode<Key, Value>*& k1) const;
    void doubleWithLeftChild(AvlNode<Key, Value>*& k3) const;
    void doubleWithRightChild(AvlNode<Key, Value>*& k1) const;
public:
    // Constructor
    AVLSearchTree() : root(nullptr) {}
    AVLSearchTree(const AVLSearchTree& rhs);
    ~AVLSearchTree();
    const Key& findMin() const;
    const Key& findMax() const;
    const Key& find(const Key& x) const;
    bool isEmpty() const;
    void makeEmpty();
    void insert(const Key& x, const Value& y);
    void remove(const Key& x);
    Value getValue(const Key& x);
};
template <class Key, class Value>
int AVLSearchTree<Key, Value>::height(AvlNode<Key, Value>* t)const
{
    if (t == NULL)
        return -1;
    return t->height;
}
template <class Key, class Value>
int AVLSearchTree<Key, Value>::max(int lhs, int rhs) const
{
    if (lhs > rhs)
        return lhs;

    return rhs;
}
template <class Key, class Value>
const Key& AVLSearchTree<Key, Value>::elementAt(AvlNode<Key, Value>* t) const
{
    if (t != NULL)
    {
        return t->key;
    }
    return not_exist;
}
template <class Key, class Value>
void AVLSearchTree<Key, Value>::insert(const Key& x, const Value& y, AvlNode<Key, Value>*& t) const
{
    if (t == NULL)
        t = new AvlNode<Key, Value>(x, y, NULL, NULL);

    else if (x < t->key) {
        // X should be inserted to the left tree!
        insert(x, y, t->left);

        // Check if the left tree is out of balance (left subtree grew in height!)
        if (height(t->left) - height(t->right) == 2)
            if (x < t->left->key)  // X was inserted to the left-left subtree!
                rotateWithLeftChild(t);
            else			     // X was inserted to the left-right subtree!
                doubleWithLeftChild(t);
    }
    else if (t->key < x)
    {    // Otherwise X is inserted to the right subtree
        insert(x, y, t->right);
        if (height(t->right) - height(t->left) == 2)
            // height of the right subtree increased
            if (t->right->key < x)
                // X was inserted to right-right subtree
                rotateWithRightChild(t);
            else // X was inserted to right-left subtree
                doubleWithRightChild(t);
    }
    else
        ;  // Duplicate; do nothing

  // update the height the node
    t->height = max(height(t->left), height(t->right)) + 1;
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::rotateWithLeftChild(AvlNode<Key, Value>*& k2) const
{
    AvlNode<Key, Value>* k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = max(height(k2->left), height(k2->right)) + 1;
    k1->height = max(height(k1->left), k2->height) + 1;
    k2 = k1;
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::rotateWithRightChild(AvlNode<Key, Value>*& k1) const
{
    AvlNode<Key, Value>* k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1->height = max(height(k1->left), height(k1->right)) + 1;
    k2->height = max(height(k2->right), k1->height) + 1;
    k1 = k2;
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::doubleWithLeftChild(AvlNode<Key, Value>*& k3) const
{
    rotateWithRightChild(k3->left);
    rotateWithLeftChild(k3);
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::doubleWithRightChild(AvlNode<Key, Value>*& k1) const
{
    rotateWithLeftChild(k1->right);
    rotateWithRightChild(k1);
}

template <class Key, class Value>
const Key& AVLSearchTree<Key, Value>::find(const Key& x) const
{
    return elementAt(find(x, root));
}

template <class Key, class Value>
const Key& AVLSearchTree<Key, Value>::findMin() const
{
    return elementAt(findMin(root));
}

template <class Key, class Value>
AvlNode<Key, Value>* AVLSearchTree<Key, Value>::findMin(AvlNode<Key, Value>* t) const
{
    if (t == NULL)
        return NULL;
    if (t->left == NULL)
        return t;
    return findMin(t->left);
}

template <class Key, class Value>
const Key& AVLSearchTree<Key, Value>::findMax() const
{
    return elementAt(findMax(root));
}

template <class Key, class Value>
AvlNode<Key, Value>* AVLSearchTree<Key, Value>::findMax(AvlNode<Key, Value>* t) const
{
    if (t != NULL)
        while (t->right != NULL)
            t = t->right;
    return t;

}

template <class Key, class Value>
AvlNode<Key, Value>* AVLSearchTree<Key, Value>::find(const Key& x, AvlNode<Key, Value>* t) const
{
    if (t == NULL)
        return NULL;
    else if (x < t->key)
        return find(x, t->left);
    else if (t->key < x)
        return find(x, t->right);
    else
        return t;    // Match
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::insert(const Key& x, const Value& y)
{
    insert(x, y, root);
}
template <class Key, class Value>
bool AVLSearchTree<Key, Value>::isEmpty() const
{
    return root == NULL;
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::makeEmpty()
{
    makeEmpty(root);
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::makeEmpty(AvlNode<Key, Value>*& t) const
{
    if (t != NULL)
    {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }
    t = NULL;
}

template <class Key, class Value>
AVLSearchTree<Key, Value>::~AVLSearchTree()
{
    makeEmpty();
}

template <class Key, class Value>
Value AVLSearchTree<Key, Value>::getValue(const Key& x)
{
    AvlNode<Key, Value>* result = find(x, root);
    if (result != nullptr) {
        return result->value; // Return the value associated with the key
    }
    else {
        return nullptr; // Key not found, return nullptr or throw an exception
    }
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::remove(const Key& x)
{
    remove(x, root);
}
template<class Key, class Value>
void AVLSearchTree<Key, Value>::remove(const Key& x, AvlNode<Key, Value>*& t)const {
    if (t == NULL)
        return;   // Item not found ; do nothing
    if (x < t->key) {
        remove(x, t->left);
    }
    else if (t->key < x) {
        remove(x, t->right);
    }
    else if (t->left != NULL && t->right != NULL) // Two children
    {
        t->key = findMax(t->left)->key;
        t->value = findMax(t->left)->value;
        remove(t->key, t->left);
    }
    else // one or no children
    {
        AvlNode<Key, Value>* oldNode = t;
        t = (t->left != NULL) ? t->left : t->right;
        delete oldNode;
    }
    if (t != NULL) {//just like adding a  new node
        t->height = max(height(t->left), height(t->right)) + 1;
        if (height(t->left) - height(t->right) == 2) {//deletin from right "probably no need because we always deleting from left side 
            if (t->left == NULL)
            {
                rotateWithLeftChild(t);
            }
            else if (x > t->right->key)//right of left
            {
                doubleWithLeftChild(t);
            }
            else //left of left
            {
                rotateWithLeftChild(t);
            }
        }
        if (height(t->right) - height(t->left) == 2) {//deletion from left
            if (t->left == NULL)
            {
                rotateWithRightChild(t);
            }
            else if (x > t->left->key)//right of left
            {
                rotateWithRightChild(t);
            }
            else //left of left
            {
                doubleWithRightChild(t);
            }
        }
    }
}
