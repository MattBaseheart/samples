
#include <iostream>
#include "person.h"

int main(){

    Person c1("vince","Espiritu","02/08/1998","(home) vepirit@usc.edu","(mobile) 818-297-7969");
    c1.print_person();
    cout << "----------------\n";

    Person c3;
    cout << "----------------\n";

	Person c2("person_template.txt");
    c2.print_person();
    cout << "----------------\n";

    if(c1==c3){
        cout<<"Duplicate"<<endl;
    }
    else{
        cout<<"Not Duplicate"<<endl;
    }

    return 0;
}
