--Na podany temat nale¿y przygotowaæ nastêpuj¹ce skrypty (*.sql):
--1. Tworz¹cy co najmniej 7 - 10 procedur oraz funkcji zapewniaj¹cych pe³n¹ obs³ugê bazy danych.

SET SERVEROUTPUT ON;
/
--1. PROCEDURA DODAJACA OSOBE - pracownika, lub klienta do bazy danych.
CREATE OR REPLACE PROCEDURE dodaj_osobe(v_czy_klient NUMBER, v_imie VARCHAR2, v_nazwisko VARCHAR2, v_data_rej DATE, v_pesel VARCHAR2, v_telefon NUMBER)
IS
v_id_osoba osoba.Id_osoba%TYPE;
v_id_pracownika pracownik.Id_pracownika%TYPE;
v_id_klienta klient.Id_klienta%TYPE;
czy_klient_exception EXCEPTION;
BEGIN
--
    SELECT max(Id_osoba)+1
    INTO v_id_osoba
    FROM osoba;
--
    INSERT INTO osoba VALUES (v_id_osoba, v_imie, v_nazwisko, NULL);
--
IF v_czy_klient=1 THEN
--
    SELECT max(Id_klienta)+1
    INTO v_id_klienta
    FROM klient;
--
    INSERT INTO klient VALUES (v_id_klienta, v_id_osoba, v_data_rej, v_pesel, v_telefon);
--
ELSIF v_czy_klient=0 THEN
--
    SELECT max(Id_pracownika)+1
    INTO v_id_pracownika
    FROM pracownik;
--
    INSERT INTO pracownik VALUES (v_id_pracownika, v_id_osoba, v_pesel, v_data_rej, 1, v_telefon);
--
ELSE
--
    RAISE czy_klient_exception;
--
END IF;
EXCEPTION
    WHEN czy_klient_exception THEN
    DBMS_OUTPUT.ENABLE;
        DBMS_OUTPUT.PUT_LINE('BLAD:');
    DBMS_OUTPUT.PUT_LINE('Zmienna czy_klient powinna zawierac 1 lub 0');
    RETURN;
END;
/
SELECT * FROM klient;
/
EXECUTE dodaj_osobe(1, 'Michalina', 'Owczarzak', SYSDATE, '23435617823', 121456789);
/
SELECT * FROM klient;
/
SELECT * FROM pracownik;
/
EXECUTE dodaj_osobe(0, 'Robert', 'Orzechowski', SYSDATE, '23335797823', 113456789);
/
SELECT * FROM pracownik;
/
EXECUTE dodaj_osobe(5, 'Michal', 'Owczarzak', SYSDATE, '23435697823', 123456789);
/
--2. Dodawanie nowej uslugi
CREATE OR REPLACE PROCEDURE dodaj_nowa_usluge(v_id_pracownika NUMBER, v_id_klienta NUMBER, v_id_auta NUMBER)
IS
CURSOR pracownicy IS
    SELECT Id_pracownika
    FROM pracownik;
CURSOR klienci IS
    SELECT Id_klienta
    FROM klient;
CURSOR auta IS
    SELECT Id_auta
    FROM auta;
pracownik_exception EXCEPTION;
klient_exception EXCEPTION;
auto_exception EXCEPTION;
v_pr_ex_check NUMBER(1);
v_kl_ex_check NUMBER(1);
v_au_ex_check NUMBER(1);
v_id_uslugi uslugi.Id_uslugi%TYPE;
BEGIN
v_pr_ex_check:=0;
v_kl_ex_check:=0;
v_au_ex_check:=0;
FOR pr IN pracownicy LOOP
    IF pr.Id_pracownika=v_id_pracownika THEN
        DBMS_OUTPUT.ENABLE;
        DBMS_OUTPUT.PUT_LINE('MAMY TAKIEGO PRACOWNIKA');
        v_pr_ex_check:=1;
        FOR kl IN klienci LOOP
            IF kl.Id_klienta=v_id_klienta THEN
                DBMS_OUTPUT.ENABLE;
                DBMS_OUTPUT.PUT_LINE('MAMY TAKIEGO KLIENTA');
                v_kl_ex_check:=1;
                FOR au IN auta LOOP
                    IF au.Id_auta=v_id_auta THEN
                        DBMS_OUTPUT.ENABLE;
                        DBMS_OUTPUT.PUT_LINE('MAMY TAKIE AUTO');
                        v_au_ex_check:=1;
                    END IF;
                END LOOP;
            END IF;
        END LOOP;
    END IF;
