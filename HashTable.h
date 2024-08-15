
//created by Metehan Pamuklu
#include <vector>
#include <cmath>
#include <string>
#include <iostream>

using namespace std;

//necesarry data structures to hold data in hashtable

enum EntryType { ACTIVE, EMPTY, DELETED };
//defining hash table functions
template <class Key, class Value>
struct HashEntry
{
    Key key;
    Value value;
    EntryType info= EMPTY;
};
//all taken from lecture slides
template <class Key, class Value>
class HashTable
{
public:
    HashTable(const Key& notFound, int size);
    HashTable(const HashTable& rhs)
        : ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND),
        array(rhs.array), currentSize(rhs.currentSize) {}

    const HashEntry<Key, Value> find(const Key& x);
    int hash_1(const Key& key, int tableSize);

    void makeEmpty();
    void insert(const Key& x, const Value& y);
    void remove(const Key& x);

    int getTableSize();
    double getLoadFactor();

private:
    vector<HashEntry<Key, Value>> array;
    int currentSize;
    Key ITEM_NOT_FOUND;

    bool isActive(int currentPos) const;
    int findPos(const Key& x);
    void rehash();
};
int hash_1(const string & key, int tableSize);

//taken from lecture slides
//implementing hash table functions
bool isPrime(int n)
{
    if (n == 2 || n == 3)
        return true;

    if (n == 1 || n % 2 == 0)
        return false;

    for (int i = 3; i * i <= n; i += 2)
        if (n % i == 0)
            return false;

    return true;
}
int nextPrime(int n)
{
    if (n % 2 == 0)
        n++;

    for (; !isPrime(n); n += 2)
        ;

    return n;
}

template <class Key, class Value>
HashTable<Key, Value>::HashTable(const Key& notFound,
    int size)
    : ITEM_NOT_FOUND(notFound), currentSize(0), array(nextPrime(size)) {}

template <class Key, class Value>
int HashTable<Key, Value>::hash_1(const Key& key, int tableSize)
{
    int hashVal = 0;

    for (int i = 0; i < key.length(); i++)
        hashVal = 37 * hashVal + key[i];

    hashVal = hashVal % tableSize;

    if (hashVal < 0)
        hashVal = hashVal + tableSize;

    return(hashVal);
}
template <class Key, class Value>
const HashEntry<Key, Value> HashTable<Key, Value>::find(const Key& x)
{
    HashEntry<Key, Value> notFound;
    notFound.key = ITEM_NOT_FOUND;

    int currentPos = findPos(x);
    if (isActive(currentPos))
        return array[currentPos];

    return notFound;
}

template <class Key, class Value>
void HashTable<Key, Value>::insert(const Key& x, const Value& y)
{
    // Insert x as active
    int currentPos = findPos(x);
    if (isActive(currentPos))
        return;

    HashEntry<Key, Value> itemToAdd;
    itemToAdd.key = x;
    itemToAdd.value = y;
    itemToAdd.info = ACTIVE;

    array[currentPos] = itemToAdd;
    double loadFactor = (double(++currentSize)) / (double(array.size())); // Defined the load factor

    // enlarge the hash table if necessary
    if (loadFactor >= 0.5) {
        cout << "rehashed..." << endl;
        cout << "previous table size:" << array.size();
        rehash();
        double curLoadFactor = double(currentSize / double(array.size()));
        cout << ", new table size: " << array.size() << ", current unique word count "
            << currentSize << ", current load factor: " << curLoadFactor << endl;
    }
}

template <class Key, class Value>
int HashTable<Key, Value>::getTableSize() {
    return currentSize;
}
template <class Key, class Value>
double HashTable<Key, Value>::getLoadFactor() {
    return double(currentSize) / double(array.size());
}


//quadratic probing
template <class Key, class Value>
int HashTable<Key, Value>::findPos(const Key& x) {
    int collisionNum = 0;
    int currentPos = hash_1(x, array.size());

    while (array[currentPos].info != EMPTY &&
        array[currentPos].key != x)
    {

        currentPos += (++collisionNum * collisionNum);
        if (currentPos >= array.size())              // perform the mod
            currentPos = currentPos - array.size();                 // if necessary
    }
    return currentPos;
}
 
template <class Key, class Value>
bool HashTable<Key, Value>::isActive(int currentPos) const
{
    return array[currentPos].info == ACTIVE;
}

template <class Key, class Value>
void HashTable<Key, Value>::rehash()
{
    vector<HashEntry<Key, Value>> oldArray = array;

    // Create new double-sized, empty table
    array.resize(nextPrime(2 * oldArray.size()));
    for (int j = 0; j < array.size(); j++)
        array[j].info = EMPTY;

    // Copy table over
    currentSize = 0;
    for (int i = 0; i < oldArray.size(); i++)
        if (oldArray[i].info == ACTIVE)
            insert(oldArray[i].key, oldArray[i].value);
}

template <class Key, class Value>
void HashTable<Key, Value>::makeEmpty()
{
    for (int i = 0; i < array.size(); i++)
    {
        array[i].info = EMPTY;
    }
}
template <class Key, class Value>
void HashTable<Key, Value>::remove(const Key& x)
{
    int currentPos = findPos(x);
    if (isActive(currentPos))
        array[currentPos].info = DELETED;
}






