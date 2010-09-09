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

using namespace std ;

LeScienze500::LeScienze500(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LeScienze500)
{
    ui->setupUi(this);
    cfg_d = 0 ;
//    scrollArea = new QScrollArea;
//    scrollArea->setWidget( this );
//    scrollArea->setWidgetResizable(true);
//    setCentralWidget(scrollArea);

    fillLists() ;
}

LeScienze500::~LeScienze500()
{
    delete ui;
}

void LeScienze500::fillLists()
{
    bool f ;

    f = fillListaCategorie() ;
    f = f && fillListaAutori() ;
    f = f && fillListaAnni() ;
    f = f && fillRubriche() ;

    if ( !f )
    {
        QErrorMessage Errore_db ;
        Errore_db.showMessage( QString("Errore di accesso al database") ) ;
        Errore_db.exec();
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

bool LeScienze500::fillListaCategorie()
{
    QueryDB db ;
    list<string> lista = db.getCategorie() ;

    if ( lista.empty() )
        return false ;

    list<string>::const_iterator it;

    for( it = lista.begin() ; it != lista.end() ; it++ )
    {
        ui->ListaCategorie->addItem( QString( it->c_str() ) );
        cout << it->c_str() << endl ;
    }

    return true ;
}

bool LeScienze500::fillListaAutori( string filtro )
{
    QueryDB db ;
    list<string> lista = db.getAutori( filtro ) ;

    if ( lista.empty() )
        return false ;

    list<string>::const_iterator it;

    ui->ListaAutori->clear();

    for( it = lista.begin() ; it != lista.end() ; it++ )
    {
        ui->ListaAutori->addItem( QString( it->c_str() ) );
        cout << it->c_str() << endl ;
    }

    return true ;
}

bool LeScienze500::fillRubriche()
{
    QueryDB db ;
    list<string> lista = db.getRubriche() ;

    if ( lista.empty() )
        return false ;

    list<string>::const_iterator it;

    for( it = lista.begin() ; it != lista.end() ; it++ )
    {
        ui->ListaRubriche->addItem( QString( it->c_str() ) );
        cout << it->c_str() << endl ;
    }

    return true ;
}

bool LeScienze500::fillListaAnni()
{
    QueryDB db ;
    list<int> lista = db.getAnni() ;

    if ( lista.empty() )
        return false ;

    list<int>::const_iterator it;

    for( it = lista.begin() ; it != lista.end() ; it++ )
    {
        stringstream ss;
        ss << *it;
        ui->ListaAnni->addItem( QString( ss.str().c_str() ) );
        cout << *it << endl ;
    }

    return true ;
}

bool LeScienze500::ExecQuery()
{
    QueryDB db ;

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

//    if ( ui->Select_NomeAutore->isChecked() )
//        db.autori_n = true ;

    if ( db.p_chiave )
    {
        QString testo = ui->ParoleChiave->text() ;
        testo = testo.replace( "*" , "%" ) ;
        QStringList parole_c = testo.split(" " , QString::SkipEmptyParts ) ;

        db.setParoleChiave( parole_c ) ;

        QString testo_b = ui->ParoleChiaveAbstract->text() ;
        testo_b = testo_b.replace( "*" , "%" ) ;
        QStringList parole_c_a = testo_b.split(" " , QString::SkipEmptyParts ) ;

        db.setParoleChiaveAbstract( parole_c_a ) ;
    }

    if ( db.rubriche )
    {
        QStringList lista_rubriche ;
        QList<QListWidgetItem *> s_items = ui->ListaRubriche->selectedItems() ;
        for (  QList<QListWidgetItem*>::iterator it = s_items.begin() ; it < s_items.end() ; it++ )
        {
            lista_rubriche.append( (*it)->text() ) ;
            qDebug() << (*it)->text() ;
        }
        db.setListaRubriche( lista_rubriche );
    }

    if ( db.categorie )
    {
        QStringList lista_categorie ;
        QList<QListWidgetItem *> c_items = ui->ListaCategorie->selectedItems() ;
        for (  QList<QListWidgetItem*>::iterator it = c_items.begin() ; it < c_items.end() ; it++ )
        {
            lista_categorie.append( (*it)->text() ) ;
            qDebug() << (*it)->text() ;
        }
        db.setListaCategorie( lista_categorie );
    }

    QueryResult q_result = db.execMainQuery() ;
    fillResultTable( q_result ) ;

    return true ;
}


//////////////////////////////////////////////////////////////////
void  LeScienze500::fillResultTable( QueryResult q_res )
{

    q_result.clear();

    q_result = q_res ;

    int cols = q_result.columns_names.size()-1 ;
    ui->TabellaRisultati->setColumnCount( cols );

    ui->TabellaRisultati->setColumnWidth( 0 , 390 );
    ui->TabellaRisultati->setColumnWidth( 1 , 40 );

    int row = 0 ;
    for( int i = 0 ; i < cols ; i++ )
    {
        QTableWidgetItem item ;
        item.setText( q_result.columns_names[i] ) ;
        ui->TabellaRisultati->setHorizontalHeaderItem( i , new QTableWidgetItem( item ) );
    }

    row = q_result.q_result.size() ;
    ui->TabellaRisultati->setRowCount( row );

    int r = 0 ;
    int c ;

    for( QList<QStringList>::iterator it_r = q_result.q_result.begin() ; it_r < q_result.q_result.end() ; it_r++ )
    {
        c = 0 ;
        for( QStringList::iterator it_c = it_r->begin() ; it_c < it_r->end()-1 ; it_c++ )
        {
            QTableWidgetItem item ;
            ui->TabellaRisultati->setCurrentCell( r , c );
            item.setText( *it_c );
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

    QString id_art = q_result.q_result[index.row()][2] ;

    QString query = "SELECT * FROM articoli WHERE id = " ;
    query.append( id_art ) ;

    QueryDB db ;

    QueryResult qr = db.execQuery( query ) ;

    this->pdf_file = qr.q_result[0][index_pdf] ;

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


bool LeScienze500::OpenPDF()
{
    QProcess process_pdf ;
    if ( this->pdf_file.isEmpty() )
        return false ;

    configLS500 cfg ;

    QString file_p , pdf_appl , command ;

    bool flag = false ;

    pdf_appl = cfg.getPDFAppl();

    file_p.append( cfg.getPDFPath1() ) ;
    file_p.append( "" ) ;
    file_p.append( this->pdf_file ) ;

    //qDebug() << file_p ;

    QFile file ;
    file.setFileName( file_p );
    if ( file.exists() )
    {
        command.append( pdf_appl ) ;
        command.append( " " ) ;
        command.append( "\"" ) ;

        command.append( file_p ) ;
        command.append( "\"" ) ;

        qDebug() << command ;
        process_pdf.startDetached( command );
        flag = true ;
    }


    file_p.clear();
    file_p.append( cfg.getPDFPath2() ) ;
    file_p.append( "" ) ;
    file_p.append( this->pdf_file ) ;

    file.setFileName( file_p );
    if ( file.exists() && flag == false )
    {
        command.append( pdf_appl ) ;
        command.append( " " ) ;
        command.append( "\"" ) ;

        command.append( file_p ) ;
        command.append( "\"" ) ;

        qDebug() << command ;
        process_pdf.startDetached( command );
        flag = true ;
    }

//    if ( flag == false )
//    {
//        QErrorMessage em ;
//        em.showMessage ( "Impossibile trovare il file: \n Prova a cambiare il DVD o setta le directory correttamente" ) ;
//    }

    return flag ;
}


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

void LeScienze500::on_Select_ParoleChiave_toggled(bool checked)
{
        ui->ParoleChiave->setReadOnly( !checked );
        ui->ParoleChiaveAbstract->setReadOnly( !checked );
}

void LeScienze500::on_Cerca_clicked()
{
    ExecQuery() ;
}

void LeScienze500::on_FiltroAutori_textChanged( QString filtro )
{
    string fil_s = string ( filtro.toAscii().data() ) ;

//    cout << fil_s ;
//
    this->fillListaAutori( fil_s ) ;
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
    if ( cfg_d == 0 )
        cfg_d = new Configura() ;

    cfg_d->setModal(true);
    cfg_d->setFocus();
    cfg_d->show();
}
