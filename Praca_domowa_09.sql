--1. Tworz¹cy po jednej sekwencji dla ka¿dego klucza g³ównego.

CREATE SEQUENCE id_osoba_seq;
CREATE SEQUENCE id_klienta_seq;
CREATE SEQUENCE id_pracownika_seq;
CREATE SEQUENCE id_naprawy_seq;
CREATE SEQUENCE id_auta_seq;
CREATE SEQUENCE id_uslugi_seq;
CREATE SEQUENCE id_zwrotu_seq;
CREATE SEQUENCE id_wypozyczenia_seq;
CREATE SEQUENCE id_eksploatacji_seq;
CREATE SEQUENCE id_cennik_seq;

--2. Usuwaj¹cy wszystkie sekwencje.
DROP SEQUENCE id_osoba_seq;
DROP SEQUENCE id_klienta_seq;
DROP SEQUENCE id_pracownika_seq;
DROP SEQUENCE id_naprawy_seq;
DROP SEQUENCE id_auta_seq;
DROP SEQUENCE id_uslugi_seq;
DROP SEQUENCE id_zwrotu_seq;
DROP SEQUENCE id_wypozyczenia_seq;
DROP SEQUENCE id_eksploatacji_seq;
DROP SEQUENCE id_cennik_seq;

--3. Zawieraj¹cy co najmniej 5-10 poleceñ INSERT dla ka¿dej tabeli.
--OSOBNY PLIK INSERT.sql

--4. Zawieraj¹cy po jednej instrukcji INSERT naruszaj¹cej ka¿dy z typów ograniczeñ 
--(np. UNIQUE, NOT NULL, CHECK, klucza g³ównego, klucza obcego, itp) dla dowolnej tabeli.

--ZLY FORMAT DATY
INSERT INTO pracownik VALUES
(1, 1, 12343245691, TO_DATE('DD-MM-YYYY', '23-12-1999'), 1, 733359555);

--NARUSZONO UNIQUE KLUCZA G£ÓWNEGO
INSERT INTO pracownik VALUES
(1, 1, 12343245691, TO_DATE('12-12-2022 12:00:00','DD-MM-YYYY HH24:MI:SS'), 1, 733359555);

--NARUSZONO WIÊZY SPÓJNOŒCI - KLUCZ OBCY
INSERT INTO pracownik VALUES
(6, 12, 12343245691, TO_DATE('12-12-2022 12:00:00','DD-MM-YYYY HH24:MI:SS'), 1, 733359555);

--NARUSZONO UNIQUE - NUMER TELEFONU
INSERT INTO pracownik VALUES
(6, 7, 12343245691, TO_DATE('12-12-2022 12:00:00','DD-MM-YYYY HH24:MI:SS'), 1, 733359559);

--NARUSZONO NOT NULL
INSERT INTO naprawy VALUES
(6, NULL, 500, 'Przykladowa naprawa', TO_DATE('MM-DD-YYYY', '10-15-2002'), TO_DATE('MM-DD-YYYY', '10-20-2002'));

--NARUSZONO CHECK
INSERT INTO cennik VALUES
(6, 50,23,TO_DATE('09-03-2020','DD-MM-YYYY'), 4, 'Przykladowe auta');

-- INSERTY naruszajace ograniczenia
INSERT INTO zwroty VALUES
(5,5,5, TO_DATE('11-12-2022 12:00:00','DD-MM-YYYY HH24:MI:SS'), 120, 'Poplamiona tapicerka');

INSERT INTO naprawy VALUES
(6, NULL, 500, 'Naprawa nadwozia', TO_DATE('06-15-2022', 'MM-DD-YYYY'), TO_DATE('06-24-2022', 'MM-DD-YYYY'));

INSERT INTO cennik 
VALUES(6,20,4,TO_DATE('09-02-2020','DD-MM-YYYY'),3,'Opel');

--5. Modyfikuj¹cy dowolnie wybran¹ tabelê: 
-- * dodaj¹cy kolumnê (np. test VARCHAR2(20)),
ALTER TABLE pracownik
ADD test VARCHAR2(20);

-- * dodaj¹cy ograniczenia do dodanej kolumny (np. NOT NULL),
ALTER TABLE pracownik
MODIFY test CONSTRAINT test_u UNIQUE;

-- * usuwaj¹cy ograniczenia dotycz¹ce dodanej kolumny,
ALTER TABLE pracownik
DROP CONSTRAINT test_u;

-- * usuwaj¹cy dodan¹ kolumnê.
ALTER TABLE pracownik
DROP COLUMN test;

--6. Usuwaj¹cy wszystkie wiersze ze wszystkich tabel.
--OSOBNY PLIK DELETE.sql

--7. Zawieraj¹cy co najmniej 10 – 15 zaawansowanych zapytañ / widoków wykorzystuj¹cych np.:

