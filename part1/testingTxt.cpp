#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream> 

using namespace std;

struct Task {
    string deskripsi;
    bool completed;

    Task(const string& desc) : deskripsi(desc), completed(false) {}

    Task(const string& desc, bool comp) : deskripsi(desc), completed(comp) {}

    string serialize() const {
        return deskripsi + "," + (completed ? "true" : "false");
    }

    static Task deserialize(const string& data) {
        stringstream ss(data);
        string deskripsi;
        string completedStr;
        getline(ss, deskripsi, ',');
        getline(ss, completedStr, ',');
        bool completed = (completedStr == "true");
        return Task(deskripsi, completed);
    }
};


struct Project {
private:
    bool completed;
    string nama;
    string deskripsi;
    string deadline;
    string progress;
    vector<string> daftarAnggota;
    vector<Task> daftarTugas;

public:
    // constructor
    Project(const string& name, const string& desc, const vector<string>& members, const string& deadline) :
        nama(name), deskripsi(desc), daftarAnggota(members), deadline(deadline), completed(false), progress("0% selesai") {}

    void displayInfo() const {
        cout << "----------------------------------------------------" << endl;
        cout << "Nama Proyek: " << nama << endl;
        cout << "Deskripsi: " << deskripsi << endl;
        cout << "Daftar Anggota Tim: " << endl;
        for (const auto& anggota : daftarAnggota) {
            cout << " - " << anggota << endl;
        }
        cout << "Deadline: " << deadline << endl;
        cout << "Progress: " << progress << endl;

        cout << "Daftar Tugas:\n";
        for (const auto& tugas : daftarTugas) {
            cout << "- " << tugas.deskripsi << " (" << (tugas.completed ? "Selesai" : "Belum selesai") << ")\n";
        }
        cout << "----------------------------------------------------" << endl;
    }

    void ubahDeskripsi(const string& newDeskripsi) {
        deskripsi = newDeskripsi;
        cout << "Deskripsi proyek berhasil diubah menjadi: " << deskripsi << endl;
    }

    void ubahProgress(const string& newProgress) {
        progress = newProgress;
        cout << "Progress berhasil diubah menjadi: " << progress << endl;
    }

    void ubahDeadline(const string& newDeadline) {
        deadline = newDeadline;
        cout << "Deadline proyek berhasil diubah menjadi: " << deadline << endl;
    }

    void tambahAnggota(const string& newAnggota) {
        daftarAnggota.push_back(newAnggota);
        cout << "Anggota baru (" << newAnggota << ") berhasil ditambahkan ke dalam proyek.\n";
    }

    void tambahTugas(const Task& newTask) {
        daftarTugas.push_back(newTask);
        cout << "Tugas baru \"" << newTask.deskripsi << "\" berhasil ditambahkan.\n";
    }

    string serialize() const {
        stringstream ss;
        ss << nama << "\n" << deskripsi << "\n" << deadline << "\n" << progress << "\n";
        
        ss << "Anggota:" << "\n";
        for (const auto& anggota : daftarAnggota) {
            ss << anggota << "\n";
        }

        ss << "Tugas:" << "\n";
        for (const auto& tugas : daftarTugas) {
            ss << tugas.serialize() << "\n";
        }

        return ss.str();
    }

    static Project deserialize(const string& data) {
        stringstream ss(data);
        string nama, deskripsi, deadline, progress;

        getline(ss, nama);
        getline(ss, deskripsi);
        getline(ss, deadline);
        getline(ss, progress);

        vector<string> anggota;
        vector<Task> tugas;

        string line;
        getline(ss, line); 
        while (getline(ss, line) && line != "Tugas:") {
            anggota.push_back(line);
        }

        while (getline(ss, line)) {
            tugas.push_back(Task::deserialize(line));
        }

        Project p(nama, deskripsi, anggota, deadline);
        p.progress = progress;
        p.daftarTugas = tugas;

        return p;
    }
};

void saveProjectsToFile(const vector<Project>& projects, const string& filename) {
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Error: Could not open file for writing." << endl;
        return;
    }

    for (const auto& project : projects) {
        outFile << project.serialize() << "---\n";
    }

    outFile.close();
}

