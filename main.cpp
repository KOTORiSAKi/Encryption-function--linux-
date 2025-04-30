#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class standard_object{
    protected:
    string test_source1;
    string test_source2;
    string test_key;

    string default_key = "AZBYCX";

    string TEMP;
    vector<string> id;
    vector<string> en_id;
    vector<string> password;
    vector<string> en_password;
    vector<string> text;
    vector<string> en_text;

    vector<string> raw_source;

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
    void encryption(){
        if(id.size() == 0 || password.size() == 0 || text.size() == 0){cout << "There is no data for encryption!" << endl << endl << "\tPress Enter..."; getchar();}
        else{
            en_id.clear();
            en_text.clear();
            en_password.clear();
            for(int i = 0; i < id.size(); i++){en_id.push_back(xorEncryptDecrypt(id.at(i),default_key));}
            for(int i = 0; i < password.size(); i++){en_password.push_back(xorEncryptDecrypt(password.at(i),default_key));}
            for(int i = 0; i < text.size(); i++){en_text.push_back(xorEncryptDecrypt(text.at(i),default_key));}
        }
    }
    void decryption(){
        if(raw_source.size() == 0){}
        else{
            id.clear();
            password.clear();
            text.clear();
            for(int i = 0; i < en_id.size(); i++){id.push_back(xorEncryptDecrypt(en_id.at(i),default_key));}
            for(int i = 0; i < en_password.size(); i++){password.push_back(xorEncryptDecrypt(en_password.at(i),default_key));}
            for(int i = 0; i < en_text.size(); i++){text.push_back(xorEncryptDecrypt(en_text.at(i),default_key));}
        }
    }

    void test_vector();
    void test_encryption();
    void test_read_log();
};

class account_system: public standard_object{
    protected:
    bool logined = 0;
    short current_index = 0;

    public:
    bool checking_id(string &id){
        bool check = 0;
        for(int i = 0; i < this->id.size(); i++){
            if(!id.compare(this->id.at(i))){check = 1;}
        }
        return check;
    }

    void main_system();

    void login();
    void logout();
    void create_account();
    void delete_account();
    void change_password();
    void change_text();
};

void standard_object::write_log(){
    string filepassword = "log_file.txt";
    ofstream outputFile(filepassword);
    if (outputFile.is_open()) {

        for(int i = 0; i < id.size(); i++){
            if(i != 0){outputFile << endl;}
            outputFile << en_id.at(i) << endl << "#*#*#*#*#*#*#*#*"<< endl << en_password.at(i) << endl << "#*#*#*#*#*#*#*#*" << endl << en_text.at(i) << endl << "#*#*#*#*#*#*#*#*"; // Write user input to the file
        }

        outputFile.close(); // Close the file stream
        cout << "File '" << filepassword << "' written successfully." << endl << "\tPress Enter..."; getchar();
    } else {
        cerr << "Error: Could not open file '" << filepassword << "' for writing." << endl;
    }
}

void standard_object::setup_raw_data(){
    en_id.clear();
    en_password.clear();
    en_text.clear();
    int top_up = 0;
    string temp1;
    short temp2;
    for(int i = 0; i < raw_source.size(); i++){
        if((i - top_up)%4 == 0){
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
            en_id.push_back(temp1);
            i += temp2;
            top_up += temp2;
        }
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
            top_up += temp2 - 1;//? -1 to let (%4 == 3) Check the point-gate before continue to the next user's data
        }
        else if((i - top_up) % 4 == 3 || raw_source.at(i).compare("#*#*#*#*#*#*#*#*") == 0){cout << endl << endl << "Done for user " << (i / 4) + 1 << endl;}
        ////else{cout << endl << endl << "Error! (setup_raw_data)" << endl; exit(0);}
    }
    //cout << "DONE";
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
    // for(int i = 0; i < raw_source.size(); i++){
    //     cout << raw_source.at(i) << endl;
    // }
    // cout << endl << endl << "======================" << endl << endl;
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
    decryption();
    for(int i = 0; i < id.size(); i++){
        cout << id.at(i) << endl;
        cout << password.at(i) << endl;
        cout << text.at(i) << endl << endl;
    }
    cout << "\tPress Enter..."; getchar();
}

void standard_object::test_encryption(){
    test_vector();
    encryption();
    write_log();
}

//!=============================================================================================

