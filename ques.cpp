#include <stdio.h>
#include <iostream>
#include <cstring>
using namespace std;

class Book
{
private:
    char* title;
    char* authors;
    int publishingYear;

public:
    Book()
    {
        title = authors = nullptr;
        publishingYear = 0;
    }

    Book(const char* title, const char* authors, int publishingYear)
    {
        this->title = new char[strlen(title)];
        strcpy(this->title, title);

        this->authors = new char[strlen(authors)];
        strcpy(this->authors, authors);

        this->publishingYear = publishingYear;
    }

    Book(const Book &book)
    {
        this->title = new char[strlen(book.title)];
        strcpy(this->title, book.title);

        this->authors = new char[strlen(book.authors)];
        strcpy(this->authors, book.authors);

        this->publishingYear = book.publishingYear;
    }

    ~Book()
    {
        if (this->title) delete[] title;
        if (this->authors) delete[] authors;
    }

    friend bool checkAuthor(Book book, const char* author)
    {
        char* pch;
        pch = strtok(book.authors, ",");
    
        while (pch != nullptr) 
        {
            if (strcmp(pch, author) == 0) return true;
            if (strcmp(pch + 1, author) == 0) return true;
            pch = strtok(NULL, ",");
        }
        return false;
    }
    
    void setTitle(const char* title)
    {
        this->title = new char[strlen(title)];
        strcpy(this->title, title);
    }

    void setAuthors(const char* authors)
    {
        this->authors = new char[strlen(authors)];
        strcpy(this->authors, authors);
    }

    void setPublishingYear(int publishingYear)
    {
        this->publishingYear = publishingYear;
    }

    char* getTitle() const
    {
        return this->title;
    }

    char* getAuthors() const
    {
        return this->authors;
    }

    int getPublishingYear() const
    {
        return this->publishingYear;
    }

    void printBook(){
        printf("%s\n%s\n%d", this->title, this->authors, this->publishingYear);
    }

    friend class Printer;
};
class Printer
{
public:
    static void printBook(const Book book)
    {
        cout << book.title << '\n';
        char* pch;

        pch = strtok(book.authors, ",");
        cout << pch << '\n';
        pch = strtok(nullptr, ",");

        while (pch != nullptr)
        {
            if (pch) cout << pch + 1 << '\n';
            cout << "n00b\n";
            pch = strtok(nullptr, ",");
        }

        cout << book.publishingYear;
    }
};
enum Color
{
    red,
    green,
    blue
};
enum Size
{
    small,
    medium,
    big
};
class Toy
{
protected:
    double price;

public:
    Toy(double price)
    {
        this->price = price;
    }

    virtual ~Toy();
    virtual void printType() = 0;
    friend class ToyBox;
};
class CarToy : public Toy
{
private:
    Color color;

public:
    CarToy(double price, Color color) : Toy(price)
    {
        this->color = color;
    }

    void printType()
    {
        cout << "This is a car toy";
    }

    friend class ToyBox;
};
class PuzzleToy : public Toy
{
private:
    Size size;

public:
    PuzzleToy(double price, Size size) : Toy(price)
    {
        this->size = size;
    }

    void printType()
    {
        cout << "This is a puzzle toy\n";
    }

    friend class ToyBox;
};
class ToyBox
{
private:
    Toy* toyBox[5];
    int numberOfItems;

public:
    ToyBox()
    {
        this->numberOfItems = 0;
        for (int i = 0; i < 5; i++)
        {
            toyBox[i] = nullptr;
        }
    }

    ~ToyBox()
    {
        for (int i = 0; i < 5; i++)
        {
            if (toyBox[i]) delete toyBox[i];
        }
    }
    
    int addItem(const CarToy carToy)
    {
        /*
         * STUDENT ANSWER
         * TODO: function add a new Car toy to the box.
                 If successfully added, the function returns the current number of toys in the box. 
                 If the box is full, return -1.
         */

        if (numberOfItems == 5) return -1;

        int i = 0;
        while (toyBox[i] != nullptr) i++;
        toyBox[i] = new CarToy(carToy.price, carToy.color);
        numberOfItems++;
        return numberOfItems;
    }
    
