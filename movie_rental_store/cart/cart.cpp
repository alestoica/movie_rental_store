#include "cart.h"

using std::shuffle;


void Cart::addToCart(const Movie &m) {
    cartMovies.push_back(m);
    notify();
}

void Cart::emptyCart() {
    cartMovies.clear();
    notify();
}

void Cart::addRandomMovies(vector<Movie> originalMovies, int nr) {
    shuffle(originalMovies.begin(), originalMovies.end(), std::default_random_engine(std::random_device{}()));
    while (cartMovies.size() < nr && !originalMovies.empty()) {
        cartMovies.push_back(originalMovies.back());
        originalMovies.pop_back();
    }
    notify();
}

const vector <Movie>& Cart::getAllFromCart() {
    return cartMovies;
}

void Cart::exportCart(const string &filename) {
    std::ofstream fout(filename);
    fout << "<!DOCTYPE html><html><head></head><body>";
    for (const auto& movie : cartMovies) {
        fout << "<p><span style='font-weight: bold'>" << "Title: " + movie.getTitle() + " | Genre: " + movie.getGenre() + " | Actor: " + movie.getActor() + " | Year: " + std::to_string(movie.getYear()) << "</span></p>";
    }
    fout << "</body></html>";
}
