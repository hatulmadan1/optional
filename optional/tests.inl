#include "gtest/gtest.h"
#include "counted.h"
#include "optional.h"

template <typename T>
T const& as_const(T& obj)
{
    return obj;
}

TEST(correctness, default_ctor)
{
    counted::no_new_instances_guard g;

    optional<counted> a;
    EXPECT_FALSE(static_cast<bool>(a));
    g.expect_no_instances();
}

TEST(correctness, value_ctor)
{
    counted::no_new_instances_guard g;

    optional<counted> a = counted(5);
    EXPECT_TRUE(static_cast<bool>(a));
    EXPECT_EQ(5, *a);
}

TEST(correctness, copy_ctor_1)
{
    counted::no_new_instances_guard g;

    optional<counted> a;
    optional<counted> copy = a;
    EXPECT_FALSE(static_cast<bool>(a));
}

TEST(correctness, copy_ctor_2)
{
    counted::no_new_instances_guard g;

    optional<counted> a = counted(6);
    optional<counted> copy = a;
    EXPECT_TRUE(static_cast<bool>(a));
    EXPECT_EQ(6, *a);
}

TEST(correctness, assignment_empty_empty)
{
    counted::no_new_instances_guard g;
    
    optional<counted> a, b;
    a = b;
    EXPECT_FALSE(static_cast<bool>(a));
}

TEST(correctness, assignment_empty_full)
{
    counted::no_new_instances_guard g;
    
    optional<counted> a;
    optional<counted> b = counted(7);
    a = b;
    EXPECT_TRUE(static_cast<bool>(a));
    EXPECT_EQ(7, *a);
}

TEST(correctness, assignment_full_empty)
{
    counted::no_new_instances_guard g;
    
    optional<counted> a = counted(8);
    optional<counted> b;
    a = b;
    EXPECT_FALSE(static_cast<bool>(a));
}

TEST(correctness, assignment_full_full)
{
    counted::no_new_instances_guard g;
    
    optional<counted> a = counted(9);
    optional<counted> b = counted(10);
    a = b;
    EXPECT_TRUE(static_cast<bool>(a));
    EXPECT_EQ(10, *a);
}

TEST(correctness, self_assignment_empty)
{
    counted::no_new_instances_guard g;
    
    optional<counted> a;
    a = a;
}

TEST(correctness, self_assignment_full)
{
    counted::no_new_instances_guard g;
    
    optional<counted> a = counted(11);
    a = a;
    
    EXPECT_TRUE(static_cast<bool>(a));
    EXPECT_EQ(11, *a);
}

TEST(correctness, clear)
{
    counted::no_new_instances_guard g;

    optional<counted> a = counted(7);
    EXPECT_TRUE(static_cast<bool>(a));
    a.clear();
    EXPECT_FALSE(static_cast<bool>(a));
    a.clear();
    EXPECT_FALSE(static_cast<bool>(a));
}

TEST(correctness, deref)
{
    counted::no_new_instances_guard g;

    optional<counted> a = counted(8);

    *a = 9;
    EXPECT_EQ(9, *as_const(a));
}

TEST(correctness, swap_empty_empty)
{
    counted::no_new_instances_guard g;
    
    optional<counted> a, b;
    
    swap(a, b);
    EXPECT_FALSE(static_cast<bool>(a));
    EXPECT_FALSE(static_cast<bool>(b));
}

TEST(correctness, swap_empty_full)
{
    counted::no_new_instances_guard g;
    
    optional<counted> a;
    optional<counted> b = counted(12);
    
    swap(a, b);
    EXPECT_TRUE(static_cast<bool>(a));
    EXPECT_EQ(12, *a);
    EXPECT_FALSE(static_cast<bool>(b));
}

TEST(correctness, swap_full_empty)
{
    counted::no_new_instances_guard g;
    
    optional<counted> a = counted(13);
    optional<counted> b;
    
    swap(a, b);
    EXPECT_FALSE(static_cast<bool>(a));
    EXPECT_TRUE(static_cast<bool>(b));
    EXPECT_EQ(13, *b);
}

TEST(correctness, swap_full_full)
{
    counted::no_new_instances_guard g;
    
    optional<counted> a = counted(14);
    optional<counted> b = counted(15);
    
    swap(a, b);
    EXPECT_TRUE(static_cast<bool>(a));
    EXPECT_EQ(15, *a);
    EXPECT_TRUE(static_cast<bool>(b));
    EXPECT_EQ(14, *b);
}

TEST(correctness, comparisions_empty_empty)
{
    optional<counted> a, b;
    
    EXPECT_FALSE(a < b);
    EXPECT_TRUE (a <= b);
    EXPECT_TRUE (a == b);
    EXPECT_FALSE(a != b);
    EXPECT_FALSE(a > b);
    EXPECT_TRUE (a >= b);
}

TEST(correctness, comparisions_empty_full)
{
    optional<counted> a;
    optional<counted> b = counted(16);
    
    EXPECT_TRUE (a < b);
    EXPECT_TRUE (a <= b);
    EXPECT_FALSE(a == b);
    EXPECT_TRUE (a != b);
    EXPECT_FALSE(a > b);
    EXPECT_FALSE(a >= b);
}

TEST(correctness, comparisions_full_empty)
{
    optional<counted> a = counted(17);
    optional<counted> b;
    
    EXPECT_FALSE(a < b);
    EXPECT_FALSE(a <= b);
    EXPECT_FALSE(a == b);
    EXPECT_TRUE (a != b);
    EXPECT_TRUE (a > b);
    EXPECT_TRUE (a >= b);
}

TEST(correctness, comparisions_full_full_1)
{
    optional<counted> a = counted(18);
    optional<counted> b = counted(19);
    
    EXPECT_TRUE (a < b);
    EXPECT_TRUE (a <= b);
    EXPECT_FALSE(a == b);
    EXPECT_TRUE (a != b);
    EXPECT_FALSE(a > b);
    EXPECT_FALSE(a >= b);
}

TEST(correctness, comparisions_full_full_2)
{
    optional<counted> a = counted(20);
    optional<counted> b = counted(20);
    
    EXPECT_FALSE(a < b);
    EXPECT_TRUE (a <= b);
    EXPECT_TRUE (a == b);
    EXPECT_FALSE(a != b);
    EXPECT_FALSE(a > b);
    EXPECT_TRUE(a >= b);
}
