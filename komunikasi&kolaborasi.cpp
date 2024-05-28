#include <iostream>
#include <vector>
#include <string>

using namespace std;

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
    cout << "1. Kirim Pesan\n";
    cout << "2. Mulai Kolaborasi\n";
    cout << "3. Keluar\n";
    cout << "Pilihan: ";
}

int main() {
    string name;
    cout << "Masukkan nama Anda: ";
    getline(cin, name);

    string choiceStr;
    int choice;
    do {
        displayMenu();
        getline(cin, choiceStr);
        try {
            choice = stoi(choiceStr);
        } catch (...) {
            choice = 0;
        }

        switch (choice) {
            case 1: {
                string receiver, message;
                cout << "Masukkan nama penerima: ";
                getline(cin, receiver);
                cout << "Masukkan pesan: ";
                getline(cin, message);
                Communication comm(message, name, receiver);
                sendCommunication(comm);
                break;
            }
            case 2: {
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