#ifndef LAB6_7_MOVIE_H
#define LAB6_7_MOVIE_H

#pragma once
#include <string>
#include <iostream>

using std::string;
using std::cout;


class Movie {
    string title;
    string genre;
    string actor;
    int year;

public:
    Movie() = default;

    Movie(const string& t, const string& g, const string& a, int y): title{ t }, genre{ g }, actor{ a }, year{ y } {}

    Movie(const Movie& ot): title{ ot.title }, genre{ ot.genre }, actor{ ot.actor }, year{ ot.year } {
//        cout << "copy constructor called\n";
    }

    string getTitle() const {
        return title;
    }

    string getGenre() const {
        return genre;
    }

    string getActor() const {
        return actor;
    }

    int getYear() const noexcept {
        return year;
    }

    void setTitle(const string& new_title) {
        title = new_title;
    }

    void setGenre(const string& new_genre) {
        genre = new_genre;
    }

    void setActor(const string& new_actor) {
        actor =  new_actor;
    }

    void setYear(const string& new_year) {
        year = stoi(new_year);
    }
};

/// compares two movies by the title
/// \param m1: first movie (Movie)
/// \param m2: second movie (Movie)
/// \return true if m1 < m2 false otherwise
bool cmpTitle(const Movie& m1, const Movie& m2);

/// compares two movies by the actor
/// \param m1: first movie (Movie)
/// \param m2: second movie (Movie)
/// \return true if m1 < m2 false otherwise
bool cmpActor(const Movie& m1, const Movie& m2);

/// compares two movies by the year and the genre
/// \param m1: first movie (Movie)
/// \param m2: second movie (Movie)
/// \return true if m1 < m2 false otherwise
bool cmpYearGenre(const Movie& m1, const Movie& m2);

#endif //LAB6_7_MOVIE_H
