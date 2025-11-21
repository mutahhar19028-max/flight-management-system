#ifndef DS_DOUBLYLINKEDLIST_HPP
#define DS_DOUBLYLINKEDLIST_HPP

#include <iostream>
#include <stdexcept>
#include <functional>

namespace ds {

template <class dataType>
class DoublyLinkedList {
private:
    struct Node {
        dataType data;
        Node* prev;
        Node* next;
        Node(const dataType& val) : data(val), prev(nullptr), next(nullptr) {}
    };
    using nptr = Node*;
    nptr _m_head;
    nptr _m_tail;
    int _m_size;

private:
    nptr _getNodeAt(int index) const;

public:
    DoublyLinkedList();
    ~DoublyLinkedList();
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
    void displayReverse() const;
    
    template <typename Predicate>
    dataType* find(Predicate pred);
    
    template <typename Func>
    void traverse(Func f) const;
    
    template <typename Func>
    void traverseNonConst(Func f);
};

template <class dataType>
DoublyLinkedList<dataType>::DoublyLinkedList() 
    : _m_head(nullptr), _m_tail(nullptr), _m_size(0) {}

template <class dataType>
DoublyLinkedList<dataType>::~DoublyLinkedList() {
    clear();
}

template <class dataType>
typename DoublyLinkedList<dataType>::nptr 
DoublyLinkedList<dataType>::_getNodeAt(int index) const {
    if (index < 0 || index >= _m_size) {
        throw std::out_of_range("Index out of range");
    }
    nptr current;
    if (index < _m_size / 2) {
        current = _m_head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
    } else {
        current = _m_tail;
        for (int i = _m_size - 1; i > index; --i) {
            current = current->prev;
        }
    }
    return current;
}

template <class dataType>
void DoublyLinkedList<dataType>::insertAtHead(const dataType& val) {
    nptr newNode = new Node(val);
    if (!_m_head) {
        _m_head = _m_tail = newNode;
    } else {
        newNode->next = _m_head;
        _m_head->prev = newNode;
        _m_head = newNode;
    }
    _m_size++;
}

template <class dataType>
void DoublyLinkedList<dataType>::insertAtTail(const dataType& val) {
    nptr newNode = new Node(val);
    if (!_m_tail) {
        _m_head = _m_tail = newNode;
    } else {
        newNode->prev = _m_tail;
        _m_tail->next = newNode;
        _m_tail = newNode;
    }
    _m_size++;
}

template <class dataType>
void DoublyLinkedList<dataType>::insertAt(int index, const dataType& val) {
    if (index < 0 || index > _m_size) {
        throw std::out_of_range("Index out of range");
    }
    if (index == 0) {
        insertAtHead(val);
        return;
    }
    if (index == _m_size) {
        insertAtTail(val);
        return;
    }
    nptr newNode = new Node(val);
    nptr current = _getNodeAt(index);
    newNode->prev = current->prev;
    newNode->next = current;
    current->prev->next = newNode;
    current->prev = newNode;
    _m_size++;
}

template <class dataType>
void DoublyLinkedList<dataType>::removeAtHead() {
    if (!_m_head) {
        throw std::runtime_error("List is empty");
    }
    nptr temp = _m_head;
    if (_m_head == _m_tail) {
        _m_head = _m_tail = nullptr;
    } else {
        _m_head = _m_head->next;
        _m_head->prev = nullptr;
    }
    delete temp;
    _m_size--;
}

template <class dataType>
void DoublyLinkedList<dataType>::removeAtTail() {
    if (!_m_tail) {
        throw std::runtime_error("List is empty");
    }
    nptr temp = _m_tail;
    if (_m_head == _m_tail) {
        _m_head = _m_tail = nullptr;
    } else {
        _m_tail = _m_tail->prev;
        _m_tail->next = nullptr;
    }
    delete temp;
    _m_size--;
}

template <class dataType>
void DoublyLinkedList<dataType>::removeAt(int index) {
    if (index < 0 || index >= _m_size) {
        throw std::out_of_range("Index out of range");
    }
    if (index == 0) {
        removeAtHead();
        return;
    }
    if (index == _m_size - 1) {
        removeAtTail();
        return;
    }
    nptr toDelete = _getNodeAt(index);
    toDelete->prev->next = toDelete->next;
    toDelete->next->prev = toDelete->prev;
    delete toDelete;
    _m_size--;
}

template <class dataType>
dataType& DoublyLinkedList<dataType>::getAt(int index) {
    return _getNodeAt(index)->data;
}

template <class dataType>
const dataType& DoublyLinkedList<dataType>::getAt(int index) const {
    return _getNodeAt(index)->data;
}

template <class dataType>
int DoublyLinkedList<dataType>::size() const {
    return _m_size;
}

template <class dataType>
bool DoublyLinkedList<dataType>::isEmpty() const {
    return _m_head == nullptr;
}

template <class dataType>
void DoublyLinkedList<dataType>::clear() {
    while (_m_head) {
        nptr temp = _m_head;
        _m_head = _m_head->next;
        delete temp;
    }
    _m_tail = nullptr;
    _m_size = 0;
}

template <class dataType>
void DoublyLinkedList<dataType>::display() const {
    nptr current = _m_head;
    std::cout << "nullptr <-> ";
    while (current) {
        std::cout << current->data << " <-> ";
        current = current->next;
    }
    std::cout << "nullptr\n";
}

template <class dataType>
void DoublyLinkedList<dataType>::displayReverse() const {
    nptr current = _m_tail;
    std::cout << "nullptr <-> ";
    while (current) {
        std::cout << current->data << " <-> ";
        current = current->prev;
    }
    std::cout << "nullptr\n";
}

template <class dataType>
template <typename Predicate>
dataType* DoublyLinkedList<dataType>::find(Predicate pred) {
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
void DoublyLinkedList<dataType>::traverse(Func f) const {
    nptr current = _m_head;
    while (current) {
        f(current->data);
        current = current->next;
    }
}

template <class dataType>
template <typename Func>
void DoublyLinkedList<dataType>::traverseNonConst(Func f) {
    nptr current = _m_head;
    while (current) {
        f(current->data);
        current = current->next;
    }
}

}

#endif