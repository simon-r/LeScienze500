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


#include "lescienze500.h"
#include "ui_lescienze500.h"
#include "configura.h"
#include "querydb.h"
#include <list>
#include <string>
#include <iostream>
#include <sstream>
#include <QErrorMessage>
#include <QStringList>
#include <QTableWidgetItem>
#include <QDebug>
#include "configls500.h"
#include <qapplication.h>
#include <qprocess.h>
#include <QRegExp>
#include <QIcon>
#include "browsercopertine.h"
#include <QDesktopServices>
#include <QDir>
#include <QFileInfoList>
#include "bookmark.h"
#include <QMessageBox>
#include <QFileInfoList>
#include <QCursor>

#ifdef Q_WS_WIN
#include<windows.h>
#endif

using namespace std ;

LeScienze500::LeScienze500(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::LeScienze500)
{
    ui->setupUi(this);

    cfg_d = 0 ;
    preview = 0 ;
    error_message = 0 ;
    b_copertine_d = 0 ;
    bk_gui = 0 ;
    about_d = 0 ;

    fillLists() ;

    connect( ui->actionConfigura , SIGNAL(triggered()) , this , SLOT(on_Configura_clicked()) ) ;
    connect( ui->actionOrganizza_preferiti , SIGNAL(triggered()) , this , SLOT(on_openBookmark()) ) ;
    connect( ui->actionAbout_2 , SIGNAL(triggered()) , this , SLOT(on_openAbout()) ) ;
    connect( ui->actionMostra_Copertine , SIGNAL(triggered()) , this , SLOT(on_ApriBrowserCopertine_clicked()) ) ;
    connect( ui->actionBackup_preferiti , SIGNAL(triggered()) , this , SLOT(on_backUpBookmark()) ) ;

    connect( ui->addFavoriti , SIGNAL(clicked()) , this , SLOT(on_addFavoriti()) ) ;

    BuildBookmark() ;

    connect( ui->menuFavoriti , SIGNAL(aboutToShow()) , this , SLOT(on_menuFavoritesClicked()) ) ;

    menu_organize = new QAction( tr("Organizza preferiti ...") , 0 ) ;
    menu_organize->setIcon( QIcon( ":/icons/crystal/bookmark_add-256.png" ) );
    connect( menu_organize , SIGNAL(triggered()) , this , SLOT(on_openBookmark()) ) ;


    menu_logical = new QMenu() ;
    set_logical = new QActionGroup(0) ;

    menu_AND = new QAction( tr("AND") , 0 ) ;
    menu_AND->setCheckable(true);
    menu_AND->setChecked(true);
    menu_logical->addAction( menu_AND ) ;

    menu_OR = new QAction( tr("OR") , 0 ) ;
    menu_OR->setCheckable(true);
    menu_logical->addAction( menu_OR ) ;

    set_logical->addAction(menu_AND) ;
    set_logical->addAction(menu_OR) ;
    set_logical->setExclusive(true);

    connect(menu_AND,SIGNAL(triggered()),this,SLOT(on_setTestoEstesoLogical())) ;
    connect(menu_OR,SIGNAL(triggered()),this,SLOT(on_setTestoEstesoLogical())) ;

    ui->SelectLogical->setText("AND");
    ui->SelectLogical->setMenu( menu_logical );


    menu_logical_titolo = new QMenu() ;
    set_logical_titolo = new QActionGroup(0) ;

    menu_AND_titolo = new QAction( tr("AND") , 0 ) ;
    menu_AND_titolo->setCheckable(true);
    menu_AND_titolo->setChecked(true);
    menu_logical_titolo->addAction( menu_AND_titolo ) ;

    menu_OR_titolo = new QAction( tr("OR") , 0 ) ;
    menu_OR_titolo->setCheckable(true);
    menu_logical_titolo->addAction( menu_OR_titolo ) ;

    set_logical_titolo->addAction( menu_AND_titolo ) ;
    set_logical_titolo->addAction( menu_OR_titolo ) ;
    set_logical_titolo->setExclusive(true);

    connect(menu_AND_titolo,SIGNAL(triggered()),this,SLOT(on_setTitoloLogical())) ;
    connect(menu_OR_titolo,SIGNAL(triggered()),this,SLOT(on_setTitoloLogical())) ;

    ui->SelectLogicalTitolo->setText("AND");
    ui->SelectLogicalTitolo->setMenu( menu_logical_titolo );
}

LeScienze500::~LeScienze500()
{
    delete ui;
}

void LeScienze500::fillLists()
{
    bool f ;

    clearLists() ;

    f = fillListaCategorie() ;
    f = f && fillListaAutori() ;
    f = f && fillListaAnni() ;
    f = f && fillRubriche() ;

    if ( !f )
    {
        ShowDBConnectError() ;
    }
}

