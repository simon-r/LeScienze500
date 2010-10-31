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
#include "previewarticolo.h"
#include "browsercopertine.h"
#include "lserrormessage.h"
#include "browsercopertinebase.h"
#include "querydata.h"
#include "bookmarkgui.h"
#include "about.h"


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
    bool OpenPDF( int id_articolo ) ;
    bool OpenPDF( QString file ) ;
    bool OpenPDFDvd( QString file ) ;

    QString getDvdPath() ;


    bool ViewPreview() ;

   // bool OpenBrowserCopertine() ;
    bool OpenBrowserCopertine( int id_articolo = -1 ) ;

    bool ShowErrorMessage( QString error_name , QString message ) ;
    void ShowDBConnectError() ;
    void ShowArticleNotFoundError( QString file_name ) ;
    void ShowReaderNotStartedError() ;
    void ShowCopertineNotFoundError() ;
    void ShowDvdNotFoundError() ;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::LeScienze500 *ui;
    bool fillListaCategorie() ;
    bool fillListaAutori(  QString filtro  = "" ) ;
    bool fillListaAnni() ;
    bool fillRubriche() ;
    bool fillQListWidget( QListWidget *w_list , QueryResult &r_list ) ;

    void fillLists() ;

    void fillResultTable( QueryResult& )  ;

    void fillInformazioni( QModelIndex index ) ;

    void clear() ;
    void clearLists() ;
    void clearResult() ;

    QStringList ReadSelectedItems( QListWidget *ui_list ) ;

    QString pdf_file ;

    QueryResult q_result ;

    Configura *cfg_d ;
    PreviewArticolo *preview ;
    BrowserCopertine *b_copertine_d ;
    LSErrorMessage *error_message ;
    BookmarkGui *bk_gui ;
    About *about_d ;

    void BuildErrorMessage() ;
    void BuildConfigura() ;
    void BuildBookmark() ;
    void BuildAbout() ;

    QList<int> history_id_articoli ;

private slots:

    void on_openPDF( int id_articolo ) ;
    void on_errorLinkClicked( const QUrl &url ) ;
    void on_openBookmark() ;
    void on_openBookmark( int id ) ;
    void on_openAbout() ;
    void on_addFavoriti() ;
    void on_ApriBrowserCopertine( int id ) ;

    void on_pushButton_clicked();
    void on_ApriBrowserCopertine_clicked();
    void on_PreviewArticolo_2_clicked();
    void on_TabellaRisultati_doubleClicked(QModelIndex index);
    void on_PreviewArticolo_clicked();
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

    void on_favoriteSelected() ;

public slots:

    void on_NewConfigFile() ;

};

#endif // LESCIENZE500_H
