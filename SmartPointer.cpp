#include<iostream>
#include <string>
#include <memory>

// g++ -std=c++11 -g3 SmartPointer.cpp -o SmartPointer
// SmartPointer.exe >> SmartPointer.output

using namespace std;
class Dog
{
    string name_;
   
    public:
    Dog(string name) {cout<<"Dog is created"<< name << endl; name_ = name;}
    Dog() {cout<<"Dog is created" << endl; name_ = "Nameless";}
    ~Dog(){cout<<"Dog destryed is : "<< name_ << endl;}
    void bark() { cout<<"I am "<< name_ << endl;}
    
    /* part2: weak pointer */
    private:
     //shared_ptr<Dog> __my_frnd;  create a memory leak in cyclic refrence foo6
    weak_ptr<Dog> __my_frnd; /* No ownership, cannot be deleted , empty will thow exception */
    public:
    void make_frnd(shared_ptr<Dog> f ){__my_frnd = f;}
    
    void show_frnd() { cout << " My frind is " << __my_frnd.lock()->name_ <<" used count:"<< __my_frnd.use_count() <<endl;}
    
};

int foo()
{
   cout <<"---------------" <<__FUNCTION__ <<"------------------" <<endl; 
    Dog *p  = new Dog("Gunner");
    p->bark();
    // in case function crash in between
   /* careless use can cause dangling pointer or memory leak */
    delete p;
}
/* 
Shared Pointer
--------------
1. smart pointer holding reference to raw pointer(object)
2. First a object of dog is created and that is passed to construtor of shared pointer.
3. Object pointed by shared pointer is deleted once it is out of scope 
4. deletion is scoped in stack 
5. reference count is maintiained 
6. If a raw  pointer is hold by two shared_ptr, double delete (foo2)
7. use_count() gives no of refrences, get() gives raw pointer, reset() deletes and resets
8. If raw pointer from shared pointer is stored, it may become dangling pointer (foo2)
9. raw pointer should be assigend to smart pointer to imeddiately, and never used directly.
10. make_shared<class>() is one step exception safe way to construct a shared pointer.
11. Array of shared_ptr need to be deleted by custom deleter (foo4, foo5)
*/

int foo1()
{
    cout <<"---------------" <<__FUNCTION__ <<"------------------" <<endl;
    //shared_ptr<Dog> p  = new Dog("Gunner");
    /*  error: conversion from 'Dog*' to non-scalar type 'std::shared_ptr<Dog>' requested */
    shared_ptr<Dog> p(new Dog("Gunner")); 
    {
        shared_ptr<Dog> p2 = p;
        p2->bark();
        cout<< p.use_count() <<endl;
    }
    p->bark();
    cout<< p.use_count() <<endl;
   
}

// pitfall is undefined behavior
int foo2()
{
    
    cout <<"---------------" <<__FUNCTION__ <<"------------------" <<endl;
    // ;
    Dog * p = new Dog( "Dog1");
    
    shared_ptr<Dog> p1(p);
    shared_ptr<Dog> p2(p);
    
    p1->bark();
    p2->bark();
    
    cout<< p1.use_count() <<endl;
    cout<< p1.use_count() <<endl;
    
    Dog * d  = p1.get();
    
    // map.insert(d) // d will be dangling pointer
   
}
 /* point 7:  make_shared is best way and is exception safe. */

int foo3()
{
    
    cout <<"---------------" <<__FUNCTION__ <<"------------------" <<endl;
    shared_ptr<Dog> p1 = make_shared<Dog>("Tank");
    p1->bark();
    (*p1).bark();
    Dog *d = p1.get();
}



 /* custom deleter*/
int foo4()
{
    cout <<"---------------" <<__FUNCTION__ <<"------------------" <<endl;
    shared_ptr<Dog> p1 = make_shared<Dog>("Tank");
    shared_ptr<Dog> p2 = shared_ptr<Dog>(new Dog ("gunner"), [](Dog *d) { delete d; cout<<"using Custom deleter"<<endl;});
    
    p2 = p1;
    //p2 = nullptr;
    p2.reset();
}

 /* https://stackoverflow.com/questions/13061979/shared-ptr-to-an-array-should-it-be-used */
int foo5()
{
    cout <<"---------------" <<__FUNCTION__ <<"------------------" <<endl;
    
    shared_ptr<Dog> p1 = shared_ptr<Dog>(new Dog[3], [](Dog *d) { delete[] d; cout<<"using Custom deleter"<<endl;});
    shared_ptr<Dog> p2 = shared_ptr<Dog>(new Dog[3], [](Dog *d) { delete[] d; cout<<"using Custom deleter"<<endl;});
    shared_ptr<Dog> p3 = shared_ptr<Dog>(new Dog[4]);
    //shared_ptr<Dog> p4 = shared_ptr<Dog>(new Dog[3]); ???
    
}

 int foo6()
{
   cout <<"---------------" <<__FUNCTION__ <<"------------------" <<endl; 
  
    shared_ptr<Dog>  d1 = make_shared<Dog>("d1");
    shared_ptr<Dog>  d2 = make_shared<Dog>("d2");
    d1->make_frnd(d2);
    d2->make_frnd(d1);
    d1->show_frnd();
    d2->show_frnd();
    
}

/* point 10: */

//static_pointer_cast
//dynamic_ponter_cast
//const_pointer_cast

/* unique pointer:
------------------
 1. exclusive ownership, light weight smart pointer 
 
 */

int ufoo()
{
    cout <<"---------------" <<__FUNCTION__ <<"------------------" <<endl;
    unique_ptr<Dog> d1(new Dog("d1"));
    d1->bark();
    
    unique_ptr<Dog> d2(new Dog("d1"));
    d2->bark();
    
    d1.reset(new Dog("d12"));
    
    Dog *d = d1.release();
    if(!d1) cout <<" unique pointer is empty" << endl ;
    d->bark();
    
    // support array , no deleter needed
    unique_ptr<Dog[]> d3(new Dog[3]);
}

int ufoo1()
{
    cout <<"---------------" <<__FUNCTION__ <<"------------------" <<endl;
    unique_ptr<Dog> d1(new Dog("d1"));
    d1->bark();
    
    unique_ptr<Dog> d2(new Dog("d1"));
    d2->bark();
    
    d2 = move(d1);
    d2->bark();
    
}

void recv_dog(unique_ptr<Dog> d)
{
     d->bark();
}

unique_ptr<Dog> get_dog()
{
    unique_ptr<Dog> d1(new Dog("d1-get"));
    return d1;
}

int ufoo2()
{
    cout <<"---------------" <<__FUNCTION__ <<"------------------" <<endl;
    unique_ptr<Dog> d1(new Dog("d1"));
    d1->bark();
    
    recv_dog(move(d1));
    if(!d1) cout <<" unique pointer is empty" << endl ;
    
    d1 = get_dog();
    d1->bark();
}

int main()
{
    foo();
    foo1();
    //foo2();
    foo3();
    foo4();
    foo5();
    foo6();
    ufoo();
    ufoo1();
    ufoo2();
}