void LeScienze500::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

bool LeScienze500::fillListaCategorie( QString filtro )
{
    QueryDB db ;
    QueryResult lista = db.getCategorie() ;

    return fillQListWidget( ui->ListaCategorie , lista ) ;
}

bool LeScienze500::fillListaAutori(  QString filtro )
{
    QueryDB db ;
    QueryResult lista = db.getAutori( filtro ) ;

    return fillQListWidget( ui->ListaAutori , lista ) ;
}

bool LeScienze500::fillRubriche()
{
    QueryDB db ;
    QueryResult lista = db.getRubriche() ;

    return fillQListWidget( ui->ListaRubriche , lista ) ;
}

bool LeScienze500::fillListaAnni()
{
    QueryDB db ;
    QueryResult lista = db.getAnni() ;

    return fillQListWidget( ui->ListaAnni , lista ) ;
}

bool LeScienze500::fillQListWidget( QListWidget *w_list , QueryResult &r_list )
{
    if ( r_list.empty() )
        return false ;

    w_list->clear();
    QString col_name = r_list.getFirstColumnName() ;

    QueryResult::iterator it;

    for( it = r_list.begin() ; it < r_list.end() ; it++ )
    {
        w_list->addItem( r_list.getField( col_name , it ) );
    }

    return true ;
}

void LeScienze500::clear()
{
    clearLists() ;
    clearResult() ;
}

void LeScienze500::clearLists()
{
    ui->ListaCategorie->clear();
    ui->ListaAutori->clear();
    ui->ListaRubriche->clear();
    ui->ListaAnni->clear();
}

void LeScienze500::clearResult()
{
    ui->TabellaRisultati->clear() ;
    ui->MostraTitolo->clear();
    ui->MostraAbstract->clear();
    ui->MostraData->clear();
    ui->MostraNumero->clear();
    ui->MostraRubrica->clear();
    ui->MostraCategoria->clear();
    ui->MostraFilePDF->clear();
}

bool LeScienze500::ExecQuery()
{
    QueryDB db ;
    QueryData query_data ;

    if ( ui->Select_ParoleChiave->isChecked() )
        db.p_chiave = true ;

    if ( ui->Select_RicercaTesto->isChecked() )
        db.testo = true ;

    if ( ui->Select_RicercaCategoria->isChecked() )
        db.categorie = true ;

    if ( ui->Select_Anno->isChecked() )
        db.anno = true ;

    if ( ui->Select_ListaAutori->isChecked() )
        db.autori_l = true ;

    if ( ui->Select_Rubriche->isChecked() )
        db.rubriche = true ;

    if ( ui->AND_Button->isChecked() )
        db.global_and = true ;
    else
        db.global_and = false ;

    if ( this->menu_AND->isChecked() )
        db.setLogicalTestoEsteso( true );
    else if ( this->menu_OR->isChecked() )
        db.setLogicalTestoEsteso( false );

    if ( this->menu_AND_titolo->isChecked() )
        db.setLogicalTitolo( true );
    else if ( this->menu_OR_titolo->isChecked() )
        db.setLogicalTitolo( false );

    if ( db.p_chiave )
    {
        QString testo = ui->ParoleChiave->text() ;
        testo = testo.replace( "*" , "%" ) ;
        QStringList parole_c = testo.split( " " , QString::SkipEmptyParts ) ;
        db.setParoleChiave( parole_c );

        QString testo_b = ui->ParoleChiaveAbstract->text() ;
        testo_b = testo_b.replace( "*" , "%" ) ;
        QStringList parole_c_a = testo_b.split(" " , QString::SkipEmptyParts ) ;
        db.setParoleChiaveAbstract( parole_c_a );;

        // Nuovo in sviluppo
        query_data.setParoleChiaveTitolo( ui->ParoleChiave->text() );
        query_data.setParoleChiaveAbstract( ui->ParoleChiaveAbstract->text() );
    }

    if ( db.rubriche )
    {
        QStringList lista_rubriche ;
        lista_rubriche = ReadSelectedItems( ui->ListaRubriche ) ;
        db.setListaRubriche( lista_rubriche );

        query_data.setRubriche( lista_rubriche );
    }

    if ( db.categorie )
    {
        QStringList lista_categorie ;
        lista_categorie = ReadSelectedItems( ui->ListaCategorie ) ;
        db.setListaCategorie( lista_categorie ) ;

        query_data.setCategorie( lista_categorie );
    }

    if( db.autori_l )
    {
        QStringList lista_autori ;
        lista_autori = ReadSelectedItems( ui->ListaAutori ) ;
        db.setListaAutori( lista_autori ); ;
    }

    if( db.anno )
    {
        QStringList lista_anni ;
        lista_anni = ReadSelectedItems( ui->ListaAnni ) ;
        db.setListaAnni( lista_anni );
    }

    if ( db.testo )
    {
        QStringList lista_frasi ;
        QString frasi = ui->ParoleChiaveTesto->text() ;
        //qDebug() << "frase: " << frasi ;

        QString word_char = "\\w\\.:;,\\+-/\\*'`" ;
        //QString word_char = "\\w" ;
        QString S_regx = QString( "(\"[%1\\s]+\"|\\s[%1]+\\s|[%1]+\\s|^[%1]+\\s|\\s[%1]+$|[%1]+$|^[%1]+$)" ).arg(word_char) ;
        //QString S_regx = QString( "(\\s[%1]+\\s)" ).arg(word_char) ;

        QRegExp regx( S_regx )  ;

        if ( !regx.isValid() )
        {
            qDebug() << "Regex Error: " << regx.errorString() ;
        }

        qDebug() << S_regx ;

        int pos = 0;
        while ( pos >= 0 )
        {
            pos = regx.indexIn( frasi, pos ) ;
            if ( pos > -1 )
            {
                lista_frasi += regx.cap(1) ;
                pos  += regx.matchedLength() ;
            }
        }

        for (QStringList::iterator it = lista_frasi.begin(); it < lista_frasi.end(); it++ )
        {
            it->remove( QRegExp("(\"|^\\s|\\s$)") ) ;
            qDebug() << "parola: " << *it ;
        }

        db.setFrasiTestoEsteso( lista_frasi ) ;
    }

    QueryResult  q_result ;

    db.execMainQuery( q_result ) ;

    fillResultTable( q_result ) ;

    return true ;
}

