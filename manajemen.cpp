#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Task {
    string deskripsi;
    bool completed;

    // Constructor
    Task(const string& desc) : deskripsi(desc), completed(false) {}
};

struct TeamMember {
    string nama;
    vector<Task> daftarTugas;

    // Constructor
    TeamMember(const string& name) : nama(name) {}
};

void assignTaskToMember(vector<TeamMember>& team, const string& memberName, const string& taskDescription) {
    for (auto& member : team) {
        if (member.nama == memberName) {
            member.daftarTugas.push_back(Task(taskDescription));
            cout << "Tugas \"" << taskDescription << "\" berhasil ditugaskan kepada " << memberName << ".\n";
            return;
        }
    }
    cout << "Anggota tim dengan nama \"" << memberName << "\" tidak ditemukan.\n";
}

void displayAssignedTasks(const vector<TeamMember>& team) {
    cout << "Daftar Tugas yang Telah Ditugaskan:\n";
    for (const auto& member : team) {
        cout << "Anggota Tim: " << member.nama << endl;
        if (member.daftarTugas.empty()) {
            cout << "Belum ada tugas yang ditugaskan.\n";
        } else {
            cout << "Daftar Tugas:\n";
            for (const auto& task : member.daftarTugas) {
                cout << "- " << task.deskripsi;
                if (task.completed) {
                    cout << " (Selesai)";
                }
                cout << endl;
            }
        }
        cout << endl;
    }
}

void markTaskAsCompleted(vector<TeamMember>& team, const string& memberName, const string& taskDescription) {
    for (auto& member : team) {
        if (member.nama == memberName) {
            for (auto& task : member.daftarTugas) {
                if (task.deskripsi == taskDescription) {
                    task.completed = true;
                    cout << "Tugas \"" << taskDescription << "\" telah ditandai sebagai selesai.\n";
                    return;
                }
            }
            cout << "Tugas dengan deskripsi \"" << taskDescription << "\" tidak ditemukan untuk anggota tim " << memberName << ".\n";
            return;
        }
    }
    cout << "Anggota tim dengan nama \"" << memberName << "\" tidak ditemukan.\n";
}

int main() {
    vector<TeamMember> team {
        {"Mahfud"},
        {"Mega"},
        {"Joko"}
    };

    int choice;
    string memberName, taskDescription;
    do {
        cout << "\nMenu:\n";
        cout << "1. Menugaskan tugas kepada anggota tim\n";
        cout << "2. Melihat daftar tugas yang telah ditugaskan\n";
        cout << "3. Tandai tugas sebagai selesai\n";
        cout << "4. Keluar\n";
        cout << "Pilihan: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                cout << "Masukkan nama anggota tim: ";
                cin >> memberName;
                cout << "Masukkan deskripsi tugas: ";
                cin.ignore();
                getline(cin, taskDescription);
                assignTaskToMember(team, memberName, taskDescription);
                break;
            case 2:
                displayAssignedTasks(team);
                break;
            case 3:
                cout << "Masukkan nama anggota tim: ";
                cin >> memberName;
                cout << "Masukkan deskripsi tugas: ";
                cin.ignore();
                getline(cin, taskDescription);
                markTaskAsCompleted(team, memberName, taskDescription);
                break;
            case 4:
                cout << "Terima kasih!\n";
                break;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi.\n";
                break;
        }
    } while (choice != 4);

    return 0;
}
