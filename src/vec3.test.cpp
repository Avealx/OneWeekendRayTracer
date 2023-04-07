#include <gtest/gtest.h>
#include <iostream>
#include <gmock/gmock.h>

#include <cmath>

using testing::Eq;
using testing::DoubleEq;
using namespace testing;

#include <array>

class vec3 {
public:
    using      iterator = double       *;
    using const_iterator = double const *;

    vec3() = default;
    vec3(double d): x{d}, y{d}, z{d} {}
    vec3(double x, double y, double z): x{x}, y{y}, z{z} {}

    double& operator[](std::size_t index)       { return *(&x + index); }
    double  operator[](std::size_t index) const { return *(&x + index); }

          iterator  begin()       { return &x; }
    const_iterator  begin() const { return &x; }
    const_iterator cbegin() const { return &x; }

          iterator  end()       { return &x + 3; }
    const_iterator  end() const { return &x + 3; }
    const_iterator cend() const { return &x + 3; }

    vec3  operator-() const { return vec3{-x, -y, -z}; }

    vec3& operator+=(vec3 const & v) {
        for (std::size_t i = 0; i < 3; ++i)
            (*this)[i] += v[i];
        return *this;
    }

    vec3& operator*=(vec3 const & v) {
        for (std::size_t i = 0; i < 3; ++i)
            (*this)[i] *= v[i];
        return *this;
    }

    vec3& operator/=(double d) { return *this *= 1.0 / d; }

    double length()         const { return std::sqrt(x*x + y*y + z*z); }
    double length_squared() const { return           x*x + y*y + z*z ; }

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

struct a_vec3 : ::testing::Test {
    vec3        v{1, 2, 3};
    vec3 const cv{1, 2, 3};
};

TEST_F(a_vec3, has_can_be_used_in_range_for_const) {
    double sum = 0.0;
    for (auto x : v)
        sum += x;
    EXPECT_THAT(sum, Eq(6));
}

TEST_F(a_vec3, can_be_negated) {
    EXPECT_THAT(-v, Eq(vec3{-1, -2, -3}));
}

TEST_F(a_vec3, has_add_assignment) {
    v += v;
    EXPECT_THAT(v, Eq(vec3{2, 4, 6}));
}

TEST_F(a_vec3, has_mult_assignment) {
    v *= v;
    EXPECT_THAT(v, Eq(vec3{1, 4, 9}));
}

void expect_double_equal(vec3 const & u, vec3 const & v) {
    EXPECT_THAT(u.x, DoubleEq(v.x));
    EXPECT_THAT(u.y, DoubleEq(v.y));
    EXPECT_THAT(u.z, DoubleEq(v.z));
}

TEST_F(a_vec3, has_div_assignment_with_scalar) {
    v /= 10.0;
    expect_double_equal(v, vec3{0.1, 0.2, 0.3});
}

TEST_F(a_vec3, has_length_function) {
    vec3 const v{1.0, 2.0, 2.0};
     EXPECT_THAT(v.length(), DoubleEq(3.0));
}

TEST_F(a_vec3, has_length_squared_function) {
    EXPECT_THAT(v.length_squared(), DoubleEq(14.0));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}
