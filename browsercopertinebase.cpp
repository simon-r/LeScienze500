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

bool BrowserCopertineBase::fillListaAnni()
{
    QueryDB db ;
    QueryResult lista = db.getAnni() ;

    if ( lista.empty() )
        return false ;

    QString col_name = lista.getFirstColumnName() ;

    QueryResult::iterator it;

    for( it = lista.begin() ; it < lista.end() ; it++ )
    {
        this->appendAnnoGUI( lista.getField( col_name , it ) );
    }
    return true ;
}

bool BrowserCopertineBase::showAnno( const QString &anno )
{
    QString query = "select FileCopertina, Mese  from Riviste Where anno = " ;
    query += anno ;
    query += " order by FileCopertina " ;

    QueryDB db ;
    QueryResult riviste_anno = db.execQuery( query ) ;

    configLS500 cfg ;

    QString path_copertine = cfg.getCopertinePath() ;

    this->openListaRiviste( anno ) ;
    for ( QueryResult::iterator it = riviste_anno.begin() ; it < riviste_anno.end() ; it++ )
    {
        QString copertina = riviste_anno.getField( "FileCopertina" , it ) ;
        copertina.replace( QRegExp( "(^[\\d]{4,4}_)" ) , "" ) ;
        copertina.replace( QRegExp( "pdf$" ) , "jpg" ) ;
        copertina.prepend( path_copertine ) ;

        QString mese = riviste_anno.getField( QString( "Mese" ) , it ) ;

//         qDebug() << copertina << " " << mese ;

        this->appendRivista( copertina , mese ) ;
    }
    this->closeListaCopertine();
}
