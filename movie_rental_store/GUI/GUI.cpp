#include "GUI.h"

void MovieStoreGUI::initGUI() {
    QHBoxLayout* lyMain;
    lyMain = new QHBoxLayout;
    this->setLayout(lyMain);

    left = new QWidget;
    QVBoxLayout* lyLeft;
    lyLeft = new QVBoxLayout;
    left->setLayout(lyLeft);

    QWidget* form;
    form = new QWidget;
    QFormLayout* lyForm;
    lyForm = new QFormLayout;
    form->setLayout(lyForm);

    editTitle = new QLineEdit;
    editGenre = new QLineEdit;
    editActor = new QLineEdit;
    editYear = new QLineEdit;

    lyForm->addRow(lblTitle, editTitle);
    lyForm->addRow(lblGenre, editGenre);
    lyForm->addRow(lblActor, editActor);
    lyForm->addRow(lblYear, editYear);

    QWidget* buttons1;
    buttons1 = new QWidget;
    QHBoxLayout* lyButtons1;
    lyButtons1 = new QHBoxLayout;
    buttons1->setLayout(lyButtons1);

    btnAddMovie = new QPushButton("add movie");
    lyButtons1->addWidget(btnAddMovie);

    btnDeleteMovie = new QPushButton("delete movie");
    lyButtons1->addWidget(btnDeleteMovie);

    btnFindMovie = new QPushButton("find movie");
    lyButtons1->addWidget(btnFindMovie);

    lyForm->addWidget(buttons1);

    lyLeft->addWidget(form);

    QWidget* updateForm;
    updateForm = new QWidget;
    QFormLayout* lyUpdateForm;
    lyUpdateForm = new QFormLayout;
    updateForm->setLayout(lyUpdateForm);

    editField = new QLineEdit;
    editValue = new QLineEdit;

    lyUpdateForm->addRow(lblField, editField);
    lyUpdateForm->addRow(lblValue, editValue);

    btnUpdateMovie = new QPushButton("update movie");
    lyUpdateForm->addWidget(btnUpdateMovie);

    lyLeft->addWidget(updateForm);

    QWidget* filterForm;
    filterForm = new QWidget;
    QFormLayout* lyFilterForm;
    lyFilterForm = new QFormLayout;
    filterForm->setLayout(lyFilterForm);

    editCriteria = new QLineEdit;

    lyFilterForm->addRow(lblCriteria, editCriteria);

    QWidget* buttons2;
    buttons2 = new QWidget;
    QHBoxLayout* lyButtons2;
    lyButtons2 = new QHBoxLayout;
    buttons2->setLayout(lyButtons2);

    btnFilterTitleMovies = new QPushButton("filter movies by title");
    lyButtons2->addWidget(btnFilterTitleMovies);

    btnFilterYearMovies = new QPushButton("filter movies by year");
    lyButtons2->addWidget(btnFilterYearMovies);

    lyFilterForm->addWidget(buttons2);

    lyLeft->addWidget(filterForm);

    QGroupBox* groupBox;
    groupBox = new QGroupBox(tr("sort by:"));
    QVBoxLayout* lyRadioBox;
    lyRadioBox = new QVBoxLayout;
    groupBox->setLayout(lyRadioBox);
    lyRadioBox->addWidget(radioSortByTitle);
    lyRadioBox->addWidget(radioSortByActor);
    lyRadioBox->addWidget(radioSortByYearGenre);

    btnSortMovies = new QPushButton("sort movies");
    lyRadioBox->addWidget(btnSortMovies);

    lyLeft->addWidget(groupBox);

    btnUndo = new QPushButton("undo");
    lyLeft->addWidget(btnUndo);

    btnReloadData = new QPushButton("reload data");
    lyLeft->addWidget(btnReloadData);

    lyMain->addWidget(left);

    middle = new QWidget;
    QVBoxLayout* lyMiddle;
    lyMiddle = new QVBoxLayout;
    middle->setLayout(lyMiddle);

//    int noLines = 10;
//    int noColumns = 4;
//    this->tableMovies = new QTableWidget{ noLines, noColumns };
//
//    QStringList tblHeaderList;
//    tblHeaderList << "title" << "genre" << "main actor" << "release year";
//    this->tableMovies->setHorizontalHeaderLabels(tblHeaderList);

    tableMovies = new QTableView;

    this->tableMovies->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    this->tableMovies->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    lyMiddle->addWidget(tableMovies);

    btnOpenCart = new QPushButton("open cart");
    lyMiddle->addWidget(btnOpenCart);

    lyMain->addWidget(middle);

    right = new QWidget;
    lyRight = new QVBoxLayout;
    right->setLayout(lyRight);

    lyMain->addWidget(right);
}

