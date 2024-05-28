#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <ctime>
#include <algorithm>

using namespace std;

struct Task {
    string description;
    bool isComplete;
};

struct Message {
    string sender;
    string content;
};

struct Project {
    string id;
    string name;
    string description;
    string startDate;
    string deadline;
    vector<Task> tasks;
    vector<Message> messages;
};

struct User {
    string username;
    string passwordHash;
    string id;
    bool isAdmin;
};

vector<Project> projects;
unordered_map<string, User> users;

string hashPassword(const string& password) {
    // Simple hash function for demonstration purposes
    hash<string> hasher;
    return to_string(hasher(password));
}

void loadProjects() {
    ifstream infile("projects.txt");
    if (infile.is_open()) {
        Project project;
        while (infile >> project.id >> project.name >> project.description >> project.startDate >> project.deadline) {
            projects.push_back(project);
        }
        infile.close();
    }
}

void saveProjects() {
    ofstream outfile("projects.txt");
    if (outfile.is_open()) {
        for (const auto& project : projects) {
            outfile << project.id << " " << project.name << " " << project.description << " "
                    << project.startDate << " " << project.deadline << "\n";
            for (const auto& task : project.tasks) {
                outfile << task.description << " " << task.isComplete << "\n";
            }
            for (const auto& message : project.messages) {
                outfile << message.sender << " " << message.content << "\n";
            }
        }
        outfile.close();
    }
}

void loadUsers() {
    ifstream infile("datausers.txt");
    if (infile.is_open()) {
        User user;
        while (infile >> user.username >> user.passwordHash >> user.id >> user.isAdmin) {
            users[user.username] = user;
        }
        infile.close();
    }
}

void saveUsers() {
    ofstream outfile("datausers.txt");
    if (outfile.is_open()) {
        for (const auto& user : users) {
            outfile << user.first << " " << user.second.passwordHash << " " << user.second.id << " "
                    << user.second.isAdmin << "\n";
        }
        outfile.close();
    }
}

void createProject() {
    Project project;
    cout << "Enter Project ID (max 3 alphanum char): ";
    cin >> project.id;
    cout << "Enter Project Name: ";
    cin.ignore();
    getline(cin, project.name);
    cout << "Enter Project Description: ";
    getline(cin, project.description);
    cout << "Enter Start Date (YYYY-MM-DD): ";
    cin >> project.startDate;
    cout << "Enter Deadline (YYYY-MM-DD): ";
    cin >> project.deadline;
    projects.push_back(project);
    saveProjects();
    cout << "Project created successfully.\n";
}

void showProjectDetails(const Project& project) {
    cout << "Project ID: " << project.id << "\n";
    cout << "Project Name: " << project.name << "\n";
    cout << "Description: " << project.description << "\n";
    cout << "Start Date: " << project.startDate << "\n";
    cout << "Deadline: " << project.deadline << "\n";
    cout << "Tasks:\n";
    for (const auto& task : project.tasks) {
        cout << "- " << task.description << " [" << (task.isComplete ? "Complete" : "Incomplete") << "]\n";
    }
    cout << "Messages:\n";
    for (const auto& message : project.messages) {
        cout << message.sender << ": " << message.content << "\n";
    }
}

void addTask(Project& project) {
    Task task;
    cout << "Enter Task Description: ";
    cin.ignore();
    getline(cin, task.description);
    task.isComplete = false;
    project.tasks.push_back(task);
    saveProjects();
    cout << "Task added successfully.\n";
}

void analyzeTeam(const Project& project) {
    int completedTasks = 0;
    for (const auto& task : project.tasks) {
        if (task.isComplete) {
            completedTasks++;
        }
    }
    double completionRate = (project.tasks.empty()) ? 0.0 : static_cast<double>(completedTasks) / project.tasks.size() * 100;
    cout << "Team Performance Analysis:\n";
    cout << "Total Tasks: " << project.tasks.size() << "\n";
    cout << "Completed Tasks: " << completedTasks << "\n";
    cout << "Completion Rate: " << completionRate << "%\n";
}

void markTaskComplete(Project& project) {
    cout << "List of Tasks:\n";
    for (size_t i = 0; i < project.tasks.size(); ++i) {
        cout << i + 1 << ". " << project.tasks[i].description << " [" << (project.tasks[i].isComplete ? "Complete" : "Incomplete") << "]\n";
    }
    cout << "Enter Task Number to Mark Complete: ";
    int taskNumber;
    cin >> taskNumber;
    if (taskNumber >= 1 && taskNumber <= project.tasks.size()) {
        project.tasks[taskNumber - 1].isComplete = true;
        saveProjects();
        cout << "Task marked as complete.\n";
    } else {
        cout << "Invalid task number.\n";
    }
}

void sendMessage(Project& project, const string& sender) {
    Message message;
    message.sender = sender;
    cout << "Enter Message Content: ";
    cin.ignore();
    getline(cin, message.content);
    project.messages.push_back(message);
    saveProjects();
    cout << "Message sent successfully.\n";
}

void viewMessages(const Project& project) {
    cout << "Messages:\n";
    for (const auto& message : project.messages) {
        cout << message.sender << ": " << message.content << "\n";
    }
}

