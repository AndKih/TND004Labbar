#ifndef SET_H_INCLUDED
#define SET_H_INCLUDED

#include "memory"
#include "utility"
#include "iostream"

using namespace std;

template <typename T>
class Set{
public:
    Set();
    Set(const T& val);
    Set(const T val[], int n);
    Set(const Set& s);
    Set(const Set&& s) noexcept;
    ~Set();

    bool _empty() const;
    int cardinality() const;
    bool is_member(const T& val) const;
    void make_empty();

    Set operator=(Set s);

    Set& operator+=(const Set& s);
    Set& operator*=(const Set& s);
    Set& operator-=(const Set& s);
    bool operator==(const Set& s) const;
    bool operator!=(const Set& s) const;
    bool operator<(const Set& s) const;
    bool operator<=(const Set& s) const;
    friend Set operator+(const Set& a, const Set& b)
    {
        return move(a + b);
    }
    friend Set operator-(const Set& a, const Set& b)
    {
        return move(a - b);
    }
    friend Set operator*(const Set& a, const Set& b)
    {
        return move(a * b);
    }
    friend ostream& operator<<(ostream& os, const Set<T>& s)
{
    if(s._empty())
    {
        os << "Set is empty!";
    }
    else
    {
        shared_ptr<Node> temp = s.head->next;
        while(temp->next)
        {
            os << temp->value;
            temp = temp->next;
        }
    }
    return os;
}

private:
    class Node
    {
    public:

        Node(T nodeVal, shared_ptr<Node> nextPtr, weak_ptr<Node> prevPtr) :
               value{nodeVal}, next{nextPtr}, prev{prevPtr}{}
//        Node(T&& nodeVal, shared_ptr<Node> nextPtr, weak_ptr<Node> prevPtr) :
//               value{nodeVal}, next{nextPtr}, prev{prevPtr}{}

        T value;
        shared_ptr<Node> next;
        weak_ptr<Node> prev;

    };
    shared_ptr<Node> tail;
    shared_ptr<Node> head;
    void insert(const T& Val);
};

template<typename T>
Set<T>::Set()
{
    head = make_shared<Node>(T(), nullptr, weak_ptr<Node>());
    tail = make_shared<Node>(T(), nullptr, head);
    head->next = tail;
}
template<typename T>
Set<T>::Set(const T& val) //conversion constructor
{
    head = make_shared<Node>(T(), nullptr, weak_ptr<Node>());
    tail = make_shared<Node>(T(), nullptr, head);
    head->next = tail;
    insert(val);
}
template<typename T>
Set<T>::Set(const T val[], int n)
{
    head = make_shared<Node>(T(), nullptr, weak_ptr<Node>());
    tail = make_shared<Node>(T(), nullptr, head);
    head->next = tail;
    for(int idx = 0; idx < n; ++idx)
    {
        //Node temp = new Node(val[idx], head->next, head)
        insert(val[idx]);
    }
}
template<typename T>
Set<T>::Set(const Set& s)
{
    head = make_shared<Node>(T(), nullptr, weak_ptr<Node>());
    tail = make_shared<Node>(T(), nullptr, head);
    head->next = tail;
    shared_ptr<Node> tempPtr = s.head->next;
    while(tempPtr->next)
    {
        insert(tempPtr->value);
        tempPtr = tempPtr->next;
    }
}
template<typename T>
Set<T>::Set(const Set&& s) noexcept
{
    head = make_shared<Node>(T(), nullptr, weak_ptr<Node>());
    tail = make_shared<Node>(T(), nullptr, head);
    head->next = tail;
    shared_ptr<Node> tempPtr = s.head->next;
    while(tempPtr->next)
    {
        insert(tempPtr->value);
        tempPtr = tempPtr->next;
    }
}
template<typename T>
Set<T>::~Set()
{

}
template<typename T>
void Set<T>::insert(const T& val)
{
    shared_ptr<Node> temp = make_shared<Node>(val, tail, tail->prev);
    shared_ptr<Node> tempPtr = tail->prev.lock();
    tail->prev = temp;
    tempPtr->next = temp;
}

template<typename T>
bool Set<T>::_empty() const
{
    if(!head->next->next)
        return true;
    else
        return false;
}
template<typename T>
int Set<T>::cardinality() const
{
    return 1;
}
template<typename T>
bool Set<T>::is_member(const T& val) const
{
    return false;
}
template<typename T>
void Set<T>::make_empty()
{
    return;
}

template<typename T>
Set<T> Set<T>::operator=(Set s)
{
    //cout << "S2: " << s2 << "\n";
    return Set<T>(s);
}
template<typename T>
Set<T>& Set<T>::operator+=(const Set& s)
{
    return *this;
}
template<typename T>
Set<T>& Set<T>::operator*=(const Set& s)
{
    return *this;
}
template<typename T>
Set<T>& Set<T>::operator-=(const Set& s)
{
    return *this;
}
template<typename T>
bool Set<T>::operator==(const Set& s) const
{
    return false;
}
template<typename T>
bool Set<T>::operator!=(const Set& s) const
{
    return false;
}
template<typename T>
bool Set<T>::operator<(const Set& s) const
{
    return false;
}
template<typename T>
bool Set<T>::operator<=(const Set& s) const
{
    return false;
}

//template<typename T>
//Set<T>::Set()
//    {
//        tail = nullptr;
//        head = nullptr;
//    }

#endif // SET_H_INCLUDED
