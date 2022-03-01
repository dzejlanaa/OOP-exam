#include<iostream>
#include<exception>
#include<regex>
#include<vector>
#include<string>
#include<thread>
#include<mutex>

using namespace std;

const char* crt = "\n-------------------------------------------\n";
enum Predmet { UIT, PRI, PRII, PRIII, RSI, RSII };
enum Dupliranje { SA_DUPLIKATIMA, BEZ_DUPLIKATA };
const char* NIJE_VALIDNA = "<VRIJEDNOST_NIJE_VALIDNA>";



bool ValidirajLozinku(string tekst)
{
    return regex_match(tekst, regex("(?=.{6,7})([*_])([0-9])([a-z])([\\s]?)([A-Z])([0-9])([a-z])"));
}


char* GetNizKaraktera(const char* sadrzaj, bool dealociraj = false) {
    if (sadrzaj == nullptr)return nullptr;
    int vel = strlen(sadrzaj) + 1;
    char* temp = new char[vel];
    strcpy_s(temp, vel, sadrzaj);
    if (dealociraj)
        delete[]sadrzaj;
    return temp;
}

template<class T1, class T2>
class Kolekcija {
    T1* _elementi1;
    T2* _elementi2;
    int* _trenutno;
    Dupliranje _dupliranje;
public:
    Kolekcija(Dupliranje dupliranje = SA_DUPLIKATIMA) {
        _trenutno = new int(0);
        _elementi1 = nullptr;
        _elementi2 = nullptr;
        _dupliranje = dupliranje;
    }


    Kolekcija(const Kolekcija& k)
    {
        _dupliranje = k._dupliranje;
        _trenutno = new int(*k._trenutno);
        _elementi1 = new T1[*k._trenutno];
        _elementi2 = new T2[*k._trenutno];
        for (int i = 0; i < *k._trenutno; i++)
        {
            _elementi1[i] = k._elementi1[i];
            _elementi2[i] = k._elementi2[i];
        }

    }

    Kolekcija& operator=(const Kolekcija& k)
    {
        if (this != &k)
        {
            delete[]_elementi1;
            delete[]_elementi2;
            delete _trenutno;
            _dupliranje = k._dupliranje;
            _trenutno = new int(*k._trenutno);
            _elementi1 = new T1[*k._trenutno];
            _elementi2 = new T2[*k._trenutno];
            for (int i = 0; i < *k._trenutno; i++)
            {
                _elementi1[i] = k._elementi1[i];
                _elementi2[i] = k._elementi2[i];
            }


        }
        return *this;
    }


    void AddElement(T1  el1, T2  el2)
    {

        for (int i = 0; i < *_trenutno; i++)
        {
            if (_dupliranje == BEZ_DUPLIKATA && (_elementi1[i] == el1 && _elementi2[i] == el2))
                throw exception("Dupli element");
        }

        T1* temp1 = new T1[*_trenutno + 1];
        T2* temp2 = new T2[*_trenutno + 1];
        for (int i = 0; i < *_trenutno; i++)
        {
            temp1[i] = _elementi1[i];
            temp2[i] = _elementi2[i];
        }
        delete[]_elementi1;
        delete[]_elementi2;
        temp1[*_trenutno] = el1;
        temp2[*_trenutno] = el2;
        (*_trenutno)++;
        _elementi1 = temp1;
        _elementi2 = temp2;
    }
    ~Kolekcija() {
        delete _trenutno; _trenutno = nullptr;
        delete[] _elementi1; _elementi1 = nullptr;
        delete[] _elementi2; _elementi2 = nullptr;
    }

  
    Kolekcija<T1, T2>  operator[](const T1& el)
    {
        Kolekcija<T1, T2> temp;
        for (int i = 0; i < *_trenutno; i++)
        {
            if (_elementi1[i] == el)
                temp.AddElement(_elementi1[i], _elementi2[i]);
        }
        return temp;
    }


