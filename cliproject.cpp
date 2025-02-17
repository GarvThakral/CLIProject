#include <iostream>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
using namespace std;

void greet(vector<string> arguments){
    cout << "GarvCli > ";
    cout << "Hello " << arguments[0] << "!" <<  endl;
}

void exit(vector<string> arguments){
    cout << "GarvCli > ";
    cout << "Exiting the cli ......." ;
    exit(4);
}

void makeFile(vector<string> arguments){
    ofstream file(arguments[0]);
    cout << "GarvCli > ";
    if(file){
        cout << "New file " << arguments[0] << " created" << endl;
    }else{
        cout << "Error while creating the file , please check permissions" << endl;
    }
}

void deleteFile(vector<string> arguments){
    int status = remove(arguments[0].c_str());
    cout << "GarvCli > ";
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
    cout << "GarvCli > ";
    for(int i = 0 ;i<arguments.size() ;i++){
        cout << arguments[i] << " ";
    }
    cout << endl;
}

void listFiles(vector<string> arguments){
    string input = "dir";
    system(input.c_str());
}

void help(vector<string> arguments){
    cout << endl << endl;
    cout << "Heres how to use this Cli" << endl;
    cout << endl;
    cout << "---------------------------------";
    cout << endl;
    cout << "greet arg" << "            --Say hello" << endl;
    cout << "banao fileName" << "       --Create a new File" << endl;
    cout << "hatao fileName" << "       --Remove a file" << endl;
    cout << "bolo text" << "            --Similar usage to the echo command" << endl;
    cout << "reactinit" << "            --Initialize a react project" << endl;
    cout << "exit" << "                 --Exit the cli" << endl;
    cout << endl << endl;
}

void createReact(vector<string> arguments){

    unordered_map<int,string> projectArgs;
    projectArgs[1] = "-- --template react";
    projectArgs[2] = "-- --template react-ts";

    string projectName;
    cout << "GarvCli > Enter the name of your project : ";
    cin >> projectName;
    cout << endl;
    
    int option;
    cout << "GarvCli > " << endl;
    cout << "1) Javascript " << endl;
    cout << "2) Typescript " << endl;
    cout << "> ";
    
    cin >> option;

    string executableString = "npm create vite@latest " + projectName + " " + projectArgs[option] + " && cd " + projectName + " && npm i";
    cout << executableString <<endl;
    system(executableString.c_str());
    
    char lowerOp;

    while(true){
        cout << "Do you want to initialise tailwindCSS ?" << endl;
        cout << "(y/n) : " ;
        char opt;
        cin >> opt;
        lowerOp = tolower(opt);
        if(lowerOp == 'y'){
            string installCmd = "cd " + projectName + " && npm install -D tailwindcss@3 postcss autoprefixer";
            system(installCmd.c_str());

            // Create a temporary package.json script to run tailwind init
            string addScriptCmd = "cd " + projectName + " && npx tailwindcss init -p";
            system(addScriptCmd.c_str());
            
            ofstream of;
            of.open("./"+projectName+"/tailwind.config.js");
            if(!of){
                cout  << "Error finding tailwind config" << endl;
            }else{
                of << 
                "/** @type {import('tailwindcss').Config} */\n"
                "export default {\n"
                "  content: [\n"
                "    './index.html',\n"
                "    './src/**/*.{js,ts,jsx,tsx}',\n"
                "  ],\n"
                "  theme: {\n"
                "    extend: {},\n"
                "  },\n"
                "  plugins: [],\n"
                "}";
            }
            of.close();


            of.open("./"+projectName+"/src/index.css");
            if(!of){
                cout  << "Error finding tailwind config" << endl;
            }else{
                of << 
                "@tailwind base;\n"
                "@tailwind components;\n"
                "@tailwind utilities;";
            }
            of.close();
            break;
        }else if(lowerOp == 'n'){
            break;
        }else{
            cout << "Invalid command" << endl;
        }
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
    cout << "Type Garv for help > " << endl;
    unordered_map<string , void(*)(vector<string> arguments)> map;
    map["greet"] = greet;
    map["exit"] = exit;
    map["banao"] = makeFile;
    map["hatao"] = deleteFile;
    map["bolo"] = repeat;
    map["filesDikhao"] = listFiles;
    map["Garv"] = help;
    map["reactinit"] = createReact;

    while(true){
        cout << "GarvCli > ";
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