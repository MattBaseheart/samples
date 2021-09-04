
#include "network.h"
#include <limits>
#include "misc.h"
#include <fstream>
#include <iostream>
#include <string>
#include <dirent.h>

Network::Network(){
    head = NULL;
    tail = NULL;
    count = 0;
}


Network::Network(string fileName){
    // TODO: complete this method!
    // Implement it in one single line!
    // You may need to implement the load method before this!
    loadDB(fileName);

}

Network::~Network(){
    // TODO: Complete this method
    // Destructure delete all the Person
    Person* ptr = head;
    while(head != NULL){
        ptr = head->next;
        delete head;
        head = ptr;
        count--;
    }
}


void Network::push_front(Person* newEntry){
    // Adds a new Person (newEntry) to the front of LL
    newEntry->prev = NULL;
    newEntry->next = head;

    if (head != NULL)
        head->prev = newEntry;
    else
        tail = newEntry;
    
    head = newEntry;
    count++;
}


void Network::push_back(Person* newEntry){
    // TODO: Complete this method!
    // Adds a new Person (newEntry) to the back of LL
    newEntry->prev = tail;
    newEntry->next = NULL;

    if(tail != NULL)
        tail -> next = newEntry;
    else
        head = newEntry;

    tail = newEntry;
    count++;
}


void Network::printDB(){
    // Don't change this method
    // Note: Notice that we don't need to update this even after adding to Person attributes
    // This is a feature of OOP, classes are supposed to take care of themselves!
    cout << "Number of persons: " << count << endl;
    cout << "------------------------------" << endl;
    Person* ptr = head;
    while(ptr != NULL){
        ptr->print_person();
        cout << "------------------------------" << endl;
        ptr = ptr->next;
    }
}


void Network::saveDB(string filename){
    // TODO: Complete this method!
    // Saves the netwrok in file <filename>
    // Note: The format of the output file is very abstract, just raw data
    // Look at studentDB.db as a template
    // Note: notice the intentional flaw in this code, as compared to the note mentioned in printDB, 
    // now the one who is responsible for implementing Network should be aware of implementation of Person, not good! You will fix this in PA2.
    ofstream output;
    output.open(filename.c_str());
    output << "Number of persons: " << count << endl;
    output << "------------------------------" << endl;
    Person* ptr = head;
    while(ptr != NULL){
        //ptr->print_person(); // THIS DOESNT WORK FIX IT (must output to a file not print to terminal)
        output << ptr->l_name << ", " << ptr->f_name <<"\n";
        output<< ptr->birthdate->get_date()<<"\n";
        output<< ptr->email->get_contact()<<"\n";
        output<< ptr->phone->get_contact()<<"\n";
        output << "------------------------------" << endl;
        ptr = ptr->next;
    }
    output.close();
}


void Network::loadDB(string filename){
    // TODO: Complete this method!
    // Loads the netwrok from file <filename>
    // The format of the input file is similar to saveDB
    // Look at network studentDB.txt as a template
    // Phone number can be with dashes or without them
    // You need to use the following constructor of the Person class, Person::Person(fname, lname, bdate, email, phone)
    
    Person* ptr = head;
    while(head != NULL){
        ptr = head->next;
        delete head;
        head = ptr;
        count--;
    }
    head = NULL;
    tail = NULL;

    ifstream infile;
    infile.open(filename.c_str());
    string buff, fname, lname, bdate, email, phone;
   
    while(getline(infile, buff)){
        lname = buff.substr(0, buff.find(','));
        fname = buff.substr(buff.find(',')+2);
        getline(infile, bdate);
        
        // TODO: read email and phone
        getline(infile, email);
        getline(infile, phone);
        // this line is to read the dash line
        getline(infile, buff);
        // TODO: use the constructor Person::Person(fname, lname, bdate, email, phone) to modify the following line
        Person* newEntry = new Person(fname, lname, bdate, email, phone);

        this->push_back(newEntry);
    }

    infile.close();
}

Person* Network::search(string fname, string lname, string bdate){
    // TODO: Complete this method!
    // Search the Network for the given fname, lname, bdate
    // You may create a new person, say searchEntry, with fname, lname and bdate. Think about what Person constructor will be helpful with these arguments.
    // And use == overloaded operator between two persons
    // if found, returns a pointer to it, else returns NULL
    // Don't forget to delete allocated memory.
    if(count == 0){
        return NULL;
    }

    Person* ptr = head;
    Person* searchEntry=new Person(fname, lname, bdate,"(default) No_email", "(default) No_phone");
    while(ptr != NULL){

        if(*(ptr) == *searchEntry){
            return ptr;
        }
        else
            ptr = ptr->next;
    }
    
    return NULL;

}

