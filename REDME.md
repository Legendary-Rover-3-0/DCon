# DCon
1. Określić architekturę SW - podział na pliki i foldery.
2. Stworzenie instrukcji instalacji narzędzi wymaganych do odpalenia kompilacji i wgrania kodu na płytkę. Instrukcja na podstawie: https://www.youtube.com/watch?v=JcfHbPxRYw8
3. Integracja biblioteki DLT- logowanie błędów: https://www.youtube.com/watch?v=htQmj42eDHw
4. Implementacja SW.
5. Odpalenie na HW.

## INSTRUKCJA:
# Należy pobrać toolchain z filmiku z punktu 2. Aby otworzyć menager dodatków 'Ctrl+Shift+X'. Lista wymaganych dodatków w VSCode:
* Arm Assemly.
* C/C++.
* C/C++ Extension Pack.
* C/C++ Themes.
* CMake.
* CMake Tools.
* Cortex-Debug.
* Embedded Tools.
* Makefile Tools.
* Peripherial Viewer.
* vscode-pdf.

# Przed dodaniem skrótów klawiszowych należy skonfigurować pliki w katalogu '.vscode'. Należy w odpowiednich miejscach
# zmienić ścieżki zależnie od indywidualnej preferencji zapisu wymaganego toolchaina na swoim komputerze. Linie wymagające zmiany są odpowiednio oznaczone.

# Dodawanie skrótów klawiszowych w VSCode. Aby dodać skróty klawiszowe w Visual Studio Code, wykonaj następujące kroki:
1. Naciśnij kombinację klawiszy 'Ctrl+K Ctrl+S'.
2. W prawym górnym rogu okna wybierz opcję Open Keyboard Shortcuts (JSON) - ikonka folderu ze strzałką.
3. Wklej poniższą konfigurację:
```
[
    {
        "key": "ctrl+shift+f6",
        "command": "workbench.action.tasks.runTask",
        "args": "Clean"
    },
    {
        "key": "ctrl+shift+f7",
        "command": "workbench.action.tasks.runTask",
        "args": "Make"
    },
    {
        "key": "ctrl+shift+f8",
        "command": "workbench.action.tasks.runTask",
        "args": "Flash"
    },
]
```

## Funkcje skrótów klawiszowych
# Po dodaniu tej konfiguracji możesz wykonywać następujące czynności za pomocą skrótów klawiszowych:
1. 'Ctrl+Shift+F6' – Usuwanie katalogu 'build'.
2. 'Ctrl+Shift+F7' – Budowanie projektu.
3. 'Ctrl+Shift+F8' – Wgrywanie projektu na płytkę.
Po wykonaniu zweryfikuj działanie ustawień np. zbuduj a później usuń folder build za pomocą skrótów.

# Plik .vscode zawiera specyficzne ustawienia dla użytkownika. W repozytorium istnieją podstawowe ustawienia, które należy zedytować, ale zmianę
należy zablokować dla inncyh użytkowników.
Otwórz terminal i wklej komendę- 'Ctrl+Shift+'' lub Terminal->new Terminal: 
```
git update-index --assume-unchanged .vscode/*
```