END LOOP;

IF v_pr_ex_check!=1 THEN
    RAISE pracownik_exception;
END IF;

IF v_kl_ex_check!=1 THEN
    RAISE klient_exception;
END IF;

IF v_au_ex_check!=1 THEN
    RAISE auto_exception;
END IF;

SELECT max(id_uslugi)+1 INTO v_id_uslugi FROM uslugi;

INSERT INTO uslugi VALUES (v_id_uslugi, v_id_pracownika, v_id_klienta, v_id_auta);
DBMS_OUTPUT.ENABLE;
DBMS_OUTPUT.PUT_LINE('Dodano nowa usluge');

EXCEPTION
WHEN pracownik_exception THEN
    DBMS_OUTPUT.ENABLE;
    DBMS_OUTPUT.PUT_LINE('Nie ma takiego pracownika');
    RETURN;
WHEN klient_exception THEN
    DBMS_OUTPUT.ENABLE;
    DBMS_OUTPUT.PUT_LINE('Nie ma takiego klienta');
    RETURN;
WHEN auto_exception THEN
    DBMS_OUTPUT.ENABLE;
    DBMS_OUTPUT.PUT_LINE('Nie ma takiego auta');
    RETURN;
END;
/
EXECUTE dodaj_nowa_usluge(5,3,3);
/
SELECT * FROM uslugi;
/
--3. Dodanie nowej naprawy
CREATE OR REPLACE PROCEDURE dodaj_nowa_naprawe(v_id_uslugi NUMBER, v_koszt_naprawy NUMBER, v_rodzaj_naprawy VARCHAR2, v_data_odbioru DATE)
IS
v_id_naprawy naprawy.Id_naprawy%TYPE;
CURSOR wolne_uslugi IS
    SELECT id_uslugi FROM uslugi WHERE id_uslugi != ALL 
    (SELECT id_uslugi FROM naprawy UNION SELECT id_uslugi FROM wypozyczenia);
v_id_ex_check NUMBER(1);
id_uslug_exception EXCEPTION;
BEGIN
--
SELECT max(Id_naprawy)+1 INTO v_id_naprawy FROM naprawy;
v_id_ex_check:=0;
FOR wu IN wolne_uslugi LOOP
    IF wu.Id_uslugi = v_id_uslugi THEN
        v_id_ex_check:=1;
    END IF;
END LOOP;
--
IF v_id_ex_check != 1 THEN
    RAISE id_uslug_exception;
END IF;
--
INSERT INTO naprawy VALUES(v_id_naprawy, v_id_uslugi, v_koszt_naprawy, v_rodzaj_naprawy, SYSDATE, v_data_odbioru);
--
EXCEPTION
WHEN id_uslug_exception THEN
    DBMS_OUTPUT.ENABLE;
    DBMS_OUTPUT.PUT_LINE('Nie ma wolnej uslugi(bez przypisanego do niej wypozyczenia lub naprawy) o takim ID');
    RETURN;
END;
/
EXECUTE dodaj_nowa_naprawe(11,500,'Naprawa podwozia', to_date('23/11/22', 'DD/MM/YYYY'));
/
SELECT * FROM naprawy;
/

--4. Usuwajacy usluge o podanym ID, wraz z przypisan¹ do niej napraw¹ lub wypozyczeniem.

