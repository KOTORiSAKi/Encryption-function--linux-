#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

void test_vector(){
    string temp1;
    vector<string> Dynamic_array;
    for(int i = 0;; i++){
        cout << "Enter " << i+1 << " : "; getline(cin,temp1);
        if(temp1.compare("stop") == 0){cout << endl << "Stoped" << endl; break;}
        Dynamic_array.push_back(temp1);
    }
    cout << endl;
    for(int i=0; i<Dynamic_array.size(); i++){
        cout << Dynamic_array.at(i) << endl;
    }
}

string starting_command(){
    string command;
    cout << endl << endl << "\tCommand => "; getline(cin,command);
    return command;
}

class standard_object{
    private:
    vector<string> id;
    vector<string> name;
    vector<string> text;

    int current_user;

    public:
    
};

int main(){
    string temp1;
    test_vector();
    return 0;
}