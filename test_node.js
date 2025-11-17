const fs = require('fs');
const process = require('process');

// --- Implementasi Algoritma ---

function bubbleSort(arr) {
    const n = arr.length;
    for (let i = 0; i < n; i++) {
        let swapped = false;
        for (let j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                [arr[j], arr[j + 1]] = [arr[j + 1], arr[j]]; // swap
                swapped = true;
            }
        }
        if (!swapped) break;
    }
    return arr;
}

function mergeSort(arr) {
    if (arr.length <= 1) {
        return arr;
    }
    const mid = Math.floor(arr.length / 2);
    const left = mergeSort(arr.slice(0, mid));
    const right = mergeSort(arr.slice(mid));
    return merge(left, right);
}

function merge(left, right) {
    let resultArray = [], leftIndex = 0, rightIndex = 0;
    while (leftIndex < left.length && rightIndex < right.length) {
        if (left[leftIndex] < right[rightIndex]) {
            resultArray.push(left[leftIndex]);
            leftIndex++;
        } else {
            resultArray.push(right[rightIndex]);
            rightIndex++;
        }
    }
    return resultArray
        .concat(left.slice(leftIndex))
        .concat(right.slice(rightIndex));
}

function quickSort(arr) {
    // Implementasi non-rekursif (in-place) untuk menghindari stack overflow
    let stack = [];
    stack.push(0);
    stack.push(arr.length - 1);

    while (stack.length > 0) {
        let high = stack.pop();
        let low = stack.pop();

        let pi = partition(arr, low, high);

        if (pi - 1 > low) {
            stack.push(low);
            stack.push(pi - 1);
        }
        if (pi + 1 < high) {
            stack.push(pi + 1);
            stack.push(high);
        }
    }
    return arr;
}

function partition(arr, low, high) {
    let pivot = arr[high];
    let i = (low - 1);
    for (let j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            [arr[i], arr[j]] = [arr[j], arr[i]]; // swap
        }
    }
    [arr[i + 1], arr[high]] = [arr[high], arr[i + 1]]; // swap
    return (i + 1);
}

// --- Fungsi Utilitas ---

function loadData(filename) {
    console.log(`Memuat data dari ${filename}...`);
    try {
        const data = fs.readFileSync(filename, 'utf8');
        return data.split('\n').map(Number);
    } catch (e) {
        console.error(`Error saat membaca file: ${e.message}`);
        process.exit(1);
    }
}

function runTest(algoFunc, data, algoName, dataSize) {
    console.log(`  Menguji ${algoName}...`);
    const dataCopy = [...data]; // Salin data

    // Paksa Garbage Collection sebelum tes memori
    if (global.gc) { global.gc(); }
    const memBefore = process.memoryUsage().heapUsed;
    
    const startTime = process.hrtime.bigint();
    
    // Jalankan algoritma
    const sortedArray = algoFunc(dataCopy);
    
    const endTime = process.hrtime.bigint();
    const memAfter = process.memoryUsage().heapUsed;

    const durationMs = Number(endTime - startTime) / 1_000_000;
    // Mengukur *tambahan* memori yang digunakan (heap)
    const peakMemMb = (memAfter - memBefore) / 1024**2;

    // Bahasa,Algoritma,UkuranData,Waktu(ms),MemoriPuncak(MB)
    console.log(`HASIL,NodeJS,${algoName},${dataSize},${durationMs.toFixed(2)},${peakMemMb.toFixed(2)}`);
}

// --- Main ---
const filename = process.argv[2];
if (!filename) {
    console.log("Cara pakai: node --expose-gc test_node.js <nama_file_data.txt>");
    process.exit(1);
}

const data = loadData(filename);
const dataSize = data.length;
console.log(`Data ter-load: ${dataSize} angka.\n`);

const algos = {
    "MergeSort": mergeSort,
    "QuickSort": quickSort
};

if (dataSize <= 100_000) {
    runTest(bubbleSort, data, "BubbleSort", dataSize);
} else {
    console.log(`HASIL,NodeJS,BubbleSort,${dataSize},N/A,N/A (Skipped, >100k data)`);
}

for (const name in algos) {
    runTest(algos[name], data, name, dataSize);
}

console.log("\nPengujian Node.js selesai.");