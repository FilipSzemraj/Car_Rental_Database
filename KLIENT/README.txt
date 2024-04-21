Wymagania:
1. Kompilator C++, np. pochodz¹cy z Visual Studio, skrypty by³y testowane dla wersji Visual Studio Community 14,
2. Baza Danych Oracle, skrypty by³y testowane dla wersji Oracle 18c Express Edition,
3. Pliki pobrane ze strony Instant Client Downloads for Microsoft Windows (x64) 64-bit
https://www.oracle.com/pl/database/technologies/instant-client/winx64-64-downloads.html :
* Basic Package np. instantclient-basic-windows.x64-19.5.0.0.0dbru.zip
* SDK Package np.  instantclient-sdk-windows.x64-19.5.0.0.0dbru.zip
4. Konfiguracja bazy danych
Nale¿y pod³¹czyæ siê do bazy danych jak administartor (CONNECT SYS AS SYSDBA) wykorzystuj¹c SQL Developer lub SQL plus i wykonaæ:

ALTER SESSION SET "_ORACLE_SCRIPT"=true;
CREATE USER test IDENTIFIED BY test DEFAULT TABLESPACE USERS QUOTA 10M ON USERS TEMPORARY TABLESPACE TEMP;
GRANT CONNECT TO test;
GRANT CREATE SESSION TO test;
GRANT CREATE TABLE TO test;
GRANT CREATE VIEW TO test;
GRANT CREATE PROCEDURE TO test;
GRANT CREATE SEQUENCE TO test;
GRANT CREATE TRIGGER TO test;
DISCONNECT

Kompilacja:
W skrypcie makeOracle18cXE.bat nale¿y dostosowaæ ustawienia i go wykonaæ:

SETLOCAL 

REM Ustawienia
CALL "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64
SET ORACLE_SDK_HOME=C:\instantclient-sdk-windows.x64-18.5.0.0.0dbru\instantclient_18_5\sdk

REM Pliki naglowkowe
SET INCLUDE=%ORACLE_SDK_HOME%\include;%INCLUDE%

REM Kompilacja
cl -c /EHsc project.cpp -Fo:project.obj

REM Biblioteki statyczne
SET LIB=%ORACLE_SDK_HOME%\lib\msvc;%LIB%

REM Linkowanie
cl project.obj %ORACLE_SDK_HOME%\lib\msvc\oraocci18.lib -Fe:project.exe

ENDLOCAL  

Uruchomienie:
W skrypcie runOracle18cXE.bat nale¿y dostosowaæ ustawienia i go uruchomiæ:

SETLOCAL 

REM Ustawienie stalych
SET ORACLE_BASIC_HOME=C:\instantclient-basic-windows.x64-18.5.0.0.0dbru\instantclient_18_5

REM Biblioteki dynamiczne
SET PATH=%ORACLE_BASIC_HOME%;%PATH%

REM Uruchomienie
project.exe test test 192.168.0.14

ENDLOCAL

Typowe problemy:

1. B³êdy podczas kompilacji
CALL "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64 
System nie mo¿e odnaleŸæ okreœlonej œcie¿ki.
oraz 
project.cpp(1): fatal error C1083: Cannot open include file: 'iostream': No such file or directory
Prawdopodobnym powodem jest zainstalowanie œrodowiska Visual Studio w innym katalogu ni¿ domyœlny lub zainstalowanie innej wersji. Nale¿y sprawdziæ czy plik "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" istnieje i ewentualnie dostosowaæ œcie¿kê.

2. B³¹d podczas kompilacji
project.cpp(2): fatal error C1083: Cannot open include file: 'occi.h': No such file or directory
Prawdopodobnym powodem jest b³êdne ustawienie ORACLE_SDK_HOME. Nale¿y sprawdziæ czy istnieje katalog %ORACLE_SDK_HOME%\include i znajduje siê w nim plik occi.h, a nastêpnie ewentualnie dostosowaæ œcie¿kê.

3. B³¹d podczas linkowania
project.obj : error LNK2019: unresolved external symbol "public: static class oracle::occi::Environment 
...
warning LNK4272: library machine type 'x64' conflicts with target machine type 'X86'
project.exe : fatal error LNK1120: 4 unresolved externals
Prawdopodobnym powodem jest niezgodnoœæ pomiêdzy ustawieniami œrodowiska Visual Studio (32 bity) oraz bibliotek OCI Oracle (64 bity). Mo¿liwa jest te¿ odwrotna niezgodnoœæ: Visual Studio (64 bity) oraz bibliotek OCI Oracle (32 bity). Nale¿y sprawdziæ wersjê biblioteki OCI wykonuj¹c nastêpuj¹c¹ instrukcjê w katalogu gdzie znajduje siê ta biblioteka:
dumpbin oraocci18.lib /headers | findstr machine
Je¿eli wersja jest 32 bitowa, to z poni¿szej instrukcji w plik makeOracle18cXE.bat nale¿y usun¹æ "amd64":
CALL "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64 
Po zmianach powinno pozostaæ:
CALL "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat"

4. B³¹d podczas uruchomienia
"Nie mo¿na uruchomiæ programu, poniewa¿ na komputerze nie znaleziono oracocci18.dll. Spróbuj ponownie zainstalowaæ program, aby naprawiæ ten problem."
Prawdopodobnym powodem jest b³êdne ustawienie ORACLE_BASIC_HOME. Nale¿y sprawdziæ czy istnieje katalog %ORACLE_BASIC_HOME%\bin i znajduje siê w nim plik oracocci18.dll, a nastêpnie ewentualnie dostosowaæ œcie¿kê.

5. B³¹d podczas uruchomienia
Project executed not correctly
ORA-12560: TNS:protocol adapter error
Domyœlny sposób uruchomienia zak³ada, ¿e zainstalowana jest lokalnie:
project.exe test test 
Mo¿na sprawdziæ czy baza danych nas³uchuje na domyœlnym porcie 1521 wykonuj¹c instrukcjê:
netstat -abon | findstr "1521"
Je¿eli baza danych jest zainstalowana na innym komputerze nale¿y zmieniæ sposób uruchamiania, np. 
project.exe test test 192.168.0.1

6. B³¹d podczas uruchomienia
Project executed not correctly
ORA-01017: invalid username/password; logon denied
U¿ytkownik test nie istnieje w bazie danych Oracle. Nale¿y go utworzyæ zgodnie z instrukcj¹ znajduj¹ca siê na pocz¹tku tego pliku lub zmieniæ w pliku runOracle18cXE.bat nastêpuj¹c¹ instrukcjê:
project.exe test test
Po zmianach:
project.exe InnyLogin InneHas³o