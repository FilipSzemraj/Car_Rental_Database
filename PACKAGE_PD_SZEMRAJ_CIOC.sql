CREATE OR REPLACE PACKAGE obsluga_bazy
AS
    PROCEDURE dodaj_osobe(v_czy_klient NUMBER, v_imie VARCHAR2, v_nazwisko VARCHAR2, v_data_rej DATE, v_pesel VARCHAR2, v_telefon NUMBER);
    PROCEDURE dodaj_nowa_usluge(v_id_pracownika NUMBER, v_id_klienta NUMBER, v_id_auta NUMBER);
    PROCEDURE dodaj_nowa_naprawe(v_id_uslugi NUMBER, v_koszt_naprawy NUMBER, v_rodzaj_naprawy VARCHAR2, v_data_odbioru DATE);
    PROCEDURE usun_usluge(v_id_uslugi NUMBER);
    PROCEDURE dodaj_zaimek_do_osob;
    PROCEDURE dodaj_losowe_cenniki(v_ilosc NUMBER);
    FUNCTION naj_osoba(v_czy_klient NUMBER) RETURN NUMBER;
    PROCEDURE dodaj_wypozyczenia(v_id_uslugi NUMBER, v_stan_przed NUMBER,v_kaucja NUMBER, v_uwagi VARCHAR2,v_id_cennika NUMBER);
    FUNCTION eksploatacja RETURN NUMBER;
    FUNCTION Ilosc_wyp RETURN NUMBER;
END obsluga_bazy;
/
CREATE OR REPLACE PACKAGE BODY obsluga_bazy
AS
    PROCEDURE dodaj_osobe(v_czy_klient NUMBER, v_imie VARCHAR2, v_nazwisko VARCHAR2, v_data_rej DATE, v_pesel VARCHAR2, v_telefon NUMBER)
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
    PROCEDURE dodaj_nowa_usluge(v_id_pracownika NUMBER, v_id_klienta NUMBER, v_id_auta NUMBER)
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
    PROCEDURE dodaj_nowa_naprawe(v_id_uslugi NUMBER, v_koszt_naprawy NUMBER, v_rodzaj_naprawy VARCHAR2, v_data_odbioru DATE)
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
    PROCEDURE usun_usluge(v_id_uslugi NUMBER)
    IS
BEGIN
DELETE FROM uslugi WHERE id_uslugi=v_id_uslugi;
END;
    PROCEDURE dodaj_zaimek_do_osob
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
    PROCEDURE dodaj_losowe_cenniki(v_ilosc NUMBER)
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
    FUNCTION naj_osoba(v_czy_klient NUMBER) RETURN NUMBER
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
    PROCEDURE dodaj_wypozyczenia(v_id_uslugi NUMBER, v_stan_przed NUMBER,v_kaucja NUMBER, v_uwagi VARCHAR2,v_id_cennika NUMBER)
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
    FUNCTION eksploatacja RETURN NUMBER
    IS CURSOR kwota_eksploatacji IS
SELECT * FROM koszty_eksploatacji k 
JOIN auta a ON k.Id_auta = a.Id_auta
WHERE a.marka = 'Mercedes';
suma NUMBER(7,2) := 0;
BEGIN
FOR tmp IN kwota_eksploatacji LOOP
    suma := suma + tmp.Koszt_naprawy+tmp.Koszt_wym_ogumienia+tmp.Koszt_wym_oleju+tmp.Koszt_wym_rozrzadu+tmp.Koszt_wym_hamulcow+tmp.Koszt_wym_filtrow;
END LOOP;
RETURN suma;
END;
    FUNCTION Ilosc_wyp RETURN NUMBER
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
END obsluga_bazy;
    