    T1 getElement1(int lokacija)const { return _elementi1[lokacija]; }
    T2 getElement2(int lokacija)const { return _elementi2[lokacija]; }
    int getTrenutno()const { return *_trenutno; }
    friend ostream& operator<< (ostream& COUT, const Kolekcija& obj) {
        for (size_t i = 0; i < obj.getTrenutno(); i++)
            COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
        return COUT;
    }
};
class Datum {
    int* _dan, * _mjesec, * _godina;
public:
    Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
        _dan = new int(dan);
        _mjesec = new int(mjesec);
        _godina = new int(godina);
    }

    Datum& operator=(const Datum& d)
    {
        if (this != &d)
        {
            *_dan = *d._dan;
            *_mjesec = *d._mjesec;
            *_godina = *d._godina;
        }
        return *this;
    }

    bool operator==(const Datum& d)
    {
        return *_dan == *d._dan && *_mjesec == *d._mjesec && *_godina == *d._godina;
    }
    ~Datum() {
        delete _dan; _dan = nullptr;
        delete _mjesec; _mjesec = nullptr;
        delete _godina; _godina = nullptr;
    }
    friend ostream& operator<< (ostream& COUT, const Datum& obj) {
        COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
        return COUT;
    }
    int GetDani()const
    {
        return *_dan + *_mjesec * 30 + *_godina * 365;
    }

    int operator-(const Datum& d)
    {
        return GetDani() - d.GetDani();
    }
    Datum(const Datum& obj) {
        _dan = new int(*obj._dan);
        _mjesec = new int(*obj._mjesec);
        _godina = new int(*obj._godina);
    }
};
class Pitanje {
    char* _sadrzaj;
    //int se odnosi na ocjenu u opsegu  1 – 5, a Datum na datum kada je odgovor/rjesenje ocijenjeno
    Kolekcija<int, Datum*> _ocjeneRjesenja;

 
public:
    Pitanje(const char* sadrzaj = "") {
        _sadrzaj = GetNizKaraktera(sadrzaj);
    }
    Pitanje(const Pitanje& p)
    {
        _sadrzaj = GetNizKaraktera(p._sadrzaj);
        _ocjeneRjesenja = p._ocjeneRjesenja;
    }

    Pitanje& operator=(const Pitanje& p)
    {
        if (this != &p)
        {
            delete[]_sadrzaj;
            _sadrzaj = GetNizKaraktera(p._sadrzaj);
            _ocjeneRjesenja = p._ocjeneRjesenja;

        }
        return *this;
    }

    ~Pitanje() {
        delete[] _sadrzaj; _sadrzaj = nullptr;
    }
    char* GetSadrzaj() { return _sadrzaj; }
    Kolekcija<int, Datum*>& GetOcjene() { return _ocjeneRjesenja; }

   
    bool AddOcjena(int ocjena, Datum& datum)
    {
        for (int i = 0; i < _ocjeneRjesenja.getTrenutno(); i++)
        {
            if (datum - *_ocjeneRjesenja.getElement2(i) < 3)
                return false;
        }
        _ocjeneRjesenja.AddElement(ocjena, &datum);
        return true;


    }
    float GetProsjek() const
    {
        float prosjek = 0;

        for (int i = 0; i < _ocjeneRjesenja.getTrenutno(); i++)
        {
            prosjek += _ocjeneRjesenja.getElement1(i);
        }

        prosjek /= _ocjeneRjesenja.getTrenutno();

        return prosjek;
    }
    bool operator==(Pitanje& drugo)
    {
        return strcmp(_sadrzaj, drugo._sadrzaj) == 0;
    }

