#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>

using namespace std;

struct Task {
    string deskripsi;
    bool completed;
    bool delayed;

    Task(const string& desc) : deskripsi(desc), completed(false), delayed(false) {}
};

struct TeamMember {
    string nama;
    vector<Task> daftarTugas;

    TeamMember(const string& name) : nama(name) {}
};

struct Communication {
    string message;
    string sender;
    string receiver;

    Communication(const string& msg, const string& sdr, const string& rcv)
        : message(msg), sender(sdr), receiver(rcv) {}
};

struct Collaboration {
    string task;
    vector<string> collaborators;

    Collaboration(const string& tsk, const vector<string>& collabs)
        : task(tsk), collaborators(collabs) {}
};

void analyzeTeam(const vector<TeamMember>& team) {
    int totalTasks = 0;
    int completedTasks = 0;
    int delayedTasks = 0;

    for (const auto& member : team) {
        for (const auto& task : member.daftarTugas) {
            totalTasks++;
            if (task.completed) {
                completedTasks++;
                if (task.delayed) {
                    delayedTasks++;
                }
            }
        }
    }

    double completionRate = (totalTasks > 0) ? (static_cast<double>(completedTasks) / totalTasks) * 100 : 0.0;
    double delayRate = (completedTasks > 0) ? (static_cast<double>(delayedTasks) / completedTasks) * 100 : 0.0;

    cout << "Kinerja Tim:\n";
    cout << "Jumlah Tugas: " << totalTasks << endl;
    cout << "Tugas Selesai: " << completedTasks << endl;
    cout << "Tugas Terlambat: " << delayedTasks << endl;
    cout << "Persentase Penyelesaian: " << fixed << setprecision(2) << completionRate << "%" << endl;
    cout << "Persentase Keterlambatan: " << fixed << setprecision(2) << delayRate << "%" << endl;
}

void analyzeIndividual(const TeamMember& member) {
    int totalTasks = member.daftarTugas.size();
    int completedTasks = 0;
    int delayedTasks = 0;

    for (const auto& task : member.daftarTugas) {
        if (task.completed) {
            completedTasks++;
            if (task.delayed) {
                delayedTasks++;
            }
        }
    }

    double completionRate = (totalTasks > 0) ? (static_cast<double>(completedTasks) / totalTasks) * 100 : 0.0;
    double delayRate = (completedTasks > 0) ? (static_cast<double>(delayedTasks) / completedTasks) * 100 : 0.0;

    cout << "Kinerja Individu (" << member.nama << "):\n";
    cout << "Jumlah Tugas: " << totalTasks << endl;
    cout << "Tugas Selesai: " << completedTasks << endl;
    cout << "Tugas Terlambat: " << delayedTasks << endl;
    cout << "Persentase Penyelesaian: " << fixed << setprecision(2) << completionRate << "%" << endl;
    cout << "Persentase Keterlambatan: " << fixed << setprecision(2) << delayRate << "%" << endl;
}

void sendCommunication(const Communication& comm) {
    cout << "Pesan dari " << comm.sender << " kepada " << comm.receiver << ":\n";
    cout << comm.message << endl;
}

void startCollaboration(const Collaboration& collab) {
    cout << "Kolaborasi untuk tugas: " << collab.task << endl;
    cout << "Anggota tim yang terlibat:\n";
    for (const auto& collaborator : collab.collaborators) {
        cout << "- " << collaborator << endl;
    }
}

void displayMenu() {
    cout << "Menu:\n";
    cout << "1. Analisis Kinerja Tim\n";
    cout << "2. Analisis Kinerja Individu\n";
    cout << "3. Kirim Pesan\n";
    cout << "4. Mulai Kolaborasi\n";
    cout << "5. Keluar\n";
    cout << "Pilihan: ";
}

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
    string name;
    cout << "Masukkan nama Anda: ";
    getline(cin, name);

    vector<TeamMember> team {
        {"John"},
        {"Alice"},
        {"Bob"}
    };

    team[0].daftarTugas.push_back(Task("Buat laporan progres"));
    team[0].daftarTugas.push_back(Task("Persiapkan presentasi"));
    team[1].daftarTugas.push_back(Task("Kumpulkan data-data terbaru"));
    team[2].daftarTugas.push_back(Task("Review kode"));

    int choice;
    do {
        displayMenu();
        string choiceStr;
        getline(cin, choiceStr);
        try {
            choice = stoi(choiceStr);
        } catch (...) {
            choice = 0;
        }

        switch (choice) {
            case 1:
                analyzeTeam(team);
                break;
            case 2:
                for (const auto& member : team) {
                    analyzeIndividual(member);
                    cout << endl;
                }
                break;
            case 3: {
                string receiver, message;
                cout << "Masukkan nama penerima: ";
                getline(cin, receiver);
                cout << "Masukkan pesan: ";
                getline(cin, message);
                Communication comm(message, name, receiver);
                sendCommunication(comm);
                break;
            }
            case 4: {
                string task;
                cout << "Masukkan deskripsi tugas: ";
                getline(cin, task);
                vector<string> collaborators;
                string collaborator;
                cout << "Masukkan nama anggota tim yang akan berkolaborasi (akhiri dengan 'selesai'): ";
                while (true) {
                    getline(cin, collaborator);
                    if (collaborator == "selesai") {
                        break;
                    }
                    collaborators.push_back(collaborator);
                }
                Collaboration collab(task, collaborators);
                startCollaboration(collab);
                break;
            }
            case 5:
                cout << "Terima kasih!\n";
                break;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi.\n";
                break;
        }
    } while (choice != 5);

    return 0;
}
