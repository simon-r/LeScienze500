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

list<string> QueryDB::getCategorie()
{
    list<string> lista_categorie ;

    string query = "select Categoria from Categorie" ;

    rc = sqlite3_open( dbPath.toAscii().data() , &db);
    if( rc ){
        return lista_categorie;
    }

    rc = sqlite3_prepare_v2( db, query.c_str() , -1, &stmt, 0);
    if( rc ){
        return lista_categorie;
    }

    cols = sqlite3_column_count(stmt);
    // execute the statement
    do{
        rc = sqlite3_step(stmt);
        switch( rc ){
        case SQLITE_DONE:
            break;
        case SQLITE_ROW:
            // print results for this row
            for( col=0; col<cols; col++){
                lista_categorie.push_back( string( (const char*)sqlite3_column_text( stmt, col ) ) );
                //printf("%s = %s\n", sqlite3_column_name(stmt, col), txt ? txt : "NULL" );
            }
            break;
        default:
            //fprintf(stderr, "Error: %d : %s\n",  rc, sqlite3_errmsg(db));
            break;
        }
        //cout << "========================================================================" << "\n" ;
    }while( rc==SQLITE_ROW );
    // finalize the statement to release resources
    sqlite3_finalize(stmt);

    rc = 0 ;
    return lista_categorie ;
}

list<string> QueryDB::getAutori( string filtro )
{
    list<string> lista_autori ;

    string query = "select Autore from Autori where Autore like " ;
    query.append("'%");
    query.append( filtro );
    query.append("%'");
    query.append(" order by Autore");

    rc = sqlite3_open( dbPath.toAscii().data() , &db);
    if( rc ){
        return lista_autori;
    }

    rc = sqlite3_prepare_v2( db, query.c_str() , -1, &stmt, 0);
    if( rc ){
        return lista_autori;
    }

    cols = sqlite3_column_count(stmt);
    // execute the statement
    do{
        rc = sqlite3_step(stmt);
        switch( rc ){
        case SQLITE_DONE:
            break;
        case SQLITE_ROW:
            // print results for this row
            for( col=0; col<cols; col++){
                lista_autori.push_back( string( (const char*)sqlite3_column_text( stmt, col ) ) );
                //printf("%s = %s\n", sqlite3_column_name(stmt, col), txt ? txt : "NULL" );
            }
            break;
        default:
            //fprintf(stderr, "Error: %d : %s\n",  rc, sqlite3_errmsg(db));
            break;
        }
        //cout << "========================================================================" << "\n" ;
    }while( rc==SQLITE_ROW );
    // finalize the statement to release resources
    sqlite3_finalize(stmt);

    rc = 0 ;
    return lista_autori ;
}

list<int> QueryDB::getAnni()
{
    list<int> lista_anni ;

    string query = "select anno from Riviste group by anno" ;

    rc = sqlite3_open( dbPath.toAscii().data() , &db);
    if( rc ){
        return lista_anni;
    }

    rc = sqlite3_prepare_v2( db, query.c_str() , -1, &stmt, 0);
    if( rc ){
        return lista_anni;
    }

    cols = sqlite3_column_count(stmt);
    // execute the statement
    do{
        rc = sqlite3_step(stmt);
        switch( rc ){
        case SQLITE_DONE:
            break;
        case SQLITE_ROW:
            // print results for this row
            for( col=0; col<cols; col++){
                lista_anni.push_back( sqlite3_column_int( stmt, col ) );
                //printf("%s = %s\n", sqlite3_column_name(stmt, col), txt ? txt : "NULL" );
            }
            break;
        default:
            //fprintf(stderr, "Error: %d : %s\n",  rc, sqlite3_errmsg(db));
            break;
        }
    } while( rc==SQLITE_ROW );
    // finalize the statement to release resources
    sqlite3_finalize(stmt);

    rc = 0 ;
    return lista_anni ;
}

list<string> QueryDB::getRubriche()
{
    list<string> lista_rubriche ;

    string query = "select rubrica from rubriche" ;

    rc = sqlite3_open( dbPath.toAscii().data() , &db);
    if( rc ){
        return lista_rubriche;
    }

    rc = sqlite3_prepare_v2( db, query.c_str() , -1, &stmt, 0);
    if( rc ){
        return lista_rubriche;
    }

    cols = sqlite3_column_count(stmt);
    // execute the statement
    do{
        rc = sqlite3_step(stmt);
        switch( rc ){
        case SQLITE_DONE:
            break;
        case SQLITE_ROW:
            // print results for this row
            for( col=0; col<cols; col++){
                lista_rubriche.push_back( string( (const char*)sqlite3_column_text( stmt, col ) ) );
                //printf("%s = %s\n", sqlite3_column_name(stmt, col), txt ? txt : "NULL" );
            }
            break;
        default:
            //fprintf(stderr, "Error: %d : %s\n",  rc, sqlite3_errmsg(db));
            break;
        }
    }while( rc==SQLITE_ROW );
    // finalize the statement to release resources
    sqlite3_finalize(stmt);

    rc = 0 ;
    return lista_rubriche ;
}

bool StrIntLessThan(const QString &s1, const QString &s2)
{
    return s1.toInt() < s2.toInt() ;
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
        q_result.appendColumnName( QString( (const char*)sqlite3_column_name( stmt, col ) ) );
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
                row.append( QString( (const char*)sqlite3_column_text( stmt, col ) ) );
                //printf("%s = %s\n", sqlite3_column_name(stmt, col), txt ? txt : "NULL" );
                //qDebug() << QString( (const char*)sqlite3_column_text( stmt, col ) ) ;
            }
            q_result.appendResultRow( row ) ;
            break;
        default:
            //fprintf(stderr, "Error: %d : %s\n",  rc, sqlite3_errmsg(db));
            break;
        }
    }while( rc==SQLITE_ROW );



    // finalize the statement to release resources
    sqlite3_finalize(stmt);

    rc = 0 ;
    return q_result ;
}
