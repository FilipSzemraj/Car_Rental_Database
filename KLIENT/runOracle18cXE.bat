SETLOCAL 

REM Ustawienie stalych
SET ORACLE_BASIC_HOME=C:\BAZY_DANYCH\instantClient_BASIC\instantclient_21_8

REM Biblioteki dynamiczne
SET PATH=%ORACLE_BASIC_HOME%;%PATH%

REM Uruchomienie
project.exe test test localhost:1521/xe

PAUSE

ENDLOCAL