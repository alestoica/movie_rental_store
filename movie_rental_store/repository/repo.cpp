#include "repo.h"
#include <cassert>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <cstring>

using std::ostream;
using std::stringstream;


const Movie& Repo::findElem(const string& title, const int& year) const {
    Movie m{title, "", "", year};
    auto it = std::find(all.begin(), all.end(), m);
    if (it != all.end())
        return *it;
    else
        throw RepoException("Nu exista filmul cu titlul " + title + ", lansat in anul " + std::to_string(year) + "!");
}

auto Repo::findPosition(const string& title, const int& year) const {
    Movie m{title, "", "", year};
    auto it = std::find(all.begin(), all.end(), m);
    if (it != all.end())
        return std::distance(all.begin(), it);
}

bool Repo::exist(const string& title, const int& year) const {
    try {
        findElem(title, year);
        return true;
    }
    catch (RepoException&) {
        return false;
    }
}

void Repo::addElem(const Movie& m) {
    if (exist(m.getTitle(), m.getYear())) {
        throw RepoException("Exista deja filmul cu numele " + m.getTitle() + " si aparut in anul " + std::to_string(m.getYear()) + "!");
    }
    all.push_back(m);
}

void Repo::deleteElem(const string& title, const int& year) {
    if (!exist(title, year)) {
        throw RepoException("Nu exista filmul cu titlul " + title + ", lansat in anul " + std::to_string(year) + "!");
    }
    all.erase(all.begin() + findPosition(title, year));
}

const Movie& Repo::updateElem(const string& title, const int& year, const string& field, const string& value) {
    if (!exist(title, year)) {
        throw RepoException("Nu exista filmul cu titlul " + title + ", lansat in anul " + std::to_string(year) + "!");
    }
    int pos = findPosition(title, year);
    if (field == "title")
        all[pos].setTitle(value);
    if (field == "genre")
        all[pos].setGenre(value);
    if (field == "actor")
        all[pos].setActor(value);
    if (field == "year")
        all[pos].setYear(value);
    return all[pos];
}

const vector <Movie>& Repo::getAll() const {
    return all;
}

void FileRepo::loadFromFile() {
    std::ifstream fin(filename);

//    if (!fin.is_open())
//        throw RepoException("Nu s-a putut deschide fisierul: " + filename);

    char title[30];
    char genre[30];
    char actor[30];
    int year;
    char new_line[100];
    string line;
    while (!fin.eof()) {
        std::getline(fin, line);
        strcpy(new_line, line.c_str());
        int i = 0;
        char *token = strtok(new_line, "|");
        strcpy(title, token);
        while (token != nullptr) {
            if (i == 1)
                strcpy(genre, token);
            else if (i == 2)
                strcpy(actor, token);
            else if (i == 3)
                year = std::stoi(token);
            i++;
            token = strtok(nullptr, "|");
        }
        Movie m{title, genre, actor, year};
        Repo::addElem(m);
    }
}

void FileRepo::saveToFile() {
    std::ofstream fout(filename);

//    if (!fout.is_open())
//        throw RepoException("Nu s-a putut deschide fisierul: " + filename);

    auto nr = Repo::getAll().size();
    for (const auto& m : Repo::getAll()) {
        fout << m.getTitle() << "|" << m.getGenre() << "|" << m.getActor() << "|" << m.getYear();
        if (nr != 1)
            fout << std::endl;
        nr--;
    }
}


ostream& operator <<(ostream& out, const RepoException& ex) {
    out << ex.msg;
    return out;
}

string RepoException::getMsg() {
    return this->msg;
}

bool operator ==(const Movie& m1, const Movie& m2) {
    if (m1.getTitle() == m2.getTitle() && m1.getYear() == m2.getYear())
        return true;
    return false;
}


void testAdd() {
    Repo repo;

    repo.addElem(Movie{"Babylon", "musical", "Margot Robbie", 2022});
    assert(repo.getAll().size() == 1);

    assert(repo.findElem("Babylon", 2022).getTitle() == "Babylon");
    assert(repo.findElem("Babylon", 2022).getGenre() == "musical");
    assert(repo.findElem("Babylon", 2022).getActor() == "Margot Robbie");
    assert(repo.findElem("Babylon", 2022).getYear() == 2022);

    assert(repo.findPosition("Babylon", 2022) == 0);

    repo.addElem(Movie{"Fight Club", "action", "Edward Norton", 2014});
    assert(repo.getAll().size() == 2);

    try {
        repo.addElem(Movie{"Babylon", "drama", "Someone New", 2022});
    }
    catch (const RepoException& ex) {
        stringstream out;
        out << ex;
        assert(out.str().find("exista") >= 0);
    }
}

void testFind() {
    Repo repo;

    Movie m{"Babylon", "musical", "Margot Robbie", 2022};

    repo.addElem(Movie{"Babylon", "musical", "Margot Robbie", 2022});

    assert(repo.findElem("Babylon", 2022) == m);

    assert(repo.findPosition("Babylon", 2022) == 0);

    try {
        repo.findElem("Idk", 2022);
    }
    catch (RepoException&) {
        assert(true);
    }
}

void testDelete() {
    Repo repo;

    try {
        repo.deleteElem("Babylon", 2022);
    }
    catch (const RepoException&) {
        assert(true);
    }

    repo.addElem(Movie{"Babylon", "musical", "Margot Robbie", 2022});
    repo.addElem(Movie{"Fight Club", "action", "Edward Norton", 2014});
    assert(repo.getAll().size() == 2);

    repo.deleteElem("Babylon", 2022);
    assert(repo.getAll().size() == 1);
}

void testUpdate() {
    Repo repo;

    try {
        repo.updateElem("Babylon", 2022, "title", "NewBabylon");
    }
    catch (const RepoException&) {
        assert(true);
    }

    Movie m{"Babylon", "musical", "Margot Robbie", 2022};
    repo.addElem(m);

    repo.updateElem("Babylon", 2022, "title", "NewBabylon");
    assert(repo.getAll()[0].getTitle() == "NewBabylon");

    repo.updateElem("NewBabylon", 2022, "genre", "NewMusical");
    assert(repo.getAll()[0].getGenre() == "NewMusical");

    repo.updateElem("NewBabylon", 2022, "actor", "NewActor");
    assert(repo.getAll()[0].getActor() == "NewActor");

    repo.updateElem("NewBabylon", 2022, "year", "2023");
    assert(repo.getAll()[0].getYear() == 2023);

    Movie new_m{"Babylon", "musical", "Margot Robbie", 2022};
    assert(bool(repo.getAll()[0] == new_m) == false);
}

void testFileRepo() {
    FileRepo repo{"test.txt"};

//    repo.loadFromFile();
    assert(repo.getAll().size() == 4);

    Movie m{"Years and Years", "drama", "Emma Thompson", 2020};
    repo.addElem(m);
//    repo.loadFromFile();
    assert(repo.getAll().size() == 5);

    repo.deleteElem("Years and Years", 2020);
//    repo.loadFromFile();
    assert(repo.getAll().size() == 4);

    repo.updateElem("Babylon", 2022, "genre", "mystery");
//    repo.loadFromFile();
    assert(repo.findElem("Babylon", 2022).getGenre() == "mystery");

    repo.updateElem("Babylon", 2022, "genre", "comedy");
}

void testRepo() {
    testAdd();
    testDelete();
    testFind();
    testUpdate();
    testFileRepo();
}