SELECT * FROM uslugi WHERE id_uslugi=3;
/
CREATE OR REPLACE PROCEDURE usun_usluge(v_id_uslugi NUMBER)
IS
BEGIN
DELETE FROM uslugi WHERE id_uslugi=v_id_uslugi;
END;
/
EXECUTE usun_usluge(2);
/
SELECT * FROM uslugi;
SELECT * FROM wypozyczenia;
/

--5. Procedura dodaj¹ca do tabeli osoba kolumne 'Plec', w której wartosci beda typu VARCHAR2 'Pan' lub 'Pani'

ALTER TABLE osoba
ADD Plec VARCHAR2(4);
/
CREATE OR REPLACE PROCEDURE dodaj_zaimek_do_osob
IS
CURSOR osobniczki IS 
    SELECT * 
    FROM osoba
    WHERE Imie LIKE '%a';
CURSOR osobnicy IS 
    SELECT * 
    FROM osoba
    WHERE Imie NOT LIKE '%a';
tmp osoba%ROWTYPE;
v_pani osoba.Plec%TYPE;
v_pan osoba.Plec%TYPE;
BEGIN
v_pani:='Pani';
v_pan:='Pan';
OPEN osobniczki;
LOOP
    FETCH osobniczki INTO tmp;
    UPDATE osoba
    SET Plec=v_pani
    WHERE Id_osoba=tmp.Id_osoba;

    EXIT WHEN osobniczki%NOTFOUND;
END LOOP;

CLOSE osobniczki;
OPEN osobnicy;

LOOP 
    FETCH osobnicy INTO tmp;
    UPDATE osoba
    SET Plec=v_pan
    WHERE Id_osoba=tmp.Id_osoba;
    
    EXIT WHEN osobnicy%NOTFOUND;
END LOOP;

CLOSE osobnicy;

END;
/
SELECT * FROM osoba;
/
CALL dodaj_zaimek_do_osob();
/
--6. PROCEDURA DODAJACA NOWE CENNIKI ORAZ UZUPELNIAJACE JE PSEUDOLOSOWYMI DANYMI
CREATE OR REPLACE PROCEDURE dodaj_losowe_cenniki(v_ilosc NUMBER)
IS
v_cena_za_h cennik.Cena_za_h%TYPE;
v_cena_za_km cennik.Cena_za_km%TYPE;
v_data_godzina_zmiany cennik.Data_godzina_zmiany%TYPE;
v_czy_aktualny cennik.Czy_aktualny%TYPE;
v_dotyczy_aut cennik.Dotyczy_aut%TYPE;
v_id_cennika cennik.Id_cennika%TYPE;
i NUMBER(1):=0;
BEGIN
SELECT max(Id_cennika)+1 INTO v_id_cennika FROM cennik;
WHILE i<v_ilosc LOOP
v_cena_za_h:=DBMS_RANDOM.VALUE(1,99999);
v_cena_za_km:=DBMS_RANDOM.VALUE(1,99999);
v_data_godzina_zmiany:=SYSDATE;
v_czy_aktualny:=0;
v_dotyczy_aut:=DBMS_RANDOM.STRING('a', 10);
v_id_cennika:=v_id_cennika+i;
INSERT INTO cennik VALUES (v_id_cennika, v_cena_za_h, v_cena_za_km, v_data_godzina_zmiany, v_czy_aktualny, v_dotyczy_aut);
i:=i+1;
END LOOP;
END;
/
SELECT * FROM cennik;
/
EXECUTE dodaj_losowe_cenniki(2);
/

