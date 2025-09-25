#include <iostream>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <filesystem>
#include <ctime>
#ifndef _WIN32
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#else
#include <direct.h>
#include <windows.h>
#endif
#include <typeinfo>
#include <limits>
#include <system_error>
#include <chrono>
#include "system_message.h"
// #include <sqlite3.h>
namespace fs = std::filesystem;
using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::endl;

using namespace std;
// --- Platform helpers -----------------------------------------------------
#ifdef _WIN32
const char PATH_SEP = '\\';
const string DEFAULT_BIN_DIR = "./bin"; // using local ./bin for portability
inline int set_executable(const fs::path &p) { (void)p; return 0; }
#else
const char PATH_SEP = '/';
const string DEFAULT_BIN_DIR = "./bin";
inline int set_executable(const fs::path &p) {
    // make file executable: owner + rx
    return ::chmod(p.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
}
#endif

inline int run_cmd(const string &cmd) {
#ifdef _WIN32
    // run via cmd.exe
    string full = "cmd /C " + cmd;
    return system(full.c_str());
#else
    // prefer bash on POSIX if available
    string full = cmd;
    return system(full.c_str());
#endif
}

// --- Commands -------------------------------------------------------------
void help(vector<string> arguments){
    cout << "\n\n";
    cout << "Welcome to GarvCli" << endl;
    cout << "---------------------------------" << endl;
    cout << "greet arg" << "                     --Say hello" << endl;
    cout << "banao fileName" << "                --Create a new File" << endl;
    cout << "hatao fileName" << "                --Remove a file" << endl;
    cout << "bolo text" << "                     --Similar usage to the echo command" << endl;
    cout << "rinit" << "                         --Initialize a react project " << endl;
    cout << "rinit -t" << "                      --Initialize a react project with tailwind preconfigured" << endl;
    cout << "einit" << "                         --Initialize an express backend" << endl;
    cout << "einit -l" << "                      --Initialize an express backend along with widely used libraries" << endl;
    cout << "gclone --url" << "                  --Clone a github repository , install dependencies and setup the environment" << endl;
    cout << "gpush  -commit_message" << "        --Initialize an express backend along with widely used libraries" << endl;
    cout << "nd" << "                            --alias for the command npm run dev" << endl;
    cout << "compileEXE" << "                    --compile the exe with libraries embedded in it for all device compatability" << endl;
    cout << "backup" << "                        --backup the current project directory" << endl;
    cout << "datetime" << "                      --view the current date and time" << endl;
    cout << "clear" << "                         --clear logs" << endl;
    cout << "exit" << "                          --Exit the cli" << endl;
    cout << endl;
}

void greet(vector<string> arguments){
    cout << "GarvCli > ";
    if(!arguments.empty()) cout << "Hello " << arguments[0] << "!" <<  endl;
    else cout << "Hello!" << endl;
}

void exit_cli(vector<string> arguments){
    cout << "GarvCli > Exiting the cli ......." << endl;
    std::exit(0);
}

void makeFile(vector<string> arguments){
    if(arguments.empty()){
        cout << "GarvCli > Filename required" << endl;
        return;
    }
    ofstream file(arguments[0]);
    cout << "GarvCli > ";
    if(file){
        cout << "New file " << arguments[0] << " created" << endl;
    }else{
        cout << "Error while creating the file , please check permissions" << endl;
    }
}

void deleteFile(vector<string> arguments){
    if(arguments.empty()){
        cout << "GarvCli > Filename required" << endl;
        return;
    }
    int status = std::remove(arguments[0].c_str());
    cout << "GarvCli > ";
    if(status == 0){
        cout << "File " << arguments[0] << " removed." << endl;
    }else{
        cout << "Error with file deletion ." << endl; 
    }
}

void repeat(vector<string> arguments){
    if(arguments.empty()){
        cout << "";
        return;
    }
    cout << "GarvCli > ";
    for(size_t i = 0 ; i < arguments.size(); ++i){
        cout << arguments[i] << " ";
    }
    cout << endl;
}

void listFiles(vector<string> arguments){
#ifdef _WIN32
    run_cmd("dir");
#else
    run_cmd("ls -la");
#endif
}

void createReact(vector<string> arguments){
    char opt = 'n';
    if(!arguments.empty()){
        if(arguments[0] == "-t") opt = 'y';
    }

    std::unordered_map<int,string> projectArgs;
    projectArgs[1] = "-- --template react";
    projectArgs[2] = "-- --template react-ts";

    string projectName;
    cout << "GarvCli > Enter the name of your project : ";
    cin >> projectName;
    cout << endl;
    int option = 1;
    cout << "GarvCli > \n1) Javascript \n2) Typescript \n> ";
    cin >> option;

    string executableString = "npm create vite@latest " + projectName + " " + projectArgs[option] + " && cd " + projectName + " && npm i";
    cout << executableString << endl;
    run_cmd(executableString);

    if(opt == 'y'){
        string installCmd = "cd " + projectName + " && npm install -D tailwindcss@3 postcss autoprefixer";
        run_cmd(installCmd);
        string addScriptCmd = "cd " + projectName + " && npx tailwindcss init -p";
        run_cmd(addScriptCmd);

        ofstream of("./" + projectName + "/tailwind.config.js");
        if(!of){
            cout  << "Error finding tailwind config" << endl;
        }else{
            of << "/** @type {import('tailwindcss').Config} */\n"
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

        of.open("./" + projectName + "/src/index.css");
        if(!of){
            cout  << "Error finding tailwind config" << endl;
        }else{
            of << "@tailwind base;\n@tailwind components;\n@tailwind utilities;";
        }
        of.close();
    }
}

void createExpress(vector<string> arguments){
    cout << "GarvCli > Initializing backend ...." << endl;
    string projectName; 
    cout << "Enter project name : ";
    cin >> projectName;
    char op , lowerOp;
    cout << "Do you want to use typescript ? (y/n)" << endl;
    while(true){
        cout << "GarvCli > ";
        cin >> op;
        lowerOp = static_cast<char>(std::tolower(static_cast<unsigned char>(op)));
        if(lowerOp == 'y' || lowerOp == 'n') break;
        cout << "Incorrect option try again ....." << endl;
    }
    if(lowerOp == 'y'){
        string execString = "mkdir " + projectName;
        run_cmd(execString);
        execString = "cd " + projectName + " && npm init -y && npm install typescript express jsonwebtoken bcrypt cors zod && npm pkg set scripts.build=\"npx tsc -b && node index.js\"";
        run_cmd(execString);
        execString = "cd " + projectName + " && npm install --save-dev @types/express @types/jsonwebtoken @types/bcrypt @types/cors";
        run_cmd(execString);
        execString = "cd " + projectName + " && npx tsc --init";
        run_cmd(execString);
        ofstream file("./" + projectName + "/index.ts");
        file << "import express from 'express';\nimport cors from 'cors'\n\nconst app = express();\napp.use(express.json());\napp.use(cors());\n\napp.listen(3000,()=>{console.log('server running on port 3000')});\n";
        file.close();
    }else{
        string execString = "cd " + projectName + " && npm init -y && npm install express jsonwebtoken bcrypt zod @types/express @types/jsonwebtoken @types/bcrypt && npm pkg set scripts.build=\"node index.js\"";
        run_cmd(execString);
        ofstream("./" + projectName + "/index.js");
    }
    cout << "\nRun command : \nnpm run build\n" << endl;
}

void gitpush(vector<string> arguments){
    string commitMessage;
    if(arguments.empty()){
        cout << "GarvCli > Commit message cannot be empty." << endl;
        return;
    }
    for(size_t i = 0 ; i < arguments.size(); ++i){
        commitMessage += arguments[i] + " ";
    }
    string execString = "git add . && git commit -m \"" + commitMessage + "\" && git push";
    cout << execString << endl;
    run_cmd(execString);
}

void clear_screen(vector<string> arguments){
#ifdef _WIN32
    run_cmd("cls");
#else
    run_cmd("clear");
#endif
};

void datetime(vector<string> arguments){
    time_t timestamp;
    time(&timestamp);
    cout << "GarvCli > " <<  ctime(&timestamp);
}

void compileEXE(vector<string> arguments){
#ifdef _WIN32
    string execString = "g++ -static -static-libgcc -static-libstdc++ -std=c++17 cliproject.cpp -o %CD%\\gex.exe -O2 -Wall -Wextra";
#else
    string execString = "g++ -static -static-libgcc -static-libstdc++ -std=c++17 cliproject.cpp -o ./gex -O2 -Wall -Wextra";
#endif
    run_cmd(execString);
};

void runDev(vector<string> arguments) {
#ifdef _WIN32
    run_cmd("cmd /C npm run dev");
#else
    run_cmd("bash -c \"npm run dev\"");
#endif
}

void clean(vector<string> arguments){
    vector<string> dirVec = {"node_modules" , "dist" , ".vite" , ".turbo" , ".next" , "package-lock.json"};
    for(const auto &d : dirVec){
        fs::path p = fs::current_path() / d;
        if(fs::exists(p)){
            // remove files and directories recursively
            std::error_code ec;
            fs::remove_all(p, ec);
            if(ec){
                cout << "Error removing " << p << ": " << ec.message() << endl;
            }
        }
    }
}

void backup(vector<string> arguments) {
    string dirName;
    bool exists = true;

    while (exists) {
        cout << "GarvCli > Enter the name of the new directory: ";
        std::getline(cin, dirName);
        exists = fs::exists(dirName);
        if (exists) {
            cout << "GarvCli > This folder already exists" << endl;
        } else {
            fs::create_directory(dirName);
            exists = false;

            for (const auto& x : fs::directory_iterator(fs::current_path())) {
                if (x.path().filename() != dirName) {
                    try {
                        fs::path destination = fs::path(dirName) / x.path().filename();

                        if (fs::exists(destination)) {
                            cout << "Skipping " << destination << " because it already exists." << endl;
                            continue;
                        }
                        if (fs::is_directory(x)) {
                            fs::copy(x, destination, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
                        } else {
                            fs::copy(x, destination, fs::copy_options::overwrite_existing);
                        }

                    } catch (const std::exception& e) {
                        cout << "Error copying " << x.path() << ": " << e.what() << endl;
                    }
                }
            }
        }
    }
}

void createCommandFile(vector<string> arguments){
    if(arguments.empty()){
        cout << "GarvCli > Arguments are required to run this command" << endl;
        return;
    }

    // choose a local ./bin directory for portability
    fs::path binDir = fs::current_path() / "bin";
    if(!fs::exists(binDir)) fs::create_directories(binDir);

    for(size_t i = 0 ; i < arguments.size() ; ++i){
        string commandName = arguments[i];
        fs::path winPath = binDir / (commandName + ".bat");
        fs::path shPath  = binDir / commandName;

        // Windows .bat
        {
            std::ofstream out1(winPath);
            if(out1.is_open()){
                out1 << "@echo off\n";
                out1 << "set scriptdir=%~dp0\n";
                out1 << "\"%scriptdir%gex.exe\" " + commandName + " %*\n";
            }
        }

        // POSIX shell wrapper
        {
            std::ofstream out(shPath);
            if(out.is_open()){
                out << "#!/bin/sh\n";
                out << "$(dirname \"$0\")/gex " + commandName + " \"$@\"\n";
            }
            // try to make it executable on POSIX
#ifndef _WIN32
            set_executable(shPath);
#endif
        }
    }
}

void ml_init(vector<string> arguments){
    if(arguments.size() == 0){
        cout << "GarvCli > ml-init project1 3.11 scikit-learn lab" << endl;
        cout << "GarvCli > ml-init <env_name> <version> <specific-libraries>" << endl;
        return;
    }
    double version = 0.0;
    try{
        version = stod(arguments[1]);
    } catch(...) {
        cout << "GarvCli > Invalid version provided" << endl; return;
    }
    std::string str = std::to_string(version);
    str.erase ( str.find_last_not_of('0') + 1, std::string::npos );
    str.erase ( str.find_last_not_of('.') + 1, std::string::npos );
    if(!(version < 4.00)){
        cout << "GarvCli > Invalid python version" << endl;
    }
    string libraries = "";
    if(arguments.size() > 2){
        for(size_t i = 2 ; i < arguments.size() ; ++i){
            libraries += (arguments[i] + " ");
        }
        libraries+= "jupyterlab";
    }
    string command = "conda create -n " + arguments[0]  + " python=" + str + " -y" ;
    run_cmd(command);
    string activate = "conda activate " + arguments[0];
    run_cmd(activate);
    if(!libraries.empty()){
        string install = "conda install " + libraries + " -y";
        run_cmd(install);
    }
}

void runCFile(vector<string> arguments){
    if(arguments.empty()){
        cout << "GarvCli > File name not specified" <<endl;
        return;
    }
    string execString;
#ifdef _WIN32
    execString = "g++ " + arguments[0] + ".cpp -o " + arguments[0] + " && " + arguments[0];
#else
    execString = "g++ " + arguments[0] + ".cpp -o " + arguments[0] + " && ./" + arguments[0];
#endif
    run_cmd(execString);
}

string detectProjectType(){
    bool nodeProject = fs::exists("package.json");
    bool pythonProject = fs::exists("requirements.txt");
    if(nodeProject) return "Node";
    if(pythonProject) return "Python";
    return "Unidentified";
}

void checkDeps(vector<string> arguments){
    string projectType = detectProjectType();
    if(projectType == "Node"){
        cout << "GarvCli > Node project detected , running appropriate checks ....." << endl;
        run_cmd("npm outdated");
        run_cmd("npm audit");
    }else if(projectType == "Python"){
        cout << "GarvCli > Python project detected , running appropriate checks ....." << endl;
        run_cmd("pip list --outdated");
    }else{
        cout << "Invalid project or support not available in this version " << endl;
    }
}

void updateDeps(vector<string> arguments){
    string type = detectProjectType();
    if(type == "Node"){
        cout << "GarvCli > Node project detected updating dependencies in package.json ....." << endl;
        run_cmd("npm update");
    }else if(type == "Python"){
        // NOTE: `system` return 0 on success; handle gracefully
        if(run_cmd("pip show pip-upgrader") == 0){
            cout << "GarvCli > Python project detected updating dependencies in requirements.txt ....." << endl;
            run_cmd("pip install pip-upgrader");
            run_cmd("pip-upgrade --skip-virtualenv-check");
        }
    }else{
        cout << "Invalid project or support not available in this version " << endl;
    }
}

void dataset(vector<string> arguments){
    if(arguments.empty()){
        cout << "GarvCli > Enter a valid dataset source . Supported 'kaggle' 'hf'" << endl;
        return;
    }
    if(arguments[0] == "kaggle"){
        char buffer [1024];
        // avoid using ~ expansion — use $HOME or let curl handle it in shell
        snprintf(buffer, sizeof(buffer), "curl -L -o $HOME/Downloads/%s.zip https://www.kaggle.com/api/v1/datasets/download/%s", arguments[1].c_str(), arguments[1].c_str());
        run_cmd(buffer);
    }else if(arguments[0] == "hf"){
        SystemMessage("Ensure that you have installed datasets , if you haven't already then run 'pip install datasets'");
        char buffer[1024];
        snprintf(buffer, sizeof(buffer), "python -c 'from datasets import load_dataset; ds = load_dataset(\"%s\")'", arguments[1].c_str());
        run_cmd(buffer);
        cout << "Dataset downloaded to ~/.cache/huggingface/datasets" << endl;
    }else{
        cout << "GarvCli > Enter a valid dataset source . Supported 'kaggle' 'hf'" << endl;
    }
}

void benchmark(vector<string> arguments){
    if(arguments.empty()){
        cout << "Pass a command as argument to benchmark your machine " << endl;
        return;
    }
    string execString;
    for(size_t i = 0 ; i < arguments.size(); ++i){
        execString += arguments[i] + " ";
    }
    auto start = std::chrono::high_resolution_clock::now();
    run_cmd(execString);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout<< "Execution Time:\n" << duration.count() << " ms\n";
    double seconds = duration.count() / 1000.0;
    cout  << seconds << " s" << endl;
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    (void)NotUsed;
    for(int i = 0; i<argc; i++) {
       printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
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
    unordered_map<string , void(*)(vector<string> arguments)> map;
    map["greet"] = greet;
    map["exit"] = exit_cli; // renamed to avoid conflict with std::exit
    map["banao"] = makeFile;
    map["hatao"] = deleteFile;
    map["bolo"] = repeat;
    map["filesDikhao"] = listFiles;
    map["Garv"] = help;
    map["rinit"] = createReact;
    map["einit"] = createExpress;
    map["gpush"] = gitpush;
    map["clear"] = clear_screen;
    map["datetime"] = datetime;
    map["compileEXE"] = compileEXE;
    map["backup"] = backup;
    map["create"] = createCommandFile;
    map["crun"] = runCFile;
    map["nd"] = runDev;
    map["clean"] = clean;
    map["deps-check"] = checkDeps;
    map["benchmark"] = benchmark;
    map["ml-init"] = ml_init;
    map["dataset"] = dataset;

    if(argc > 1){
        string command = argv[1];
        vector<string> arguments;
        for(int i = 2 ; i < argc;i++){
            arguments.push_back(argv[i]);
        }
        if(map.find(command) != map.end()){
            map[command](arguments);
            return 0;
        }else{
            cout << "Unknown command" << endl;
        }
    }
    while(true){
        SystemMessage("");
        std::getline(cin , userInput);
        auto [command , arguments] = parseInput(userInput);
        if(map.find(command) != map.end()){
            map[command](arguments);
        }else{
            cout << "GarvCli > Unknown command" << endl;
        }
    }
    return 0;
}
