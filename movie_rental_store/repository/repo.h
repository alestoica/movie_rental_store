#ifndef LAB6_7_REPO_H
#define LAB6_7_REPO_H

#pragma once
#include "../domain/movie.h"

#include <vector>
#include <string>
#include <ostream>
#include <map>
#include <random>

using std::vector;
using std::string;
using std::ostream;
using std::map;


class RepoException {
    string msg;

public:
    RepoException(const string& m): msg{ m } {}

    string getMsg();

    friend ostream& operator <<(ostream& out, const RepoException& ex);
};

ostream& operator <<(ostream& out, const RepoException& ex);


class AbstractRepo {
public:
    virtual ~AbstractRepo() = default;

    virtual bool exist(const string& title, const int& year) const = 0;

    virtual void addElem(const Movie& m) = 0;

    virtual void deleteElem(const string& title, const int& year) = 0;

    virtual const Movie& updateElem(const string& title, const int& year, const string& field, const string& value) = 0;

    virtual const Movie& findElem(const string& title, const int& year) const = 0;

    virtual const vector <Movie>& getAll() const = 0;

    friend bool operator ==(const Movie& m1, const Movie& m2);
};


class LabRepo : public AbstractRepo {
    map <int, Movie> all;
    double probability = 0.5;
    int key = 0;

    int findKey(const string& title, const int& year) const {
        for (int i = 0; i < this->key; i++) {
            if (all.find(i)->second.getTitle() == title && all.find(i)->second.getYear() == year)
                return i;
        }
        return -1;
    }

public:
    LabRepo() = default;

    bool exist(const string& title, const int& year) const override {
        std::mt19937 mt{ std::random_device{}() };
        std::uniform_int_distribution<> dist(0, 10);
        int rN = dist(mt);
        if (rN < this->probability)
            throw RepoException("Probability exception");
        else {
            try {
                findElem(title, year);
                return true;
            }
            catch (RepoException&) {
                return false;
            }
        }
    }

    void addElem(const Movie& m) override {
        std::mt19937 mt{ std::random_device{}() };
        std::uniform_int_distribution<> dist(0, 10);
        int rN = dist(mt);
        if (rN < this->probability)
            throw RepoException("Probability exception");
        else {
            if (exist(m.getTitle(), m.getYear())) {
                throw RepoException("Exista deja filmul cu numele " + m.getTitle() + " si aparut in anul " +
                                    std::to_string(m.getYear()) + "!");
            }
            all.insert({this->key, m});
            this->key++;
        }
    }

    void deleteElem(const string& title, const int& year) override {
        std::mt19937 mt{ std::random_device{}() };
        std::uniform_int_distribution<> dist(0, 10);
        int rN = dist(mt);
        if (rN < this->probability)
            throw RepoException("Probability exception");
        else {
            if (!exist(title, year)) {
                throw RepoException("Nu exista filmul cu titlul " + title + ", lansat in anul " + std::to_string(year) + "!");
            }
            all.erase(findKey(title, year));
        }
    }

    const Movie& updateElem(const string& title, const int& year, const string& field, const string& value) override {
        std::mt19937 mt{ std::random_device{}() };
        std::uniform_int_distribution<> dist(0, 10);
        int rN = dist(mt);
        if (rN < this->probability)
            throw RepoException("Probability exception");
        else {
            if (!exist(title, year)) {
                throw RepoException("Nu exista filmul cu titlul " + title + ", lansat in anul " + std::to_string(year) + "!");
            }
            int current_key = findKey(title, year);
            if (field == "title")
                all.find(current_key)->second.setTitle(value);
            if (field == "genre")
                all.find(current_key)->second.setGenre(value);
            if (field == "actor")
                all.find(current_key)->second.setActor(value);
            if (field == "year")
                all.find(current_key)->second.setYear(value);
            return all.find(current_key)->second;
        }
    }

    const Movie& findElem(const string& title, const int& year) const override {
        std::mt19937 mt{ std::random_device{}() };
        std::uniform_int_distribution<> dist(0, 10);
        int rN = dist(mt);
        if (rN < this->probability)
            throw RepoException("Probability exception");
        else {
            int current_key = findKey(title, year);
            if (current_key != -1)
                return all.find(current_key)->second;
            else
                throw RepoException("Nu exista filmul cu titlul " + title + ", lansat in anul " + std::to_string(year) + "!");
        }
    }

    const vector <Movie>& getAll() const override {
        std::mt19937 mt{ std::random_device{}() };
        std::uniform_int_distribution<> dist(0, 10);
        int rN = dist(mt);
        if (rN < this->probability)
            throw RepoException("Probability exception");
        else {
            vector <Movie> v;
            for (int i = 0; i < this->key; i++) {
                v.push_back(all.find(i)->second);
            }
            return v;
        }
    }
};


class Repo : public AbstractRepo {
    vector <Movie> all;

public:
    Repo() = default;

    Repo(const Repo& r) = delete;

    /// private method that verifies if an movie already exists
    /// \param title: the name of the movie (string)
    /// \param year: the year of the movie's release (int)
    /// \return true if it already exists and false otherwise
    bool exist(const string& title, const int& year) const override;

    /// adds a new movie
    /// \param m: the movie (Movie)
    /// \throws RepoException: if the movie already exists
    virtual void addElem(const Movie& m) override;

    /// deletes a movie
    /// \param title: the name of the movie (string)
    /// \param year: the year of the movie's release (int)
    /// raises RepoException if the movie doesn't exist
    virtual void deleteElem(const string& title, const int& year) override;

    /// updates the given field of a movie with the given value
    /// \param title: the name of the movie (string)
    /// \param year: the year of the movie's release (int)
    /// \param field: the given field (string)
    /// \param value: the given value (string)
    /// \return the updated movie
    virtual const Movie& updateElem(const string& title, const int& year, const string& field, const string& value) override;

    /// finds the given movie
    /// \param title: the name of the movie (string)
    /// \param year: the year of the movie's release (int)
    /// \return the found movie
    const Movie& findElem(const string& title, const int& year) const override;

    /// finds the position of the given movie
    /// \param title: the name of the movie (string)
    /// \param year: the year of the movie's release (int)
    /// \return the position of the found movie
    auto findPosition(const string& title, const int& year) const;

    /// gives the whole list of movies
    /// \return the list of movies
    const vector <Movie>& getAll() const override;

    friend bool operator ==(const Movie& m1, const Movie& m2);
};

bool operator ==(const Movie& m1, const Movie& m2);


class FileRepo : public Repo {
    string filename;

public:
    void loadFromFile();

    void saveToFile();

    FileRepo(const string& f) : Repo(), filename{ f } {
        loadFromFile();
    }

    void addElem(const Movie& m) override {
        Repo::addElem(m);
        saveToFile();
    }

    void deleteElem(const string& title, const int& year) override {
        Repo::deleteElem(title, year);
        saveToFile();
    }

    const Movie& updateElem(const string& title, const int& year, const string& field, const string& value) override {
        const Movie& m = Repo::updateElem(title, year, field, value);
        saveToFile();
        return m;
    }
};


void testRepo();

#endif //LAB6_7_REPO_H
