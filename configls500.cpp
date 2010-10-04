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


configLS500::configLS500()
{
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
        writeDfaultFile( &file ) ;
    }
}

void configLS500::resetToDefault()
{

}

void configLS500::writeDfaultFile( QFile* file )
{

    QString db_dir ;
    QString pdf_appl ;
    QString art_pdf_path1 ;
    QString art_pdf_path2 ;
    QString copertine_path ;
    QString dvd ;
    //QString bk ;

    db_dir.append(DB_DIR) ; db_dir.append(SPACING) ; db_dir.append(DB_DIR_V) ; db_dir.append(ENDL) ;
    pdf_appl.append(PDF_APPL) ; pdf_appl.append(SPACING) ; pdf_appl.append(PDF_APPL_V) ; pdf_appl.append(ENDL) ;
    art_pdf_path1.append(ART_PATH_1) ; art_pdf_path1.append(SPACING) ; art_pdf_path1.append(ART_PATH_1_V) ; art_pdf_path1.append(ENDL) ;
    art_pdf_path2.append(ART_PATH_2) ; art_pdf_path2.append(SPACING) ; art_pdf_path2.append(ART_PATH_2_V) ; art_pdf_path2.append(ENDL) ;
    copertine_path.append(COPERTINE_P) ; copertine_path.append(SPACING) ; copertine_path.append(COPERTINE_P_V) ; copertine_path.append(ENDL) ;
    dvd.append(USE_DVD) ; dvd.append(SPACING) ; dvd.append(USE_DVD_V) ; dvd.append(ENDL) ;

    file->open(  QIODevice::ReadWrite ) ;
    file->write( db_dir.toAscii().data() ) ;
    file->write( pdf_appl.toAscii().data() ) ;
    file->write( art_pdf_path1.toAscii().data() ) ;
    file->write( art_pdf_path2.toAscii().data() ) ;
    file->write( copertine_path.toAscii().data() ) ;
    file->write( dvd.toAscii().data() ) ;
    file->close();
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
     if ( bk.isEmpty() )
         return QString( BOOKMARK_PATH_V ) ;
     else
         return bk ;
 }

QString configLS500::getDVD()
{
    QString dvd = getConfigParameter( USE_DVD ) ;

    if ( dvd.isEmpty() )
        return QString( USE_DVD_V ) ;
    else
        return dvd ;
}


QString configLS500::getConfigParameter( QString name )
{
    char buf[1024];
    QString result ;

    QFile file ;
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
                if (list[0] == name )
                    result = list[1] ;
            }
        }

    } while ( lineLength != -1 ) ;

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
    getAllParameters() ;
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

void configLS500::close()
{
    writeAllParameters() ;
    parameters.clear() ;
}
