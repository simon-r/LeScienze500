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


#ifndef CONFIGURA_H
#define CONFIGURA_H

#include <QDialog>

namespace Ui {
    class Configura;
}

class Configura : public QDialog {
    Q_OBJECT
public:
    Configura(QWidget *parent = 0);
    ~Configura();

    void setConfigData() ;
    void writeConfigData() ;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Configura *ui;

private slots:
    void on_SearchPdfAppl_clicked();
    void on_SearchPDFPath2_clicked();
    void on_SearchPDFPath1_clicked();
    void on_SearchDBPath_clicked();
    void on_Ok_clicked();
    void on_Cancel_clicked();

signals:

    void sig_NewConfigFile() ;
};

#endif // CONFIGURA_H
