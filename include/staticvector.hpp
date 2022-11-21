#pragma once
#include <cstdint>
#include <cstdlib>
#include <stdexcept>
#include <memory>
#include <iterator>

#ifdef STATICVEC_SUPPORT_IOSTREAM
#include <ostream>
#endif

template<typename T, std::size_t S>
class StaticVector {
public:
    using value_type = T;

    class const_iterator;

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

        bool operator ==(const iterator &it) const
        {
            return (_parent == it._parent) &&
                (_index == it._index);
        }

        bool operator !=(const iterator &it) const
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
            if (n < 0) {
                return (*this) -= -n;
            }

            difference_type amnt = std::min(
                    _parent->size() - _index,
                    static_cast<std::size_t>(n));

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
            if (n < 0) {
                return (*this) += -n;
            }

            difference_type amnt = std::min(_index,
                    static_cast<std::size_t>(n));

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

        bool operator<(const iterator &it) const
        {
            return _index < it._index;
        }

        bool operator>(const iterator &it) const
        {
            return _index > it._index;
        }

        bool operator>=(const iterator &it) const
        {
            return _index >= it._index;
        }

        bool operator<=(const iterator &it) const
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

    class const_iterator {
    public:
        using interator_category = std::contiguous_iterator_tag;
        using difference_type    = std::ptrdiff_t;
        using value_type         = const T;
        using pointer            = const T*;
        using reference          = const T&;

        const_iterator(const const_iterator &it) = default;
        const_iterator(const_iterator &&it) = default;

        const_iterator(const iterator &it)
            : _parent(it._parent), _index(it._index)
        {

        }

        const_iterator& operator=(const const_iterator &it) = default;
        const_iterator& operator=(const_iterator &&it) = default;

        bool operator ==(const const_iterator &it) const
        {
            return (_parent == it._parent) &&
                (_index == it._index);
        }

        bool operator !=(const const_iterator &it) const
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

        const_iterator& operator++()
        {
            if (_index < _parent->size()) {
                _index++;
            }

            return *this;
        }

        const_iterator operator++(int)
        {
            iterator tmp = *this;

            if (_index < _parent->size()) {
                _index++;
            }

            return tmp;
        }

        const_iterator& operator--()
        {
            if (_index > 0) {
                _index--;
            }

            return *this;
        }

        const_iterator& operator--(int)
        {
            const_iterator tmp = *this;

            if (_index > 0) {
                _index--;
            }

            return tmp;
        }

        const_iterator& operator+=(difference_type n)
        {
            if (n < 0) {
                return (*this) -= -n;
            }

            difference_type amnt = std::min(
                    _parent->size() - _index,
                    static_cast<std::size_t>(n));

            _index += amnt;
            return *this;
        }

        const_iterator operator+(difference_type n)
        {
            const_iterator tmp = *this;

            tmp += n;
            return tmp;
        }

        const_iterator& operator-=(difference_type n)
        {
            if (n < 0) {
                return (*this) += -n;
            }

            difference_type amnt = std::min(_index,
                    static_cast<std::size_t>(n));

            _index -= amnt;
            return *this;
        }

        const_iterator operator-(difference_type n)
        {
            const_iterator tmp = *this;

            tmp -= n;
            return tmp;
        }

        reference operator[](difference_type n)
        {
            return (*_parent)[_index + n];
        }

        bool operator<(const const_iterator &it) const
        {
            return _index < it._index;
        }

        bool operator>(const const_iterator &it) const
        {
            return _index > it._index;
        }

        bool operator>=(const const_iterator &it) const
        {
            return _index >= it._index;
        }

        bool operator<=(const const_iterator &it) const
        {
            return _index <= it._index;
        }

    private:
        friend StaticVector<T, S>;

        const_iterator(const StaticVector<T, S> *parent, difference_type start = 0)
            : _parent(parent), _index(start)
        {

        }

