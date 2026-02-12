#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>
#include <algorithm>

void roles_list (std::vector<std::string> v) {
    std::cout << "Number of roles left: " << v.size() << std::endl;
    
    std::cout << "Roles play: ";
    for (int i = 0; i < v.size(); i++) {
        std::cout << v[i] << ", ";
    }
    std::cout << std::endl << std::endl;
}

void checkIf_deleted (std::vector<std::string> v) {
            std::cout << "Roles left: ";
            for (const auto& r : v)
                    std::cout << r << ", ";
                std::cout << "\n\n";
}

std::string check_yesNo (std::string answer) {
    while (std::cin >> answer && !(answer == "yes" || answer == "Yes" || answer == "no" || answer == "No")) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "ERROR: Only 'yes' or 'no' answer: ";
//        std::cin >> answer;
    }
    return answer;
}

int main () {
//    std::setlocale (LC_ALL, "UA");
    
    //Count of players
    int playerCount = 0;
    std::cout << "Number of players: ";
    while (!(std::cin >> playerCount)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "ERROR: Invalid input. Please enter number: ";
    }
    if (playerCount < 6) {
        std::cerr << "ERROR: Not enough players. Recommended minimum of players - 6.\n";
        return 1;
    }
    
    std::vector<int> playerNums = {};
    for (int i = 1; i <= playerCount; i++) {
        playerNums.push_back(i);
    }
    
    std::vector<std::string> roles = {};
    
    int host = 0;
    std::cout << "Host - Player#: ";
    std::cin >> host;
    roles.push_back("Host");
    
    //Count of Mafia
    int mafiaCount = 0;
    std::cout << "Number of Mafia: ";
    while (!(std::cin >> mafiaCount)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "ERROR: Invalid input. Please enter number: ";
    }
    while ((playerCount < 8 && mafiaCount > 1) || (playerCount < 10 && mafiaCount > 2) || (playerCount < 15 && mafiaCount > 3)) {
        std::cerr << "Too many Mafia for " << playerCount << " players.\n";
        std::cout << "Enter correct number of Mafia: ";
        std::cin >> mafiaCount;
    }
    while (mafiaCount < 1) {
        std::cerr << "ERROR: Should be at least 1 Mafia.\n";
        std::cout << "Enter correct number of Mafia: ";
        std::cin >> mafiaCount;
    }
    
    
    std::string answer;
    
    bool don = false;
    std::cout << "Is Mafia Don playing?: ";
    std::string isDon = check_yesNo(answer); // Checking correct yes/no answer
    if (isDon == "Yes" || isDon == "yes") {
        don = true;
        roles.push_back("Mafia Don");
        mafiaCount--; // Don replaces one mafia
    }

    for (int i = 0; i < mafiaCount; i++) {
        roles.push_back("Mafia");
    }
    
    bool doc = false;
    std::cout << "Is Doctor playing: ";
    std::string isDoctor = check_yesNo(answer); // Checking correct yes/no answer
    if (isDoctor == "Yes" || isDoctor == "yes") {
        doc = true;
        roles.push_back("Doctor");
    }
    
    bool sh = false;
    std::cout << "Is Sheriff playing?: ";
    std::string isSheriff = check_yesNo(answer); // Checking correct yes/no answer
    if (isSheriff == "Yes" || isSheriff == "yes") {
        sh = true;
        roles.push_back("Sheriff");
    }
        
    while (roles.size() < playerCount) {
        roles.push_back("Citizen");
    }
    
    //Check list and count of roles
// roles_list(roles);
    
    if (host > 0) {
        std::cout << "\nPlayer#" << host << ": Host\n\n";
        
        // Removing Host from players number
        playerNums.erase(std::remove(playerNums.begin(), playerNums.end(), host), playerNums.end());
        
        // Removing Host from roles list.
        auto it = std::find(roles.begin(), roles.end(), "Host");
        if (it != roles.end()) {
            roles.erase(it);
        }
    }

    
    // Shuffle roles
    std::mt19937 gen(std::random_device{}());
    std::shuffle(roles.begin(), roles.end(), gen);
    
    //Randomly assign role
    for (int i = 0; i < playerNums.size(); i++) {
        std::uniform_int_distribution<std::size_t> dist (0,(roles.size()-1));
        std::size_t role = dist(gen);
        std::cout << "Player #" << playerNums[i] << ": " << roles[role] << std::endl;
        //Delete assigned role
        roles.erase(roles.begin() + role);
        
        //Check if assigned role was deleted
//        checkIf_deleted (roles);
    }
    
    return 0;
}





