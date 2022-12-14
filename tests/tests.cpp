#include <memory>
#include <gtest/gtest.h>
#include <staticvector.hpp>

TEST(StaticVector, push_back)
{
    StaticVector<int, 16> vec;

    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    ASSERT_EQ(vec[0], 1);
    ASSERT_EQ(vec[1], 2);
    ASSERT_EQ(vec[2], 3);

    ASSERT_EQ(vec.size(), 3);
    ASSERT_EQ(vec.capacity(), 16);

    vec.emplace_back(4);
    ASSERT_EQ(vec[3], 4);
}

TEST(StaticVector, pop_back)
{
    StaticVector<int, 16> vec;

    vec.push_back(1);
    vec.push_back(2);

    ASSERT_EQ(vec[0], 1);
    ASSERT_EQ(vec[1], 2);

    ASSERT_EQ(vec.back(), 2);
    vec.pop_back();
    ASSERT_EQ(vec.back(), 1);
    vec.pop_back();

    ASSERT_EQ(vec.size(), 0);
}

TEST(StaticVector, indexing)
{
    StaticVector<int, 16> vec({23, 45, 87, 90, 10});

    ASSERT_EQ(vec.size(), 5);
    ASSERT_EQ(vec[4], 10);
    ASSERT_EQ(vec[3], 90);
    ASSERT_EQ(vec[0], 23);
}

TEST(StaticVector, iterators)
{
    StaticVector<int, 16> vec = {23, 21, 83, 93, 148, 128, 304, 1};

    std::size_t i = 0;
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        ASSERT_EQ(vec[i], *it);
        ++i;
    }

    auto it = vec.begin();

    ASSERT_EQ(*(it + 2), 83);
    ASSERT_EQ(*(it + 7), 1);

    ASSERT_EQ(it + 10, vec.end());

    it += 4;
    ASSERT_EQ(*it, 148);
    ASSERT_EQ(*(it - 1), 93);
    ASSERT_EQ(it + 4, vec.end());
}

TEST(StaticVector, assignment)
{
    StaticVector<int, 16> vec1 = {23, 21, 83, 93, 148, 128, 304, 1};
    StaticVector<int, 16> vec2;

    vec2 = vec1;
    ASSERT_EQ(vec2.size(), vec1.size());

    for (std::size_t i = 0; i < vec1.size(); ++i) {
        ASSERT_EQ(vec1[i], vec2[i]);
    }

    StaticVector<int, 16> vec3(vec1);

    ASSERT_EQ(vec3.size(), vec1.size());

    for (std::size_t i = 0; i < vec1.size(); ++i) {
        ASSERT_EQ(vec1[i], vec2[i]);
    }
}

TEST(StaticVector, move)
{
    StaticVector<int, 16> vec1 = {23, 21, 83, 93, 148, 128, 304, 1};
    StaticVector<int, 16> vec2 = vec1;
    StaticVector<int, 16> vec3(std::move(vec2));

    ASSERT_EQ(vec2.size(), 0);
    ASSERT_EQ(vec3.size(), 8);

    for (std::size_t i = 0; i < vec1.size(); ++i) {
        ASSERT_EQ(vec1[i], vec3[i]);
    }
}

TEST(StaticVector, exceptions)
{
    StaticVector<int, 1> vec;

    ASSERT_THROW(vec.at(2), std::out_of_range);

    vec.push_back(1);
    ASSERT_THROW(vec.push_back(2), std::length_error);

    ASSERT_THROW(vec.emplace_back(4), std::length_error);
}

TEST(StaticVector, accessors)
{
    StaticVector<int, 16> vec = {23, 21, 83, 93, 148, 128, 304, 1};

    ASSERT_EQ(vec.front(), 23);
    ASSERT_EQ(vec.back(), 1);

    vec.pop_back();

    ASSERT_EQ(vec.back(), 304);

    vec.clear();
    ASSERT_THROW(vec.front(), std::out_of_range);
    ASSERT_THROW(vec.back(), std::out_of_range);
    ASSERT_EQ(vec.empty(), true);
    ASSERT_EQ(vec.size(), 0);

    vec.push_back(42);
    ASSERT_EQ(vec.at(0), 42);
    ASSERT_EQ(vec.back(), vec.front());

    ASSERT_EQ(vec.data()[0], 42);
}

