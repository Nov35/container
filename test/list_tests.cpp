#include <gtest/gtest.h>

#include "../src/lifetime_helper/lifetime_helper.h"
#include "list/list.h"
#include <list>

TEST(ListTests, SizeIsChangingCorrectly)
{
    List<int> l;
    ASSERT_TRUE(l.empty());

    for (int i = 1; i <= 10; ++i)
    {
        l.push_front(i);
        l.push_back(i * 2);

        EXPECT_EQ(i * 2, l.size());
    }

    for (int i = 9; i >= 0; --i)
    {
        l.pop_front();
        l.pop_back();
        EXPECT_EQ(i * 2, l.size());
    }

    EXPECT_TRUE(l.empty());
}

TEST(ListTests, PushedObjectsArePlacedInRightOrder)
{
    List<int> l;
    for (int i = 10; i >= 1; --i)
    {
        l.push_front(i);
        l.push_back(i);
        ASSERT_EQ(l.front(), l.back());
    }

    while (!l.empty())
    {
        EXPECT_EQ(l.front(), l.back());

        l.pop_front();
        l.pop_back();
    }
}

TEST(ListTests, SwapWorksCorrectly)
{
    List reference{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    List reversed_reference{9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    List l1(reference);
    List l2(reversed_reference);

    l1.swap(l2);

    EXPECT_TRUE(std::equal(reversed_reference.begin(), reversed_reference.end(), l1.begin()));
    EXPECT_TRUE(std::equal(reference.begin(), reference.end(), l2.begin()));
}

TEST(ListTests, CanBeInitializedWithStdInitializerList)
{
    List l{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    for (int i = 0; i < 10; ++i)
    {
        EXPECT_EQ(i, l.front());
        l.pop_front();
    }
}

TEST(ListTests, RangeBasedAssignWorksCorrecty)
{
    std::vector vec{0, 1, 2, 3, 4};
    List l{1, 3, 4, 5, 6};

    l.assign(vec.begin(), vec.end());

    EXPECT_TRUE(std::equal(vec.cbegin(), vec.cend(), l.cbegin()));
    EXPECT_TRUE(std::equal(l.cbegin(), l.cend(), vec.cbegin()));
}

TEST(ListTests, ListCanBeResized)
{
    const List<int> reference{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    List l = reference;

    int first_resize = 4;
    l.resize(first_resize);

    ASSERT_EQ(first_resize, l.size());

    EXPECT_TRUE(std::equal(l.begin(), l.end(), reference.begin()));

    int second_resize = 12;

    l.resize(second_resize);

    for (int i = 0; i < second_resize; ++i)
    {
        auto ref_it = reference.cbegin();
        auto it = l.cbegin();

        EXPECT_EQ(i < first_resize ? *(ref_it++) : 0, *(it++));
    }

    l.resize(0);
    EXPECT_TRUE(l.empty());
}

TEST(ListTests, ReverseIteratorsWorkCorrectly)
{
    List<int> l{432, 66, 123, 778, 1, 745, 7, 1, 6543, 78, 345, 777, 124, 55, 11, 44, 53};
    List<int> reversed_l {53, 44, 11, 55, 124, 777, 345, 78, 6543, 1, 7, 745, 1, 778, 123, 66, 432};

    ASSERT_EQ(l.size(), reversed_l.size());

    EXPECT_TRUE(std::equal(l.crbegin(), l.crend(), reversed_l.cbegin()));
}

TEST(ListTests, ObjectsAreConstructedAndDestructedCorrectly)
{
    {
        List<LifetimeHelper> l;

        ASSERT_EQ(0, LifetimeHelper::get_alive_count());

        int size = 14;
        l.resize(size);

        EXPECT_EQ(size, LifetimeHelper::get_alive_count());
    }

    EXPECT_EQ(0, LifetimeHelper::get_alive_count());
}

TEST(ListTests, SortingWorksCorrectly)
{
    List l{432, 66, 123, 778, 1, 745, 7, 1, 6543, 78, 345, 777, 124, 55, 11, 44, 53};

    l.sort();

    EXPECT_TRUE(std::is_sorted(l.cbegin(), l.cend()));
}

TEST(ListTests, SplceWorksCorrectly)
{
    List reference{432, 66, 123, 778, 1, 745, 7, 1, 6543, 78, 345, 777, 124, 55, 11, 44, 53};
    List source(reference);

    List<int> destination;

    destination.splice(destination.cbegin(), source);

    ASSERT_EQ(0, source.size());
    ASSERT_EQ(reference.size(), destination.size());

    EXPECT_TRUE(std::equal(reference.begin(), reference.end(), destination.begin()));


    source = reference;
    auto first = std::find(source.begin(), source.end(), 778);
    auto last = std::find(source.begin(), source.end(), 124);

    auto dest_last_element = --destination.end();
    destination.splice(destination.end(), source, first, last);


    first = std::find(reference.begin(), reference.end(), 778);
    last = std::find(reference.begin(), reference.end(), 124);

    ASSERT_TRUE(std::equal(reference.begin(), reference.end(), destination.begin()));

    EXPECT_TRUE(std::equal(first, last, ++dest_last_element));
}

TEST(ListTests, ReverseWorksCorrectly)
{
    List<int> l{432, 66, 123, 778, 1, 745, 7, 1, 6543, 78, 345, 777, 124, 55, 11, 44, 53};
    List<int> reversed_l {53, 44, 11, 55, 124, 777, 345, 78, 6543, 1, 7, 745, 1, 778, 123, 66, 432};

    l.reverse();

    ASSERT_EQ(l.size(), reversed_l.size());

    EXPECT_TRUE(std::equal(l.cbegin(), l.cend(), reversed_l.cbegin()));

    l.clear();

    EXPECT_TRUE(l.empty());
}

TEST(ListTests, MergeWorksCorrectly)
{
    List destination{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    List source{10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

    int expected_size = destination.size() + source.size();

    destination.merge(source);

    ASSERT_EQ(expected_size, destination.size());
    ASSERT_TRUE(source.empty());
    EXPECT_TRUE(std::is_sorted(destination.begin(), destination.end()));

    source = {12, 654, 757, 777, 1231, 345, 1, 333, 435, 876, 678, 22, 5, 7434, 234, 23};
    destination = {432, 66, 123, 778, 1, 745, 7, 1, 6543, 78, 345, 777, 124, 55, 11, 44, 53};

    expected_size = destination.size() + source.size();

    destination.sort();
    source.sort();

    destination.merge(source);

    ASSERT_TRUE(source.empty());
    ASSERT_EQ(expected_size, destination.size());
    EXPECT_TRUE(std::is_sorted(destination.begin(), destination.end()));
}
