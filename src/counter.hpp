#pragma once

#include <optional>
#include <thread>

class Counter {
public:
    Counter(std::size_t end) : end_{end} {}

    Counter(Counter const &)            = delete;
    Counter& operator=(Counter const &) = delete;

    std::optional<std::size_t> next() {
        std::lock_guard lock{mutex_};
        return counter_ != end_ ? std::make_optional(counter_++) : std::nullopt;
    }

private:
    std::size_t counter_{0}, end_{};
    std::mutex mutex_;
};