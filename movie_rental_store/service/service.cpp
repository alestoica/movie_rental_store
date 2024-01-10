#include "service.h"
#include <functional>
#include <cassert>


void Service::addMovie(const string& title, const string& genre, const string& actor, const int& year) {
    Movie m{title, genre, actor, year};
    val.validate(m);
    repo.addElem(m);
    undoActions.push_back(std::make_unique<UndoAdd>(repo, m));
}

void Service::deleteMovie(const string &title, const int& year) {
    Movie m = repo.findElem(title, year);
    val.validate(m);
    repo.deleteElem(title, year);
    undoActions.push_back(std::make_unique<UndoDelete>(repo, m));
}

const Movie& Service::updateMovie(const string &title, const int& year, const string& field, const string& value) {
    Movie m = repo.findElem(title, year);
    Movie initial_m = m;
    if (field == "title")
        m.setTitle(value);
    if (field == "genre")
        m.setGenre(value);
    if (field == "actor")
        m.setActor(value);
    if (field == "year")
        m.setYear(value);
    val.validate(m);
    undoActions.push_back(std::make_unique<UndoUpdate>(repo, initial_m, m));
    return repo.updateElem(title, year, field, value);
}

const Movie& Service::findMovie(const string& title, const int& year) {
    Movie m = repo.findElem(title, year);
    val.validate(m);
    return repo.findElem(title, year);
}

const vector <Movie>& Service::getAll() const {
    return repo.getAll();
}

map <string, int> Service::getMovieGenres() const {
    map <string, int> m;
    for (const auto& i : getAll()) {
        auto it = m.find(i.getGenre());
        if (it != m.end()) {
            it->second++;
            m.insert({i.getGenre(), it->second});
        }
        else
            m.insert({i.getGenre(), 1});
    }
    return m;
}

vector <Movie> Service::generalSort(bool (*cmpF)(const Movie &, const Movie &)) {
    vector <Movie> v{ repo.getAll() };
    std::sort(v.begin(), v.end(), cmpF);
    return v;}

vector <Movie> Service::sortByTitle() {
    return generalSort(cmpTitle);
}

vector <Movie> Service::sortByActor() {
    return generalSort(cmpActor);
}

vector <Movie> Service::sortByYearGenre() {
    return generalSort(cmpYearGenre);
}

vector <Movie> Service::filter(const function<bool(const Movie &)>& fct) {
    vector <Movie> v{ repo.getAll().size() };
    auto it = std::copy_if(repo.getAll().begin(), repo.getAll().end(), v.begin(), fct);
    v.resize(std::distance(v.begin(), it));
    return v;}

vector <Movie> Service::filterTitle(const string& title) {
    return filter([title](const Movie& m)->bool {
        return m.getTitle() == title;
    });
}

vector <Movie> Service::filterYear(int year) {
    return filter([year](const Movie& m)->bool {
        return m.getYear() == year;
    });
}

void Service::addToCart(const string& title, const int& year) {
    const Movie& m = repo.findElem(title, year);
    val.validate(m);
    currentCart.addToCart(m);
}

int Service::addRandomToCart(int nr) {
    currentCart.addRandomMovies(getAll(), nr);
    return currentCart.getAllFromCart().size();
}

void Service::emptyCart() {
    currentCart.emptyCart();
}

const vector <Movie>& Service::getAllFromCart() {
    return currentCart.getAllFromCart();
}

void Service::exportCart(const string &filename) {
    currentCart.exportCart(filename);
}

void Service::undo() {
    if (undoActions.empty())
        throw RepoException("Nu mai exista operatii!");
    undoActions.back()->doUndo();
    undoActions.pop_back();
}

Cart &Service::getCart() {
    return currentCart;
}


