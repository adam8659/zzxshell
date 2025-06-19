#include <iostream>
#include <filesystem>
#include <string>
#include <deque>
#include <cstdlib>

namespace fs = std::filesystem;

int main() {
    std::string command;
    int erdp = 0;  // 0 = no error shown, 1 = error shown
    std::deque<std::string> history;  // to store last 25 commands

    while (true) {
	erdp = 0;
        std::cout << fs::current_path().string() << " > ";
        std::getline(std::cin, command);

        // Add command to history
        history.push_back(command);
        if (history.size() > 25) {
            history.pop_front();  // remove oldest command to keep size 25
        }
        if (command == "help"){
		std::cout << "echo - Repeats text\nexit - Exit\ncd - Change directory\ndirs - Command history\nhelp - Shows this message.\n";
		erdp = 1;
	}
	if (command == "dirs") {
            std::cout << "Command history(Last 25):\n";
            for (const auto& cmd : history) {
                std::cout << cmd << '\n';  // newline per command
            }
            erdp = 0;  // reset error flag on normal command
        }
        else if (command == "cd" || command == "cd ") {
            if (erdp == 0) {
                std::cout << "Usage: cd {path name}\n";
                erdp = 1;
            }
        }
	        else if (command == "cd ~") {
            // Handle cd ~ (go to home directory)
            const char* home = std::getenv("HOME");
            if (!home) {
                home = std::getenv("USERPROFILE"); // Windows fallback
            }
            if (home) {
                try {
                    fs::current_path(home);
                    std::cout << "Changed directory to: " << fs::current_path().string() << "\n";
                } catch (const fs::filesystem_error& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
            } else {
                std::cout << "Error: Cannot determine home directory\n";
            }
        }  
        else if (command.substr(0, 3) == "cd ") {
            std::string path = command.substr(3);
            try {
                fs::current_path(path);
                std::cout << "Changed directory to: " << fs::current_path().string() << "\n";
                erdp = 0;  // reset error flag on success
            } catch (const fs::filesystem_error& e) {
                if (erdp == 0) {
                    std::cerr << "Error: " << e.what() << "\n";
                    erdp = 1;
                }
            }
        }
	else if (command.substr(0, 5) == "echo "){
			std::string inp = command.substr(5);
			std::cout << inp << "\n";
	}
        else if (command == "exit") {
            break;
        }
        else {
            if (erdp == 0) {
                std::cout << "Unknown command: " << command << "\n";
                erdp = 1;
            }
        }

        // Optionally reset error flag if command is empty
        if (command.empty()) {
            erdp = 0;
	}
    }
    return 0;
}

