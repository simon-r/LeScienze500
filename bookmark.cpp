//     Copyright 2010 Simone Riva
//     mail:  simone (dot) rva (-at-) gmail.com
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

#include "bookmark.h"

#include "configls500.h"
#include <QFile>
#include <QDir>
#include <QStringList>
#include <QDebug>
#include <QList>
#include <QString>
#include <sqlite3.h>
#include "querydb.h"

Bookmark::Bookmark()
{
}

bool Bookmark::exists()
{
    configLS500 cfg ;

    QString db_path = cfg.getBookmarkPath() ;
    db_path.replace( QRegExp( "(^\\$HOME)" ) , QDir::homePath() ) ;

    QFile file ;
    file.setFileName( db_path );

    return file.exists() ;
}

bool Bookmark::initBookmark()
{
    configLS500 cfg ;

    QString db_path = cfg.getBookmarkPath() ;
    db_path.replace( QRegExp( "(^\\$HOME)" ) , QDir::homePath() ) ;

    QFile file ;
    file.setFileName( db_path );

    bool result , result_b ;

    QFile res ;
    res.setFileName(":/sql/sql/bookmark.sql" );
    res.open(QIODevice::ReadOnly) ;
    QString full_sql = QString::fromUtf8( res.readAll() ) ;
    qDebug() << full_sql ;
    res.close();

    res.setFileName(":/sql/sql/bookmark_init.sql" );
    res.open(QIODevice::ReadOnly) ;
    QString full_init = QString::fromUtf8( res.readAll() ) ;
    res.close();

    if ( !file.exists() )
    {
        result = QueryDB::execNAQuery( db_path , full_sql ) ;
        result_b = QueryDB::execNAQuery( db_path , full_init ) ;

        return result_b ;
    }
    else
        return true ;
}

void Bookmark::getStati( QueryResult& query_r )
{
    query_r.clear();
    QString query = "select Stato from Stato" ;
    this->execQuery( query , query_r ) ;
}

void Bookmark::getValutazioni( QueryResult& query_r )
{
    query_r.clear();
    QString query = "select Valutazione from Valutazioni" ;
    this->execQuery( query , query_r ) ;
}

void  Bookmark::getFavorites( QueryResult& query_r )
{
    query_r.clear();
    QString query = "select IdArticolo from Favoriti" ;
    this->execQuery( query , query_r ) ;
}

void  Bookmark::getCategorie( QueryResult& query_r )
{
    query_r.clear();
    QString query = "select Id, Categoria from Categorie" ;
    this->execQuery( query , query_r ) ;
}

void Bookmark::getCategorie( QueryResult& query_r , const QString& parent )
{
    query_r.clear();

    if ( parent.size() == 0 )
    {
        this->getRootCategorie( query_r );
        return ;
    }

    QString query  = "select Id, Categoria from Categorie where " ;
            query += "Id in ( select IdSottoCategoria from Categoria_SottoCategoria where IdCategoria in " ;
            query += " ( select Id from Categorie where Categoria like \"" ;
            query += parent ;
            query += "\" ) ) " ;
    this->execQuery( query , query_r ) ;
}

void Bookmark::getParentCategoria( QueryResult& query_r , const QString& categoria )
{
    query_r.clear();
    QString query =  "select Id, Categoria from Categorie where " ;
            query += "Id in ( select IdCategoria from Categoria_SottoCategoria where IdSottoCategoria in " ;
            query += " ( select Id from Categorie where Categoria like \"" ;
            query += categoria ;
            query += "\"" ;
            query += " ) ) " ;
    this->execQuery( query , query_r ) ;
}

void Bookmark::getRootCategorie( QueryResult& query_r )
{
    query_r.clear();
    QString query =  "select Id, Categoria, Ordine from Categorie where " ;
            query += "Id not in ( select IdSottoCategoria from Categoria_SottoCategoria ) " ;
    this->execQuery( query , query_r ) ;
}

