 string userInput;
    unordered_map<string , void(*)(vector<string> arguments)> map;
    map["greet"] = greet;
    map["exit"] = exit;
    map["banao"] = makeFile;
    map["hatao"] = deleteFile;
    map["bolo"] = repeat;
    map["filesDikhao"] = repeat;
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