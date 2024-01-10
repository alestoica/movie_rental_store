#ifndef LAB8_9_CART_H
#define LAB8_9_CART_H

#pragma once
#include "../domain/movie.h"
#include "../observer.h"

#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>

using std::vector;

class Cart : public Observable {
    vector <Movie> cartMovies;

public:
    Cart()=default;

    /// adds a new movie in the movie cart
    /// \param m: the movie (Movie)
    void addToCart(const Movie& m);

    /// empties the movie cart
    void emptyCart();

    /// adds a given number of randomly chosen movies
    /// (from a given list of movies) in the movie cart
    /// \param originalMovies: the given list of movies (vector <Movie>)
    /// \param nr: the given number of movies (int)
    void addRandomMovies(vector <Movie> originalMovies, int nr);

    /// gives the whole list of movies from the cart
    /// \return the list of movies from the cart
    const vector <Movie>& getAllFromCart();

    /// saves the contents of the cart to the given file
    /// \param filename: the name of the given file (string)
    void exportCart(const string& filename);
};

#endif //LAB8_9_CART_H