void MovieStoreGUI::reloadMovieList(vector <Movie> movies) {
//    this->tableMovies->clearContents();
//    if (movies.size() > this->tableMovies->rowCount())
//        this->tableMovies->setRowCount(movies.size());
//
//    int lineNumber = 0;
//    for (auto& movie : movies) {
//        this->tableMovies->setItem(lineNumber, 0, new QTableWidgetItem(QString::fromStdString(movie.getTitle())));
//        this->tableMovies->setItem(lineNumber, 1, new QTableWidgetItem(QString::fromStdString(movie.getGenre())));
//        this->tableMovies->setItem(lineNumber, 2, new QTableWidgetItem(QString::fromStdString(movie.getActor())));
//        this->tableMovies->setItem(lineNumber, 3, new QTableWidgetItem(QString::number(movie.getYear())));
//        lineNumber++;
//    }
    model = new MyTableModel{ serv.getAll() };
    tableMovies->setModel(model);

    model->setMovies(movies);

    this->clearDynamicButtons();
    this->reloadDynamicButtons();
}

void MovieStoreGUI::connectSignalsSlots() {
    QObject::connect(btnAddMovie, &QPushButton::clicked, this, &MovieStoreGUI::guiAddMovie);

    QObject::connect(btnDeleteMovie, &QPushButton::clicked, this, &MovieStoreGUI::guiDeleteMovie);

    QObject::connect(btnUpdateMovie, &QPushButton::clicked, this, &MovieStoreGUI::guiUpdateMovie);

    QObject::connect(btnFindMovie, &QPushButton::clicked, this, &MovieStoreGUI::guiFindMovie);

    QObject::connect(btnSortMovies, &QPushButton::clicked, [&]() {
        if (this->radioSortByTitle->isChecked())
            this->reloadMovieList(serv.sortByTitle());
        else if (this->radioSortByActor->isChecked())
            this->reloadMovieList(serv.sortByActor());
        else if (this->radioSortByYearGenre->isChecked())
            this->reloadMovieList(serv.sortByYearGenre());
    });

    QObject::connect(btnFilterTitleMovies, &QPushButton::clicked, [&]() {
        string title = this->editCriteria->text().toStdString();
        this->reloadMovieList(serv.filterTitle(title));
    });

    QObject::connect(btnFilterYearMovies, &QPushButton::clicked, [&]() {
        int year = this->editCriteria->text().toInt();
        this->reloadMovieList(serv.filterYear(year));
    });

    QObject::connect(btnUndo, &QPushButton::clicked, [&]() {
        try {
            serv.undo();
            this->reloadMovieList(const_cast<vector<Movie> &>(serv.getAll()));
        }
        catch (RepoException& re) {
            QMessageBox::warning(this, "warning", QString::fromStdString(re.getMsg()));
        }
        catch (ValidateException& ve) {
            QMessageBox::warning(this, "warning", QString::fromStdString(ve.getMsgs()));
        }
    });

    QObject::connect(btnReloadData, &QPushButton::clicked, [&]() {
        this->reloadMovieList(const_cast<vector<Movie> &>(serv.getAll()));
    });

    QObject::connect(btnOpenCart, &QPushButton::clicked, [&]() {
            cart.show();
    });
}