bool Network::remove(string fname, string lname, string bdate){
    // TODO: Complete this method! Follow these steps:
    // Create a new person with the the give arguments as you do in search
    // Search if this person exists using search method. If it does not exist just return false! Else, remove the person from LL, make the other connections connected
    // Don't forget to delete allocated memory, change count and returning values

    Person* removePerson=search(fname,lname,bdate);

    if(removePerson!=NULL){

        if(removePerson->next==NULL){
            tail=removePerson->prev;
        }
        else if(removePerson->prev==NULL){
            head=removePerson->next;
        }
        else{
        Person* connect=removePerson->prev;
        connect->next=removePerson->next->prev;
        }
        delete removePerson;
        count--;
        return true;

    }
    else{
        return false;
    }

}

void Network::showMenu(){
    // TODO: Complete this method!
    // All the prompts are given to you, 
    // You should add code before, between and after prompts!

    int opt;
    while(1){
        cout << "\033[2J\033[1;1H";
        printMe("banner"); // from misc library

        cout << "Select from below: \n";
        cout << "1. Load network database \n";
        cout << "2. Save network database \n";
        cout << "3. Add a new person \n";
        cout << "4. Search \n";
        cout << "5. Remove a person \n";
        cout << "6. Print database \n";
        
        cout << "\nSelect an option ... ";
        
        if (cin >> opt) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Wrong option! " << endl;
            return;
        }
        
        // You may need these variables! Add more if you want!
        string fname, lname, fileName, bdate;
        cout << "\033[2J\033[1;1H";

        if (opt==1){
            // TODO: Complete me!
            cout << "Loading network database \n";
            // TODO: print all the files in this same directory that have ".db" format
            DIR *dir;
            struct dirent *ent;
            char targetFolderAddr[] = "./";
            if ((dir = opendir ("./")) != NULL) { 
                while ((ent = readdir (dir)) != NULL) {
                    string fname = ent->d_name;
                    if(fname.find(".db") != std::string::npos)
                        cout << ent->d_name << endl;
                }
                closedir (dir);
            } 
            // Take a look into sample_files.cpp 
            cout << "Enter the name of the load file: ";
            cin >> fileName; 

            int found = 0;
            //DIR *dir;
            //struct dirent *ent;
            //char targetFolderAddr[] = "./";
            if ((dir = opendir ("./")) != NULL) { 
                while ((ent = readdir (dir)) != NULL) {
                    fname = ent->d_name;
                    if(fname.compare(fileName) == 0){
                        loadDB(fileName);
                        //READ THE FILE IN
                        
                        found = 1;

                    }
                }
                closedir (dir);
            }            

             // If file with name FILENAME does not exist: 
            if(found == 0){
                cout << "File " << fileName << " does not exist!" << endl;
            }
            else{
                cout << "Network loaded from " << fileName << " with " << count << " persons \n" << endl;
            }
            
        }
        else if (opt==2){
            // TODO: Complete me!
            cout << "Saving network database \n";
            cout << "Enter the name of the save file: ";
            cin >> fileName;
            saveDB(fileName);
            cout << "Network saved in " << fileName << endl;
        }
        else if (opt == 3){
            // TODO: Complete me!
            // TODO: use push_front, and not push_back 
            // Add a new person ONLY if it does not exists!
            Person* addMe = new Person();

            if(search(addMe->f_name, addMe->l_name, addMe->birthdate->get_date()) == NULL){
                this->push_front(addMe);
            }
            else{
                delete addMe;
                cout<< "Person already in database."<<endl;
            }
        }
        else if (opt == 4){
            // TODO: Complete me!
            cout << "Searching: \n";
            cout << "First Name: ";
            getline(cin,fname);

            cout << "Last Name: ";
            getline(cin,lname);

            cout << "Birthdate (M/D/YYYY): ";
            cin>>bdate;

            Person* searchEntry= search(fname,lname,bdate);
            if( search(fname,lname,bdate) != NULL) { // if found: print person's firstname, lastname, bdate, email, phone using print_person()
                searchEntry->print_person();
            }
            else{ // if not, cout << "Not found! \n";
                cout<< "Not found! \n";
            }

        }
        else if (opt==5){
            // TODO: Complete me!
            cout << "Removing a person \n";
            cout << "First name: ";
            getline(cin,fname);
            cout << "Last name: ";
            getline(cin,lname);
            cout << "Birthdate (M/D/YYYY): ";
            getline(cin,bdate);
            if(remove(fname,lname,bdate)){ // if found, cout << "Remove Successful! \n";
            cout << "Remove Successful! \n";
            }
            else{ // if not found: cout << "Person not found! \n";
            cout << "Person not found! \n";
            }
        }
        else if (opt==6){
            // TODO: Complete me!
            cout << "Network Database: \n";
            printDB();

        }
        else
            cout << "Nothing matched!\n";
        
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "\n\nPress Enter key to go back to main menu ... ";
        string temp;
        std::getline (std::cin, temp);
        cout << "\033[2J\033[1;1H";
    }
}


