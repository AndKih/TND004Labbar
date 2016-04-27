
/*
  Author: Aida Nordman
  Course: TND004, Lab 2
  Description: template class HashTable represents an open addressing hash table
              (also known as closed_hashing) with linear probing
*/

#include "Item.h"

#include <iostream>
#include <iomanip>

using namespace std;

const int NOT_FOUND = -1;
const double MAX_LOAD_FACTOR = 0.5;


//Template class to represent an open addressing hash table using linear probing to resolve collisions
//Internally the table is represented as an array of pointers to Items
template <typename Key_Type, typename Value_Type>
class HashTable
{
public:

    //New type HASH: pointer to a hash function
    typedef unsigned (*HASH)(Key_Type, int);


    //Constructor to create a hash table
    //table_size is number of slots in the table (next prime number is used)
    //f is the hash function
    HashTable(int table_size, HASH f);


    //Destructor
    ~HashTable();


    //Return the load factor of the table, i.e. percentage of slots in use or deleted
    double loadFactor() const
    {
        return (double) (nItems+nDeleted) / _size;
    }


    //Return number of items stored in the table
    unsigned get_number_OF_items() const
    {
        return nItems;
    }

    //Return the total number of visited slots (during search, insert, remove, or re-hash)
    unsigned get_total_visited_slots() const
    {
        return total_visited_slots;
    }

    //Return the total number of call to new Item()
    unsigned get_count_new_items() const
    {
        return count_new_items;
    }


    //Return a pointer to the value associated with key
    //If key does not exist in the table then nullptr is returned
    const Value_Type* _find(const Key_Type& key);


    //Insert the Item (key, v) in the table
    //If key already exists in the table then change the value associated with key to v
    //Re-hash if the table reaches the MAX_LOAD_FACTOR
    void _insert(const Key_Type& key, const Value_Type& v);


    //Remove Item with key, if the item exists
    //If an Item was removed then return true
    //otherwise, return false
    bool _remove(const Key_Type& key);

    //Overloaded subscript operator
    //If key is not in the table then insert a new Item = (key, Value_Type())
    Value_Type& operator[](const Key_Type& key);


    //Display all items in table T to stream os
    friend ostream& operator<<(ostream& os, const HashTable& T)
    {
        for (unsigned i = 0; i < T._size; ++i)
        {
            if (T.hTable[i] && T.hTable[i] != Deleted_Item<Key_Type,Value_Type>::get_Item())
            {
                os << *T.hTable[i] << endl;
            }
        }

        return os;
    }


    //Display the table for debug and testing purposes
    //Thus, empty and deleted entries are also displayed
    void display(ostream& os);


private:

    /* ********************************** *
    * Data members                        *
    * *********************************** */

    //Number of slots in the table, a prime number
    unsigned _size;

    //Hash function
    const HASH h;

    //Number of items stored in the table
    //Instances of Deleted_Items are not counted
    unsigned nItems;

    //Number of slots that are marked as deleted
    unsigned nDeleted;

    //Table is an array of pointers to Items
    //Each slot of the table stores a pointer to an Item =(key, value)
    Item<Key_Type, Value_Type>** hTable;

    //Some statistics
    unsigned total_visited_slots;  //total number of visited slots
    unsigned count_new_items;      //number of calls to new Item()


    /* ********************************** *
    * Auxiliar member functions           *
    * *********************************** */

    void rehash();

    //Disable copy constructor!!
    HashTable(const HashTable &) = delete;

    //Disable assignment operator!!
    const HashTable& operator=(const HashTable &) = delete;

};


//Test if a number is prime
bool isPrime( int n );

//Return a prime number at least as large as n
int nextPrime( int n );


/* ********************************** *
* Member functions implementation     *
* *********************************** */

//Constructor to create a hash table
//table_size number of slots in the table (next prime number is used)
//f is the hash function
template <typename Key_Type, typename Value_Type>
HashTable<Key_Type, Value_Type>::HashTable(int table_size, HASH f)
    : h(f)
{
    //IMPLEMENT
    _size = nextPrime(table_size);
    hTable = new Item<Key_Type, Value_Type>*[_size];
    for(int i = 0; i < _size; ++i)
    {
        hTable[i] = nullptr;
    }
    nDeleted = 0;
    nItems = 0;
}


//Destructor
template <typename Key_Type, typename Value_Type>
HashTable<Key_Type, Value_Type>::~HashTable()
{
    //IMPLEMENT
    delete[] hTable;
}


