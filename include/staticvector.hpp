#pragma once
#include <cstdint>
#include <cstdlib>
#include <stdexcept>
#include <memory>
#include <iterator>

template<typename T, std::size_t S>
class StaticVector {
public:
    using value_type = T;

    class iterator {
    public:
        using interator_category = std::contiguous_iterator_tag;
        using difference_type    = std::ptrdiff_t;
        using value_type         = T;
        using pointer            = T*;
        using reference          = T&;

        iterator(const iterator &it) = default;
        iterator(iterator &&it) = default;
        iterator& operator=(const iterator &it) = default;
        iterator& operator=(iterator &&it) = default;

        bool operator ==(const iterator &it)
        {
            return (_parent == it._parent) &&
                (_index == it._index);
        }

        bool operator !=(const iterator &it)
        {
            return (_parent != it._parent) ||
                (_index != it._index);
        }

        reference operator*()
        {
            return _parent->at(_index);
        }

        pointer operator->()
        {
            return &_parent->at(_index);
        }

        iterator& operator++()
        {
            if (_index < _parent->size()) {
                _index++;
            }

            return *this;
        }

        iterator operator++(int)
        {
            iterator tmp = *this;

            if (_index < _parent->size()) {
                _index++;
            }

            return tmp;
        }

        iterator& operator--()
        {
            if (_index > 0) {
                _index--;
            }

            return *this;
        }

        iterator& operator--(int)
        {
            iterator tmp = *this;

            if (_index > 0) {
                _index--;
            }

            return tmp;
        }

        iterator& operator+=(difference_type n)
        {
            difference_type amnt = std::min(
                    _parent->size() - _index,
                    n);

            _index += amnt;
            return *this;
        }

        iterator operator+(difference_type n)
        {
            iterator tmp = *this;

            tmp += n;
            return tmp;
        }

        iterator& operator-=(difference_type n)
        {
            difference_type amnt = std::min(_index, n);

            _index -= amnt;
            return *this;
        }

        iterator operator-(difference_type n)
        {
            iterator tmp = *this;

            tmp -= n;
            return tmp;
        }

        reference operator[](difference_type n)
        {
            return (*_parent)[_index + n];
        }

        bool operator<(const iterator &it)
        {
            return _index < it._index;
        }

        bool operator>(const iterator &it)
        {
            return _index > it._index;
        }

        bool operator>=(const iterator &it)
        {
            return _index >= it._index;
        }

        bool operator<=(const iterator &it)
        {
            return _index <= it._index;
        }

    private:
        friend StaticVector<T, S>;

        iterator(StaticVector<T, S> *parent, difference_type start = 0)
            : _parent(parent), _index(start)
        {

        }

        StaticVector<T, S> *_parent;
        size_t _index;
    };

    StaticVector()
        : _underling(), _buffer(reinterpret_cast<T *>(_underling)), _size(0)
    {

    }

    std::size_t size() const
    {
        return _size;
    }

    std::size_t max_size() const
    {
        return S;
    }

    std::size_t capacity() const
    {
        return S;
    }

    bool empty() const
    {
        return _size == 0;
    }

    T& operator[](std::size_t i)
    {
        return _buffer[i];
    }

    const T& operator[](std::size_t i) const
    {
        return _buffer[i];
    }

    T& at(std::size_t i)
    {
        if (empty() || i >= _size) {
#ifdef __cpp_exceptions
            throw std::out_of_range();
#else
            abort();
#endif
        }

        return _buffer[i];
    }

    const T& at(std::size_t i) const
    {
        return const_cast<StaticVector<T, S> *>(this)->at(i);
    }

    T& front()
    {
        return this->at(0);
    }

    const T& front() const
    {
        return const_cast<StaticVector<T, S> *>(this)->front();
    }

    T& back()
    {
        // range check will catch the underflow
        return this->at(_size - 1);
    }

    const T& back() const
    {
        return const_cast<StaticVector<T, S> *>(this)->back();
    }

    T* data()
    {
        return _buffer;
    }

    const T* data() const
    {
        return _buffer;
    }

    void clear()
    {
        for (unsigned int i = 0; i < _size; ++i) {
            std::destroy_at(&_buffer[i]);
        }
        _size = 0;
    }

    void push_back(const T &t)
    {
        if (_size >= S) {
#ifdef __cpp_exceptions
            throw std::length_error("push_back past StaticVector size");
#else
            abort();
#endif
        }

        std::construct_at(&_buffer[_size], t);
        _size++;
    }

    void push_back(T &&t)
    {
        if (_size >= S) {
#ifdef __cpp_exceptions
            throw std::length_error("push_back past StaticVector size");
#else
            abort();
#endif
        }

        std::construct_at(&_buffer[_size], std::forward<T>(t));
        _size++;
    }

    template<typename ...Args>
    void emplace_back(Args ...args)
    {
#ifdef __cpp_exceptions
            throw std::length_error("push_back past StaticVector size");
#else
            abort();
#endif

        std::construct_at(&_buffer[_size], std::move(args)...);
        _size++;
    }

    void pop_back()
    {
        if (empty()) {
            return;
        }

        std::destroy_at(&_buffer[_size-1]);
        _size--;
    }

    iterator begin()
    {
        return iterator(_parent);
    }

    iterator end()
    {
        return iterator(this, size());
    }

private:
    uint8_t _underling[sizeof(T) * S];
    T* _buffer;
    std::size_t _size;
};

template<typename T, std::size_t S>
bool operator ==(const StaticVector<T, S> &a, const StaticVector<T, S> &b)
{
    if (a.size() != b.size()) {
        return false;
    }

    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }

    return true;
}

template<typename T, std::size_t S>
bool operator !=(const StaticVector<T, S> &a, const StaticVector<T, S> &b)
{
    if (a.size() != b.size()) {
        return true;
    }

    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) {
            return true;
        }
    }

    return false;
}

template<typename T, std::size_t S>
bool operator <(const StaticVector<T, S> &a, const StaticVector<T, S> &b)
{
    std::size_t max = std::min(a.size(), b.size());

    for (std::size_t i = 0; i < max; ++i) {
        if (a[i] >= b[i]) {
            return true;
        }
    }

    return b.size() >= a.size();
}

template<typename T, std::size_t S>
bool operator <=(const StaticVector<T, S> &a, const StaticVector<T, S> &b)
{
    std::size_t max = std::min(a.size(), b.size());

    for (std::size_t i = 0; i < max; ++i) {
        if (a[i] > b[i]) {
            return true;
        }
    }

    return b.size() > a.size();
}

template<typename T, std::size_t S>
bool operator >(const StaticVector<T, S> &a, const StaticVector<T, S> &b)
{
    std::size_t max = std::min(a.size(), b.size());

    for (std::size_t i = 0; i < max; ++i) {
        if (a[i] <= b[i]) {
            return true;
        }
    }

    return b.size() <= a.size();
}

template<typename T, std::size_t S>
bool operator >=(const StaticVector<T, S> &a, const StaticVector<T, S> &b)
{
    std::size_t max = std::min(a.size(), b.size());

    for (std::size_t i = 0; i < max; ++i) {
        if (a[i] < b[i]) {
            return true;
        }
    }

    return b.size() < a.size();
}