    float GetProsjekPitanja()const
    {
        float prosjek = 0;
        if (_ocjeneRjesenja.getTrenutno() == 0)
            return prosjek;
        for (int i = 0; i < _ocjeneRjesenja.getTrenutno(); i++)
            prosjek += _ocjeneRjesenja.getElement1(i);
        return prosjek / _ocjeneRjesenja.getTrenutno();
    }

  
    friend ostream& operator<< (ostream& COUT, Pitanje  obj) 
    {
        COUT << "Sadrzaj pitanja-> " << obj._sadrzaj << endl;
        COUT << ":: OCJENE RJESENJA ::" << endl;
        for (int i = 0; i < obj.GetOcjene().getTrenutno(); i++)
            COUT << "Ocjena-> " << obj.GetOcjene().getElement1(i) << "     postignuta -> " << *obj.GetOcjene().getElement2(i) << endl;
        COUT << "Prosjek ocjena za pitanje je -> " << obj.GetProsjek() << endl;
        return COUT;
    }


};

class Ispit {
    Predmet _predmet;
    //string se odnosi na napomenu/zapazanje nastavnika
    Kolekcija<Pitanje, string> _pitanjaOdgovori;
public:
    Ispit(Predmet predmet = UIT) {
        _predmet = predmet;
    }
    Kolekcija<Pitanje, string>& GetPitanjaOdgovore() { return _pitanjaOdgovori; }
    Predmet GetPredmet() { return _predmet; }
    friend ostream& operator<< (ostream& COUT, const Ispit& obj) {
        COUT << obj._predmet << endl;
        for (size_t i = 0; i < obj._pitanjaOdgovori.getTrenutno(); i++)
        {
            COUT << obj._pitanjaOdgovori << endl;
        }
        return COUT;
    }
    Ispit(const Ispit& original) : _predmet(original._predmet), _pitanjaOdgovori(original._pitanjaOdgovori) {}

    Ispit& operator=(const Ispit& drugi)
    {
        if (this != &drugi)
        {
            _predmet = drugi._predmet;
            _pitanjaOdgovori = drugi._pitanjaOdgovori;
        }
        return *this;
    }

    bool AddPitanje(Pitanje pitanje, string napomena)
    {

        _pitanjaOdgovori.AddElement(pitanje, napomena);
        return true;
    }
    float GetProsjekSvihPitanja()
    {
        float prosjek = 0;
        if (_pitanjaOdgovori.getTrenutno() == 0)
            return prosjek;
        for (int i = 0; i < _pitanjaOdgovori.getTrenutno(); i++)
            prosjek += _pitanjaOdgovori.getElement1(i).GetProsjek();
        return prosjek / _pitanjaOdgovori.getTrenutno();
    }
};
class Korisnik {
    char* _imePrezime;
    string _emailAdresa;
    string _lozinka;
   
public:
    Korisnik(const char* imePrezime, string emailAdresa, string lozinka)
    {
        _imePrezime = GetNizKaraktera(imePrezime);
        _emailAdresa = emailAdresa;
        _lozinka = ValidirajLozinku(lozinka) ? lozinka : NIJE_VALIDNA;
    }

    Korisnik(const Korisnik& k)
    {
        _imePrezime = GetNizKaraktera(k._imePrezime);
        _emailAdresa = k._emailAdresa;
        _lozinka = k._lozinka;
    }

    Korisnik& operator=(const Korisnik& k)
    {
        if (this != &k)
        {
            delete[] _imePrezime;
            _imePrezime = GetNizKaraktera(k._imePrezime);
            _emailAdresa = k._emailAdresa;
            _lozinka = k._lozinka;
        }
        return *this;

    }

    ~Korisnik() { delete[] _imePrezime; _imePrezime = nullptr; }
    string GetEmail() { return _emailAdresa; }
    string GetLozinka() { return _lozinka; }
    char* GetImePrezime() { return _imePrezime; }
    virtual void Info() = 0;
};

mutex m;

