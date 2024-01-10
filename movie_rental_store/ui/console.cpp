#include "console.h"
#include <iostream>
#include <string>

using std::cout;
using std::cin;


void Console::printMenu() {
    cout << "Menu: \n";
    cout << "1. print all movies\n";
    cout << "2. add movie\n";
    cout << "3. delete movie\n";
    cout << "4. find movie\n";
    cout << "5. update movie\n";
    cout << "6. sort movies by title\n";
    cout << "7. sort movies by actor\n";
    cout << "8. sort movies by year and genre\n";
    cout << "9. filter movies by title\n";
    cout << "10. filter movies by year\n";
    cout << "11. undo\n";
    cout << "12. cart menu\n";
    cout << "0. exit\n";
}

void Console::printCartMenu() {
    cout << "Cart menu: \n";
    cout << "1. print current movies from the cart\n";
    cout << "2. add movie to cart\n";
    cout << "3. add random movies to cart\n";
    cout << "4. empty cart\n";
    cout << "5. export cart\n";
    cout << "6. back to main menu\n";
}

void Console::printAll(const vector <Movie>& movies) {
    if (movies.empty())
        cout << "The list of movies is currently empty!\n";
    else {
        cout << "Current movies:\n";
        for (const auto &movie: movies) {
            cout << "Title: " << movie.getTitle() << " | Genre: " << movie.getGenre() << " | Actor: "
                 << movie.getActor() << " | Year: " << movie.getYear() << "\n";
        }
    }
}

void Console::printMovie(const Movie &movie) {
    cout << "Title: " << movie.getTitle() << " | Genre: " << movie.getGenre() << " | Actor: " << movie.getActor() << " | Year: " << movie.getYear() << "\n";
}

void Console::addUI() {
    string title, genre, actor;
    int year;
    cin.ignore();
    cout << "Enter a movie title:";
    std::getline(cin, title);
    cout << "Enter a movie genre:";
    std::getline(cin, genre);
    cout << "Enter an actor:";
    std::getline(cin, actor);
    cout << "Enter the release date:";
    cin >> year;
    try {
    serv.addMovie(title, genre, actor, year);
    }
    catch (...) {
        cout << "nu e bine";
    }
    cout << "Added successfully!\n";
}

void Console::deleteUI() {
    string title;
    int year;
    cin.ignore();
    cout << "Enter a movie title:";
    std::getline(cin, title);
    cout << "Enter the release date:";
    cin >> year;
    serv.deleteMovie(title, year);
    cout << "Deleted successfully!\n";
}

void Console::findUI() {
    string title;
    int year;
    cin.ignore();
    cout << "Enter a movie title:";
    std::getline(cin, title);
    cout << "Enter the release date:";
    cin >> year;
    Movie movie = serv.findMovie(title, year);
    cout << "The movie you are looking for:\n";
    printMovie(movie);
}

void Console::updateUI() {
    string title, field, value;
    int year;
    cin.ignore();
    cout << "Enter a movie title:";
    std::getline(cin, title);
    cout << "Enter the release date:";
    cin >> year;
    cin.ignore();
    cout << "Enter what field you would like to update (title, genre, actor, year):";
    std::getline(cin, field);
    cout << "Enter the updated value:";
    std::getline(cin, value);
    Movie movie = serv.updateMovie(title, year, field, value);
    cout << "The updated movie:\n";
    printMovie(movie);
}

void Console::filterByTitleUI() {
    string title;
    cout << "Enter a title:";
    cin.ignore();
    std::getline(cin, title);
    printAll(serv.filterTitle(title));
}

void Console::filterByYearUI() {
    int year;
    cout << "Enter a release date:";
    cin >> year;
    printAll(serv.filterYear(year));
}

void Console::uiAddToCart() {
    string title;
    int year;
    cin.ignore();
    cout << "Enter a movie title:";
    std::getline(cin, title);
    cout << "Enter the release date:";
    cin >> year;
    serv.addToCart(title, year);
    cout << "Added successfully to cart!\n";
}

void  Console::uiAddRandomToCart() {
    int nr;
    cout << "Enter the number of movies to be added to the cart:";
    cin >> nr;
    int nr_added = serv.addRandomToCart(nr);
    cout << nr_added << " movies succesfully added to cart!\n";
}

void Console::uiEmptyCart() {
    serv.emptyCart();
    cout << "Cart emptied.\n";
}

