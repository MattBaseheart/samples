#ifndef CONTACT_H
#define CONTACT_H

#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
// TODO: You may need to add other libraries here!
using namespace std;


class Contact {
protected:  // TODO: private or protected? Look at its children!    
    string type;
public:
    // the following functions are pure virtual functions
    virtual void print() = 0;
    virtual string get_contact(string style="full") = 0;
    virtual void set_contact() = 0;
};


class Email: public Contact{
private:
    string email_addr;
public:
    Email();
    Email(string type, string email_addr);
    // TODO: Complete me!
    void print();
    string get_contact(string style="full");
    void set_contact();

};


class Phone: public Contact{
private:
    // TODO: modify dataType! Can int store 10 digit phone-number? 
    long int phone_num; 
public:
    Phone();
    Phone(string type, string num);
    // TODO: Complete me!
    void print();
    string get_contact(string style="full");
    void set_contact();
};

#endif
