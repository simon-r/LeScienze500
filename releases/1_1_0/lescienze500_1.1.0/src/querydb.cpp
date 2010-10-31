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
#include <stdlib.h>
#include <QLatin1String>

QueryDB::QueryDB()
{
    configLS500 cfg ;

    dbPath = cfg.getDBPath() ;
    //dbPath = "./LeScienze.db" ;

    //rc = 0 ;
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

bool QueryDB::execNAQuery(QString db_path , QString query )
{
    sqlite3* db;
    char* db_err;
    int sqlite_err ;

    sqlite3_open( db_path.toUtf8().data() , &db ) ;

    sqlite3_exec( db , "PRAGMA encoding = UTF8", NULL , 0 , &db_err );
    sqlite_err = sqlite3_exec(db, query.toUtf8().data() , NULL, 0, &db_err) ;
    sqlite3_close(db) ;

    if ( sqlite_err == SQLITE_OK )
        return true ;
    else
    {
        qDebug() << "SQL Lite error code: " << sqlite_err ;
        return false ;
    }
}

void QueryDB::execQuery( const QString& db_path , const QString& query , QueryResult& q_result )
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc = 0 ;
    int col, cols;

    q_result.clear();
    rc = sqlite3_open( db_path.toAscii().data() , &db );
    if( rc ){
        return ;
    }

    rc = sqlite3_prepare_v2( db, query.toUtf8().data() , -1, &stmt, 0);
    if( rc ){
        return ;
    }

    cols = sqlite3_column_count(stmt);
    for( col=0; col<cols; col++)
    {
        char *sql_buffer = NULL ;
        sql_buffer = (char*)sqlite3_column_name( stmt, col ) ;
        QString name = QString::fromUtf8( (const char*)sql_buffer ) ;

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
                const unsigned char *sql_buffer = NULL ;
                sql_buffer = sqlite3_column_text( stmt, col ) ;
                //if ( cols == 1 )
                row.append( QString::fromUtf8( (const char*)sql_buffer ) ) ;
            }
            q_result.appendResultRow( row ) ;
            break;
        default:
            break;
        }
    } while ( rc == SQLITE_ROW ) ;

    // finalize the statement to release resources
    sqlite3_finalize(stmt);
    sqlite3_close( db ) ;
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
QueryResult QueryDB::execQuery( QString query )
{
    QueryResult q_result ;
    execQuery( dbPath , query , q_result ) ;
    return q_result ;
}


int QueryDB::backup( QString from_db , QString to_db )
{
    sqlite3 *p_from , *p_dest ;
    sqlite3_backup *p_backup ;
    int rc = 0 ;

    rc = sqlite3_open( from_db.toAscii().data() , &p_from );
    if( rc != SQLITE_OK ){
        return rc ;
    }

    rc = sqlite3_open( to_db.toAscii().data() , &p_dest );
    if( rc != SQLITE_OK && rc != SQLITE_ERROR ){
        sqlite3_close( p_from ) ;
        return rc ;
    }

    p_backup = sqlite3_backup_init( p_dest , "main" , p_from , "main" );
    if( p_backup )
    {
        do {
            rc = sqlite3_backup_step( p_backup , 1 ) ;
        } while( rc==SQLITE_OK || rc==SQLITE_BUSY || rc==SQLITE_LOCKED );
        sqlite3_backup_finish( p_backup );
    }
    else
        return SQLITE_ERROR ;

    sqlite3_close( p_from ) ;
    sqlite3_close( p_dest ) ;

    return SQLITE_OK ;
}