void Console::uiExportCart() {
    string filename;
    cout << "Enter the name of the file where the contents of the cart will be saved:";
    cin.ignore();
    std::getline(cin, filename);
//    std::ofstream fout(filename);
//    fout << "<!DOCTYPE html><html><head></head><body>";
//    for (const auto& movie : serv.getAllFromCart()) {
//        fout << "<p><span style='font-weight: bold'>" << "Title: " + movie.getTitle() + " | Genre: " + movie.getGenre() + " | Actor: " + movie.getActor() + " | Year: " + std::to_string(movie.getYear()) << "</span></p>";
//    }
//    fout << "</body></html>";
//    cout << "Current number of movies in cart: " <<serv.getAllFromCart().size() << "\n";
    serv.exportCart(filename);
    cout << "The contents of the cart have been saved to the " << filename << " file.\n";
}

void Console::uiCart() {
    int runningCart = 1;
    while (runningCart) {
        printCartMenu();
        int cmd;
        cout << "Enter your option:\n";
        cin >> cmd;
        try {
            switch (cmd) {
                case 1:
                    printAll(serv.getAllFromCart());
                    cout << "Current number of movies in cart: " <<serv.getAllFromCart().size() << "\n";
                    break;
                case 2:
                    uiAddToCart();
                    cout << "Current number of movies in cart: " <<serv.getAllFromCart().size() << "\n";
                    break;
                case 3:
                    uiAddRandomToCart();
                    cout << "Current number of movies in cart: " <<serv.getAllFromCart().size() << "\n";
                    break;
                case 4:
                    uiEmptyCart();
                    cout << "Current number of movies in cart: " <<serv.getAllFromCart().size() << "\n";
                    break;
                case 5:
                    uiExportCart();
                    break;
                case 6:
                    runningCart = 0;
                    break;
                default:
                    break;
            }
        }
        catch (const RepoException& ex) {
            cout << ex <<'\n';
        }
        catch (const ValidateException& ex) {
            cout << ex << '\n';
        }
    }
}

void Console::start() {
    while (true) {
        printMenu();
        int cmd;
        cout << "Enter your option:\n";
        cin >> cmd;
        try {
            switch (cmd) {
                case 1:
                    printAll(serv.getAll());
                    cout << "Current number of movies in cart: " <<serv.getAllFromCart().size() << "\n";
                    break;
                case 2:
                    addUI();
                    cout << "Current number of movies in cart: " <<serv.getAllFromCart().size() << "\n";
                    break;
                case 3:
                    deleteUI();
                    cout << "Current number of movies in cart: " <<serv.getAllFromCart().size() << "\n";
                    break;
                case 4:
                    findUI();
                    cout << "Current number of movies in cart: " <<serv.getAllFromCart().size() << "\n";
                    break;
                case 5:
                    updateUI();
                    cout << "Current number of movies in cart: " <<serv.getAllFromCart().size() << "\n";
                    break;
                case 6:
                    printAll(serv.sortByTitle());
                    cout << "Current number of movies in cart: " <<serv.getAllFromCart().size() << "\n";
                    break;
                case 7:
                    printAll(serv.sortByActor());
                    cout << "Current number of movies in cart: " <<serv.getAllFromCart().size() << "\n";
                    break;
                case 8:
                    printAll(serv.sortByYearGenre());
                    cout << "Current number of movies in cart: " <<serv.getAllFromCart().size() << "\n";
                    break;
                case 9:
                    filterByTitleUI();
                    cout << "Current number of movies in cart: " <<serv.getAllFromCart().size() << "\n";
                    break;
                case 10:
                    filterByYearUI();
                    cout << "Current number of movies in cart: " <<serv.getAllFromCart().size() << "\n";
                    break;
                case 11:
                    serv.undo();
                    cout << "Successful undo!\n";
                    cout << "Current number of movies in cart: " <<serv.getAllFromCart().size() << "\n";
                    break;
                case 12:
                    uiCart();
                    cout << "Current number of movies in cart: " <<serv.getAllFromCart().size() << "\n";
                    break;
                case 0:
                    cout << "Current number of movies in cart: " <<serv.getAllFromCart().size() << "\n";
                    return;
                default:
                    cout << "Invalid option!\n";
            }
        }
        catch (const RepoException& ex) {
            cout << ex <<'\n';
        }
        catch (const ValidateException& ex) {
            cout << ex << '\n';
        }
    }
}