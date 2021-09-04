#include "contact.h"

// TODO: Add needed libraries! 
Email::Email(){

    //default constructor to make use of set_contact
    //this->type="Default";
    set_contact();
}


Email::Email(string type, string email_addr){
    // TODO: Complete me!
    this-> type = type;
    this-> email_addr = email_addr;
}


void Email::set_contact(){
    // TODO: Do not change the prompts!
    string input;
    cout << "Enter the type of email address: ";
    cin >> input;
    cout << endl;
    this-> type = input;
    // some code here
    cout << "Enter email address: ";
    cin >> input;
    cout << endl;
    this-> email_addr = input;
    // some code here
}


string Email::get_contact(string style){
    // Note: We have default argument in declaration and not in definition!
    // emaple: Email (USC): tommytrojan@usc.edu
    if (style=="full")
        return "Email (" + type + "): " + email_addr;
    else 
        return email_addr;
}


void Email::print(){
    // Note: get_contact is called with default argument
    cout << get_contact() << endl;
}

//----------------------------------------------------------------------------------//

Phone::Phone(){
    //default constructor
    //this-> type = "Default";
    set_contact();
}

Phone::Phone(string type, string num){
    // TODO: It is possible that num includes "-" or not, manage it! 222-444-7898
    // TODO: Complete this method!
    this-> type = type;
    long int area_code=0;
    long int number1=0;
    long int number2=0;

    /* Note: Modify the following code can help you implementing this function
     * This code has a bug, can you understand why?*/
     if(num.substr(3,1)=="-" && num.substr(7,1)=="-"){
        area_code = atoi(num.substr(0, 3).c_str());
        number1 = atoi(num.substr(4,3).c_str());
        number2 = atoi(num.substr(8,4).c_str()); 
    }
    else if(num.substr(3,1)=="-"){
        area_code = atoi(num.substr(0, 3).c_str());
        number1 = atoi(num.substr(4,3).c_str());
        number2= atoi(num.substr(7,4).c_str());

    }
    else if(num.substr(6,1)=="-"){
        area_code = atoi(num.substr(0, 3).c_str());
        number1 = atoi(num.substr(3,3).c_str());
        number2= atoi(num.substr(7,4).c_str());

    }
    else{
        area_code = atoi(num.substr(0, 3).c_str());
        number1 = atoi(num.substr(3,3).c_str());
        number2= atoi(num.substr(6,4).c_str()); 
    }

    this-> phone_num = area_code*10000000 + number1*10000 + number2; 

    
}


void Phone::set_contact(){
    // TODO: Complete this method
    // Use the same prompts as given!
    string type;
    cout <<"Enter the type of phone number: "<<endl;
    cin >> type;

    string num;
    cout << "Enter the phone number: "<<endl;
    cin >> num;

     // TODO: It is possible that num includes "-" or not, manage it! 222-444-7898
    // TODO: Complete this method!
    this-> type = type;

    /* Note: Modify the following code can help you implementing this function
     * This code has a bug, can you understand why?*/
    long int area_code=0;
    long int number1=0;
    long int number2=0;

    if(num.substr(3,1)=="-" && num.substr(7,1)=="-"){
        area_code = atoi(num.substr(0, 3).c_str());
        number1 = atoi(num.substr(4,3).c_str());
        number2 = atoi(num.substr(8,4).c_str()); 
    }
    else if(num.substr(3,1)=="-"){
        area_code = atoi(num.substr(0, 3).c_str());
        number1 = atoi(num.substr(4,3).c_str());
        number2= atoi(num.substr(7,4).c_str());

    }
    else if(num.substr(6,1)=="-"){
        area_code = atoi(num.substr(0, 3).c_str());
        number1 = atoi(num.substr(3,3).c_str());
        number2= atoi(num.substr(7,4).c_str());

    }
    else{
        area_code = atoi(num.substr(0, 3).c_str());
        number1 = atoi(num.substr(3,3).c_str());
        number2= atoi(num.substr(6,4).c_str()); 
    }

    this-> phone_num = area_code*10000000 + number1*10000 + number2; 



}

string Phone::get_contact(string style){
    // TODO: Complete this method, get hint from Email 
    // follow the format of this example: Phone (Office): 310-192-2847

    int x1 = (phone_num/10000000);
    int x2 = ((phone_num)/10000)%1000;
    int x3 = phone_num%10000;

    //int to string conversion using string stream
    ostringstream y1, y2, y3;
    y1 << x1;
    y2 << x2;
    y3 << x3;

    string s1=y1.str();
    string s2=y2.str();
    string s3=y3.str();

    string str = s1 + "-" + s2 + "-" + s3;

    if (style=="full"){
        return "Phone (" + type + "): " + str;
    }
    else {

        return str;
    }
    
}


void Phone::print(){
    // Note: get_contact is called with default argument
    cout << get_contact() << endl;
}


