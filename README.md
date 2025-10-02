# Minishell 🐚

**42 Türkiye** için geliştirilen tam fonksiyonel bir Unix shell implementasyonu. Bu proje, bash benzeri bir kabuk uygulaması oluşturarak sistem programlama, süreç yönetimi ve Unix sistemlerinin derinlemesine anlaşılmasını hedefler.

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Language](https://img.shields.io/badge/language-C-blue.svg)
![42](https://img.shields.io/badge/42-project-ff69b4.svg)

## 📋 İçindekiler
- [Teknik Terimler Sözlüğü](#teknik-terimler-sözlüğü)
- [Proje Amacı](#proje-amacı)
- [Özellikler](#özellikler)
- [Teknik Mimari](#teknik-mimari)
- [Kurulum](#kurulum)
- [Kullanım](#kullanım)
- [Test](#test)
- [Kod Yapısı](#kod-yapısı)
- [İmplementasyon Detayları](#implementasyon-detayları)
- [Katkıda Bulunanlar](#katkıda-bulunanlar)

## 📚 Teknik Terimler Sözlüğü

### 🔧 Temel Kavramlar

**Shell (Kabuk)**: Kullanıcı ile işletim sistemi arasındaki arayüz. Komutları yorumlar ve çalıştırır.

**Process (Süreç)**: Çalışan bir programın işletim sistemindeki temsili. Her komut ayrı bir process'te çalışır.

**Fork**: Yeni bir process oluşturma sistem çağrısı. Mevcut process'in birebir kopyasını oluşturur.

**Exec**: Mevcut process'in hafızasını yeni bir programla değiştiren sistem çağrısı.

**Wait**: Parent process'in child process'lerin bitmesini beklemesi için kullanılan sistem çağrısı.

### 🔀 Pipeline ve Redirection

**Pipeline**: Komutları zincirleyerek bir komutun çıktısını diğerinin girdisi yapma (`|` ile).

**Pipe**: İki process arasında veri iletimi için kullanılan kanal. Bir ucundan yazılan veri diğer ucundan okunur.

**Redirection (Yönlendirme)**: Komutların girdi/çıktılarını dosyalara yönlendirme:
- `>`: Çıktıyı dosyaya yaz (üzerine yaz)
- `>>`: Çıktıyı dosyaya ekle
- `<`: Dosyadan girdi al
- `<<`: Here document (satır içi girdi)

**File Descriptor (Dosya Tanımlayıcı)**: Açık dosyaları ve I/O kanallarını temsil eden sayılar (0=stdin, 1=stdout, 2=stderr).

### 🧠 Parsing ve Analysis

**Token**: Lexer tarafından oluşturulan en küçük anlamlı birim (kelime, operatör, vs.).

**Lexer (Tokenizer)**: Girdi metnini token'lara bölen modül.

**Parser**: Token'ları alıp komut yapıları oluşturan modül.

**Syntax Analysis (Sözdizimi Analizi)**: Komutların doğru yazılıp yazılmadığını kontrol etme.

### 💾 Memory ve Signal

**Memory Leak**: Ayrılan belleğin serbest bırakılmaması durumu.

**Signal**: Process'ler arası iletişim mekanizması:
- `SIGINT`: Ctrl+C (interrupt)
- `SIGQUIT`: Ctrl+\ (quit)
- `SIGTERM`: Programı sonlandırma isteği

**Exit Status (Çıkış Kodu)**: Programın başarı/başarısızlık durumunu belirten sayı (0=başarılı, >0=hata).

### 🔧 Built-in Commands

**Built-in**: Shell içinde gömülü olan, ayrı process gerektirmeyen komutlar (`cd`, `echo`, `exit`, vs.).

**Environment Variables (Çevre Değişkenleri)**: Process'lerin erişebildiği global değişkenler (`PATH`, `HOME`, vs.).

**Variable Expansion**: Değişkenlerin (`$VAR`) gerçek değerleriyle değiştirilmesi.

## 🎯 Proje Amacı

Minishell, Unix sistemlerinin temel shell fonksiyonlarını C dilinde yeniden implemente eden bir projedir. Bu proje ile aşağıdaki konularda derinlemesine bilgi edinilmiştir:

- **Sistem Programlama**: Fork, exec, wait, pipe gibi sistem çağrıları
- **Süreç Yönetimi**: Child process yönetimi ve pipeline implementasyonu
- **Signal Handling**: SIGINT, SIGQUIT gibi sinyallerin doğru işlenmesi
- **Bellek Yönetimi**: Dynamic memory allocation ve memory leak önleme
- **Lexer/Parser Implementasyonu**: Komut satırı parsing ve token analizi

## ✨ Özellikler

### 🔧 Temel Shell Fonksiyonları
- ✅ **Prompt Gösterimi**: Interactive shell prompt
- ✅ **Command History**: Readline kütüphanesi ile komut geçmişi
- ✅ **Command Execution**: External program çalıştırma
- ✅ **Path Resolution**: $PATH değişkeni ile komut bulma

### 🚀 Built-in Commands
- `echo` (with -n option)
- `cd` (relative/absolute paths)
- `pwd` (current directory)
- `export` (environment variables)
- `unset` (remove environment variables)
- `env` (display environment)
- `exit` (with exit codes)

### 🔀 Pipeline & Redirection
- **Pipes**: `|` ile komutları zincirleyebilme
- **Input Redirection**: `<` ile dosyadan girdi alma
- **Output Redirection**: `>` ile dosyaya çıktı yönlendirme
- **Append Redirection**: `>>` ile dosyaya ekleme
- **Here Document**: `<<` ile heredoc desteği

### 💬 Quote Handling
- **Single Quotes**: `'text'` - Literal string
- **Double Quotes**: `"text"` - Variable expansion ile
- **Escape Characters**: Backslash escaping

### 🔄 Variable Expansion
- **Environment Variables**: `$VAR` expansion
- **Exit Status**: `$?` son komutun çıkış kodu
- **Process ID**: `$$` shell process ID

### ⚡ Signal Handling
- **Ctrl+C (SIGINT)**: Yeni prompt gösterme
- **Ctrl+D (EOF)**: Shell'den çıkış
- **Ctrl+\\ (SIGQUIT)**: Interactive modda ignore

## 🏗️ Teknik Mimari

### Pipeline Yönetimi Sistemi

**Pipeline Nedir?**
Pipeline, bir komutun çıktısını başka bir komutun girdisi olarak kullanma yöntemidir. `|` (pipe) karakteri ile komutlar zincirlenir. Örneğin: `ls | grep ".c"` - ls komutunun çıktısı grep komutuna girdi olarak gönderilir.

Proje, modern shell'lerin pipeline yönetimini etkili bir şekilde implemente eder:

```c
// Pipeline execution flow
Pipeline → Fork Processes → Setup Pipes → Execute Commands → Wait Children
```

#### 🔄 Pipeline Execution Algorithm

1. **Command Parsing (Komut Ayrıştırma)**: Kullanıcının yazdığı metin → Token listesi → Komut yapıları
2. **Fork Management (Süreç Yönetimi)**: Her komut için ayrı child process (alt süreç) oluşturma
3. **Pipe Setup (Boru Kurulumu)**: Komutlar arası veri akışı için pipe'lar (borular) oluşturma
4. **Process Synchronization (Süreç Senkronizasyonu)**: Parent process (ana süreç) tüm child'ları bekler
5. **Exit Status Handling (Çıkış Durumu İşleme)**: Son komutun exit status'ü (çıkış kodu) shell'e aktarılır

```c
int wait_for_children(pid_t last_pid, int process_count)
{
    int i, status, last_status = 0;
    
    // Tüm child process'leri bekle
    while (i < process_count) {
        if (wait(&status) == last_pid)
            last_status = status;  // Son komutun statusu
        i++;
    }
    
    // Sinyal/çıkış kodu işleme
    if (WIFEXITED(last_status))
        return (WEXITSTATUS(last_status));
    if (WIFSIGNALED(last_status))
        return (128 + WTERMSIG(last_status));
    
    return (last_status);
}
```

### 🧠 Lexer & Parser Architecture

**Lexer Nedir?**
Lexer (Lexical Analyzer - Sözcüksel Çözümleyici), kullanıcının yazdığı metni küçük anlamlı parçalara (token'lara) böler. Örneğin: `echo "hello" | grep h` → [`echo`, `"hello"`, `|`, `grep`, `h`]

**Parser Nedir?**
Parser (Sözdizimi Çözümleyici), lexer'dan gelen token'ları alır ve komut yapıları oluşturur. Hangi argümanın hangi komuta ait olduğunu, pipe'ların nasıl bağlandığını belirler.

#### Lexical Analysis
```c
typedef enum e_token_type {
    TOKEN_WORD,           // Komut/argüman
    TOKEN_PIPE,           // |
    TOKEN_REDIR_IN,       // <
    TOKEN_REDIR_OUT,      // >
    TOKEN_REDIR_APPEND,   // >>
    TOKEN_REDIR_HEREDOC,  // <<
    TOKEN_EOF
} t_token_type;
```

#### Syntax Analysis
- **Command Structures (Komut Yapıları)**: Arguments (argümanlar) ve redirections (yönlendirmeler) ayrıştırma
- **Pipeline Detection (Pipeline Tespiti)**: Pipe ile bağlı komut zincirlerini tespit etme
- **Error Detection (Hata Tespiti)**: Syntax error'ları (sözdizimi hatalarını) erken tespit etme

### 💾 Memory Management

**Memory Management Nedir?**
Programın çalışma sırasında kullandığı belleği (RAM) etkili bir şekilde yönetme işlemidir. C dilinde malloc() ile ayrılan her bellek alanının free() ile serbest bırakılması gerekir.

**Custom Memory Allocator**: Tüm dynamic memory allocations (dinamik bellek ayırmaları) takip edilir
```c
// Bellek takip yapıları
typedef struct s_shell {
    char **envp;           // Çevre değişkenleri
    t_token *tokens;       // Lexer çıktısı
    t_command *cmds;       // Parser çıktısı
    int last_exit_status;  // Çıkış kodu takibi
} t_shell;
```

**Memory Safety**:
- Her malloc'a karşılık free garantisi
- Pipeline execution sonrası cleanup
- Error handling sırasında memory leak prevention

## 🛠️ Kurulum

### Gereksinimler
- GCC compiler
- GNU Readline library
- Make utility
- Linux/Unix environment

### Derleme
```bash
# Projeyi klonla
git clone 
cd minishell

# Derle
make

# Çalıştır
./minishell
```

### Debug Mode
```bash
# Debug bilgileri ile derleme (eğer Makefile'da tanımlıysa)
make debug

# Valgrind ile bellek sızıntısı kontrolü
valgrind --leak-check=full ./minishell
```

## 🎮 Kullanım

### Temel Komutlar
```bash
$ ./minishell
minishell$ echo "Hello World"
Hello World

minishell$ pwd
/home/user/minishell

minishell$ export MY_VAR="test value"
minishell$ echo $MY_VAR
test value
```

### Pipeline Örnekleri
```bash
# Basit pipeline
minishell$ ls -la | grep ".c" | wc -l

# Karmaşık pipeline
minishell$ cat file.txt | grep "pattern" | sort | uniq

# Yönlendirme ile birlikte
minishell$ ls -la > output.txt
minishell$ cat < input.txt | grep "search" >> results.txt
```

### Heredoc Kullanımı
```bash
minishell$ cat << EOF
> This is a heredoc
> Multiple lines supported
> EOF
This is a heredoc
Multiple lines supported
```

## 🧪 Test

Projeyi test etmek için:

```bash
# Derleme testi
make

# Temel shell fonksiyonlarını test et
./minishell
```

### Manuel Test Önerileri
```bash
# Pipeline testleri
echo "hello" | cat | wc -l

# Yönlendirme testleri  
echo "test" > file.txt
cat < file.txt

# Yerleşik komut testleri
cd /tmp && pwd
export TEST=value && echo $TEST
```

## 📁 Kod Yapısı

```
minishell/
├── 📂 includes/           # Başlık dosyaları
│   └── minishell.h       # Ana başlık dosyası
├── 📂 libft/             # Yardımcı fonksiyonlar
├── 📂 srcs/              # Kaynak kodlar
│   ├── 📂 builtins/      # Yerleşik komutlar
│   ├── 📂 executor/      # Pipeline ve süreç yönetimi
│   ├── 📂 expander/      # Değişken genişletme
│   ├── 📂 lexer/         # Tokenizasyon
│   ├── 📂 parser/        # Sözdizimi analizi
│   ├── 📂 signal/        # Sinyal işleme
│   ├── 📂 memory/        # Bellek yönetimi
│   └── 📂 error/         # Hata işleme
└── Makefile              # Derleme yapılandırması
```

## 🔧 İmplementasyon Detayları

### Pipeline İmplementasyonu

**Multi-process Pipeline**:
```c
// Her komut için fork
pid_t child_pid = fork();
if (child_pid == 0) {
    // Alt süreç: komut çalıştırma
    setup_pipes_for_child();
    execute_command();
} else {
    // Ana süreç: pipeline koordinasyonu
    manage_pipes();
    continue_to_next_command();
}
```

**Pipe Management (Boru Yönetimi)**:
- Her komut çifti arasında pipe (boru) oluşturma
- File descriptor'ların (dosya tanımlayıcı) doğru yönetimi
- Parent process'te pipe cleanup (temizlik) işlemleri

### Signal Handling Strategy

**Signal Handling Nedir?**
Signal'lar, process'lere gönderilen mesajlardır. Ctrl+C basıldığında SIGINT sinyali gönderilir. Shell bu sinyalleri yakalayıp uygun şekilde işler.

```c
// Etkileşimli mod sinyalleri
signal(SIGINT, handle_sigint);    // Ctrl+C
signal(SIGQUIT, SIG_IGN);         // Ctrl+\ göz ardı edilir

// Çalıştırma modu sinyalleri
signal(SIGINT, SIG_DFL);          // Varsayılan davranış
signal(SIGQUIT, SIG_DFL);         // Varsayılan davranış
```

### Variable Expansion Engine

**Variable Expansion Nedir?**
`$VAR` şeklinde yazılan değişken isimlerinin gerçek değerleriyle değiştirilmesi işlemidir. Örneğin `echo $HOME` komutu `/home/user` olarak genişletilir.

**Dollar Expansion Algorithm (Dolar Genişletme Algoritması)**:
1. String'i tara ve `$` karakterlerini bul
2. Variable name'i (değişken adını) parse et (ayrıştır)
3. Environment'tan (çevre değişkenlerinden) değeri al
4. String'i reconstruct et (yeniden oluştur)

**Quote State Machine (Tırnak Durum Makinesi)**:
- `NO_QUOTE`: Normal expansion (normal genişletme)
- `SINGLE_QUOTE`: No expansion (genişletme yok - 'tek tırnak')
- `DOUBLE_QUOTE`: Limited expansion (sınırlı genişletme - "çift tırnak")

### Error Handling Philosophy

**Error Handling Nedir?**
Programın hata durumlarıyla karşılaştığında nasıl davranacağını belirleyen yaklaşımdır. İyi bir hata yönetimi, programın çökmesini engeller ve kullanıcıya anlamlı mesajlar verir.

**Graceful Degradation (Zarif Bozulma)**:
- Command not found (komut bulunamadı) → 127 exit code (çıkış kodu)
- Permission denied (izin reddedildi) → 126 exit code
- Syntax errors (sözdizimi hataları) → 2 exit code
- Memory allocation failures (bellek ayırma hataları) → Clean exit (temiz çıkış)

## 🤝 Katkıda Bulunanlar

- **[@yalives7](https://github.com/yalives7)** 
- **[@msngl0234](https://github.com/msngl0234)** 


## 📜 Lisans

Bu proje MIT lisansı altında lisanslanmıştır. Detaylar için `LICENSE` dosyasını inceleyiniz.

## 🎓 42 School Project

Bu proje 42 Kocaeli müfredatının bir parçası olarak geliştirilmiştir. Proje, sistem programlama ve Unix shell implementasyonu konularında derinlemesine öğrenme sağlar.

-

**Not**: Bu README, projenin teknik derinliğini ve implementasyon kalitesini showcase etmek için hazırlanmıştır. Her modül, gerçek dünya shell'lerinin complexity'sini yansıtacak şekilde tasarlanmıştır.
