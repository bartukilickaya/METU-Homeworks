#ifndef __HASHTABLE__
#define __HASHTABLE__

#include "HashUtils.cpp"
#include "ItemNotFoundException.h"
#include <iostream>
using namespace std;
/* Do not add new libraries or files */

#define BUCKET_SIZE 2

// Do not modify the public interface of this class.
// Otherwise, your code will note compile!
template <class K, class T>
class HashTable {
    struct Entry {
        K Key;             // the key of the entry
        T Value;   // the value of the entry
        bool Deleted;        // flag indicating whether this entry is deleted
        bool Active;         // flag indicating whether this item is currently used

        Entry() : Key(), Value(), Deleted(false), Active(false) {}
    };

    struct Bucket {
        Entry entries[BUCKET_SIZE];
    };

    int _capacity; // INDICATES THE TOTAL CAPACITY OF THE TABLE
    int _size; // INDICATES THE NUMBER OF ITEMS IN THE TABLE

    Bucket* _table; // THE HASH TABLE
    // == DEFINE HELPER METHODS & VARIABLES BELOW ==

public:
    // TODO: IMPLEMENT THESE FUNCTIONS.
    // CONSTRUCTORS, ASSIGNMENT OPERATOR, AND THE DESTRUCTOR
    HashTable();
    // COPY THE WHOLE CONTENT OF RHS INCLUDING THE KEYS THAT WERE SET AS DELETED
    HashTable(const HashTable<K, T>& rhs);
    HashTable<K, T>& operator=(const HashTable<K, T>& rhs);
    ~HashTable();

    // TODO: IMPLEMENT THIS FUNCTION.
    // INSERT THE ENTRY IN THE HASH TABLE WITH THE GIVEN KEY & VALUE
    // IF THE GIVEN KEY ALREADY EXISTS, THE NEW VALUE OVERWRITES
    // THE ALREADY EXISTING ONE. IF THE LOAD FACTOR OF THE TABLE IS GREATER THAN 0.6,
    // RESIZE THE TABLE WITH THE NEXT PRIME NUMBER.
    void Insert(const K& key, const T& value);

    // TODO: IMPLEMENT THIS FUNCTION.
    // DELETE THE ENTRY WITH THE GIVEN KEY FROM THE TABLE
    // IF THE GIVEN KEY DOES NOT EXIST IN THE TABLE, THROW ItemNotFoundException()
    void Delete(const K& key);

    // TODO: IMPLEMENT THIS FUNCTION.
    // IT SHOULD RETURN THE VALUE THAT CORRESPONDS TO THE GIVEN KEY.
    // IF THE KEY DOES NOT EXIST, THROW ItemNotFoundException()
    T& Get(const K& key) const;

    // TODO: IMPLEMENT THIS FUNCTION.
    // AFTER THIS FUNCTION IS EXECUTED THE TABLE CAPACITY MUST BE
    // EQUAL TO newCapacity AND ALL THE EXISTING ITEMS MUST BE REHASHED
    // ACCORDING TO THIS NEW CAPACITY.
    // WHEN CHANGING THE SIZE, YOU MUST REHASH ALL OF THE ENTRIES
    void Resize(int newCapacity);

    // THE IMPLEMENTATION OF THESE FUNCTIONS ARE GIVEN TO YOU
    // DO NOT MODIFY!
    int Capacity() const; // RETURN THE TOTAL CAPACITY OF THE TABLE
    int Size() const; // RETURN THE NUMBER OF ACTIVE ITEMS
    void getKeys(K* keys); // PUT THE ACTIVE KEYS TO THE GIVEN INPUT PARAMETER
};


template <class K, class T>
HashTable<K, T>::HashTable() {
    // TODO: IMPLEMENT THIS FUNCTION.
    _size = 0;
    _capacity = 7;
    _table = new Bucket[_capacity];
}

template <class K, class T>
HashTable<K, T>::HashTable(const HashTable<K, T>& rhs) {
    // TODO: IMPLEMENT THIS FUNCTION.
    _size = rhs.Size();
    _capacity = rhs.Capacity();
    _table = new Bucket[_capacity];
    for(int i = 0 ; i < rhs._capacity; i++){
        _table[i] = rhs._table[i];
    }
}

template <class K, class T>
HashTable<K, T>& HashTable<K, T>::operator=(const HashTable<K, T>& rhs) {
    // TODO: IMPLEMENT THIS FUNCTION.
    delete [] _table;
    _capacity = rhs.Capacity();
    _size = rhs.Size();
    _table = new Bucket[_capacity];
    for(int i = 0; i < _capacity;i++){
        _table[i].entries[0].Deleted = rhs._table[i].entries[0].Deleted;
        _table[i].entries[0].Active = rhs._table[i].entries[0].Active;
        _table[i].entries[0].Key = rhs._table[i].entries[0].Key;
        _table[i].entries[0].Value = rhs._table[i].entries[0].Value;
        _table[i].entries[1].Deleted = rhs._table[i].entries[1].Deleted;
        _table[i].entries[1].Active = rhs._table[i].entries[1].Active;
        _table[i].entries[1].Key = rhs._table[i].entries[1].Key;
        _table[i].entries[1].Value = rhs._table[i].entries[1].Value;
    }
    return *this;
}

