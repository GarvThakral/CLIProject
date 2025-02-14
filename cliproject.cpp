#include <iostream>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <string>
using namespace std;

void greet(vector<string> arguments){
    cout << "Hello john " << endl;
}
void exit(vector<string> arguments){
    cout << "Exiting the cli ......." ;
    exit(4);
}
void makeFile(vector<string> arguments){
    string touchCommand = "touch " + arguments[0];
    string x(touchCommand);
    const char* ccx = x.c_str();
    int executedSuccessfully = system(ccx);
    if(executedSuccessfully == 0){
        cout << "New file " << arguments[0] << " created" << endl;
    }else{
        cout << "Error while creating the file , please check permissions" << endl;
    }
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
    map["make"] = makeFile;
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
    return 0;
}