QStringList LeScienze500::ReadSelectedItems( QListWidget *ui_list )
{
    QStringList el_list ;
    QList<QListWidgetItem *> c_items = ui_list->selectedItems() ;
    for( QList<QListWidgetItem*>::iterator it = c_items.begin() ; it < c_items.end() ; it++ )
    {
        el_list.append(  (*it)->text() );
        qDebug() << (*it)->text() ;
    }

    return el_list ;
}

//////////////////////////////////////////////////////////////////
void  LeScienze500::fillResultTable( QueryResult& q_res )
{
    q_result.clear();

    q_result = q_res ;

    int cols = q_result.getColumnsCnt()-1 ;

    ui->TabellaRisultati->clearContents();
    ui->TabellaRisultati->setColumnCount( cols );

    QRect rect = ui->TabellaRisultati->geometry() ;

    int w = rect.width() ;

    ui->TabellaRisultati->setColumnWidth( 0 , w * 0.78 );
    ui->TabellaRisultati->setColumnWidth( 1 , w * 0.10 );

    int row = 0 ;
    for( int i = 0 ; i < cols ; i++ )
    {
        QTableWidgetItem item ;
        item.setText( q_result.getColumnName( i ) ) ;
        ui->TabellaRisultati->setHorizontalHeaderItem( i , new QTableWidgetItem( item ) );
    }

    row = q_result.size() ;
    ui->TabellaRisultati->setRowCount( row );

    int r = 0 ;
    int c ;

    for( QueryResult::iterator it_r = q_result.begin() ; it_r < q_result.end() ; it_r++ )
    {
        c = 0 ;
        for( QueryResult::col_iterator it_c = q_result.colBegin() ; it_c < q_result.colEnd()-1 ; it_c++ )
        {
            QTableWidgetItem item ;
            ui->TabellaRisultati->setCurrentCell( r , c );
            item.setText( q_result.getField( it_c , it_r ) );
            item.setFlags( item.flags() & !Qt::ItemIsEditable );
            item.setFlags( item.flags() | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled );

            ui->TabellaRisultati->setItem( r , c , new QTableWidgetItem( item ) );
            c++ ;
        }
        r++ ;
    }
}