void command_action(string &call_command){
    standard_object A;
    vector<string> command = {
        "test_vector","test_encryption","test_read_log"
    };
    short temp2;
    system("clear");//!clear
    // for(int i = 0; i < command.size(); i++){
    //     cout << "\t" << i+1 << ". " << command.at(i) << endl;
    // }
    //----------------------
    for(int i = 0; i < command.size(); i++){
        if(!call_command.compare(command.at(i))){temp2 = i + 1; break;}
    }
    switch(temp2){
        case 1:system("clear");A.test_vector();break;
        case 2:system("clear");A.test_encryption();break;
        case 3:system("clear");A.test_read_log();break;
    }
    temp2 = 0;
}

//?====={function caller}=====
void starting();
//?=========================

int main(){
    starting();
    return 0;
}

void starting(){
    account_system A;
    vector<string> command = {
        "system -start"
    };
    string choice1;
    while(1){
        choice1 = '\0';
        cout << endl << endl << "\tCommand => "; getline(cin,choice1);
        command_action(choice1);
        short temp2;
        for(int i = 0; i < command.size(); i++){
            cout << "\t" << i+1 << ". " << command.at(i) << endl;
        }
        //----------------------
        for(int i = 0; i < command.size(); i++){
            if(!choice1.compare(command.at(i))){temp2 = i + 1; break;}
        }
        switch(temp2){
            case 1: A.main_system(); break;
            default:command_action(choice1);
        }
        system("clear");//!clear
    }
}

void account_system::main_system(){
    string functions[]={"login","logout","create account","delete account","change password","change text"};
    string choice1;
    short temp1 = 0, temp2 = 0;
    while(1){
        if(logined){break;}
        system("clear");
        for(int i = 0; i < sizeof(functions) / sizeof(functions[0]); i++){
            cout << "\t" << i+1 << ". " << functions[i];
            if(i == 0 && logined){cout << " (unavailable)";}
            if(i == 1 && !logined){cout << " (unavailable)";}
            if(i == 2 && logined){cout << " (unavailable)";}
            if(i == 3 && !logined){cout << " (unavailable)";}
            if(i == 4 && !logined){cout << " (unavailable)";}
            if(i == 5 && !logined){cout << " (unavailable)";}
            cout << endl;
        }
        cout << endl << endl << "\tChoose function => "; getline(cin,choice1);
        //---------
        if(!choice1.compare("system -resetfactory")){
            id.clear();
            password.clear();
            text.clear();
            current_index = '\n';
            logined = 0;
            encryption();
            write_log();
            continue;
        }
        //---------
        for(int i = 0; i < sizeof(functions) / sizeof(functions[0]); i++){
            if(!choice1.compare(functions[i])){temp2 = i + 1; break;}
        }
        choice1 = '\0';
        switch(temp2){
            case 1: if(logined){break;} login(); break;
            case 2: if(!logined){break;} logout(); break;
            case 3: if(logined){break;} create_account(); break;
            case 4: if(!logined){break;} delete_account(); break;
            case 5: if(!logined){break;} change_password(); break;
            case 6: if(!logined){break;} change_text(); break;
        }
    }
    //------------------------------------
    while(1){
        if(!logined){main_system();}
        system("clear");
        cout << "\t Welcome, " << id.at(current_index) << "!" << endl << endl;
        for(int i = 0; i < sizeof(functions) / sizeof(functions[0]); i++){
            cout << "\t" << i+1 << ". " << functions[i];
            if(i == 0 && logined){cout << " (unavailable)";}
            if(i == 1 && !logined){cout << " (unavailable)";}
            if(i == 2 && logined){cout << " (unavailable)";}
            if(i == 3 && !logined){cout << " (unavailable)";}
            if(i == 4 && !logined){cout << " (unavailable)";}
            if(i == 5 && !logined){cout << " (unavailable)";}
            cout << endl;
        }
        cout << endl << "Text : " << text.at(current_index) << endl;
        cout << endl << endl << "\tChoose function => "; getline(cin,choice1);
        //---------
        if(!choice1.compare("system -resetfactory")){
            id.clear();
            password.clear();
            text.clear();
            logined = 0;
            current_index = '\n';
            encryption();
            write_log();
            continue;
        }
        //---------
        for(int i = 0; i < sizeof(functions) / sizeof(functions[0]); i++){
            if(!choice1.compare(functions[i])){temp2 = i + 1; break;}
        }
        choice1 = '\0';
        switch(temp2){
            case 1: if(logined){break;} login(); break;
            case 2: if(!logined){break;} logout(); break;
            case 3: if(logined){break;} create_account(); break;
            case 4: if(!logined){break;} delete_account(); break;
            case 5: if(!logined){break;} change_password(); break;
            case 6: if(!logined){break;} change_text(); break;
        }
    }
}

