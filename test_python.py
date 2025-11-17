import time
import tracemalloc
import sys
import os

# Menambah batas rekursi untuk dataset besar (penting untuk Quick Sort)
sys.setrecursionlimit(20_000_000)

# --- Implementasi Algoritma ---

def bubble_sort(arr):
    n = len(arr)
    for i in range(n):
        swapped = False
        for j in range(0, n-i-1):
            if arr[j] > arr[j+1]:
                arr[j], arr[j+1] = arr[j+1], arr[j]
                swapped = True
        if not swapped:
            break
    return arr

def merge_sort(arr):
    if len(arr) > 1:
        mid = len(arr) // 2
        L = arr[:mid]
        R = arr[mid:]

        merge_sort(L)
        merge_sort(R)

        i = j = k = 0
        while i < len(L) and j < len(R):
            if L[i] < R[j]:
                arr[k] = L[i]
                i += 1
            else:
                arr[k] = R[j]
                j += 1
            k += 1

        while i < len(L):
            arr[k] = L[i]
            i += 1
            k += 1
        while j < len(R):
            arr[k] = R[j]
            j += 1
            k += 1
    return arr

def _quick_sort_partition(arr, low, high):
    i = (low - 1)
    pivot = arr[high]
    for j in range(low, high):
        if arr[j] <= pivot:
            i = i + 1
            arr[i], arr[j] = arr[j], arr[i]
    arr[i + 1], arr[high] = arr[high], arr[i + 1]
    return (i + 1)

def quick_sort_impl(arr, low, high):
    if low < high:
        pi = _quick_sort_partition(arr, low, high)
        quick_sort_impl(arr, low, pi - 1)
        quick_sort_impl(arr, pi + 1, high)

def quick_sort(arr):
    quick_sort_impl(arr, 0, len(arr) - 1)
    return arr

# --- Fungsi Utilitas ---

def load_data(filename):
    print(f"Memuat data dari {filename}...")
    try:
        with open(filename, 'r') as f:
            # Membaca semua baris dan konversi ke integer
            return [int(line.strip()) for line in f]
    except FileNotFoundError:
        print(f"Error: File {filename} tidak ditemukan.")
        sys.exit(1)
    except Exception as e:
        print(f"Error saat membaca file: {e}")
        sys.exit(1)

def run_test(algo_func, data, algo_name, data_size):
    """Menjalankan tes dan mengukur waktu & memori."""
    print(f"  Menguji {algo_name}...")
    
    # Salin data agar pengujian adil (tidak mengurutkan data yang sudah terurut)
    data_copy = list(data)
    
    tracemalloc.start()
    start_time = time.perf_counter()
    
    # Jalankan algoritma
    try:
        sorted_data = algo_func(data_copy)
    except RecursionError:
        print(f"  {algo_name} GAGAL: RecursionError")
        return
    except Exception as e:
        print(f"  {algo_name} GAGAL: {e}")
        return

    end_time = time.perf_counter()
    current, peak = tracemalloc.get_traced_memory()
    tracemalloc.stop()
    
    duration_ms = (end_time - start_time) * 1000
    peak_mem_mb = peak / 1024**2
    
    # Format output agar mudah di-copy ke Excel (CSV format)
    # Bahasa,Algoritma,UkuranData,Waktu(ms),MemoriPuncak(MB)
    print(f"HASIL,Python,{algo_name},{data_size},{duration_ms:.2f},{peak_mem_mb:.2f}")

# --- Main ---
if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Cara pakai: python test_python.py <nama_file_data.txt>")
        sys.exit(1)
        
    filename = sys.argv[1]
    if not os.path.exists(filename):
        print(f"File tidak ditemukan: {filename}")
        sys.exit(1)

    data = load_data(filename)
    data_size = len(data)
    print(f"Data ter-load: {data_size} angka.\n")
    
    algos = {
        "MergeSort": merge_sort,
        "QuickSort": quick_sort
    }
    
    # Bubble Sort dipisah, karena sangat lambat untuk data besar
    if data_size <= 10_000: # Hanya jalankan Bubble Sort untuk 100k
        run_test(bubble_sort, data, "BubbleSort", data_size)
    else:
        print("HASIL,Python,BubbleSort,data_size,N/A,N/A (Skipped)")

    for name, func in algos.items():
        run_test(func, data, name, data_size)
        
    print("\nPengujian Python selesai.")