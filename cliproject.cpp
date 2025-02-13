#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;

void greet(){
    cout << "Hello John" << endl;
}
void exit(){
    cout << "Exiting cli " ;
    exit(0);
}

int main(){
    unordered_map<string , void(*)()> commands;
    commands["greet"] = greet;
    commands["exit"] = greet;
    string input;
    while(true){
        cout << "Garv >";
        getline(cin , input);
        if(commands.find(input)!=commands.end()){
            commands[input]();
        }else{
            cout << "Unknown command " << endl;
        }
    }
}