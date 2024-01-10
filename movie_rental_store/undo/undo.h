#ifndef LAB8_9_UNDO_H
#define LAB8_9_UNDO_H

#pragma once
#include "../domain/movie.h"
#include "../repository/repo.h"

class UndoAction {
public:
    virtual void doUndo() = 0;
    virtual ~UndoAction() = default;
};

class UndoAdd : public UndoAction {
    AbstractRepo& repo;
    Movie addedMovie;
public:
    UndoAdd(AbstractRepo& rep, const Movie& m): repo{ rep }, addedMovie{ m } {}

    void doUndo() override {
        repo.deleteElem(addedMovie.getTitle(), addedMovie.getYear());
    }
};

class UndoDelete : public UndoAction {
    AbstractRepo& repo;
    Movie deletedMovie;
public:
    UndoDelete(AbstractRepo& rep, const Movie& m): repo{ rep }, deletedMovie{ m } {}

    void doUndo() override {
        repo.addElem(deletedMovie);
    }
};

class UndoUpdate : public UndoAction {
    AbstractRepo& repo;
    Movie notUpdatedMovie;
    Movie updatedMovie;
public:
    UndoUpdate(AbstractRepo& rep, const Movie& initial_m, const Movie& updated_m): repo{ rep }, notUpdatedMovie{ initial_m }, updatedMovie{ updated_m } {}

    void doUndo() override {
        repo.deleteElem(updatedMovie.getTitle(), updatedMovie.getYear());
        repo.addElem(notUpdatedMovie);
    }
};

#endif //LAB8_9_UNDO_H