        const StaticVector<T, S> *_parent;
        size_t _index;
    };

    StaticVector()
        : _underling(), _buffer(reinterpret_cast<T *>(_underling)), _size(0)
    {

    }

    StaticVector(const StaticVector<T, S> &other)
        : StaticVector()
    {
        *this = other;
    }

    StaticVector(StaticVector<T, S> &&other)
        : StaticVector()
    {
        *this = std::forward<StaticVector<T, S>>(other);
    }

    StaticVector(const T &val, std::size_t s = S)
        : StaticVector()
    {
        for (std::size_t i = 0; i < s; ++i) {
            push_back(val);
        }
    }

    StaticVector(const std::initializer_list<T> &l)
        : StaticVector()
    {
        for (auto &i : l) {
            push_back(i);
        }
    }

    ~StaticVector()
    {
        clear();
    }

    StaticVector<T, S>& operator=(const std::initializer_list<T> &l)
    {
        clear();

        for (const auto &i : l) {
            push_back(i);
        }

        return *this;
    }

    StaticVector<T, S>& operator=(const StaticVector<T, S> &rhs)
    {
        clear();

        for (auto it = rhs.cbegin(); it != rhs.cend(); ++it) {
            push_back(*it);
        }

        return *this;
    }

    StaticVector<T, S>& operator=(StaticVector<T, S> &&rhs)
    {
        clear();

        for (std::size_t i = 0; i < rhs.size(); ++i) {
            push_back(std::move(rhs[i]));
        }

        rhs.clear();
        return *this;
    }

    void assign(std::size_t count, const T& value)
    {
        clear();

        for (std::size_t i = 0; i < count; ++i) {
            push_back(value);
        }
    }

    template<typename InputIT>
    void assign(InputIT first, InputIT back)
    {
        clear();

        while (first != back) {
            push_back(*first);
            ++first;
        }
    }

    void assign(std::initializer_list<T> ilist)
    {
        (*this) = ilist;
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
            throw std::out_of_range("StaticVector::at");
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
            std::destroy_at(std::addressof(_buffer[i]));
        }
        _size = 0;
    }

    iterator insert(const_iterator pos, const T& val)
    {
        if (_size >= S) {
#ifdef __cpp_exceptions
            throw std::length_error("insert past StaticVector size");
#else
            abort();
#endif
        }

        shift_back(pos._index);
        std::construct_at(&_buffer[pos._index], val);
        _size++;
        return iterator(this, pos._index);
    }

    iterator insert(const_iterator pos, T&& val)
    {
        if (_size >= S) {
#ifdef __cpp_exceptions
            throw std::length_error("insert past StaticVector size");
#else
            abort();
#endif
        }

        shift_back(pos._index);
        std::construct_at(&_buffer[pos._index], std::forward<T>(val));
        _size++;
        return iterator(this, pos._index);
    }

    iterator insert(const_iterator pos, std::size_t count, const T& value)
    {
        if (0 == count) {
            return iterator(this, pos._index);
        }

        if (_size + count > S) {
#ifdef __cpp_exceptions
            throw std::length_error("insert past StaticVector size");
#else
            abort();
#endif
        }

        shift_back(pos._index, count);
        for (std::size_t i = 0; i < count; ++i) {
            std::construct_at(&_buffer[pos._index + i], value);
        }

        _size += count;
        return iterator(this, pos._index);
    }

    template<typename IT>
    iterator insert(const_iterator pos, IT first, IT last)
    {
        std::size_t count = std::distance(first, last);

        if (0 == count) {
            return iterator(this, pos._index);
        }

        if (_size + count > S) {
#ifdef __cpp_exceptions
            throw std::length_error("insert past StaticVector size");
#else
            abort();
#endif
        }

        shift_back(pos._index, count);
        std::size_t index = 0;
        while (first != last) {
            std::construct_at(&_buffer[pos._index + index], *first);
            first++;
            index++;
        }

        _size += count;

        return iterator(this, pos._index);
    }

    iterator insert(const_iterator pos, std::initializer_list<T> l)
    {
        return insert(pos, l.begin(), l.end());
    }

    template<typename ...Args>
    iterator emplace(const_iterator pos, Args&& ...args)
    {
        if (_size >= S) {
#ifdef __cpp_exceptions
            throw std::length_error("insert past StaticVector size");
#else
            abort();
#endif
        }

        shift_back(pos._index);
        std::construct_at(&_buffer[pos._index], std::forward<Args>(args)...);
        _size++;
        return iterator(this, pos._index);
    }

    iterator erase(const_iterator pos)
    {
        if (pos == this->cend()) {
            return this->end();
        }

        return erase(pos, pos+1);
    }

    iterator erase(const_iterator first, const_iterator last)
    {
        if (first._index >= last._index) {
            return this->end();
        }

        std::size_t dist = std::min(last._index, _size) - first._index;
        std::size_t dst = first._index;
        std::size_t src = dst + dist;

        while (dst < _size) {
            _buffer[dst] = std::move(_buffer[src]);

            std::destroy_at(&_buffer[src]);

            ++src;
            ++dst;
        }

        _size -= dist;
        return iterator(this, first._index);
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
        if (_size >= S) {
#ifdef __cpp_exceptions
            throw std::length_error("push_back past StaticVector size");
#else
            abort();
#endif
        }

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
        return iterator(this);
    }

    iterator end()
    {
        return iterator(this, size());
    }

    const_iterator cbegin() const
    {
        return const_iterator(this);
    }

    const_iterator cend() const
    {
        return const_iterator(this, size());
    }

    iterator find(const T& val)
    {
        return find(begin(), val);
    }

    iterator find(const_iterator start, const T& val)
    {
        auto end = this->end();

        while(start != end) {
            if (*start == val) {
                return iterator(this, start._index);
            }
        }

        return this->end();
    }

