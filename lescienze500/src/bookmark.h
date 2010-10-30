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
    bool initBookmarkForce();

    bool autoBackup() ;
    bool backupDatabase( QString file_name = "" ) ;

    bool exists() ;

    void getFavorites( QueryResult& query_r ) ;

    void getFolders( QueryResult& query_r ) ;
    void getFolders( QueryResult& query_r , const QString& parent ) ;
    void getFoldersId( QueryResult& query_r , const QString& parent_id ) ;

    bool folderExistsId( QString parent_id , QString folder_name ) ;

    bool isFavoriteBookmarked( QString id ) ;

    bool isFolderEmpty( QString folder_id ) ;

    void getParentFolder( QueryResult& query_r , const QString& cat ) ;

    void getRootFolders( QueryResult& query_r ) ;

    void getEvaluations( QueryResult& query_r ) ;
    int  setEvaluation( const QString& stars , QString favorite_id ) ;
    bool getFavoritesByEvaluation(  QueryResult& query_r , const QString& stars ) ;
    bool getEvaluation( QueryResult& query_r , QString favorite_id ) ;
    bool deleteEvaluation( QString favorite_id ) ;

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

    bool renameFolder( QString folder_id , QString new_name ) ;

    bool moveFolder( QString folder_id , QString new_parent_id ) ;
    bool moveFavorite( QString favorite_id , QString new_parent_id ) ;

    bool getComment( QueryResult& query_r , QString favorite_id ) ;
    bool setComment( const QString& comment , QString favorite_id ) ;

    bool getState( QueryResult& query_r , QString favorite_id ) ;
    int  setState( const QString& state_name , QString favorite_id ) ;
    bool deleteState( QString favorite_id ) ;
    bool getFavoritesByState(  QueryResult& query_r , const QString& state_name ) ;
    void getStates( QueryResult& query_r ) ;

private:

    void execQuery( QString& query , QueryResult& qr ) ;
    bool execQuery( const QString& query ) ;
};

#endif // BOOKMARK_H
