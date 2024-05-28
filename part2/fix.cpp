#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <unordered_map>

using namespace std;

// Kelas untuk manajemen pengguna, termasuk login, logout, dan penyimpanan data akun dalam file
class UserManagement {
private:
    unordered_map<string, string> users; // username -> hashed password
    string currentUser;
    string userFile; // Nama file untuk menyimpan data pengguna

    string hashPassword(const string& password) {
        hash<string> hasher;
        return to_string(hasher(password));
    }

public:
    UserManagement(const string& filename) : userFile(filename) {
        loadUsersFromFile();
    }

    void loadUsersFromFile() {
        ifstream inFile(userFile);
        if (!inFile) {
            cerr << "Error: Tidak dapat membuka file " << userFile << " untuk membaca.\n";
            return;
        }

        string username, hashedPassword;
        while (inFile >> username >> hashedPassword) {
            users[username] = hashedPassword;
        }

        inFile.close();
    }

    void saveUsersToFile() {
        ofstream outFile(userFile);
        if (!outFile) {
            cerr << "Error: Tidak dapat membuka file " << userFile << " untuk menulis.\n";
            return;
        }

        for (const auto& user : users) {
            outFile << user.first << " " << user.second << "\n";
        }

        outFile.close();
    }

    bool signUp(const string& username, const string& password) {
        if (users.find(username) != users.end()) {
            cout << "Username sudah ada!\n";
            return false;
        }
        users[username] = hashPassword(password);
        saveUsersToFile(); // Simpan perubahan ke file
        cout << "Akun berhasil dibuat!\n";
        return true;
    }

    bool login(const string& username, const string& password) {
        if (users.find(username) == users.end()) {
            cout << "Username tidak ditemukan!\n";
            return false;
        }
        if (users[username] != hashPassword(password)) {
            cout << "Password salah!\n";
            return false;
        }
        currentUser = username;
        cout << "Login berhasil!\n";
        return true;
    }

    void logout() {
        currentUser.clear();
        cout << "Logout berhasil!\n";
    }

    bool isLoggedIn() const {
        return !currentUser.empty();
    }

    string getCurrentUser() const {
        return currentUser;
    }
};

// Struktur data tugas dalam proyek
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

// Struktur data proyek
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
    Project(const string& name, const string& desc, const vector<string>& members, const string& deadline)
        : nama(name), deskripsi(desc), daftarAnggota(members), deadline(deadline), completed(false), progress("0% selesai") {}

    void displayInfo() const {
        cout << "----------------------------------------------------\n";
        cout << "Nama Proyek: " << nama << "\n";
        cout << "Deskripsi: " << deskripsi << "\n";
        cout << "Daftar Anggota Tim:\n";
        for (const auto& anggota : daftarAnggota) {
            cout << " - " << anggota << "\n";
        }
        cout << "Deadline: " << deadline << "\n";
        cout << "Progress: " << progress << "\n";

        cout << "Daftar Tugas:\n";
        for (const auto& tugas : daftarTugas) {
            cout << "- " << tugas.deskripsi << " (" << (tugas.completed ? "Selesai" : "Belum selesai") << ")\n";
        }
        cout << "----------------------------------------------------\n";
    }

    void ubahDeskripsi(const string& newDeskripsi) {
        deskripsi = newDeskripsi;
        cout << "Deskripsi proyek berhasil diubah menjadi: " << deskripsi << "\n";
    }

    void ubahProgress(const string& newProgress) {
        progress = newProgress;
        cout << "Progress berhasil diubah menjadi: " << progress << "\n";
    }

    void ubahDeadline(const string& newDeadline) {
        deadline = newDeadline;
        cout << "Deadline proyek berhasil diubah menjadi: " << deadline << "\n";
    }

    void tambahAnggota(const string& newAnggota) {
        daftarAnggota.push_back(newAnggota);
        cout << "Anggota baru (" << newAnggota << ") berhasil ditambahkan ke proyek.\n";
    }

    void tambahTugas(const Task& newTask) {
        daftarTugas.push_back(newTask);
        cout << "Tugas baru \"" << newTask.deskripsi << "\" berhasil ditambahkan.\n";
    }

    string serialize() const {
        stringstream ss;
        ss << nama << "\n" << deskripsi << "\n" << deadline << "\n" << progress << "\n";

        ss << "Anggota:\n";
        for (const auto& anggota : daftarAnggota) {
            ss << anggota << "\n";
        }

        ss << "Tugas:\n";
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
        getline(ss, line); // "Anggota:"
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

// Fungsi untuk menyimpan proyek ke file
void saveProjectsToFile(const vector<Project>& projects, const string& filename) {
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Error: Tidak dapat membuka file " << filename << " untuk menulis.\n";
        return;
    }

    for (const auto& project : projects) {
        outFile << project.serialize() << "---\n"; // Penanda akhir proyek
    }

    outFile.close();
}

