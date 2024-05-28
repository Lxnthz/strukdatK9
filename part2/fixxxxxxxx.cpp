#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <queue>
#include <algorithm>

using namespace std;

struct Task {
    string description;
    bool isComplete;
    string userID;
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
    ifstream infile("dataprojects.txt");
    if (infile.is_open()) {
        Project project;
        while (getline(infile, project.id, '|') && getline(infile, project.name, '|') &&
               getline(infile, project.description, '|') && getline(infile, project.startDate, '|') &&
               getline(infile, project.deadline)) {
            projects.push_back(project);
        }
        infile.close();
    }
}

void saveProjects() {
    ofstream outfile("dataprojects.txt");
    if (outfile.is_open()) {
        for (const auto& project : projects) {
            outfile << project.id << "|" << project.name << "|" << project.description << "|"
                    << project.startDate << "|" << project.deadline << "\n";
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
    cin.ignore();
    cout << "Enter Project Name: ";
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

void deleteProject() {
    cout << "List of Projects:\n";
    for (size_t i = 0; i < projects.size(); ++i) {
        cout << i + 1 << ". [" << projects[i].id << "] " << projects[i].name << "\n";
    }
    cout << "Enter Project ID to delete: ";
    string projectId;
    cin >> projectId;
    auto it = remove_if(projects.begin(), projects.end(), [&projectId](const Project& project) {
        return project.id == projectId;
    });
    if (it != projects.end()) {
        projects.erase(it, projects.end());
        saveProjects();
        cout << "Project deleted successfully.\n";
    } else {
        cout << "Project not found.\n";
    }
}

void showProjectDetails(const Project& project) {
    cout << "Project ID: " << project.id << "\n";
    cout << "Project Name: " << project.name << "\n";
    cout << "Description: " << project.description << "\n";
    cout << "Start Date: " << project.startDate << "\n";
    cout << "Deadline: " << project.deadline << "\n";

    ifstream infileTasks("dataTask.txt");
    if (infileTasks.is_open()) {
        queue<Task> taskQueue;
        string projectID, description, userID;
        bool isComplete;
        while (infileTasks >> projectID >> ws && getline(infileTasks, description, '|') && infileTasks >> userID >> isComplete) {
            if (projectID == project.id) {
                taskQueue.push({description, isComplete, userID});
                if (taskQueue.size() > 3) {
                    taskQueue.pop();
                }
            }
        }
        infileTasks.close();

        cout << "Tasks:\n";
        while (!taskQueue.empty()) {
            Task task = taskQueue.front();
            taskQueue.pop();
            cout << "- " << task.description << " [" << (task.isComplete ? "Complete" : "Incomplete") << "]\n";
        }
    }

    ifstream infileMessages("datamessage.txt");
    if (infileMessages.is_open()) {
        queue<Message> messageQueue;
        string projectID, sender, content;
        while (getline(infileMessages, projectID, '|') && getline(infileMessages, sender, '|') && getline(infileMessages, content)) {
            if (projectID == project.id) {
                messageQueue.push({sender, content});
                if (messageQueue.size() > 3) {
                    messageQueue.pop();
                }
            }
        }
        infileMessages.close();

        cout << "Messages:\n";
        while (!messageQueue.empty()) {
            Message message = messageQueue.front();
            messageQueue.pop();
            cout << message.sender << ": " << message.content << "\n";
        }
    }
}

void addTask(Project& project) {
    Task task;
    cout << "Enter Task Description: ";
    cin.ignore();
    getline(cin, task.description);
    task.isComplete = false;
    task.userID = "";

    ofstream outfile("dataTask.txt", ios::app);
    if (outfile.is_open()) {
        outfile << project.id << " " << task.description << "|" << task.userID << "|" << task.isComplete << "\n";
        outfile.close();
    }

    cout << "Task added successfully.\n";
}

void analyzeTeam(const Project& project) {
    int completedTasks = 0;
    int totalTasks = 0;
    ifstream infileTasks("dataTask.txt");
    if (infileTasks.is_open()) {
        string projectID, description, userID;
        bool isComplete;
        while (infileTasks >> projectID >> ws && getline(infileTasks, description, '|') && infileTasks >> userID >> isComplete) {
            if (projectID == project.id) {
                totalTasks++;
                if (isComplete) {
                    completedTasks++;
                }
            }
        }
        infileTasks.close();
    }

    double completionRate = (totalTasks == 0) ? 0.0 : static_cast<double>(completedTasks) / totalTasks * 100;
    cout << "Team Performance Analysis:\n";
    cout << "Total Tasks: " << totalTasks << "\n";
    cout << "Completed Tasks: " << completedTasks << "\n";
    cout << "Completion Rate: " << completionRate << "%\n";
}

void markTaskComplete(Project& project) {
    vector<Task> tasks;
    ifstream infileTasks("dataTask.txt");
    if (infileTasks.is_open()) {
        string projectID, description, userID;
        bool isComplete;
        while (infileTasks >> projectID >> ws && getline(infileTasks, description, '|') && infileTasks >> userID >> isComplete) {
            if (projectID == project.id) {
                tasks.push_back({description, isComplete, userID});
            }
        }
        infileTasks.close();
    }

    cout << "List of Tasks:\n";
    for (size_t i = 0; i < tasks.size(); ++i) {
        cout << i + 1 << ". " << tasks[i].description << " [" << (tasks[i].isComplete ? "Complete" : "Incomplete") << "]\n";
    }

    cout << "Enter Task Number to Mark Complete: ";
    int taskNumber;
    cin >> taskNumber;
    if (taskNumber >= 1 && taskNumber <= tasks.size()) {
        tasks[taskNumber - 1].isComplete = true;
        ofstream outfile("dataTask.txt");
        if (outfile.is_open()) {
            for (const auto& task : tasks) {
                outfile << project.id << " " << task.description << "|" << task.userID << "|" << task.isComplete << "\n";
            }
            outfile.close();
        }
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

    ofstream outfile("datamessage.txt", ios::app);
    if (outfile.is_open()) {
        outfile << project.id << "|" << message.sender << "|" << message.content << "\n";
        outfile.close();
    }

    cout << "Message sent successfully.\n";
}

void viewMessages(const Project& project) {
    cout << "Messages:\n";
    ifstream infile("datamessage.txt");
    if (infile.is_open()) {
        string projectID, sender, content;
        while (getline(infile, projectID, '|') && getline(infile, sender, '|') && getline(infile, content)) {
            if (projectID == project.id) {
                cout << sender << ": " << content << "\n";
            }
        }
        infile.close();
    }
}

void collaborate(Project& project, const string& username) {
    int choice;
    do {
        cout << "-------COLLABORATION MENU--------\n";
        cout << "1. Send Message\n";
        cout << "2. View Messages\n";
        cout << "3. Exit\n";
        cout << "---------------------------------\n";
        cout << "Input: ";
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
    unordered_map<string, int> userTaskCount; // To store the count of tasks completed by each user
    unordered_map<string, string> userIdToUsername; // To store the mapping of user ID to username
    int totalTasks = 0; // To store the total number of tasks

    // Load all tasks and count the completed tasks for each user
    ifstream infile("dataTask.txt");
    if (infile.is_open()) {
        string projectID, description, userID;
        bool isComplete;
        while (infile >> projectID >> ws && getline(infile, description, '|') && infile >> userID >> isComplete) {
            if (projectID == project.id) {
                totalTasks++;
                if (isComplete) {
                    userTaskCount[userID]++;
                }
            }
        }
        infile.close();
    }

    // Load all users to get the username for each user ID
    ifstream infileUsers("datausers.txt");
    if (infileUsers.is_open()) {
        User user;
        while (infileUsers >> user.username >> user.passwordHash >> user.id >> user.isAdmin) {
            userIdToUsername[user.id] = user.username;
        }
        infileUsers.close();
    }

    // Output the performance analysis
    cout << "Individual Performance Analysis:\n";
    cout << "Total team task = " << totalTasks << "\n";

    for (const auto& pair : userTaskCount) {
        const string& userID = pair.first;
        int completedTasks = pair.second;
        double completionPercentage = (static_cast<double>(completedTasks) / totalTasks) * 100.0;
        cout << "- [" << userID << "] " << userIdToUsername[userID] << " : " << completedTasks
             << " | " << completionPercentage << "%\n";
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
            cout << "4. Hapus Proyek\n";
            cout << "5. Keluar Proyek\n";
            cout << "----------------------------" << endl;
            cout << "Input: ";
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
                    deleteProject();
                    break;
                case 5:
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

void menuAdmin() {
    while (true) {
        cout << "-------CONTROL PANEL--------" << endl;
        cout << "1. Tambah Proyek\n";
        cout << "2. Kontrol Proyek\n";
        cout << "3. Hapus Proyek\n";
        cout << "4. Logout\n";
        cout << "----------------------------" << endl;
        cout << "Input: ";
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
                deleteProject();
                break;
            case 4:
                return;
        }
    }
}

void menuNonAdmin(const User& user) {
    auto it = find_if(projects.begin(), projects.end(), [&user](const Project& project) {
        return project.id == user.id;
    });
    if (it == projects.end()) {
        cout << "No project found with matching ID.\n";
        users.erase(user.username);
        saveUsers();
        return;
    }
    cout << "\n" << "Project Menu for: [" << it->id << "] " << it->name << "\n";
    while (true) {
        cout << "-------PROJECT MENU--------" << endl;
        cout << "1. Lihat Detail Proyek\n";
        cout << "2. List Tugas\n";
        cout << "3. Kolaborasi\n";
        cout << "4. Nilai Kinerja\n";
        cout << "5. Keluar\n";
        cout << "---------------------------" << endl;
        cout << "Input: ";
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
                collaborate(*it, user.username);
                break;
            case 4:
                analyzeOthers(*it);
                break;
            case 5:
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
                        menuAdmin();
                    } else {
                        menuNonAdmin(loggedInUser);
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

void initializeAdmin() {
    User admin;
    admin.username = "admin";
    admin.passwordHash = hashPassword("admin");
    admin.id = "000";
    admin.isAdmin = true;
    if (users.find(admin.username) == users.end()) {
        users[admin.username] = admin;
        saveUsers();
    }
}

int main() {
    loadProjects();
    loadUsers();
    initializeAdmin();

    mainMenu();
    return 0;
}
