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


#ifndef LESCIENZE500_H
#define LESCIENZE500_H

#include <QMainWindow>
#include "querydb.h"
#include <QScrollArea>
#include <QModelIndex>
#include <qprocess.h>
#include <QModelIndex>
#include <QListWidget>
#include <QList>
#include <qstring.h>
#include "configura.h"


namespace Ui {
    class LeScienze500;
}

class LeScienze500 : public QMainWindow {
    Q_OBJECT
public:
    LeScienze500(QWidget *parent = 0);
    ~LeScienze500();

    bool ExecQuery() ;
    bool OpenPDF() ;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::LeScienze500 *ui;
    bool fillListaCategorie() ;
    bool fillListaAutori( string filtro = "" ) ;
    bool fillListaAnni() ;
    bool fillRubriche() ;

    void fillLists() ;

    void fillResultTable( QueryResult )  ;

    void fillInformazioni( QModelIndex index ) ;

    void clear() ;
    void clearLists() ;
    void clearResult() ;

    QStringList ReadSelectedItems( QListWidget *ui_list ) ;

    QString pdf_file ;

    QueryResult q_result ;

    QScrollArea* scrollArea ;

    Configura *cfg_d ;

private slots:

    void on_Select_RicercaTesto_toggled(bool checked);
    void on_CleanAnni_clicked();
    void on_CleanCategorie_clicked();
    void on_CleanRubriche_clicked();
    void on_CleanAutori_clicked();
    void on_CeanCategorie_clicked();
    void on_Select_Anno_toggled(bool checked);
    void on_Select_Rubriche_toggled(bool checked);
    void on_Select_ListaAutori_toggled(bool checked);
    void on_Select_RicercaCategoria_toggled(bool checked);
    void on_Select_ParoleChiave_clicked(bool checked);
    void on_Configura_clicked();
    void on_CopiaASinistra_clicked();
    void on_CopiaADestra_clicked();
    void on_ApriPDF_B_clicked();
    void on_ApriPDF_A_clicked();
    void on_TabellaRisultati_clicked(QModelIndex index);
    void on_FiltroAutori_textChanged(QString filtro);
    void on_Cerca_clicked();
    void on_Select_ParoleChiave_toggled(bool checked);

public slots:

    void on_NewConfigFile() ;

};

#endif // LESCIENZE500_H
