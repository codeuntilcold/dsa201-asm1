#include <sstream>
#include <string>
#include <iostream>
#include <type_traits>
using namespace std;

#ifndef ILIST_H
#define ILIST_H

template <class T>
class IList
{
public:
    virtual void add(const T& element) = 0;
    virtual void add(int index, const T& element) = 0;
    virtual T removeAt(int index) = 0;
    virtual bool removeItem(const T& item) = 0;
    virtual bool empty() = 0;
    virtual int size() = 0;
    virtual void clear() = 0;
    virtual T& get(int index) = 0;
    virtual void set(int index, const T& element) = 0;
    virtual int indexOf(const T& item) = 0;
    virtual bool contains(const T& item) = 0;
    virtual string toString() = 0;
};

#endif

// STUDENT ANSWER
#ifndef FRAGMENT_LINKED_LIST
#define FRAGMENT_LINKED_LIST
template <class T>
class FragmentLinkedList : IList<T>
{
public:
    class Node;
    class Iterator;

protected:
    Node **fragmentPointers;
    int fragmentMaxSize;
    int count;

    int nFragPtr() const;           // Number of pointers
    bool full() const;              // All fragments are full
    bool full_plus_one() const;     // Last fragment is last node

public:
    FragmentLinkedList(int fragmentMaxSize = 5)
    {
        this->count = 0;
        this->fragmentMaxSize = fragmentMaxSize;
        this->fragmentPointers = new Node *[2];
        this->fragmentPointers[0] = NULL;
        this->fragmentPointers[1] = NULL;
    }
    virtual ~FragmentLinkedList();
    virtual void add(const T& element);
    virtual void add(int index, const T& element);
    virtual T removeAt(int index);
    virtual bool removeItem(const T& item);
    virtual bool empty();
    virtual int size();
    virtual void clear();
    virtual T& get(int index);
    virtual void set(int index, const T& element);
    virtual int indexOf(const T& item);
    virtual bool contains(const T& item);
    virtual string toString();
    Iterator begin(int index = 0);
    Iterator end(int index = -1);

    void printHeadOfFragments()
    {
        if (count) 
        {
            cout << "HEADS:\t";
            for (int i = 0; i < nFragPtr(); i++)
            {
                cout << fragmentPointers[i]->data << '\t';
            }
        }
        cout << '\n';
    }

public:
    class Node
    {
    private:
        T data;
        Node *next;
        Node *prev;
        friend class FragmentLinkedList<T>;

    public:
        Node()
        {
            next = 0;
            prev = 0;
        }
        Node(Node *next, Node *prev)
        {
            this->next = next;
            this->prev = prev;
        }
        Node(T data, Node *next, Node *prev)
        {
            this->data = data;
            this->next = next;
            this->prev = prev;
        }
    };

    class Iterator
    {
    private:
        FragmentLinkedList<T> *pList;
        Node *pNode;

    public:
        Iterator(FragmentLinkedList<T> *pList = 0, bool begin = true);
        Iterator(FragmentLinkedList<T> *pList = 0, int fragmentIndex = 0, bool begin = true);
        Iterator &operator=(const Iterator &iterator);
        T &operator*();
        bool operator!=(const Iterator &iterator);
        void remove();
        void set(const T& element);
        Iterator &operator++();
        Iterator operator++(int);

        bool operator==(const Iterator &iterator);

    };
};


/********************************************************
 *                                                      *
 *           FRAGMENT LINKED LIST DEFINITIONS           *
 *                                                      *
*********************************************************/

template <class T>
int FragmentLinkedList<T>::nFragPtr() const    // Number of pointers
{
    if (count) 
    {
        if (count % fragmentMaxSize == 0)
            return count / fragmentMaxSize + 1;
        else
            return count / fragmentMaxSize + 2;
    }
    else return 2;
}

template <class T>
bool FragmentLinkedList<T>::full() const // All fragments are full
{
    return (count % fragmentMaxSize == 0 && count);
}

template <class T>
bool FragmentLinkedList<T>::full_plus_one() const
{
    return (count % fragmentMaxSize == 1);
}

template <class T>
FragmentLinkedList<T>::~FragmentLinkedList()
{
    clear();
    if (fragmentPointers) { delete[] this->fragmentPointers; }
}

