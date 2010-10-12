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

void  Bookmark::getFolders( QueryResult& query_r )
{
    query_r.clear();
    QString query = "select Id, Categoria from Categorie" ;
    this->execQuery( query , query_r ) ;
}

void Bookmark::getFolders( QueryResult& query_r , const QString& parent )
{
    query_r.clear();

    if ( parent.size() == 0 )
    {
        this->getRootFolders( query_r );
        return ;
    }

    QString query  = "select Id, Categoria from Categorie where " ;
            query += "Id in ( select IdSottoCategoria from Categoria_SottoCategoria where IdCategoria in " ;
            query += " ( select Id from Categorie where Categoria like \"" ;
            query += parent ;
            query += "\" ) ) " ;
    this->execQuery( query , query_r ) ;
}

void Bookmark::getFoldersId( QueryResult& query_r , const QString& parent_id )
{
    query_r.clear();

    if ( parent_id.size() == 0 )
    {
        this->getRootFolders( query_r );
        return ;
    }

    QString query  = "select Id, Categoria from Categorie where " ;
            query += "Id in ( select IdSottoCategoria from Categoria_SottoCategoria where IdCategoria = " ;
            query += parent_id ;
            query += " ) " ;
    this->execQuery( query , query_r ) ;

}

bool Bookmark::folderExistsId( QString parent_id , QString folder_name )
{
    if ( parent_id.isEmpty() )
        parent_id = "1" ;

    QString query  = "select Id from Categorie where Id in " ;
            query += "( select IdSottoCategoria from Categoria_SottoCategoria where IdCategoria = " ;
            query += parent_id ;
            query += " and IdSottocategoria in ( " ;
            query += " select  Id from Categorie where Ctagoria like " ;
            query += folder_name ;
            query += " ) ) " ;

    QueryResult query_r ;
    this->execQuery( query , query_r ) ;

    if ( query_r.empty() )
        return false ;
    else
        return true ;
}

void Bookmark::getParentFolder( QueryResult& query_r , const QString& categoria )
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

void Bookmark::getRootFolders( QueryResult& query_r )
{
    query_r.clear();
    QString query =  "select Id, Categoria, Ordine from Categorie where " ;
            query += "Id not in ( select IdSottoCategoria from Categoria_SottoCategoria ) " ;
    this->execQuery( query , query_r ) ;
}

void Bookmark::getFavoritesByParentId( QueryResult& query_r , const QString& parent_id )
{
    QString query ;

    if ( parent_id.isEmpty() )
    {
        query = "select * from Favoriti  where Id not in ( select IdFavorito from Categorie_Favoriti ) " ;
    }
    else
    {
        query =  "select * from Favoriti where " ;
        query += "Id in ( select IdFavorito from Categorie_Favoriti where IdCategoria = " ;
        query += parent_id ;
        query += " ) " ;
    }

    query_r.clear() ;
    this->execQuery( query , query_r ) ;
}

