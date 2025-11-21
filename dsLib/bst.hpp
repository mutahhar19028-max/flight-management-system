#ifndef DS_BST_HPP
#define DS_BST_HPP

#include <iostream>
#include <stdexcept>
#include <vector>
#include <functional>

namespace ds {

template <class dataType, typename Compare = std::less<dataType>>
class BST {
private:
    struct Node {
        dataType data;
        Node* left;
        Node* right;
        Node(const dataType& val) : data(val), left(nullptr), right(nullptr) {}
    };
    using nptr = Node*;
    nptr _m_root;
    Compare _m_cmp;

private:
    nptr _insert(nptr node, const dataType& val);
    nptr _search(nptr node, const dataType& val) const;
    nptr _remove(nptr node, const dataType& val);
    nptr _findMin(nptr node) const;
    void _inorder(nptr node) const;
    void _clear(nptr node);
    void _getAllNodes(nptr node, std::vector<dataType>& out) const;
    
    template <typename Predicate>
    dataType* _find(nptr node, Predicate pred);
    
    template <typename Func>
    void _inorderTraverse(nptr node, Func f) const;
    
    template <typename Func>
    void _inorderTraverseNonConst(nptr node, Func f);

public:
    BST();
    ~BST();
    void insert(const dataType& val);
    nptr search(const dataType& val) const;
    void remove(const dataType& val);
    void inorder() const;
    void clear();
    bool isEmpty() const;
    void getAllNodes(std::vector<dataType>& out) const;
    
    template <typename Predicate>
    dataType* find(Predicate pred);
    
    template <typename Func>
    void inorderTraverse(Func f) const;
    
    template <typename Func>
    void inorderTraverseNonConst(Func f);
};

template <class dataType, typename Compare>
BST<dataType, Compare>::BST() : _m_root(nullptr), _m_cmp(Compare()) {}

template <class dataType, typename Compare>
BST<dataType, Compare>::~BST() {
    clear();
}

template <class dataType, typename Compare>
typename BST<dataType, Compare>::nptr 
BST<dataType, Compare>::_insert(nptr node, const dataType& val) {
    if (!node) return new Node(val);
    if (_m_cmp(val, node->data))
        node->left = _insert(node->left, val);
    else
        node->right = _insert(node->right, val);
    return node;
}

template <class dataType, typename Compare>
void BST<dataType, Compare>::insert(const dataType& val) {
    _m_root = _insert(_m_root, val);
}

template <class dataType, typename Compare>
typename BST<dataType, Compare>::nptr 
BST<dataType, Compare>::_search(nptr node, const dataType& val) const {
    if (!node) return nullptr;
    if (node->data == val) return node;
    if (_m_cmp(val, node->data))
        return _search(node->left, val);
    return _search(node->right, val);
}

template <class dataType, typename Compare>
typename BST<dataType, Compare>::nptr 
BST<dataType, Compare>::search(const dataType& val) const {
    return _search(_m_root, val);
}

template <class dataType, typename Compare>
typename BST<dataType, Compare>::nptr 
BST<dataType, Compare>::_findMin(nptr node) const {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

template <class dataType, typename Compare>
typename BST<dataType, Compare>::nptr 
BST<dataType, Compare>::_remove(nptr node, const dataType& val) {
    if (!node) return nullptr;
    
    if (_m_cmp(val, node->data)) {
        node->left = _remove(node->left, val);
    } else if (_m_cmp(node->data, val)) {
        node->right = _remove(node->right, val);
    } else {
        if (!node->left && !node->right) {
            delete node;
            return nullptr;
        } else if (!node->left) {
            nptr temp = node->right;
            delete node;
            return temp;
        } else if (!node->right) {
            nptr temp = node->left;
            delete node;
            return temp;
        } else {
            nptr minNode = _findMin(node->right);
            node->data = minNode->data;
            node->right = _remove(node->right, minNode->data);
        }
    }
    return node;
}

template <class dataType, typename Compare>
void BST<dataType, Compare>::remove(const dataType& val) {
    _m_root = _remove(_m_root, val);
}

template <class dataType, typename Compare>
void BST<dataType, Compare>::_inorder(nptr node) const {
    if (!node) return;
    _inorder(node->left);
    std::cout << node->data << " ";
    _inorder(node->right);
}

template <class dataType, typename Compare>
void BST<dataType, Compare>::inorder() const {
    _inorder(_m_root);
    std::cout << "\n";
}

template <class dataType, typename Compare>
void BST<dataType, Compare>::_clear(nptr node) {
    if (!node) return;
    _clear(node->left);
    _clear(node->right);
    delete node;
}

template <class dataType, typename Compare>
void BST<dataType, Compare>::clear() {
    _clear(_m_root);
    _m_root = nullptr;
}

template <class dataType, typename Compare>
bool BST<dataType, Compare>::isEmpty() const {
    return _m_root == nullptr;
}

template <class dataType, typename Compare>
void BST<dataType, Compare>::_getAllNodes(nptr node, std::vector<dataType>& out) const {
    if (!node) return;
    _getAllNodes(node->left, out);
    out.push_back(node->data);
    _getAllNodes(node->right, out);
}

template <class dataType, typename Compare>
void BST<dataType, Compare>::getAllNodes(std::vector<dataType>& out) const {
    _getAllNodes(_m_root, out);
}

template <class dataType, typename Compare>
template <typename Predicate>
dataType* BST<dataType, Compare>::_find(nptr node, Predicate pred) {
    if (!node) return nullptr;
    if (pred(node->data))
        return &(node->data);
    if (auto left = _find(node->left, pred))
        return left;
    return _find(node->right, pred);
}

template <class dataType, typename Compare>
template <typename Predicate>
dataType* BST<dataType, Compare>::find(Predicate pred) {
    return _find(_m_root, pred);
}

template <class dataType, typename Compare>
template <typename Func>
void BST<dataType, Compare>::_inorderTraverse(nptr node, Func f) const {
    if (!node) return;
    _inorderTraverse(node->left, f);
    f(node->data);
    _inorderTraverse(node->right, f);
}

template <class dataType, typename Compare>
template <typename Func>
void BST<dataType, Compare>::inorderTraverse(Func f) const {
    _inorderTraverse(_m_root, f);
}

template <class dataType, typename Compare>
template <typename Func>
void BST<dataType, Compare>::_inorderTraverseNonConst(nptr node, Func f) {
    if (!node) return;
    _inorderTraverseNonConst(node->left, f);
    f(node->data);
    _inorderTraverseNonConst(node->right, f);
}

template <class dataType, typename Compare>
template <typename Func>
void BST<dataType, Compare>::inorderTraverseNonConst(Func f) {
    _inorderTraverseNonConst(_m_root, f);
}

}

#endif