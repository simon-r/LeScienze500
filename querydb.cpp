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


#include "querydb.h"
#include <sqlite3.h>
#include "lescienze500.h"
#include <QDebug>
#include "configls500.h"
#include <QPair>
#include <QtAlgorithms>

QueryDB::QueryDB()
{
    configLS500 cfg ;

    dbPath = cfg.getDBPath() ;
    //dbPath = "./LeScienze.db" ;

    rc = 0 ;
    p_chiave = false ;
    testo = false  ;
    categorie = false  ;
    autori_l = false  ;
    autori_n = false  ;
    anno = false  ;
    rubriche = false  ;
}

QueryResult QueryDB::getCategorie()
{
    QString query = "select Categoria from Categorie order by Categoria" ;
    return execQuery( query ) ;
}

QueryResult QueryDB::getAutori( QString filtro )
{
    list<string> lista_autori ;

    QString query = "select Autore from Autori where Autore like " ;
    query.append("'%");
    query.append( filtro );
    query.append("%'");
    query.append(" order by Autore");

    return execQuery( query ) ;
}

QueryResult QueryDB::getAnni()
{
    QString query = "select anno from Riviste group by anno" ;

    return execQuery( query ) ;
}

QueryResult QueryDB::getRubriche()
{
    QString query = "select rubrica from rubriche" ;

    return execQuery( query ) ;
}



//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
QueryResult QueryDB::execQuery( QString query )
{
    QueryResult q_result ;

    rc = sqlite3_open( dbPath.toAscii().data() , &db);
    if( rc ){
        return q_result;
    }

    rc = sqlite3_prepare_v2( db, query.toAscii().data() , -1, &stmt, 0);
    if( rc ){
        return q_result;
    }

    cols = sqlite3_column_count(stmt);
    for( col=0; col<cols; col++){
        QString name = QString::fromUtf8( (const char*)sqlite3_column_name( stmt, col ) ) ;
        q_result.appendColumnName( name , col );
//        qDebug() << QString( (const char*)sqlite3_column_name( stmt, col ) ) ;
    }

    QStringList row ;
    cols = sqlite3_column_count(stmt);
    // execute the statement

    do{
        rc = sqlite3_step(stmt);
        switch( rc ){
        case SQLITE_DONE:
            break;
        case SQLITE_ROW:
            row.clear();
            // print results for this row
            for( col=0; col<cols; col++){
                row.append( QString::fromUtf8( ( (const char*)sqlite3_column_text( stmt, col ) ) ) ) ;
            }
            q_result.appendResultRow( row ) ;
            break;
        default:
            break;
        }
    }while( rc==SQLITE_ROW );

    // finalize the statement to release resources
    sqlite3_finalize(stmt);

    rc = 0 ;
    return q_result ;
}