//Return a pointer to the value associated with key
//If key does not exist in the table then nullptr is returned
template <typename Key_Type, typename Value_Type>
const Value_Type* HashTable<Key_Type, Value_Type>::_find(const Key_Type& key)
{
    //IMPLEMENT
    if(hTable[h(key, _size)])
    {
        return &(hTable[h(key, _size)]->get_value());
    }
    return nullptr;
    //return (new Value_Type()); //delete this code
}


//Insert the Item (key, v) in the table
//If key already exists in the table then change the value associated with key to v
//Re-hash if the table reaches the MAX_LOAD_FACTOR
template <typename Key_Type, typename Value_Type>
void HashTable<Key_Type, Value_Type>::_insert(const Key_Type& key, const Value_Type& v)
{
    //IMPLEMENT
    unsigned index = h(key, _size);
    if(!hTable[index])
    {
        Item<Key_Type, Value_Type> *temp = new Item<Key_Type, Value_Type>(key, v);
        //cout << "Inserting: " << *temp << "\n";
        hTable[index] = temp;
        ++nItems;
    }
    else
    {
        for(; index <= _size; ++index)
        {
            if(index == _size)
            {
                index = 0;
            }
            if(!hTable[index])
            {
                Item<Key_Type, Value_Type> *temp = new Item<Key_Type, Value_Type>(key, v);
                //cout << "Inserting: " << *temp << "\n";
                hTable[index] = temp;
                ++nItems;
                break;
            }
        }
    }
    if(loadFactor() >= MAX_LOAD_FACTOR)
       rehash();
}


//Remove Item with key, if the item exists
//If an Item was removed then return true
//otherwise, return false
template <typename Key_Type, typename Value_Type>
bool HashTable<Key_Type, Value_Type>::_remove(const Key_Type& key)
{
    //IMPLEMENT
    unsigned index = h(key, _size);
    while(hTable[index])
    {
        if(hTable[index]->get_key() != key)
        {
            cout << "Current key: " << hTable[index]->get_key() << "\n";
            ++index;
            if(index == _size)
                index = 0;
            continue;
        }
        hTable[index] = Deleted_Item<Key_Type, Value_Type>::get_Item();
        --nItems;
        ++nDeleted;
        return true;
    }
    return false;
}


//Display the table for debug and testing purposes
//This function is used for debugging and testing purposes
//Thus, empty and deleted entries are also displayed
template <typename Key_Type, typename Value_Type>
void HashTable<Key_Type, Value_Type>::display(ostream& os)
{
    os << "-------------------------------\n";
    os << "Number of items in the table: " << get_number_OF_items() << endl;
    os << "Load factor: " << fixed << setprecision(2) << loadFactor() << endl;

    for (unsigned i = 0; i < _size; ++i)
    {
        os << setw(6) << i << ": ";

        if(!hTable[i])
        {
            os << "null" << endl;
        }
        else if (hTable[i] == Deleted_Item<Key_Type, Value_Type>::get_Item())
        {
            os << "deleted" << endl;
        }
        else
        {
            //cout << "Neither deleted nor null\n";
            os << *hTable[i]
               << "  (" << h(hTable[i]->get_key(),_size) << ")" << endl;
        }
    }

    os << endl;
}


/* ********************************** *
* Auxiliar member functions           *
* *********************************** */
template <typename Key_Type, typename Value_Type>
void HashTable<Key_Type, Value_Type>::rehash()
{
    cout << "Rehashing ...\n";
    nItems = 0;
    unsigned old_size = _size;
    _size = nextPrime(_size*2);
    Item<Key_Type, Value_Type>** oldTable = hTable;
    hTable = new Item<Key_Type, Value_Type>*[_size];
    for(int i = 0; i < _size; ++i)
    {
        hTable[i] = nullptr;
    }
    for(int idt = 0; idt < old_size; ++idt)
    {
        if(oldTable[idt])
        {
            //cout << oldTable[idt]->get_key() << "\n";
            _insert(oldTable[idt]->get_key(), oldTable[idt]->get_value());
        }

    }
    delete[] oldTable;
    cout << "New size = " << _size << "\n";
}


/* ********************************** *
* Functions to find prime numbers     *
* *********************************** */


//Test if a number is prime
bool isPrime( int n )
{
    if( n == 2 || n == 3 )
        return true;

    if( n == 1 || n % 2 == 0 )
        return false;

    for( int i = 3; i * i <= n; i += 2 )
        if( n % i == 0 )
            return false;

    return true;
}


//Return a prime number at least as large as n
int nextPrime( int n )
{
    if( n % 2 == 0 )
        n++;

    for(; !isPrime( n ); n += 2 );

    return n;
}


