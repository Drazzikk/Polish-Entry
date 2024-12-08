#pragma once

#include <iostream>
#include <stdexcept>
#include <algorithm>


template<typename T>
class TVector {
protected:
    size_t sz;
    size_t cap;
    T* pMem;

public:
    TVector(size_t size = 0) : sz(size), cap(size * 2 + 2) {
        pMem = new T[cap];
        if (!pMem) throw std::domain_error("domain_error");
    }

    TVector(T* arr, size_t size) : sz(size), cap(size * 2 + 2) {
        if (!arr) throw std::domain_error("domain_error");
        pMem = new T[cap];
        if (!pMem) throw std::domain_error("domain_error");
        std::copy(arr, arr + sz, pMem);
    }

    TVector(const TVector& v) : sz(v.sz), cap(v.cap) {
        pMem = new T[cap];
        if (!pMem) throw std::domain_error("domain_error");
        std::copy(v.pMem, v.pMem + sz, pMem);
    }

    TVector(TVector&& v) noexcept : sz(v.sz), cap(v.cap), pMem(v.pMem) {
        v.sz = 0;
        v.cap = 0;
        v.pMem = nullptr;
    }

    ~TVector() {
        delete[] pMem;
        pMem = nullptr;
    }

    TVector& operator=(const TVector& v) {
        if (this != &v) {
            delete[] pMem;
            sz = v.sz;
            cap = v.cap;
            pMem = new T[cap];
            if (!pMem) throw std::domain_error("domain_error");
            std::copy(v.pMem, v.pMem + sz, pMem);
        }
        return *this;
    }

    TVector& operator=(TVector&& v) noexcept {
        if (this != &v) {
            delete[] pMem;
            sz = v.sz;
            cap = v.cap;
            pMem = v.pMem;
            v.sz = 0;
            v.cap = 0;
            v.pMem = nullptr;
        }
        return *this;
    }

    size_t size() const noexcept 
    { 
        return sz; 
    }
    size_t getCapacity() const noexcept 
    { 
        return cap; 
    }
    bool Full() const noexcept 
    { 
        return (sz == cap); 
    }
    bool Empty() const noexcept 
    { 
        return (sz == 0); 
    }

    T& operator[](size_t i) {
        return pMem[i];
    }

    const T& operator[](size_t i) const {
        return pMem[i];
    }

    bool operator==(const TVector& v) const noexcept {
        if (sz != v.sz) return false;
        for (size_t i = 0; i < sz; i++)
            if (pMem[i] != v.pMem[i]) return false;
        return true;
    }

    bool operator!=(const TVector& v) const noexcept {
        return !(*this == v);
    }

    void resize(size_t size) {
        if (size > (SIZE_MAX / sizeof(T) - 2) / 2) { 
            throw std::domain_error("domain_error");
        }

        try {
            T* p = new T[size * 2 + 2]; 
            std::copy(pMem, pMem + sz, p);
            delete[] pMem;
            pMem = p;
            cap = size * 2 + 2;
        }
        catch (const std::bad_alloc&) {
            throw std::domain_error("domain_error");
        }
    }

    void push_back(T elem) {
        if (Full()) resize(sz + 1);
        pMem[sz++] = elem;
    }
    
    void pop_back() {
        if (sz == 0) throw std::domain_error("domain_error");
        --sz;
    }

    void push_front(T elem) {
        if (Full()) resize(sz + 1);
        size_t i;
        for (i = sz; i > 0; --i)
            pMem[i] = pMem[i - 1];

        pMem[i] = elem;
        ++sz;
    }

    void pop_front() {
        if (0 >= sz || sz == 0)
            throw std::domain_error("domain_error");

        for (size_t i = 0; i < sz - 1; ++i)
            pMem[i] = pMem[i + 1];
        --sz;
    }

    
};

template<typename T>
class Stack : private TVector<T> {

public:
    using TVector<T>::Full;
    using TVector<T>::Empty;
    using TVector<T>::size;
    using TVector<T>::getCapacity;

    Stack() : TVector<T>(0) {}

    void push(T elem) {
        this->push_back(elem);
    }

    void pop() {
        this->pop_back();
    }

    T top() const {
        if (Empty()) throw std::domain_error("domain_error");
        return this->pMem[this->sz - 1];
    }
};

template<typename T>
class Queue : private TVector<T> {
private:
    using TVector<T>::cap;
    using TVector<T>::pMem;
    size_t front;
    size_t back;

public:
    using TVector<T>::Full;
    using TVector<T>::Empty;
    using TVector<T>::size;
    using TVector<T>::getCapacity;

    Queue() : TVector<T>(0), front(0), back(0) {}

    void push(T elem) {
        this->push_back(elem);
        if (back == cap)
            back = 0;
        else
            back++;
    }

    void pop() {
        if (Empty()) throw std::domain_error("domain_error");
        ++front;

        if (front >= this->cap)
            front = 0;

        --this->sz;
    }

    T getFront() const noexcept {
        return this->pMem[front];
    }

    T getBack() const noexcept {
        if (back == 0)
            return pMem[cap - 1];
        return pMem[back - 1];
    }

};

template<typename T>
class QueueOnStacks : private TVector<Stack<T>> {
    size_t sz;

public:
    QueueOnStacks() : TVector<Stack<T>>(2), sz(0) {}

    size_t size() const noexcept { return sz; }
    bool Full() const noexcept { return (this->pMem[0].Full() && this->pMem[1].Full()); }
    bool Empty() const noexcept { return (this->pMem[0].Empty() && this->pMem[1].Empty()); }

    void pop() {
        if (this->pMem[0].Empty()) {
            while (!this->pMem[1].Empty()) {
                T tmp = this->pMem[1].top();
                this->pMem[0].push(tmp);
                this->pMem[1].pop();
            }
        }

        if (!this->pMem[0].Empty())
        {
            this->pMem[0].pop();
            sz--;
        }
    }

    void push(T elem) {
        this->pMem[1].push(elem);
        sz++;
    }

    T front() const {
        if (this->pMem[0].Empty() && this->pMem[1].Empty()) {
            throw std::domain_error("domain_error");
        }

        if (this->pMem[0].Empty()) {
            while (!this->pMem[1].Empty()) {
                T tmp = this->pMem[1].top();
                this->pMem[0].push(tmp);
                this->pMem[1].pop();
            }
        }

        return this->pMem[0].top(); 
    }

    T back() const {
        if (this->pMem[1].Empty() && this->pMem[0].Empty()) {
            throw std::domain_error("domain_error");
        }

        if (!this->pMem[1].Empty()) {
            return this->pMem[1].top(); 
        }
        else {
            
            while (!this->pMem[0].Empty()) {
                T tmp = this->pMem[0].top();
                this->pMem[1].push(tmp);
                this->pMem[0].pop();
            }
            return this->pMem[1].top(); 
        }
    }
};