class Kandidat : public Korisnik {
    vector<Ispit*> _polozeniPredmeti;
    void PosaljiEmail(const char* sadrzajPitanja, float prosjekPitanja)
    {
        m.lock();
        cout << "FROM: info@kursevi.ba\nTO: " << GetEmail() << "\n Postovani " << GetImePrezime() << ", evidentirana vam je ocjena->   za odgovor na pitanje-> " << sadrzajPitanja;
        cout << ".Dosadasnji uspjeh za pitanje-> " << sadrzajPitanja << " iznosi-> " << prosjekPitanja << ", a ukupni uspjeh na svim predmetima iznosi -> " << ProsjekSvihOdgovora() << endl;
        cout << "Pozdrav.\nEDUTeam.\n";
        m.unlock();
    }

public:
    Kandidat(const char* imePrezime, string emailAdresa, string lozinka) : Korisnik(imePrezime, emailAdresa, lozinka) {
    }
    Kandidat(const Kandidat& original) : Korisnik(original)
    {
        for (int i = 0; i < original._polozeniPredmeti.size(); i++)
            _polozeniPredmeti.push_back(new Ispit(*original._polozeniPredmeti[i]));
    }

    ~Kandidat() {
        for (size_t i = 0; i < _polozeniPredmeti.size(); i++)
            delete _polozeniPredmeti[i];
    }
    friend ostream& operator<< (ostream& COUT, Kandidat& obj) {
        COUT << obj.GetImePrezime() << " " << obj.GetEmail() << " " << obj.GetLozinka() << endl;
        for (size_t i = 0; i < obj._polozeniPredmeti.size(); i++)
            COUT << *obj._polozeniPredmeti[i];
        return COUT;
    }

    void Info()
    {}

    
    bool PostojeLiIsti(Predmet predmet, Pitanje pitanje)
    {
        for (int i = 0; i < _polozeniPredmeti.size(); i++)
            if (_polozeniPredmeti[i]->GetPredmet() == predmet)
                for (int j = 0; j < _polozeniPredmeti[i]->GetPitanjaOdgovore().getTrenutno(); j++)
                    if (_polozeniPredmeti[i]->GetPitanjaOdgovore().getElement1(j) == pitanje)
                        return true;
        return false;
    }

    bool AddPitanje(Predmet predmet, Pitanje pitanje, string napomena = "-----")
    {

        for (int i = 0; i < _polozeniPredmeti.size(); i++)
        {

            if (predmet > _polozeniPredmeti[i]->GetPredmet() && (_polozeniPredmeti[i]->GetPitanjaOdgovore().getTrenutno() < 3 || _polozeniPredmeti[i]->GetProsjekSvihPitanja() < 3.5))
                return false;

            if (PostojeLiIsti(predmet, pitanje))
                return false;

            _polozeniPredmeti[i]->AddPitanje(pitanje, napomena);
            const char* sadrzaj = GetNizKaraktera(pitanje.GetSadrzaj());
            float prosjekPitanja = pitanje.GetProsjekPitanja();
            thread t1(&Kandidat::PosaljiEmail, this, sadrzaj, prosjekPitanja);
            t1.join();
            return true;

        }

        Ispit u(predmet);
        u.AddPitanje(pitanje, napomena);
        _polozeniPredmeti.push_back(new Ispit(u));

        const char* sadrzaj = GetNizKaraktera(pitanje.GetSadrzaj());
        float prosjekPitanja = pitanje.GetProsjekPitanja();
        thread t1(&Kandidat::PosaljiEmail, this, sadrzaj, prosjekPitanja);
        t1.join();
        return true;
    }

    float ProsjekSvihOdgovora()
    {
        float prosjek = 0;
        if (_polozeniPredmeti.size() == 0)
            return prosjek;
        for (int i = 0; i < _polozeniPredmeti.size(); i++)
            prosjek += _polozeniPredmeti[i]->GetProsjekSvihPitanja();
        return prosjek / _polozeniPredmeti.size();
    }



    vector<Ispit*>& GetPolozeniPredmeti() { return _polozeniPredmeti; }

};

