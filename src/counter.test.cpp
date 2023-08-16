#include <counter.hpp>

#include <gmock/gmock.h>
#include <gmock/gmock.h>

#include <vector>
#include <numeric>


TEST(counter, is_thread_safe) {
    // test that if every entry in an initially all zeroes vector is
    // incremented in parallel, the resulting vector is all ones.
    // No entry skipped or incremented twice.
    std::size_t const number_of_elements{10000};
    Counter counter{number_of_elements};
    std::vector results(number_of_elements, 0);

    std::vector<std::jthread> threads(20);
    for (auto & t : threads)
        t = std::jthread{[&results, &counter]() {
                             while (auto ii = counter.next())
                                 ++results[ii.value()];
                         }};

    for (auto & t : threads)
        if (t.joinable())
            t.join();

    EXPECT_THAT(std::accumulate(std::cbegin(results),
                                std::cend(results),
                                1,
                                std::multiplies<>{}),
                1);
}


int main(int argc, char ** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}