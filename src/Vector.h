#pragma once
#include <cstdlib>   
#include <new>      
#include <utility>   

template<typename T>
class Vector {
private:
    T* data;
    size_t _size;
    size_t _capacity;

    void reallocate(size_t newCap) {
        T* newData = (T*)::operator new(sizeof(T) * newCap);
        for (size_t i = 0; i < _size; ++i) {
            new (newData + i) T(std::move(data[i]));
            data[i].~T();
        }
        ::operator delete(data);
        data = newData;
        _capacity = newCap;
    }

public:
    Vector() : data(nullptr), _size(0), _capacity(0) {}

    ~Vector() {
        clear();
        ::operator delete(data);
    }

    Vector(const Vector& other) : data(nullptr), _size(0), _capacity(0) {
        reserve(other._size);
        for (size_t i = 0; i < other._size; ++i)
            push_back(other.data[i]);
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            clear();
            reserve(other._size);
            for (size_t i = 0; i < other._size; ++i)
                push_back(other.data[i]);
        }
        return *this;
    }

    Vector(Vector&& other) noexcept
        : data(other.data), _size(other._size), _capacity(other._capacity) {
        other.data = nullptr;
        other._size = 0;
        other._capacity = 0;
    }
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            clear();
            ::operator delete(data);
            data = other.data;
            _size = other._size;
            _capacity = other._capacity;
            other.data = nullptr;
            other._size = 0;
            other._capacity = 0;
        }
        return *this;
    }

    void push_back(const T& value) {
        if (_size >= _capacity)
            reserve(_capacity == 0 ? 2 : _capacity * 2);
        new (data + _size) T(value);
        ++_size;
    }
    void push_back(T&& value) {
        if (_size >= _capacity)
            reserve(_capacity == 0 ? 2 : _capacity * 2);
        new (data + _size) T(std::move(value));
        ++_size;
    }
    void pop_back() {
        if (_size > 0) {
            --_size;
            data[_size].~T();
        }
    }

    void clear() {
        for (size_t i = 0; i < _size; ++i)
            data[i].~T();
        _size = 0;
    }

    void reserve(size_t newCap) {
        if (newCap > _capacity)
            reallocate(newCap);
    }
    size_t size() const { return _size; }
    size_t capacity() const { return _capacity; }
    bool empty() const { return _size == 0; }

    T& operator[](size_t i) { return data[i]; }
    const T& operator[](size_t i) const { return data[i]; }
    T* begin() { return data; }
    T* end() { return data + _size; }
    const T* begin() const { return data; }
    const T* end() const { return data + _size; }
};
