import random

# Tentukan ukuran dataset yang Anda inginkan
# 100_000 (100 ribu), 1_000_000 (1 Juta), 10_000_000 (10 Juta)
sizes = [100_000, 1_000_000, 10_000_000]
max_val = 50_000_000 # Angka acak maksimum

print("Memulai pembuatan dataset...")

for size in sizes:
    filename = f"data_{size}.txt"
    print(f"Membuat {filename}...")
    
    try:
        with open(filename, 'w') as f:
            # Cara cepat untuk menulis banyak angka
            numbers = [str(random.randint(1, max_val)) for _ in range(size)]
            f.write("\n".join(numbers))
        print(f"Selesai: {filename}")
    except Exception as e:
        print(f"Error saat membuat {filename}: {e}")

print("Semua dataset selesai dibuat.")