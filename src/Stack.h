#pragma once

template<typename T>
class Stack {
public:
    Stack()
        : data_(nullptr), capacity_(0), count_(0) {
        reserve(4); // dung lượng khởi đầu
    }

    ~Stack() {
        delete[] data_;
    }

    // Copy constructor
    Stack(const Stack& other)
        : data_(nullptr), capacity_(0), count_(0) {
        reserve(other.capacity_);
        for (int i = 0; i < other.count_; ++i)
            data_[i] = other.data_[i];
        count_ = other.count_;
    }
    Stack& operator=(const Stack& other) {
        if (this != &other) {
            delete[] data_;
            data_ = nullptr;
            capacity_ = count_ = 0;
            reserve(other.capacity_);
            for (int i = 0; i < other.count_; ++i)
                data_[i] = other.data_[i];
            count_ = other.count_;
        }
        return *this;
    }
    void push(const T& value) {
        if (count_ >= capacity_)
            reserve(capacity_ * 2);
        data_[count_++] = value;
    }
    void pop() {
        if (count_ > 0)
            --count_;
    }
    T& top() {
        // Nếu stack rỗng, trả về phần tử mặc định tĩnh
        static T dummy{};
        if (count_ == 0)
            return dummy;
        return data_[count_ - 1];
    }

    const T& top() const {
        static T dummy{};
        if (count_ == 0)
            return dummy;
        return data_[count_ - 1];
    }
    bool empty() const { return count_ == 0; }
    int size() const { return count_; }
    void clear() { count_ = 0; }

private:
    void reserve(int new_capacity) {
        if (new_capacity <= capacity_)
            return;
        T* new_data = new T[new_capacity];
        for (int i = 0; i < count_; ++i)
            new_data[i] = data_[i];
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }

    T* data_;
    int capacity_;
    int count_;
};
