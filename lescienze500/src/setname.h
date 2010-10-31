//     Copyright 2010 Simone Riva
//     mail:  simone (dot) rva (lumachina) gmail.com
//
//    This file is part of LeScienze500.
//
//    LeScienze500 is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    LeScienze500 is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with LeScienze500.  If not, see <http://www.gnu.org/licenses/>.

#ifndef SETNAME_H
#define SETNAME_H

#include <QDialog>
#include <QString>

namespace Ui {
    class SetName;
}

class SetName : public QDialog
{
    Q_OBJECT

public:
    explicit SetName(QWidget *parent = 0);
    ~SetName();

    void open() ;
    void open( QString txt ) ;

    void setMessage( QString msg ) ;

    QString text() { return p_text ; } 

private:
    Ui::SetName *ui;
    QString p_text ;

private slots:
    void on_Close_clicked();
    void on_OK_clicked();
};

#endif // SETNAME_H
