#include <iostream>
#include <cstdlib>
#include <sstream>
#include <string>
#include <filesystem>

std::string get_path(std::string command) {
  std::string path_env = std::getenv("PATH");
  std::string path; 

  std::stringstream ss(path_env);

  while (!ss.eof()) {
    getline(ss, path, ':'); 
    std::string absolute_path = path + "/" + command; 

    if (std::filesystem::exists(absolute_path)) {
      return absolute_path;
    }
  }

  return ""; 
}

void cd_command(const std::string& input) {

   std::string path = input.substr(input.find_first_of(" ") + 1);
   
   try {
     std::filesystem::current_path(path); 
   } catch (const std::filesystem::filesystem_error&) {
     std::cerr << "cd: " << path << ": No such file or directory" << '\n';
   }
}


int execute_command(const std::string& command) {
  return system(command.c_str()); 
}

void pwd_command() { 
   std::string current_path { std::filesystem::current_path() };
   std::cout << current_path << std::endl; 
}

bool type_command(std::string command) {
  command = command.substr(command.find_first_of(" \t") + 1);
  // whats an enum lmao
  if (!command.compare("exit") || !command.compare("echo") || !command.compare("type") || !command.compare("pwd") || !command.compare("cd")) {
    std::cout << command << " is a shell builtin" << '\n';
    return true; 
  }
  return false;
}


void echo_command(std::string sentence) { 
  sentence = sentence.substr(sentence.find_first_of(" \t") + 1); 

  std::cout << sentence << '\n'; 
} 

void exit_command() {
  return std::_Exit(EXIT_SUCCESS); 
}


bool find_command(std::string input) {
  if (!input.compare("exit 0")) {
    exit_command(); 
    return true; 
  }
  if (input.find("type") != std::string::npos) {
    return type_command(input); 
  }
  if (input.find("echo") != std::string::npos) {
    echo_command(input); 
    return true;
  }
 if (input.find("pwd") != std::string::npos) {
    pwd_command();
    return true; 
  }
  if (input.find("cd") != std::string::npos) {
    cd_command(input);
    return true; 
  }

  return false;
}


int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  while (true) { 
    std::cout << "$ ";

    std::string input;
    std::getline(std::cin, input);
    

    std::string prefix = input.substr(0, input.find(" ")); 
    std::string postfix = input.substr(input.find_first_of(" \t") + 1); 
    if (!find_command(input)) {
      if (!prefix.compare("type")) {
        std::string path = get_path(postfix); 

        if (path.empty()) {
          std::cout << postfix << ": not found" << '\n';
        } else {
          std::cout << postfix << " is " << path << '\n';
        }

      } else {
        std::string path = get_path(prefix); 

        if (path.empty()) {
          std::cout << input << ": command not found" << std::endl;
        } else {
          execute_command(input); 
        }
      }
    }
   }
}