void LeScienze500::fillInformazioni( QModelIndex index )
{
    //    QTableWidgetItem *item = ui->TabellaRisultati->takeItem( index.row() , 0 ) ;
    //    QString titolo = item->text();
    //    item->setText( titolo ) ;

    const int index_id = 0 ;
    const int index_rivista = 1 ;
    const int index_rubrica = 2 ;
    const int index_titolo = 3 ;
    const int index_abs = 4 ;
    const int index_pdf = 5 ;

    this->pdf_file.clear();

    //QString id_art = q_result.q_result[index.row()][2] ;
    QString id_art = q_result.getField( 2 , index.row() ) ;

    QString query = "SELECT * FROM articoli WHERE id = " ;
    query.append( id_art ) ;

    QueryDB db ;

    QueryResult qr = db.execQuery( query ) ;

    this->pdf_file = qr.q_result[0][index_pdf] ;

    this->history_id_articoli.push_front( qr.q_result[0][index_id].toInt() ) ;

    //qDebug() << "id a confronto; " << qr.q_result[0][index_id] << "  " << this->id_articolo_attuale  ;

    QString titolo ;
    titolo.append( "<h3><b>" ) ;
    titolo.append( qr.q_result[0][index_titolo] ) ;
    titolo.append( "</b></h3>" ) ;

    ui->MostraTitolo->setHtml( titolo ) ;

    QString query_autori = "SELECT autore FROM autori WHERE id in (SELECT idautore FROM articoli_autori WHERE idarticolo = " ;
    query_autori.append( qr.q_result[0][index_id] ) ;
    query_autori.append( " ) " ) ;

    QueryResult qr_autori =  db.execQuery( query_autori ) ;

    QString lista_autori = "";

    lista_autori.append("<i>") ;
    for ( QList<QStringList>::iterator it_r = qr_autori.q_result.begin() ; it_r < qr_autori.q_result.end() ; it_r++ )
    {
        for( QStringList::iterator it_c = it_r->begin() ; it_c < it_r->end() ; it_c++ )
        {
            lista_autori.append( *it_c ) ;
            lista_autori.append(" <br /> ") ;
        }
    }

    lista_autori.append("</i>") ;

    QString abstract ;
    abstract.append( qr.q_result[0][index_abs] ) ;
    abstract.append( "<br /> <br /> Autori: <br />" ) ;
    abstract.append( lista_autori ) ;

    ui->MostraAbstract->setHtml( abstract );

    QString query_data = "SELECT mese,anno FROM riviste WHERE id = " ;
    query_data.append( qr.q_result[0][index_rivista] ) ;

    qDebug() << query_data ;

    QueryResult qr_data =  db.execQuery( query_data ) ;

    QString data_articolo = "" ;

    for ( QList<QStringList>::iterator it_r = qr_data.q_result.begin() ; it_r < qr_data.q_result.end() ; it_r++ )
    {
        for( QStringList::iterator it_c = it_r->begin() ; it_c < it_r->end() ; it_c++ )
        {
            data_articolo.append( *it_c ) ;
            data_articolo.append("  ") ;
        }
    }
    ui->MostraData->setHtml( data_articolo ) ;


    QString query_numero = "SELECT numero FROM riviste WHERE id = " ;
    query_numero.append( qr.q_result[0][index_rivista] ) ;

    QString numero = "Nr.: ";
    QueryResult qr_numero =  db.execQuery( query_numero ) ;
    for ( QList<QStringList>::iterator it_r = qr_numero.q_result.begin() ; it_r < qr_numero.q_result.end() ; it_r++ )
    {
        for( QStringList::iterator it_c = it_r->begin() ; it_c < it_r->end() ; it_c++ )
        {
            numero.append( *it_c ) ;
        }
    }

    ui->MostraNumero->setHtml( numero );

    QString file_pdf ;
    file_pdf.append( qr.q_result[0][index_pdf] ) ;
    ui->MostraFilePDF->setHtml( file_pdf );

    QString query_rubrica = "SELECT rubrica FROM rubriche WHERE id = " ;
    query_rubrica.append( qr.q_result[0][index_rubrica] ) ;
    QueryResult qr_rubrica =  db.execQuery( query_rubrica ) ;

    QString rubrica ;
    for ( QList<QStringList>::iterator it_r = qr_rubrica.q_result.begin() ; it_r < qr_rubrica.q_result.end() ; it_r++ )
    {
        for( QStringList::iterator it_c = it_r->begin() ; it_c < it_r->end() ; it_c++ )
        {
            rubrica.append( *it_c ) ;
        }
    }
    ui->MostraRubrica->setHtml( rubrica ) ;


    QString query_categoria = "select categoria from categorie where id in ( select idcategoria from articoli_categorie where idarticolo = " ;
    query_categoria.append( qr.q_result[0][index_id] ) ;
    query_categoria.append(" )") ;
    QueryResult qr_categoria =  db.execQuery( query_categoria ) ;
    QString categoria ;
    for ( QList<QStringList>::iterator it_r = qr_categoria.q_result.begin() ; it_r < qr_categoria.q_result.end() ; it_r++ )
    {
        for( QStringList::iterator it_c = it_r->begin() ; it_c < it_r->end() ; it_c++ )
        {
            categoria.append( *it_c ) ;
            categoria.append(" <br /> ") ;
        }
    }
    ui->MostraCategoria->setHtml( categoria ) ;
}

