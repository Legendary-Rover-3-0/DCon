### Drive Control

## To do:
* Określić architekturę SW - podział na pliki i foldery.
* Stworzenie instrukcji instalacji narzędzi wymaganych do odpalenia kompilacji i wgrania kodu na płytkę. Instrukcja na podstawie: https://www.youtube.com/watch?v=JcfHbPxRYw8
(zostało dodać opis instalacji krok po kroku oraz wymagane linki w taki sposób jak w `INSTRUKCJA DLA VSCODE`).
* Integracja biblioteki DLT- logowanie błędów: https://www.youtube.com/watch?v=htQmj42eDHw (W trakcie).
* Implementacja SW.
* Odpalenie na HW.
* Dodanie instrukcji oraz instalacji programu DLT Viewer. Biblioteka `DLT (Diagnostic Log Trace)` służy do zbierania logów w celu analizy działania programu w bardziej praktyczny sposób jak samo printowanie na konsolę UART. DLT Viewer jest programem do zbierania i analizy uzyskanch logów.

## Instrukcja korzystania z Gita z podziałem na gałęzie master i develop

1. Pobranie repozytorium

Sklonuj repozytorium: Otwórz terminal lub linię poleceń i wykonaj poniższe polecenie, aby pobrać repozytorium:
```
git clone https://github.com/Legendary-Rover-3-0/DCon.git
```
Przejdź do katalogu z repozytorium. Po sklonowaniu repozytorium wejdź do odpowiedniego katalogu:
```
cd twoj-folder
```
Sprawdź obecne gałęzie. Aby zobaczyć listę wszystkich gałęzi w repozytorium, użyj:
```
git branch -a
```

2. Praca na gałęzi develop

Przełącz się na gałęź develop:
```
git checkout develop
```
Zaktualizuj lokalną kopię gałęzi. Zanim zaczniesz pracować, upewnij się, że masz najnowsze zmiany:
```
git pull origin develop
```
Dokonaj zmian w kodzie. Otwórz pliki w swoim edytorze kodu, wprowadź zmiany i zapisz je.

Dodaj zmiany do obszaru staging. Po zakończeniu edycji oznacz zmienione pliki jako gotowe do zatwierdzenia:
```
git add .
```
Zatwierdź zmiany:Dodaj krótki opis swoich zmian:
```
git commit -m "Opis wprowadzonych zmian"
```
Wypchnij zmiany na zdalne repozytorium:
```
git push origin develop
```

3. Rozwiązywanie konfliktów

Podczas wypychania może wystąpić konflikt. W takim przypadku pobierz najnowsze zmiany z gałęzi develop:
```
git pull origin develop
```
Rozwiąż konflikty w swoim edytorze. Zmodyfikuj pliki tak, aby usunąć konflikty, a następnie ponownie zatwierdź zmiany:
```
git add .
git commit -m "Rozwiązano konflikty"
```
Spróbuj ponownie wypchnąć zmiany:
```
git push origin develop
```

4. Ważne uwagi

Gałęź master jest przeznaczona do stabilnych wersji projektu. Nie pracuj bezpośrednio na tej gałęzi.
Pamiętaj o regularnym pobieraniu najnowszych zmian z gałęzi develop przed rozpoczęciem pracy:
```
git pull origin develop
```
Używaj zrozumiałych opisów podczas zatwierdzania zmian.

## INSTRUKCJA DLA VSCODE:
Należy pobrać wymagany toolchain z punktu 2. Aby otworzyć menager dodatków użyj `Ctrl+Shift+X`. Lista wymaganych dodatków w VSCode:
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

Przed dodaniem skrótów klawiszowych należy skonfigurować pliki w katalogu `.vscode`. Należy w odpowiednich miejscach zmienić ścieżki zależnie od indywidualnej preferencji zapisu wymaganego toolchaina na swoim komputerze. Linie wymagające zmiany są odpowiednio oznaczone.

Dodawanie skrótów klawiszowych w VSCode. Aby dodać skróty klawiszowe w Visual Studio Code, wykonaj następujące kroki:
1. Naciśnij kombinację klawiszy `Ctrl+K Ctrl+S`.
2. W prawym górnym rogu okna wybierz opcję `Open Keyboard Shortcuts (JSON)` - ikonka folderu ze strzałką.
3. Wklej poniższą konfigurację i zapisz:
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
# Funkcje skrótów klawiszowych
Po dodaniu tej konfiguracji możesz wykonywać następujące czynności za pomocą skrótów klawiszowych:
1. 'Ctrl+Shift+F6' – Usuwanie katalogu `build`.
2. 'Ctrl+Shift+F7' – Budowanie projektu.
3. 'Ctrl+Shift+F8' – Wgrywanie projektu na płytkę.
Po wykonaniu zweryfikuj działanie ustawień np. zbuduj a później usuń folder build za pomocą skrótów.

# Ignorowania zmian w folderze .vscode
Plik .vscode zawiera specyficzne ustawienia dla użytkownika. W repozytorium istnieją podstawowe ustawienia, które należy zedytować, ale zmianę
należy zablokować dla inncyh użytkowników.
Otwórz terminal i wklej poniższą komendę (`Ctrl+Shift+'` lub `Terminal->new Terminal`): 
```
git update-index --assume-unchanged .vscode/*
```

# Logi DLT
Przykład użycia w kodzie:
```
DLT_DEBUG("Lps25hb initialized error!");
DLT_DEBUG("Press:%d hPa, Temp:%d °C", Pressure, Temperature);
DLT_DEBUG("Status %d", Status);
```