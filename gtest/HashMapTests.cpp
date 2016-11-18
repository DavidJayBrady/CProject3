// HashMapTests.cpp
//
// ICS 45C Fall 2016
// Project #3: Maps and Legends
//
// Write unit tests for your HashMap class here.  I've provided one test already,
// though I've commented it out, because it won't compile until you've implemented
// three things: the HashMap constructor, the HashMap destructor, and HashMap's
// size() member function.

#include <gtest/gtest.h>
#include "HashMap.hpp"


TEST(TestHashMap, sizeOfNewlyCreatedHashMapIsZero)
{
    HashMap empty;
    ASSERT_EQ(0, empty.size());
}

// cant just run this because buckets is a private variable
//TEST(TestHashMapEmptyConstructor, EmptyConstructorSetsupCorrectly)
//{
//    HashMap empty;
//    for (int i=0; i < empty.initialBucketCount; ++i){
//        ASSERT_EQ(nullptr, empty.buckets[i]);
//    }
//}


HashMap stuff;

TEST(TestHashMap, AddToHashMap)
{
    stuff.add("hello", "world");
    stuff.add("second", "round2");
    ASSERT_EQ(2, stuff.size());
    ASSERT_EQ(10, stuff.bucketCount());
    ASSERT_EQ(0.2, stuff.loadFactor());
}

TEST(TestHashMap, RemoveFromHashMap)
{
    stuff.remove("hello");
    ASSERT_EQ(1, stuff.size());
    ASSERT_EQ(.1, stuff.loadFactor());
}

TEST(TestHashMap, Contains)
{
    stuff.add("hello", "world");
    stuff.add("aadit", "eight");
    stuff.add("seven", "nine");
    ASSERT_EQ(4, stuff.size());
    ASSERT_EQ(true, stuff.contains("hello"));
    stuff.remove("hello");
    ASSERT_EQ(3, stuff.size());
    ASSERT_EQ(true, stuff.contains("second"));
    ASSERT_EQ(false, stuff.contains("hello"));
    ASSERT_EQ(false, stuff.contains("hello! =P"));
    ASSERT_EQ(true, stuff.contains("aadit"));
    ASSERT_EQ(true, stuff.contains("seven"));
}

TEST(TestHashMap, Values)
{
    ASSERT_EQ("nine", stuff.value("seven"));
    ASSERT_EQ("eight", stuff.value("aadit"));
    ASSERT_EQ("round2", stuff.value("second"));
}

TEST(TestHashMap, MaxBucketSizes)
{
    ASSERT_EQ(2, stuff.maxBucketSize());
    stuff.add("sevfm", "whoooo");
    ASSERT_EQ(3, stuff.maxBucketSize());
    ASSERT_EQ(4, stuff.size());
}

HashMap hashMap3;

TEST(TestHashMap, AssignmentOperator)
{
    HashMap hashMap2;
    hashMap2 = stuff;
    ASSERT_EQ("nine", hashMap2.value("seven"));
    ASSERT_EQ("eight", hashMap2.value("aadit"));
    ASSERT_EQ(3, stuff.maxBucketSize());
    ASSERT_EQ(4, stuff.size());
    ASSERT_EQ(10, stuff.bucketCount());
    ASSERT_EQ(0.4, stuff.loadFactor());

    HashMap stuff2;
    stuff2.add("one", "heyyy");
    stuff2.add("pnd", "whoo");
    ASSERT_EQ(2, stuff2.maxBucketSize());
    stuff2.add("rekt", "hardwork");
    ASSERT_EQ(3, stuff2.size());
    hashMap3 = stuff2;
    hashMap3.add("oncemoar", "idk");
    // Check that assignment operator keeps everything separate
    ASSERT_TRUE(hashMap3.size() != stuff2.size() && hashMap3.loadFactor() != stuff2.loadFactor());
    ASSERT_TRUE(hashMap3.contains("oncemoar") && !stuff2.contains("oncemoar"));
    hashMap3.remove("oncemoar");
    // Check that assignment operator made everything correctly, and hashMap3 can still operate.
    ASSERT_EQ(hashMap3.size(), 3);
    ASSERT_EQ(2, hashMap3.maxBucketSize());
    ASSERT_EQ("heyyy", hashMap3.value("one"));
    ASSERT_EQ("whoo", hashMap3.value("pnd"));
    ASSERT_EQ(0.3, hashMap3.loadFactor());
    ASSERT_EQ(false, hashMap3.contains("doge"));
    ASSERT_EQ(true, hashMap3.contains("rekt"));
    ASSERT_EQ(10, hashMap3.bucketCount());
    hashMap3.remove("rektrektrektkret"); // should not do anything if key isn't present
    ASSERT_EQ(3, hashMap3.size());
}

TEST(TestHashMap, CopyConstructor)
{
    HashMap hashMap4 = hashMap3;
    ASSERT_EQ(true, hashMap4.contains("one"));
    ASSERT_EQ("heyyy", hashMap4.value("one"));
    ASSERT_EQ("whoo", hashMap4.value("pnd"));
    ASSERT_EQ(0.3, hashMap4.loadFactor());
    ASSERT_EQ(false, hashMap4.contains("doge"));
    ASSERT_EQ(true, hashMap4.contains("rekt"));
    ASSERT_EQ(10, hashMap4.bucketCount());
}

TEST(TestHashMap, ReHash)
{
    HashMap storage;
    storage.add("one", "jisdk");
    storage.add("two", "fasiod");
    storage.add("three", "fasiod");
    storage.add("four", "fasiod");
    ASSERT_EQ(0.4, storage.loadFactor());
    storage.add("five", "fasiod");
    storage.add("six", "whoop");
    storage.add("seven", "fasiod");
    storage.add("eight", "fasiod");
    ASSERT_EQ(0.8, storage.loadFactor());
    storage.add("nine", "fasiod");
    storage.add("ten", "fasiod");
    storage.add("eleven", "fasiod");
    ASSERT_EQ((double(11)/21), storage.loadFactor());
    storage.add("twelve", "fasiod");
    ASSERT_EQ((double(12)/21), storage.loadFactor());
}