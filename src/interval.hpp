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

    TypedInterval expand(Value const & delta) const {
        Value const increment = 0.5 * delta;
        return TypedInterval(min - delta, max + delta);
    }

};

using TimeInterval = TypedInterval<double, struct TimeIntervalTag>;