TEST(StaticVector, operators)
{
    StaticVector<int, 16> vec1 = {1, 2, 3, 4};
    StaticVector<int, 16> vec2 = vec1;

    ASSERT_TRUE(vec1 == vec2);
    vec2[1]--;
    ASSERT_TRUE(vec1 != vec2);
    ASSERT_TRUE(vec1 > vec2);
    ASSERT_TRUE(vec1 >= vec2);

    vec2 = vec1;
    ASSERT_TRUE(vec1 >= vec2);
    ASSERT_TRUE(vec1 <= vec2);

    vec2[1]++;
    ASSERT_TRUE(vec1 < vec2);
    ASSERT_TRUE(vec1 <= vec2);
    ASSERT_TRUE(vec1 != vec2);
}

TEST(StaticVector, construction)
{
    StaticVector<int, 4> vec1(42);

    for (auto &i : vec1) {
        ASSERT_EQ(i, 42);
    }

    vec1 = {3,4,5};
    ASSERT_EQ(vec1[0], 3);
    ASSERT_EQ(vec1[1], 4);
    ASSERT_EQ(vec1[2], 5);

    ASSERT_EQ(vec1.max_size(), 4);
}

TEST(StaticVector, constness)
{
    const StaticVector<int, 4> vec = {1, 2, 3, 4};

    ASSERT_EQ(vec.front(), 1);
    ASSERT_EQ(vec[1], 2);
    ASSERT_EQ(vec[2], 3);
    ASSERT_EQ(vec.back(), 4);

    ASSERT_EQ(vec.data()[2], 3);
}

TEST(StaticVector, insert)
{
    StaticVector<int, 32> vec = {1, 2, 3};
    StaticVector<int, 32> expected;

    vec.insert(vec.begin() + 1, 45);

    expected = {1, 45, 2, 3};
    ASSERT_EQ(vec, expected);

    vec.insert(vec.begin() + 1, static_cast<std::size_t>(3), 42);
    expected = {1, 42, 42, 42, 45, 2, 3};
    ASSERT_EQ(vec, expected);

    vec = {1, 2, 3};
    vec.insert(vec.begin(), {3, 2, 1});
    expected = {3, 2, 1, 1, 2, 3};
    ASSERT_EQ(vec, expected);

    vec = {1, 2, 3};
    vec.insert(vec.end(), {4, 5, 6});
    expected = {1, 2, 3, 4, 5, 6};
    ASSERT_EQ(vec, expected);

    vec = {};
    vec.insert(vec.begin(), {1, 2, 3});
    expected = {1, 2, 3};
    ASSERT_EQ(vec, expected);
}

TEST(StaticVector, emplace)
{
    // unique_ptr is a nice non-copyable type to use
    StaticVector<std::unique_ptr<int>, 16> vec;

    auto ptr = std::make_unique<int>(42);

    vec.emplace(vec.begin(), std::move(ptr));
    ASSERT_EQ(*vec.at(0), 42);
}

TEST(StaticVector, erase)
{
    StaticVector<int, 16> vec = {1, 2, 3, 4, 5, 6, 7};
    StaticVector<int, 16> expected;

    vec.erase(vec.begin() + 2);
    expected = {1, 2, 4, 5, 6, 7};
    ASSERT_EQ(vec, expected);

    vec = {1, 2, 3, 4, 5, 6, 7};
    vec.erase(vec.begin(), vec.begin() + 4);
    expected = {5, 6, 7};
    ASSERT_EQ(vec, expected);

    vec = {1, 2, 3, 4, 5, 6, 7};
    vec.erase(vec.begin() + 4, vec.end());
    expected = {1, 2, 3, 4};
    ASSERT_EQ(vec, expected);

    vec = {1, 2, 3, 4, 5, 6, 7};
    vec.erase(vec.begin() + 2, vec.begin() + 5);
    expected = {1, 2, 6, 7};
    ASSERT_EQ(vec, expected);
}

TEST(StaticVector, assign)
{
    StaticVector<int, 16> vec;
    StaticVector<int, 16> expected;

    vec.assign(static_cast<std::size_t>(5), 42);
    expected = {42, 42, 42, 42, 42};
    ASSERT_EQ(vec, expected);

    vec.assign(expected.begin(), expected.end());
    ASSERT_EQ(vec, expected);

    vec.assign({42, 42, 42, 42, 42});
    ASSERT_EQ(vec, expected);
}
