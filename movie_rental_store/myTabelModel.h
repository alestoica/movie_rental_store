#ifndef LAB13_14_OBSERVER_MYTABELMODEL_H
#define LAB13_14_OBSERVER_MYTABELMODEL_H

#include <QAbstractTableModel>
#include <Qt>
#include <qdebug.h>
#include <QFont>
#include <QBrush>
#include "domain/movie.h"

using std::vector;

class MyTableModel : public QAbstractTableModel {
private:
    vector <Movie> movies;

public:
    MyTableModel(const vector <Movie>& _movies) : movies{ _movies } {}

    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        return movies.size();
    }
    int columnCount(const QModelIndex& parent = QModelIndex()) const override {
        return 4;
    }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
        qDebug() << "row: " << index.row() << " | col: " << index.column() << " | role: " << role;

        if (role == Qt::ForegroundRole) {
            auto song = this->movies[index.row()];
            if (song.getYear() >= 2022) {
                return QBrush(Qt::magenta);
            }
        }

        if (role == Qt::FontRole) {
            QFont f;
            f.setItalic(index.row() % 5 == 1);
            f.setBold(index.row() % 5 == 1);
            return f;
        }

        if (role == Qt::DisplayRole) {
            Movie m = movies[index.row()];
            if (index.column() == 0) {
                return QString::fromStdString(m.getTitle());
            }
            else if (index.column() == 1) {
                return QString::fromStdString(m.getGenre());
            }
            else if (index.column() == 2) {
                return QString::fromStdString(m.getActor());
            }
            else if (index.column() == 3) {
                return QString::number(m.getYear());
            }
        }

        return QVariant{};
    }

    void setMovies(const vector <Movie> v) {
        this->movies = v;
        auto topLeft = createIndex(0, 0);
        auto bottomR = createIndex(rowCount(), columnCount());
        emit dataChanged(topLeft, bottomR);
    }

    Qt::ItemFlags flags(const QModelIndex& /*index*/) const {
        return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role == Qt::DisplayRole)
        {
            if (orientation == Qt::Horizontal) {
                switch (section)
                {
                    case 0:
                        return tr("Title");
                    case 1:
                        return tr("Genre");
                    case 2:
                        return tr("Actor");
                    case 3:
                        return tr("Year");
                    default:
                        return QString("");
                }
            }
        }
        return QVariant();
    }
};

#endif //LAB13_14_OBSERVER_MYTABELMODEL_H
