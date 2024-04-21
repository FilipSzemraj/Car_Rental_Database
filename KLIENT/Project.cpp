#include <stdlib.h>
#include <iostream>
#include <occi.h>
using namespace oracle::occi;
using namespace std;

class  Project
{
private:
	Environment *env;
	Connection *conn;
	Statement *stmt;
public:
	Project(string user, string passwd, string db)
	{
		env = Environment::createEnvironment(Environment::DEFAULT);
		conn = env->createConnection(user, passwd, db);
	}

	~Project()
	{
		env->terminateConnection(conn);
		Environment::terminateEnvironment(env);
	}

	void executeStatement(int stmc, string stms[])
	{
		for (int i = 0; i < stmc; i++)
		{
			stmt = conn->createStatement(stms[i]);
			try {
				stmt->executeUpdate();
				cout << "Statement \"" << stms[i].substr(0, stms[i].find("(")) << "\" executed correctly" << endl;
			}
			catch (SQLException ex)
			{
				cout << "Statement \"" << stms[i].substr(0, stms[i].find("(")) << "\" executed not correctly" << endl;
				cout << ex.getMessage() << endl;
			}
			conn->terminateStatement(stmt);
		}
	}

	void createAllTables()
	{
		string creates[] =
		{
			"CREATE TABLE osoba\
			(\
			Id_osoba NUMBER(3) CONSTRAINT osoba_pkk PRIMARY KEY ,\
			Imie VARCHAR2(23),\
			Nazwisko VARCHAR2(30),\
			Plec VARCHAR2(4)\
			)",
			
			"CREATE TABLE pracownik\
			(\
			Id_pracownika NUMBER(3) CONSTRAINT pracownik_pkk PRIMARY KEY,\
			Id_osoba NUMBER(3) CONSTRAINT Pracownik_Osoba_FKF REFERENCES osoba(Id_osoba) ON DELETE CASCADE,\
			PESEL VARCHAR2(11),\
			Data_zatrudnienia DATE,\
			Czy_obecny number(1) CONSTRAINT co_check_ischk CHECK (Czy_obecny IN(1,0)),\
			Telefon_komorkowy NUMBER(9) UNIQUE\
			)",
			"CREATE TABLE klient\
			(\
			Id_klienta NUMBER(3) CONSTRAINT klient_pkk PRIMARY KEY,\
			Id_osoba NUMBER(3) CONSTRAINT Klient_Osoba_FKK REFERENCES osoba(Id_osoba) ON DELETE CASCADE,\
			Data_rejestracji DATE,\
			PESEL VARCHAR2(11),\
			Telefon_k NUMBER(9)\
			)",
			"CREATE TABLE auta\
			(\
			Id_auta NUMBER(3) CONSTRAINT auta_pkk PRIMARY KEY,\
			Nr_rejestracyjny VARCHAR(8),\
			Czy_sprawny NUMBER(1) CONSTRAINT cs_check_ischkk CHECK (Czy_sprawny IN(1,0)),\
			Przebieg NUMBER(7),\
			Nr_vin VARCHAR2(32),\
			Moc_silnika NUMBER(4),\
			Rodzaj_paliwa VARCHAR2(11),\
			Rodzaj_nadwozia VARCHAR2(9),\
			Marka VARCHAR2(35),\
			Model VARCHAR2(35),\
			Data_produkcji DATE,\
			Uwagi VARCHAR2(255)\
			)",
			"CREATE TABLE uslugi\
			(\
			Id_uslugi NUMBER(3) CONSTRAINT uslugi_pkk PRIMARY KEY,\
			Id_pracownika NUMBER(3) REFERENCES pracownik(Id_pracownika) ON DELETE CASCADE,\
			Id_klienta NUMBER(3) REFERENCES klient(Id_klienta) ON DELETE CASCADE,\
			Id_auta NUMBER(3) REFERENCES auta(Id_auta) ON DELETE CASCADE\
			)",
			"CREATE TABLE naprawy\
			(\
			Id_naprawy NUMBER(3) CONSTRAINT naprawy_pkk PRIMARY KEY,\
			Id_uslugi NUMBER(3) NOT NULL CONSTRAINT Naprawy_Uslugi_FKK REFERENCES uslugi(Id_uslugi) ON DELETE CASCADE,\
			Koszt_naprawy NUMBER(5),\
			Rodzaj_naprawy VARCHAR2(35),\
			Data_przyjecia DATE,\
			Data_odbioru DATE\
			)",

			"CREATE TABLE cennik\
			(\
			Id_cennika NUMBER(3) CONSTRAINT cennik_pkk PRIMARY KEY,\
			Cena_za_h NUMBER(5),\
			Cena_za_km NUMBER(5),\
			Data_godzina_zmiany DATE,\
			Czy_aktualny NUMBER(1) CONSTRAINT ca_check_ischkk CHECK (Czy_aktualny IN(1,0)),\
			Dotyczy_aut VARCHAR2(255)\
			)",

			"CREATE TABLE koszty_eksploatacji\
			(\
			Id_eksploatacji NUMBER(3) CONSTRAINT koszty_eksploatacji_pkk PRIMARY KEY,\
			Id_auta NUMBER(3) REFERENCES auta(Id_auta) ON DELETE CASCADE,\
			Koszt_naprawy NUMBER(5),\
			Koszt_wym_ogumienia NUMBER(5),\
			Koszt_wym_oleju NUMBER(5),\
			Koszt_wym_rozrzadu NUMBER(5),\
			Koszt_wym_hamulcow NUMBER(5),\
			Koszt_wym_filtrow NUMBER(5),\
			Inne VARCHAR2(255)\
			)",

			"CREATE TABLE wypozyczenia\
			(\
			Id_wypozyczenia NUMBER(3) CONSTRAINT wypozyczenia_pkk PRIMARY KEY,\
			Id_uslugi NUMBER(3) NOT NULL CONSTRAINT Wypozyczenia_Uslugi_FKK REFERENCES uslugi(Id_uslugi) ON DELETE CASCADE,\
			Stan_licznika_przed NUMBER(7),\
			Data_godzina_wypo DATE,\
			Kaucja NUMBER(4),\
			Uwagi VARCHAR2(255),\
			Id_cennika NUMBER(3) REFERENCES cennik(Id_cennika) ON DELETE CASCADE\
			)",

			"CREATE TABLE zwroty\
			(\
			Id_zwrotu NUMBER(3) CONSTRAINT zwroty_pkk PRIMARY KEY,\
			Id_wypozyczenia NUMBER(3) REFERENCES wypozyczenia(Id_wypozyczenia) ON DELETE CASCADE,\
			Id_pracownik NUMBER(3) REFERENCES pracownik(Id_pracownika) ON DELETE CASCADE,\
			Data_godzina_zwrot DATE,\
			Stan_licznika_po NUMBER(7),\
			Uwagi VARCHAR2(255)\
			)"
			
			
		};
		executeStatement(10, creates);
	}

	void dropAllTables()
	{
		string drops[] =
		{
			"DROP TABLE osoba CASCADE CONSTRAINTS",
			"DROP TABLE pracownik CASCADE CONSTRAINTS",
			"DROP TABLE klient CASCADE CONSTRAINTS",
			"DROP TABLE auta CASCADE CONSTRAINTS",
			"DROP TABLE uslugi CASCADE CONSTRAINTS",
			"DROP TABLE naprawy CASCADE CONSTRAINTS",
			"DROP TABLE cennik CASCADE CONSTRAINTS",
			"DROP TABLE koszty_eksploatacji CASCADE CONSTRAINTS",
			"DROP TABLE wypozyczenia CASCADE CONSTRAINTS",
			"DROP TABLE zwroty CASCADE CONSTRAINTS"
		};
		executeStatement(10, drops);
	}

