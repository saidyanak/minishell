# Minishell - 42 Projesi

**Kendi UNIX Kabuğumuz**

Bu proje, [42 Okulu](https://www.42.org.tr) müfredatı kapsamında, `bash` gibi popüler kabukların temel işlevlerini taklit eden kendi UNIX kabuğumuzu sıfırdan oluşturmayı amaçlamaktadır. Proje, proses yönetimi, dosya tanımlayıcıları (file descriptors), sinyal işleme ve komut satırı yorumlama gibi temel C ve UNIX konseptlerini derinlemesine anlamamızı sağlamıştır.

![Minishell Demo](<img width="1183" height="490" alt="Screenshot 2025-09-30 004649" src="https://github.com/user-attachments/assets/7153c35a-d2e2-408b-862b-d1e4a78df836" />)

## 📜 Projenin Amacı

Minishell, bir komut satırı yorumlayıcısıdır. Kullanıcıdan aldığı komutları analiz eder, çalıştırır ve sonucunu ekrana basar. Bu süreç, modern işletim sistemlerinin temel taşlarından birini anlamak için harika bir fırsattır.

## ✨ Özellikler

Minishell'imiz aşağıdaki temel kabuk (shell) özelliklerini desteklemektedir:

* **Komut İstemi (Prompt):** Her komut girişinden önce interaktif bir istem gösterir.
* **Komut Geçmişi:** `readline` kütüphanesi sayesinde yukarı/aşağı ok tuşlarıyla komut geçmişinde gezinebilir.
* **Komut Yürütme:** `PATH` ortam değişkeninde bulunan veya mutlak/göreli yolla belirtilen tüm sistem komutlarını çalıştırır.
* **Ayrıştırma (Parsing):**
    * Komutları, opsiyonları ve argümanları doğru bir şekilde ayırır.
    * Tek tırnak (`'`) ve çift tırnak (`"`) içindeki ifadeleri bir bütün olarak ele alır. Çift tırnak içinde ortam değişkenlerini ($VAR) genişletir.
* **Ortam Değişkenleri:** `$VAR` veya `${VAR}` formatındaki ortam değişkenlerini komut yürütülmeden önce çözümler. `$?` ile son komutun çıkış koduna erişim sağlar.
* **Yönlendirmeler (Redirections):**
    * `<` : Dosyadan girdi alır (input).
    * `>` : Çıktıyı dosyaya yazar (output), dosya yoksa oluşturur, varsa içeriğini siler.
    * `>>` : Çıktıyı dosyanın sonuna ekler (append).
    * `<<` : "Here Document" (heredoc) ile geçici girdi oluşturur.
* **Pipes (`|`):** Bir komutun çıktısını diğer bir komutun girdisine bağlar. Çoklu pipe zincirleri desteklenmektedir.
* **Sinyal Yönetimi:**
    * `Ctrl-C`: Çalışan bir prosesi sonlandırır, komut satırı boşken yeni bir satır oluşturur.
    * `Ctrl-D`: Komut satırı boşken kabuğu sonlandırır (EOF).
    * `Ctrl-\`: Çalışan bir prosesi `SIGQUIT` sinyali ile sonlandırır.

## 🛠️ Yerleşik (Built-in) Komutlar

Aşağıdaki komutlar, yeni bir proses oluşturulmadan doğrudan Minishell içinde çalıştırılır:

* `echo`: Argümanları ekrana basar. `-n` seçeneği ile yeni satır eklemez.
* `cd`: Çalışma dizinini değiştirir.
* `pwd`: Mevcut çalışma dizinini gösterir.
* `export`: Yeni ortam değişkeni oluşturur veya mevcut olanı günceller.
* `unset`: Bir ortam değişkenini siler.
* `env`: Tüm ortam değişkenlerini listeler.
* `exit`: Minishell'den çıkış yapar.

## 🚀 Kurulum ve Çalıştırma

Projeyi klonladıktan sonra derlemek ve çalıştırmak için aşağıdaki adımları izleyin:

```bash
# 1. Proje reposunu klonlayın
git clone [https://github.com/kullanici-adiniz/minishell.git](https://github.com/kullanici-adiniz/minishell.git)

# 2. Proje dizinine gidin
cd minishell

# 3. Projeyi derleyin
make

# 4. Minishell'i başlatın
./minishell
```

## 💻 Kullanım Örnekleri

**Basit Komut:**
```shell
minishell> ls -l | grep ".c"
```

**Yönlendirmeler:**
```shell
minishell> cat Makefile > output.txt
minishell> grep "main" < main.c
minishell> echo "yeni satir" >> output.txt
```

**Here Document (`<<`):**
```shell
minishell> cat << END
> Bu bir here document testidir.
> Birden fazla satır yazılabilir.
> END
```

**Ortam Değişkenleri ve Tırnaklar:**
```shell
minishell> export MESAJ="Merhaba Dunya"
minishell> echo "Mesajim: $MESAJ"
Mesajim: Merhaba Dunya
minishell> echo 'Mesajim: $MESAJ'
Mesajim: $MESAJ
```

## 🧠 Projede Öğrenilen Temel Konseptler

* **Proses Yönetimi:** `fork()`, `execve()`, `waitpid()`
* **Dosya Tanımlayıcıları:** `open()`, `close()`, `read()`, `write()`, `dup()`, `dup2()`
* **Pipe Yönetimi:** `pipe()` ile prosesler arası iletişim.
* **Sinyal İşleme:** `signal()`, `sigaction()`, `kill()`
* **Bellek Yönetimi:** `malloc()`, `free()` ve bellek sızıntılarının önlenmesi.
* **Ayrıştırma (Parsing):** Komut satırını mantıksal birimlere (token) ayırma ve bu birimleri yorumlayarak bir yürütme ağacı (execution tree) oluşturma.
* **Hata Yönetimi:** Sistem çağrılarından dönen hataları doğru bir şekilde ele alma.

## 👤 Yazarlar

Bu proje aşağıdaki kişiler tarafından geliştirilmiştir:

* **[Adın Soyadın]** - ([@github-kullanici-adin](https://github.com/github-kullanici-adin))
* **[Partnerinin Adı Soyadı]** - ([@partner-github-kullanici-adi](https://github.com/partner-github-kullanici-adi))

---
*Bu proje, 42 Network'ün eğitim müfredatının bir parçasıdır.*