void MovieStoreGUI::guiAddMovie() {
    try {
        string title = editTitle->text().toStdString();
        string genre = editGenre->text().toStdString();
        string actor = editActor->text().toStdString();
        int year = editYear->text().toInt();

        editTitle->clear();
        editGenre->clear();
        editActor->clear();
        editYear->clear();

        this->serv.addMovie(title, genre, actor, year);
        this->reloadMovieList(const_cast<vector<Movie> &>(this->serv.getAll()));
        this->clearDynamicButtons();
        this->reloadDynamicButtons();

        QMessageBox::information(this, "Info", QString::fromStdString("movie added successfully!"));
    }
    catch (RepoException& re) {
        QMessageBox::warning(this, "warning", QString::fromStdString(re.getMsg()));
    }
    catch (ValidateException& ve) {
        QMessageBox::warning(this, "warning", QString::fromStdString(ve.getMsgs()));
    }
}

void MovieStoreGUI::guiDeleteMovie() {
    try {
        string title = editTitle->text().toStdString();
        int year = editYear->text().toInt();

        editTitle->clear();
        editYear->clear();

        this->serv.deleteMovie(title, year);
        this->reloadMovieList(const_cast<vector<Movie> &>(this->serv.getAll()));
        this->clearDynamicButtons();
        this->reloadDynamicButtons();

        QMessageBox::information(this, "Info", QString::fromStdString("movie deleted successfully!"));
    }
    catch (RepoException& re) {
        QMessageBox::warning(this, "warning", QString::fromStdString(re.getMsg()));
    }
    catch (ValidateException& ve) {
        QMessageBox::warning(this, "warning", QString::fromStdString(ve.getMsgs()));
    }
}

void MovieStoreGUI::guiUpdateMovie() {
    try {
        string title = editTitle->text().toStdString();
        int year = editYear->text().toInt();
        string field = editField->text().toStdString();
        string value = editValue->text().toStdString();

        editField->clear();
        editValue->clear();
        editTitle->clear();
        editYear->clear();

        this->serv.updateMovie(title, year, field, value);
        this->reloadMovieList(const_cast<vector<Movie> &>(this->serv.getAll()));
        this->clearDynamicButtons();
        this->reloadDynamicButtons();

        QMessageBox::information(this, "Info", QString::fromStdString("movie updated successfully!"));
    }
    catch (RepoException& re) {
        QMessageBox::warning(this, "warning", QString::fromStdString(re.getMsg()));
    }
    catch (ValidateException& ve) {
        QMessageBox::warning(this, "warning", QString::fromStdString(ve.getMsgs()));
    }
}

void MovieStoreGUI::guiFindMovie() {
    try {
        string title = editTitle->text().toStdString();
        int year = editYear->text().toInt();

        editTitle->clear();
        editYear->clear();

        Movie result = this->serv.findMovie(title, year);

        QMessageBox::information(this, "Info", QString::fromStdString("the movie you are looking for:\ntitle: " + result.getTitle() + "\ngenre: " + result.getGenre() + "\nactor: " + result.getActor() + "\nyear: " + std::to_string(result.getYear())));
    }
    catch (RepoException& re) {
        QMessageBox::warning(this, "warning", QString::fromStdString(re.getMsg()));
    }
    catch (ValidateException& ve) {
        QMessageBox::warning(this, "warning", QString::fromStdString(ve.getMsgs()));
    }
}

void MovieStoreGUI::reloadDynamicButtons() {
    for (const auto& i : serv.getMovieGenres()) {
        QPushButton* btn;
        btn = new QPushButton{ QString::fromStdString(i.first) };
        lyRight->addWidget(btn);
        QObject::connect(btn, &QPushButton::clicked, [btn, i]() {
            QMessageBox::information(nullptr, "info", QString::number(i.second));
        });
    }
}

void MovieStoreGUI::clearDynamicButtons() {
    QLayoutItem* item;
    while (lyRight->count() != 0) {
        item = lyRight->takeAt(0);
        if (item->widget() != nullptr)
            delete item->widget();
        delete item;
    }
}
