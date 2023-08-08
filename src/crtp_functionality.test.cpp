#include <crtp_functionality.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>


using testing::Eq;
using testing::Ne;
using testing::Test;


struct inject_double_iterators {
    using       iterator = double       *;
    using const_iterator = double const *;
};

struct MockClassWithInjectedBeginAndEnd : inject_double_iterators
                                        , inject_trivial_begin_end<MockClassWithInjectedBeginAndEnd> {
    double first_entry, second_entry;

    MockClassWithInjectedBeginAndEnd(double const first, double const second)
        : first_entry{first}, second_entry{second} {}
 };

struct AClassWithInjectedBeginAndEnd : Test {
    double const first_entry{1.0};
    double const last_entry{42.0};
    MockClassWithInjectedBeginAndEnd             cl{first_entry, last_entry};
    MockClassWithInjectedBeginAndEnd const const_cl{first_entry, last_entry};
};

TEST_F(AClassWithInjectedBeginAndEnd, has_correct_bounds) {
   EXPECT_THAT(*cl.begin(),     Eq(first_entry));
   EXPECT_THAT(*(cl.end() - 1), Eq(last_entry));
}

TEST_F(AClassWithInjectedBeginAndEnd, has_correct_const_bounds) {
   EXPECT_THAT(*const_cl.cbegin(),     Eq(first_entry));
   EXPECT_THAT(*(const_cl.cend() - 1), Eq(last_entry));
}


struct MockClassWithInjectedIndexing : inject_double_iterators
                                     , inject_trivial_begin_end<MockClassWithInjectedIndexing>
                                     , inject_trivial_indexing<MockClassWithInjectedIndexing> {
    double a, b, c;
    MockClassWithInjectedIndexing(double const a, double const b, double const c): a{a}, b{b}, c{c} {}
};

struct AClassWithInjectedIndexing : Test {
    double const a{1.0};
    double const b{42.0};
    double const c{123.0};
    MockClassWithInjectedIndexing             cl{a, b, c};
    MockClassWithInjectedIndexing const const_cl{a, b, c};
};

TEST_F(AClassWithInjectedIndexing, has_correct_indexing) {
    for (std::size_t ii = 0; ii < 3; ++ii)
        cl[ii] = static_cast<double>(ii);

    EXPECT_THAT(cl[0], Eq(0.0));
    EXPECT_THAT(cl[1], Eq(1.0));
    EXPECT_THAT(cl[2], Eq(2.0));
}

TEST_F(AClassWithInjectedIndexing, has_correct_const_indexing) {
    EXPECT_THAT(const_cl[0], Eq(a));
    EXPECT_THAT(const_cl[1], Eq(b));
    EXPECT_THAT(const_cl[2], Eq(c));
}


struct MockClassWithInjectedContainerEqual : MockClassWithInjectedBeginAndEnd
                                           , inject_container_equal<MockClassWithInjectedContainerEqual> {
    MockClassWithInjectedContainerEqual(double const first, double const second)
        : MockClassWithInjectedBeginAndEnd(first, second) {}
};


struct AClassWithInjectedContainerEqual : Test {
    double const first_entry{1.0};
    double const last_entry{42.0};
    MockClassWithInjectedContainerEqual             cl{first_entry, last_entry};
    MockClassWithInjectedContainerEqual const const_cl{first_entry, last_entry};
};

TEST_F(AClassWithInjectedContainerEqual, can_be_compared_for_equality) {
    auto const copy{const_cl};
    EXPECT_THAT(const_cl, Eq(copy));

    auto not_copy{cl};
    (*not_copy.begin()) += 1.0;
    EXPECT_THAT(cl, Ne(not_copy));

}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}