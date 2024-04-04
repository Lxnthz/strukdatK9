#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

struct Task {
    string deskripsi;
    bool completed;
    bool delayed;

    // Constructor
    Task(const string& desc) : deskripsi(desc), completed(false), delayed(false) {}
};

struct TeamMember {
    string nama;
    vector<Task> daftarTugas;

    // Constructor
    TeamMember(const string& name) : nama(name) {}
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

int main() {
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
        cout << "\nMenu:\n";
        cout << "1. Analisis Kinerja Tim\n";
        cout << "2. Analisis Kinerja Individu\n";
        cout << "3. Keluar\n";
        cout << "Pilihan: ";
        cin >> choice;
        
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
            case 3:
                cout << "Terima kasih!\n";
                break;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi.\n";
                break;
        }
    } while (choice != 3);

    return 0;
}