	void insertAll()
	{
		string insertsOsoba[] =
		{
			"INSERT INTO osoba VALUES\
			('1', 'Filip', 'Szemraj', NULL)",
			"INSERT INTO osoba VALUES\
			('2', 'Karol', 'Cioc', NULL)",
			"INSERT INTO osoba VALUES\
			('3', 'Jan', 'Nowak', NULL)",
			"INSERT INTO osoba VALUES\
			('4', 'Jan', 'Luka', NULL)",
			"INSERT INTO osoba VALUES\
			('5', 'Paris', 'Platynov', NULL)",
			"INSERT INTO osoba VALUES\
			('6', 'Mariusz', 'Pudzianowski', NULL)",
			"INSERT INTO osoba VALUES\
			('7', 'Tupac', 'Shakur', NULL)",
			"INSERT INTO osoba VALUES\
			('8', 'Jan', 'Blachowicz', NULL)",
			"INSERT INTO osoba VALUES\
			('9', 'Tony', 'Soprano', NULL)",
			"INSERT INTO osoba VALUES\
			('10', 'Tony', 'Montana', NULL)"
		};
		executeStatement(10, insertsOsoba);
		string insertsPracownik[] =
		{
			"INSERT INTO pracownik\
			VALUES(1,1,01909876541,TO_DATE('03-03-2020 16:00:00','DD-MM-YYYY HH24:MI:SS'),1,512345567)",
			"INSERT INTO pracownik\
			VALUES(2,2,01509866571,TO_DATE('02-02-2021 14:00:00','DD-MM-YYYY HH24:MI:SS'),1,542341267)",
			"INSERT INTO pracownik\
			VALUES(3,3,01709376741,TO_DATE('14-01-2021 12:00:00','DD-MM-YYYY HH24:MI:SS'),1,592873217)",
			"INSERT INTO pracownik\
			VALUES(4,4,05703216741,TO_DATE('18-10-2022 12:00:00','DD-MM-YYYY HH24:MI:SS'),1,543175567)",
			"INSERT INTO pracownik\
			VALUES(5,5,08709342311,TO_DATE('12-12-2022 12:00:00','DD-MM-YYYY HH24:MI:SS'),1,599843767)"
			
		};
		executeStatement(5, insertsPracownik);
		string insertsKlient[] =
		{ 
			"INSERT INTO klient VALUES\
			('1','6', TO_DATE('01-01-2011', 'MM-DD-YYYY'), '01312305698', '733359559')",
			"INSERT INTO klient VALUES\
			('2','7', TO_DATE('01-03-2011', 'MM-DD-YYYY'), '01315555698', '733555559')",
			"INSERT INTO klient VALUES\
			('3','8', TO_DATE('05-01-2011', 'MM-DD-YYYY'), '01233305698', '732333559')",
			"INSERT INTO klient VALUES\
			('4','9', TO_DATE('05-01-2011', 'MM-DD-YYYY'), '01263305698', '632333559')",
			"INSERT INTO klient VALUES\
			('5','10', TO_DATE('05-01-2011', 'MM-DD-YYYY'), '01263305698', '632333559')"
		
		};
		executeStatement(5, insertsKlient);
		
				string insertsAuta[] =
		{ 
			"INSERT INTO auta\
			VALUES(1,'RJK45678',1,20000,'NBVCFGBVHGBFTYJREDSEQAWSERFTCX12',3.0,'diesel','sedan','Audi','c8',TO_DATE('30-10-2009','DD-MM-YYYY'),'Brak')",
			"INSERT INTO auta\
			VALUES(2,'TOS14678',1,10000,'NBVCFGBVHGBFTYWREDSEQAWSERFTCX12',2.0,'benzyna','combi','Bmw','e46',TO_DATE('12-09-2019','DD-MM-YYYY'),'Gruz')",
			"INSERT INTO auta\
			VALUES(3,'WLI45768',1,70000,'NBVCFGBVHGBFTYAREDSEQAWSEDFTCX12',4.0,'benzyna','SUV','Audi','Q8',TO_DATE('09-10-2015','DD-MM-YYYY'),'Brak')",
			"INSERT INTO auta\
			VALUES(4,'PPK41278',1,50000,'NBVCFGBVHGBFTYFREDSEQAWSEDFTCX12',6.3,'diesel','hatchback','Mercedes','s500',TO_DATE('09-10-2020','DD-MM-YYYY'),'Brak')",
			"INSERT INTO auta\
			VALUES(5,'WOS44378',1,30000,'NBVCFGBVHGBFTYGREDSEQAWSEDFTCX12',4.4,'diesel','SUV','Audi','SQ7',TO_DATE('09-10-2022','DD-MM-YYYY'),'Brak')"
		};
		executeStatement(5, insertsAuta);
		
				string insertsKoszty_eksploatacji[] =
		{
			"INSERT INTO koszty_eksploatacji VALUES\
			(1,1,540,1200,320,2000,500,250,'Folia PPF - 3000zl')",
			"INSERT INTO koszty_eksploatacji VALUES\
			(2,2,640,2200,920,3000,1000,800,'Nowa szyba przednia - 4000zl')",
			"INSERT INTO koszty_eksploatacji VALUES\
			(3,3,840,1900,980,3000,290,600,'Prostowanie samochodu po zderzeniu - 15000zl')",
			"INSERT INTO koszty_eksploatacji VALUES\
			(4,4,840,1900,980,3000,290,600,'Wymiana tapicerki - 5000zl')",
			"INSERT INTO koszty_eksploatacji VALUES\
			(5,5,340,2900,1200,1500,240,700,'Wymiana uszczelek pod glowica - 12000zl')"
		};
		executeStatement(5, insertsKoszty_eksploatacji);
		
				string insertsUslugi[] =
		{
			"INSERT INTO uslugi VALUES\
			(1,5,5,4)",
			"INSERT INTO uslugi VALUES\
			(2,5,4,4)",
			"INSERT INTO uslugi VALUES\
			(3,4,3,3)",
			"INSERT INTO uslugi VALUES\
			(4,4,2,2)",
			"INSERT INTO uslugi VALUES\
			(5,3,1,1)",
			"INSERT INTO uslugi VALUES\
			(6,3,5,5)",
			"INSERT INTO uslugi VALUES\
			(7,3,3,3)",
			"INSERT INTO uslugi VALUES\
			(8,2,2,2)",
			"INSERT INTO uslugi VALUES\
			(9,2,1,1)",
			"INSERT INTO uslugi VALUES\
			(10,2,1,1)"
		};
		executeStatement(10, insertsUslugi);
		
				string insertsNaprawy[] =
		{
			"INSERT INTO naprawy VALUES\
			(1, 6, 500, 'Naprawa silnika', TO_DATE('06-11-2022', 'MM-DD-YYYY'), TO_DATE('06-20-2022', 'MM-DD-YYYY'))",
			"INSERT INTO naprawy VALUES\
			(2, 7, 500, 'Naprawa napedu', TO_DATE('06-12-2022', 'MM-DD-YYYY'), TO_DATE('06-21-2022', 'MM-DD-YYYY'))",
			"INSERT INTO naprawy VALUES\
			(3, 8, 1000, 'Naprawa elektroniki', TO_DATE('06-13-2022', 'MM-DD-YYYY'), TO_DATE('06-22-2022', 'MM-DD-YYYY'))",
			"INSERT INTO naprawy VALUES\
			(4, 9, 500, 'Naprawa podwozia', TO_DATE('06-14-2022', 'MM-DD-YYYY'), TO_DATE('06-23-2022', 'MM-DD-YYYY'))",
			"INSERT INTO naprawy VALUES\
			(5, 10, 500, 'Naprawa nadwozia', TO_DATE('06-15-2022', 'MM-DD-YYYY'), TO_DATE('06-24-2022', 'MM-DD-YYYY'))"
		};
		executeStatement(5, insertsNaprawy);
		
				string insertsCennik[] =
		{
			"INSERT INTO cennik\
			VALUES(1,20,3,TO_DATE('09-02-2020','DD-MM-YYYY'),1,'Mercedes')",
			"INSERT INTO cennik\
			VALUES(2,30,2,TO_DATE('09-02-2020','DD-MM-YYYY'),1,'Audi')",
			"INSERT INTO cennik\
			VALUES(3,25,4,TO_DATE('09-02-2020','DD-MM-YYYY'),1,'Bmw')",
			"INSERT INTO cennik\
			VALUES(4,15,2,TO_DATE('09-02-2020','DD-MM-YYYY'),1,'Honda')",
			"INSERT INTO cennik\
			VALUES(5,20,4,TO_DATE('09-02-2020','DD-MM-YYYY'),1,'Opel')"

		};
		executeStatement(5, insertsCennik);
		
				string insertsWypozyczenia[] =
		{
			"INSERT INTO wypozyczenia\
			VALUES(1,1,45600,TO_DATE('03-12-2022 16:00:00','DD-MM-YYYY HH24:MI:SS'),88,'Brak',1)",
			"INSERT INTO wypozyczenia\
			VALUES(2,2,55600,TO_DATE('02-12-2022 16:00:00','DD-MM-YYYY HH24:MI:SS'),89,'Brak',2)",
			"INSERT INTO wypozyczenia\
			VALUES(3,3,65600,TO_DATE('04-12-2022 16:00:00','DD-MM-YYYY HH24:MI:SS'),76,'Brak',3)",
			"INSERT INTO wypozyczenia\
			VALUES(4,4,75600,TO_DATE('06-12-2022 16:00:00','DD-MM-YYYY HH24:MI:SS'),90,'Brak',4)",
			"INSERT INTO wypozyczenia\
			VALUES(5,5,85600,TO_DATE('02-12-2022 16:00:00','DD-MM-YYYY HH24:MI:SS'),80,'Brak',5)"
		};
		executeStatement(5, insertsWypozyczenia);
		
				string insertsZwroty[] =
		{
			"INSERT INTO zwroty VALUES\
			(1,1,1, TO_DATE('10-12-2022 12:00:00','DD-MM-YYYY HH24:MI:SS'), 190, 'Uszkodzona szyba')",
			"INSERT INTO zwroty VALUES\
			(2,2,2, TO_DATE('11-12-2022 11:00:00','DD-MM-YYYY HH24:MI:SS'), 112, 'Brak')",
			"INSERT INTO zwroty VALUES\
			(3,3,3, TO_DATE('12-12-2022 15:00:00','DD-MM-YYYY HH24:MI:SS'), 170, 'Brak')",
			"INSERT INTO zwroty VALUES\
			(4,4,4, TO_DATE('10-12-2022 10:00:00','DD-MM-YYYY HH24:MI:SS'), 100, 'Zlapal gume')",
			"INSERT INTO zwroty VALUES\
			(5,5,5, TO_DATE('11-12-2022 12:00:00','DD-MM-YYYY HH24:MI:SS'), 120, 'Poplamiona tapicerka')"

		};
		executeStatement(5, insertsZwroty);
		
	}

	void deleteAll()
	{
		string drops[] =
		{
			"DELETE FROM osoba",
			"DELETE FROM klienci",
			"DELETE FROM pracownik",
			"DELETE FROM auta",
			"DELETE FROM koszty_eksploatacji",
			"DELETE FROM uslugi",
			"DELETE FROM naprawy",
			"DELETE FROM zwroty",
			"DELETE FROM wypozyczenia",
			"DELETE FROM cennik"

		};
		executeStatement(10, drops);
	}

	void createAllSequences()
	{
		string sequences[] =
        {
            "CREATE SEQUENCE id_osoba_seq START WITH 11",
            "CREATE SEQUENCE id_klienta_seq START WITH 6",
            "CREATE SEQUENCE id_pracownika_seq START WITH 6",
            "CREATE SEQUENCE id_naprawy_seq START WITH 6",
            "CREATE SEQUENCE id_auta_seq START WITH 6",
            "CREATE SEQUENCE id_uslugi_seq START WITH 11",
            "CREATE SEQUENCE id_zwrotu_seq START WITH 6",
            "CREATE SEQUENCE id_wypozyczenia_seq START WITH 6",
            "CREATE SEQUENCE id_eksploatacji_seq START WITH 6",
            "CREATE SEQUENCE id_cennik_seq START WITH 6"
        };
        executeStatement(10, sequences);
	}