bool LeScienze500::ViewPreview()
{
    if ( this->history_id_articoli.size() == 0 )
        return false ;

    QueryDB db ;
    QString query_testo , query_titolo;
    QString id_a ;

    id_a.setNum( this->history_id_articoli.first() ) ;

    query_testo = "select testoesteso from RicercaEstesa where idarticolo = " ;
    query_testo += id_a ;

    QueryResult qr_testo =  db.execQuery( query_testo ) ;

    if ( qr_testo.size() == 0 )
    {
        this->ShowErrorMessage("Anteprima non disponiblile","Nel database non esiste l'anteprima per questo articolo") ;
        return false ;
    }

    query_titolo = "select titolo, abstract from articoli where id = " ;
    query_titolo += id_a ;

    QueryResult qr_titolo =  db.execQuery( query_titolo ) ;

    if ( preview == 0 )
    {
        preview = new PreviewArticolo() ;

    }

    QString testo ;

    testo = testo + "<h1>" + qr_titolo.q_result[0][0] + "</h1> <br> <HR> <br> ";
    testo = testo + "<i>" + qr_titolo.q_result[0][1] + "</i>" ;
    testo += " <br> <HR> <br>" ;
    testo += qr_testo.q_result[0][0] ;

    preview->setHtml( testo );
    preview->setModal( true );
    preview->setWinTitle( "Anteprima aticolo" ) ;
    preview->show();
    preview->exec();

    return true ;
}


QString LeScienze500::getDvdPath()
{
#ifdef Q_WS_X11
        QString path = "/media/LESCIENZE" ;

        QString path_1 = path + "/DVD1" ;
        QString path_2 = path + "/DVD2" ;

        if ( QFile().exists( path_1 ) || QFile().exists( path_2 ) )
            return path ;
        else
            return QString() ;
#endif

#ifdef Q_WS_WIN
        QFileInfoList dev_list ;
        dev_list = QDir::drives() ;
        for (  QFileInfoList::iterator itr = dev_list.begin() ; itr < dev_list.end() ; itr++ )
        {
            QString path = itr->absolutePath() ;
            QString path_1 = path + "/DVD1" ;
            QString path_2 = path + "/DVD2" ;

            if ( QFile().exists( path_1 ) || QFile().exists( path_2 ) )
            {
                return path ;
            }
        }

        return QString() ;
#endif


}

bool LeScienze500::OpenPDFDvd( QString file_pdf )
{
    QString dvd_dir = this->getDvdPath() ;

    if ( dvd_dir.isEmpty() )
    {
        ShowDvdNotFoundError() ;
        ejectDVD() ;

        return false ;
    }

    QString file_n ;

    bool flag , process_strated ;

    QProcess process_pdf ;

    file_n += dvd_dir ;
    file_n += "/articoli/" ;
    file_n += file_pdf ;

    QFile file ;
    file.setFileName( file_n ) ;
    if ( file.exists() )
    {
        process_strated = startPdfAppl( file_n ) ;
        flag = true ;
    }
    else
    {
        ShowDvdNotFoundError() ;
        this->ejectDVD() ;

        flag = false ;
    }

    return flag && process_strated ;
}

bool LeScienze500::startPdfAppl( QString file_path )
{
    QString pdf_appl , command ;
    bool flag , process_strated ;

    QProcess process_pdf ;

    configLS500 cfg ;
    pdf_appl = cfg.getPDFAppl() ;

    command.append( pdf_appl ) ;
    command.append( " " ) ;
    command.append( "\"" ) ;

    command.append( file_path ) ;
    command.append( "\"" ) ;

    qDebug() << command ;

    if ( pdf_appl != "desktop" )
        process_strated = process_pdf.startDetached( command );
    else
    {
        QUrl url ;
        url.setScheme( "file" );
        url.setPath( file_path );
        process_strated = QDesktopServices::openUrl( url ) ;
    }

    return process_strated ;
}

void LeScienze500::ejectDVD( QString dvd_path )
{
#ifdef Q_WS_X11
    QString command = "eject "  + dvd_path ;
    QProcess().startDetached( command ) ;
#endif

#ifdef Q_WS_WIN
//    QString command ;
//    if ( dvd_path.isEmpty() )
//        command = "set cdaudio door open" ;
//    else
//        command = "open " + dvd_path ;
//    mciSendString( (LPCTSTR)command.data()->toAscii() , null, 0, 0) ;
#endif
}

