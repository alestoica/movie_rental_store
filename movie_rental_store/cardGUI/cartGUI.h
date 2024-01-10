#ifndef LAB13_14_OBSERVER_CARTGUI_H
#define LAB13_14_OBSERVER_CARTGUI_H

#include <qwidget.h>
#include "../observer.h"
#include "../service/service.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QHeaderView>
#include <QPainter>
#include <QGridLayout>

class MovieCartGUI : public QWidget, public Observer {
private:
    Service& serv;

    QWidget* cartLeft;
    QWidget* cartRight;

    QLabel* cartLblTitle = new QLabel{ "title:" };
    QLabel* cartLblYear = new QLabel{ "release year:" };
//    QLabel* cartLblNumber = new QLabel{ "number of random movies:" };
    QLabel* cartLblFilename = new QLabel{ "filename:" };

    QLineEdit* cartEditTitle;
    QLineEdit* cartEditYear;
//    QLineEdit* cartEditNumber;
    QLineEdit* cartEditFilename;


    QPushButton* btnAddCart;
//    QPushButton* btnAddRandom;
//    QPushButton* btnEmptyCart;
    QPushButton* btnExportCart;
    QPushButton* btnCartCRUD;
    QPushButton* btnCartReadOnly;

    QTableWidget* cartMovies;

    void initCart();

    void connectSignalsSlotsCart();

    void reloadMovieListCart(vector <Movie> movies);

    void update() override {
        reloadMovieListCart(serv.getAllFromCart());
    }

public:
    MovieCartGUI(Service& _serv) : serv{ _serv } {
        initCart();
        connectSignalsSlotsCart();
    }

    void guiAddToCart();

//    void guiAddRandomToCart();

    void guiEmptyCart();

    void guiExportCart();
};


class MovieCartCRUDGUI : public QWidget, public Observer {
private:
    Cart& cart;
    Service& serv;

    QLabel* cartLblNumber;
    QLineEdit* cartEditNumber;
    QPushButton* btnAddRandom;
    QPushButton* btnEmptyCart;
    QTableWidget* cartMovies;

    void initCartCRUD() {
        QHBoxLayout* lyMain;
        lyMain = new QHBoxLayout;
        this->setLayout(lyMain);

        QWidget* left;
        left = new QWidget;
        QFormLayout* lyLeft;
        lyLeft = new QFormLayout;
        left->setLayout(lyLeft);

        cartLblNumber = new QLabel{ "number of random movies:" };
        cartEditNumber = new QLineEdit;

        lyLeft->addRow(cartLblNumber, cartEditNumber);

        btnAddRandom = new QPushButton{"add random movies"};
        lyLeft->addWidget(btnAddRandom);

        btnEmptyCart = new QPushButton("empty cart");
        lyLeft->addWidget(btnEmptyCart);

        QWidget* right;
        right = new QWidget;
        QVBoxLayout* lyRight;
        lyRight = new QVBoxLayout;
        right->setLayout(lyRight);

        int noLines = 10;
        int noColumns = 4;
        this->cartMovies = new QTableWidget{ noLines, noColumns };


        QStringList tblHeaderList;
        tblHeaderList << "title" << "genre" << "main actor" << "release year";
        this->cartMovies->setHorizontalHeaderLabels(tblHeaderList);

        this->cartMovies->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        this->cartMovies->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        lyRight->addWidget(cartMovies);

        lyMain->addWidget(left);
        lyMain->addWidget(right);
        setAttribute(Qt::WA_DeleteOnClose);
//        cart.addObserver(this);
    }

    void reloadMovieListCart(vector <Movie> movies) {
        this->cartMovies->clearContents();
        if (movies.size() > this->cartMovies->rowCount())
            this->cartMovies->setRowCount(movies.size());

        int lineNumber = 0;
        for (auto& movie : movies) {
            this->cartMovies->setItem(lineNumber, 0, new QTableWidgetItem(QString::fromStdString(movie.getTitle())));
            this->cartMovies->setItem(lineNumber, 1, new QTableWidgetItem(QString::fromStdString(movie.getGenre())));
            this->cartMovies->setItem(lineNumber, 2, new QTableWidgetItem(QString::fromStdString(movie.getActor())));
            this->cartMovies->setItem(lineNumber, 3, new QTableWidgetItem(QString::number(movie.getYear())));
            lineNumber++;
        }
    }

    void connectSignalsSlotsCart() {
//        cart.addObserver(this);

        QObject::connect(btnAddRandom, &QPushButton::clicked, this, [&](){
            int nr = cartEditNumber->text().toInt();

            cartEditNumber->clear();

            this->cart.addRandomMovies(serv.getAll(), nr);
            this->reloadMovieListCart(this->cart.getAllFromCart());

            QMessageBox::information(this, "Info", QString::fromStdString("random movies added successfully to cart!"));
        });

        QObject::connect(btnEmptyCart, &QPushButton::clicked, this, [&]() {
            this->cart.emptyCart();
            this->reloadMovieListCart(this->cart.getAllFromCart());
            QMessageBox::information(this, "Info", QString::fromStdString("cart successfully emptied!"));
        });
    }

public:
    MovieCartCRUDGUI(Cart& _cart, Service& _serv) : cart{ _cart }, serv{ _serv } {
        cart.addObserver(this);
        initCartCRUD();
        connectSignalsSlotsCart();
    };

    void update() override {
        this->reloadMovieListCart(cart.getAllFromCart());
    }

    ~MovieCartCRUDGUI() {
        cart.removeObserver(this);
    }
};


class MovieCartReadOnlyGUI  : public QWidget, public Observer {
private:
    Cart& cart;

    void paintEvent(QPaintEvent* e) override {
        QPainter p{ this };
        int x, y;
        for (const auto& m : cart.getAllFromCart()) {
            x = rand() % 400 + 1, y = rand() % 400 + 1;
            p.drawRect(x, y, 10, m.getYear() % 100);
        }
    }

public:
    MovieCartReadOnlyGUI(Cart& _cart) : cart{ _cart } {
        cart.addObserver(this);
    };

    void update() override {
        repaint();
    }

    ~MovieCartReadOnlyGUI() {
        cart.removeObserver(this);
    };
};

#endif //LAB13_14_OBSERVER_CARTGUI_H