private:
    uint8_t _underling[sizeof(T) * S];
    T* _buffer;
    std::size_t _size;

    void shift_back(std::size_t at, std::size_t amnt = 1)
    {
        if (0 == _size) {
            return;
        }

        std::size_t new_size = _size + amnt;
        std::size_t shift_size = new_size - at;

        std::size_t src = _size - 1;
        std::size_t dst = new_size - 1;

        while (src >= at) {
            std::construct_at(&_buffer[dst],
                              std::move(_buffer[src]));

            std::destroy_at(&_buffer[src]);

            // If src is 0 then subtracting will underflow the unsigned int
            if (src == 0) {
                break;
            }

            dst--;
            src--;
        }
    }


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
        if (a[i] < b[i]) {
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
            return false;
        }
    }

    return a.size() <= b.size();
}

template<typename T, std::size_t S>
bool operator >(const StaticVector<T, S> &a, const StaticVector<T, S> &b)
{
    std::size_t max = std::min(a.size(), b.size());

    for (std::size_t i = 0; i < max; ++i) {
        if (a[i] > b[i]) {
            return true;
        }
    }

    return b.size() < a.size();
}

template<typename T, std::size_t S>
bool operator >=(const StaticVector<T, S> &a, const StaticVector<T, S> &b)
{
    std::size_t max = std::min(a.size(), b.size());

    for (std::size_t i = 0; i < max; ++i) {
        if (a[i] < b[i]) {
            return false;
        }
    }

    return a.size() >= b.size();
}

#ifdef STATICVEC_SUPPORT_IOSTREAM
template<typename T, std::size_t S>
std::ostream& operator<<(std::ostream &os, const StaticVector<T, S> &vec)
{
    bool first = true;
    os << "{";
    for (auto it = vec.cbegin(); it != vec.cend(); ++it) {
        if (!first) {
            os << ", ";
        } else {
            first = false;
        }

        os << *it;
    }

    os << "}";
    return os;
}
#endif
