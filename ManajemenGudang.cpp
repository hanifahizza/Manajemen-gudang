#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>

using namespace std;

// Struktur untuk menyimpan data barang
struct Barang {
    string id;
    string nama;
    int stok;
    double harga;

    Barang() : id(""), nama(""), stok(0), harga(0.0) {}
    Barang(string i, string n, int s, double h) : id(i), nama(n), stok(s), harga(h) {}
};

// Node untuk AVL Tree (terurut berdasarkan nama)
struct AVLNode {
    Barang data;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(Barang b) : data(b), left(nullptr), right(nullptr), height(1) {}
};

// Node untuk Hash Table (chaining)
struct HashNode {
    Barang data;
    HashNode* next;

    HashNode(Barang b) : data(b), next(nullptr) {}
};

// AVL Tree Class
class AVLTree {
private:
    AVLNode* root;

    int getHeight(AVLNode* node) {
        return node ? node->height : 0;
    }

    int getBalance(AVLNode* node) {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }

    AVLNode* rotateRight(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

        return x;
    }

    AVLNode* rotateLeft(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

        return y;
    }

    AVLNode* insert(AVLNode* node, Barang barang) {
        if (!node) return new AVLNode(barang);

        if (barang.nama < node->data.nama)
            node->left = insert(node->left, barang);
        else if (barang.nama > node->data.nama)
            node->right = insert(node->right, barang);
        else
            return node;

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));

        int balance = getBalance(node);

        // Left Left
        if (balance > 1 && barang.nama < node->left->data.nama)
            return rotateRight(node);

        // Right Right
        if (balance < -1 && barang.nama > node->right->data.nama)
            return rotateLeft(node);

        // Left Right
        if (balance > 1 && barang.nama > node->left->data.nama) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // Right Left
        if (balance < -1 && barang.nama < node->right->data.nama) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    AVLNode* minValueNode(AVLNode* node) {
        AVLNode* current = node;
        while (current->left)
            current = current->left;
        return current;
    }

    AVLNode* deleteNode(AVLNode* root, string nama) {
        if (!root) return root;

        if (nama < root->data.nama)
            root->left = deleteNode(root->left, nama);
        else if (nama > root->data.nama)
            root->right = deleteNode(root->right, nama);
        else {
            if (!root->left || !root->right) {
                AVLNode* temp = root->left ? root->left : root->right;
                if (!temp) {
                    temp = root;
                    root = nullptr;
                } else
                    *root = *temp;
                delete temp;
            } else {
                AVLNode* temp = minValueNode(root->right);
                root->data = temp->data;
                root->right = deleteNode(root->right, temp->data.nama);
            }
        }

        if (!root) return root;

        root->height = 1 + max(getHeight(root->left), getHeight(root->right));

        int balance = getBalance(root);

        // Left Left
        if (balance > 1 && getBalance(root->left) >= 0)
            return rotateRight(root);

        // Left Right
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = rotateLeft(root->left);
            return rotateRight(root);
        }

        // Right Right
        if (balance < -1 && getBalance(root->right) <= 0)
            return rotateLeft(root);

        // Right Left
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rotateRight(root->right);
            return rotateLeft(root);
        }

        return root;
    }

    AVLNode* search(AVLNode* node, string nama) {
        if (!node || node->data.nama == nama)
            return node;

        if (nama < node->data.nama)
            return search(node->left, nama);

        return search(node->right, nama);
    }

    void inorder(AVLNode* node) {
        if (node) {
            inorder(node->left);
            cout << left << setw(10) << node->data.id
                 << setw(25) << node->data.nama
                 << setw(10) << node->data.stok
                 << "Rp " << fixed << setprecision(2) << node->data.harga << endl;
            inorder(node->right);
        }
    }

    void lowStock(AVLNode* node, int threshold) {
        if (node) {
            lowStock(node->left, threshold);
            if (node->data.stok < threshold) {
                cout << left << setw(10) << node->data.id
                     << setw(25) << node->data.nama
                     << setw(10) << node->data.stok
                     << "Rp " << fixed << setprecision(2) << node->data.harga << endl;
            }
            lowStock(node->right, threshold);
        }
    }

public:
    AVLTree() : root(nullptr) {}

    void insert(Barang barang) {
        root = insert(root, barang);
    }

    void remove(string nama) {
        root = deleteNode(root, nama);
    }

    Barang* search(string nama) {
        AVLNode* result = search(root, nama);
        return result ? &(result->data) : nullptr;
    }

    void displayAll() {
        if (!root) {
            cout << "Tidak ada barang dalam gudang.\n";
            return;
        }
        cout << left << setw(10) << "ID"
             << setw(25) << "Nama"
             << setw(10) << "Stok"
             << "Harga" << endl;
        cout << string(60, '-') << endl;
        inorder(root);
    }

    void displayLowStock(int threshold) {
        if (!root) {
            cout << "Tidak ada barang dalam gudang.\n";
            return;
        }
        cout << left << setw(10) << "ID"
             << setw(25) << "Nama"
             << setw(10) << "Stok"
             << "Harga" << endl;
        cout << string(60, '-') << endl;
        lowStock(root, threshold);
    }
};

// Hash Table Class
class HashTable {
private:
    static const int TABLE_SIZE = 100;
    HashNode* table[TABLE_SIZE];
    AVLTree* avlTree;

    int hashFunction(string key) {
        int hash = 0;
        for (char c : key)
            hash = (hash * 31 + c) % TABLE_SIZE;
        return hash;
    }

public:
    HashTable(AVLTree* tree) : avlTree(tree) {
        for (int i = 0; i < TABLE_SIZE; i++)
            table[i] = nullptr;
    }

