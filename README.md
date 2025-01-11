# Movie Rental Store

This is a C++ application that implements a movie rental store with a layered architecture, demonstrating key Object-Oriented Programming (OOP) concepts and focusing on GUI development using Qt Designer. The application provides an interactive movie database, allowing users to perform various operations such as adding, deleting movies from their cart, filtering movies by title or year, and undoing operations.

## Features

- **Movie Database**: Store and manage a list of movies with attributes such as title, year, and genre.
- **Cart Operations**: Add or remove movies from the user's cart.
- **Filtering**: Filter movies by title or year to easily find specific movies.
- **Undo Operations**: Reverse any operation (e.g., add or remove) performed on the cart.
- **GUI Interface**: A user-friendly interface built with Qt Designer for easy navigation and interaction.

## Architecture

The application follows a **layered architecture**, which separates different components into distinct layers for easier maintenance and scalability:

1. **Presentation Layer**: Handles the graphical user interface (GUI) using Qt Designer.
2. **Business Logic Layer**: Manages the operations such as adding, deleting, and filtering movies.
3. **Data Access Layer**: Interacts with the data storage to manage the movie database.

## Requirements

- **C++**: The application is built using C++ for its core functionality.
- **Qt**: The GUI is designed using Qt Designer (requires Qt framework).
- **CMake**: The project uses CMake as the build system.
- **Qt5 or Qt6**: Ensure you have the required Qt version installed.

## Usage

Once the application is running, you can interact with it via the GUI:

- **Add Movies**: Use the interface to add movies to the cart.
- **Remove Movies**: Delete movies from the cart as needed.
- **Filter Movies**: Filter movies by title or year using the provided options.
- **Undo**: Reverse any operation performed on the cart.