	void dropAllSequences()
	{
		string sequences[] =
        {
            "DROP SEQUENCE id_osoba_seq",
            "DROP SEQUENCE id_klienta_seq",
            "DROP SEQUENCE id_pracownika_seq",
            "DROP SEQUENCE id_naprawy_seq",
            "DROP SEQUENCE id_auta_seq",
            "DROP SEQUENCE id_uslugi_seq",
            "DROP SEQUENCE id_zwrotu_seq",
            "DROP SEQUENCE id_wypozyczenia_seq",
            "DROP SEQUENCE id_eksploatacji_seq",
            "DROP SEQUENCE id_cennik_seq"

        };
        executeStatement(10, sequences);
	}

void createAllProcedures()
	{
		string procedures[] =
		{			
			"CREATE OR REPLACE PROCEDURE dodaj_osobe(v_czy_klient NUMBER, v_imie VARCHAR2, v_nazwisko VARCHAR2, v_pesel VARCHAR2, v_telefon NUMBER)\
			IS\
			v_id_osoba osoba.Id_osoba%TYPE;\
			v_id_pracownika pracownik.Id_pracownika%TYPE;\
			v_id_klienta klient.Id_klienta%TYPE;\
			czy_klient_exception EXCEPTION;\
			BEGIN\
				SELECT max(Id_osoba)+1\
				INTO v_id_osoba\
				FROM osoba;\
				INSERT INTO osoba VALUES (v_id_osoba, v_imie, v_nazwisko, NULL);\
			IF v_czy_klient=1 THEN\
				SELECT max(Id_klienta)+1\
				INTO v_id_klienta\
				FROM klient;\
				INSERT INTO klient VALUES (v_id_klienta, v_id_osoba, SYSDATE, v_pesel, v_telefon);\
			ELSIF v_czy_klient=0 THEN \
				SELECT max(Id_pracownika)+1 \
				INTO v_id_pracownika\
				FROM pracownik;\
				INSERT INTO pracownik VALUES (v_id_pracownika, v_id_osoba, v_pesel, SYSDATE, 1, v_telefon); \
			ELSE \
				RAISE czy_klient_exception; \
			END IF; \
			EXCEPTION \
				WHEN czy_klient_exception THEN \
				DBMS_OUTPUT.ENABLE; \
					DBMS_OUTPUT.PUT_LINE('BLAD:'); \
				DBMS_OUTPUT.PUT_LINE('Zmienna czy_klient powinna zawierac 1 lub 0'); \
				RETURN; \
			END;",
			
			"CREATE OR REPLACE PROCEDURE dodaj_nowa_usluge(v_id_pracownika NUMBER, v_id_klienta NUMBER, v_id_auta NUMBER) \
			IS \
			CURSOR pracownicy IS \
				SELECT Id_pracownika \
				FROM pracownik;\
			CURSOR klienci IS\
				SELECT Id_klienta\
				FROM klient;\
			CURSOR auta IS\
				SELECT Id_auta\
				FROM auta;\
			pracownik_exception EXCEPTION;\
			klient_exception EXCEPTION;\
			auto_exception EXCEPTION;\
			v_pr_ex_check NUMBER(1);\
			v_kl_ex_check NUMBER(1);\
			v_au_ex_check NUMBER(1);\
			v_id_uslugi uslugi.Id_uslugi%TYPE;\
			BEGIN\
			v_pr_ex_check:=0;\
			v_kl_ex_check:=0;\
			v_au_ex_check:=0;\
			FOR pr IN pracownicy LOOP\
				IF pr.Id_pracownika=v_id_pracownika THEN\
					DBMS_OUTPUT.ENABLE;\
					DBMS_OUTPUT.PUT_LINE('MAMY TAKIEGO PRACOWNIKA');\
					v_pr_ex_check:=1;\
					FOR kl IN klienci LOOP\
						IF kl.Id_klienta=v_id_klienta THEN\
							DBMS_OUTPUT.ENABLE;\
							DBMS_OUTPUT.PUT_LINE('MAMY TAKIEGO KLIENTA');\
							v_kl_ex_check:=1;\
							FOR au IN auta LOOP\
								IF au.Id_auta=v_id_auta THEN\
									DBMS_OUTPUT.ENABLE;\
									DBMS_OUTPUT.PUT_LINE('MAMY TAKIE AUTO');\
									v_au_ex_check:=1;\
								END IF;\
							END LOOP;\
						END IF;\
					END LOOP;\
				END IF;\
			END LOOP;\
			IF v_pr_ex_check!=1 THEN\
				RAISE pracownik_exception;\
			END IF;\
			IF v_kl_ex_check!=1 THEN\
				RAISE klient_exception;\
			END IF;\
			IF v_au_ex_check!=1 THEN\
				RAISE auto_exception;\
			END IF;\
			SELECT max(id_uslugi)+1 INTO v_id_uslugi FROM uslugi;\
			INSERT INTO uslugi VALUES (v_id_uslugi, v_id_pracownika, v_id_klienta, v_id_auta);\
			DBMS_OUTPUT.ENABLE;\
			DBMS_OUTPUT.PUT_LINE('Dodano nowa usluge');\
			EXCEPTION\
			WHEN pracownik_exception THEN\
				DBMS_OUTPUT.ENABLE;\
				DBMS_OUTPUT.PUT_LINE('Nie ma takiego pracownika');\
				RETURN;\
			WHEN klient_exception THEN\
				DBMS_OUTPUT.ENABLE;\
				DBMS_OUTPUT.PUT_LINE('Nie ma takiego klienta');\
				RETURN;\
			WHEN auto_exception THEN\
				DBMS_OUTPUT.ENABLE;\
				DBMS_OUTPUT.PUT_LINE('Nie ma takiego auta');\
				RETURN;\
			END;",			
			"CREATE OR REPLACE PROCEDURE dodaj_nowa_naprawe(v_id_uslugi NUMBER, v_koszt_naprawy NUMBER, v_rodzaj_naprawy VARCHAR2, v_data_odbioru DATE) \
			IS \
			v_id_naprawy naprawy.Id_naprawy%TYPE; \
			CURSOR wolne_uslugi IS \
				SELECT id_uslugi FROM uslugi WHERE id_uslugi != ALL  \
				(SELECT id_uslugi FROM naprawy UNION SELECT id_uslugi FROM wypozyczenia); \
			v_id_ex_check NUMBER(1); \
			id_uslug_exception EXCEPTION; \
			BEGIN \
			SELECT max(Id_naprawy)+1 INTO v_id_naprawy FROM naprawy; \
			v_id_ex_check:=0; \
			FOR wu IN wolne_uslugi LOOP \
				IF wu.Id_uslugi = v_id_uslugi THEN \
					v_id_ex_check:=1; \
				END IF; \
			END LOOP; \
			IF v_id_ex_check != 1 THEN \
				RAISE id_uslug_exception; \
			END IF; \
			INSERT INTO naprawy VALUES(v_id_naprawy, v_id_uslugi, v_koszt_naprawy, v_rodzaj_naprawy, SYSDATE, v_data_odbioru); \
			EXCEPTION \
			WHEN id_uslug_exception THEN \
				DBMS_OUTPUT.ENABLE; \
				DBMS_OUTPUT.PUT_LINE('Nie ma wolnej uslugi(bez przypisanego do niej wypozyczenia lub naprawy) o takim ID'); \
				RETURN; \
			END;",			
			"CREATE OR REPLACE PROCEDURE usun_usluge(v_id_uslugi NUMBER) \
			IS \
			BEGIN \
			DELETE FROM uslugi WHERE id_uslugi=v_id_uslugi; \
			END;",
			
			"CREATE OR REPLACE PROCEDURE dodaj_zaimek_do_osob \
			IS \
			CURSOR osobniczki IS  \
				SELECT *  \
				FROM osoba \
				WHERE Imie LIKE '%a'; \
			CURSOR osobnicy IS  \
				SELECT *  \
				FROM osoba \
				WHERE Imie NOT LIKE '%a'; \
			tmp osoba%ROWTYPE; \
			v_pani osoba.Plec%TYPE; \
			v_pan osoba.Plec%TYPE; \
			BEGIN \
			v_pani:='Pani'; \
			v_pan:='Pan'; \
			OPEN osobniczki; \
			LOOP \
				FETCH osobniczki INTO tmp; \
				UPDATE osoba \
				SET Plec=v_pani \
				WHERE Id_osoba=tmp.Id_osoba; \
				EXIT WHEN osobniczki%NOTFOUND; \
			END LOOP; \
			CLOSE osobniczki; \
			OPEN osobnicy; \
			LOOP  \
				FETCH osobnicy INTO tmp; \
				UPDATE osoba \
				SET Plec=v_pan \
				WHERE Id_osoba=tmp.Id_osoba; \
				EXIT WHEN osobnicy%NOTFOUND; \
			END LOOP; \
			CLOSE osobnicy; \
			END;",	
			"CREATE OR REPLACE PROCEDURE dodaj_losowe_cenniki(v_ilosc NUMBER) \
			IS \
			v_cena_za_h cennik.Cena_za_h%TYPE; \
			v_cena_za_km cennik.Cena_za_km%TYPE; \
			v_data_godzina_zmiany cennik.Data_godzina_zmiany%TYPE; \
			v_czy_aktualny cennik.Czy_aktualny%TYPE; \
			v_dotyczy_aut cennik.Dotyczy_aut%TYPE; \
			v_id_cennika cennik.Id_cennika%TYPE; \
			i NUMBER(1):=0; \
			BEGIN \
			SELECT max(Id_cennika)+1 INTO v_id_cennika FROM cennik; \
			WHILE i<v_ilosc LOOP \
			v_cena_za_h:=DBMS_RANDOM.VALUE(1,99999); \
			v_cena_za_km:=DBMS_RANDOM.VALUE(1,99999); \
			v_data_godzina_zmiany:=SYSDATE; \
			v_czy_aktualny:=0; \
			v_dotyczy_aut:=DBMS_RANDOM.STRING('a', 10); \
			v_id_cennika:=v_id_cennika+i; \
			INSERT INTO cennik VALUES (v_id_cennika, v_cena_za_h, v_cena_za_km, v_data_godzina_zmiany, v_czy_aktualny, v_dotyczy_aut); \
			i:=i+1; \
			END LOOP; \
			END;",
			"CREATE OR REPLACE PROCEDURE dodaj_wypozyczenia(v_id_uslugi NUMBER, v_stan_przed NUMBER,v_kaucja NUMBER, v_uwagi VARCHAR2,v_id_cennika NUMBER)\
			IS\
			v_id_wypozyczenia wypozyczenia.Id_wypozyczenia%TYPE;\
			CURSOR wolna_usluga IS\
				SELECT id_uslugi FROM uslugi WHERE id_uslugi != ALL\
				(SELECT id_uslugi FROM naprawy UNION SELECT id_uslugi FROM wypozyczenia);\
			v_id_ex_check NUMBER(1);\
			id_uslug_exception EXCEPTION;\
			BEGIN\
			SELECT max(Id_wypozyczenia)+1 INTO v_id_wypozyczenia FROM wypozyczenia;\
			v_id_ex_check := 0;\
			FOR w IN wolna_usluga LOOP\
				IF w.Id_uslugi = v_id_uslugi THEN\
				v_id_ex_check := 1;\
				END IF;\
			END LOOP;\
			IF v_id_ex_check != 1 THEN\
				RAISE id_uslug_exception;\
			END IF;\
			INSERT INTO wypozyczenia VALUES(v_id_wypozyczenia,v_id_uslugi,v_stan_przed,TO_DATE('04-01-2022 16:00:00','DD-MM-YYYY HH24:MI:SS'),v_kaucja,v_uwagi,v_id_cennika);\
			EXCEPTION\
			WHEN id_uslug_exception THEN\
				DBMS_OUTPUT.ENABLE;\
				DBMS_OUTPUT.PUT_LINE('Nie ma wolnej uslugi o takim ID');\
				RETURN;\
			END;"
		};
		executeStatement(7, procedures);
	}

