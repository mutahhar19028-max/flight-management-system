#ifndef DS_LINKEDLIST_HPP
#define DS_LINKEDLIST_HPP

#include <iostream>
#include <stdexcept>
#include <functional>

namespace ds {

template <class dataType>
class LinkedList {
private:
    struct Node {
        dataType data;
        Node* next;
        Node(const dataType& val) : data(val), next(nullptr) {}
    };
    using nptr = Node*;
    nptr _m_head;
    int _m_size;

private:
    nptr _getNodeAt(int index) const;

public:
    LinkedList();
    ~LinkedList();
    void insertAtHead(const dataType& val);
    void insertAtTail(const dataType& val);
    void insertAt(int index, const dataType& val);
    void removeAtHead();
    void removeAtTail();
    void removeAt(int index);
    dataType& getAt(int index);
    const dataType& getAt(int index) const;
    int size() const;
    bool isEmpty() const;
    void clear();
    void display() const;
    
    template <typename Predicate>
    dataType* find(Predicate pred);
    
    template <typename Func>
    void traverse(Func f) const;
    
    template <typename Func>
    void traverseNonConst(Func f);
};

template <class dataType>
LinkedList<dataType>::LinkedList() : _m_head(nullptr), _m_size(0) {}

template <class dataType>
LinkedList<dataType>::~LinkedList() {
    clear();
}

template <class dataType>
typename LinkedList<dataType>::nptr 
LinkedList<dataType>::_getNodeAt(int index) const {
    if (index < 0 || index >= _m_size) {
        throw std::out_of_range("Index out of range");
    }
    nptr current = _m_head;
    for (int i = 0; i < index; ++i) {
        current = current->next;
    }
    return current;
}

template <class dataType>
void LinkedList<dataType>::insertAtHead(const dataType& val) {
    nptr newNode = new Node(val);
    newNode->next = _m_head;
    _m_head = newNode;
    _m_size++;
}

template <class dataType>
void LinkedList<dataType>::insertAtTail(const dataType& val) {
    nptr newNode = new Node(val);
    if (!_m_head) {
        _m_head = newNode;
    } else {
        nptr current = _m_head;
        while (current->next) {
            current = current->next;
        }
        current->next = newNode;
    }
    _m_size++;
}

template <class dataType>
void LinkedList<dataType>::insertAt(int index, const dataType& val) {
    if (index < 0 || index > _m_size) {
        throw std::out_of_range("Index out of range");
    }
    if (index == 0) {
        insertAtHead(val);
        return;
    }
    nptr newNode = new Node(val);
    nptr prev = _getNodeAt(index - 1);
    newNode->next = prev->next;
    prev->next = newNode;
    _m_size++;
}

template <class dataType>
void LinkedList<dataType>::removeAtHead() {
    if (!_m_head) {
        throw std::runtime_error("List is empty");
    }
    nptr temp = _m_head;
    _m_head = _m_head->next;
    delete temp;
    _m_size--;
}

template <class dataType>
void LinkedList<dataType>::removeAtTail() {
    if (!_m_head) {
        throw std::runtime_error("List is empty");
    }
    if (!_m_head->next) {
        delete _m_head;
        _m_head = nullptr;
        _m_size--;
        return;
    }
    nptr current = _m_head;
    while (current->next->next) {
        current = current->next;
    }
    delete current->next;
    current->next = nullptr;
    _m_size--;
}

template <class dataType>
void LinkedList<dataType>::removeAt(int index) {
    if (index < 0 || index >= _m_size) {
        throw std::out_of_range("Index out of range");
    }
    if (index == 0) {
        removeAtHead();
        return;
    }
    nptr prev = _getNodeAt(index - 1);
    nptr toDelete = prev->next;
    prev->next = toDelete->next;
    delete toDelete;
    _m_size--;
}

template <class dataType>
dataType& LinkedList<dataType>::getAt(int index) {
    return _getNodeAt(index)->data;
}

template <class dataType>
const dataType& LinkedList<dataType>::getAt(int index) const {
    return _getNodeAt(index)->data;
}

template <class dataType>
int LinkedList<dataType>::size() const {
    return _m_size;
}

template <class dataType>
bool LinkedList<dataType>::isEmpty() const {
    return _m_head == nullptr;
}

template <class dataType>
void LinkedList<dataType>::clear() {
    while (_m_head) {
        nptr temp = _m_head;
        _m_head = _m_head->next;
        delete temp;
    }
    _m_size = 0;
}

template <class dataType>
void LinkedList<dataType>::display() const {
    nptr current = _m_head;
    while (current) {
        std::cout << current->data << " -> ";
        current = current->next;
    }
    std::cout << "nullptr\n";
}

template <class dataType>
template <typename Predicate>
dataType* LinkedList<dataType>::find(Predicate pred) {
    nptr current = _m_head;
    while (current) {
        if (pred(current->data)) {
            return &(current->data);
        }
        current = current->next;
    }
    return nullptr;
}

template <class dataType>
template <typename Func>
void LinkedList<dataType>::traverse(Func f) const {
    nptr current = _m_head;
    while (current) {
        f(current->data);
        current = current->next;
    }
}

template <class dataType>
template <typename Func>
void LinkedList<dataType>::traverseNonConst(Func f) {
    nptr current = _m_head;
    while (current) {
        f(current->data);
        current = current->next;
    }
}

}

#endif