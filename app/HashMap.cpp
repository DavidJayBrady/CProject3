#include <string>
#include "HashMap.hpp"

#include <iostream>

unsigned int defaultHasher(const std::string& key);

HashMap::HashMap()
{
    buckets = new Node*[initialBucketCount]{nullptr};
    hasher = defaultHasher;
    numberOfPairs = 0;
    numberOfBuckets = initialBucketCount;
}

HashMap::HashMap(HashFunction hasher)
{
    buckets = new Node*[initialBucketCount]{nullptr};
    this->hasher = hasher;
    numberOfPairs = 0;
    numberOfBuckets = initialBucketCount;
}

HashMap::HashMap(const HashMap& hm)
{
    hasher = hm.hasher;
    numberOfPairs = hm.numberOfPairs;
    numberOfBuckets = hm.numberOfBuckets;
    buckets = copyBuckets(hm.buckets, hm.numberOfBuckets);
}


HashMap& HashMap::operator=(const HashMap &hm)
{
    if (this != &hm) {
        cleanBuckets(buckets);          // deallocate memory
        numberOfPairs = hm.numberOfPairs;
        numberOfBuckets = hm.numberOfBuckets;
        buckets = copyBuckets(hm.buckets, hm.numberOfBuckets);

    }
    return *this;
}


HashMap::~HashMap()
{
    cleanBuckets(buckets);
}

void HashMap::add(const std::string& key, const std::string& value)
{
    // do nothing if key is already in here
    if (!contains(key)) {
        if (weShouldReHash()) {
            rehashBuckets();
        }
        // now add the key value in
        addKeyValue(buckets, numberOfBuckets, key, value);
        numberOfPairs++;
    }
}

void HashMap::remove(const std::string &key)
{
    // do nothing if key is not in HashMap
    if (contains(key)) {
        int index = findIndex(key, numberOfBuckets);
        Node *previous = buckets[index];
        Node *current = previous;
        while (true) {
            if (current->key == key) {
                // this next if is true if at the starting point
                // remove the Node by just having the linked list
                // skip it and deallocating memory
                if (current == buckets[index]) {
                    buckets[index] = current->next;
                }
                else {
                    previous->next = current->next;
                }
                delete current;
                numberOfPairs--;
                break;
            } else {
                // we haven't found the match yet, keep looking
                previous = current;
                current = current->next;
            }
        }
    }
}

bool HashMap::contains(const std::string &key) const
{
    // traverseAndFind returns nullptr if the key isn't inside the HashMap
    // so returning matched != nullptr does the job
    Node* matched = traverseAndFind(key);
    return matched != nullptr;
}

std::string HashMap::value(const std::string &key) const
{
    // return value if key is in there, empty string if not
    if (contains(key)) {
        // matched will be the node that had the matching key
        Node *matched = traverseAndFind(key);
        return matched->value;
    }
    else {
        return "";
    }
}

unsigned int HashMap::size() const
{
    return numberOfPairs;
}

unsigned int HashMap::bucketCount() const
{
    return numberOfBuckets;
}

double HashMap::loadFactor() const
{
    return double(numberOfPairs)/numberOfBuckets;
}

unsigned int HashMap::maxBucketSize() const
{
    // hold onto largest linked list size and replace when a larger
    // linked list is found. the one left at the
    // end of the loop will be the largest size.
    unsigned int largest = 0;
    unsigned int linkedListLength;
    for (int i=0; i < numberOfBuckets; i++) {
        linkedListLength = count(buckets[i]);
        if (largest < linkedListLength) {
            largest = linkedListLength;
        }
    }
    return largest;
}

/*
 * Helper member functions defined below here
 */

HashMap::Node* HashMap::traverseAndFind(const std::string& key) const
{
    // start our search in the correct bucket.
    Node* node = buckets[findIndex(key, numberOfBuckets)];
    while (node != nullptr) {
        if (node->key == key) {
            // found our match! return the node
            return node;
        }
        else {
            // have not found match yet, keep searching
            node = node->next;
        }
    }
    // represent no match being found with nullptr
    return nullptr;
}

// copyBuckets and copyLinkedList work together to copy Node**
HashMap::Node** HashMap::copyBuckets(HashMap::Node** oldBuckets, int numberOfBuckets)
{
    // initialize a dynamic array filled with nullptr
    HashMap::Node** newBuckets = new HashMap::Node*[numberOfBuckets]{nullptr};
    for (int i=0; i < numberOfBuckets; ++i) {
        if (oldBuckets[i] != nullptr) {
            // allocate the starting bucket. need to do this here
            // because copyLinkedList is implemented recursively
            newBuckets[i] = new Node;
            copyLinkedList(newBuckets[i], oldBuckets[i]);
        }
        // else: just leave newBuckets[i] as nullptr.
    }
    return newBuckets;
}