-- 7. Zwraca Id_osoba, dla pracownika o najwiekszej ilosci uslug lub klienta zaleznie od parametru
CREATE OR REPLACE FUNCTION naj_osoba(v_czy_klient NUMBER)
RETURN NUMBER
IS
CURSOR maks_klient IS
    SELECT Id_osoba, count(Id_osoba) ilosc_uslug FROM klient k
    JOIN uslugi u ON u.Id_klienta = k.Id_klienta
    GROUP BY Id_osoba
    HAVING count(Id_osoba) = (SELECT MAX(COUNT(Id_osoba)) 
    FROM klient k
    JOIN uslugi u ON u.Id_klienta = k.Id_klienta
    GROUP BY Id_osoba);
CURSOR maks_prac IS
    SELECT Id_osoba, count(Id_osoba) ilosc_uslug FROM pracownik p
    JOIN uslugi u ON u.Id_pracownika = p.Id_pracownika
    GROUP BY Id_osoba
    HAVING count(Id_osoba) = (SELECT MAX(COUNT(Id_osoba)) 
    FROM pracownik p
    JOIN uslugi u ON u.Id_pracownika = p.Id_pracownika
    GROUP BY Id_osoba);
v_id_osoba osoba.Id_osoba%TYPE;
v_ilosc_uslug NUMBER;
zly_parametr EXCEPTION;
BEGIN
IF v_czy_klient=1 THEN
	OPEN maks_klient;
	FETCH maks_klient INTO v_id_osoba, v_ilosc_uslug;
	DBMS_OUTPUT.ENABLE;
	DBMS_OUTPUT.PUT_LINE('Klient o Id_osoba: ' || v_czy_klient || ' ma najwiecej uslug, czyli: ' || v_czy_klient);
	CLOSE maks_klient;
	RETURN v_id_osoba;
ELSIF v_czy_klient = 0 THEN
	OPEN maks_prac;
	FETCH maks_prac INTO v_id_osoba, v_ilosc_uslug;
	DBMS_OUTPUT.ENABLE;
	DBMS_OUTPUT.PUT_LINE('Pracownik o Id_osoba: ' || v_czy_klient || ' ma najwiecej uslug, czyli: ' || v_czy_klient);
	CLOSE maks_prac;
	RETURN v_id_osoba;
ELSE
    RAISE zly_parametr;
END IF;

EXCEPTION
WHEN zly_parametr THEN
    DBMS_OUTPUT.ENABLE;
    DBMS_OUTPUT.PUT_LINE('Parametr przy wywolaniu powinien byc rowny 1 lub 0');
    RETURN 0;
END;
/
SELECT naj_osoba(3) FROM DUAL;
/
SELECT naj_osoba(1) FROM DUAL;
/
SELECT naj_osoba(0) FROM DUAL;
--8. Dodawanie nowego wypozyczenia
CREATE OR REPLACE PROCEDURE dodaj_wypozyczenia(v_id_uslugi NUMBER, v_stan_przed NUMBER,v_kaucja NUMBER, v_uwagi VARCHAR2,v_id_cennika NUMBER)
IS
v_id_wypozyczenia wypozyczenia.Id_wypozyczenia%TYPE;
CURSOR wolna_usluga IS
    SELECT id_uslugi FROM uslugi WHERE id_uslugi != ALL
    (SELECT id_uslugi FROM naprawy UNION SELECT id_uslugi FROM wypozyczenia);
v_id_ex_check NUMBER(1);
id_uslug_exception EXCEPTION;
BEGIN
SELECT max(Id_wypozyczenia)+1 INTO v_id_wypozyczenia FROM wypozyczenia;
v_id_ex_check := 0;
FOR w IN wolna_usluga LOOP
    IF w.Id_uslugi = v_id_uslugi THEN
    v_id_ex_check := 1;
    END IF;
END LOOP;

IF v_id_ex_check != 1 THEN
    RAISE id_uslug_exception;
END IF;

INSERT INTO wypozyczenia VALUES(v_id_wypozyczenia,v_id_uslugi,v_stan_przed,TO_DATE('04-01-2022 16:00:00','DD-MM-YYYY HH24:MI:SS'),v_kaucja,v_uwagi,v_id_cennika);