-- * Wyswietlanie pracownikow z imienia i nazwiska, oraz ilosci uslug, w których brali udzial
CREATE VIEW Ilosc_uslug_wg_pracownika AS
SELECT Ilosc_uslug, p.Id_pracownika numer_pracownika, o.imie imie, o.nazwisko nazwisko
FROM pracownik p
JOIN osoba o ON o.Id_osoba = p.Id_osoba
JOIN (
    SELECT COUNT(u.Id_uslugi) Ilosc_uslug, u.Id_pracownika widp
    FROM uslugi u
    GROUP BY u.Id_pracownika
) ON p.Id_pracownika = widp
ORDER BY p.Id_pracownika
WITH READ ONLY;

SELECT * FROM Ilosc_uslug_wg_pracownika;

DROP VIEW Ilosc_uslug_wg_pracownika;

-- * Imie i nazwisko klienta, ktory ma wiecej uslug niz srednia.
CREATE VIEW Klient_premium AS
SELECT u.Id_klienta numer_klienta, COUNT(u.Id_uslugi) Ilosc_uslug, imie, nazwisko
FROM uslugi u
JOIN 
    (
    SELECT o.imie imie, o.nazwisko nazwisko, k.Id_klienta widk
    FROM klient k
    JOIN osoba o ON o.Id_osoba = k.Id_osoba
    ) ON u.Id_klienta = widk
GROUP BY u.Id_klienta, imie, nazwisko
HAVING COUNT(u.Id_uslugi) > 
    (
    SELECT COUNT(u.Id_uslugi)
    FROM uslugi u
    )/
    (
    SELECT COUNT(k.Id_klienta)
    FROM klient k
    WHERE EXISTS 
        (
            SELECT 1
            FROM uslugi u
            WHERE u.Id_klienta = k.Id_klienta
        )
    )    
ORDER BY u.Id_klienta
WITH READ ONLY;

SELECT * FROM Klient_premium;

DROP VIEW Klient_premium;

-- * Wyœwietlanie pracowników, ktorzy nie mieli jeszcze zadnej uslugi.
CREATE VIEW Pracownicy_bez_uslug AS
SELECT o.imie, o.nazwisko
FROM osoba o
JOIN pracownik p ON p.Id_osoba = o.Id_osoba
WHERE p.Id_pracownika = (SELECT p.Id_pracownika FROM pracownik p
GROUP BY p.Id_pracownika
MINUS
SELECT u.Id_pracownika FROM uslugi u
GROUP BY u.Id_pracownika)
WITH READ ONLY;

SELECT * FROM Pracownicy_bez_uslug;

DROP VIEW Pracownicy_bez_uslug;

--Wszystkie PESELE(osoby) w bazie danych.
CREATE VIEW Wszystkie_osoby_w_bazie AS
SELECT * FROM (
SELECT k.PESEL pesel, k.Id_osoba id, o.imie, o.nazwisko FROM klient k
JOIN osoba o ON o.Id_osoba = k.Id_osoba
UNION ALL
SELECT p.PESEL pesel, p.Id_osoba id, o.imie, o.nazwisko FROM pracownik p
JOIN osoba o ON o.Id_osoba = p.Id_osoba
)
ORDER BY id
WITH READ ONLY;

SELECT * FROM Wszystkie_osoby_w_bazie;

DROP VIEW Wszystkie_osoby_w_bazie;

-- Szczegolowe dane dla wszystkich wypozyczen
CREATE VIEW Szczegolowe_dane_wypozyczen AS
SELECT u.Id_uslugi ID, a.Nr_rejestracyjny NR_REJ, imie_pracownika, nazwisko_pracownika, imie_klienta, nazwisko_klienta
FROM uslugi u
LEFT JOIN 
    (
    SELECT o.imie imie_pracownika, o.nazwisko nazwisko_pracownika, p.Id_pracownika widp
    FROM osoba o
    JOIN pracownik p ON p.Id_osoba = o.Id_osoba
    ) ON u.Id_pracownika = widp
JOIN 
    (
    SELECT o.imie imie_klienta, o.nazwisko nazwisko_klienta, k.Id_klienta widk
    FROM osoba o
    JOIN klient k ON k.Id_osoba = o.Id_osoba
    ) ON u.Id_klienta = widk
JOIN auta a ON a.Id_auta = u.Id_auta
WHERE u.Id_uslugi = ANY(
    SELECT w.Id_uslugi FROM wypozyczenia w
    INTERSECT
    SELECT u.Id_uslugi FROM uslugi u
    )
    ORDER BY u.Id_uslugi
    WITH READ ONLY;
    
SELECT * FROM Szczegolowe_dane_wypozyczen;

DROP VIEW Szczegolowe_dane_wypozyczen;