vector<Project> loadProjectsFromFile(const string& filename) {
    vector<Project> projects;
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "Error: Could not open file for reading." << endl;
        return projects;
    }

    stringstream ss;
    string line;
    while (getline(inFile, line)) {
        if (line == "---") { 
            projects.push_back(Project::deserialize(ss.str()));
            ss.str(""); 
        } else {
            ss << line << "\n"; 
        }
    }

    inFile.close();
    return projects;
}

void tambahProyek(vector<Project>& projects) {
    string nama, deskripsi, deadline;
    vector<string> anggota;
    
    cout << "Masukkan Nama Proyek: ";
    getline(cin, nama);
    cout << "Masukkan Deskripsi Proyek: ";
    getline(cin, deskripsi);
    cout << "Masukkan Deadline Proyek: ";
    getline(cin, deadline);
    cout << "Masukkan Daftar Anggota Tim (pisahkan dengan koma): ";
    string inputAnggota;
    getline(cin, inputAnggota);
    stringstream ss(inputAnggota);
    string anggotaIndividu;
    while (getline(ss, anggotaIndividu, ',')) {
        anggota.push_back(anggotaIndividu);
    }

    projects.push_back(Project(nama, deskripsi, anggota, deadline));
    cout << "Proyek \"" << nama << "\" telah ditambahkan.\n";
}

void displayAllProjects(const vector<Project>& projects) {
    if (projects.empty()) {
        cout << "Tidak ada proyek yang tersedia.\n";
        return;
    }

    cout << "Daftar Proyek:\n";
    for (const auto& project : projects) {
        project.displayInfo(); 
    }
}

void editProject(vector<Project>& projects) {
    if (projects.empty()) {
        cout << "Tidak ada proyek yang tersedia untuk diedit.\n";
        return;
    }

    cout << "Pilih proyek yang ingin diedit (masukkan nomor proyek): ";
    int nomorProyek;
    cin >> nomorProyek;
    cin.ignore(); 

    if (nomorProyek < 1 || nomorProyek > projects.size()) {
        cout << "Nomor proyek tidak valid.\n";
        return;
    }

    Project& proyek = projects[nomorProyek - 1];

    cout << "\nMenu Edit Proyek:\n";
    cout << "1. Ubah Deskripsi\n";
    cout << "2. Ubah Deadline\n";
    cout << "3. Tambah Anggota Tim\n";
    cout << "4. Ubah Progress\n";
    cout << "Pilihan: ";
    int pilihan;
    cin >> pilihan;
    cin.ignore(); 

    switch (pilihan) {
        case 1: {
            cout << "Masukkan Deskripsi Baru: ";
            string newDeskripsi;
            getline(cin, newDeskripsi);
            proyek.ubahDeskripsi(newDeskripsi);
            break;
        }
        case 2: {
            cout << "Masukkan Deadline Baru: ";
            string newDeadline;
            getline(cin, newDeadline);
            proyek.ubahDeadline(newDeadline);
            break;
        }
        case 3: {
            cout << "Masukkan Nama Anggota Baru: ";
            string newAnggota;
            getline(cin, newAnggota);
            proyek.tambahAnggota(newAnggota);
            break;
        }
        case 4: {
            cout << "Masukan Progress: ";
            string newProgress;
            getline(cin, newProgress);
            proyek.ubahProgress(newProgress);
        }
        default:
            cout << "Pilihan tidak valid.\n";
            break;
    }
}

int main() {
    vector<Project> projects;
    string filename = "projects.txt";

    projects = loadProjectsFromFile(filename);

    int pilihan;
    do {
        cout << "\nMenu:\n";
        cout << "1. Tambah Proyek Baru\n";
        cout << "2. Tampilkan Semua Proyek\n";
        cout << "3. Edit Informasi Proyek\n";
        cout << "4. Keluar\n";
        cout << "Pilihan: ";
        cin >> pilihan;
        cin.ignore(); 

        switch (pilihan) {
            case 1:
                tambahProyek(projects);
                break;
            case 2:
                displayAllProjects(projects);
                break;
            case 3:
                editProject(projects);
                break;
            case 4:
                cout << "Terima kasih!\n";
                break;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi.\n";
                break;
        }
    } while (pilihan != 4);

    saveProjectsToFile(projects, filename);

    return 0;
}