EXCEPTION 
WHEN id_uslug_exception THEN
    DBMS_OUTPUT.ENABLE;
    DBMS_OUTPUT.PUT_LINE('Nie ma wolnej uslugi o takim ID');
    RETURN;
END;
/
SELECT * FROM wypozyczenia;
/
EXEC dodaj_wypozyczenia(11,12000,500,'Bmka na trzech kolach',5);
/
EXEC dodaj_wypozyczenia(12,12000,500,'Bmka na trzech kolach',5);
/
EXEC dodaj_wypozyczenia(13,12000,500,'Bmka na trzech kolach',5);
/
EXEC dodaj_wypozyczenia(14,12000,500,'Bmka na trzech kolach',5);
/
EXEC dodaj_wypozyczenia(15,12000,500,'Bmka na trzech kolach',5);
/
EXEC dodaj_wypozyczenia(16,12000,500,'Bmka na trzech kolach',5);
/
--9. Funkcja zwracajaca sumaryczna kwote eksploatacji aut Mercedes
CREATE OR REPLACE FUNCTION eksploatacja(v_marka VARCHAR2)
RETURN NUMBER
IS CURSOR kwota_eksploatacji IS
SELECT * FROM koszty_eksploatacji k 
JOIN auta a ON k.Id_auta = a.Id_auta
WHERE a.marka = v_marka;
suma NUMBER(7,2) := 0;
BEGIN
FOR tmp IN kwota_eksploatacji LOOP
    suma := suma + tmp.Koszt_naprawy+tmp.Koszt_wym_ogumienia+tmp.Koszt_wym_oleju+tmp.Koszt_wym_rozrzadu+tmp.Koszt_wym_hamulcow+tmp.Koszt_wym_filtrow;
END LOOP;
RETURN suma;
END;
/
SELECT eksploatacja AS Koszta_Eksploatacji_Auta_Mercedes FROM DUAL; 
/

--10. Wyswietlenie ilosci wypozyczen w kwartale w ktorym bylo ich najwiecej
CREATE OR REPLACE FUNCTION Ilosc_wyp
RETURN NUMBER
IS
kw_pierwszy NUMBER;
kw_drugi NUMBER;
kw_trzeci NUMBER;
kw_czwarty NUMBER;
najwiekszy NUMBER;
kw_kwartal VARCHAR2(25);
CURSOR miesiac IS 
SELECT TO_CHAR(Data_godzina_wypo,'MM') miesiac FROM wypozyczenia;
BEGIN
kw_pierwszy := 0;
kw_drugi := 0;
kw_trzeci := 0;
kw_czwarty := 0;
kw_kwartal := 'Pierwszy';
FOR tmp IN miesiac LOOP
    IF tmp.miesiac > 0 AND tmp.miesiac <4 THEN
        kw_pierwszy := kw_pierwszy+1;
    ELSIF tmp.miesiac > 3 AND tmp.miesiac <7 THEN
        kw_drugi := kw_drugi+1;
    ELSIF tmp.miesiac > 6 AND tmp.miesiac <10 THEN
        kw_trzeci := kw_trzeci+1;
    ELSE
        kw_czwarty := kw_czwarty+1;
    END IF;
END LOOP;
najwiekszy := kw_pierwszy;
IF kw_drugi>najwiekszy THEN
    najwiekszy := kw_drugi;
    kw_kwartal := 'Drugi';
ELSIF kw_trzeci > najwiekszy THEN
    najwiekszy := kw_trzeci;
    kw_kwartal := 'Trzeci';
ELSIF kw_czwarty > najwiekszy THEN
    najwiekszy := kw_czwarty;
    kw_kwartal := 'Czwarty';