void testAddService() {
    Repo repo;
    MovieValidator val;
    Service serv{repo, val};

    serv.addMovie("Babylon", "musical", "Margot Robbie", 2022);
    assert(serv.getAll().size() == 1);

    try {
        serv.addMovie("Movie", "Genre", "Actor", -1);
    }
    catch (ValidateException&) {
        assert(true);
    }

    try {
        serv.addMovie("", "Genre", "Actor", 2023);
    }
    catch (ValidateException&) {
        assert(true);
    }

    try {
        serv.addMovie("Movie", "", "Actor", 2023);
    }
    catch (ValidateException&) {
        assert(true);
    }

    try {
        serv.addMovie("Movie", "Genre", "", 2023);
    }
    catch (ValidateException&) {
        assert(true);
    }
}

void testFindService() {
    Repo repo;
    MovieValidator val;
    Service serv{repo, val};

    Movie m{"Babylon", "musical", "Margot Robbie", 2022};

    serv.addMovie("Babylon", "musical", "Margot Robbie", 2022);

    assert(serv.findMovie("Babylon", 2022) == m);

    try {
        serv.findMovie("Idk", 2022);
    }
    catch (const RepoException&) {
        assert(true);
    }
}

void testDeleteService() {
    Repo repo;
    MovieValidator val;
    Service serv{repo, val};

    try {
        serv.deleteMovie("Babylon", 2022);
    }
    catch (const RepoException&) {
        assert(true);
    }

    serv.addMovie("Babylon", "musical", "Margot Robbie", 2022);
    serv.addMovie("Fight Club", "action", "Edward Norton", 2014);
    assert(serv.getAll().size() == 2);

    serv.deleteMovie("Babylon", 2022);
    assert(serv.getAll().size() == 1);

    try {
        serv.deleteMovie("", 2022);
    }
    catch (const RepoException&) {
        assert(true);
    }
}

void testUpdateService() {
    Repo repo;
    MovieValidator val;
    Service serv{repo, val};

    serv.addMovie("Babylon", "musical", "Margot Robbie", 2022);

    try {
        serv.updateMovie("Babylon", 2022, "title", "");
    }
    catch (const ValidateException&) {
        assert(true);
    }

    try {
        serv.updateMovie("Babylon", 2022, "genre", "");
    }
    catch (const ValidateException&) {
        assert(true);
    }

    try {
        serv.updateMovie("Babylon", 2022, "actor", "");
    }
    catch (const ValidateException&) {
        assert(true);
    }

    try {
        serv.updateMovie("Babylon", 2022, "year", "-1");
    }
    catch (const ValidateException&) {
        assert(true);
    }

    assert(serv.updateMovie("Babylon", 2022, "title", "NewTitle").getTitle() == "NewTitle");
    assert(serv.getAll()[0].getTitle() == "NewTitle");
    assert(serv.updateMovie("NewTitle", 2022, "genre", "NewGenre").getGenre() == "NewGenre");
    assert(serv.getAll()[0].getGenre() == "NewGenre");
    serv.updateMovie("NewTitle", 2022, "actor", "NewActor");
    assert(serv.getAll()[0].getActor() == "NewActor");
    serv.updateMovie("NewTitle", 2022, "year", "2030");
    assert(serv.getAll()[0].getYear() == 2030);
}

void testSort() {
    Repo repo;
    MovieValidator val;
    Service serv{repo, val};

    serv.addMovie("b", "b", "a", 2000);
    serv.addMovie("a", "a", "d", 2000);
    serv.addMovie("d", "c", "c", 2002);
    serv.addMovie("c", "d", "b", 2001);
    serv.addMovie("e", "e", "e", 2003);

    auto sorted_list = serv.sortByTitle();
    assert(sorted_list[0].getTitle() == "a");
    assert(sorted_list[1].getTitle() == "b");
    assert(sorted_list[2].getTitle() == "c");
    assert(sorted_list[3].getTitle() == "d");
    assert(sorted_list[4].getTitle() == "e");

    sorted_list = serv.sortByActor();
    assert(sorted_list[0].getActor() == "a");
    assert(sorted_list[1].getActor() == "b");
    assert(sorted_list[2].getActor() == "c");
    assert(sorted_list[3].getActor() == "d");
    assert(sorted_list[4].getActor() == "e");

    sorted_list = serv.sortByYearGenre();
    assert(sorted_list[0].getYear() == 2000 && sorted_list[0].getGenre() == "a");
    assert(sorted_list[1].getYear() == 2000 && sorted_list[1].getGenre() == "b");
    assert(sorted_list[2].getYear() == 2001 && sorted_list[2].getGenre() == "d");
    assert(sorted_list[3].getYear() == 2002 && sorted_list[3].getGenre() == "c");
    assert(sorted_list[4].getYear() == 2003 && sorted_list[4].getGenre() == "e");
}