void Bookmark::getFavoritesByParent( QueryResult& query_r , const QString& parent )
{
    QString query ;

    if ( parent.isEmpty() )
    {
        query = "select * from Favoriti  where Id not in ( select IdFavorito from Categorie_Favoriti ) " ;
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

bool Bookmark::folderIdExist( QString id )
{
    QString query = "select Id from Categorie where Id = " ;
    query += id ;
    query += " " ;

    QueryResult query_r ;
    this->execQuery( query , query_r ) ;

    if ( query_r.size() > 0 )
        return true ;
    else
        return false ;
}


QPair<QString,QString> Bookmark::addFolderId( QString parent_id , QString name )
{
    QPair<QString,QString> ret ;
    if ( parent_id.isEmpty() || !this->folderIdExist( parent_id ) )
    {
        QueryResult query_root ;
        getRootFolders( query_root ) ;
        if ( query_root.empty() ) return ret ;
        parent_id = query_root.getField( "Id" , query_root.begin() ) ;
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

   QueryResult query_r_id ;
   this->execQuery( query_id , query_r_id ) ;

   QString query_parent = "insert into Categoria_SottoCategoria ( IdCategoria , IdSottoCategoria ) values ( " ;
   query_parent += parent_id ;
   query_parent += " , " ;
   query_parent += query_r_id.getField( "Id" , query_r_id.begin() ) ;
   query_parent += " ) " ;

   this->execQuery( query_parent );
   qDebug() <<  query_parent ;

   ret.first = query_r_id.getField( "Id" , query_r_id.begin() ) ;
   ret.second = name_cnt_save ;
   return ret ;
}

QPair<QString,QString> Bookmark::addFavoriteId( QString parent_id , QString id_articolo )
{
    bool flag ;

    if ( parent_id.isEmpty() || !this->folderIdExist( parent_id ) )
    {
        QueryResult query_root ;
        getRootFolders( query_root ) ;
        if ( query_root.empty() )
            return qMakePair(QString(""),QString("")) ;
        parent_id = query_root.getField( "Id" , query_root.begin() ) ;
    }

    QueryResult query_r ;
    getFavoriteFullData( query_r , id_articolo ) ;

    if ( query_r.empty() ) return qMakePair(QString(""),QString("")) ;

    QString query = "insert into Favoriti ( IdArticolo ) values ( " ;
    query += id_articolo ;
    query += " ) " ;

    flag = this->execQuery( query ) ;
    if ( !flag ) qMakePair(QString(""),QString("")) ;

    QString query_ida = "select max ( Id ) from Favoriti where IdArticolo = " ;
    query_ida += id_articolo ;

    QueryResult query_r_ida ;
    this->execQuery( query_ida , query_r_ida ) ;

    QString query_tr = "insert into Categorie_Favoriti ( IdCategoria , IdFavorito ) values ( " ;
    query_tr += parent_id ;
    query_tr += " , " ;
    query_tr += query_r_ida.getField( "Id" , query_r_ida.begin() ) ;
    query_tr += " ) " ;

    flag = this->execQuery( query_tr ) ;
    if ( !flag )
    {
        QString remove = "delete from Favoriti where Id = " ;
        remove += query_r_ida.getField( "Id" , query_r_ida.begin() ) ;
        this->execQuery( remove ) ;
        return qMakePair(QString(""),QString("")) ;
    }

    QPair<QString,QString> ret ;

    ret.first = query_r_ida.getField( "Id" , query_r_ida.begin() ) ;
    ret.second = query_r.getField( "Titolo" , query_r.begin() ) ;

    return ret ;
}


bool  Bookmark::removeFavorite( QString parent_id , QString id )
{
    bool ret ;

    if ( parent_id.isEmpty() || !this->folderIdExist( parent_id ) )
    {
      return false ;
    }

    QString remove = "delete from Favoriti where Id = " ;
    remove += id ;
    ret = this->execQuery( remove ) ;

    if ( !ret ) return false ;

    QString remove_ff = "delete from Categorie_Favoriti  where IdFavorito = " ;
    remove_ff += id ;

    ret = this->execQuery( remove_ff ) ;

    return ret ;
}

bool Bookmark::isFolderEmpty( QString folder_id )
{

    bool flag = false  ;

    if ( folder_id.isEmpty() )
     return true ;

    QString query =  "select * from Categorie where Id = " ;
    query += folder_id ;

    QueryResult q_result ;
    this->execQuery( query , q_result  );

    if ( q_result.empty() )
        return true ;

    query = "select IdCategoria from Categoria_SottoCategoria where IdCategoria = " ;
    query += folder_id ;

    q_result.clear();
    this->execQuery( query , q_result  ) ;

    flag = false ;
    if ( q_result.empty() )
        flag = true ;

    query = "select IdFavorito from Categorie_Favoriti where IdCategoria = " ;
    query += folder_id ;

    q_result.clear();
    this->execQuery( query , q_result ) ;

    if ( q_result.empty() )
        flag = flag && true ;

    return flag ;
}

bool Bookmark::removeFolder( QString folder_id )
{ 
    if ( !this->isFolderEmpty( folder_id ) ) return false ;

   QString remove = "delete from Categorie where Id = " ;
   remove += folder_id ;

   return this->execQuery( remove ) ;
}

void Bookmark::execQuery( QString& query , QueryResult& qr )
{
    configLS500 cfg ;
    QString db_path = cfg.getBookmarkPath() ;

    QueryDB db ;
    db.execQuery( db_path , query , qr ) ;

    //qr.printResult();
}

bool Bookmark::execQuery( const QString& query )
{
    configLS500 cfg ;
    QString db_path = cfg.getBookmarkPath() ;

    QueryDB db ;
    return db.execNAQuery( db_path , query ) ;
}








///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////





QString Bookmark::addFavorite( QString parent , QString id )
{
    if ( parent.isEmpty() || !this->folderExist( parent ) )
    {
        QueryResult query_root ;
        getRootFolders( query_root ) ;
        if ( query_root.empty() ) return QString( "" ) ;
        parent = query_root.getField( "Categoria" , query_root.begin() ) ;
    }

    QueryResult query_r ;
    getFavoriteFullData( query_r , id ) ;

    if ( query_r.empty() ) return QString( "" ) ;

    QString query = "insert into Favoriti ( IdArticolo ) values ( " ;
    query += id ;
    query += " ) " ;
    this->execQuery( query ) ;

    QString query_ida = "select Id from Favoriti where IdArticolo = " ;
    query_ida += id ;

    QueryResult query_r_ida ;
    this->execQuery( query_ida , query_r_ida ) ;

    QString query_id_p = "select Id from Categorie where Categoria like \"" ;
    query_id_p += parent ;
    query_id_p += "\" " ;

    QueryResult query_r_idp ;
    this->execQuery( query_id_p , query_r_idp ) ;

    QString query_tr = "insert into Categorie_Favoriti ( IdCategoria , IdFavorito ) values ( " ;
    query_tr += query_r_idp.getField( "Id" , query_r_idp.begin() ) ;
    query_tr += " , " ;
    query_tr += query_r_ida.getField( "Id" , query_r_ida.begin() ) ;
    query_tr += " ) " ;

    qDebug() << query_tr ;


    this->execQuery( query_tr ) ;

    return query_r.getField( "Titolo" , query_r.begin() ) ;
}



QPair<QString,QString> Bookmark::addFolder( QString parent , QString name )
{
    if ( parent.isEmpty() || !this->folderExist( name ) )
    {
        QueryResult query_root ;
        getRootFolders( query_root ) ;
        if ( query_root.empty() ) {
            QPair<QString,QString> r ;
            return r ;
        }
        parent = query_root.getField( "Categoria" , query_root.begin() ) ;
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

    QPair<QString,QString> res ;
    res.first = query_r_idp.getField( "Id" , query_r_idp.begin() ) ;
    res.second = name_cnt_save ;

    return res ;
}
