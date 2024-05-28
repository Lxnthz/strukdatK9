#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include <map>
#include <algorithm>
#include <functional> // for std::hash

using namespace std;

struct TeamMember {
    string id;
    string name;
};

struct Project {
    string id;
    string name;
    string description;
    time_t startDate;
    time_t deadline;
    map<string, vector<TeamMember>> teams; // e.g., {"frontend": [TeamMember1, TeamMember2], "backend": [TeamMember3]}
    string admin;
};

struct User {
    string username;
    size_t hashedPassword;
    string projectID;
};

vector<Project> projects;
vector<User> users;

// Function to generate a random project ID
string generateProjectID() {
    const char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string id;
    for (int i = 0; i < 3; ++i) {
        id += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return id;
}

// Helper function to convert date string to time_t
time_t convertStringToDate(const string& dateStr) {
    struct tm tm = {};
    istringstream ss(dateStr);
    ss >> tm.tm_year;
    ss.ignore(); // ignore the '-'
    ss >> tm.tm_mon;
    ss.ignore(); // ignore the '-'
    ss >> tm.tm_mday;
    tm.tm_year -= 1900; // tm_year is years since 1900
    tm.tm_mon -= 1;     // tm_mon is 0-11 for Jan-Dec
    return mktime(&tm);
}

// Helper function to convert time_t to date string
string convertDateToString(time_t date) {
    struct tm* tm_info = localtime(&date);
    char buffer[11];
    strftime(buffer, 11, "%Y-%m-%d", tm_info);
    return string(buffer);
}

// Function to display all projects for SuperAdmin
void displayProjectList() {
    cout << "List proyek:\n";
    for (const auto& project : projects) {
        cout << "- [" << project.id << "] " << project.name << "\n";
    }
}

// Function to display project details for SuperAdmin
void displayProjectDetails(const string& projectID) {
    for (const auto& project : projects) {
        if (project.id == projectID) {
            cout << "ID Proyek: " << project.id << "\n";
            cout << "Nama Proyek: " << project.name << "\n";
            cout << "Deskripsi Proyek: " << project.description << "\n";
            cout << "Tanggal Mulai: " << convertDateToString(project.startDate) << "\n";
            cout << "Deadline: " << convertDateToString(project.deadline) << "\n";
            cout << "Tim:\n";
            for (const auto& team : project.teams) {
                cout << "  " << team.first << ":\n";
                for (const auto& member : team.second) {
                    cout << "    - " << member.name << " (ID: " << member.id << ")\n";
                }
            }
            cout << "Admin: " << project.admin << "\n";
            return;
        }
    }
    cout << "Proyek dengan ID tersebut tidak ditemukan.\n";
}

// Function to create a new project
void createProject() {
    Project newProject;
    newProject.id = generateProjectID();
    cout << "Masukkan nama proyek: ";
    cin.ignore();
    getline(cin, newProject.name);
    cout << "Masukkan deskripsi proyek: ";
    getline(cin, newProject.description);
    cout << "Masukkan tanggal mulai (format: YYYY-MM-DD): ";
    string startDateStr;
    cin >> startDateStr;
    newProject.startDate = convertStringToDate(startDateStr);
    cout << "Masukkan deadline proyek (format: YYYY-MM-DD): ";
    string deadlineStr;
    cin >> deadlineStr;
    newProject.deadline = convertStringToDate(deadlineStr);
    cout << "Masukkan nama admin proyek: ";
    cin.ignore();
    getline(cin, newProject.admin);

    projects.push_back(newProject);
    cout << "Proyek berhasil dibuat dengan ID: " << newProject.id << "\n";
}

// Function to hash a password
size_t hashPassword(const string& password) {
    return hash<string>{}(password);
}

// Function to load users from file
void loadUsers() {
    ifstream file("datausers.txt");
    if (!file.is_open()) return;

    User user;
    while (file >> user.username >> user.hashedPassword >> user.projectID) {
        users.push_back(user);
    }
    file.close();
}

// Function to save users to file
void saveUsers() {
    ofstream file("datausers.txt");
    for (const auto& user : users) {
        file << user.username << ' ' << user.hashedPassword << ' ' << user.projectID << '\n';
    }
    file.close();
}

// Function to load projects from file
void loadProjects() {
    ifstream file("dataprojects.txt");
    if (!file.is_open()) return;

    Project project;
    string startDateStr, deadlineStr;
    while (file >> project.id >> project.name >> project.description >> startDateStr >> deadlineStr >> project.admin) {
        project.startDate = convertStringToDate(startDateStr);
        project.deadline = convertStringToDate(deadlineStr);

        int numTeams;
        file >> numTeams;
        for (int i = 0; i < numTeams; ++i) {
            string teamName;
            int numMembers;
            file >> teamName >> numMembers;
            vector<TeamMember> teamMembers;
            for (int j = 0; j < numMembers; ++j) {
                TeamMember member;
                file >> member.id >> member.name;
                teamMembers.push_back(member);
            }
            project.teams[teamName] = teamMembers;
        }
        projects.push_back(project);
    }
    file.close();
}

// Function to save projects to file
void saveProjects() {
    ofstream file("dataprojects.txt");
    for (const auto& project : projects) {
        file << project.id << ' ' << project.name << ' ' << project.description << ' '
             << convertDateToString(project.startDate) << ' ' << convertDateToString(project.deadline) << ' '
             << project.admin << '\n';

        file << project.teams.size() << '\n';
        for (const auto& team : project.teams) {
            file << team.first << ' ' << team.second.size() << '\n';
            for (const auto& member : team.second) {
                file << member.id << ' ' << member.name << '\n';
            }
        }
    }
    file.close();
}

// Function to register a new user
void registerUser() {
    User newUser;
    cout << "Masukkan username: ";
    cin >> newUser.username;
    string password;
    cout << "Masukkan password: ";
    cin >> password;
    newUser.hashedPassword = hashPassword(password);
    cout << "Masukkan ID proyek: ";
    cin >> newUser.projectID;

    users.push_back(newUser);
    saveUsers();
    cout << "User berhasil terdaftar.\n";
}

// Function to login a user
bool loginUser(User& loggedInUser) {
    string username, password;
    cout << "Masukkan username: ";
    cin >> username;
    cout << "Masukkan password: ";
    cin >> password;
    size_t hashedPassword = hashPassword(password);

    for (const auto& user : users) {
        if (user.username == username && user.hashedPassword == hashedPassword) {
            loggedInUser = user;
            return true;
        }
    }
    return false;
}

// Function to check if SuperAdmin exists
bool superAdminExists() {
    for (const auto& user : users) {
        if (user.username == "superadmin" && user.projectID == "SuperAdmin") {
            return true;
        }
    }
    return false;
}

// Function to add SuperAdmin
void addSuperAdmin() {
    if (!superAdminExists()) {
        User superAdmin;
        superAdmin.username = "superadmin";
        superAdmin.hashedPassword = hashPassword("superpassword");
        superAdmin.projectID = "SuperAdmin";
        users.push_back(superAdmin);
        saveUsers();
        cout << "SuperAdmin berhasil ditambahkan.\n";
    } else {
        cout << "SuperAdmin sudah ada.\n";
    }
}

// Main menu function
void showMainMenu(const User& loggedInUser) {
    int choice;
    do {
        cout << "\nMenu:\n";
        cout << "1. Tambah Proyek Baru\n";
        cout << "2. Tampilkan Semua Proyek\n";
        cout << "3. Edit Informasi Proyek\n";
        cout << "4. Logout\n";
        cout << "5. Keluar\n";
        cout << "Pilihan: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                if (loggedInUser.projectID == "SuperAdmin") {
                    createProject();
                    saveProjects(); // Save projects after creation
                } else {
                    cout << "Anda tidak memiliki izin untuk menambah proyek.\n";
                }
                break;
            case 2:
                displayProjectList();
                break;
            case 3: {
                cout << "Masukkan ID proyek yang ingin dilihat: ";
                string projectID;
                cin >> projectID;
                displayProjectDetails(projectID);
                break;
            }
            case 4:
                cout << "Logging out...\n";
                return;
            case 5:
                cout << "Exiting...\n";
                return;
            default:
                cout << "Pilihan tidak valid.\n";
        }
    } while (choice != 5);
}

int main() {
    srand(time(nullptr)); // Initialize random seed
    loadUsers();
    addSuperAdmin(); // Add SuperAdmin if not already present
    loadProjects(); // Load projects from file

    int choice;
    User loggedInUser;
    do {
        cout << "\nLogin/Register Menu:\n";
        cout << "1. Login\n";
        cout << "2. Register\n";
        cout << "3. Keluar\n";
        cout << "Pilihan: ";
        cin >> choice;

        switch (choice) {
            case 1:
                if (loginUser(loggedInUser)) {
                    cout << "Login berhasil.\n";
                    showMainMenu(loggedInUser);
                } else {
                    cout << "Login gagal. Username atau password salah.\n";
                }
                break;
            case 2:
                registerUser();
                break;
            case 3:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Pilihan tidak valid.\n";
        }
    } while (choice != 3);

    saveProjects(); // Save projects before exiting

    return 0;
}