END IF;
DBMS_OUTPUT.ENABLE;
DBMS_OUTPUT.PUT_LINE('Kwartal'||TO_CHAR(kw_kwartal));
RETURN najwiekszy;
END;
/
SELECT Ilosc_wyp FROM dual;
--2. Usuwaj¹cy procedury oraz funkcje.
DROP PROCEDURE dodaj_osobe;
DROP PROCEDURE dodaj_nowa_usluge;
DROP PROCEDURE dodaj_nowa_naprawe;
DROP PROCEDURE usun_usluge;
DROP PROCEDURE dodaj_zaimek_do_osob;
DROP PROCEDURE dodaj_losowe_cenniki;
DROP FUNCTION naj_osoba;
DROP PROCEDURE dodaj_wypozyczenia;
DROP PROCEDURE eksploatacja;
DROP FUNCTION Ilosc_wyp;
--3. Tworz¹cy co najmniej 5-10 wyzwalaczy.

--1.  Ograniczenie przebiegu dodawanego auta do bazy
CREATE OR REPLACE TRIGGER ograniczenie_przebieg
BEFORE INSERT ON uslugi
FOR EACH ROW
DECLARE 
v_przebieg auta.Przebieg%TYPE;
v_marka VARCHAR2(30);
BEGIN
SELECT Przebieg INTO v_przebieg FROM auta
WHERE Id_auta = :NEW.Id_auta;
IF (v_przebieg>100000) THEN
    SELECT marka INTO v_marka FROM auta 
    WHERE Id_auta = :NEW.Id_auta;
    RAISE_APPLICATION_ERROR(-20000,'Przebieg auta '  v_marka  ' jest zbyt duzy');
END IF;
END;
/
ALTER TRIGGER ograniczenie_przebieg ENABLE;
/
INSERT INTO uslugi VALUES(17,3,3,3);
/
DROP TRIGGER ograniczenie_przebieg;
/
SELECT  * FROM uslugi;
/
--2. Ograniczenie dodania nowej osoby, jesli w imieniu lub nazwisku wystepuja cyfry
CREATE OR REPLACE TRIGGER ograniczenie_nowej_osoby
BEFORE INSERT ON osoba
FOR EACH ROW
DECLARE
BEGIN
IF :NEW.Imie LIKE '%0%'
    OR :NEW.Imie Like '%1%'
    OR :NEW.Imie Like '%2%'
    OR :NEW.Imie Like '%3%'
    OR :NEW.Imie Like '%4%'
    OR :NEW.Imie Like '%5%'
    OR :NEW.Imie Like '%6%'
    OR :NEW.Imie Like '%7%'
    OR :NEW.Imie Like '%8%'
    OR :NEW.Imie Like '%9%'
THEN
    RAISE_APPLICATION_ERROR(-20000,'Imie nie moze zawierac cyfr!');
END IF;

IF :NEW.Nazwisko LIKE '%0%'
    OR :NEW.Nazwisko Like '%1%'
    OR :NEW.Nazwisko Like '%2%'
    OR :NEW.Nazwisko Like '%3%'
    OR :NEW.Nazwisko Like '%4%'
    OR :NEW.Nazwisko Like '%5%'
    OR :NEW.Nazwisko Like '%6%'
    OR :NEW.Nazwisko Like '%7%'
    OR :NEW.Nazwisko Like '%8%'
    OR :NEW.Nazwisko Like '%9%'
THEN
    RAISE_APPLICATION_ERROR(-20000,'Nazwisko nie moze zawierac cyfr!');
END IF;

