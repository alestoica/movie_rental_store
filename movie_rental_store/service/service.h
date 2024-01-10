#ifndef LAB6_7_SERVICE_H
#define LAB6_7_SERVICE_H

#pragma once
#include "../domain/movie.h"
#include "../repository/repo.h"
#include "../validators/validator.h"
#include "../undo/undo.h"
#include "../cart/cart.h"

#include <string>
#include <vector>
#include <functional>
#include <map>
#include <memory>
#include <map>

using std::vector;
using std::function;
using std::map;
using std::unique_ptr;
using std::map;


class Service {
    AbstractRepo& repo;
    MovieValidator& val;

    vector<unique_ptr<UndoAction>> undoActions;

    Cart currentCart;

    /// general sorting function
    /// \param cmpF: function that compares 2 movies based on different fields
    /// \return the sorted list
    vector <Movie> generalSort(bool (*cmpF)(const Movie&, const Movie&));

    /// general filtering function
    /// \param fct: function or lambda
    /// \return the filtered list
    vector <Movie> filter(const function <bool(const Movie&)>& fct);

public:
    Service(AbstractRepo& _repo, MovieValidator& _val): repo{ _repo }, val {_val} {}

    Service(const Service& s) = delete;

    /// adds a movie with the given values
    /// \param title: the title of the movie (string)
    /// \param genre: the genre of the movie (string)
    /// \param actor: the main actor of the movie (string)
    /// \param year: the release year of the movie (int)
    /// raises ValidationException if the given values are not correct
    void addMovie(const string& title, const string& genre, const string& actor, const int& year);

    /// deletes a movie with the given values
    /// \param title: the title of the movie (string)
    /// \param year: the release year of the movie (int)
    /// raises ValidationException if the given values are not correct
    void deleteMovie(const string& title, const int& year);

    /// updates the the given field of the movie
    /// with the given values using the given value
    /// \param title: the title of the movie (string)
    /// \param year: the release year of the movie (int)
    /// \param field: the given field (string)
    /// \param value: the given value (string)
    /// \return the updated movie
    /// raises ValidationException if the given values are not correct
    const Movie& updateMovie(const string& title, const int& year, const string& field, const string& value);

    /// finds the movie with the given values
    /// \param title: the title of the movie (string)
    /// \param year: the release year of the movie (int)
    /// \return the found movie
    /// raises ValidationException if the given values are not correct
    const Movie& findMovie(const string& title, const int& year);

    /// gives the whole list of movies
    /// \return the list of movies
    const vector <Movie>& getAll() const;

    map <string, int> getMovieGenres() const;

    /// sorts the list of movies by title
    /// \return the sorted list
    vector <Movie> sortByTitle();

    /// sorts the list of movies by actor
    /// \return the sorted list
    vector <Movie> sortByActor();

    /// sorts the list of movies by year and genre
    /// \return the sorted list
    vector <Movie> sortByYearGenre();

    /// filters the list of movies by the given title
    /// \param title: the given title
    /// \return the filtered list
    vector <Movie> filterTitle(const string& title);

    /// filters the list of movies by the given year
    /// \param year: the given year
    /// \return the filtered list
    vector <Movie> filterYear(int year);

    /// adds the movie with the given title and year in the movie cart
    /// \param title: the title of the movie (string)
    /// \param year: the release year of the movie (int)
    void addToCart(const string& title, const int& year);

    /// adds a given number of randomly chosen movies
    /// (from a given list of movies) in the movie cart
    /// \param nr: the given number of movies (int)
    /// \return how many movies have been added
    int addRandomToCart(int nr);

    /// empties the movie cart
    void emptyCart();

    /// gives the whole list of movies from the cart
    /// \return the list of movies from the cart
    const vector <Movie>& getAllFromCart();

    /// returns a cart object
    /// \return the cart object
    Cart& getCart();

    /// saves the contents of the cart to the given file
    /// \param filename: the name of the given file (string)
    void exportCart(const string& filename);

    /// undoes the last operation
    void undo();
};

void testService();

#endif //LAB6_7_SERVICE_H