    int addItem(const PuzzleToy puzzleToy)
    {
        /*
         * STUDENT ANSWER
         * TODO: function add a new Puzzle toy to the box.
                 If successfully added, the function returns the current number of toys in the box. 
                 If the box is full, return -1.
         */
        if (numberOfItems == 5) return -1;

        int i = 0;
        while (toyBox[i] != nullptr) i++;
        toyBox[i] = new PuzzleToy(puzzleToy.price, puzzleToy.size);
        numberOfItems++;
        return numberOfItems;
    }
    
    void printBox()
    {
        for (int i = 0; i < numberOfItems; i++)
            toyBox[i]->printType();
    }
};
void swap(int& a, int& b)
{
    int temp = a;
    a = b;
    b = temp;
}
void ascendingSort(int a[], int n) {
    /*
     * STUDENT ANSWER
     * TODO: sort array in ascending order. 
     */
    
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (a[j] > a[j + 1])
                swap(a[j], a[j + 1]);
}
void descendingSort(int a[], int n) {
    /*
     * STUDENT ANSWER
     * TODO: sort array in descending order. 
     */
 
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (a[j] < a[j + 1])
                swap(a[j], a[j + 1]);
}
void mySort(int a[], int n, void (*sort)(int[], int)) {
	/*
     * STUDENT ANSWER
     * TODO: sort array based on sort algorithm of function sort. 
     */
     
    (*sort)(a, n);
}

void printArray(int n)
{
    if (n == 0) 
    {
        cout << 0;
        return;
    }
    printArray(n - 1);
    cout << ", " << n;
}
int strLen(char* str)     
{ 
    if (str[0] == '\0') return 0;
    return strLen(str + 1) + 1;
} 
bool found(char* text, char* pattern)
{
    if (pattern[0] == '\0') {
        return true;
    }
    if (text[0] == '\0') {
        return false;
    }

    if (text[0] == pattern[0])
    {
        return found(text + 1, pattern + 1);
    }
    else
    {
        return false;
    }
}
bool containsPattern(char* text, char* pattern)
{
    if (text[0] == '\0')
    {
        return false;
    }

    if (found(text, pattern))
    {
        return true;
    }
    else
    {
        text++;
        return containsPattern(text, pattern);
    }
}
void printHailstone(int number)
{
    cout << number;
    
    if (number == 1)
    {    
        return;
    }
    
    cout << " ";
    if (number % 2 == 0)
    {
        printHailstone(number / 2);
    }
    else
    {
        printHailstone(number * 3 + 1);
    }
}
int myArrayToInt(char *str, int n) 
{ 
    if (n == 0) return 0;
    int last = (int)str[n - 1] - 48;
    return myArrayToInt(str, n - 1) * 10 + last;
}
int findMax(int* arr, int length)
{
    if (length == 1)
    {
        return arr[length - 1];
    }
    int max = findMax(arr, length - 1);
    if (max > arr[length - 1])
    {    
        return max;
    }
    else
    {
        return arr[length - 1];
    }
}




int descendPrint(int n) // Stop printing if n <= 0
{
    if (n > 0)  
    {
        cout << n << " ";
    }
    else
    {
        return n;
    } 
    descendPrint(n - 5);
}
void ascendPrint(int start, int n)  // Print from start to n
{
    if (start < n)
    {
        cout << start << " ";
        ascendPrint(start + 5, n);
    }
    else
    {
        cout << start;
    }
    return;
}
void printPattern(int n) 
{
    int start = descendPrint(n);
    ascendPrint(start, n);
}




bool isPalindromeC(char* sC, int length)
{
    if (length <= 1) return true;

    if (sC[0] == ' ')
    {
        return isPalindromeC(sC + 1, length - 1);
    }
    else if (sC[length - 1] == ' ')
    {
        return isPalindromeC(sC, length - 1);
    }
    else if (sC[0] == sC[length - 1])
    {
        return isPalindromeC(sC + 1, length - 2);
    }
    else
    {
        return false;
    }
}
bool isPalindrome(string str)
{
    char* sC = (char*)str.c_str();
    int length = str.length();

    return isPalindromeC(sC, length);
}




int count(int x, int n)
{
    if (x - n * n == 1) { return 1; }
    else if (x - n * n < 0) { return 0; }
    else return count(x - n * n, n + 1) + count(x, n + 1); 
}

int countWaySumOfSquare(int x)
{
    int n = 1;
    return count(x, n);
}



int main()
{
    // printPattern(14);
    
    // string alabame = "do geese   see god";
    // cout << isPalindrome(alabame);

    cout << countWaySumOfSquare(100);

    return 0;
}