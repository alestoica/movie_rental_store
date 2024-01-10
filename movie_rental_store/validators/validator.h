#ifndef LAB6_7_VALIDATOR_H
#define LAB6_7_VALIDATOR_H

#pragma once
#include "../domain/movie.h"
#include <string>
#include <vector>

using std::vector;
using std::string;
using std::ostream;


class MovieValidator {
public:
    /// validates if the parameters of the movie are correct
    /// \param m: the given movie (Movie)
    void validate(const Movie& m);
};


class ValidateException {
    vector <string> msgs;

public:
    ValidateException(const vector <string>& errors): msgs{ errors } {}

    string getMsgs();

    friend ostream& operator <<(ostream& out, const ValidateException& ex);
};

ostream& operator <<(ostream& out, const ValidateException& ex);


void testValidator();

#endif //LAB6_7_VALIDATOR_H