bool LeScienze500::OpenPDF( QString file_pdf )
{
    QProcess process_pdf ;
    bool process_strated = false ;

    configLS500 cfg ;

    QString dvd = cfg.getDVD() ;
    QRegExp reg( "(yes)" ) ;
    if ( reg.indexIn( dvd ) > -1  ){
        return this->OpenPDFDvd( file_pdf ) ;
    }

    QString file_p , pdf_appl , command ;

    bool flag = false ;

    pdf_appl = cfg.getPDFAppl() ;

    file_p.append( cfg.getPDFPath1() ) ;
    file_p.append( "/" ) ;
    file_p.append( file_pdf ) ;

    //qDebug() << file_p ;

    QFile file ;
    file.setFileName( file_p );
    if ( file.exists() )
    {
        process_strated = startPdfAppl( file_p ) ;
        flag = true ;
    }

    file_p.clear();
    file_p.append( cfg.getPDFPath2() ) ;
    file_p.append( "/" ) ;
    file_p.append( file_pdf ) ;

    file.setFileName( file_p );
    if ( file.exists() && flag == false )
    {
        process_strated = startPdfAppl( file_p ) ;
        flag = true ;
    }

    if ( flag == false )
    {
        ShowArticleNotFoundError( this->pdf_file ) ;
        return false ;
    }

    if ( process_strated == false )
    {
        ShowReaderNotStartedError();
    }

    return flag ;
}

bool LeScienze500::OpenPDF( int id_articolo )
{
    QString query = "select FilePDF from Articoli where id = " ;
    query += QString().setNum( id_articolo ) ;

    QueryDB db ;
    QueryResult qr_pdf = db.execQuery( query ) ;

    if ( qr_pdf.empty() )
        return false ;

    QString file_pdf = qr_pdf.getField( "FilePDF" , qr_pdf.begin() ) ;

    return this->OpenPDF( file_pdf ) ;
}

bool LeScienze500::OpenPDF()
{

    QFileInfoList drives = QDir::drives() ;


    for ( QFileInfoList::iterator it = drives.begin() ; it < drives.end() ; it++ )
    {
        qDebug() << it->path() ;
    }


    if ( this->pdf_file.isEmpty() )
        return false ;

    return this->OpenPDF( this->pdf_file ) ;
}

bool LeScienze500::OpenBrowserCopertine( int id_articolo )
{
    if ( this->b_copertine_d == 0 )
    {
        this->b_copertine_d = new BrowserCopertine() ;
        connect( this->b_copertine_d , SIGNAL( sig_openPDF(int) ) , this , SLOT( on_openPDF(int) ) ) ;
        connect( this->b_copertine_d , SIGNAL(sig_addBookmark(int)) , this , SLOT(on_openBookmark(int)) ) ;
    }

    configLS500 cfg ;
    QString file_n = cfg.getCopertinePath() ;
    QFile file ;

    file_n.append("/000.jpg") ;
    file.setFileName( file_n );

    if ( !file.exists() )
    {
        this->ShowCopertineNotFoundError() ;
        return false ;
    }

    if ( id_articolo > -1 )
        b_copertine_d->openBrowserID( id_articolo );
    else
        b_copertine_d->openBrowser();
    return true ;
}

bool LeScienze500::ShowErrorMessage( QString error_name , QString message )
{
    BuildErrorMessage() ;

    this->error_message->setHtmlMessage( error_name , message );

    this->error_message->setModal( true );
    this->error_message->setFocus();
    this->error_message->show();

    return true ;
}

void LeScienze500::ShowDBConnectError()
{
    BuildErrorMessage() ;
    this->error_message->showDBError();
}


void LeScienze500::ShowArticleNotFoundError( QString file_name )
{
    BuildErrorMessage() ;
    this->error_message->showArticleNotFound( file_name );
}

void LeScienze500::ShowReaderNotStartedError()
{
    BuildErrorMessage() ;
    this->error_message->showReaderNotStarted();
}

void LeScienze500::ShowCopertineNotFoundError()
{
    BuildErrorMessage() ;
    this->error_message->showCopertineNotFoundError();
}

void LeScienze500::ShowDvdNotFoundError()
{
    BuildErrorMessage() ;
    this->error_message->showDvdNotFoundError();
}

void LeScienze500::BuildErrorMessage()
{
    if ( this->error_message == 0 )
    {
        this->error_message = new LSErrorMessage() ;
        connect( error_message , SIGNAL(sig_linkClicked(QUrl)) , this , SLOT(on_errorLinkClicked(QUrl))) ;
    }
}

void LeScienze500::BuildConfigura()
{
    if ( cfg_d == 0 )
    {
        cfg_d = new Configura() ;
        connect( cfg_d , SIGNAL(sig_NewConfigFile()) , this , SLOT(on_NewConfigFile()) ) ;
    }
}