--Klient, ktory ma najwiecej napraw w warsztacie
CREATE VIEW Klient_z_najwieksza_liczba_napraw AS
SELECT u.Id_klienta ID, ilosc ilosc_napraw, o.imie imie, o.nazwisko nazwisko
FROM uslugi u
JOIN klient k ON u.Id_klienta = k.Id_klienta
JOIN osoba o ON o.Id_osoba = k.Id_osoba
JOIN
    (
    SELECT COUNT(n.Id_naprawy) ilosc, u.Id_klienta widk
    FROM uslugi u
    JOIN naprawy n ON n.Id_uslugi = u.Id_uslugi
    GROUP BY u.Id_klienta
    HAVING COUNT(n.Id_naprawy) = 
        (
        SELECT MAX(COUNT(u.Id_uslugi))
        FROM uslugi u
        JOIN naprawy n ON n.Id_uslugi = u.Id_uslugi
        GROUP BY u.Id_klienta
        )
    ) ON u.Id_klienta = widk
GROUP BY u.Id_klienta, ilosc, o.imie, o.nazwisko
WITH READ ONLY;

SELECT * FROM Klient_z_najwieksza_liczba_napraw;

DROP VIEW Klient_z_najwieksza_liczba_napraw;


/*SELECT o.Imie, o.Nazwisko, ilosc
FROM osoba o 
JOIN(
SELECT k.Id_osoba kid, COUNT(n.Id_naprawy) ilosc
    FROM klient k
    JOIN uslugi u ON k.Id_klienta = u.Id_klienta
    JOIN naprawy n ON u.Id_uslugi = n.Id_uslugi
    GROUP BY k.Id_osoba
    HAVING COUNT(n.Id_naprawy) = (
        SELECT MAX(COUNT(n.Id_naprawy)) 
        FROM naprawy n
        JOIN uslugi u ON n.Id_uslugi = u.Id_uslugi
        JOIN klient k ON u.Id_klienta = k.Id_klienta
        JOIN osoba o ON k.Id_osoba = o.Id_osoba
        GROUP BY k.Id_osoba
    ))ON o.Id_osoba = kid;*/
--Wyœwietl informacje o wypozyczeniach dla auta o marce Mercedes
SELECT w.Stan_licznika_przed, w.Data_godzina_wypo, w.Kaucja, w.Uwagi 
FROM wypozyczenia w 
JOIN cennik ON w.Id_cennika = cennik.Id_cennika
WHERE cennik.dotyczy_aut = 'Mercedes';

--Wyœwietl informacje o autach, które byly w naprawie
SELECT a.Nr_rejestracyjny,a.Czy_sprawny, a.Przebieg, a.Nr_vin, a.Moc_silnika, a.Marka,a.Model
FROM auta a
JOIN uslugi u ON a.Id_auta = u.Id_auta
JOIN naprawy n ON n.Id_uslugi = u.Id_uslugi;

--Wyœwietl dane klienta, ktory mial najdluzszy okres wypozyczenia.
SELECT o.Imie, o.nazwisko, (z.data_godzina_zwrot-w.data_godzina_wypo)*24 okres_wypozyczenia_w_H
FROM osoba o
JOIN klient k ON o.Id_osoba = k.Id_osoba
JOIN uslugi u ON k.Id_klienta = u.Id_klienta
JOIN wypozyczenia w ON u.Id_uslugi = w.Id_uslugi
JOIN zwroty z ON w.Id_wypozyczenia = z.Id_wypozyczenia
WHERE (z.data_godzina_zwrot-w.data_godzina_wypo) = (
    SELECT MAX(z.data_godzina_zwrot-w.data_godzina_wypo)
    FROM wypozyczenia w JOIN zwroty z ON w.Id_wypozyczenia = z.Id_wypozyczenia
);

--Wyœwietl dane auta, ktore mialo najwiekszy koszt naprawy
SELECT a.Nr_rejestracyjny, a.Przebieg, a.Nr_vin, a.Moc_silnika, a.Rodzaj_paliwa, 
a.Rodzaj_nadwozia, a.Marka, a.Model, a.Data_produkcji, koszt
FROM auta a
JOIN(
    SELECT u.Id_auta aid, MAX(n.Koszt_naprawy) koszt
    FROM uslugi u
    JOIN naprawy n ON u.Id_uslugi = n.Id_uslugi
    WHERE (n.Koszt_naprawy) = (
        SELECT MAX(koszt_naprawy) FROM naprawy
    )
    GROUP BY u.Id_auta
) ON a.Id_auta = aid;

--Wyœwietl dane klienta i auta dla ostatniego wypozyczenia
SELECT o.Imie, o.Nazwisko, a.Nr_rejestracyjny, a.Przebieg, a.Marka, a.Model, a.Data_produkcji
FROM osoba o 
JOIN klient k ON o.Id_osoba = k.Id_osoba
JOIN uslugi u ON k.Id_klienta = u.Id_klienta
JOIN auta a ON u.Id_auta = a.Id_auta
JOIN wypozyczenia w ON w.Id_uslugi = u.Id_uslugi
AND w.data_godzina_wypo = (
    SELECT MAX(data_godzina_wypo) FROM wypozyczenia
);
--RAZEM 11