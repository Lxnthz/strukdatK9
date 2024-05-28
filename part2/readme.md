USED DATA STRUCTURES
Hashing
Vector

USER JOURNEY ADMIN
1. Ketika sistem dijalankan akan muncul menu yang berisikan:
- Login
- Register
- Exit

2. Anggap yang pertama kali masuk adalah Admin, maka admin masuk kedalam sistem dan muncul menu:
- Tambah Proyek: createProject()
- Kontrol Proyek: controlProject()
- Logout: logOut()

2a.1 Misalkan admin memilih opsi Tambah Proyek, maka admin diminta untuk memasukan detail proyek sebagai berikut:
- ID Proyek: <string> max 3 alphanum char
- Nama proyek: <string>
- Deskripsi proyek: <string>
- Start: <dateformat>
- Deadline: <dateformat>
setelah itu data akan disimpan sistem didalam "projects.txt"

2a.1 Misalkan admin memilih opsi Kontrol Project, maka sistem akan mendisplay semua project yang tersimpan dalam database dan menampilkan menu baru sebagai berikut:
Daftar Seluruh Proyek:
1. [ID] Proyek A
2. [ID] Proyek B
3. [ID] Proyek C
4. [ID] Proyek D
------------------------------------------
Kontrol Proyek: [Admin menginput ID dari proyek] \n

3. Setelah memilih proyek yang ingin dikontrol, sistem menampilkan menu baru:
| Project Menu for: [ID] (nama proyek) |
- Lihat Detail Proyek: showDetails()
- Tambahkan Tugas: addTask()
- Nilai Kinerja Tim: analyzeTeam()
- Keluar Proyek: exit()

USER JOURNEY NON-ADMIN
1. Ketika sistem dijalankan akan muncul menu yang berisikan:
- Login
- Register
- Exit

2. Apabila user belum terdaftar maka user memilih menu Register dan memasukan data yang diminta:
- Username
- Password
- ID
data user disimpan dalam "datausers.txt"

3. Setelah itu user dikembalikan ke menu sebelumnya dan bisa memilih Login untuk masuk kesistem

4. Sistem akan membaca ID dari akun user dan mencocokannya dengan ID Proyek yang ada di database. Apabila tidak ditemukan, sistem akan memberikan pesan error dan akun pengguna akan dihapus dari database. Apabila ditemukan Id yang sama maka user akan masuk ke sistem dan otomatis akan mengkontrol proyek dengan ID yang sama dengan user ID akan diberikan menu sebagai berikut:
| [ID] (nama proyek) | 
- Lihat Detail Proyek: showDetails()
- Update Status Tugas: markTaskComplete()
- Komunikasi: message()
- Kolaborasi: collaborate()
- Nilai Kinerja: analyzeOthers()
- Keluar: exit()

FUNCTION BUILDING PLAN
1) Fungsi Login dan Register akun
- Lingkup pengguna sistem adalah Admin dan Non-Admin
- hashPassword(): Password dari pengguna harus di Hashing
- Akun non-admin akan memiliki sebuah ID yang sama dengan ID project yang akun tersebut bertugas, dengan kata lain, akun non-admin hanya bisa bekerja didalam project dengan ID yang sama dengan ID pada akun non-admin.
- Menu yang dimiliki Admin dan Non-admin akan berbeda
- Admin memiliki akses kontrol ke seluruh project dalam sistem
- Semua data akun terdaftar tersimpan dalam file txt "datausers.txt" sebagai database sistem
- inisialisasi akun admin dari awal, sehingga admin tidak perlu register.

2) Fungsi untuk pencatatan dan pemantauan progress proyek. [ADMIN ONLY]
a) Pencatatan:
- Sistem memiliki sistem pencatatan project beserta detail project.
- createProject(), menambah project baru dengan mengisi detail project.
- Detail project berisi id, nama, deskripsi, deadline, daftar tim yang bertanggung jawab, daftar tugas dalam project tersebut.
- controlProject(), akan menampilkan list semua project dan memberikan menu untuk memilih project mana yang ingin dikontrol.
- Daftar project disimpan dalam bentuk vektor
- Data projects disimpan dalam "dataprojects.txt" sebagai database.
b) Pemantauan
- updateProgress(), digunakan untuk mengupdate progress baru.


3) Fungsi untuk manajemen tugas dan penugasan tim.
- assignTask(), untuk memberikan tugas kepada sebuah tim didalam proyek. 
- markTaskComplete(), untuk memberikan mark complete pada list tugas ketika tugas diselesaikan.
- displayTask(), untuk melihat list tugas yang perlu dikerjakan.

4) Fungsi untuk komunikasi dan kolaborasi antar anggota tim.
- collaborate(), untuk melakukan kolaborasi antar anggota tim 
- communicate() {sendMessage(), incomingMessage()}, untuk komunikasi antar tim

5) Fungsi untuk analisis kinerja tim dan individu.
- analyzeOthers() - NonAdmin
- analyzeTeam() - Admin