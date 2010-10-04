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

bool Bookmark::buildDataBase()
{
    configLS500 cfg ;

    QString db_path = cfg.getBookmarkPath() ;

    db_path.replace( QRegExp( "(^\\$HOME)" ) , QDir::homePath() ) ;
//    config_dir.append( "/.config/LeScienze500/" ) ;

    QFile file ;
    file.setFileName( db_path );

    bool result ;

    QFile res ;
    res.setFileName(":/sql/sql/bookmark.sql" );
    res.open(QIODevice::ReadOnly) ;
    QString full_sql = QString ( res.readAll() ) ;
    res.close();

    res.setFileName(":/sql/sql/bookmark_init.sql" );
    res.open(QIODevice::ReadOnly) ;
    QString full_init = QString ( res.readAll() ) ;
    res.close();

    if ( !file.exists() ) {
        result = QueryDB::execNAQuery( db_path , full_sql ) ;
        result = result && QueryDB::execNAQuery( db_path , full_init ) ;

        return result ;
    }
    else
        return true ;
}