template <class T>
void    FragmentLinkedList<T>::add(const T& element)
{
    if (empty())
    {
        fragmentPointers[0] = fragmentPointers[1] = new Node(element, NULL, NULL);
    }
    else if (!full())
    {
        Node* & tail = fragmentPointers[nFragPtr() - 1];

        tail->next = new Node(element, NULL, tail);
        tail = tail->next;
    }
    else
    {
        // Make room and move data to new array
        int sizeofNewArray = nFragPtr() + 1;

        Node** temp = new Node* [sizeofNewArray];
        for (int i = 0; i < sizeofNewArray - 1; i++) {
            temp[i] = this->fragmentPointers[i];
        }
        delete[] this->fragmentPointers;

        this->fragmentPointers = temp;

        // Add new node
        Node* & newFrag     =   this->fragmentPointers[sizeofNewArray - 2];
        Node* & tail        =   this->fragmentPointers[sizeofNewArray - 1];
        newFrag->next = new Node(element, NULL, newFrag);
        
        newFrag = newFrag->next;
        tail = newFrag;
    }
    
    count++;
}

template <class T>
void    FragmentLinkedList<T>::add(int index, const T& element)
{
    if (index < 0 || index > count)
    {
        throw out_of_range("The index is out of range");
    }

    // Add the element to the end of the list
    add(element);
    if (count == 1 || index == count - 1) { return; }

    // Putting the added element to the appropriate position
    int idx     = index / fragmentMaxSize;
    int step    = index % fragmentMaxSize;
    Node* p             = fragmentPointers[idx];
    Node* tail          = fragmentPointers[ nFragPtr() - 1 ];
    for (int i = 0; i < step; i++) 
    {
        p = p->next;
    }
    fragmentPointers[ nFragPtr() - 1 ] = tail->prev;
    if (full_plus_one()) 
    {
        fragmentPointers[ nFragPtr() - 2 ] = tail->prev;
    }
    tail->prev->next    = NULL;
    tail->prev          = p->prev;
    tail->next          = p;
    if (p->prev) { p->prev->next = tail; }
    p->prev             = tail; 
    
    // Change the fragment pointers
    int beginMoving     = index / fragmentMaxSize + ( (index % fragmentMaxSize) ? 1 : 0 );
    int stopMoving      = nFragPtr() - ( full_plus_one() ? 2 : 1 );
    for (int i = beginMoving; i < stopMoving; i++) 
    {
        fragmentPointers[i] = fragmentPointers[i]->prev;
    }
}

template <class T>
T       FragmentLinkedList<T>::removeAt(int index)
{
    if (index < 0 || index >= count)
    {
        throw out_of_range("The index is out of range");
    }

    // Cases of removing
    // 1. Removing the only node
    // 2. Remove and not resizing: Noble nodes and Normie nodes
    // 3. Removing and resizing: Nurishing nodes, Noble nodes and Normie nodes

    if (count == 1)
    {
        T temp = fragmentPointers[0]->data;
        delete fragmentPointers[0];
        fragmentPointers[0] = fragmentPointers[1] = NULL;
        count--;
        return temp;
    }

    int idx     = index / fragmentMaxSize;
    int step    = index % fragmentMaxSize;
    Node* p     = fragmentPointers[idx];
    for (int i = 0; i < step; i++) {
        p = p->next;
    }

    T temp = p->data;

    if (full_plus_one())
    {
        int sizeofNewArray = nFragPtr() - 1;

        Node** temp = new Node* [sizeofNewArray];
        for (int i = 0; i < sizeofNewArray; i++) {
            temp[i] = this->fragmentPointers[i];
        }
        delete[] this->fragmentPointers;
        this->fragmentPointers = temp;
    }

    int beginMoving     = index / fragmentMaxSize + ( (index % fragmentMaxSize) ? 1 : 0 );
    int stopMoving      = nFragPtr() - ( (full_plus_one()) ? 2 : 1 );
    for (int i = beginMoving; i < stopMoving; i++) 
    {
        fragmentPointers[i] = fragmentPointers[i]->next;
    }
    
    if (p->prev) p->prev->next = p->next;
    if (p->next) 
    {
        p->next->prev = p->prev;
    }
    else
    {
        if (full_plus_one())
            fragmentPointers[nFragPtr() - 2] = p->prev;
        else
            fragmentPointers[nFragPtr() - 1] = p->prev;
    }
    
    delete p;
    count--;

    return temp;
}