void LeScienze500::BuildBookmark()
{
    if ( bk_gui == 0 )
    {
        bk_gui = new BookmarkGui() ;
        connect( bk_gui , SIGNAL(sig_openPdf(int)) , this , SLOT(on_openPDF(int)) ) ;
        connect( bk_gui , SIGNAL(sig_openBrowser(int)) , this , SLOT(on_ApriBrowserCopertine(int)) ) ;

        bk_gui->setMenuFavorites( ui->menuFavoriti );
        //bk_gui->fillBookmarkMenu() ;
    }
}

  void LeScienze500::BuildAbout()
  {
      if ( about_d == 0 )
      {
          about_d = new About() ;
      }
  }

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

void LeScienze500::on_openPDF( int id_articolo )
{
    this->OpenPDF( id_articolo ) ;
}

void LeScienze500::on_openBookmark()
{
    BuildBookmark() ;
    this->bk_gui->open();
}

void LeScienze500::on_openBookmark( int id )
{
    BuildBookmark() ;
    this->bk_gui->open( QString().setNum( id ) );
}

void LeScienze500::on_openAbout()
{
    BuildAbout() ;
    this->about_d->setModal( true ) ;
    this->about_d->show() ;
    this->about_d->exec() ;
}

void LeScienze500::on_errorLinkClicked( const QUrl &url )
{

    BuildConfigura() ;

    QString error_msg = url.toString() ;
    QRegExp reg ;
    reg.setPattern( "(lescienze500\\.wordpress\\.com)" );

    if ( reg.indexIn( error_msg ) > -1 )
    {
           if ( QDesktopServices::openUrl ( url ) )
           return ;
    }
    this->error_message->close() ;
    this->cfg_d->setConfigData();

    reg.setPattern( "(#configura_copertine)" );
    if ( reg.exactMatch( error_msg ) )
    {
        this->cfg_d->setCurrentTool( Configura::SetCopertinePath );
    }

    reg.setPattern( "(#configura_articoli)" );
    if ( reg.exactMatch( error_msg ) )
    {
        this->cfg_d->setCurrentTool( Configura::SetPdfPath );
    }

    reg.setPattern( "(#configura_db)" );
    if ( reg.exactMatch( error_msg ) )
    {
        this->cfg_d->setCurrentTool( Configura::SetDbPath );
    }

    reg.setPattern( "(#configura_reader)" );
    if ( reg.exactMatch( error_msg ) )
    {
        this->cfg_d->setCurrentTool( Configura::SetPdfReader );
    }

    this->cfg_d->setModal( true );
    this->cfg_d->show();
    this->cfg_d->exec() ;
    return ;
}

void LeScienze500::on_addFavoriti()
{
    if ( this->history_id_articoli.isEmpty() )
        return ;

    int id = this->history_id_articoli.first() ;
    BuildBookmark() ;
    this->bk_gui->open( QString().setNum( id ) );
}

void LeScienze500::on_Select_ParoleChiave_toggled(bool checked)
{
    ui->ParoleChiave->setReadOnly( !checked );
    ui->ParoleChiaveAbstract->setReadOnly( !checked );

    ui->ParoleChiave->setEnabled( checked );
    ui->ParoleChiaveAbstract->setEnabled( checked );
    ui->CopiaADestra->setEnabled( checked );
    ui->CopiaASinistra->setEnabled( checked );

    ui->SelectLogicalTitolo->setEnabled( checked );
}

void LeScienze500::on_Cerca_clicked()
{
    QCursor cursor ;

    cursor.setShape( Qt::WaitCursor ) ;
    ui->centralWidget->setCursor( cursor );

    ExecQuery() ;

    cursor.setShape( Qt::ArrowCursor ) ;
    ui->centralWidget->setCursor( cursor );
}

void LeScienze500::on_FiltroAutori_textChanged( QString filtro )
{
    this->fillListaAutori( filtro ) ;
}

void LeScienze500::on_TabellaRisultati_clicked(QModelIndex index)
{
    fillInformazioni( index ) ;
}

void LeScienze500::on_ApriPDF_A_clicked()
{
    OpenPDF() ;
}

void LeScienze500::on_ApriPDF_B_clicked()
{
    OpenPDF() ;
}

void LeScienze500::on_CopiaADestra_clicked()
{
    QString sinistra = ui->ParoleChiave->text() ;
    ui->ParoleChiaveAbstract->clear();
    ui->ParoleChiaveAbstract->setText( sinistra );
}

void LeScienze500::on_CopiaASinistra_clicked()
{
    QString destra = ui->ParoleChiaveAbstract->text() ;
    ui->ParoleChiave->clear();
    ui->ParoleChiave->setText( destra );
}

