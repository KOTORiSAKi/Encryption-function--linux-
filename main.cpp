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

    string key = "abcdefg";
    vector<string> id;
    vector<string> password;
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
    void write_log();
    void read_log();
    void setup_raw_data();
    void test_vector();
    void test_encrytion(){
        test_vector();
        write_log();
    }
    void test_read_log();
};

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

void standard_object::write_log(){
    string filepassword = "log_file.txt";
    ofstream outputFile(filepassword);
    en_text.clear();
    for(int i = 0; i < text.size(); i++){en_text.push_back(xorEncryptDecrypt(text.at(i),key));}
    if (outputFile.is_open()) {

        for(int i = 0; i < id.size(); i++){
            outputFile << id.at(i) << endl << password.at(i) << endl << en_text.at(i) << endl << "#*#*#*#*#*#*#*#*" << endl; // Write user input to the file
        }

        outputFile.close(); // Close the file stream
        cout << "File '" << filepassword << "' written successfully." << endl << "\tPress Enter..."; getchar();
    } else {
        cerr << "Error: Could not open file '" << filepassword << "' for writing." << endl;
    }
}

void standard_object::setup_raw_data(){
    id.clear(); 
    password.clear();
    en_text.clear();
    int top_up = 0;
    string temp1;
    short temp2, temp3;
    for(int i = 0; i < raw_source.size(); i++){
        if((i - top_up)%4 == 0){id.push_back(raw_source.at(i));}
        else if((i - top_up)%4 == 1){password.push_back(raw_source.at(i));}
        else if((i - top_up)%4 == 2){//TODO: Fix layering array to store all en_text data in one index of vector array
            temp2 = 0;
            for(int j = 0; ; j++){
                if(raw_source.at(i + j).compare("#*#*#*#*#*#*#*#*") == 0){temp2 = j; break;}
            }
            cout << temp2 << endl;
            temp1 = "";
            for(int j = 0; j < temp2; j++){
                temp1 += raw_source.at(i + j);
                if(j != temp2 - 1){temp1 += '\n';}
            }
            en_text.push_back(temp1);
            i += temp2;
            top_up += temp2 - 1;
            //TODO: fix evcrytion part that data went wrong in some case and character
        }
        else if((i - top_up) % 4 == 3 || raw_source.at(i).compare("#*#*#*#*#*#*#*#*") == 0){}
        ////else{cout << endl << endl << "Error! (setup_raw_data)" << endl; exit(0);}
    }
}

// temp2 = i; loop1:

// if(raw_source.at(temp2 + 1).compare("#*#*#*#*#*#*#*#*") != 0){
//     activate1 = 1;
//     temp1 = raw_source.at(temp2) + raw_source.at(temp2 + 1);
//     top_up++;
//     if(raw_source.at(temp2 + 2).compare("#*#*#*#*#*#*#*#*") != 0){++i; ++temp2; goto loop1;}
//      n_text.push_back(temp1);
// }
// else if(raw_source.at(temp2 + 1).compare("#*#*#*#*#*#*#*#*") == 0 && activate1 == 1){en_text.push_back(raw_source.at(temp2));}
// else if(raw_source.at(temp2 + 1).compare("#*#*#*#*#*#*#*#*") == 0){en_text.push_back(raw_source.at(temp2));}


void standard_object::read_log(){
    //raw_source.clear();
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
    setup_raw_data();
}

void standard_object::test_read_log(){
    read_log();
    text.clear();
    for(int i = 0; i < en_text.size(); i++){
        text.push_back(xorEncryptDecrypt(en_text.at(i),key));
    }
    for(int i = 0; i < id.size(); i++){
        cout << id.at(i) << endl << password.at(i) << endl << en_text.at(i) << " => " <<text.at(i) << endl << endl;
    }
    cout << "\tPress Enter..."; getchar();
}
    

void starting_command(){
    standard_object A;
    vector<string> command = {
        "test_vector","test_encrytion","test_read_log"
    };
    string temp1;
    short temp2;
    while(1){
        system("clear");//!clear
        for(int i = 0; i < command.size(); i++){
            cout << "\t" << i+1 << ". " << command.at(i) << endl;
        }
        cout << endl << endl << "\tCommand => "; getline(cin,temp1);
        //----------------------
        for(int i = 0; i < command.size(); i++){
            if(!temp1.compare(command.at(i))){temp2 = i + 1; break;}
        }
        switch(temp2){
            case 1:system("clear");A.test_vector();break;
            case 2:system("clear");A.test_encrytion();break;
            case 3:system("clear");A.test_read_log();break;
        }
    }
}

int main(){
    starting_command();
    return 0;
}