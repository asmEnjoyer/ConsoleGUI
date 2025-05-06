#pragma once
#include <stddef.h>
#include <stdexcept>

template <typename T>
struct queueNode
{
    queueNode<T> *pNext = nullptr;
    T value;
    queueNode(T value) : value(value) {}
};

template <typename T>
class queue
{
public:
    struct iterator
    {
    public:
        iterator(queueNode<T> *ptr) : _ptr(ptr) {}
        iterator operator++()
        {
            _ptr = _ptr->pNext;
            return *this;
        }
        iterator operator++(T)
        {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        bool operator==(const iterator &other) { return this->_ptr == other._ptr; }
        bool operator!=(const iterator &other) { return this->_ptr != other._ptr; }
        T &operator*() { return _ptr->value; }

    private:
        queueNode<T> *_ptr;
    };

private:
    queueNode<T> *_pFront = nullptr;
    queueNode<T> *_pBack = nullptr;
    size_t _size = 0;

public:
    size_t size() { return _size; }
    void push(T value)
    {
        if (_size == 0)
        {
            _pBack = new queueNode<T>(value);
            _pFront = _pBack;
        }
        else
        {
            _pBack->pNext = new queueNode<T>(value);
            _pBack = _pBack->pNext;
        }
        ++_size;
    }
    T pop()
    {
        if (_size == 0)
            throw std::out_of_range("");
        T value = _pFront->value;
        queueNode<T> *toDelete = _pFront;
        _pFront = _pFront->pNext;
        delete toDelete;
        --_size;
        return value;
    }
    T front() { return _pFront->value; }
    T back() { return _pBack->value; }

    iterator begin() { return iterator(_pFront); }
    iterator end() { return iterator(nullptr); }
};
