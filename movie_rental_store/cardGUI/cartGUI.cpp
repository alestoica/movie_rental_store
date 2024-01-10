#include "cartGUI.h"


void MovieCartGUI::initCart() {
    QHBoxLayout* lyCart;
    lyCart = new QHBoxLayout;
    this->setLayout(lyCart);

    cartLeft = new QWidget;
    QVBoxLayout* lyLeftCart;
    lyLeftCart = new QVBoxLayout;
    cartLeft->setLayout(lyLeftCart);

    QWidget* form1;
    form1 = new QWidget;
    QFormLayout* lyForm1;
    lyForm1 = new QFormLayout;
    form1->setLayout(lyForm1);

    cartEditTitle = new QLineEdit;
    cartEditYear = new QLineEdit;

    lyForm1->addRow(cartLblTitle, cartEditTitle);
    lyForm1->addRow(cartLblYear, cartEditYear);

    btnAddCart = new QPushButton("add movie");
    lyForm1->addWidget(btnAddCart);

    lyLeftCart->addWidget(form1);

    QWidget* form2;
    form2 = new QWidget;
    QFormLayout* lyForm2;
    lyForm2 = new QFormLayout;
    form2->setLayout(lyForm2);

//    cartEditNumber = new QLineEdit;
//
//    lyForm2->addRow(cartLblNumber, cartEditNumber);

//    btnAddRandom = new QPushButton("add random movies");
//    lyForm2->addWidget(btnAddRandom);

    lyLeftCart->addWidget(form2);

    QWidget* form3;
    form3 = new QWidget;
    QFormLayout* lyForm3;
    lyForm3 = new QFormLayout;
    form3->setLayout(lyForm3);

    cartEditFilename = new QLineEdit;

    lyForm3->addRow(cartLblFilename, cartEditFilename);

    btnExportCart = new QPushButton("export cart");
    lyForm3->addWidget(btnExportCart);

    lyLeftCart->addWidget(form3);

//    btnEmptyCart = new QPushButton("empty cart");
//    lyLeftCart->addWidget(btnEmptyCart);

    lyCart->addWidget(cartLeft);

    cartRight = new QWidget;
    QVBoxLayout* lyRightCart;
    lyRightCart = new QVBoxLayout;
    cartRight->setLayout(lyRightCart);

    int noLines = 10;
    int noColumns = 4;
    this->cartMovies = new QTableWidget{ noLines, noColumns };

    QStringList tblHeaderList;
    tblHeaderList << "title" << "genre" << "main actor" << "release year";
    this->cartMovies->setHorizontalHeaderLabels(tblHeaderList);

    this->cartMovies->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->cartMovies->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    lyRightCart->addWidget(cartMovies);

    btnCartCRUD = new QPushButton{"open CRUD cart"};
    lyRightCart->addWidget(btnCartCRUD);

    btnCartReadOnly = new QPushButton{"open Read Only cart"};
    lyRightCart->addWidget(btnCartReadOnly);

    lyCart->addWidget(cartRight);
}

void MovieCartGUI::connectSignalsSlotsCart() {
    serv.getCart().addObserver(this);

    QObject::connect(btnAddCart, &QPushButton::clicked, this, &MovieCartGUI::guiAddToCart);

//    QObject::connect(btnAddRandom, &QPushButton::clicked, this, &MovieCartGUI::guiAddRandomToCart);

//    QObject::connect(btnEmptyCart, &QPushButton::clicked, this, &MovieCartGUI::guiEmptyCart);

    QObject::connect(btnExportCart, &QPushButton::clicked, this, &MovieCartGUI::guiExportCart);

    QObject::connect(btnCartCRUD, &QPushButton::clicked, this, [&]() {
        auto cartCRUD = new MovieCartCRUDGUI{ serv.getCart(), serv };
        cartCRUD->show();
    });

    QObject::connect(btnCartReadOnly, &QPushButton::clicked, this, [&]() {
        auto cartReadOnly = new MovieCartReadOnlyGUI{ serv.getCart() };
        cartReadOnly->show();
    });
}

void MovieCartGUI::reloadMovieListCart(vector<Movie> movies) {
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

void MovieCartGUI::guiAddToCart() {
    try {
        string title = cartEditTitle->text().toStdString();
        int year = cartEditYear->text().toInt();

        cartEditTitle->clear();
        cartEditYear->clear();

        this->serv.addToCart(title, year);
        this->reloadMovieListCart(const_cast<vector<Movie> &>(this->serv.getAllFromCart()));

        QMessageBox::information(this, "Info", QString::fromStdString("movie added successfully to cart!"));
    }
    catch (RepoException& re) {
        QMessageBox::warning(this, "warning", QString::fromStdString(re.getMsg()));
    }
    catch (ValidateException& ve) {
        QMessageBox::warning(this, "warning", QString::fromStdString(ve.getMsgs()));
    }
}

//void MovieCartGUI::guiAddRandomToCart() {
//    try {
//        int nr = cartEditNumber->text().toInt();
//
//        cartEditNumber->clear();
//
//        this->serv.addRandomToCart(nr);
//        this->reloadMovieListCart(const_cast<vector<Movie> &>(this->serv.getAllFromCart()));
//
//        QMessageBox::information(this, "Info", QString::fromStdString("random movies added successfully to cart!"));
//    }
//    catch (RepoException& re) {
//        QMessageBox::warning(this, "warning", QString::fromStdString(re.getMsg()));
//    }
//    catch (ValidateException& ve) {
//        QMessageBox::warning(this, "warning", QString::fromStdString(ve.getMsgs()));
//    }
//}

void MovieCartGUI::guiEmptyCart() {
    this->serv.emptyCart();
    this->reloadMovieListCart(const_cast<vector<Movie> &>(this->serv.getAllFromCart()));
    QMessageBox::information(this, "Info", QString::fromStdString("cart successfully emptied!"));
}

void MovieCartGUI::guiExportCart() {
    try {
        string filename = cartEditFilename->text().toStdString();

        cartEditFilename->clear();

        this->serv.exportCart(filename);
        this->guiEmptyCart();

        QMessageBox::information(this, "Info", QString::fromStdString("cart successfully exported!"));
    }
    catch (RepoException& re) {
        QMessageBox::warning(this, "warning", QString::fromStdString(re.getMsg()));
    }
    catch (ValidateException& ve) {
        QMessageBox::warning(this, "warning", QString::fromStdString(ve.getMsgs()));
    }
}