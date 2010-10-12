//     Copyright 2010 Simone Riva
//     mail:  simone (dot) rva (-a-) gmail.com
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

#ifndef BOOKMARK_H
#define BOOKMARK_H

#include <QStringList>
#include <QList>
#include <queryresult.h>
#include <QPair>

class Bookmark
{
public:
    Bookmark();

    bool initBookmark() ;

    bool exists() ;

    void getFavorites( QueryResult& query_r ) ;

    void getFolders( QueryResult& query_r ) ;
    void getFolders( QueryResult& query_r , const QString& parent ) ;
    void getFoldersId( QueryResult& query_r , const QString& parent_id ) ;

    bool folderExistsId( QString parent_id , QString folder_name ) ;

    bool isFolderEmpty( QString folder_id ) ;

    void getParentFolder( QueryResult& query_r , const QString& cat ) ;

    void getRootFolders( QueryResult& query_r ) ;

    void getStati( QueryResult& query_r ) ;
    void getValutazioni( QueryResult& query_r ) ;

    void getFavoritesByParent( QueryResult& query_r , const QString& parent ) ;
    void getFavoritesByParentId( QueryResult& query_r , const QString& parent_id ) ;


    void getFavoriteFullData( QueryResult& query_r , const QString& id ) ;

    QueryResult getFavoritesByStato( QString IdStato ) ;


    QPair<QString,QString> addFolderId( QString parent_id , QString name ) ;

    bool folderExist( QString name ) ;
    bool folderIdExist( QString id ) ;

    QPair<QString,QString> addFavoriteId( QString parent_id , QString id_articolo ) ;

    bool removeFavorite( QString parent_id , QString id ) ;
    bool removeFolder( QString folder_id ) ;

    QueryResult getCommento( QString IdFavorito ) ;
    QueryResult getValutazione( QString IdFavorito ) ;
    QueryResult getStato( QString IdFavorito ) ;
    QueryResult getCategoria( QString IdFavorito ) ;

    // QueryResult getSottoCategorie( QString IdCategoria ) ;
 QString addFavorite( QString parent , QString id ) __attribute__ ((deprecated)) ;
 QPair<QString,QString> addFolder( QString parent , QString name ) __attribute__ ((deprecated)) ;
private:

    void execQuery( QString& query , QueryResult& qr ) ;
    bool execQuery( const QString& query ) ;
};

#endif // BOOKMARK_H
