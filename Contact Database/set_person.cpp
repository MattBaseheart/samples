#include <fstream>
#include <iostream>

using namespace std;

int main(){

int size=20;
    string line[size];
    int i=0;
    ifstream infile; 
    infile.open("person_template.txt");
    int flag=0;

    while(!flag){
        //process each line

        infile>>line[i];
        i++;

        if(i==11){
        	flag=1;
        }
    }
    infile.close();

    for(int j=0; j<i; j++){
    	cout<<line[j]<<endl;
    }

}