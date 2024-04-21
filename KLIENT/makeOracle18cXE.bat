SETLOCAL 

REM Ustawienie stalych
CALL "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" amd64
SET ORACLE_SDK_HOME=C:\BAZY_DANYCH\instantClient_SDK\instantclient_21_8\sdk

REM Pliki naglowkowe
SET INCLUDE=%ORACLE_SDK_HOME%\include;%INCLUDE%

REM Kompilacja
cl -c /EHsc project.cpp -Fo:project.obj

REM Biblioteki statyczne
SET LIB=%ORACLE_SDK_HOME%\lib\msvc;%LIB%

REM Linkowanie
cl project.obj %ORACLE_SDK_HOME%\lib\msvc\oraocci21.lib -Fe:project.exe

PAUSE

ENDLOCAL