    void insert(Barang barang) {
        int index = hashFunction(barang.id);
        HashNode* newNode = new HashNode(barang);

        if (!table[index]) {
            table[index] = newNode;
        } else {
            HashNode* temp = table[index];
            while (temp->next)
                temp = temp->next;
            temp->next = newNode;
        }

        avlTree->insert(barang);
    }

    Barang* search(string id) {
        int index = hashFunction(id);
        HashNode* temp = table[index];

        while (temp) {
            if (temp->data.id == id)
                return &(temp->data);
            temp = temp->next;
        }
        return nullptr;
    }

    bool update(string id, int newStok, double newHarga) {
        Barang* barang = search(id);
        if (barang) {
            avlTree->remove(barang->nama);
            barang->stok = newStok;
            barang->harga = newHarga;
            avlTree->insert(*barang);
            return true;
        }
        return false;
    }

    bool remove(string id) {
        int index = hashFunction(id);
        HashNode* temp = table[index];
        HashNode* prev = nullptr;

        while (temp) {
            if (temp->data.id == id) {
                if (prev)
                    prev->next = temp->next;
                else
                    table[index] = temp->next;

                avlTree->remove(temp->data.nama);
                delete temp;
                return true;
            }
            prev = temp;
            temp = temp->next;
        }
        return false;
    }
};

// Fungsi utilitas
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void displayMenu() {
    cout << "\n=== SISTEM MANAJEMEN GUDANG ===\n";
    cout << "1. Tambah Barang\n";
    cout << "2. Cari Barang (by ID)\n";
    cout << "3. Cari Barang (by Name)\n";
    cout << "4. Update Barang\n";
    cout << "5. Hapus Barang\n";
    cout << "6. Tampilkan Semua Barang (Terurut)\n";
    cout << "7. Lihat Stok Rendah (< 10)\n";
    cout << "8. Keluar\n";
    cout << "Pilih menu: ";
}

int main() {
    AVLTree avlTree;
    HashTable hashTable(&avlTree);
    int pilihan;

    do {
        displayMenu();
        cin >> pilihan;
        cin.ignore();

        switch (pilihan)GG {
            case 1: {
                string id, nama;
                int stok;
                double harga;

                cout << "\n--- Tambah Barang ---\n";
                cout << "ID Barang: ";
                getline(cin, id);
                cout << "Nama Barang: ";
                getline(cin, nama);
                cout << "Stok: ";
                cin >> stok;
                cout << "Harga: ";
                cin >> harga;

                Barang barang(id, nama, stok, harga);
                hashTable.insert(barang);
                cout << "\nBarang berhasil ditambahkan!\n";
                break;
            }

            case 2: {
                string id;
                cout << "\n--- Cari Barang by ID ---\n";
                cout << "Masukkan ID: ";
                getline(cin, id);

                Barang* result = hashTable.search(id);
                if (result) {
                    cout << "\nBarang ditemukan:\n";
                    cout << "ID    : " << result->id << endl;
                    cout << "Nama  : " << result->nama << endl;
                    cout << "Stok  : " << result->stok << endl;
                    cout << "Harga : Rp " << fixed << setprecision(2) << result->harga << endl;
                } else {
                    cout << "\nBarang tidak ditemukan!\n";
                }
                break;
            }

            case 3: {
                string nama;
                cout << "\n--- Cari Barang by Name ---\n";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Masukkan Nama: ";
                getline(cin, nama);

                Barang* result = avlTree.search(nama);
                if (result) {
                    cout << "\nBarang ditemukan:\n";
                    cout << "ID    : " << result->id << endl;
                    cout << "Nama  : " << result->nama << endl;
                    cout << "Stok  : " << result->stok << endl;
                    cout << "Harga : Rp " << fixed << setprecision(2) << result->harga << endl;
                } else {
                    cout << "\nBarang tidak ditemukan!\n";
                }
                break;
            }

            case 4: {
                string id;
                int newStok;
                double newHarga;

                cout << "\n--- Update Barang ---\n";
                cout << "ID Barang: ";
                getline(cin, id);
                cout << "Stok Baru: ";
                cin >> newStok;
                cout << "Harga Baru: ";
                cin >> newHarga;

                if (hashTable.update(id, newStok, newHarga))
                    cout << "\nBarang berhasil diupdate!\n";
                else
                    cout << "\nBarang tidak ditemukan!\n";
                break;
            }

            case 5: {
                string id;
                cout << "\n--- Hapus Barang ---\n";
                cout << "ID Barang: ";
                getline(cin, id);

                if (hashTable.remove(id))
                    cout << "\nBarang berhasil dihapus!\n";
                else
                    cout << "\nBarang tidak ditemukan!\n";
                break;
            }

            case 6:
                cout << "\n--- Semua Barang (Terurut by Nama) ---\n";
                avlTree.displayAll();
                break;

            case 7:
                cout << "\n--- Barang dengan Stok Rendah (< 10) ---\n";
                avlTree.displayLowStock(10);
                break;

            case 8:
                cout << "\nTerima kasih telah menggunakan sistem ini!\n";
                break;

            default:
                cout << "\nPilihan tidak valid!\n";
        }

        if (pilihan != 8) {
            cout << "\nTekan Enter untuk melanjutkan...";
            cin.ignore();
            cin.get();
        }

    } while (pilihan != 8);

    return 0;
}
