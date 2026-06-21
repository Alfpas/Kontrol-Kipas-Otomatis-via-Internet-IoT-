# 🌀 Kontrol Kipas Otomatis via Internet (IoT)
### Menggunakan ESP32 + MQTT + Web Dashboard

> Proyek ini memungkinkan kamu menyalakan dan mematikan kipas angin dari mana saja — cukup lewat browser di HP atau komputer, selama terhubung ke internet.

---

## 📖 Daftar Isi
1. [Apa Itu Proyek Ini?](#-apa-itu-proyek-ini)
2. [Alat dan Bahan](#-alat-dan-bahan)
3. [Cara Kerja Sistem](#-cara-kerja-sistem)
4. [Persiapan Sebelum Mulai](#-persiapan-sebelum-mulai)
5. [Upload Program ke ESP32](#-upload-program-ke-esp32)
6. [Menjalankan Web Dashboard](#-menjalankan-web-dashboard)
7. [Akses dari HP (Jaringan yang Sama)](#-akses-dari-hp-jaringan-yang-sama)
8. [Cara Pakai Dashboard](#-cara-pakai-dashboard)
9. [Informasi Koneksi MQTT](#-informasi-koneksi-mqtt)
10. [Penjelasan Topik MQTT](#-penjelasan-topik-mqtt)
11. [Troubleshooting (Kalau Ada Masalah)](#-troubleshooting-kalau-ada-masalah)

---

## 💡 Apa Itu Proyek Ini?

Proyek ini adalah sistem **kendali kipas jarak jauh** menggunakan teknologi IoT *(Internet of Things)*. Singkatnya, kipas angin biasa disambungkan ke modul relay yang dikendalikan oleh **ESP32** (semacam mini komputer kecil), lalu kamu bisa kontrol kipasnya lewat **website** di browser HP atau laptop.

Teknologi yang dipakai namanya **MQTT** — ini adalah protokol komunikasi ringan yang biasa dipakai di dunia IoT. Bayangkan seperti aplikasi chat: ESP32 dan website kamu saling "ngobrol" lewat server MQTT di internet.

---

## 🛒 Alat dan Bahan

### Perangkat Keras (Hardware)
| No | Nama Barang | Keterangan |
|----|-------------|------------|
| 1 | ESP32 | Mikrokontroler utama, otak dari sistem ini |
| 2 | Relay Module 1 Channel | Saklar elektronik untuk menghidupkan/mematikan kipas |
| 3 | Kipas Angin | Kipas yang ingin dikendalikan |
| 4 | Kabel Jumper | Untuk menghubungkan ESP32 ke relay |
| 5 | Kabel USB | Untuk upload program ke ESP32 |
| 6 | Adaptor / Power Supply | Sumber daya untuk ESP32 dan relay |

### Perangkat Lunak (Software)
| No | Nama Software | Fungsi | Download |
|----|--------------|--------|----------|
| 1 | Arduino IDE | Untuk upload program ke ESP32 | [arduino.cc](https://www.arduino.cc/en/software) |
| 2 | Python | Untuk menjalankan web server lokal | [python.org](https://www.python.org/downloads/) |
| 3 | Browser (Chrome/Firefox) | Untuk membuka dashboard web | Sudah ada di HP/PC |

---

## ⚙️ Cara Kerja Sistem

Berikut alur komunikasi sistem ini secara sederhana:

```
[Kamu tekan tombol di Website]
           ↓
    [Server MQTT di Internet]
    (ycdb2f7b.ala...emqxcloud.com)
           ↓
      [ESP32 menerima perintah]
           ↓
    [Relay ON/OFF → Kipas menyala/mati]
           ↓
    [ESP32 kirim status balik ke Website]
           ↓
    [Website tampilkan status terbaru]
```

> **Intinya:** Website → Internet → ESP32 → Relay → Kipas. Semua terjadi dalam hitungan detik!

---

## 🔧 Persiapan Sebelum Mulai

### 1. Install Library Arduino
Buka Arduino IDE, lalu install library berikut lewat menu **Tools → Manage Libraries**:

- `PubSubClient` — oleh Nick O'Leary (untuk komunikasi MQTT)
- `WiFi` — sudah bawaan ESP32, tidak perlu install

### 2. Tambahkan Board ESP32 ke Arduino IDE
Buka **File → Preferences**, lalu di bagian *Additional Board Manager URLs*, tambahkan:
```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```
Kemudian buka **Tools → Board → Board Manager**, cari `esp32` dan install.

### 3. Setting Akun MQTT di EMQX Cloud
Login ke dashboard EMQX Cloud kamu, lalu tambahkan dua akun di menu **Authentication**:

| Username | Password | Dipakai Oleh |
|----------|----------|--------------|
| `web_kontrol` | `WebKontrol123!` | Website / Dashboard |
| `esp32_kipas` | `Esp32Kipas123!` | ESP32 |

---

## 📤 Upload Program ke ESP32

### Skema Wiring (Sambungan Kabel)
```
ESP32 Pin 27  ──→  IN  (Relay)
ESP32 GND     ──→  GND (Relay)
ESP32 3.3V    ──→  VCC (Relay)
```
> ⚠️ **Perhatian:** Relay untuk kipas (tegangan 220V) harus disambungkan dengan hati-hati. Jika belum yakin, minta bantuan yang lebih berpengalaman.

### Sesuaikan Kode Program
Sebelum upload, buka file `esp32_kipas.ino` dan pastikan bagian ini sudah benar:

```cpp
const char* ssid     = "NAMA_WIFI_KAMU";    // Ganti dengan nama WiFi kamu
const char* password = "PASSWORD_WIFI";      // Ganti dengan password WiFi kamu
```

> Nama WiFi dan password harus sama persis, termasuk huruf besar/kecilnya!

### Langkah Upload
1. Sambungkan ESP32 ke komputer pakai kabel USB
2. Buka Arduino IDE
3. Pilih board: **Tools → Board → ESP32 Dev Module**
4. Pilih port: **Tools → Port → COM... (pilih yang muncul)**
5. Klik tombol **Upload** (ikon panah ke kanan ➡️)
6. Tunggu sampai muncul tulisan `Done uploading`
7. Buka **Serial Monitor** (Tools → Serial Monitor), set ke `115200 baud`
8. Kalau berhasil, akan muncul tulisan seperti ini:
```
WiFi TERHUBUNG!
IP Address ESP32: 192.168.x.x
Terhubung ke MQTT broker!
Subscribe ke topic: kipas/kontrol
```

---

## 🌐 Menjalankan Web Dashboard

Web dashboard dijalankan di komputer kamu secara lokal menggunakan Python.

### Langkah-langkah:
1. Buka **Command Prompt** (Windows) atau **Terminal** (Mac/Linux)
2. Masuk ke folder tempat file `index.html` berada:
```bash
cd C:\Users\NamaKamu\Downloads\IOT_Kipas
```
3. Jalankan perintah berikut:
```bash
python -m http.server 8000
```
4. Kalau berhasil, akan muncul:
```
Serving HTTP on :: port 8000 (http://[::]:8000/) ...
```
5. Buka browser dan ketik di address bar:
```
http://localhost:8000/index.html
```
6. Dashboard akan terbuka dan otomatis mencoba terhubung ke server MQTT

> ✅ Jangan tutup jendela Command Prompt selama kamu pakai dashboard-nya!

---

## 📱 Akses dari HP (Jaringan yang Sama)

Supaya HP bisa buka dashboard web yang jalan di komputer kamu, **HP dan komputer harus terhubung ke WiFi yang sama**.

### Langkah 1 — Cari IP Address Komputer Kamu

**Di Windows:**
1. Buka Command Prompt
2. Ketik perintah ini lalu tekan Enter:
```bash
ipconfig
```
3. Cari bagian **"Wireless LAN adapter Wi-Fi"**
4. Lihat baris **"IPv4 Address"**, contohnya:
```
IPv4 Address. . . . . . . . . . . : 192.168.1.5
```
Nah, `192.168.1.5` itulah IP komputer kamu.

**Di Mac/Linux:**
```bash
ifconfig | grep "inet "
```

### Langkah 2 — Buka di Browser HP

Pastikan HP kamu terhubung ke **WiFi yang sama** dengan komputer, lalu buka browser di HP dan ketik:
```
http://192.168.1.5:8000/index.html
```
> Ganti `192.168.1.5` dengan IP komputer kamu yang tadi ditemukan.

### Langkah 3 — Pastikan Firewall Tidak Memblokir

Kalau HP tidak bisa membuka halaman tersebut, kemungkinan firewall Windows memblokir. Cara mengizinkannya:
1. Buka **Windows Defender Firewall**
2. Klik **"Allow an app through firewall"**
3. Cari **Python** dan centang keduanya (Private & Public)
4. Klik OK

### Ringkasan Kondisi yang Harus Terpenuhi

| Syarat | Keterangan |
|--------|------------|
| ✅ HP dan PC satu WiFi | Harus terhubung ke router/hotspot yang sama |
| ✅ Python server jalan | Jangan tutup Command Prompt |
| ✅ Pakai IP lokal PC | Bukan `localhost`, tapi IP seperti `192.168.x.x` |
| ✅ Port 8000 terbuka | Firewall mengizinkan koneksi masuk ke port 8000 |

---

## 🕹️ Cara Pakai Dashboard

Setelah dashboard terbuka di browser:

1. **Tunggu status "Terhubung"** muncul di pojok kanan atas (indikator hijau)
2. **Tombol power besar** di tengah → untuk toggle ON/OFF kipas
3. **Tombol "Nyalakan"** → langsung kirim perintah ON ke kipas
4. **Tombol "Matikan"** → langsung kirim perintah OFF ke kipas
5. **Panel log di kanan** → menampilkan semua aktivitas secara real-time

> Kalau status masih "Menghubungkan..." lebih dari 10 detik, cek bagian Troubleshooting di bawah.

---

## 🔌 Informasi Koneksi MQTT

| Parameter | Nilai |
|-----------|-------|
| **Broker Address** | `ycdb2f7b.ala.dedicated.aws.emqxcloud.com` |
| **Port TCP (ESP32)** | `1883` |
| **Port TLS/SSL (ESP32 secure)** | `8883` |
| **Port WebSocket (Website)** | `8083` |
| **Port WebSocket TLS (Website)** | `8084` ✅ *dipakai* |
| **Username Web** | `web_kontrol` |
| **Username ESP32** | `esp32_kipas` |

> Website menggunakan port **8084** (WebSocket over TLS) karena browser tidak bisa langsung pakai protokol TCP biasa.

---

## 📡 Penjelasan Topik MQTT

Topik MQTT itu seperti "nama channel" tempat pesan dikirim dan diterima.

| Topik | Arah | Isi Pesan | Penjelasan |
|-------|------|-----------|------------|
| `kipas/kontrol` | Website → ESP32 | `ON` atau `OFF` | Website mengirim perintah ke kipas |
| `kipas/status` | ESP32 → Website | `ON` atau `OFF` | ESP32 melaporkan kondisi kipas saat ini |

---

## 🛠️ Troubleshooting (Kalau Ada Masalah)

### ❌ Dashboard tidak bisa terhubung ke MQTT

**Cek di browser (F12 → Console), kemudian:**
- Pastikan username & password di EMQX Cloud sudah didaftarkan
- Coba buka `https://ycdb2f7b.ala.dedicated.aws.emqxcloud.com:8084` di browser — kalau muncul peringatan sertifikat, klik "Lanjutkan" dulu

### ❌ ESP32 tidak konek ke WiFi

- Pastikan nama WiFi (`ssid`) dan password sudah benar
- ESP32 **tidak support WiFi 5GHz** — pastikan kamu pakai jaringan **2.4GHz**
- Coba pindahkan ESP32 lebih dekat ke router

### ❌ ESP32 gagal konek ke MQTT

Buka Serial Monitor, lihat pesan error:
- `rc=-2` → tidak bisa mencapai server (cek koneksi internet)
- `rc=-4` → timeout (server sibuk, coba lagi)
- `rc=5` → username/password salah (cek di EMQX Authentication)

### ❌ HP tidak bisa buka dashboard

- Pastikan HP dan PC pakai **WiFi yang sama**
- Gunakan **IP komputer** (contoh: `192.168.1.5`), bukan `localhost`
- Pastikan Python server masih berjalan di Command Prompt
- Coba matikan firewall sementara untuk test

### ❌ Tombol di dashboard tidak bereaksi

- Cek apakah status koneksi sudah **"Terhubung"** (dot hijau)
- Kalau masih "Menghubungkan...", tunggu beberapa detik atau refresh halaman
- Pastikan ESP32 sudah menyala dan terhubung ke MQTT

---

## 📁 Struktur File Proyek

```
IOT_Kipas/
│
├── index.html          # Dashboard web untuk kontrol kipas
└── esp32_kipas.ino     # Program untuk diupload ke ESP32
```

---
<div align="center">
           
**© 2026 UcokPLC — All Rights Reserved**  
*IoT Project | ESP32 × MQTT × Web Dashboard*

</div>

---

*Dibuat dengan ❤️ — Semoga tugasnya lancar!*
