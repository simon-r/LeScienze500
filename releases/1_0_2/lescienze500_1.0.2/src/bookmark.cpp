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
#include <QTime>
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

    if ( !file.exists() )
    {
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

        result = QueryDB::execNAQuery( db_path , full_sql ) ;
        result_b = QueryDB::execNAQuery( db_path , full_init ) ;

        return result_b ;
    }
    else
        return true ;
}

bool Bookmark::initBookmarkForce()
{
    configLS500 cfg ;

    QString db_path = cfg.getBookmarkPath() ;
    db_path.replace( QRegExp( "(^\\$HOME)" ) , QDir::homePath() ) ;

    QFile file ;
    file.setFileName( db_path );

    if ( file.exists() )
        file.remove() ;

    return this->initBookmark() ;
}

bool Bookmark::autoBackup()
{

    bool ret = true ;
    configLS500 cfg ;

    int cnt = cfg.getBkUpCnt().toInt() ;

    this->cleanBackUpFiles() ;

    if ( ( cnt % 3 ) == 0 )
        ret = this->backupDatabase() ;

    cnt++ ;

    cfg.open();
    cfg.setBkUpCnt( QString().setNum( cnt ) );
    cfg.close();

    return ret ;
}

void Bookmark::cleanBackUpFiles()
{
    configLS500 cfg ;
    QString cfg_dir = cfg.getConfigDir() ;

    QDir dir( cfg_dir ) ;

    QStringList dir_list = dir.entryList ( QDir::NoDotAndDotDot|QDir::NoSymLinks|QDir::Files , QDir::Time ) ;

    QString reg = "^" ;
    reg += cfg.getBookmarkBkUpFile().replace( "." , "\\." ) ;
    reg += "\\..*$" ;
    dir_list = dir_list.filter( QRegExp( reg ) ) ;

//    for ( int jj = 0 ; jj < dir_list.size() ; jj ++  )
//    {
//        qDebug() << dir_list[jj] ;
//    }

    if ( dir_list.size() > 5 )
    {
        dir.remove( dir_list.last() ) ;
    }
}

bool Bookmark::backupDatabase( QString file_name )
{
    configLS500 cfg ;
    QString file_name_bk ;



    if ( file_name.isEmpty() ) {
        QString bk_path = cfg.getBookmarkDumpPath() ;

        bk_path.replace( QRegExp( "(^\\$HOME)" ) , QDir::homePath() ) ;

        bk_path += "." ;
        bk_path += QDate::currentDate().toString( Qt::ISODate ) ;
        bk_path += "-" ;
        bk_path += QTime::currentTime().toString( Qt::ISODate ) ;

        QFile file ;
        int cnt = 1 ;

        file_name_bk = bk_path ;
        file.setFileName( file_name_bk );

        while( file.exists() ) {
            file_name_bk = bk_path + "." + "(" + QString().setNum(cnt++) + ")" ;
            file.setFileName( file_name_bk );
        }
    }

    QueryDB db ;

    int rc = db.backup( cfg.getBookmarkPath() , file_name_bk ) ;

    if ( rc == 0 )
        return true ;
    else
        return false ;
}

void Bookmark::getStates( QueryResult& query_r )
{
    query_r.clear();
    QString query = "select * from UserStates" ;
    this->execQuery( query , query_r ) ;
}

void Bookmark::getEvaluations( QueryResult& query_r )
{
    query_r.clear();
    QString query = "select Evaluation from Evaluations" ;
    this->execQuery( query , query_r ) ;
}


void  Bookmark::getFavorites( QueryResult& query_r )
{
    query_r.clear();
    QString query = "select IdEntry from BookmarkEntries" ;
    this->execQuery( query , query_r ) ;
}

bool Bookmark::isFavoriteBookmarked( QString id )
{
    QString query = "select count(Id) from BookmarkEntries where IdEntry = " ;
    query += id ;

    QueryResult query_r ;

    this->execQuery( query , query_r ) ;

    if ( query_r.getField(0,0).toInt() > 0 )
        return true ;
    else
        return false ;
}

