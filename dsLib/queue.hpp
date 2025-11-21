#ifndef DS_QUEUE_HPP
#define DS_QUEUE_HPP

#include <iostream>
#include <stdexcept>

namespace ds {

template <class dataType>
class Queue {
private:
    struct Node {
        dataType data;
        Node* next;
        Node(const dataType& val) : data(val), next(nullptr) {}
    };
    using nptr = Node*;
    nptr _m_front;
    nptr _m_rear;
    int _m_size;
    int _m_maxSize;

public:
    Queue();
    Queue(int maxSize);
    ~Queue();
    void enqueue(const dataType& val);
    void dequeue();
    dataType& front();
    const dataType& front() const;
    dataType& rear();
    const dataType& rear() const;
    int size() const;
    int maxSize() const;
    bool isEmpty() const;
    bool isFull() const;
    void clear();
    void display() const;
};

template <class dataType>
Queue<dataType>::Queue() : _m_front(nullptr), _m_rear(nullptr), _m_size(0), _m_maxSize(-1) {}

template <class dataType>
Queue<dataType>::Queue(int maxSize) 
    : _m_front(nullptr), _m_rear(nullptr), _m_size(0), _m_maxSize(maxSize) {}

template <class dataType>
Queue<dataType>::~Queue() {
    clear();
}

template <class dataType>
void Queue<dataType>::enqueue(const dataType& val) {
    if (_m_maxSize != -1 && _m_size >= _m_maxSize) {
        throw std::overflow_error("Queue is full");
    }
    nptr newNode = new Node(val);
    if (!_m_rear) {
        _m_front = _m_rear = newNode;
    } else {
        _m_rear->next = newNode;
        _m_rear = newNode;
    }
    _m_size++;
}

template <class dataType>
void Queue<dataType>::dequeue() {
    if (!_m_front) {
        throw std::underflow_error("Queue is empty");
    }
    nptr temp = _m_front;
    _m_front = _m_front->next;
    if (!_m_front) {
        _m_rear = nullptr;
    }
    delete temp;
    _m_size--;
}

template <class dataType>
dataType& Queue<dataType>::front() {
    if (!_m_front) {
        throw std::runtime_error("Queue is empty");
    }
    return _m_front->data;
}

template <class dataType>
const dataType& Queue<dataType>::front() const {
    if (!_m_front) {
        throw std::runtime_error("Queue is empty");
    }
    return _m_front->data;
}

template <class dataType>
dataType& Queue<dataType>::rear() {
    if (!_m_rear) {
        throw std::runtime_error("Queue is empty");
    }
    return _m_rear->data;
}

template <class dataType>
const dataType& Queue<dataType>::rear() const {
    if (!_m_rear) {
        throw std::runtime_error("Queue is empty");
    }
    return _m_rear->data;
}

template <class dataType>
int Queue<dataType>::size() const {
    return _m_size;
}

template <class dataType>
int Queue<dataType>::maxSize() const {
    return _m_maxSize;
}

template <class dataType>
bool Queue<dataType>::isEmpty() const {
    return _m_front == nullptr;
}

template <class dataType>
bool Queue<dataType>::isFull() const {
    if (_m_maxSize == -1) return false;
    return _m_size >= _m_maxSize;
}

template <class dataType>
void Queue<dataType>::clear() {
    while (_m_front) {
        nptr temp = _m_front;
        _m_front = _m_front->next;
        delete temp;
    }
    _m_rear = nullptr;
    _m_size = 0;
}

template <class dataType>
void Queue<dataType>::display() const {
    nptr current = _m_front;
    std::cout << "Front -> ";
    while (current) {
        std::cout << current->data << " -> ";
        current = current->next;
    }
    std::cout << "Rear\n";
}

}

#endif