// Fungsi untuk memuat proyek dari file
vector<Project> loadProjectsFromFile(const string& filename) {
    vector<Project> projects;
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "Error: Tidak dapat membuka file " << filename << " untuk membaca.\n";
        return projects;
    }

    stringstream ss;
    string line;
    while (getline(inFile, line)) {
        if (line == "---") { 
            projects.push_back(Project::deserialize(ss.str()));
            ss.str(""); // Membersihkan stringstream
        } else {
            ss << line << "\n"; // Menambah baris ke stringstream
        }
    }

    inFile.close();
    return projects;
}

// Fungsi untuk menambah proyek baru
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

// Fungsi untuk menampilkan semua proyek
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

// Fungsi untuk mengedit proyek
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
            cout << "Masukkan Progress Baru: ";
            string newProgress;
            proyek.ubahProgress(newProgress);
            break;
        }
        default:
            cout << "Pilihan tidak valid.\n";
            break;
    }
}

// Fungsi untuk menampilkan menu utama bagi pengguna yang sudah login
void showMainMenu() {
    cout << "\nMenu:\n";
    cout << "1. Tambah Proyek Baru\n";
    cout << "2. Tampilkan Semua Proyek\n";
    cout << "3. Edit Informasi Proyek\n";
    cout << "4. Logout\n";
    cout << "5. Keluar\n";
    cout << "Pilihan: ";
}

// Fungsi untuk menampilkan menu login/sign-up bagi pengguna yang belum login
void showLoginMenu() {
    cout << "\nPilih opsi:\n";
    cout << "1. Login\n";
    cout << "2. Daftar akun baru\n";
    cout << "3. Keluar\n";
    cout << "Pilihan: ";
}

int main() {
    vector<Project> projects;
    string projectFilename = "projects.txt"; // Nama file untuk proyek
    string userFilename = "users.txt"; // Nama file untuk pengguna

    projects = loadProjectsFromFile(projectFilename); // Memuat proyek dari file
    UserManagement userMgmt(userFilename); // Inisialisasi UserManagement dengan file pengguna

    int pilihan;
    do {
        if (userMgmt.isLoggedIn()) {
            showMainMenu(); // Tampilkan menu utama jika pengguna sudah login
        } else {
            showLoginMenu(); // Tampilkan menu login/sign-up jika belum login
        }

        cin >> pilihan;
        cin.ignore(); // Membersihkan buffer newline

        if (!userMgmt.isLoggedIn()) { // Jika belum login
            switch (pilihan) {
                case 1: {
                    string username, password;
                    cout << "Masukkan username: ";
                    getline(cin, username);
                    cout << "Masukkan password: ";
                    getline(cin, password);
                    userMgmt.login(username, password);
                    break;
                }
                case 2: {
                    string username, password;
                    cout << "Masukkan username baru: ";
                    getline(cin, username);
                    cout << "Masukkan password baru: ";
                    getline(cin, password);
                    userMgmt.signUp(username, password);
                    break;
                }
                case 3:
                    cout << "Terima kasih!\n";
                    break;
                default:
                    cout << "Pilihan tidak valid.\n";
                    break;
            }
        } else { // Jika sudah login
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
                    userMgmt.logout(); // Logout
                    break;
                case 5:
                    cout << "Terima kasih!\n";
                    break;
                default:
                    cout << "Pilihan tidak valid.\n";
                    break;
            }
        }
    } while (pilihan != 3 && pilihan != 5);

    saveProjectsToFile(projects, projectFilename); // Simpan proyek sebelum keluar

    return 0;
}