void  Bookmark::getFolders( QueryResult& query_r )
{
    query_r.clear();
    QString query = "select Id, Folder from Folders" ;
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

    QString query  = "select Id, Folder from Folders where " ;
    query += "Id in ( select IdSubFolder from Folder_SubFolders where IdFolder in " ;
    query += " ( select Id from Folders where Folder like \"" ;
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

    QString query  = "select Id, Folder from Folders where " ;
    query += "Id in ( select IdSubFolder from Folder_SubFolders where IdFolder = " ;
    query += parent_id ;
    query += " ) " ;
    this->execQuery( query , query_r ) ;

}

bool Bookmark::folderExistsId( QString parent_id , QString folder_name )
{
    if ( parent_id.isEmpty() )
        parent_id = "1" ;

    QString query  = "select Id from Folders where Id in " ;
    query += "( select IdSubFolder from Folder_SubFolders where IdFolder = " ;
    query += parent_id ;
    query += " and IdSubFolder in ( " ;
    query += " select  Id from Folders where Ctagoria like " ;
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
    QString query =  "select Id, Folder from Folders where " ;
    query += "Id in ( select IdFolder from Folder_SubFolders where IdSubFolder in " ;
    query += " ( select Id from Folders where Folder like \"" ;
    query += categoria ;
    query += "\"" ;
    query += " ) ) " ;
    this->execQuery( query , query_r ) ;
}

void Bookmark::getRootFolders( QueryResult& query_r )
{
    query_r.clear();
    QString query =  "select * from Folders where " ;
    query += "Id not in ( select IdSubFolder from Folder_SubFolders ) " ;
    this->execQuery( query , query_r ) ;
}

