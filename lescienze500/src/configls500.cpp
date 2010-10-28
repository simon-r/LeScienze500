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


#include "configls500.h"
#include <QFile>
#include <QDir>
#include <QStringList>
#include <QDebug>
#include <QList>
#include "previewarticolo.h"
#include <QRegExp>


configLS500::configLS500()
{
    this->is_open = false ;
    initConfig() ;
}

void configLS500::initConfig()
{
    this->config_dir = QDir::homePath() ;
    this->config_dir.append( "/.config/LeScienze500/" ) ;

    this->config_path = this->config_dir ;
    this->config_path.append( "le_scienze500.cfg" ) ;

    QDir dir ;
    dir.setPath( this->config_dir );
    if ( !dir.exists() )
        dir.mkpath( this->config_dir ) ;

    QFile file ;
    file.setFileName( this->config_path );

    if ( !file.exists() ) {
        writeDfaultFile() ;
    }
}

void configLS500::resetToDefault()
{

}

void configLS500::writeDfaultFile()
{
     this->open() ;

     setDBPath( DB_DIR_V ) ;
     setPDFPath1( ART_PATH_1_V ) ;
     setPDFPath2( ART_PATH_2_V ) ;
     setPDFAppl( PDF_APPL_V ) ;
     setCopertinePath( COPERTINE_P_V ) ;
     setDVD( USE_DVD_V ) ;
     setBookmarkPath( BOOKMARK_PATH_V ) ;
     setBookmarkDumpPath( BOOKMARK_DB_DUMP_PATH_V ) ;
     setBookmarkBkUpFile( BOOKMARK_DB_BKUP_FILE_V ) ;

     this->close() ;
}

QString configLS500::getDBPath()
{
    return getConfigParameter( DB_DIR ) ;
}

QString configLS500::getPDFPath1()
{
    return getConfigParameter( ART_PATH_1 ) ;
}

QString configLS500::getPDFPath2()
{
    return getConfigParameter( ART_PATH_2 ) ;
}

QString configLS500::getCopertinePath()
{
    return getConfigParameter( COPERTINE_P ) ;
}

QString configLS500::getPDFAppl()
{
    return getConfigParameter( PDF_APPL ) ;
}

 QString configLS500::getBookmarkPath()
 {
     QString bk = getConfigParameter( BOOKMARK_PATH ) ;
     QString res ;

     if ( bk.isEmpty() )
     {
         res = QString( BOOKMARK_PATH_V ) ;
         open() ;
         this->setBookmarkPath( BOOKMARK_PATH_V );
         close() ;
     }
     else
         res = bk ;

      res.replace( QRegExp( "(^\\$HOME)" ) , QDir::homePath() ) ;
      return res ;
 }

QString configLS500::getDVD()
{
    QString dvd = getConfigParameter( USE_DVD ) ;
    QString res ;

    if ( dvd.isEmpty() )
        res = QString( USE_DVD_V ) ;
    else
        res = dvd ;
    return res ;
}

QString configLS500::getBookmarkDumpPath()
{
    QString dump = getConfigParameter( BOOKMARK_DB_DUMP_PATH ) ;
    QString res ;

    if ( dump.isEmpty() )
    {
        res = QString( BOOKMARK_DB_DUMP_PATH_V ) ;
        open() ;
        this->setBookmarkDumpPath( res );
        close() ;
    }
    else
        res = dump ;
    return res ;
}

QString configLS500::getBookmarkBkUpFile()
{
    QString dump = getConfigParameter( BOOKMARK_DB_BKUP_FILE ) ;
    QString res ;

    if ( dump.isEmpty() )
    {
        res = QString( BOOKMARK_DB_BKUP_FILE_V ) ;
        open() ;
        this->setBookmarkBkUpFile( res );
        close() ;
    }
    else
        res = dump ;

    return res ;
}

QString configLS500::getConfigParameter( QString name )
{
    QString result ;

    QFile file ;
    file.setFileName( this->config_path );
    file.open(  QIODevice::ReadOnly ) ;

    while( !file.atEnd() ) {
        QString line = file.readLine() ;
        if ( line.size() > 0 ) {
            QStringList list = line.split( SPACING , QString::SkipEmptyParts );
            if ( list.size() == 2 )
            {
                if (list[0] == name )
                    result = list[1] ;
            }
        }
    } ;

    file.close() ;

    int rs = result.size() ;

    if ( rs == 0 )
        return result ;

    result = result.remove( rs-1 , 1 ) ;
    
    return result ;
}

void configLS500::getAllParameters()
{
    char buf[1024];
    QString result ;

    QFile file ;

    parameters.clear() ;

    file.setFileName( this->config_path );
    file.open(  QIODevice::ReadOnly ) ;
    qint64 lineLength ;
    do{
        lineLength = file.readLine(buf, sizeof(buf)) ;
        if (lineLength != -1) {
            QString line = QString ( buf ) ;
            QStringList list = line.split( SPACING , QString::SkipEmptyParts );
            if ( list.size() == 2 )
            {
                int ls = list[1].size() ;
                list[1] = list[1].remove( ls-1 , 1 ) ;
                parameters.insert( list[0] , list[1] ) ;
            }
        }

    } while ( lineLength != -1 ) ;

    file.close() ;
}

void configLS500::writeAllParameters()
{
    QFile file ;
    file.setFileName( this->config_path );
    file.open(  QIODevice::WriteOnly ) ;

    QList<QString> keys = parameters.keys() ;

    for( QList<QString>::iterator it = keys.begin() ; it < keys.end() ; it++ )
    {
        QString line ;

        line.append( *it ) ; line.append( SPACING ) ; line.append( parameters[*it] ) ; line.append( ENDL ) ;
        file.write( line.toAscii().data() ) ;
    }

    file.close() ;
}


void configLS500::open()
{
    if ( !is_open )
    {
        parameters.clear() ;
        getAllParameters() ;
        is_open = true ;
    }
}

void configLS500::setDBPath( QString pr )
{
    parameters.insert( DB_DIR , pr ) ;
}

void configLS500::setPDFPath1( QString pr )
{
    parameters.insert( ART_PATH_1 , pr ) ;
}

void configLS500::setPDFPath2( QString pr )
{
    parameters.insert( ART_PATH_2 , pr ) ;
}

void configLS500::setPDFAppl( QString pr )
{
    parameters.insert( PDF_APPL , pr ) ;
}

void configLS500::setCopertinePath( QString pr )
{
    parameters.insert( COPERTINE_P , pr ) ;
}

void configLS500::setDVD( QString pr )
{
    parameters.insert( USE_DVD , pr ) ;
}

void configLS500::setBookmarkDumpPath( QString pr )
 {
     parameters.insert( BOOKMARK_DB_DUMP_PATH , pr ) ;
 }

void configLS500::setBookmarkBkUpFile( QString pr )
{
    parameters.insert( BOOKMARK_DB_BKUP_FILE , pr ) ;
}

 void configLS500::setBookmarkPath( QString pr )
 {
     parameters.insert( BOOKMARK_PATH , pr ) ;
 }

void configLS500::close()
{
    if ( is_open )
    {
        writeAllParameters() ;
        parameters.clear() ;
        is_open = false ;
    }
    else
    {
        parameters.clear() ;
    }
}
