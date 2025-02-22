#include <iostream>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <filesystem>
#include <ctime>

namespace fs = std::filesystem;
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
    cout << "Welcome to GarvCli" << endl;
    cout << "---------------------------------";
    cout << endl;
    cout << "greet arg" <<                     "--Say hello" << endl;
    cout << "banao fileName" <<                "--Create a new File" << endl;
    cout << "hatao fileName" <<                "--Remove a file" << endl;
    cout << "bolo text" <<                     "--Similar usage to the echo command" << endl;
    cout << "reactinit" <<                     "--Initialize a react project " << endl;
    cout << "reactinit -t" <<                  "--Initialize a react project with tailwind preconfigured" << endl;
    cout << "expressinit" <<                   "--Initialize an express backend" << endl;
    cout << "expressinit -l" <<                "--Initialize an express backend along with widely used libraries" << endl;
    cout << "gitpush  -commit_message" <<      "--Initialize an express backend along with widely used libraries" << endl;
    cout << "clear" <<                         "--clear logs" << endl;
    cout << "exit" <<                          "--Exit the cli" << endl;
    cout << endl;
}

void createReact(vector<string> arguments){
    char opt;

    if(!arguments.size() == 0){
        if(arguments[0] == "-t"){
            opt = 'y';
        }
    }else{
        opt = 'n';
    }
    


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
    
    while(true){
        
        if(opt == 'y'){
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
        }else if(opt == 'n'){
            break;
        }else{
            cout << "Invalid command" << endl;
        }
    }

}

void createExpress(vector<string> arguments){
    cout << "GarvCli >" << "Initializing backed ....";
    string projectName; 
    cout << "Enter project name : ";
    cin >> projectName;
    char op , lowerOp;
    cout << "Do you want to use typescript ? (y/n)" << endl;
    while(true){
        cout << "GarvCli > ";
        cin >> op;
        lowerOp = tolower(op);
        if(lowerOp == 'y'){
            break;
        }else if(lowerOp == 'n'){
            break;
        }else{
            cout << "Incorrect option try again ....." << endl;
        }
    }
    if(lowerOp == 'y'){
        string execString = "mkdir "+projectName;
        system(execString.c_str());
        execString = "cd " + projectName + " && npm init -y && npm install typescript express jsonwebtoken bcrypt cors zod && npm pkg set scripts.build=\"npx tsc -b && node index.js\" " ;
        system(execString.c_str());
        execString = "cd " + projectName + " && npm install --save-dev @types/express @types/jsonwebtoken @types/bcrypt @types/cors" ;
        system(execString.c_str());
        execString = "cd " + projectName + "&& npx tsc --init";
        cout << execString << endl;
        system(execString.c_str());
        ofstream {"./" + projectName + "/index.ts"};
        ofstream file("./"+projectName+"/index.ts");
        file << 
        "import express from 'express';\n"
        "import cors from 'cors'\n\n"
        "const app = express();\n"
        "app.use(express.json());\n"
        "app.use(cors());\n\n\n\n"
        "app.listen(3000,()=>{console.log('server running on port 3000')});\n";
        file.close();
    }else{
        string execString = "cd " + projectName + " && npm init -y && npm install express jsonwebtoken bcrypt zod @types/express @types/jsonwebtoken @types/bcrypt && npm pkg set scripts.build=\"node index.js\" " ;
        system(execString.c_str());
        ofstream {"./" + projectName + "/index.js"};
    }
    cout << endl;
    cout << "Run command : \n" ;
    cout << "npm run build\n" << endl;
}

void gitpush(vector<string> arguments){
    string commitMessage;
    if(arguments.size() == 0){
        cout << "GarvCli > Commit message cannot be empty." << endl;
        return;
    }
    for(int i = 0 ; i < arguments.size() ; i++){
        commitMessage += arguments[i];
        commitMessage += " ";
    }
    string execString = "git add . && git commit -m \"" + commitMessage + "\" && git push";
    cout << execString << endl;
    system(execString.c_str());
}

void clear(vector<string> arguments){
    system("cls");
};

void datetime(vector<string> arguments){
    time_t timestamp;
    time(&timestamp);
    
    // Display the date and time represented by the timestamp
    cout << "GarvCli > " <<  ctime(&timestamp);
}

void compileEXE(vector<string> arguments){
    string execString = "g++ -static -static-libgcc -static-libstdc++ -o gex.exe cliproject.cpp";
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

int main(int argc, char* argv[]){
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
    map["expressinit"] = createExpress;
    map["gitpush"] = gitpush;
    map["clear"] = clear;
    map["datetime"] = datetime;
    map["compileEXE"] = compileEXE;

    if(argc > 1){
        string command = argv[1];
        vector<string> arguments;
        for(int i = 2 ; i < argc;i++){
            arguments.push_back(argv[i]);
        }
        if(map.find(command) != map.end()){
            map[command](arguments);
        }else{
            cout << "Unknown command" << endl;
        }
    }
    while(true){
        cout << "GarvCli > " ;
        getline(cin , userInput);
        auto [command , arguments] = parseInput(userInput);

        if(map.find(command) != map.end()){
            map[command](arguments);
        }else{
            cout << "GarvCli > " ;
            cout << "Unknown command" << endl;
        }
        
    }
    string input;
    cin >> input;
    system(input.c_str());
    return 0;
}