	void dropAllProcedures()
	{
		string procedures[] =
		{
			"DROP PROCEDURE dodaj_osobe",
			"DROP PROCEDURE dodaj_nowa_usluge",
			"DROP PROCEDURE dodaj_nowa_naprawe",
			"DROP PROCEDURE usun_usluge",
			"DROP PROCEDURE dodaj_zaimek_do_osob",
			"DROP PROCEDURE dodaj_losowe_cenniki"
		};

		executeStatement(6, procedures);
	}
	
void procedureWithParam_1()
	{
		stmt = conn->createStatement("BEGIN dodaj_osobe(:id,:i,:n,:pesel,:tele); END;");
		string osobaNazwisko;
		string osobaImie;
		int czy_klient_prac;
		int pesel;
		int telefon;
		cout << "Podaj 0-pracownik, 1-klient: "<<endl;
		cin >> czy_klient_prac;
		cout << "Podaj nazwisko nowej osoby: "<<endl;
		cin >> osobaNazwisko;
		cout << "Podaj imie nowej osoby: "<<endl;
		cin >> osobaImie;
		cout << "Podaj PESEL: "<<endl;
		cin >> pesel;
		cout << "Podaj numer telefonu: "<<endl;
		cin >> telefon;
		stmt->setInt(1,czy_klient_prac);
		stmt->setString(2, osobaNazwisko);
		stmt->setString(3, osobaImie);
		stmt->setInt(4,pesel);
		stmt->setInt(5,telefon);
		try {
			stmt->executeUpdate();
			cout << "Procedura wykonana poprawnie" << endl;
		}
		catch (SQLException ex)
		{
			cout << "Procedura wykonana niepoprawnie" << endl;
			cout << ex.getMessage() << endl;
		}
		conn->terminateStatement(stmt);
	}
	
void procedureWithParam_2()
	{
		stmt = conn->createStatement("BEGIN dodaj_nowa_usluge(:id_prac,:id_kl,:id_au); END;");
		int id_prac;
		int id_kl;
		int id_au;
		cout<<"Podaj id pracownika"<<endl;
		cin>>id_prac;
		cout<<"Podaj id klienta"<<endl;
		cin>>id_kl;
		cout<<"Podaj id auta"<<endl;
		cin>>id_au;
		stmt->setInt(1,id_prac);
		stmt->setInt(2,id_kl);
		stmt->setInt(3,id_au);
		try {
			stmt->executeUpdate();
			cout << "Procedura wykonana poprawnie" << endl;
		}
		catch (SQLException ex)
		{
			cout << "Procedura wykonana niepoprawnie" << endl;
			cout << ex.getMessage() << endl;
		}
		conn->terminateStatement(stmt);
	}
	
	void procedureWithParam_3()
	{
		stmt = conn->createStatement("BEGIN dodaj_nowa_naprawe(:id_us,:koszt,:rodzaj, TO_DATE(:data, 'MM-DD-YYYY')); END;");
		int id_us;
		int koszt;
		string rodzaj;
		string data_odb;
		cout<<"Podaj id uslugi"<<endl;
		cin>>id_us;
		cout<<"Podaj koszt naprawy"<<endl;
		cin>>koszt;
		cout<<"Podaj rodzaj naprawy"<<endl;
		cin>>rodzaj;
		cout<<"Podaj date odbioru (MM-DD-YYYY)"<<endl;
		cin>>data_odb;
		stmt->setInt(1,id_us);
		stmt->setInt(2,koszt);
		stmt->setString(3,rodzaj);
		stmt->setString(4,data_odb);
		try {
			stmt->executeUpdate();
			cout << "Procedura wykonana poprawnie" << endl;
		}
		catch (SQLException ex)
		{
			cout << "Procedura wykonana niepoprawnie" << endl;
			cout << ex.getMessage() << endl;
		}
		conn->terminateStatement(stmt);
	}
	
	void procedureWithParam_4()
	{
		stmt = conn->createStatement("BEGIN usun_usluge(:id_usl); END;");
		int id_usl;
		cout<<"Podaj id uslugi do usuniecia"<<endl;
		cin>>id_usl;
		stmt->setInt(1,id_usl);
		try {
			stmt->executeUpdate();
			cout << "Procedura wykonana poprawnie" << endl;
		}
		catch (SQLException ex)
		{
			cout << "Procedura wykonana niepoprawnie" << endl;
			cout << ex.getMessage() << endl;
		}
		conn->terminateStatement(stmt);
	}
	
	void procedureWithParam_5()
	{
		stmt = conn->createStatement("BEGIN dodaj_losowe_cenniki(:ilosc); END;");
		int ilosc;
		cout<<"Podaj ilosc cennikow do dodania"<<endl;
		cin>>ilosc;
		stmt->setInt(1,ilosc);
		try {
			stmt->executeUpdate();
			cout << "Procedura wykonana poprawnie" << endl;
		}
		catch (SQLException ex)
		{
			cout << "Procedura wykonana niepoprawnie" << endl;
			cout << ex.getMessage() << endl;
		}
		conn->terminateStatement(stmt);
	}
	
	void procedureWithParam_6()
	{
		stmt = conn->createStatement("BEGIN dodaj_wypozyczenia(:id_us,:stan,:kau,:uwagi,:id_cenn); END;");
		int id_u;
		int stan;
		int kaucja;
		string uwagi;
		int id_cen;
		cout<<"Podaj id uslugi"<<endl;
		cin>>id_u;
		cout<<"Podaj stan licznika przed"<<endl;
		cin>>stan;
		cout<<"Podaj kaucje"<<endl;
		cin>>kaucja;
		cout<<"Podaj uwagi"<<endl;
		cin>>uwagi;
		cout<<"Podaj id cennika"<<endl;
		cin>>id_cen;
		stmt->setInt(1,id_u);
		stmt->setInt(2,stan);
		stmt->setInt(3,kaucja);
		stmt->setString(4,uwagi);
		stmt->setInt(5,id_cen);
		try {
			stmt->executeUpdate();
			cout << "Procedura wykonana poprawnie" << endl;
		}
		catch (SQLException ex)
		{
			cout << "Procedura wykonana niepoprawnie" << endl;
			cout << ex.getMessage() << endl;
		}
		conn->terminateStatement(stmt);
	}
	
	
	void procedureWithParam()
	{
		cout<<"Dostepne procedury to:"<<endl;
		cout<<"1. Dodanie nowej osoby"<<endl;
		cout<<"2. Dodanie nowej uslugi"<<endl;
		cout<<"3. Dodanie nowej naprawy"<<endl;
		cout<<"4. Usuniecie uslugi"<<endl;
		cout<<"5. Dodanie losowych cennikow"<<endl;
		cout<<"6. Dodanie wypozyczenia"<<endl;
		int x=0;
		cin>>x;
		if(x==1)
		{
			procedureWithParam_1();
		}
		else if(x==2)
		{
			procedureWithParam_2();
		}
		else if(x==3)
		{
			procedureWithParam_3();
		}
		else if(x==4)
		{
			procedureWithParam_4();
		}
		else if(x==5)
		{
			procedureWithParam_5();
		}
		else if(x==6)
		{
			procedureWithParam_6();
		}
		else
		{
			return;
		}
	}

	void createAllFunctions()
	{
		string functions[] =
		{
			"CREATE OR REPLACE FUNCTION naj_osoba(v_czy_klient NUMBER)\
			RETURN NUMBER\
			IS\
			CURSOR maks_klient IS\
				SELECT Id_osoba, count(Id_osoba) ilosc_uslug FROM klient k\
				JOIN uslugi u ON u.Id_klienta = k.Id_klienta\
				GROUP BY Id_osoba\
				HAVING count(Id_osoba) = (SELECT MAX(COUNT(Id_osoba))\
				FROM klient k\
				JOIN uslugi u ON u.Id_klienta = k.Id_klienta\
				GROUP BY Id_osoba);\
			CURSOR maks_prac IS\
				SELECT Id_osoba, count(Id_osoba) ilosc_uslug FROM pracownik p\
				JOIN uslugi u ON u.Id_pracownika = p.Id_pracownika\
				GROUP BY Id_osoba\
				HAVING count(Id_osoba) = (SELECT MAX(COUNT(Id_osoba))\
				FROM pracownik p\
				JOIN uslugi u ON u.Id_pracownika = p.Id_pracownika\
				GROUP BY Id_osoba);\
			v_id_osoba osoba.Id_osoba%TYPE;\
			v_ilosc_uslug NUMBER;\
			zly_parametr EXCEPTION;\
			BEGIN\
			IF v_czy_klient=1 THEN\
				OPEN maks_klient;\
				FETCH maks_klient INTO v_id_osoba, v_ilosc_uslug;\
				DBMS_OUTPUT.ENABLE;\
				DBMS_OUTPUT.PUT_LINE('Klient o Id_osoba: ' || v_czy_klient || ' ma najwiecej uslug, czyli: ' || v_czy_klient);\
				CLOSE maks_klient;\
				RETURN v_id_osoba;\
			ELSIF v_czy_klient = 0 THEN\
				OPEN maks_prac;\
				FETCH maks_prac INTO v_id_osoba, v_ilosc_uslug;\
				DBMS_OUTPUT.ENABLE;\
				DBMS_OUTPUT.PUT_LINE('Pracownik o Id_osoba: ' || v_czy_klient || ' ma najwiecej uslug, czyli: ' || v_czy_klient);\
				CLOSE maks_prac;\
				RETURN v_id_osoba;\
			ELSE\
				RAISE zly_parametr;\
			END IF;\
			EXCEPTION\
			WHEN zly_parametr THEN\
				DBMS_OUTPUT.ENABLE;\
				DBMS_OUTPUT.PUT_LINE('Parametr przy wywolaniu powinien byc rowny 1 lub 0');\
				RETURN 0;\
			END;",
			"CREATE OR REPLACE FUNCTION eksploatacja(v_marka VARCHAR2)\
			RETURN NUMBER\
			IS CURSOR kwota_eksploatacji IS\
			SELECT * FROM koszty_eksploatacji k\
			JOIN auta a ON k.Id_auta = a.Id_auta\
			WHERE a.marka = v_marka;\
			suma NUMBER(7) := 0;\
			BEGIN\
			FOR tmp IN kwota_eksploatacji LOOP\
				suma := suma + tmp.Koszt_naprawy+tmp.Koszt_wym_ogumienia+tmp.Koszt_wym_oleju+tmp.Koszt_wym_rozrzadu+tmp.Koszt_wym_hamulcow+tmp.Koszt_wym_filtrow;\
			END LOOP;\
			RETURN suma;\
			END",
			"CREATE OR REPLACE FUNCTION Ilosc_wyp\
			RETURN NUMBER\
			IS\
			kw_pierwszy NUMBER;\
			kw_drugi NUMBER;\
			kw_trzeci NUMBER;\
			kw_czwarty NUMBER;\
			najwiekszy NUMBER;\
			kw_kwartal VARCHAR2(25);\
			CURSOR miesiac IS\
			SELECT TO_CHAR(Data_godzina_wypo,'MM') miesiac FROM wypozyczenia;\
			BEGIN\
			kw_pierwszy := 0;\
			kw_drugi := 0;\
			kw_trzeci := 0;\
			kw_czwarty := 0;\
			kw_kwartal := 'Pierwszy';\
			FOR tmp IN miesiac LOOP\
				IF tmp.miesiac > 0 AND tmp.miesiac <4 THEN\
					kw_pierwszy := kw_pierwszy+1;\
				ELSIF tmp.miesiac > 3 AND tmp.miesiac <7 THEN\
					kw_drugi := kw_drugi+1;\
				ELSIF tmp.miesiac > 6 AND tmp.miesiac <10 THEN\
					kw_trzeci := kw_trzeci+1;\
				ELSE\
					kw_czwarty := kw_czwarty+1;\
				END IF;\
			END LOOP;\
			najwiekszy := kw_pierwszy;\
			IF kw_drugi>najwiekszy THEN\
				najwiekszy := kw_drugi;\
				kw_kwartal := 'Drugi';\
			ELSIF kw_trzeci > najwiekszy THEN\
				najwiekszy := kw_trzeci;\
				kw_kwartal := 'Trzeci';\
			ELSIF kw_czwarty > najwiekszy THEN\
				najwiekszy := kw_czwarty;\
				kw_kwartal := 'Czwarty';\
			END IF;\
			DBMS_OUTPUT.ENABLE;\
			DBMS_OUTPUT.PUT_LINE('Kwartal'||TO_CHAR(kw_kwartal));\
			RETURN najwiekszy;\
			END;"
		};
		executeStatement(3, functions);
	}

