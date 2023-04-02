#include <gtest/gtest.h>
#include <iostream>
#include <gmock/gmock.h>

using testing::Eq;

#include <array>

class vec3 {
public:
    using      iterator = double       *;
    using const_iterator = double const *;

    vec3() = default;

    template<typename T>
    vec3(T d): x{static_cast<double>(d)},
               y{static_cast<double>(d)},
               z{static_cast<double>(d)} {}

    template<typename T>
    vec3(T x, T y, T z): x{static_cast<double>(x)},
                         y{static_cast<double>(y)},
                         z{static_cast<double>(z)} {}

    double& operator[](std::size_t index)       { return *(&x + index); }
    double  operator[](std::size_t index) const { return *(&x + index); }

          iterator  begin()       { return &x; }
    const_iterator  begin() const { return &x; }
    const_iterator cbegin() const { return &x; }

          iterator  end()       { return &x + 3; }
    const_iterator  end() const { return &x + 3; }
    const_iterator cend() const { return &x + 3; }

    double x = 0, y = 0, z = 0;
};

bool operator==(vec3 u, vec3 v) { return u[0] == v[0] &&
                                         u[1] == v[1] &&
                                         u[2] == v[2]; }

std::ostream& operator<<(std::ostream& out, vec3 const & v) { return out << '(' << v.x << ", "
                                                                                << v.y << ", "
                                                                                << v.z << ')'; }

TEST(vec3, has_equality) { EXPECT_THAT(vec3{}, Eq(vec3{})); }

TEST(vec3, can_be_default_constructed) { vec3{}; }

TEST(vec3, can_is_default_constructed_to_zero) {
    auto v = vec3{};
    EXPECT_THAT(v, Eq(vec3{0, 0, 0}));
}

TEST(vec3, can_be_constructed_with_values) {
     vec3 const v{1, 2, 3};
     EXPECT_THAT(v[0], Eq(1));
     EXPECT_THAT(v[1], Eq(2));
     EXPECT_THAT(v[2], Eq(3));
}

TEST(vec3, has_index_operator)       { vec3       v{}; v[0]; }

TEST(vec3, has_const_index_operator) { vec3 const v{}; v[0]; }

TEST(vec3, can_be_constructed_with_value) {
     vec3 const v{-1};
     EXPECT_THAT(v, Eq(vec3{-1, -1, -1}));
}

TEST(vec3, has_begin)  { vec3{}.begin();  }

TEST(vec3, has_cbegin) { vec3{}.cbegin(); }

TEST(vec3, has_end)    { vec3{}.end();    }

TEST(vec3, has_cend)   { vec3{}.cend();   }

TEST(vec3, has_begin_and_end_with_reasonable_diff) {
    vec3 const v{};
     EXPECT_THAT(v.end() - v.begin(), Eq(3));
}

TEST(vec3, has_can_be_used_in_range_for) {
    vec3 v{};
    for (auto &x : v)
        x = 1;
    EXPECT_THAT(v, Eq(vec3{1, 1, 1}));
}

TEST(vec3, has_can_be_used_in_range_for_const) {
    vec3 const v{1, 2, 3};
    double sum = 0.0;
    for (auto x : v)
        sum += x;
    EXPECT_THAT(sum, Eq(6));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}







