void LeScienze500::on_Configura_clicked()
{
    BuildConfigura() ;

    cfg_d->setModal(true);
    cfg_d->setFocus();
    cfg_d->setConfigData() ;
    cfg_d->show();
}


void LeScienze500::on_NewConfigFile()
{
    fillLists() ;
}


void LeScienze500::on_Select_ParoleChiave_clicked(bool checked)
{
}

void LeScienze500::on_Select_RicercaCategoria_toggled(bool checked)
{
    ui->ListaCategorie->setEnabled( checked );
    ui->CleanCategorie->setEnabled( checked );
}

void LeScienze500::on_Select_ListaAutori_toggled(bool checked)
{
    ui->ListaAutori->setEnabled( checked );
    ui->FiltroAutori->setEnabled( checked );
    ui->CleanAutori->setEnabled( checked );
}



void LeScienze500::on_Select_Rubriche_toggled(bool checked)
{
    ui->ListaRubriche->setEnabled( checked );
    ui->CleanRubriche->setEnabled( checked );
}

void LeScienze500::on_Select_Anno_toggled(bool checked)
{
    ui->ListaAnni->setEnabled( checked );
    ui->CleanAnni->setEnabled( checked );
}

void LeScienze500::on_CleanCategorie_clicked()
{
    ui->ListaCategorie->clear() ;
    this->fillListaCategorie() ;
}

void LeScienze500::on_CleanAutori_clicked()
{
    ui->ListaAutori->clear();
    this->fillListaAutori() ;
}

void LeScienze500::on_CleanRubriche_clicked()
{
    ui->ListaRubriche->clear();
    this->fillRubriche() ;
}


void LeScienze500::on_CeanCategorie_clicked()
{
}

void LeScienze500::on_CleanAnni_clicked()
{
    ui->ListaAnni->clear();
    this->fillListaAnni() ;
}

void LeScienze500::on_Select_RicercaTesto_toggled(bool checked)
{
    ui->ParoleChiaveTesto->setEnabled( checked );
    ui->SelectLogical->setEnabled( checked );
}

void LeScienze500::on_PreviewArticolo_clicked()
{
    ViewPreview() ;
}

void LeScienze500::on_TabellaRisultati_doubleClicked(QModelIndex index)
{
    fillInformazioni( index ) ;
    OpenPDF() ;
}

void LeScienze500::on_PreviewArticolo_2_clicked()
{
    ViewPreview() ;
}

void LeScienze500::on_ApriBrowserCopertine_clicked()
{
     OpenBrowserCopertine() ;
}

void LeScienze500::on_pushButton_clicked()
{
    if ( this->history_id_articoli.isEmpty() ) return ;

    int id = this->history_id_articoli.front() ;
    this->OpenBrowserCopertine( id ) ;
}

void LeScienze500::on_ApriBrowserCopertine( int id )
{
    this->OpenBrowserCopertine( id ) ;
}


void LeScienze500::on_menuFavoritesClicked()
{
    this->bk_gui->fillBookmarkMenu( menu_organize ) ;
}

void LeScienze500::on_backUpBookmark()
{
    QMessageBox msgBox;
    msgBox.setMinimumWidth( 300 );
    msgBox.setText( tr("Vuoi eseguire subito un backup dei preferiti?") );
    msgBox.setInformativeText( tr("Il backup verra salvato nella directory di configurazione. Visita la home page per ulteriori informazioni")  );
    msgBox.setIcon( QMessageBox::Question ) ;
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    int ret = msgBox.exec();

    Bookmark bk ;
    bool f ;

    switch (ret) {
    case QMessageBox::Ok:
          f = bk.backupDatabase() ;
          break;
      case QMessageBox::Cancel:
          return ;
          break;
      default:
          return ;
          break;
    }

    QMessageBox msgBox2;
    msgBox2.setMinimumWidth( 300 );
    msgBox2.setIcon( QMessageBox::Information );

    msgBox2.setStandardButtons(QMessageBox::Ok);

    if ( f )
        msgBox2.setText( tr("Backup avvenuto con sucesso") );
    else
        msgBox2.setText( tr("Backup non riuscito") );

    msgBox2.exec();
}

void LeScienze500::on_setTestoEstesoLogical()
{
    if ( this->menu_AND->isChecked() )
        ui->SelectLogical->setText("AND");
    else
        ui->SelectLogical->setText("OR ");
}

void LeScienze500::on_setTitoloLogical()
{
    if ( this->menu_AND_titolo->isChecked() )
        ui->SelectLogicalTitolo->setText("AND");
    else
        ui->SelectLogicalTitolo->setText("OR ");
}