	void dropAllFunctions()
	{
		string functions[] =
		{
			"DROP FUNCTION naj_osoba",
			"DROP FUNCTION eksploatacja",
			"DROP FUNCTION Ilosc_wyp"
		};

		executeStatement(3, functions);
	}

void selectWithoutParam_1()
	{
		string auta_w_naprawie = "SELECT a.Nr_rejestracyjny,a.Czy_sprawny, a.Przebieg, a.Nr_vin, a.Moc_silnika, a.Marka,a.Model\
		FROM auta a\
		JOIN uslugi u ON a.Id_auta = u.Id_auta\
		JOIN naprawy n ON n.Id_uslugi = u.Id_uslugi";
		stmt = conn->createStatement(auta_w_naprawie);
		ResultSet *rset = stmt->executeQuery();
		try {
			while (rset->next())
			{
				cout << "Nr rejestracyjny " << rset->getString(1) << " czy sprawny: " << rset->getInt(2) << " przebieg :" << rset->getInt(3) <<" nr vin: "<<rset->getString(4)<<" moc silnika: "<<rset->getInt(5)<<" marka: "<<rset->getString(6)<<" model: "<<rset->getString(7)<<endl;
			}
			cout << "SELECT executed correctly" << endl;
		}
		catch (SQLException ex)
		{
			cout << "SELECT executed not correctly" << endl;
			cout << ex.getMessage() << endl;
		}
		stmt->closeResultSet(rset);
		conn->terminateStatement(stmt);
	}
	
	void selectWithoutParam_2()
	{
		string klient_najdl_wyp = "SELECT o.Imie, o.nazwisko, (z.data_godzina_zwrot-w.data_godzina_wypo)*24 okres_wypozyczenia_w_H\
		FROM osoba o\
		JOIN klient k ON o.Id_osoba = k.Id_osoba\
		JOIN uslugi u ON k.Id_klienta = u.Id_klienta\
		JOIN wypozyczenia w ON u.Id_uslugi = w.Id_uslugi\
		JOIN zwroty z ON w.Id_wypozyczenia = z.Id_wypozyczenia\
		WHERE (z.data_godzina_zwrot-w.data_godzina_wypo) = (\
			SELECT MAX(z.data_godzina_zwrot-w.data_godzina_wypo)\
			FROM wypozyczenia w JOIN zwroty z ON w.Id_wypozyczenia = z.Id_wypozyczenia\
		)";
		stmt = conn->createStatement(klient_najdl_wyp);
		ResultSet *rset = stmt->executeQuery();
		try {
			while (rset->next())
			{
				cout << "Imie " << rset->getString(1) << " Nazwisko: " << rset->getString(2) << " Okres wypozyczenia w h:" << rset->getInt(3)<<endl;
			}
			cout << "SELECT executed correctly" << endl;
		}
		catch (SQLException ex)
		{
			cout << "SELECT executed not correctly" << endl;
			cout << ex.getMessage() << endl;
		}
		stmt->closeResultSet(rset);
		conn->terminateStatement(stmt);
	}
	
	
void selectWithoutParam_3()
	{
		string auto_najw_koszt = "SELECT a.Nr_rejestracyjny, a.Przebieg, a.Nr_vin, a.Moc_silnika, a.Rodzaj_paliwa, \
		a.Rodzaj_nadwozia, a.Marka, a.Model, a.Data_produkcji, koszt\
		FROM auta a\
		JOIN(\
			SELECT u.Id_auta aid, MAX(n.Koszt_naprawy) koszt\
			FROM uslugi u\
			JOIN naprawy n ON u.Id_uslugi = n.Id_uslugi\
			WHERE (n.Koszt_naprawy) = (\
				SELECT MAX(koszt_naprawy) FROM naprawy\
			)\
			GROUP BY u.Id_auta\
		) ON a.Id_auta = aid";
		stmt = conn->createStatement(auto_najw_koszt);
		ResultSet *rset = stmt->executeQuery();
		try {
			while (rset->next())
			{
				cout << "Nr rejestracyjny " << rset->getString(1)<< " przebieg :" << rset->getInt(2) <<" nr vin: "<<rset->getString(3)<<" moc silnika: "<<rset->getInt(4)<<" rodzaj paliwa: "<<rset->getString(5)<<endl;
			}
			cout << "SELECT executed correctly" << endl;
		}
		catch (SQLException ex)
		{
			cout << "SELECT executed not correctly" << endl;
			cout << ex.getMessage() << endl;
		}
		stmt->closeResultSet(rset);
		conn->terminateStatement(stmt);
	}
	
	void selectWithoutParam_4()
	{
		string dane_kl_au_ost_wyp = "SELECT o.Imie, o.Nazwisko, a.Nr_rejestracyjny, a.Przebieg, a.Marka, a.Model, a.Data_produkcji\
		FROM osoba o \
		JOIN klient k ON o.Id_osoba = k.Id_osoba\
		JOIN uslugi u ON k.Id_klienta = u.Id_klienta\
		JOIN auta a ON u.Id_auta = a.Id_auta\
		JOIN wypozyczenia w ON w.Id_uslugi = u.Id_uslugi\
		AND w.data_godzina_wypo = (\
			SELECT MAX(data_godzina_wypo) FROM wypozyczenia\
		)";
		stmt = conn->createStatement(dane_kl_au_ost_wyp);
		ResultSet *rset = stmt->executeQuery();
		try {
			while (rset->next())
			{
				cout << "Imie klienta " << rset->getString(1) << " Nazwisko: " << rset->getString(2) << " Nr rejestracyjny :" << rset->getString(3) <<" przebieg: "<<rset->getInt(4)<<" marka: "<<rset->getString(5)<<" model: "<<rset->getString(6)<<" data produkcji: "<<rset->getString(7)<<endl;
			}
			cout << "SELECT executed correctly" << endl;
		}
		catch (SQLException ex)
		{
			cout << "SELECT executed not correctly" << endl;
			cout << ex.getMessage() << endl;
		}
		stmt->closeResultSet(rset);
		conn->terminateStatement(stmt);
	}
	
	void selectWithoutParam()
	{
		cout<<"Dostepne selecty to:"<<endl;
		cout<<"1. Wyswietl auta, ktore sa w naprawie"<<endl;
		cout<<"2. Wyswietl dane klienta z najdluzszym okresem wypozyczenia"<<endl;
		cout<<"3. Wyswietl dane auta z najwyzszym kosztem naprawy"<<endl;
		cout<<"4. Wyswietl dane klienta i auta z ostatniego wypozyczenia"<<endl;
		
		int x=0;
		cin>>x;
		if(x==1){
			selectWithoutParam_1();
		}
		else if(x==2){
			selectWithoutParam_2();
		}	
		else if(x==3){
			selectWithoutParam_3();
		}
		else if(x==4){
			selectWithoutParam_4();
		}
		else{
			return;
		}
	}

	void selectWithParam_1()
	{
		string osoby = "SELECT Id_osoba, Nazwisko, Imie FROM osoba WHERE Nazwisko = :m";
		string nazwisko;
		cout<<"Podaj nazwisko szukanej osoby"<<endl;
		cin>>nazwisko;
		stmt=conn->createStatement(osoby);
		stmt->setString(1, nazwisko);
		ResultSet *rset = stmt->executeQuery();
		try {
			while (rset->next())
			{
				cout << "Osoba " << rset->getInt(1) << ": " << rset->getString(2) << " " << rset->getString(3) << endl;
			}
			cout << "SELECT executed correctly" << endl;
		}
		catch (SQLException ex)
		{
			cout << "SELECT executed not correctly" << endl;
			cout << ex.getMessage() << endl;
		}
		stmt->closeResultSet(rset);
		conn->terminateStatement(stmt);
	}
	
	void selectWithParam_2()
	{
		string pracownicy = "SELECT Id_osoba, Id_pracownika, Czy_obecny FROM pracownik WHERE Telefon_komorkowy = :m";
		int telefon;
		cout<<"Podaj telefon szukanego pracownika"<<endl;
		cin>>telefon;
		stmt=conn->createStatement(pracownicy);
		stmt->setInt(1, telefon);
		ResultSet *rset = stmt->executeQuery();
		try {
			while (rset->next())
			{
				cout << "Pracownik o id osoby: " << rset->getInt(1) << ": id pracownika - " << rset->getString(2) << ": czy obecny - " << rset->getString(3) << endl;
			}
			cout << "SELECT executed correctly" << endl;
		}
		catch (SQLException ex)
		{
			cout << "SELECT executed not correctly" << endl;
			cout << ex.getMessage() << endl;
		}
		stmt->closeResultSet(rset);
		conn->terminateStatement(stmt);
	}
	
