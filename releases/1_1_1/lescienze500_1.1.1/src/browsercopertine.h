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

#ifndef BROWSERCOPERTINE_H
#define BROWSERCOPERTINE_H

#include <QDialog>
#include "browsercopertinebase.h"
#include <QListWidgetItem>
#include <QUrl>

namespace Ui {
    class BrowserCopertine;
}

class BrowserCopertine : public QDialog, public BrowserCopertineBase
{
    Q_OBJECT
public:
    BrowserCopertine(QWidget *parent = 0);
    ~BrowserCopertine();

    void openBrowser() ;

protected:
    void changeEvent(QEvent *e);

    enum Blank { BlankAnno = 1 , BlanckRivista = 2 } ;
    void blankPage( int bl ) ;

    // funzioni virtuali ereditate:
    void appendAnnoGUI( QString anno ) ;
    void clearListaAnni() ;

    void openListaRiviste( const QString anno ) ;
    void appendRivista( const QString copertina , const QString mese ) ;
    void closeListaCopertine() ;

    void openNumeroRivista( QString copertina , QString numero , QString mese , QString anno ) ;
    void appendArticolo( QString titolo , QString abstract , QString autori , QString id ) ;
    void closeRivista() ;

    void setCurrentNumero( int nr ) ;

    void moveToMese( const QString &mese ) ;

private:
    Ui::BrowserCopertine *ui;

    QString pagina_anno ;
    QString anno_c ;

    QString rivista ;

public slots:
    void on_itemListaAnniSelected( QListWidgetItem* item ) ;
    void on_rivistaClicked( const QUrl &url ) ;
    void on_indiceArticoliClicked( const QUrl &url ) ;
    void on_spinNuovoNumero( const QString &numero ) ;

signals:
    void sig_openPDF (int id_articolo) ;
    void sig_addBookmark (int id_articolo) ;


};

#endif // BROWSERCOPERTINE_H
