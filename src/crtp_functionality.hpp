#pragma once

#include <cstddef>

template <typename Value>
struct inject_pointer_as_iterator {
    using       iterator = Value       *;
    using const_iterator = Value const *;
};

// requires Base to define the types iterator and const_iterator
template <class Base>
struct inject_trivial_begin_end {
    auto  begin()       { return reinterpret_cast<typename Base::      iterator>(this); }
    auto  begin() const { return reinterpret_cast<typename Base::const_iterator>(this); }
    auto cbegin() const { return reinterpret_cast<typename Base::const_iterator>(this); }

    auto  end()       { return reinterpret_cast<typename Base::      iterator>(static_cast<Base       *>(this) + 1); }
    auto  end() const { return reinterpret_cast<typename Base::const_iterator>(static_cast<Base const *>(this) + 1); }
    auto cend() const { return reinterpret_cast<typename Base::const_iterator>(static_cast<Base const *>(this) + 1); }
};


// requires Base implementing concept `iterable` (begin())
template <class Base>
struct inject_trivial_indexing {
    auto& operator[](std::size_t index)       { return *(static_cast<Base       *>(this)->begin() + index); }
    auto  operator[](std::size_t index) const { return *(static_cast<Base const *>(this)->begin() + index); }
};

// requires Base implementing concept `iterable` (begin())
template <class Base>
struct inject_container_equal {
    friend bool operator==(Base const & lhs, Base const & rhs) {
        for(auto l_it = lhs.cbegin(), r_it = rhs.cbegin(); l_it != lhs.cend(); ++l_it, ++r_it)
            if (*l_it != *r_it)
                return false;
        return true;
    }

    friend bool operator!=(Base const & lhs, Base const & rhs) {
        return !(lhs == rhs);
    }
};