void Bookmark::getFavoritesByParent( QueryResult& query_r , const QString& parent )
{
    QString query ;

    if ( parent.isEmpty() )
    {
        query = "select * from Favoriti  where Id not in ( select IdFavorito from Categorie_Favoriti where IdCategoria ) " ;
    }
    else
    {
        query =  "select * from Favoriti where " ;
        query += "Id in ( select IdFavorito from Categorie_Favoriti where IdCategoria in " ;
        query += " ( select Id from Categorie where Categoria like \"" ;
        query += parent ;
        query += "\"" ;
        query += " ) ) " ;
    }

    query_r.clear() ;
    this->execQuery( query , query_r ) ;
}

void  Bookmark::getFavoriteFullData( QueryResult& query_r , const QString& id )
{
    query_r.clear() ;
    QString query ;

    query = "select * from Articoli where Id = \"" ;
    query += id ;
    query += "\" " ;

    QueryDB db ;
    query_r = db.execQuery( query ) ;
}

bool Bookmark::folderExist( QString name )
{
    QString query = "select Id from Categorie where Categoria like \"" ;
    query += name ;
    query += "\"  " ;

    QueryResult query_r ;
    this->execQuery( query , query_r ) ;

    if ( query_r.size() > 0 )
        return true ;
    else
        return false ;
}

void Bookmark::addFolder( QString parent , QString name )
{
    if ( parent.isEmpty() || this->folderExist( name ) )
    {
        parent = "Favoriti" ; //////////////////// ATTENZIONE cartella root !!!!!!!!!!!!!!!!!!!!!!!!!
    }

    if ( name.isEmpty() )
    {
        name = "Nuova Cartella" ;
    }


    int cnt = 1 ;
    QString name_cnt = name ;
    QString name_cnt_save ;
    QueryResult query_r ;
    do
    {
        QString query_name = "select * from Categorie where Categoria like \"" ;
        query_name += name_cnt ;
        query_name += "\" " ;

        query_r.clear();
        qDebug() << query_name ;
        this->execQuery( query_name , query_r );

        name_cnt_save = name_cnt ;
        name_cnt = name  ;
        name_cnt += " (" ; name_cnt += QString().setNum( cnt ) ; name_cnt += ")" ;
        cnt++ ;
    }
    while ( query_r.size() > 0 ) ;

    QString query = "insert into Categorie ( Categoria ) values ( \"" ;
    query += name_cnt_save ;
    query += "\" ) " ;

    this->execQuery( query ) ;

    qDebug() << query ;

    QString query_id = "select Id from Categorie where Categoria like \"" ;
    query_id += name_cnt_save ;
    query_id += "\"  " ;

    QString query_id_p = "select Id from Categorie where Categoria like \"" ;
    query_id_p += parent ;
    query_id_p += "\" " ;

    QueryResult query_r_id , query_r_idp ;

    this->execQuery( query_id , query_r_id ) ;
    this->execQuery( query_id_p , query_r_idp ) ;

    QString query_parent = "insert into Categoria_SottoCategoria ( IdCategoria , IdSottoCategoria ) values ( " ;
    query_parent += query_r_idp.getField( "Id" , query_r_idp.begin() ) ;
    query_parent += " , " ;
    query_parent += query_r_id.getField( "Id" , query_r_id.begin() ) ;
    query_parent += " ) " ;

    this->execQuery( query_parent );
    qDebug() <<  query_parent ;
}



void Bookmark::execQuery( QString& query , QueryResult& qr )
{
    configLS500 cfg ;
    QString db_path = cfg.getBookmarkPath() ;

    QueryDB db ;
    db.execQuery( db_path , query , qr ) ;

    qr.printResult();
}

void Bookmark::execQuery( const QString& query )
{
    configLS500 cfg ;
    QString db_path = cfg.getBookmarkPath() ;

    QueryDB db ;
    db.execNAQuery( db_path , query ) ;
}

