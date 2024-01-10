#ifndef LAB8_9_QT_GUI_H
#define LAB8_9_QT_GUI_H

#pragma once

#include <vector>
#include <string>
#include <QtWidgets/QApplication>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QTableWidget>
#include <QMessageBox>
#include <QHeaderView>
#include <QGroupBox>
#include <QRadioButton>
#include <QListWidget>
#include "../service/service.h"
#include "../cardGUI/cartGUI.h"
#include "../myTabelModel.h"

using std::vector;
using std::string;

class MovieStoreGUI : public QWidget {
private:
    Service& serv;
    MovieCartGUI cart{ serv };

    QWidget* left;
    QWidget* middle;
    QWidget* right;
    QVBoxLayout* lyRight;

    QLabel* lblTitle = new QLabel{ "title:" };
    QLabel* lblGenre = new QLabel{ "genre:" };
    QLabel* lblActor = new QLabel{ "main actor:" };
    QLabel* lblYear = new QLabel{ "release year:" };

    QLineEdit* editTitle;
    QLineEdit* editGenre;
    QLineEdit* editActor;
    QLineEdit* editYear;

    QLabel* lblField = new QLabel{ "field:" };
    QLabel* lblValue = new QLabel{ "value:" };

    QLineEdit* editField;
    QLineEdit* editValue;

    QLabel* lblCriteria = new QLabel{ "criteria:" };

    QLineEdit* editCriteria;

    QPushButton* btnAddMovie;

    QPushButton* btnDeleteMovie;

    QPushButton* btnUpdateMovie;

    QPushButton* btnFindMovie;

    QPushButton* btnFilterTitleMovies;

    QPushButton* btnFilterYearMovies;

    QRadioButton* radioSortByTitle = new QRadioButton(QString::fromStdString("title"));

    QRadioButton* radioSortByActor = new QRadioButton(QString::fromStdString("actor"));

    QRadioButton* radioSortByYearGenre = new QRadioButton(QString::fromStdString("year + genre"));

    QPushButton* btnSortMovies;

    QPushButton* btnUndo;

    QPushButton* btnReloadData;

    QPushButton* btnOpenCart;

//    QTableWidget* tableMovies;
    MyTableModel* model;
    QTableView* tableMovies;

    void initGUI();

    void connectSignalsSlots();

    void reloadMovieList(vector <Movie> movies);

public:
    MovieStoreGUI(Service& _serv) : serv{ _serv } {
        initGUI();
//        model = new MyTableModel{ serv.getAll() };
//        tableMovies->setModel(model);
        connectSignalsSlots();
        reloadMovieList(const_cast<vector<Movie> &>(serv.getAll()));
    }

    void guiAddMovie();

    void guiDeleteMovie();

    void guiUpdateMovie();

    void guiFindMovie();

    void reloadDynamicButtons();

    void clearDynamicButtons();
};

#endif //LAB8_9_QT_GUI_H