void Bookmark::getFavoritesByParentId( QueryResult& query_r , const QString& parent_id )
{
    QString query ;

    if ( parent_id.isEmpty() )
    {
        query = "select * from BookmarkEntries  where Id not in ( select IdBookmarkEntry from Folders_BookmarkEntries ) " ;
    }
    else
    {
        query =  "select * from BookmarkEntries where " ;
        query += "Id in ( select IdBookmarkEntry from Folders_BookmarkEntries where IdFolder = " ;
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
        query = "select * from BookmarkEntries  where Id not in ( select IdBookmarkEntry from Folders_BookmarkEntries ) " ;
    }
    else
    {
        query =  "select * from BookmarkEntries where " ;
        query += "Id in ( select IdBookmarkEntry from Folders_BookmarkEntries where IdFolder in " ;
        query += " ( select Id from Folders where Folder like \"" ;
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
    QString query = "select Id from Folders where Folder like \"" ;
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
    QString query = "select Id from Folders where Id = " ;
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
        QString query_name = "select * from Folders where Folder like \"" ;
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

    QString query = "insert into Folders ( Folder ) values ( \"" ;
    query += name_cnt_save ;
    query += "\" ) " ;

    this->execQuery( query ) ;

    qDebug() << query ;

    QString query_id = "select max(Id) from Folders" ; // where Folder like \"" ;
    // query_id += name_cnt_save ;
    // query_id += "\"  " ;

    QueryResult query_r_id ;

    this->execQuery( query_id , query_r_id ) ;

    QString query_parent = "insert into Folder_SubFolders ( IdFolder , IdSubFolder ) values ( " ;
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

    QString query = "insert into BookmarkEntries ( IdEntry ) values ( " ;
    query += id_articolo ;
    query += " ) " ;

    flag = this->execQuery( query ) ;
    if ( !flag ) qMakePair(QString(""),QString("")) ;

    QString query_ida = "select max ( Id ) from BookmarkEntries where IdEntry = " ;
    query_ida += id_articolo ;

    QueryResult query_r_ida ;
    this->execQuery( query_ida , query_r_ida ) ;

    QString query_tr = "insert into Folders_BookmarkEntries ( IdFolder , IdBookmarkEntry ) values ( " ;
    query_tr += parent_id ;
    query_tr += " , " ;
    query_tr += query_r_ida.getField( "Id" , query_r_ida.begin() ) ;
    query_tr += " ) " ;

    flag = this->execQuery( query_tr ) ;
    if ( !flag )
    {
        QString remove = "delete from BookmarkEntries where Id = " ;
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

    this->deleteEvaluation( id ) ;
    this->deleteState( id ) ;

    QString remove = "delete from BookmarkEntries where Id = " ;
    remove += id ;
    ret = this->execQuery( remove ) ;

    if ( !ret ) return false ;

    QString remove_ff = "delete from Folders_BookmarkEntries  where IdBookmarkEntry = " ;
    remove_ff += id ;

    ret = this->execQuery( remove_ff ) ;



    return ret ;
}

bool Bookmark::isFolderEmpty( QString folder_id )
{
    bool flag = false  ;

    if ( folder_id.isEmpty() )
        return true ;

    QString query =  "select * from Folders where Id = " ;
    query += folder_id ;

    QueryResult q_result ;
    this->execQuery( query , q_result  );

    if ( q_result.empty() )
        return true ;

    query = "select IdFolder from Folder_SubFolders where IdFolder = " ;
    query += folder_id ;

    q_result.clear();
    this->execQuery( query , q_result  ) ;

    flag = false ;
    flag = q_result.empty() ;

    query = "select IdBookmarkEntry from Folders_BookmarkEntries where IdFolder = " ;
    query += folder_id ;

    q_result.clear();
    this->execQuery( query , q_result ) ;

    flag = flag && q_result.empty() ;

    return flag ;
}

bool Bookmark::removeFolder( QString folder_id )
{ 
    if ( !this->isFolderEmpty( folder_id ) ) return false ;
    if ( folder_id.toInt() == 1 ) return false ;

    QString remove = "delete from Folders where Id = " ;
    remove += folder_id ;

    bool flag = this->execQuery( remove ) ;

    remove = "delete from Folder_SubFolders where IdSubFolder = " ;
    remove += folder_id ;

    flag = flag && this->execQuery( remove ) ;

    return flag ;
}

bool Bookmark::renameFolder( QString folder_id , QString new_name )
{
    if ( folder_id.isEmpty() ) return false ;
    //if ( !this->folderExistsId( folder_id ) ) return false ;

    QString rename = "update Folders set Folder = \"" ;
    rename += new_name ;
    rename += "\" " ;
    rename += "where Id = " ;
    rename += folder_id ;

    return this->execQuery( rename ) ;
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


bool Bookmark::moveFolder( QString folder_id , QString new_parent_id )
{
    if ( folder_id.isEmpty() ) return false ;
    if ( new_parent_id.isEmpty() ) return false ;
    if ( folder_id.toInt() == 1 ) return false ;
    if ( !this->folderIdExist( folder_id ) ) return false ;
    if ( !this->folderIdExist( new_parent_id ) ) return false ;

    QString mv_query ;
    mv_query = "update Folder_SubFolders set IdFolder = " ;
    mv_query += new_parent_id ;
    mv_query += " where IdSubFolder = " ;
    mv_query += folder_id ;

    qDebug() <<  mv_query ;

    return this->execQuery( mv_query ) ;
}

bool Bookmark::moveFavorite( QString favorite_id , QString new_parent_id )
{
    if ( favorite_id.isEmpty() ) return false ;
    if ( new_parent_id.isEmpty() ) return false ;
    if ( !this->folderIdExist( new_parent_id ) ) return false ;

    QString mv_query ;
    mv_query = "update Folders_BookmarkEntries set IdFolder = " ;
    mv_query += new_parent_id ;
    mv_query += " where IdBookmarkEntry = " ;
    mv_query += favorite_id ;

    return this->execQuery( mv_query ) ;
}


bool Bookmark::getComment( QueryResult& query_r , QString favorite_id )
{
    if ( favorite_id.isEmpty() ) return false ;

    QString query ;
    query = "select Comment from Comments where Id in ( " ;
    query += " select IdComment from  Comments_BookmarkEntries where " ;
    query += " IdBookmarkEntry = " ;
    query += favorite_id ;
    query += " ) " ;

    this->execQuery( query , query_r ) ;

    if ( query_r.empty() )
        return false ;
    else
        return true ;
}

bool Bookmark::setComment( const QString& comment , QString favorite_id )
{
    if ( favorite_id.isEmpty() ) return false ;
    bool res ;

    QueryResult query_r ;

    QString query ;
    query = "select count(*) from Comments_BookmarkEntries where IdBookmarkEntry = " ;
    query += favorite_id ;

    this->execQuery( query , query_r ) ;

    QString i_comment = comment ;
    i_comment.replace( QRegExp("(\')") , "\'\'" ) ;

    if( query_r.getField(0,0).toInt() > 0 )
    {
        QString update ;
        update = "update Comments set Comment = \'" ;
        update += i_comment ; update += "\' " ;
        update += "where Id in ( select IdComment from Comments_BookmarkEntries where IdBookmarkEntry = " ;
        update += favorite_id ;
        update += " ) " ;

        qDebug() << update ;

        res = this->execQuery( update ) ;
    }
    else
    {
        QString insert ;
        insert = "insert into Comments( Comment ) values ( \'" ;
        insert += i_comment ;
        insert += "\' ) " ;

        res = this->execQuery( insert ) ;

        query_r.clear() ;
        query = " select max(Id) from Comments " ;
        this->execQuery( query , query_r ) ;

        QString max = query_r.getField(0,0) ;

        query_r.clear() ;

        insert = " insert into Comments_BookmarkEntries( IdComment , IdBookmarkEntry ) values  (  " ;
        insert += max ;
        insert += " , " ;
        insert += favorite_id ;
        insert += " ) " ;

        res = res && this->execQuery( insert ) ;
    }

    return res ;
}

bool Bookmark::getState( QueryResult& query_r , QString favorite_id )
{
    if ( favorite_id.isEmpty() ) return false ;

    QString query ;
    query = "select StateName from UserStates where Id in ( " ;
    query += " select IdUserState from  UserStates_BookmarkEntries where " ;
    query += " IdBookmarkEntry = " ;
    query += favorite_id ;
    query += " ) " ;

    this->execQuery( query , query_r ) ;

    if ( query_r.empty() )
        return false ;
    else
        return true ;
}

bool  Bookmark::deleteState( QString favorite_id )
{
    if ( favorite_id.isEmpty() ) return false ;

    QString cancel ;
    cancel = "delete from UserStates_BookmarkEntries where IdBookmarkEntry = " ;
    cancel += favorite_id ;

    qDebug() << cancel ;

    return this->execQuery( cancel ) ;
}

int Bookmark::setState( const QString& state_name , QString favorite_id )
{
    if ( favorite_id.isEmpty() ) return 0 ;
    bool res ;
    int ret ;

    QueryResult query_r ;

    QString query ;
    query = "select count(*) from UserStates_BookmarkEntries where IdBookmarkEntry = " ;
    query += favorite_id ;

    this->execQuery( query , query_r ) ;

    if( query_r.getField(0,0).toInt() > 0 )
    {
        QString update ;
        update = "update UserStates_BookmarkEntries set IdUserState = ( select Id from UserStates where StateName = \"" ;
        update += state_name ;
        update += "\" ) where IdBookmarkEntry = " ;
        update += favorite_id ;

        qDebug() << update ;

        res = this->execQuery( update ) ;
        if ( res )
            ret = 1 ;
        else
            ret = 0 ;
    }
    else
    {
        QString insert ;
        insert = " insert into UserStates_BookmarkEntries ( IdUserState , IdBookmarkEntry ) values " ;
        insert += " ( ( select Id from UserStates where StateName = \"" ;
        insert += state_name ;
        insert += "\" ) , " ;
        insert += favorite_id ;
        insert += " ) " ;

        qDebug() << insert ;
        res = this->execQuery( insert ) ;

        if ( res )
            ret = 2 ;
        else
            ret = 0 ;
    }

    return ret ;
}

bool Bookmark::getFavoritesByState( QueryResult& query_r , const QString& state_name )
{
    if ( state_name.isEmpty() ) return false ;

    QString query ;
    query = " select * from BookmarkEntries where Id in ( select IdBookmarkEntry " ;
    query += " from UserStates_BookmarkEntries where IdUserState in " ;
    query += " ( select Id from UserStates where StateName = \"" ;
    query += state_name ;
    query += "\" ) ) " ;

    this->execQuery( query , query_r ) ;

    qDebug() << query ;

    if ( query_r.empty() )
        return false ;
    else
        return true ;
}


int Bookmark::setEvaluation( const QString& stars , QString favorite_id )
{
    if ( favorite_id.isEmpty() ) return 0 ;
    bool res ;
    int ret = 0 ;

    QueryResult query_r ;

    QString query ;
    query = "select count(*) from Evaluations_BookmarkEntries where IdBookmarkEntry = " ;
    query += favorite_id ;

    this->execQuery( query , query_r ) ;

    if( query_r.getField(0,0).toInt() > 0 )
    {
        QString update ;
        update = "update Evaluations_BookmarkEntries set IdEvaluation = ( select Id from Evaluations where Evaluation = " ;
        update += stars ;
        update += " ) where IdBookmarkEntry = " ;
        update += favorite_id ;

        qDebug() << update ;

        res = this->execQuery( update ) ;
        if ( res )
            ret = 1 ;
        else
            ret = 0 ;
    }
    else
    {
        QString insert ;
        insert = " insert into Evaluations_BookmarkEntries ( IdEvaluation , IdBookmarkEntry ) values " ;
        insert += " ( ( select Id from Evaluations where Evaluation = " ;
        insert += stars ;
        insert += " ) , " ;
        insert += favorite_id ;
        insert += " ) " ;

        res = this->execQuery( insert ) ;

        if ( res )
            ret = 2 ;
        else
            ret = 0 ;
        qDebug() << "insert: " << insert << "ret: " << ret ;
    }

    return ret ;
}

bool Bookmark::getFavoritesByEvaluation(  QueryResult& query_r , const QString& stars )
{
    if ( stars.isEmpty() ) return false ;

    QString query ;
    query = " select * from BookmarkEntries where Id in ( select IdBookmarkEntry " ;
    query += " from Evaluations_BookmarkEntries where IdEvaluation in " ;
    query += " ( select Id from Evaluations where Evaluation = " ;
    query += stars ;
    query += " ) ) " ;

    this->execQuery( query , query_r ) ;

    qDebug() << query ;

    if ( query_r.empty() )
        return false ;
    else
        return true ;
}

bool  Bookmark::getEvaluation( QueryResult& query_r , QString favorite_id )
{
    if ( favorite_id.isEmpty() ) return false ;

    QString query ;
    query = "select Evaluation from Evaluations where Id in ( " ;
    query += " select IdEvaluation from  Evaluations_BookmarkEntries where " ;
    query += " IdBookmarkEntry = " ;
    query += favorite_id ;
    query += " ) " ;

    this->execQuery( query , query_r ) ;

    if ( query_r.empty() )
        return false ;
    else
        return true ;
}


bool Bookmark::deleteEvaluation( QString favorite_id )
{
    if ( favorite_id.isEmpty() ) return false ;

    QString cancel ;
    cancel = "delete from Evaluations_BookmarkEntries where IdBookmarkEntry = " ;
    cancel += favorite_id ;

    qDebug() << cancel ;

    return this->execQuery( cancel ) ;
}