void HashMap::copyLinkedList(Node* newNode, Node* oldNode)

{
    // copy the values to the new linked list
    newNode->key = oldNode->key;
    newNode->value = oldNode->value;
    if (oldNode->next == nullptr) {
        // this happens when we found the end of the linked list
        newNode->next = nullptr;
    }
    else {
        // must allocate the Node before passing it into copyLinkedList
        newNode->next = new Node;
        // recursively travel the linked list
        copyLinkedList(newNode->next, oldNode->next);
    }
}

unsigned int defaultHasher(const std::string& key)
{
    unsigned int sum = 0;
    for (int i=0; i < key.size(); ++i) {
        sum += int(key[i]);
    }
    return sum;
}

void HashMap::addToEnd(HashMap::Node* pointerToNode, const std::string& key, const std::string& value)
{
    // Found the end of the linked list, add it there
    if (pointerToNode->next == nullptr) {
        pointerToNode->next = new Node;
        pointerToNode->next->key = key;
        pointerToNode->next->value = value;
        pointerToNode->next->next = nullptr;
    }
    // Move forward through the linked list
    else {
        addToEnd(pointerToNode->next, key, value);
    }
}

// cleanBuckets and cleanBucket work together to deallocate Node**
void HashMap::cleanBuckets(Node** buckets)
{
    // first deallocate each bucket
    for (int i=0; i < numberOfBuckets; ++i) {
        if (buckets[i] != nullptr) {
            cleanBucket(buckets[i]);
        }
    }
    // then deallocate the array itself
    delete[] buckets;
}

void HashMap::cleanBucket(HashMap::Node* link)
{
    // deallocate the bucket from back to front recursively
    if (link->next == nullptr) {
        // this happens at the end of the linked list
        delete link;
    }
    else {
        // delete the links after this link
        cleanBucket(link->next);
        // after each link has everything after it de-allocated, it will deallocate here.
        delete link;
    }
}

int HashMap::findIndex(const std::string &key, int bucketNumber) const
{
    // using % on unsigned int ensures our index will be inside our array
    return hasher(key) % bucketNumber;
}

unsigned int HashMap::count(const Node* node) const
{
    // count number of nodes in linked list by traversing and incrementing
    unsigned int counter = 0;
    while (node != nullptr) {
        counter++;
        node = node->next;
    }
    return counter;
}

void HashMap::rehashBuckets()
{
    // first, make new array
    unsigned int newNumberOfBuckets = (numberOfBuckets * 2) + 1;
    Node** newBuckets = new Node*[newNumberOfBuckets]{nullptr};

    // second, hash all key/values into new array
    Node* current;
    for (int i=0; i < numberOfBuckets; ++i) {
        if (buckets[i] != nullptr) {
            current = buckets[i];
            while (current != nullptr) {
                addKeyValue(newBuckets, newNumberOfBuckets, current->key, current->value);
                current = current->next;
            }
        }
    }
    // third, clear memory of old array, store the new in object
    cleanBuckets(buckets);
    buckets = newBuckets;
    numberOfBuckets = newNumberOfBuckets;
}

void HashMap::addKeyValue(Node** buckets, int bucketNumber,
                 const std::string& key, const std::string& value)
{
    int index = findIndex(key, bucketNumber);

    // This should only happen if bucket is empty
    if (buckets[index] == nullptr) {
        buckets[index] = new Node;
        buckets[index]->key = key;
        buckets[index]->value = value;
        buckets[index]->next = nullptr;
    } else {
        addToEnd(buckets[index], key, value);
    }
}

bool HashMap::weShouldReHash()
{
    // this is same as loadFactor but with + 1 on number of pairs
    // this function will be used before adding a key value pair
    // to know whether to rehash or not
    return (double(numberOfPairs+1)/numberOfBuckets) > .8;
}

// the following two functions were used for testing. they
// picture the graph to console when the command "GRAPH" is entered
void HashMap::printChain(HashMap::Node* link)
{
    if (link != nullptr) {
        std::cout << link->key << ' ' << link->value;
        if (link->next != nullptr) {
            std::cout << " --> ";
            printChain(link->next);
        }
    }
}

void HashMap::printBuckets()
{
    for (int i = 0; i < numberOfBuckets; ++i) {
        printChain(buckets[i]);
        std::cout << " --> End" << std::endl;
    }
}
