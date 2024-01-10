#include "movie.h"

bool cmpTitle(const Movie& m1, const Movie& m2) {
    return m1.getTitle() < m2.getTitle();
}

bool cmpActor(const Movie& m1, const Movie& m2) {
    return m1.getActor() < m2.getActor();
}

bool cmpYearGenre(const Movie& m1, const Movie& m2) {
    return (m1.getYear() < m2.getYear() || (m1.getYear() == m2.getYear() && (m1.getGenre() < m2.getGenre())));
}