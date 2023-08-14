#pragma once

// We also templatize on the Tag parameter from the TypedInterval class
// template to allow specializations for the same `Value` type but different
// `Tag` types. For example in general a vec3 would choose to compute the
// length via the norm of `difference`. But some TypedInterval<vec3> classes
// might serve as a container of three values and the length should simply
// return `difference`, as we want to keep the components separated.
template<typename Value, class Tag=struct DummyTag>
struct TypedIntervalTraits {
    static double length(Value const & difference) { return difference; }
};

template <typename Value, class Tag>
struct TypedInterval {
    Value min, max;

    explicit TypedInterval() : min{}, max{} {}
    TypedInterval(Value const & min, Value const & max) : min{min}, max{max} {}

    auto length() const { return TypedIntervalTraits<Value, Tag>::length(max - min); }

    TypedInterval expand(Value const & delta) const {
        Value const half_delta = 0.5 * delta;
        return TypedInterval(min - half_delta, max + half_delta);
    }
};

template <typename Value, class Tag>
TypedInterval<Value, Tag> operator+(TypedInterval<Value, Tag> const & interval, Value val) {
    return TypedInterval<Value, Tag>{interval.min + val, interval.max + val};
}

template <typename Value, class Tag>
bool operator==(TypedInterval<Value, Tag> const & lhs, TypedInterval<Value, Tag> const & rhs) {
    return lhs.min == rhs.min && lhs.max == rhs.max;
}

using TimeInterval = TypedInterval<double, struct TimeIntervalTag>;