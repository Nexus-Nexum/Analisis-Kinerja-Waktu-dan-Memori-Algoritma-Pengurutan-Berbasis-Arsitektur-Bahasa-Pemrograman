#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>    // Untuk std::setprecision
#include <algorithm>  // Untuk std::swap

// --- Header Khusus Windows ---
#include <windows.h>
#include <psapi.h> // Untuk GetProcessMemoryInfo
// -----------------------------

// --- Implementasi Algoritma ---

void bubbleSort(std::vector<int>& arr) {
    int n = arr.size();
    bool swapped;
    for (int i = 0; i < n - 1; i++) {
        swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

void merge(std::vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    
    std::vector<int> L(n1), R(n2);
    
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];
    
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i]; i++;
        } else {
            arr[k] = R[j]; j++;
        }
        k++;
    }
    while (i < n1) { arr[k] = L[i]; i++; k++; }
    while (j < n2) { arr[k] = R[j]; j++; k++; }
}

void mergeSortImpl(std::vector<int>& arr, int l, int r) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    mergeSortImpl(arr, l, m);
    mergeSortImpl(arr, m + 1, r);
    merge(arr, l, m, r);
}

void mergeSort(std::vector<int>& arr) {
    mergeSortImpl(arr, 0, arr.size() - 1);
}

int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSortImpl(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSortImpl(arr, low, pi - 1);
        quickSortImpl(arr, pi + 1, high);
    }
}

void quickSort(std::vector<int>& arr) {
    quickSortImpl(arr, 0, arr.size() - 1);
}

// --- Fungsi Utilitas ---

std::vector<int> loadData(const std::string& filename) {
    std::cout << "Memuat data dari " << filename << "..." << std::endl;
    std::vector<int> data;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: File " << filename << " tidak ditemukan." << std::endl;
        exit(1);
    }
    int number;
    while (file >> number) {
        data.push_back(number);
    }
    file.close();
    return data;
}

// --- Fungsi Memori (Hanya Windows) ---
double getPeakMemoryMB() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        // PeakWorkingSetSize dalam BYTES
        return (double)pmc.PeakWorkingSetSize / 1024.0 / 1024.0;
    }
    return 0.0; // Gagal
}

template<typename Func>
void runTest(Func algoFunc, std::vector<int> data, const std::string& algoName, int dataSize) {
    std::cout << "  Menguji " << algoName << "..." << std::endl;
    
    auto startTime = std::chrono::high_resolution_clock::now();
    algoFunc(data);
    auto endTime = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double, std::milli> durationMs = endTime - startTime;
    double peakMemMB = getPeakMemoryMB(); 

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "HASIL,C++," << algoName << "," << dataSize << ","
              << durationMs.count() << "," << peakMemMB << std::endl;
}

// --- Main ---
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Cara pakai: ./test_cpp <nama_file_data.txt>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::vector<int> data = loadData(filename);
    int dataSize = data.size();
    std::cout << "Data ter-load: " << dataSize << " angka.\n" << std::endl;

    if (dataSize <= 100000) {
        runTest(bubbleSort, data, "BubbleSort", dataSize);
    } else {
        std::cout << "HASIL,C++,BubbleSort," << dataSize << ",N/A,N/A (Skipped, >100k data)" << std::endl;
    }

    runTest(mergeSort, data, "MergeSort", dataSize);
    runTest(quickSort, data, "QuickSort", dataSize);

    std::cout << "\nPengujian C++ selesai." << std::endl;
    return 0;
}