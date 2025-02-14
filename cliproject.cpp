#include <iostream>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
using namespace std;

void greet(vector<string> arguments){
    cout << "Hello " << arguments[0] << "!" <<  endl;
}

void exit(vector<string> arguments){
    cout << "Exiting the cli ......." ;
    exit(4);
}

void makeFile(vector<string> arguments){
    ofstream file(arguments[0]);
    if(file){
        cout << "New file " << arguments[0] << " created" << endl;
    }else{
        cout << "Error while creating the file , please check permissions" << endl;
    }
}

void deleteFile(vector<string> arguments){
    int status = remove(arguments[0].c_str());
    if(status == 0){
        cout << "File " << arguments[0] << " removed." << endl;
    }else{
        cout << "Error with file deletion ." << endl; 
    }
}

void repeat(vector<string> arguments){
    if(arguments.size() == 0){
        cout << "";
        return;
    }
    cout << "GarvCli >";
    for(int i = 0 ;i<arguments.size() ;i++){
        cout << arguments[i] << " ";
    }
    cout << endl;
}

void listFiles(vector<string> arguments){
    cout << "Reaching here";
    string input = "dir";
    system(input.c_str());
}

pair<string , vector<string>> parseInput(const string userInput){
    istringstream iss(userInput);
    string command;
    iss >> command;
    string arg;
    vector<string> arguments;
    while(iss >> arg){
        arguments.push_back(arg);
    }

    return { command , arguments };
}

int main(){
    string userInput;
    unordered_map<string , void(*)(vector<string> arguments)> map;
    map["greet"] = greet;
    map["exit"] = exit;
    map["banao"] = makeFile;
    map["hatao"] = deleteFile;
    map["bolo"] = repeat;
    map["filesDikhao"] = listFiles;
    while(true){
        cout << "GarvCli >";
        getline(cin , userInput);
        auto [command , arguments] = parseInput(userInput);

        if(map.find(command) != map.end()){
            map[command](arguments);
        }else{
            cout << "Unknown command" << endl;
        }
        
    }

    string input;
    cin >> input;
    system(input.c_str());
    return 0;
}