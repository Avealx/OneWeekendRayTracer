#pragma once


template <typename Value, class Tag>
struct TypedInterval {
    explicit TypedInterval() : min{}, max{} {}
    TypedInterval(Value const & min, Value const & max) : min{min}, max{max} {}
    Value min, max;
};

using TimeInterval = TypedInterval<double, struct TimeIntervalTag>;