//!=============================================================================================

void account_system::login(){
    read_log();
    setup_raw_data();
    decryption();
    string us, ps;
    bool complete1;
    if(id.size() == 0 || password.size() == 0 || text.size() == 0){cout << "There is no data for login!" << endl << endl << "\tPress Enter..."; getchar();}
    else{
        while(1){
            system("clear");
            cout << "\tLOGIN\n\n";
            cout << "username : "; getline(cin,us);
            cout << "password : "; getline(cin,ps);
            if(us.compare("-cancel") == 0){main_system();}
            for(int i = 0; i < id.size(); i++){
                if(us.compare(id.at(i)) == 0 && ps.compare(password.at(i)) == 0){
                    current_index = i;
                    logined = 1;
                    main_system();
                }
            }
        }
    }
}

void account_system::logout(){
    string choice1;
    while(1){
        system("clear");
        cout << "\tLOGOUT\n\n";
        cout << "Are you sure to logout for now?" << endl << endl << "\tyes\tno\n\n\t :";getline(cin,choice1);
        if(!choice1.compare("yes")){
            current_index = '\n';
            logined = 0;
            break;
        }
        else if(!choice1.compare("no")){
            main_system();
        }
    }
}

void account_system::create_account(){
    read_log();
    setup_raw_data();
    decryption();
    string us, ps;
    while(1){
        system("clear");
        cout << "\tCREATE ACCOUNT\n\n";
        cout << "username : "; getline(cin,us);
        if(us.length() < 4){cout << "\n\ncreate the user more than 3 letters ; now " << us.length() << " letters" << endl << "\tPress Enter..."; getchar(); continue;}
        else if(checking_id(us)){cout << "\n\nThere is already used account" << endl << "\tPress Enter..."; getchar(); continue;}
        else{
            cout << "password : "; getline(cin,ps);
            if(ps.length() < 6){cout << "\n\ncreate the user more than 5 letters ; now " << ps.length() << " letters" << endl << "\tPress Enter..."; getchar(); continue;}
            else if(ps.length() >= 6){break;}
        }
    }
    //-------------------
    id.push_back(us);
    password.push_back(ps);
    text.push_back("None");
    encryption();
    write_log();
}

void account_system::delete_account(){
    if(!logined){cout << "Didn't login yet!" << endl << endl << "\tPress Enter..."; getchar();}
    else{
        string source1;
        while(1){
            system("clear");
            cout << "\tDELETE ACCOUNT\n\n";
            cout << "To delete account, You must enter the password to confirm\n\n\t => "; getline(cin,source1);
            if(source1.compare(password.at(current_index)) == 0){
                logined = 0;
                id.erase(id.begin() + current_index);
                password.erase(password.begin() + current_index);
                text.erase(text.begin() + current_index);
                current_index = '\0';
                break;
            }
            else if(source1.compare("-cancel") == 0){main_system();}
        }
    }
    encryption();
    write_log();
}

void account_system::change_password(){
    string source1, source2;
    while(1){
        system("clear");
        cout << "\tCHANGE PASSWORD\n\n";
        cout << "\tEnter your password : "; getline(cin,source1);
        if(source1.compare("-cancel") == 0){main_system();}
        else if(!source1.compare(password.at(current_index))){break;}
    }
    while(1){
        system("clear");
        cout << "\tEnter new password : "; getline(cin,source2);
        if(source2.length() < 6){cout << "\n\ncreate the user more than 5 letters ; now " << source2.length() << " letters" << endl << "\tPress Enter..."; getchar(); continue;}
        else if(source2.length() >= 6){
            password.at(current_index) = source2;
            encryption();
            write_log();
            main_system();
        }
    }
}

void account_system::change_text(){
    string source1;
    system("clear");
    cout << "\tCHANGE TEXT\n\n";
    cout << "\tEnter your text (without '\\n') : "; getline(cin,source1);
    if(source1.compare("-cancel") == 0){main_system();}
    else{
        text.at(current_index) = source1;
        encryption();
        write_log();
    }
}