void collaborate(Project& project, const string& username) {
    int choice;
    do {
        cout << "Collaboration Menu:\n";
        cout << "1. Send Message\n";
        cout << "2. View Messages\n";
        cout << "3. Exit\n";
        cin >> choice;
        switch (choice) {
            case 1:
                sendMessage(project, username);
                break;
            case 2:
                viewMessages(project);
                break;
            case 3:
                return;
        }
    } while (true);
}

void analyzeOthers(const Project& project) {
    // Implement analysis of individual performance based on task completion
    // For simplicity, this will just list tasks and their completion status
    cout << "Individual Performance Analysis:\n";
    for (const auto& task : project.tasks) {
        cout << "Task: " << task.description << " - " << (task.isComplete ? "Complete" : "Incomplete") << "\n";
    }
}

void controlProject() {
    cout << "List of Projects:\n";
    for (size_t i = 0; i < projects.size(); ++i) {
        cout << i + 1 << ". [" << projects[i].id << "] " << projects[i].name << "\n";
    }
    cout << "Enter Project ID to control: ";
    string projectId;
    cin >> projectId;
    auto it = find_if(projects.begin(), projects.end(), [&projectId](const Project& project) {
        return project.id == projectId;
    });
    if (it != projects.end()) {
        cout << "Project Menu for: [" << it->id << "] " << it->name << "\n";
        while (true) {
            cout << "-------CONTROL PANEL--------" << endl;
            cout << "1. Lihat Detail Proyek\n";
            cout << "2. Tambahkan Tugas\n";
            cout << "3. Nilai Kinerja Tim\n";
            cout << "4. Keluar Proyek\n";
            cout << "----------------------------" << endl;
            int choice;
            cin >> choice;
            switch (choice) {
                case 1:
                    showProjectDetails(*it);
                    break;
                case 2:
                    addTask(*it);
                    break;
                case 3:
                    analyzeTeam(*it);
                    break;
                case 4:
                    return;
            }
        }
    } else {
        cout << "Project not found.\n";
    }
}

void registerUser() {
    User user;
    cout << "Enter Username: ";
    cin >> user.username;
    cout << "Enter Password: ";
    string password;
    cin >> password;
    user.passwordHash = hashPassword(password);
    cout << "Enter ID: ";
    cin >> user.id;
    user.isAdmin = false;
    users[user.username] = user;
    saveUsers();
    cout << "User registered successfully.\n";
}

bool login(User& loggedInUser) {
    cout << "Enter Username: ";
    string username;
    cin >> username;
    cout << "Enter Password: ";
    string password;
    cin >> password;
    string passwordHash = hashPassword(password);
    if (users.find(username) != users.end() && users[username].passwordHash == passwordHash) {
        loggedInUser = users[username];
        return true;
    }
    return false;
}

void userJourneyAdmin() {
    while (true) {
        cout << "-------CONTROL PANEL--------" << endl;
        cout << "1. Tambah Proyek\n";
        cout << "2. Kontrol Proyek\n";
        cout << "3. Logout\n";
        cout << "----------------------------" << endl;
        int choice;
        cin >> choice;
        switch (choice) {
            case 1:
                createProject();
                break;
            case 2:
                controlProject();
                break;
            case 3:
                return;
        }
    }
}

void userJourneyNonAdmin(const User& user) {
    auto it = find_if(projects.begin(), projects.end(), [&user](const Project& project) {
        return project.id == user.id;
    });
    if (it == projects.end()) {
        cout << "No project found with matching ID.\n";
        users.erase(user.username);
        saveUsers();
        return;
    }
    cout << "Project Menu for: [" << it->id << "] " << it->name << "\n";
    while (true) {
        cout << "1. Lihat Detail Proyek\n";
        cout << "2. Update Status Tugas\n";
        cout << "3. Komunikasi\n";
        cout << "4. Kolaborasi\n";
        cout << "5. Nilai Kinerja\n";
        cout << "6. Keluar\n";
        int choice;
        cin >> choice;
        switch (choice) {
            case 1:
                showProjectDetails(*it);
                break;
            case 2:
                markTaskComplete(*it);
                break;
            case 3:
                sendMessage(*it, user.username);
                break;
            case 4:
                collaborate(*it, user.username);
                break;
            case 5:
                analyzeOthers(*it);
                break;
            case 6:
                return;
        }
    }
}

void mainMenu() {
    loadProjects();
    loadUsers();

    User loggedInUser;
    while (true) {
        cout << "------- MAIN MENU ---------" << endl;
        cout << "1. Login\n";
        cout << "2. Register\n";
        cout << "3. Exit\n";
        cout << "---------------------------" << endl;
        cout << "Input: ";
        int choice;
        cin >> choice;
        switch (choice) {
            case 1:
                if (login(loggedInUser)) {
                    if (loggedInUser.isAdmin) {
                        userJourneyAdmin();
                    } else {
                        userJourneyNonAdmin(loggedInUser);
                    }
                } else {
                    cout << "!!! Login failed. Invalid username or password. !!!\n";
                }
                break;
            case 2:
                registerUser();
                break;
            case 3:
                return;
        }
    }
}

int main() {
    // Initialize admin account
    User admin;
    admin.username = "admin";
    admin.passwordHash = hashPassword("admin");
    admin.id = "000";
    admin.isAdmin = true;
    users[admin.username] = admin;
    saveUsers();

    mainMenu();
    return 0;
}