template <class T>
bool    FragmentLinkedList<T>::removeItem(const T& item)
{
    int index = indexOf(item);
    if (index >= 0) 
    {
        removeAt(index);
        return true;
    }
    else 
    {
        return false;
    }
}

template <class T>
bool    FragmentLinkedList<T>::empty()
{
    return !(this->count);
}

template <class T>
int     FragmentLinkedList<T>::size()
{
    return this->count;
}

template <class T>
void    FragmentLinkedList<T>::clear()
{
    Node* ptr = this->fragmentPointers[0];
    Node* prev = ptr;
    while (ptr != NULL)
    {
        prev = ptr;
        ptr = ptr->next;
        delete prev;
    }

    delete[] this->fragmentPointers;
    this->fragmentPointers = new Node* [2];
    this->fragmentPointers[0] = this->fragmentPointers[1] = NULL;
    
    count = 0;
}

template <class T>
T&      FragmentLinkedList<T>::get(int index)
{
    if (index < 0 || index >= count)
    {
        throw std::out_of_range("The index is out of range");
    }

    int fragIdx = index / this->fragmentMaxSize;
    int step = index % this->fragmentMaxSize;

    Node* p = this->fragmentPointers[fragIdx];
    for (int i = 0; i < step; i++) {
        p = p->next;
    }
    return p->data;
}

template <class T>
void    FragmentLinkedList<T>::set(int index, const T& element)
{
    if (index < 0 || index >= count)
    {
        throw std::out_of_range("The index is out of range");
    }
    
    int fragIdx = index / this->fragmentMaxSize;
    int step = index % this->fragmentMaxSize;

    Node* p = this->fragmentPointers[fragIdx];
    for (int i = 0; i < step; i++) {
        p = p->next;
    }

    p->data = std::move(element);
}

template <class T>
int     FragmentLinkedList<T>::indexOf(const T& item)
{
    Node* p = this->fragmentPointers[0];
    int index = 0;
    while (p && p->data != item) {
        p = p->next;
        index++;
    }
    if (p) {
        return index;
    }
    else {
        return -1;
    }
}

template <class T>
bool    FragmentLinkedList<T>::contains(const T& item)
{
    return (indexOf(item) >= 0);
}

template <class T>
string FragmentLinkedList<T>::toString()
{
    stringstream ss;
    ss << "[";
    Node *ptr = this->fragmentPointers[0];

    if (this->count == 0)
        ss << "]";

    // TODO
    if (ptr) { ss << ptr->data; ptr = ptr->next; }
    while (ptr != NULL)
    {
        ss << ", " <<ptr->data;
        ptr = ptr->next;
    }
    if (this->count) ss << "]";
    // END: TODO

    return ss.str();
}

template <class T>
typename FragmentLinkedList<T>::Iterator FragmentLinkedList<T>::begin(int index)
{
    // Default index is 0
    FragmentLinkedList<T>::Iterator it = FragmentLinkedList<T>::Iterator(this, index, true);
    return it;
}

template <class T>
typename FragmentLinkedList<T>::Iterator FragmentLinkedList<T>::end(int index)
{
    // Default index is -1
    // Return the Iterator of the node 'after'
    if (index >= 0)
    {
        FragmentLinkedList<T>::Iterator it = FragmentLinkedList<T>::Iterator(this, index, false);
        return it;
    }
    else
    {
        FragmentLinkedList<T>::Iterator it = FragmentLinkedList<T>::Iterator(this, false);
        return it;
    }
}


/********************************************************
 *                                                      *
 *                  ITERATOR DEFINITIONS                *
 *                                                      *
*********************************************************/

template <class T>
FragmentLinkedList<T>::Iterator::Iterator(FragmentLinkedList<T> *pList, bool begin)
{
    this->pList = pList;
    if (begin)
    {
        this->pNode = pList->fragmentPointers[0];
    }
    else
    {
        this->pNode = nullptr;
    }
}

template <class T>
FragmentLinkedList<T>::Iterator::Iterator(FragmentLinkedList<T> *pList, int fragmentIndex, bool begin)
{
    if (fragmentIndex < 0 || fragmentIndex >= pList->nFragPtr())
    {
        throw out_of_range("The index is out of range!");
    }
    this->pList = pList;
    if (begin)
    {
        this->pNode = pList->fragmentPointers[fragmentIndex];
    }
    else
    {
        if (fragmentIndex < pList->nFragPtr() - 1)
            this->pNode = pList->fragmentPointers[fragmentIndex + 1];
        else
        {
            // End of list
            this->pNode = NULL;
        }
    }
}