	void selectWithParam_3()
	{
		string query = "SELECT Id_osoba, Id_klienta, Telefon_k FROM klient WHERE PESEL = :m";
		string zmienna;
		cout<<"Podaj PESEL szukanego klienta"<<endl;
		cin>>zmienna;
		stmt=conn->createStatement(query);
		stmt->setString(1, zmienna);
		ResultSet *rset = stmt->executeQuery();
		try {
			while (rset->next())
			{
				cout << "Klient o id osoby: " << rset->getInt(1) << ": o id klienta - " << rset->getInt(2) << ": o numerze telefonu - " << rset->getInt(3) << endl;
			}
			cout << "SELECT executed correctly" << endl;
		}
		catch (SQLException ex)
		{
			cout << "SELECT executed not correctly" << endl;
			cout << ex.getMessage() << endl;
		}
		stmt->closeResultSet(rset);
		conn->terminateStatement(stmt);
	}
	
	void selectWithParam_4()
	{
		string query = "SELECT Nr_rejestracyjny, Czy_sprawny, Moc_silnika FROM auta WHERE Id_auta = :m";
		int zmienna;
		cout<<"Podaj id szukanego auta"<<endl;
		cin>>zmienna;
		stmt=conn->createStatement(query);
		stmt->setInt(1, zmienna);
		ResultSet *rset = stmt->executeQuery();
		try {
			while (rset->next())
			{
				cout << "Auto o numerze rejestracyjnym " << rset->getString(1) << ": Czy sprawne - " << rset->getInt(2) << ": Moc - " << rset->getInt(3) << endl;
			}
			cout << "SELECT executed correctly" << endl;
		}
		catch (SQLException ex)
		{
			cout << "SELECT executed not correctly" << endl;
			cout << ex.getMessage() << endl;
		}
		stmt->closeResultSet(rset);
		conn->terminateStatement(stmt);
	}
	
	void selectWithParam_5()
	{
		string query = "SELECT Id_pracownika, Id_klienta, Id_auta FROM uslugi WHERE Id_uslugi = :m";
		int zmienna;
		cout<<"Podaj id szukanej uslugi"<<endl;
		cin>>zmienna;
		stmt=conn->createStatement(query);
		stmt->setInt(1, zmienna);
		ResultSet *rset = stmt->executeQuery();
		try {
			while (rset->next())
			{
				cout << "Usluga o podanym ID: id pracownika = " << rset->getInt(1) << ": id klienta - " << rset->getInt(2) << ": id auta - " << rset->getInt(3) << endl;
			}
			cout << "SELECT executed correctly" << endl;
		}
		catch (SQLException ex)
		{
			cout << "SELECT executed not correctly" << endl;
			cout << ex.getMessage() << endl;
		}
		stmt->closeResultSet(rset);
		conn->terminateStatement(stmt);
	}
	
	void selectWithParam_6()
	{
		string query = "SELECT Id_uslugi, Koszt_naprawy, Rodzaj_naprawy FROM naprawy WHERE Id_naprawy = :m";
		int zmienna;
		cout<<"Podaj id szukanej naprawy"<<endl;
		cin>>zmienna;
		stmt=conn->createStatement(query);
		stmt->setInt(1, zmienna);
		ResultSet *rset = stmt->executeQuery();
		try {
			while (rset->next())
			{
				cout << "Naprawa o podanym ID: id uslugi = " << rset->getInt(1) << ": koszt naprawy - " << rset->getInt(2) << ": rodzaj naprawy - " << rset->getString(3) << endl;
			}
			cout << "SELECT executed correctly" << endl;
		}
		catch (SQLException ex)
		{
			cout << "SELECT executed not correctly" << endl;
			cout << ex.getMessage() << endl;
		}
		stmt->closeResultSet(rset);
		conn->terminateStatement(stmt);
	}
	
	void selectWithParam_7()
	{
		string query = "SELECT Cena_za_h, Cena_za_km, Czy_aktualny FROM cennik WHERE Id_cennika = :m";
		int zmienna;
		cout<<"Podaj id szukanego cennika"<<endl;
		cin>>zmienna;
		stmt=conn->createStatement(query);
		stmt->setInt(1, zmienna);
		ResultSet *rset = stmt->executeQuery();
		try {
			while (rset->next())
			{
				cout << "Cennik o podanym ID: Cena za h = " << rset->getInt(1) << ": Cena za km - " << rset->getInt(2) << ": Czy aktualny - " << rset->getInt(3) << endl;
			}
			cout << "SELECT executed correctly" << endl;
		}
		catch (SQLException ex)
		{
			cout << "SELECT executed not correctly" << endl;
			cout << ex.getMessage() << endl;
		}
		stmt->closeResultSet(rset);
		conn->terminateStatement(stmt);
	}
	
	void selectWithParam_8()
	{
		string query = "SELECT Id_auta, Koszt_naprawy, Inne FROM koszty_eksploatacji WHERE Id_eksploatacji = :m";
		int zmienna;
		cout<<"Podaj id szukanej eksploatacji"<<endl;
		cin>>zmienna;
		stmt=conn->createStatement(query);
		stmt->setInt(1, zmienna);
		ResultSet *rset = stmt->executeQuery();
		try {
			while (rset->next())
			{
				cout << "Eksploatacja o podanym ID: Id auta = " << rset->getInt(1) << ": Koszt naprawy - " << rset->getInt(2) << ": Inne - " << rset->getString(3) << endl;
			}
			cout << "SELECT executed correctly" << endl;
		}
		catch (SQLException ex)
		{
			cout << "SELECT executed not correctly" << endl;
			cout << ex.getMessage() << endl;
		}
		stmt->closeResultSet(rset);
		conn->terminateStatement(stmt);
	}
	
	void selectWithParam_9()
	{
		string query = "SELECT Id_uslugi, Stan_licznika_przed, Id_cennika FROM wypozyczenia WHERE Id_wypozyczenia = :m";
		int zmienna;
		cout<<"Podaj id szukanego wypozyczenia"<<endl;
		cin>>zmienna;
		stmt=conn->createStatement(query);
		stmt->setInt(1, zmienna);
		ResultSet *rset = stmt->executeQuery();
		try {
			while (rset->next())
			{
				cout << "Wypozyczenie o podanym ID: Id uslugi = " << rset->getInt(1) << ": Stan licznika(km) przed - " << rset->getInt(2) << ": Id cennika - " << rset->getInt(3) << endl;
			}
			cout << "SELECT executed correctly" << endl;
		}
		catch (SQLException ex)
		{
			cout << "SELECT executed not correctly" << endl;
			cout << ex.getMessage() << endl;
		}
		stmt->closeResultSet(rset);
		conn->terminateStatement(stmt);
	}
	
	void selectWithParam_10()
	{
		string query = "SELECT Id_wypozyczenia, Stan_licznika_po, Data_godzina_zwrot FROM zwroty WHERE Id_zwrotu = :m";
		int zmienna;
		cout<<"Podaj id szukanego zwrotu"<<endl;
		cin>>zmienna;
		stmt=conn->createStatement(query);
		stmt->setInt(1, zmienna);
		ResultSet *rset = stmt->executeQuery();
		try {
			while (rset->next())
			{
				cout << "Zwrot o podanym ID: Id wypozyczenia = " << rset->getInt(1) << ": Stan licznika po - " << rset->getInt(2) << ": Data zwrotu - " << rset->getString(3) << endl;
			}
			cout << "SELECT executed correctly" << endl;
		}
		catch (SQLException ex)
		{
			cout << "SELECT executed not correctly" << endl;
			cout << ex.getMessage() << endl;
		}
		stmt->closeResultSet(rset);
		conn->terminateStatement(stmt);
	}

	void selectWithParam()
	{
		int x=0;
		cout<<"1. SELECT z tablicy osoba"<<endl;
		cout<<"2. SELECT z tablicy pracownik"<<endl;
		cout<<"3. SELECT z tablicy klient"<<endl;
		cout<<"4. SELECT z tablicy auta"<<endl;
		cout<<"5. SELECT z tablicy uslugi"<<endl;
		cout<<"6. SELECT z tablicy naprawy"<<endl;
		cout<<"7. SELECT z tablicy cennik"<<endl;
		cout<<"8. SELECT z tablicy koszty eksploatacji"<<endl;
		cout<<"9. SELECT z tablicy wypozyczenia"<<endl;
		cout<<"10. SELECT z tablicy zwroty"<<endl;
		cin>>x;
		if(x==1)
		{
			selectWithParam_1();
		}
		else if(x==2)
		{
			selectWithParam_2();
		}
		else if(x==3)
		{
			selectWithParam_3();
		}
		else if(x==4)
		{
			selectWithParam_4();
		}
		else if(x==5)
		{
			selectWithParam_5();
		}
		else if(x==6)
		{
			selectWithParam_6();
		}
		else if(x==7)
		{
			selectWithParam_7();
		}
		else if(x==8)
		{
			selectWithParam_8();
		}
		else if(x==9)
		{
			selectWithParam_9();
		}
		else if(x==10)
		{
			selectWithParam_10();
		}
		else
		{
			return;
		}
	}

