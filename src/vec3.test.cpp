#include <vec3.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cmath>
#include <iostream>
#include <string>
#include <sstream>

using testing::DoubleEq;
using testing::Eq;
using testing::Ne;

#include <array>


TEST(vec3, has_equality) { EXPECT_THAT(vec3{}, Eq(vec3{})); }

TEST(vec3, has_inequality) { EXPECT_THAT(vec3{0}, Ne(vec3{1})); }

TEST(vec3, can_be_default_constructed) { vec3{}; }

TEST(vec3, can_be_constructed_with_values) {
     vec3 const v{1, 2, 3};
     EXPECT_THAT(v[0], Eq(1));
     EXPECT_THAT(v[1], Eq(2));
     EXPECT_THAT(v[2], Eq(3));
}

TEST(vec3, can_be_constexpr_constructed_with_values) { static_assert(vec3{1,2,3}.x == 1); }

TEST(vec3, has_index_operator)       { vec3       v{}; v[0]; }

TEST(vec3, has_const_index_operator) { vec3 const v{}; v[0]; }

TEST(vec3, can_be_constructed_with_value) {
     vec3 const v{-1};
     EXPECT_THAT(v, Eq(vec3{-1, -1, -1}));
}

TEST(vec3, can_be_constexpr_constructed_with_value) { static_assert(vec3{2}.x == 2); }

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

struct two_vec3: a_vec3 {
    vec3        u{10, 20, 30};
    vec3 const cu{10, 20, 30};
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
    v *= 2.0;
    EXPECT_THAT(v, Eq(vec3{2, 4, 6}));
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

TEST_F(a_vec3, can_be_printed) {
    std::stringstream ss;
    ss << v;
    EXPECT_THAT(ss.str(), Eq("(1, 2, 3)"));
}

TEST_F(two_vec3, can_be_added) {
    EXPECT_THAT(cu + cv, Eq(vec3{11, 22, 33}));
}

TEST_F(two_vec3, can_be_subtracted) {
    EXPECT_THAT(cu - cv, Eq(vec3{9, 18, 27}));
}

TEST_F(two_vec3, can_be_multiplied_element_wise) {
    EXPECT_THAT(cu * cv, Eq(vec3{10, 40, 90}));
}

TEST_F(a_vec3, can_be_multiplied_with_scalar_from_right) {
    EXPECT_THAT(cv * 2, Eq(vec3{2, 4, 6}));
}

TEST_F(a_vec3, can_be_multiplied_with_scalar_from_left) {
    EXPECT_THAT(2 * cv, Eq(vec3{2, 4, 6}));
}

TEST_F(a_vec3, can_be_divided_by_scalar) {
    EXPECT_THAT(cv / 2.0, Eq(vec3{0.5, 1.0, 1.5}));
}

TEST_F(a_vec3, can_be_normalized) {
    EXPECT_THAT(unit_vector(cv).length(), DoubleEq(1.0));
}

TEST_F(two_vec3, have_dot_product) {
    EXPECT_THAT(dot(cu, cv), Eq(140));
}

TEST_F(two_vec3, have_cross_product) {
    vec3 const x{1, 0, 0};
    vec3 const y{0, 1, 0};
    vec3 const z{0, 0, 1};
    EXPECT_THAT(cross(x, y), Eq(z));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
