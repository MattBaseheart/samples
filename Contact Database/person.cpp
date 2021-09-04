
#include "person.h"
#include <string>
using namespace std;

Person::Person(){
    // I'm already done! 
    set_person();
}



Person::Person(string fname, string lname, string bdate, string email, string phone){
    // TODO: Complete this method!
    // phone and email strings are in full version
    this->f_name = fname;
    this->l_name = lname;
    this->birthdate = new Date(bdate);

    string type;
    string value;

    type=email.substr(1,email.find(")")-1);
    value= email.substr(email.find(" ")+1);
    this->email = new Email(type, value);

    type=phone.substr(1,phone.find(")")-1);
    value= phone.substr(phone.find(" ")+1);
    this->phone = new Phone(type, value);

}


Person::Person(string filename){
    set_person(filename);
}

Person::~Person(){
    delete birthdate;
    // TODO: complete the method!
    delete email;
    delete phone;
}

void Person::set_person(){
    // prompts for the information of the user from the terminal
    // first/last name can have spaces!
    // date format must be "M/D/YYYY"
    // We are sure user enters info in correct format.
    // TODO: complete this method!
    
    string temp;
    string type;

    cout << "First Name: ";
    // pay attention to how we read first name, as it can have spaces!
    std::getline(std::cin,f_name);
    f_name = f_name + "\r";

	cout << "Last Name: ";
    std::getline(std::cin,l_name);

    cout << "Birthdate (M/D/YYYY): ";
    std::getline(std::cin,temp);
    // pay attention to how we passed argument to the constructor of a new object created dynamically using new command
    birthdate = new Date(temp); 

    cout << "Type of email address: ";
    std::getline(std::cin, type);

    cout << "Email address: ";
    std::getline(std::cin, temp);
  
    email = new Email(type, temp);

    cout << "Type of phone number: ";
    cin >> type;

    cout << "Phone number: ";
    cin >> temp;
    phone = new Phone(type, temp);
}


void Person::set_person(string filename){
    // reads a Person from a file
    // Look at person_template files as examples.     
    // Phone number in files can have '-' or not.
    // TODO: Complete this method!
    
    ifstream infile; 
    infile.open(filename.c_str());
    int size=20;
    string line[size];
    int i=2;
    int flag=0;

    getline(infile,line[0]);
    infile>>line[1];
    while(!flag){
        //process each line
        infile>>line[i];
        i++;

        if(i==11){
            flag=1;
        }
    }
    infile.close();

    //we need to access array of strings and use its value
    f_name=line[0];
    l_name=line[1];
    birthdate= new Date(line[2]);
    email= new Email(line[5].substr(1,line[5].find(")")-1),line[6]);
    phone= new Phone(line[3].substr(1,line[3].find(")")-1),line[4]);
    
}



bool Person::operator==(const Person& rhs){
    // TODO: Complete this method!
    // Note: you should check first name, last name and birthday between two persons
    // refer to bool Date::operator==(const Date& rhs)
    string compareThis = rhs.f_name + "\r";
    if ( (this->f_name == compareThis) && (this->l_name == rhs.l_name) && (*(this->birthdate) == *(rhs.birthdate)) ){
        return true;
    }
    else{
        return false;
    }
}

bool Person::operator!=(const Person& rhs){ 
    // TODO: Complete this method!
    if ( (this->f_name == rhs.f_name) && 
         (this->l_name == rhs.l_name) && 
    (*(this->birthdate) == *(rhs.birthdate))){
    return false;
    }
    else{
        return true;
    }
}


void Person::print_person(){
    // Already implemented for you! Do not change!
	cout << l_name <<", " << f_name << endl;
	birthdate->print_date("Month D, YYYY");
    email->print();
    phone->print();
}

void addFriend(Person* newFriend){
    friends.push_back(newFriend);
}

void printID(){
    cout << IDName(this->f_name, this->l_name) << endl;
}