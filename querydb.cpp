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

QueryResult QueryDB::execMainQuery()
{
    QString query ;

    QString logical_global = " AND " ;

    bool prevq = false ;

    if ( p_chiave )
    {
        QString logical_at = " OR " ;
        QString logical = " AND " ;

        query.append( " ( " ) ;

        if ( parole_chiave.size() > 0 )
        {
            query.append( "( " ) ;
            for ( QStringList::iterator it = parole_chiave.begin() ; it < parole_chiave.end() ; it++ )
            {
                QString first_word , word , last_word ;

                first_word.append( *it ) ; first_word.append( " %" ) ;
                word.append("%") ; word.append(*it) ; word.append("%") ;
                last_word.append("% ") ; last_word.append(*it) ;

                query.append(" ( titolo like \"") ;
                query.append( first_word ) ;
                query.append( "\" or " ) ;

                query.append("titolo like \"") ;
                query.append( word ) ;
                query.append( "\" or " ) ;

                query.append("titolo like \"") ;
                query.append( last_word ) ;
                query.append( "\" ) " ) ;

                if ( it+1 != parole_chiave.end() )
                    query.append( logical ) ;
            }
            query.append( " ) " ) ;
        }
        else
        {
            query.append( "( titolo like null )" ) ;
        }


        query.append( logical_at ) ;

        if ( parole_chiave_abstract.size() > 0 )
        {
            query.append( "( " ) ;
            for ( QStringList::iterator it = parole_chiave_abstract.begin() ; it < parole_chiave_abstract.end() ; it++ )
            {

                QString first_word , word , last_word ;

                first_word.append( *it ) ; first_word.append( " %" ) ;
                word.append("%") ; word.append(*it) ; word.append("%") ;
                last_word.append("% ") ; last_word.append(*it) ;

                query.append(" ( abstract like \"") ;
                query.append( first_word ) ;
                query.append( "\" or " ) ;

                query.append("abstract like \"") ;
                query.append( word ) ;
                query.append( "\" or " ) ;

                query.append("abstract like \"") ;
                query.append( last_word ) ;
                query.append( "\" ) " ) ;

                if ( it+1 != parole_chiave_abstract.end() )
                    query.append( logical ) ;
            }
            query.append( " )" ) ;
        }
        else
        {
             query.append( "( abstract like null )" ) ;
        }
        query.append( " )" ) ;

        prevq = true ;
    }

    //list<string> lista_rubriche = this->getRubriche() ;
    if ( rubriche && lista_rubriche.size() > 0 )
    {
        if ( prevq )
        {
            query.append( logical_global ) ;
            prevq = false ;
        }

        query.append( " ( idrubrica in ( " ) ;

        query.append( " select id from rubriche where " ) ;

        for ( QStringList::iterator it = lista_rubriche.begin() ; it < lista_rubriche.end() ; it++ )
        {
            query.append( " ( idrubrica in (  " ) ;
            query.append( " select id from rubriche where rubrica like \"" );
            query.append( *it ) ;
            query.append( "\" ) ) " ) ;
            if ( it < lista_rubriche.end() - 1 )
            {
                query.append( " OR " ) ;
            }
        }

        query.append( " ) ) " ) ;

        prevq = true ;
    }

    if ( categorie && lista_categorie.size() > 0 )
    {
        if ( prevq )
        {
            query.append( logical_global ) ;
            prevq = false ;
        }

        query.append( " ( articoli.id in ( " ) ;
        query.append( " select idarticolo from articoli_categorie where " ) ;
        for ( QStringList::iterator it = lista_categorie.begin() ; it < lista_categorie.end() ; it++ )
        {
            query.append( " ( idcategoria in (  " ) ;
            query.append( " select id from categorie where categoria like \"" );
            query.append( *it ) ;
            query.append( "\" ) ) " ) ;
            if ( it < lista_categorie.end() - 1 )
            {
                query.append( " OR " ) ;
            }
        }

        query.append( " ) ) " ) ;
        prevq = true ;
    }



    if ( autori_l && lista_autori.size() > 0 )
    {
        if ( prevq )
        {
            query.append( logical_global ) ;
            prevq = false ;
        }

        query.append( " ( articoli.id in ( " ) ;
        query.append( " select idarticolo from articoli_autori where " ) ;

        for ( QStringList::iterator it = lista_autori.begin() ; it < lista_autori.end() ; it++ )
        {
            query.append( " ( idautore in ( " ) ;
            query.append( " select id from autori where Autore like \"" ) ;
            query.append( *it ) ;
            query.append( "\" ) ) " ) ;
            if ( it < lista_autori.end() - 1 )
            {
                query.append( " OR " ) ;
            }
        }

        query.append( " ) ) " ) ;
        prevq = true ;
    }

    if ( anno && lista_anni.size() > 0 )
    {
        if ( prevq )
        {
            query.append( logical_global ) ;
            prevq = false ;
        }

        typedef QPair<int,int> yearsInterval ;
        QList<yearsInterval> years ;

        int a1 , a2 ;

        for ( QStringList::iterator it = lista_anni.begin() ; it < lista_anni.end() ; it++ )
        {

        }

        prevq = true ;
    }

    query.append( " ORDER BY idrivista" ) ;

    //query.prepend( "SELECT titolo,idRivista,id FROM articoli WHERE " ) ;
    query.prepend( "SELECT titolo,numero,articoli.id FROM articoli, riviste WHERE articoli.idrivista = riviste.id and " ) ;

    qDebug() << query  ;

    QueryResult q_result = execQuery( query ) ;
    q_result.printResult() ;

    return q_result ;
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