void testFilter() {
    Repo repo;
    MovieValidator val;
    Service serv{repo, val};

    serv.addMovie("a", "b", "a", 2000);
    serv.addMovie("a", "a", "d", 2001);
    serv.addMovie("b", "c", "c", 2003);
    serv.addMovie("c", "d", "b", 2003);
    serv.addMovie("a", "e", "e", 2003);

    auto filtered_list = serv.filterTitle("a");
    assert(filtered_list.size() == 3);
    assert(filtered_list[0].getTitle() == "a");
    assert(filtered_list[1].getTitle() == "a");
    assert(filtered_list[2].getTitle() == "a");

    filtered_list = serv.filterYear(2003);
    assert(filtered_list.size() == 3);
    assert(filtered_list[0].getYear() == 2003);
    assert(filtered_list[1].getYear() == 2003);
    assert(filtered_list[2].getYear() == 2003);
}

void testCart() {
    Repo repo;
    MovieValidator val;
    Service serv{repo, val};

    serv.addMovie("Babylon", "comedy", "Margot Robbie", 2022);
    serv.addMovie("Fight Club", "mystery", "Edward Norton", 2015);
    serv.addMovie("Pride and Prejudice", "drama", "Kiera Knightly", 2013);
    serv.addMovie("Titanic", "drama", "Leonardo DiCapri", 2003);
    serv.addMovie("Years and Years", "drama", "Emma Thompson", 2020);

    serv.addRandomToCart(4);
    assert(serv.getAllFromCart().size() == 4);
    serv.emptyCart();
    assert(serv.getAllFromCart().empty());

    serv.addRandomToCart(20);
    assert(serv.getAllFromCart().size() == 5);
    serv.emptyCart();
    assert(serv.getAllFromCart().empty());

    serv.addToCart("Titanic", 2003);
    assert(serv.getAllFromCart().size() == 1);

    try {
        serv.addToCart("Frozen", 2013);
    }
    catch (const RepoException&) {
        assert(true);
    }
}

void testUndo() {
    Repo repo;
    MovieValidator val;
    Service serv{repo, val};

    serv.addMovie("Babylon", "comedy", "Margot Robbie", 2022);
    serv.addMovie("Fight Club", "mystery", "Edward Norton", 2015);
    serv.addMovie("Pride and Prejudice", "drama", "Kiera Knightly", 2013);
    serv.addMovie("Titanic", "drama", "Leonardo DiCapri", 2003);
    serv.addMovie("Years and Years", "drama", "Emma Thompson", 2020);
    assert(serv.getAll().size() == 5);

    serv.undo();
    assert(serv.getAll().size() == 4);

    serv.deleteMovie("Titanic", 2003);
    assert(serv.getAll().size() == 3);

    serv.undo();
    assert(serv.getAll().size() == 4);

    serv.updateMovie("Babylon", 2022, "title", "new_title");
    assert(serv.getAll()[0].getTitle() == "new_title");

    serv.undo();

    assert(serv.getAll()[3].getTitle() == "Babylon");

    serv.undo();
    serv.undo();
    serv.undo();
    serv.undo();

    try {
        serv.undo();
    }
    catch (const RepoException&) {
        assert(true);
    }
}

void testService() {
    testAddService();
    testDeleteService();
    testFindService();
    testUpdateService();
    testSort();
    testFilter();
    testCart();
    testUndo();
}


