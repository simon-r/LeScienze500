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

#include "browsercopertinebase.h"
#include <QDebug>
#include "configls500.h"


BrowserCopertineBase::BrowserCopertineBase()
{
}

void BrowserCopertineBase::openBrowserID( int id_articolo )
{
    this->openBrowser();
    this->showMeseAnno( id_articolo ) ;
}

bool BrowserCopertineBase::fillListaAnni()
{
    QueryDB db ;
    QueryResult lista = db.getAnni() ;

    if ( lista.empty() )
        return false ;

    QString col_name = lista.getFirstColumnName() ;

    this->clearListaAnni();
    for( QueryResult::iterator it = lista.begin() ; it < lista.end() ; it++ )
    {
        this->appendAnnoGUI( lista.getField( col_name , it ) );
    }

    return true ;
}

bool BrowserCopertineBase::showMeseAnno( int id_articolo )
{
    QString query = "select Mese, Anno, Numero from riviste where id in ( select idrivista from articoli where id = " ;
    query += QString().setNum( id_articolo ) ;
    query += " ) " ;

    QueryDB db ;
    QueryResult qr_mese_anno = db.execQuery( query ) ;

    if ( qr_mese_anno.empty() )
        return false ;

    QString mese = qr_mese_anno.getField( "Mese" , qr_mese_anno.begin() ) ;
    QString anno = qr_mese_anno.getField( "Anno" , qr_mese_anno.begin() ) ;
    QString numero = qr_mese_anno.getField( "Numero" , qr_mese_anno.begin() ) ;

    bool ret = this->showMeseAnno( mese , anno ) ;
    this->moveToMese( mese ) ;
    this->setCurrentNumero( numero.toInt() );

    return ret ;
}

bool BrowserCopertineBase::showMeseAnno( const QString &numero )
{
    QString query = "select mese, anno from riviste where numero =  " ;
    query += numero ;

    QueryDB db ;
    QueryResult qr_mese_anno = db.execQuery( query ) ;

    if ( qr_mese_anno.empty() )
        return false ;

    QString mese = qr_mese_anno.getField( "Mese" , qr_mese_anno.begin() ) ;
    QString anno = qr_mese_anno.getField( "Anno" , qr_mese_anno.begin() ) ;

    bool ret = this->showMeseAnno( mese , anno ) ;
    this->moveToMese( mese ) ;

    return ret ;
}

bool BrowserCopertineBase::showMeseAnno( const QString &mese , const QString &anno )
{
    bool ret ;
    ret = this->showAnno( anno ) ;
    ret = ret && this->showRivista( mese , anno ) ;

    return ret ;
}

bool BrowserCopertineBase::showAnno( const QString &anno )
{
    QString query = "select FileCopertina, Mese, Numero from Riviste Where anno = " ;
    query += anno ;
    query += " order by FileCopertina " ;

    QueryDB db ;
    QueryResult riviste_anno = db.execQuery( query ) ;

    if ( riviste_anno.empty() )
        return false ;

    configLS500 cfg ;

    QString path_copertine = cfg.getCopertinePath() ;

    this->openListaRiviste( anno ) ;
    for ( QueryResult::iterator it = riviste_anno.begin() ; it < riviste_anno.end() ; it++ )
    {
        QString copertina = riviste_anno.getField( "FileCopertina" , it ) ;
        copertina.replace( QRegExp( "(^[\\d]{4,4}_)" ) , "" ) ;
        copertina.replace( QRegExp( "pdf$" ) , "jpg" ) ;
        copertina.prepend( "/" ) ;
        copertina.prepend( path_copertine ) ;

        QString mese = riviste_anno.getField( QString( "Mese" ) , it ) ;

        //         qDebug() << copertina << " " << mese ;

        this->appendRivista( copertina , mese ) ;
    }
    this->closeListaCopertine();

    return true ;
}

bool BrowserCopertineBase::showRivista( const QString &mese , const QString &anno )
{
    QString query = "select Numero from riviste where mese like \"" ;
    query += mese ;
    query += "\" and anno = " ;
    query += anno ;

    QueryDB db ;
    QueryResult qr_numero = db.execQuery( query ) ;

    if( qr_numero.empty() )
        return false ;

    QString numero = qr_numero.getField( "Numero" , qr_numero.begin() ) ;

    this->showRivista( numero ) ;
    this->setCurrentNumero( numero.toInt() );

    return true ;
}

bool BrowserCopertineBase::showRivista( const QString &numero )
{
    QString query = "select Mese, Anno, FileCopertina from riviste where numero = " ;
    query += numero ;
    query += " " ;

    QueryDB db ;
    QueryResult nr_rivista = db.execQuery( query ) ;

    QString mese = nr_rivista.getField( "Mese" , nr_rivista.begin() ) ;
    QString anno = nr_rivista.getField( "Anno" , nr_rivista.begin() ) ;
    QString copertina = nr_rivista.getField( "FileCopertina" , nr_rivista.begin() ) ;

    configLS500 cfg ;
    QString path_copertine = cfg.getCopertinePath() ;

    copertina.replace( QRegExp( "(^[\\d]{4,4}_)" ) , "" ) ;
    copertina.replace( QRegExp( "pdf$" ) , "jpg" ) ;
    copertina.prepend( "/" ) ;
    copertina.prepend( path_copertine ) ;

    this->openNumeroRivista( copertina , numero , mese , anno ) ;

    query = "select Id, Titolo, Abstract from articoli where idrivista = ( select id from riviste where numero = " ;
    query += numero ;
    query += " )" ;

    QueryResult articoli = db.execQuery( query ) ;

    if( articoli.empty() )
        return false ;

    for ( QueryResult::iterator it = articoli.begin() ; it < articoli.end() ; it++ )
    {
        QString id = articoli.getField( "Id" , it ) ;
        QString abstract = articoli.getField( "Abstract" , it ) ;
        QString titolo = articoli.getField( "Titolo" , it ) ;

        query = "select Autore from autori where id  in ( select idautore from articoli_autori where idarticolo = " ;
        query += id ;
        query += " ) " ;
        QueryResult qr_autori = db.execQuery( query ) ;

        QString autori = "" ;
        for ( QueryResult::iterator ita = qr_autori.begin() ; ita < qr_autori.end() ; ita++ )
        {
            autori += qr_autori.getField( "Autore" , ita ) ;
            if ( ita !=  qr_autori.end() - 1 )
                autori += "; " ;
        }
        this->appendArticolo( titolo , abstract , autori , id ) ;
    }

    this->closeRivista() ;

    return true ;
}