template <class T>
typename FragmentLinkedList<T>::Iterator&   FragmentLinkedList<T>::Iterator::operator=(const Iterator& iterator)
{
    this->pList = iterator.pList;
    this->pNode = iterator.pNode;
    return *this;
}

template <class T>
T&          FragmentLinkedList<T>::Iterator::operator*()
{
    return this->pNode->data;
}

template <class T>
bool        FragmentLinkedList<T>::Iterator::operator!=(const Iterator &iterator)
{
    return (this->pList != iterator.pList || this->pNode != iterator.pNode);
}

template <class T>
bool        FragmentLinkedList<T>::Iterator::operator==(const Iterator &iterator)
{
    return (this->pList == iterator.pList && this->pNode == iterator.pNode);
}

template <class T>
void        FragmentLinkedList<T>::Iterator::remove()
{
    if (*this == pList->end()) { return; }

    Node* p = pList->fragmentPointers[0];
    int index = 0;

    while (p != pNode)
    {
        p = p->next;
        index++;
    }

    // pNode = pNode->next;
    pList->removeAt(index);
}

template <class T>
void        FragmentLinkedList<T>::Iterator::set(const T& element)
{
    this->pNode->data = std::move(element);
}

template <class T>
typename FragmentLinkedList<T>::Iterator&   FragmentLinkedList<T>::Iterator::operator++()           // Prefix
{
    // while (this->pNode->next != nullptr)
    this->pNode = this->pNode->next;
    return *this;
}

template <class T>
typename FragmentLinkedList<T>::Iterator    FragmentLinkedList<T>::Iterator::operator++(int step)   // Postfix
{
    FragmentLinkedList<T>::Iterator prev = *this;
    ++(*this);
    return prev;
}

#endif
// END: STUDENT ANSWER

int main()
{
    FragmentLinkedList<int> fList(5);

    // for (int i = 0; i < 5; i++)
    //     fList.add(i);

    // cout << "Size is -- " << fList.size() << " -- and list is " << fList.toString() << '\n';
    // fList.printHeadOfFragments();
    
    // for (int i = 0; i < 5; i++)
    //     fList.add(2 * i, i * i);
    
    // fList.add(0, 15);
    // fList.add(1, 16);
    // fList.add(3, 18);
    // fList.add(5, 19);
    // fList.add(9, 20);
    // fList.add(10, 300);
    // cout << "Size is -- " << fList.size() << " -- and list is " << fList.toString() << '\n';
    // fList.printHeadOfFragments();
    
    // cout << "Remove at:\t\t" << fList.removeAt(0) << '\n';
    // cout << "Remove item:\t\t" << fList.removeItem(0) << '\n';
    // cout << "Size is -- " << fList.size() << " -- and list is " << fList.toString() << '\n';
    // fList.printHeadOfFragments();
    
    // cout << "Index of item:\t\t" << fList.indexOf(22) << '\n';
    // cout << "Remove item:\t\t" << fList.removeItem(22) << '\n';
    // cout << "Remove at:\t\t" << fList.removeAt(fList.size() - 1) << '\n';
    // cout << "Size is -- " << fList.size() << " -- and list is " << fList.toString() << '\n';
    // fList.printHeadOfFragments();

    // fList.clear();
    for (int i = 0; i < 21; i++)
        if (i % 2)
        {
            fList.add(i - 1, i * i);
        }    
        else
        {
            fList.add(i);
        }
    
    cout << "Size is -- " << fList.size() << " -- and list is " << fList.toString() << '\n';
    fList.printHeadOfFragments();

    if (fList.contains(169)) fList.removeItem(169);
    cout << "Size is -- " << fList.size() << " -- and list is " << fList.toString() << '\n';
    fList.printHeadOfFragments();



    fList.clear();
    cout << "Cleared: " << fList.toString();

    // for(FragmentLinkedList<int>::Iterator it = fList.begin(); it != fList.end(6); ++it)
    // {
    //     cout << *it << " ";
    // }

    cout << endl;

    return 0;
}