	void functionWithParam()
	{
		cout<<"Ktora funkcje z parametrem chcesz wywolac?"<<endl;
		cout<<"1. Osoba z najwieksza iloscia uslug, do wyboru klient lub pracownik"<<endl;
		cout<<"2. Koszty eksploatacji auta o podanej marce"<<endl;
		cout<<"3. Jesli chcesz zobaczyc funkcje bez parametru"<<endl;
		int x=0;
		cin>>x;
		if(x==1)
		{
			int y;
			string id_osoby;
			cout<<"Podaj 1 jesli chcesz wyswietlic klienta, lub 0 jesli pracownika"<<endl;
			cin>>y;
			if(y==1 || y==0)
			{
			stmt = conn->createStatement("BEGIN :r := naj_osoba(:i); END;");
			stmt->registerOutParam(1, Type::OCCISTRING, 128);
			stmt->setInt(2, y);
			try
			{
				stmt->executeUpdate();
				id_osoby=stmt->getString(1);
				cout << "Function executed correctly " << endl;
				cout << "Osoba ktora ma najwiecej uslug to osoba o ID: "<<id_osoby<<endl;
			}
			catch (SQLException ex)
			{
				cout << "Function executed not correctly" << endl;
				cout << ex.getMessage() << endl;
			}
			conn->terminateStatement(stmt);
			}
			else
			{
				return;
			}
		}
		else if(x==2)
		{
			cout<<"Podaj nazwe marki, ktorej koszty eksploatacji chcesz poznac:"<<endl;
			string marka;
			string koszty;
			cin>>marka;
			stmt=conn->createStatement("BEGIN :r := eksploatacja(:i); END;");
			stmt->registerOutParam(1, Type::OCCISTRING, 128);
			stmt->setString(2, marka);
			try
			{
				stmt->executeUpdate();
				koszty=stmt->getString(1);
				cout << "Function executed correctly " << endl;
				cout << "Koszty eksploatacji dla wybranej marki to: "<<koszty<<endl;
			}
			catch (SQLException ex)
			{
				cout << "Function executed not correctly" << endl;
				cout << ex.getMessage() << endl;
			}
			conn->terminateStatement(stmt);
		}
		else if(x==3)
		{
			functionWithoutParam();
		}
		else 
		{
			cout<<"Podano zla cyfre"<<endl;
			return;
		}
	}
	
	void functionWithoutParam()
	{
		int x;
		cout<<"Jedyna funkcja bez parametru jest: "<<endl;
		cout<<"1. Pobranie ilosci wypozyczen wzgledem kwartalow"<<endl;
		cin>>x;
		if(x==1)
		{
			string liczba_wypozyczen;
			stmt=conn->createStatement("BEGIN :r := Ilosc_wyp(); END;");
			stmt->registerOutParam(1, Type::OCCISTRING, 128);
			try
			{
				stmt->executeUpdate();
				liczba_wypozyczen = stmt->getString(1);
				cout << "Function executed correctly " << endl;
				cout << "Najwieksza ilosc wypozyczen w jednym kwartale to: "<<liczba_wypozyczen<<endl;
			}
			catch (SQLException ex)
			{
				cout << "Function executed not correctly" << endl;
				cout << ex.getMessage() << endl;
			}
			conn->terminateStatement(stmt);
		}
		else
		{
			return;
		}
	}

	void createTriggers()
	{
		string triggers[] =
		{
			"CREATE OR REPLACE TRIGGER ograniczenie_przebieg\
			BEFORE INSERT ON uslugi\
			FOR EACH ROW\
			DECLARE\
			v_przebieg auta.Przebieg%TYPE;\
			v_marka VARCHAR2(30);\
			BEGIN\
			SELECT Przebieg INTO v_przebieg FROM auta\
			WHERE Id_auta = :NEW.Id_auta;\
			IF (v_przebieg>100000) THEN\
				SELECT marka INTO v_marka FROM auta\
				WHERE Id_auta = :NEW.Id_auta;\
				RAISE_APPLICATION_ERROR(-20000,'Przebieg auta '  v_marka  ' jest zbyt duzy');\
			END IF;\
			END",
			"CREATE OR REPLACE TRIGGER ograniczenie_nowej_osoby\
			BEFORE INSERT ON osoba\
			FOR EACH ROW\
			DECLARE\
			BEGIN\
			IF :NEW.Imie LIKE '%0%'\
				OR :NEW.Imie Like '%1%'\
				OR :NEW.Imie Like '%2%'\
				OR :NEW.Imie Like '%3%'\
				OR :NEW.Imie Like '%4%'\
				OR :NEW.Imie Like '%5%'\
				OR :NEW.Imie Like '%6%'\
				OR :NEW.Imie Like '%7%'\
				OR :NEW.Imie Like '%8%'\
				OR :NEW.Imie Like '%9%'\
			THEN\
				RAISE_APPLICATION_ERROR(-20000,'Imie nie moze zawierac cyfr!');\
			END IF;\
			IF :NEW.Nazwisko LIKE '%0%'\
				OR :NEW.Nazwisko Like '%1%'\
				OR :NEW.Nazwisko Like '%2%'\
				OR :NEW.Nazwisko Like '%3%'\
				OR :NEW.Nazwisko Like '%4%'\
				OR :NEW.Nazwisko Like '%5%'\
				OR :NEW.Nazwisko Like '%6%'\
				OR :NEW.Nazwisko Like '%7%'\
				OR :NEW.Nazwisko Like '%8%'\
				OR :NEW.Nazwisko Like '%9%'\
			THEN\
				RAISE_APPLICATION_ERROR(-20000,'Nazwisko nie moze zawierac cyfr!');\
			END IF;\
			END",
			"CREATE OR REPLACE TRIGGER pracownik_obecny\
			BEFORE INSERT ON uslugi\
			FOR EACH ROW\
			DECLARE\
			v_obecny pracownik.Czy_obecny%TYPE;\
			v_imie VARCHAR2(28);\
			BEGIN\
			SELECT Czy_obecny INTO v_obecny FROM pracownik\
			WHERE Id_pracownika = :NEW.Id_pracownika;\
			IF (v_obecny = 0) THEN\
				SELECT o.imie INTO v_imie FROM osoba o JOIN pracownik p\
				ON o.Id_osoba = p.Id_osoba\
				WHERE p.Id_pracownika = :NEW.Id_pracownika;\
				RAISE_APPLICATION_ERROR(-20000, 'Pracownik '||v_imie||' jest nieobecny');\
			END IF;\
			END",
			"CREATE OR REPLACE TRIGGER auto_sprawne\
			BEFORE INSERT ON wypozyczenia\
			FOR EACH ROW\
			DECLARE\
			v_sprawny auta.Czy_sprawny%TYPE;\
			v_model auta.Model%TYPE;\
			BEGIN\
			SELECT Czy_Sprawny INTO v_sprawny FROM auta a\
			JOIN uslugi u ON u.Id_auta = a.Id_auta\
			WHERE u.Id_uslugi = :NEW.Id_uslugi;\
			IF v_sprawny=0 THEN\
				SELECT Model INTO v_model FROM auta a\
				JOIN uslugi u ON u.Id_auta = a.Id_auta\
				WHERE u.Id_uslugi = :NEW.Id_uslugi;\
				RAISE_APPLICATION_ERROR(-20000, 'Samochod : '|| v_model || ' jest nie sprawny, wiec nie moze zostac uzyty do wypozyczenia');\
			END IF;\
			END",
			"CREATE OR REPLACE TRIGGER data_odbioru\
			BEFORE INSERT ON naprawy\
			FOR EACH ROW\
			DECLARE\
			BEGIN\
			IF(:NEW.Data_odbioru < :NEW.Data_przyjecia) THEN\
			RAISE_APPLICATION_ERROR(-20000, 'Nie mozna dodac naprawy poniewaz data odbioru jest wczesniejsza od daty przyjecia');\
			END IF;\
			END"
		};

		executeStatement(5, triggers);
	}
	
	void alterAllTriggers()
	{
		int x=0;
		cout<<"Chcesz aktywowac(1) czy dezaktywowac(2) dostepne triggery?\nPodaj 1 lub 2"<<endl;
		cin>>x;
		if(x==1)
		{
			string alters[] =
			{
				"ALTER TRIGGER ograniczenie_przebieg ENABLE",
				"ALTER TRIGGER ograniczenie_nowej_osoby ENABLE",
				"ALTER TRIGGER pracownik_obecny ENABLE",
				"ALTER TRIGGER auto_sprawne ENABLE",
				"ALTER TRIGGER data_odbioru ENABLE"
				
			};
			executeStatement(5, alters);
		}
		else if(x==2)
		{
			string alters[] =
			{
				"ALTER TRIGGER ograniczenie_przebieg DISABLE",
				"ALTER TRIGGER ograniczenie_nowej_osoby DISABLE",
				"ALTER TRIGGER pracownik_obecny DISABLE",
				"ALTER TRIGGER auto_sprawne DISABLE",
				"ALTER TRIGGER data_odbioru DISABLE"
				

			};
			executeStatement(5, alters);
		}
		else
		{
			cout<<"Podano zly numer"<<endl;
			return;
		}
	}
	
	void dropAllTriggers()
	{
		string dropTriggers[] =
		{
			"DROP TRIGGER ograniczenie_przebieg",
			"DROP TRIGGER ograniczenie_nowej_osoby",
			"DROP TRIGGER pracownik_obecny",
			"DROP TRIGGER auto_sprawne",
			"DROP TRIGGER data_odbioru",
		};

		executeStatement(5, dropTriggers);
	}
	
