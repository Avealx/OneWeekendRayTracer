#pragma once

template<typename T>
struct TypedIntervalTraits {
    static double length(T const & difference) { return difference; }
};

template <typename Value, class Tag>
struct TypedInterval {
    Value min, max;

    explicit TypedInterval() : min{}, max{} {}
    TypedInterval(Value const & min, Value const & max) : min{min}, max{max} {}

    auto length() const { return TypedIntervalTraits<Value>::length(max - min); }
};

using TimeInterval = TypedInterval<double, struct TimeIntervalTag>;