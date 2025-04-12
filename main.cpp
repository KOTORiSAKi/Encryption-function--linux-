#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class standard_object{
    private:
    string test_source1;
    string test_source2;
    string test_key;

    string default_key = "AZBYCX";
    string password_key;
    string key;

    string TEMP;
    vector<string> id;
    vector<string> password;
    vector<string> en_password;
    vector<string> text;
    vector<string> en_text;

    vector<string> raw_source;

    int current_user;

    public:
    string xorEncryptDecrypt(const string& input, const string& key) {
        string output = input; // Create a copy to modify
        size_t keyLength = key.length();
        for (size_t i = 0; i < input.length(); ++i) {
            output[i] = input[i] ^ key[i % keyLength]; // XOR with key character
        }
        return output;
    }
    void g(){
        password_key = xorEncryptDecrypt(TEMP,default_key);
    }

    void write_log();
    void read_log();
    void setup_raw_data();

    void test_vector();
    void test_encryption();
    void test_read_log();
};

void standard_object::write_log(){
    string filepassword = "log_file.txt";
    ofstream outputFile(filepassword);
    en_text.clear();
    en_password.clear();
    for(int i = 0; i < password.size(); i++){en_password.push_back(xorEncryptDecrypt(password.at(i),default_key));}
    for(int i = 0; i < text.size(); i++){en_text.push_back(xorEncryptDecrypt(text.at(i),default_key));}
    if (outputFile.is_open()) {

        for(int i = 0; i < id.size(); i++){
            outputFile << id.at(i) << endl << en_password.at(i) << endl << "#*#*#*#*#*#*#*#*" << endl << en_text.at(i) << endl << "#*#*#*#*#*#*#*#*" << endl; // Write user input to the file
        }

        outputFile.close(); // Close the file stream
        cout << "File '" << filepassword << "' written successfully." << endl << "\tPress Enter..."; getchar();
    } else {
        cerr << "Error: Could not open file '" << filepassword << "' for writing." << endl;
    }
}

void standard_object::setup_raw_data(){//TODO: make set up part at password
    id.clear(); 
    en_password.clear();
    en_text.clear();
    int top_up = 0;
    string temp1;
    short temp2, temp3;
    for(int i = 0; i < raw_source.size(); i++){
        if((i - top_up)%4 == 0){id.push_back(raw_source.at(i));}
        else if((i - top_up)%4 == 1){
            temp2 = 0;
            for(int j = 0; ; j++){
                if(raw_source.at(i + j).compare("#*#*#*#*#*#*#*#*") == 0){temp2 = j; break;}
            }
            //cout << temp2 << endl;
            temp1 = "";
            for(int j = 0; j < temp2; j++){
                temp1 += raw_source.at(i + j);
                if(j != temp2 - 1){temp1 += '\n';}
            }
            en_password.push_back(temp1);
            i += temp2;
            top_up += temp2;
        }
        else if((i - top_up)%4 == 2){
            temp2 = 0;
            for(int j = 0; ; j++){
                if(raw_source.at(i + j).compare("#*#*#*#*#*#*#*#*") == 0){temp2 = j; break;}
            }
            //cout << temp2 << endl;
            temp1 = "";
            for(int j = 0; j < temp2; j++){
                temp1 += raw_source.at(i + j);
                if(j != temp2 - 1){temp1 += '\n';}
            }
            en_text.push_back(temp1);
            i += temp2;
            top_up += temp2 - 1;//! -1 to let (%4 == 3) Check the point-gate before continue to the next user's data
        }
        else if((i - top_up) % 4 == 3 || raw_source.at(i).compare("#*#*#*#*#*#*#*#*") == 0){cout << endl << endl << "Done for user " << (i / 4) + 1 << endl;}
        ////else{cout << endl << endl << "Error! (setup_raw_data)" << endl; exit(0);}
    }
}

void standard_object::read_log(){
    raw_source.clear();
    string filepassword = "log_file.txt";
    ifstream inputFile(filepassword);

    if (inputFile.is_open()){
        string line;
        while(getline(inputFile, line)){
            raw_source.push_back(line);
        }
        inputFile.close();
    } else {
        cerr << "Error: Could not open file '" << filepassword << "' for reading." << endl;
    }
    for(int i = 0; i < raw_source.size(); i++){
        cout << raw_source.at(i) << endl;
    }
    cout << endl << endl << "======================" << endl << endl;
}

//!====={testing part}==========================================================================

void standard_object::test_vector(){
    string temp1;
    short temp2;
    for(int i = 0;; i++){
        temp2 = i % 3;
        switch(temp2){
            case 0:cout << "User " << (i / 3) + 1 << " : ";getline(cin,temp1); break;
            case 1:cout << "Password " << (i / 3) + 1 << " : ";getline(cin,temp1); break;
            case 2:cout << "Text " << (i / 3) + 1 << " : ";getline(cin,temp1); break;
        }
        if(temp1.compare("stop") == 0){break;}
        switch(temp2){
            case 0:id.push_back(temp1);break;
            case 1:password.push_back(temp1);break;
            case 2:text.push_back(temp1);break;
        }
    }
    cout << endl;
}

void standard_object::test_read_log(){
    read_log();
    setup_raw_data();
    password.clear();
    text.clear();
    for(int i = 0; i < en_password.size(); i++){
        password.push_back(xorEncryptDecrypt(en_password.at(i),default_key));
    }
    for(int i = 0; i < en_text.size(); i++){
        text.push_back(xorEncryptDecrypt(en_text.at(i),default_key));
    }
    for(int i = 0; i < id.size(); i++){
        cout << id.at(i) << endl << password.at(i) << endl << en_text.at(i) << " => " << text.at(i) << endl << endl;
    }
    cout << "\tPress Enter..."; getchar();
}

void standard_object::test_encryption(){
    test_vector();
    write_log();
}

//!=============================================================================================

void command_action(string &call_command){
    standard_object A;
    vector<string> command = {
        "test_vector","test_encryption","test_read_log"
    };
    string temp1 = call_command;
    short temp2;
    system("clear");//!clear
    // for(int i = 0; i < command.size(); i++){
    //     cout << "\t" << i+1 << ". " << command.at(i) << endl;
    // }
    //----------------------
    for(int i = 0; i < command.size(); i++){
        if(!temp1.compare(command.at(i))){temp2 = i + 1; break;}
    }
    switch(temp2){
        case 1:system("clear");A.test_vector();break;
        case 2:system("clear");A.test_encryption();break;
        case 3:system("clear");A.test_read_log();break;
    }
}

//?====={call fuctions}=====
void starting();
//?=========================

int main(){
    standard_object A;
    starting();
    return 0;
}

void starting(){
    system("clear");
    string choice1;
    while(1){
        cout << endl << endl << "\tCommand => "; getline(cin,choice1);
        command_action(choice1);
    }
}