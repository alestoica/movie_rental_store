#ifndef LAB6_7_CONSOLE_H
#define LAB6_7_CONSOLE_H

#pragma once
#include "../service/service.h"
#include "../domain/movie.h"


class Console {
    Service& serv;

    void addUI();

    void deleteUI();

    void findUI();

    void updateUI();

    void filterByTitleUI();

    void filterByYearUI();

    void uiCart();

    void uiAddToCart();

    void uiAddRandomToCart();

    void uiEmptyCart();

    void uiExportCart();

    static void printAll(const vector <Movie>& movies);

    static void printMovie(const Movie& movie);

    static void printMenu();

    static void printCartMenu();

public:
    Console(Service& _serv): serv{ _serv } {}

    Console(const Console& c) = delete;

    void start();
};

#endif //LAB6_7_CONSOLE_H