void main() {


    Datum   datum19062020(19, 6, 2020),
        datum20062020(20, 6, 2020),
        datum30062020(30, 6, 2020),
        datum05072020(5, 7, 2020);

    int kolekcijaTestSize = 10;

    Kolekcija<int, int> kolekcija1(BEZ_DUPLIKATA);
    for (int i = 0; i < kolekcijaTestSize; i++)
        kolekcija1.AddElement(i, i);

    cout << kolekcija1 << endl;

    try {
        /*ukoliko dupliranje vrijednosti nije dozvoljeno (BEZ_DUPLIKATA)
        metoda AddElement baca izuzetak u slucaju da se pokusa dodati par sa vrijednostima
        identicnim postojecem paru unutar kolekcije */
        kolekcija1.AddElement(3, 3);
    }
    catch (exception& err) {
        cout << crt << "Greska -> " << err.what() << crt;
    }
    cout << kolekcija1 << crt;

    Kolekcija<int, int> kolekcija2;
    kolekcija2 = kolekcija1;
    cout << kolekcija2 << crt;

    /*na osnovu vrijednosti tip T1 (u primjeru vrijednost 1) pronalazi i vraca iz kolekcije sve parove
    koji kao vrijednost T1 imaju proslijedjenu vrijednost*/
    cout << kolekcija1[1] << crt;
    /* npr. ako unutar kolekcije postoje parovi:
    0 0
    1 9
    1 1
    2 2
    3 3
    ispis dobijenih/vracenih vrijednosti ce biti sljedeci:
    1 9
    1 1
    */

    Pitanje sortiranjeNiza("Navedite algoritme za sortiranje clanova niza."),
        dinamickaMemorija("Navedite pristupe za upravljanje dinamickom memorijom."),
        visenitnoProgramiranje("Na koji se sve nacin moze koristiti veci broj niti tokom izvrsenja programa."),
        regex("Navedite par primjera regex validacije podataka.");

    /*svako pitanje moze imati vise ocjena tj. razlicita rjesenja/odgovori se mogu postaviti u vise navrata.
        -   razmak izmedju postavljanja dva rjesenja mora biti najmanje 3 dana
        -   nije dozvoljeno dodati ocjenu sa ranijim datumom u odnosu na vec evidentirane (bez obzira sto je stariji od 3 dana)
    */
    if (sortiranjeNiza.AddOcjena(1, datum19062020))
        cout << "Ocjena evidentirana!" << endl;
    if (!sortiranjeNiza.AddOcjena(5, datum20062020))
        cout << "Ocjena NIJE evidentirana!" << endl;
    if (sortiranjeNiza.AddOcjena(5, datum30062020))
        cout << "Ocjena evidentirana!" << endl;

    // ispisuje sadrzaj/tekst pitanja, ocjene (zajedno sa datumom) i prosjecnu ocjenu za sve odgovore/rjesenja
     // ukoliko pitanje nema niti jednu ocjenu prosjecna treba biti 0
    cout << sortiranjeNiza << endl;

    if (ValidirajLozinku("*2gT2x"))
        cout << "Lozinka validna" << endl;
    if (ValidirajLozinku("*7aT2x"))
        cout << "Lozinka validna" << endl;
    if (ValidirajLozinku("_6gU9z"))
        cout << "Lozinka validna" << endl;
    if (ValidirajLozinku("*3aB1y"))
        cout << "Lozinka validna" << endl;
    if (ValidirajLozinku("*1a T2l"))
        cout << "Lozinka validna" << endl;
    if (!ValidirajLozinku("-1a T2l"))
        cout << "Lozinka NIJE validna" << endl;

    ///*
    //za autentifikaciju svaki korisnik mora posjedovati lozinku koja sadrzi 6 ili 7 znakova postujuci sljedeca pravila:
    //-   pocinje sa znak * (zvjezdica) ili _ (donja crtica), nakon cega slijedi
    //-   broj, nakon cega slijedi
    //-   malo slovo, nakon cega slijedi
    //-   razmak koji NIJE OBAVEZAN, nakon cega slijedi
    //-   veliko slovo, nakon cega slijedi
    //-   broj, nakon cega slijedi
    //-   malo slovo.
    //za provjeru validnosti lozinke koristiti globalnu funkciju ValidirajLozinku, a unutar nje regex metode.
    //validacija lozinke se vrsi unutar konstruktora klase Korisnik, a u slucaju da nije validna
    //postaviti je na podrazumijevanu vrijednost: <VRIJEDNOST_NIJE_VALIDNA>
    //*/

    Korisnik* jasmin = new Kandidat("Jasmin Azemovic", "jasmin@kursevi.ba", "*2gT2x");
    Korisnik* adel = new Kandidat("Adel Handzic", "adel@edu.kursevi.ba", "_6gU9z");
    Korisnik* lozinkaNijeValidna = new Kandidat("John Doe", "john.doe@google.com", "johndoe");

    ///*
    //svi odgovori na nivou jednog predmeta (PRI, PRII... ) se evidentiraju unutar istog objekta tipa Ispit tj. pripadajuceg objekta tipa Pitanje,
    //tom prilikom onemoguciti:
    //- dodavanje istih (moraju biti identicne vrijednosti svih atributa) odgovora na nivou jednog predmeta,
    //- dodavanje odgovora za visi predmet ako prethodni predmet nema evidentirana najmanje 3 pitanja ili nema prosjecnu ocjenu svih pitanja vecu od 3.5
    //(onemoguciti dodavanje pitanja za PRII ako ne postoje najmanje 3 pitanja za predmet PRI ili njihov prosjek nije veci od 3.5)
    //funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
    //*/

    //ukoliko je potrebno, doraditi klase da nacin da omoguce izvrsenje naredne linije koda
    Kandidat* jasminPolaznik = dynamic_cast<Kandidat*>(jasmin);

    if (jasminPolaznik != nullptr) {
        if (jasminPolaznik->AddPitanje(PRI, dinamickaMemorija, "nedostaje operator delete"))
            cout << "Pitanje uspjesno dodano!" << crt;
        //ne treba dodati visenitnoProgramiranje jer ne postoje evidentirana 3 pitanja za predmet PRI
        if (!jasminPolaznik->AddPitanje(PRII, visenitnoProgramiranje))
            cout << "Pitanje NIJE uspjesno dodano!" << crt;
        if (jasminPolaznik->AddPitanje(PRI, visenitnoProgramiranje))
            cout << "Pitanje uspjesno dodano!" << crt;
        if (jasminPolaznik->AddPitanje(PRI, regex, "unutar posljednjeg dijela nedostaje opis glavnih operatora"))
            cout << "Pitanje uspjesno dodano!" << crt;
        if (jasminPolaznik->AddPitanje(PRI, sortiranjeNiza))
            cout << "Pitanje uspjesno dodano!" << crt;
        //ne treba dodati sortiranjeNiza jer je vec dodana za predmet PRI
        if (!jasminPolaznik->AddPitanje(PRI, sortiranjeNiza))
            cout << "Pitanje NIJE uspjesno dodano!" << crt;

        //ispisuje sve dostupne podatke o kandidatu
        cout << *jasminPolaznik << crt;

        
        ///*nakon evidentiranja ocjene na bilo kojem odgovoru, kandidatu se salje email sa porukom:
        //FROM:info@kursevi.ba
        //TO: emailKorisnika
        //Postovani ime i prezime, evidentirana vam je ocjena X za odgovor na pitanje Y. Dosadasnji uspjeh (prosjek ocjena)
        //za pitanje Y iznosi F, a ukupni uspjeh (prosjek ocjena) na svim predmetima iznosi Z.
        //Pozdrav.
        //EDUTeam.
        //slanje email poruka implemenitrati koristeci zasebne thread-ove.
        //*/

        delete jasmin;
        delete adel;
        delete lozinkaNijeValidna;
   
    }
    cin.get();
    system("pause>0");
}
