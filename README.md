## Feeding Fish

Feeding Fish adalah proyek tugas besar mata kuliah Komputer Grafis yang membangun simulasi aquarium 2D interaktif menggunakan bahasa C dan Raylib. Aplikasi ini menampilkan ikan, makanan, bubble, dekorasi aquarium, serta antarmuka menu yang memanfaatkan primitive grafika komputer seperti Raylib primitive, DDA, Bresenham, midpoint circle, dan midpoint ellipse.

## Identitas Mahasiswa

- Nama: Roufiel Hadi
- NIM: 241524028
- Kelas: 2A
- Program Studi: D4 Teknik Informatika
- Institusi: Politeknik Negeri Bandung

## Informasi Akademik

- Mata Kuliah: Komputer Grafis
- Bentuk Tugas: Tugas besar / UTS
- Dosen Pembimbing: M. Riski Solahudin
- Dosen Pembimbing: Yudhi Widiyasana

## Ringkasan Proyek

Proyek ini mengangkat konsep game aquarium management di mana pemain dapat menambahkan ikan, memberi makan, mengamati perilaku entity, dan mengelola isi aquarium secara interaktif. Struktur program dipisahkan ke beberapa modul agar alur aplikasi, entity, system, rendering, dan UI tetap terorganisasi dan mudah dikembangkan.

Fitur utama:

- Welcome screen
- Main menu
- How to Play
- About
- Gameplay aquarium interaktif
- Sistem entity untuk guppy, carnivore, ultravore, food, dan bubble
- Rendering object aquarium dengan primitive 2D

## Teknologi

- C11
- Raylib
- GCC / MinGW
- Makefile

## Struktur Folder

```text
src/
  app/        alur aplikasi, menu, layout, dan konten
  core/       helper dasar aplikasi
  entities/   definisi object permainan
  systems/    logika update permainan
  render/     rendering aquarium dan UI gameplay
  ui/         komponen antarmuka
  utils/      algoritma dan primitive grafika
assets/       font, icon, dan gambar pendukung
```

## Cara Build Dan Run

Pastikan Raylib dan GCC/MinGW sudah tersedia di environment Windows, lalu jalankan:

```bash
mingw32-make
./aquarium.exe
```

## Catatan Implementasi

- Main game mempertahankan arsitektur simulasi aquarium.
- Menu non-game dirancang agar lebih ringan dengan lebih banyak penggunaan primitive Raylib.
- Preview item pada halaman How to Play tetap mempertahankan jalur primitive custom berbasis algoritma grafika komputer.

## Catatan Penggunaan

Proyek ini disusun untuk kebutuhan akademik pada mata kuliah Komputer Grafis di Politeknik Negeri Bandung.