	void selectTables()
	{
			cout<<"1.  SELECT * FROM osoba"<<endl;
			cout<<"2.  SELECT * FROM pracownik"<<endl;
			cout<<"3.  SELECT * FROM klient"<<endl;
			cout<<"4.  SELECT * FROM auta"<<endl;
			cout<<"5.  SELECT * FROM uslugi"<<endl;
			cout<<"6.  SELECT * FROM naprawy"<<endl;
			cout<<"7.  SELECT * FROM cennik"<<endl;
			cout<<"8.  SELECT * FROM koszty_eksploatacji"<<endl;
			cout<<"9.  SELECT * FROM wypozyczenia"<<endl;
			cout<<"10. SELECT * FROM zwroty"<<endl;
		int x;
		string query[] = 
		{
			"SELECT * FROM osoba",
			"SELECT * FROM pracownik",
			"SELECT * FROM klient",
			"SELECT * FROM auta",
			"SELECT * FROM uslugi",
			"SELECT * FROM naprawy",
			"SELECT * FROM cennik",
			"SELECT * FROM koszty_eksploatacji",
			"SELECT * FROM wypozyczenia",
			"SELECT * FROM zwroty"
		};
		cout<<"Wybierz ktora tabele chcesz wyswietlic:"<<endl;
		cin>>x;
		if(x==1)
		{
				stmt = conn->createStatement(query[0]);
				ResultSet *rset = stmt->executeQuery();
				try {
					while(rset->next())
					{
						cout<<"Id_osoba: "<<rset->getInt(1)<<", Imie: "<<rset->getString(2)<<", Nazwisko: "<<rset->getString(3)<<endl;
					}
					cout << "SELECT executed correctly" << endl;
				}
				catch (SQLException ex)
				{
					cout << "SELECT executed not correctly" << endl;
					cout << ex.getMessage() << endl;
				}
				stmt->closeResultSet(rset);
				conn->terminateStatement(stmt);
		}
		else if(x==2)
		{
				stmt = conn->createStatement(query[1]);
				ResultSet *rset = stmt->executeQuery();
				try {
					while(rset->next())
					{
						cout<<"Id_pracownika: "<<rset->getInt(1)<<", Id_osoba: "<<rset->getInt(2)<<", PESEL: "<<rset->getString(3)<<", Data_zatrudnienia: "<<rset->getString(4)<<", Czy_obecny: "<<rset->getInt(5)<<", Telefon_komorkowy: "<<rset->getInt(6)<<endl;
					}
					cout << "SELECT executed correctly" << endl;
				}
				catch (SQLException ex)
				{
					cout << "SELECT executed not correctly" << endl;
					cout << ex.getMessage() << endl;
				}
				stmt->closeResultSet(rset);
				conn->terminateStatement(stmt);
		}
		else if(x==3)
		{
				stmt = conn->createStatement(query[2]);
				ResultSet *rset = stmt->executeQuery();
				try {
					while(rset->next())
					{
						cout<<"Id_klienta: "<<rset->getInt(1)<<", Id_osoba: "<<rset->getInt(2)<<", Data_rejestracji: "<<rset->getString(3)<<", PESEL: "<<rset->getString(4)<<", Telefon_k: "<<rset->getInt(5)<<endl;
					}
					cout << "SELECT executed correctly" << endl;
				}
				catch (SQLException ex)
				{
					cout << "SELECT executed not correctly" << endl;
					cout << ex.getMessage() << endl;
				}
				stmt->closeResultSet(rset);
				conn->terminateStatement(stmt);
		}
		else if(x==4)
		{
				stmt = conn->createStatement(query[3]);
				ResultSet *rset = stmt->executeQuery();
				try {
					while(rset->next())
					{
						cout<<"Id_auta: "<<rset->getInt(1)<<", Nr_rejestracyjny: "<<rset->getString(2)<<", Czy_sprawny: "<<rset->getInt(3)<<", Przebieg: "<<rset->getInt(4)<<", Nr_vin: "<<rset->getString(5)<<", Moc_silnika: "<<rset->getInt(6)<<", Rodzaj_paliwa: "<<rset->getString(7)<<", Rodzaj_nadwozia: "<<rset->getString(8)<<", Marka: "<<rset->getString(9)<<", Model: "<<rset->getString(10)<<", Data_produkcji: "<<rset->getString(11)<<", Uwagi: "<<rset->getString(12)<<endl;
					}
					cout << "SELECT executed correctly" << endl;
				}
				catch (SQLException ex)
				{
					cout << "SELECT executed not correctly" << endl;
					cout << ex.getMessage() << endl;
				}
				stmt->closeResultSet(rset);
				conn->terminateStatement(stmt);
		}
		else if(x==5)
		{
				stmt = conn->createStatement(query[4]);
				ResultSet *rset = stmt->executeQuery();
				try {
					while(rset->next())
					{
						cout<<"Id_uslugi: "<<rset->getInt(1)<<", Id_pracownika: "<<rset->getInt(2)<<", Id_klienta: "<<rset->getInt(3)<<", Id_auta: "<<rset->getInt(4)<<endl;
					}
					cout << "SELECT executed correctly" << endl;
				}
				catch (SQLException ex)
				{
					cout << "SELECT executed not correctly" << endl;
					cout << ex.getMessage() << endl;
				}
				stmt->closeResultSet(rset);
				conn->terminateStatement(stmt);
		}
		else if(x==6)
		{
				stmt = conn->createStatement(query[5]);
				ResultSet *rset = stmt->executeQuery();
				try {
					while(rset->next())
					{
						cout<<"Id_naprawy: "<<rset->getInt(1)<<", Id_uslugi: "<<rset->getInt(2)<<", Koszt_naprawy: "<<rset->getInt(3)<<", Rodzaj_naprawy: "<<rset->getString(4)<<", Data_przyjecia: "<<rset->getString(5)<<", Data_odbioru: "<<rset->getString(6)<<endl;
					}
					cout << "SELECT executed correctly" << endl;
				}
				catch (SQLException ex)
				{
					cout << "SELECT executed not correctly" << endl;
					cout << ex.getMessage() << endl;
				}
				stmt->closeResultSet(rset);
				conn->terminateStatement(stmt);
		}
		else if(x==7)
		{
				stmt = conn->createStatement(query[6]);
				ResultSet *rset = stmt->executeQuery();
				try {
					while(rset->next())
					{
						cout<<"Id_cennika: "<<rset->getInt(1)<<", Cena_za_h: "<<rset->getInt(2)<<", Cena_za_km: "<<rset->getInt(3)<<", Data_zmiany"<<rset->getString(4)<<", Czy_aktualny: "<<rset->getInt(5)<<", Dotyczy_aut: "<<rset->getString(6)<<endl;
					}
					cout << "SELECT executed correctly" << endl;
				}
				catch (SQLException ex)
				{
					cout << "SELECT executed not correctly" << endl;
					cout << ex.getMessage() << endl;
				}
				stmt->closeResultSet(rset);
				conn->terminateStatement(stmt);
		}
		else if(x==8)
		{
				stmt = conn->createStatement(query[7]);
				ResultSet *rset = stmt->executeQuery();
				try {
					while(rset->next())
					{
						cout<<"Id_eksploatacji: "<<rset->getInt(1)<<", Id_auta: "<<rset->getInt(2)<<", Koszt_naprawy: "<<rset->getInt(3)<<", Koszt_wym_ogumienia: "<<rset->getInt(4)<<", Koszt_wym_oleju: "<<rset->getInt(5)<<", Koszt_wym_rozrzadu: "<<rset->getInt(6)<<", Koszt_wym_hamulcow: "<<rset->getInt(7)<<", Koszt_wym_filtrow: "<<rset->getInt(8)<<", Inne: "<<rset->getString(9)<<endl;
					}
					cout << "SELECT executed correctly" << endl;
				}
				catch (SQLException ex)
				{
					cout << "SELECT executed not correctly" << endl;
					cout << ex.getMessage() << endl;
				}
				stmt->closeResultSet(rset);
				conn->terminateStatement(stmt);
		}
		else if(x==9)
		{
				stmt = conn->createStatement(query[8]);
				ResultSet *rset = stmt->executeQuery();
				try {
					while(rset->next())
					{
						cout<<"Id_wypozyczenia: "<<rset->getInt(1)<<", Id_uslugi: "<<rset->getInt(2)<<", Stan_licznika_przed: "<<rset->getInt(3)<<", Data_godzina_wypo: "<<rset->getString(4)<<", Kaucja: "<<rset->getInt(5)<<", Uwagi: "<<rset->getString(6)<<", Id_cennika: "<<rset->getString(7)<<endl;
					}
					cout << "SELECT executed correctly" << endl;
				}
				catch (SQLException ex)
				{
					cout << "SELECT executed not correctly" << endl;
					cout << ex.getMessage() << endl;
				}
				stmt->closeResultSet(rset);
				conn->terminateStatement(stmt);
		}
		else if(x==10)
		{
				stmt = conn->createStatement(query[9]);
				ResultSet *rset = stmt->executeQuery();
				try {
					while(rset->next())
					{
						cout<<"Id_zwrotu: "<<rset->getInt(1)<<", Id_wypozyczenia: "<<rset->getInt(2)<<", Id_pracownika: "<<rset->getInt(3)<<", Data_godzina_zwrot: "<<rset->getString(4)<<", Stan_licznika_po: "<<rset->getInt(5)<<", Uwagi: "<<rset->getString(4)<<endl;
					}
					cout << "SELECT executed correctly" << endl;
				}
				catch (SQLException ex)
				{
					cout << "SELECT executed not correctly" << endl;
					cout << ex.getMessage() << endl;
				}
				stmt->closeResultSet(rset);
				conn->terminateStatement(stmt);
		}
		else
		{
			return;
		}
		
	}
};

	

int main(int argc, char * argv[])
{
	cout << "argc = " << argc << " argv = [";
	for (int i = 0; i < argc; i++)
		cout << argv[i] << ", ";
	cout << "]" << endl;

	if (argc < 3)
	{
		cout << "Usage: " << argv[0] << " user password [database]";
		return 1;
	}
	string user = argv[1];
	string passwd = argv[2];
	string db;
	if (argc < 4)
		db = "";
	else
		db = argv[3];
	try {
		Project *project = new Project(user, passwd, db);
		char command;
		do {
			system("cls");
			cout << "1. CREATE TABLES" << endl;
			cout << "2. DROP TABLES" << endl;
			cout << "3. INSERT INTO TABLES" << endl;
			cout << "4. DELETE FROM TABLES" << endl;
			cout << "5. SELECT WITHOUT PARAM FROM TABLES" << endl;
			cout << "6. SELECT WITH PARAM FROM TABLES" << endl;
			cout << "7. CREATE SEQUENCES" << endl;
			cout << "8. DROP SEQUENCES" << endl;
			cout << "9. CREATE FUNTION" << endl;
			cout << "A. EXEC FUNCTION" << endl;
			cout << "B. DROP FUNTION" << endl;
			cout << "C. CREATE PROCEDURE" << endl;
			cout << "D. EXEC PROCEDURE" << endl;
			cout << "E. DROP PROCEDURE" << endl;
			cout << "F. CREATE TRIGGERS" <<endl;
			cout << "G. ALTER TRIGGERS" <<endl;
			cout << "H. DROP TRIGGERS" <<endl;
			cout << "I. SELECT WHOLE TABLES"<<endl;
			cin >> command;
			switch (command) {
			case '0': break;
			case '1': project->createAllTables(); break;
			case '2': project->dropAllTables(); break;
			case '3': project->insertAll(); break;
			case '4': project->deleteAll(); break;
			case '5': project->selectWithoutParam(); break;
			case '6': project->selectWithParam(); break;
			case '7': project->createAllSequences(); break;
			case '8': project->dropAllSequences(); break;
			case '9': project->createAllFunctions(); break;
			case 'A': project->functionWithParam(); break;
			case 'B': project->dropAllFunctions(); break;
			case 'C': project->createAllProcedures(); break;
			case 'D': project->procedureWithParam(); break;
			case 'E': project->dropAllProcedures(); break;
			case 'F': project->createTriggers(); break;
			case 'G': project->alterAllTriggers(); break;
			case 'H': project->dropAllTriggers(); break;
			case 'I': project->selectTables(); break;
			}
			cout << "# Wcisnij ENTER #" << endl;
			cin.get();
			cin.get();
		} while (command != '0');
		delete (project);
		cout << "Project executed correctly" << endl;
	}
	catch (SQLException ex)
	{
		cout << "Project executed not correctly" << endl;
		cout << ex.getMessage() << endl;
	}
}