#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <limits>

using namespace std;

struct Barang {
    string kode;
    string nama;
    int jumlah;
    float harga;
};

// Simpan data ke file
void simpan_data(const vector<Barang>& barangList, const string& namaFile = "data_barang.txt") {
    ofstream file(namaFile);
    for (const auto& barang : barangList) {
        file << barang.kode << "," << barang.nama << "," << barang.jumlah << "," << barang.harga << endl;
    }
    file.close();
}

// Baca data dari file
vector<Barang> baca_data(const string& namaFile = "data_barang.txt") {
    vector<Barang> barangList;
    ifstream file(namaFile);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string kode, nama, jumlahStr, hargaStr;
        getline(ss, kode, ',');
        getline(ss, nama, ',');
        getline(ss, jumlahStr, ',');
        getline(ss, hargaStr, ',');

        if (kode.empty() || nama.empty() || jumlahStr.empty() || hargaStr.empty()) continue;

        try {
            Barang barang;
            barang.kode = kode;
            barang.nama = nama;
            barang.jumlah = stoi(jumlahStr);
            barang.harga = stof(hargaStr);
            barangList.push_back(barang);
        } catch (const exception& e) {
            cerr << "Baris rusak dilewati: " << line << endl;
        }
    }
    file.close();
    return barangList;
}

// Cari barang berdasarkan kode
Barang* cari_barang(vector<Barang>& barangList, const string& kodeBarang) {
    for (auto& barang : barangList) {
        if (barang.kode == kodeBarang) {
            return &barang;
        }
    }
    return nullptr;
}

// Edit barang
bool edit_barang(vector<Barang>& barangList, const string& kodeBarang, int jumlahBaru, float hargaBaru) {
    Barang* barang = cari_barang(barangList, kodeBarang);
    if (barang) {
        barang->jumlah = jumlahBaru;
        barang->harga = hargaBaru;
        return true;
    }
    return false;
}

// Hapus barang
bool hapus_barang(vector<Barang>& barangList, const string& kodeBarang) {
    auto it = remove_if(barangList.begin(), barangList.end(),
                        [&](const Barang& b) { return b.kode == kodeBarang; });
    if (it != barangList.end()) {
        barangList.erase(it, barangList.end());
        return true;
    }
    return false;
}

// Tampilkan semua barang
void tampilkan_barang(const vector<Barang>& barangList) {
    if (barangList.empty()) {
        cout << "\n[!] Tidak ada barang dalam daftar.\n";
        return;
    }

    cout << "\n--- Daftar Barang ---\n";
    for (const auto& barang : barangList) {
        cout << "Kode: " << barang.kode
             << " | Nama: " << barang.nama
             << " | Jumlah: " << barang.jumlah
             << " | Harga: " << barang.harga << endl;
    }
}

// Fungsi validasi input angka
void clear_input() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    vector<Barang> daftarBarang = baca_data();
    int pilihan;

    do {
        cout << "\n=== Menu Manajemen Barang ===\n";
        cout << "1. Tambah Barang\n";
        cout << "2. Lihat Semua Barang\n";
        cout << "3. Cari Barang\n";
        cout << "4. Edit Barang\n";
        cout << "5. Hapus Barang\n";
        cout << "6. Simpan dan Keluar\n";
        cout << "Pilihan: ";
        cin >> pilihan;
        clear_input();

        if (pilihan == 1) {
            Barang barang;
            cout << "Masukkan kode barang: ";
            getline(cin, barang.kode);
            cout << "Masukkan nama barang: ";
            getline(cin, barang.nama);
            cout << "Masukkan jumlah: ";
            while (!(cin >> barang.jumlah)) {
                cout << "Input tidak valid. Masukkan angka jumlah: ";
                clear_input();
            }
            cout << "Masukkan harga: ";
            while (!(cin >> barang.harga)) {
                cout << "Input tidak valid. Masukkan angka harga: ";
                clear_input();
            }
            clear_input();
            daftarBarang.push_back(barang);
            cout << "Barang berhasil ditambahkan!\n";

        } else if (pilihan == 2) {
            tampilkan_barang(daftarBarang);

        } else if (pilihan == 3) {
            string kode;
            cout << "Masukkan kode barang yang dicari: ";
            getline(cin, kode);
            Barang* hasil = cari_barang(daftarBarang, kode);
            if (hasil) {
                cout << "Ditemukan: " << hasil->nama << " | Jumlah: " << hasil->jumlah << " | Harga: " << hasil->harga << endl;
            } else {
                cout << "Barang tidak ditemukan.\n";
            }

        } else if (pilihan == 4) {
            string kode;
            int jumlah;
            float harga;
            cout << "Masukkan kode barang yang akan diedit: ";
            getline(cin, kode);
            cout << "Masukkan jumlah baru: ";
            while (!(cin >> jumlah)) {
                cout << "Input tidak valid. Masukkan angka jumlah: ";
                clear_input();
            }
            cout << "Masukkan harga baru: ";
            while (!(cin >> harga)) {
                cout << "Input tidak valid. Masukkan angka harga: ";
                clear_input();
            }
            clear_input();
            if (edit_barang(daftarBarang, kode, jumlah, harga)) {
                cout << "Barang berhasil diedit.\n";
            } else {
                cout << "Barang tidak ditemukan.\n";
            }

        } else if (pilihan == 5) {
            string kode;
            cout << "Masukkan kode barang yang akan dihapus: ";
            getline(cin, kode);
            if (hapus_barang(daftarBarang, kode)) {
                cout << "Barang berhasil dihapus.\n";
            } else {
                cout << "Barang tidak ditemukan.\n";
            }

        } else if (pilihan == 6) {
            simpan_data(daftarBarang);
            cout << "Data berhasil disimpan. Keluar...\n";
        } else {
            cout << "Pilihan tidak valid.\n";
        }

    } while (pilihan != 6);

    return 0;
}
