CREATE TABLE osoba
(
Id_osoba NUMBER(3) CONSTRAINT osoba_pkk PRIMARY KEY ,
Imie VARCHAR2(23),
Nazwisko VARCHAR2(30)
);
CREATE TABLE pracownik
(
Id_pracownika NUMBER(3) CONSTRAINT pracownik_pkk PRIMARY KEY,
Id_osoba NUMBER(3) CONSTRAINT Pracownik_Osoba_FKF REFERENCES osoba(Id_osoba) ON DELETE CASCADE,
PESEL VARCHAR2(11),
Data_zatrudnienia DATE,
Czy_obecny number(1) CONSTRAINT co_check_ischk CHECK (Czy_obecny IN(1,0)),
Telefon_komorkowy NUMBER(9) UNIQUE
);
CREATE TABLE klient
(
Id_klienta NUMBER(3) CONSTRAINT klient_pkk PRIMARY KEY,
Id_osoba NUMBER(3) CONSTRAINT Klient_Osoba_FKK REFERENCES osoba(Id_osoba) ON DELETE CASCADE,
Data_rejestracji DATE,
PESEL VARCHAR2(11),
Telefon_k NUMBER(9)
);
CREATE TABLE auta
(
Id_auta NUMBER(3) CONSTRAINT auta_pkk PRIMARY KEY,
Nr_rejestracyjny VARCHAR(8),
Czy_sprawny NUMBER(1) CONSTRAINT cs_check_ischkk CHECK (Czy_sprawny IN(1,0)),
Przebieg NUMBER(7),
Nr_vin VARCHAR2(32),
Moc_silnika NUMBER(4),
Rodzaj_paliwa VARCHAR2(11),
Rodzaj_nadwozia VARCHAR2(9),
Marka VARCHAR2(35),
Model VARCHAR2(35),
Data_produkcji DATE,
Uwagi VARCHAR2(255)
);
CREATE TABLE uslugi
(
Id_uslugi NUMBER(3) CONSTRAINT uslugi_pkk PRIMARY KEY,
Id_pracownika NUMBER(3) REFERENCES pracownik(Id_pracownika) ON DELETE CASCADE,
Id_klienta NUMBER(3) REFERENCES klient(Id_klienta) ON DELETE CASCADE,
Id_auta NUMBER(3) REFERENCES auta(Id_auta) ON DELETE CASCADE
);
CREATE TABLE naprawy
(
Id_naprawy NUMBER(3) CONSTRAINT naprawy_pkk PRIMARY KEY,
Id_uslugi NUMBER(3) NOT NULL CONSTRAINT Naprawy_Uslugi_FKK REFERENCES uslugi(Id_uslugi) ON DELETE CASCADE,
Koszt_naprawy NUMBER(5),
Rodzaj_naprawy VARCHAR2(35),
Data_przyjecia DATE,
Data_odbioru DATE
);

CREATE TABLE cennik
(
Id_cennika NUMBER(3) CONSTRAINT cennik_pkk PRIMARY KEY,
Cena_za_h NUMBER(5),
Cena_za_km NUMBER(5),
Data_godzina_zmiany DATE,
Czy_aktualny NUMBER(1) CONSTRAINT ca_check_ischkk CHECK (Czy_aktualny IN(1,0)),
Dotyczy_aut VARCHAR2(255)
);

CREATE TABLE koszty_eksploatacji
(
Id_eksploatacji NUMBER(3) CONSTRAINT koszty_eksploatacji_pkk PRIMARY KEY,
Id_auta NUMBER(3) REFERENCES auta(Id_auta) ON DELETE CASCADE,
Koszt_naprawy NUMBER(5),
Koszt_wym_ogumienia NUMBER(5),
Koszt_wym_oleju NUMBER(5),
Koszt_wym_rozrzadu NUMBER(5),
Koszt_wym_hamulcow NUMBER(5),
Koszt_wym_filtrow NUMBER(5),
Inne VARCHAR2(255)
);

CREATE TABLE wypozyczenia
(
Id_wypozyczenia NUMBER(3) CONSTRAINT wypozyczenia_pkk PRIMARY KEY,
Id_uslugi NUMBER(3) NOT NULL CONSTRAINT Wypozyczenia_Uslugi_FKK REFERENCES uslugi(Id_uslugi) ON DELETE CASCADE,
Stan_licznika_przed NUMBER(7),
Data_godzina_wypo DATE,
Kaucja NUMBER(4),
Uwagi VARCHAR2(255),
Id_cennika NUMBER(3) REFERENCES cennik(Id_cennika) ON DELETE CASCADE
);

CREATE TABLE zwroty
(
Id_zwrotu NUMBER(3) CONSTRAINT zwroty_pkk PRIMARY KEY,
Id_wypozyczenia NUMBER(3) REFERENCES wypozyczenia(Id_wypozyczenia) ON DELETE CASCADE,
Id_pracownik NUMBER(3) REFERENCES pracownik(Id_pracownika) ON DELETE CASCADE,
Data_godzina_zwrot DATE,
Stan_licznika_po NUMBER(7),
Uwagi VARCHAR2(255)
);