END;
/
ALTER TRIGGER ograniczenie_nowej_osoby ENABLE;
/
SELECT * FROM osoba;
/
INSERT INTO osoba VALUES (12, 'Filip', 'S2zemraj', NULL);
/
--3. Trigger ktory nie pozwala dodac pracownika do uslugi jezeli jest nieobecny
INSERT INTO osoba VALUES
('12', 'KKKK', 'CCCC', NULL)
/
INSERT INTO pracownik 
VALUES(6,11,0870934000,TO_DATE('23-01-2023 12:00:00','DD-MM-YYYY HH24:MI:SS'),0,599000767)
/
CREATE OR REPLACE TRIGGER pracownik_obecny
BEFORE INSERT ON uslugi
FOR EACH ROW
DECLARE
v_obecny pracownik.Czy_obecny%TYPE;
v_imie VARCHAR2(28);
BEGIN
SELECT Czy_obecny INTO v_obecny FROM pracownik
WHERE Id_pracownika = :NEW.Id_pracownika;
IF (v_obecny = 0) THEN
    SELECT o.imie INTO v_imie FROM osoba o JOIN pracownik p 
    ON o.Id_osoba = p.Id_osoba
    WHERE p.Id_pracownika = :NEW.Id_pracownika; 
    RAISE_APPLICATION_ERROR(-20000, 'Pracownik '||v_imie||' jest nieobecny');
END IF;
END;
/
ALTER TRIGGER pracownik_obecny ENABLE;
/
INSERT INTO uslugi VALUES(11,6,1,1);
/
SELECT * FROM pracownik;

--4. Nie pozwala na dodanie auta do wypozyczen jesli jest nie sprawne
/
SELECT Czy_sprawny FROM auta a
JOIN uslugi u ON u.Id_auta = a.Id_auta
WHERE u.Id_uslugi = 11;
/
CREATE OR REPLACE TRIGGER auto_sprawne
BEFORE INSERT ON wypozyczenia
FOR EACH ROW
DECLARE
v_sprawny auta.Czy_sprawny%TYPE;
v_model auta.Model%TYPE;
BEGIN
SELECT Czy_Sprawny INTO v_sprawny FROM auta a
JOIN uslugi u ON u.Id_auta = a.Id_auta
WHERE u.Id_uslugi = :NEW.Id_uslugi;
IF v_sprawny=0 THEN
    SELECT Model INTO v_model FROM auta a
    JOIN uslugi u ON u.Id_auta = a.Id_auta
    WHERE u.Id_uslugi = :NEW.Id_uslugi;
    RAISE_APPLICATION_ERROR(-20000, 'Samochod : '|| v_model || ' jest nie sprawny, wiec nie moze zostac uzyty do wypozyczenia');
END IF;
END;
/
ALTER TRIGGER auto_sprawne ENABLE;
/
INSERT INTO uslugi VALUES(11,2,1,1);
/
UPDATE auta SET Czy_sprawny=0
WHERE Id_auta=1;
/
SELECT * FROM auta;
/
SELECT * FROM uslugi;
/
SELECT * FROM wypozyczenia;
/
INSERT INTO wypozyczenia 
VALUES(6,11,75600,TO_DATE('06-12-2022 16:00:00','DD-MM-YYYY HH24:MI:SS'),90,'Brak',4)
/
--5. Trigger nie pozwalajacy dodac naprawy w ktorej data odbioru jest mniejszy od daty przyjecia
CREATE OR REPLACE TRIGGER data_odbioru
BEFORE INSERT ON naprawy
FOR EACH ROW
DECLARE
BEGIN
IF(:NEW.Data_odbioru < :NEW.Data_przyjecia) THEN
RAISE_APPLICATION_ERROR(-20000, 'Nie mozna dodac naprawy poniewaz data odbioru jest wczesniejsza od daty przyjecia');
END IF;
END;
/
ALTER TRIGGER data_odbioru ENABLE;
/
INSERT INTO naprawy VALUES(6,11, 500, 'Naprawa nadwozia', TO_DATE('06-15-2022', 'MM-DD-YYYY'), TO_DATE('01-24-2021', 'MM-DD-YYYY'));

--DEKLARACJA DO PACZKI Z TRIGGEROW
    TRIGGER ograniczenie_przebieg;
    TRIGGER ograniczenie_nowej_osoby;
    TRIGGER pracownik_obecny;
    TRIGGER auto_sprawne;
    TRIGGER data_odbioru;
-----------------------------------------
--4. Usuwaj¹cy wyzwalacze.