template <class K, class T>
HashTable<K, T>::~HashTable() {
    // TODO: IMPLEMENT THIS FUNCTION.
    delete [] _table;
}

template <class K, class T>
void HashTable<K, T>::Insert(const K& key, const T& value) {
    // TODO: IMPLEMENT THIS FUNCTION.
    //cout << _size << ":::" << _capacity << endl;
    //cout << _size / (2.*_capacity) << "q:" << endl;
    if(_size / (2.*_capacity) > 0.6){
        //cout << _size  << "::::" << endl;
        _size = 0;
        Resize(_capacity);
    }
    //cout << _size / (2.*_capacity) << endl;
    int h,probe = 0,flag = 0,i;
    h = Hash(key);
    while(1){
        h = (h + probe*probe) % _capacity;
        for(i = 0; i < 2; i++){
            if(!_table[h].entries[i].Deleted && _table[h].entries[i].Key == key && _table[h].entries[i].Active ){
                _table[h].entries[i].Value = value;
                flag = 1;
                break;
            }
            else if(!_table[h].entries[i].Active){
                flag = 2;
                break;
            }
        }
        if(flag){
            break;
        }
        else{
            probe++;
        }
    }
    if(flag == 2){
        flag = 0;
        probe = 0;
        h = Hash(key);
        while(1){
            h = (h + probe*probe) % _capacity;
            for(int i = 0; i < 2; i++){
                if( !_table[h].entries[i].Active || _table[h].entries[i].Deleted){
                    //cout << key << "::" << endl;
                    flag = 1;
                    _table[h].entries[i].Active = true;
                    _table[h].entries[i].Value = value;
                    _table[h].entries[i].Key = key;
                    _table[h].entries[i].Deleted = false;
                    _size++;
                    break;
                }

            }
            if(flag){
                break;
            }
            else{
                probe++;
            }
        }
    }

    //cout << _table[1].entries[0].Key << endl;
}
template <class K, class T>
void HashTable<K, T>::Delete(const K& key) {
    // TODO: IMPLEMENT THIS FUNCTION.
    int h,probe = 0,flag = 0,i;
    h = Hash(key);
    while(1){
        h = (h + probe*probe) % _capacity;
        for(i = 0 ; i < 2; i++){
            if(_table[h].entries[i].Active && _table[h].entries[i].Key == key && !_table[h].entries[i].Deleted){
                flag = 1;
                _table[h].entries[i].Deleted = true;
                _table[h].entries[i].Active = true;
                _size--;
                break;
            }
            else if(!_table[h].entries[i].Active){
                throw ItemNotFoundException();
            }
        }
        if(flag){
            break;
        }
        probe++;
    }
}

template <class K, class T>
T& HashTable<K, T>::Get(const K& key) const {
    // TODO: IMPLEMENT THIS FUNCTION. 
    int h,probe = 0,i;
    h = Hash(key);
    while(1){
        h = (h + probe*probe) % _capacity;
        for(i = 0 ; i < 2; i++){
            if(!_table[h].entries[i].Deleted && _table[h].entries[i].Active && (_table[h].entries[i].Key == key) ){
                return _table[h].entries[i].Value;
            }
            else if(!_table[h].entries[i].Active){
                throw ItemNotFoundException();
            }
        }
        probe++;
    }
}


template <class K, class T>
void HashTable<K, T>::Resize(int newCapacity) {
    Bucket * tmp = new Bucket[newCapacity];
    _capacity = NextCapacity(_capacity);
    for(int i = 0 ; i < newCapacity; i++) {
        tmp[i].entries[0].Deleted = _table[i].entries[0].Deleted;
        tmp[i].entries[0].Active = _table[i].entries[0].Active;
        tmp[i].entries[0].Key = _table[i].entries[0].Key;
        tmp[i].entries[0].Value = _table[i].entries[0].Value;
        tmp[i].entries[1].Deleted = _table[i].entries[1].Deleted;
        tmp[i].entries[1].Active = _table[i].entries[1].Active;
        tmp[i].entries[1].Key = _table[i].entries[1].Key;
        tmp[i].entries[1].Value = _table[i].entries[1].Value;
    }
    delete []_table;
    //cout << _capacity << ":a:" << _size << endl;
    _table = new Bucket[_capacity];
    for(int i = 0 ; i < newCapacity; i++){
        for(int j = 0; j < 2; j++){
            if(tmp[i].entries[j].Active && !tmp[i].entries[j].Deleted){
                //cout << tmp[i].entries[j].Key << "::" << endl;
                Insert(tmp[i].entries[j].Key,tmp[i].entries[j].Value);
                //cout << tmp[i].entries[j].Key << "::" << endl;
                //_size++;
            }
        }

    }
    delete []tmp;
}



template <class K, class T>
int HashTable<K, T>::Capacity() const {
    return _capacity;
}

template <class K, class T>
int HashTable<K, T>::Size() const {
    return _size;
}


template <class K, class T>
void HashTable<K, T>::getKeys(K* keys) {
    int index = 0;
    for (int i = 0; i < _capacity; i++) {
        Bucket& bucket = _table[i];
        for (int j = 0; j < BUCKET_SIZE; j++) {
            if (bucket.entries[j].Active && !bucket.entries[j].Deleted) {
                keys[index++] = bucket.entries[j].Key;
            }
        }
    }
}

#endif