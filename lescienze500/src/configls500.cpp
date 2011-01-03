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

QString configLS500::PARAMETER( parName pn )
{
    switch ( pn )
    {
        //    case Spacing:
        //        return "space" ;
    case  PdfAppl:
        return  "pdf_appl";
    case DbDir:
        return "db_dir" ;
    case ArtPath1:
        return "art_pdf_path1" ;
    case ArtPath2:
        return "art_pdf_path2" ;
    case CopertinePath:
        return "copertine_path" ;
    case UseDvd:
        return "dvd" ;
    case BookmarkPath:
        return "bookmark" ;
    case BookmarkBkupPath:
        return "bk_db_dump" ;
    case BookmarkBkupFile:
        return "bk_db_dump_file" ;
    case BookmarkBkupCnt:
        return "bk_bkup_cnt" ;
    case DictionaryPath:
        return "dict_path" ;
    default:
        break ;
    }
    return "invalid" ;
}

#ifdef Q_WS_X11
QString configLS500::DEFAULT( parName pn )
{
    switch ( pn )
    {
        //     case Spacing:
        //         return "\\t" ;
    case  PdfAppl:
        return  "gv";
    case DbDir:
        return "/opt/LeScienze500/LeScienze.db" ;
    case ArtPath1:
        return "/media/LESCIENZE/articoli/" ;
    case ArtPath2:
        return "/media/LESCIENZE/articoli/" ;
    case CopertinePath:
        return "/opt/LeScienze500/copertine/" ;
    case  UseDvd:
        return "no" ;
    case BookmarkPath:
        return "$HOME/.config/LeScienze500/bookmark_ls500.db" ;
    case BookmarkBkupPath:
        return "$HOME/.config/LeScienze500/bookmark_ls500.backup" ;
    case BookmarkBkupFile:
        return "bookmark_ls500.backup" ;
    case BookmarkBkupCnt:
        return "0" ;
    case DictionaryPath:
        return "$HOME/.config/LeScienze500/dictionary_ls500.db" ;
    default:
        break ;
    }
    return "undef" ;
}
#endif


#ifdef Q_WS_WIN
QString configLS500::DEFAULT( parName pn )
{
    switch ( pn )
    {
        //     case Spacing:
        //         return "\\t" ;
    case  PdfAppl:
        return "desktop";
    case DbDir:
        return "C:/Programmi/Le Scienze - 500 Numeri/data/LeScienze.db" ;
    case ArtPath1:
        return "C:/LESCIENZE/articoli 1" ;
    case ArtPath2:
        return "C:/LESCIENZE/articoli 2" ;
    case CopertinePath:
        return "C:/Programmi/Le Scienze - 500 Numeri/copertine" ;
    case  UseDvd:
        return "no" ;
    case BookmarkPath:
        return "$HOME/AppData/LeScienze500/bookmark_ls500.db" ;
    case BookmarkBkupPath:
        return "$HOME/AppData/LeScienze500/bookmark_ls500.backup" ;
    case BookmarkBkupFile:
        return "bookmark_ls500.backup" ;
    case BookmarkBkupCnt:
        return "0" ;
    case DictionaryPath:
        return "$HOME/AppData/LeScienze500/dictionary_ls500.db" ;
    default:
        break ;
    }
    return "undef" ;
}
#endif


configLS500::configLS500()
{
    this->is_open = false ;
    initConfig() ;
}

void configLS500::initConfig()
{
    this->config_dir = QDir::homePath() ;

#ifdef Q_WS_X11
    this->config_dir.append( "/.config/LeScienze500" ) ;
#endif

#ifdef Q_WS_WIN
    this->config_dir.append( "/AppData/LeScienze500" ) ;
#endif

    this->config_path = this->config_dir ;
    this->config_path.append( "/" ) ;
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

    setDBPath( configLS500::DEFAULT( DbDir ) ) ;
    setPDFPath1( configLS500::DEFAULT( ArtPath1 ) ) ;
    setPDFPath2( configLS500::DEFAULT( ArtPath2 ) ) ;
    setPDFAppl( configLS500::DEFAULT( PdfAppl ) ) ;
    setCopertinePath( configLS500::DEFAULT( CopertinePath ) ) ;
    setDVD( configLS500::DEFAULT( UseDvd ) ) ;
    setBookmarkPath( configLS500::DEFAULT( BookmarkPath ) ) ;
    setBookmarkDumpPath( configLS500::DEFAULT( BookmarkBkupPath ) ) ;
    setBookmarkBkUpFile( configLS500::DEFAULT( BookmarkBkupFile ) ) ;
    setBkUpCnt( configLS500::DEFAULT( BookmarkBkupCnt ) ) ;

    this->close() ;
}

QString configLS500::getDBPath()
{
    return getConfigParameter( configLS500::PARAMETER( DbDir ) ) ;
}

QString configLS500::getPDFPath1()
{
    return getConfigParameter( configLS500::PARAMETER( ArtPath1 ) ) ;
}

QString configLS500::getPDFPath2()
{
    return getConfigParameter( configLS500::PARAMETER( ArtPath2 ) ) ;
}

QString configLS500::getCopertinePath()
{
    return getConfigParameter( configLS500::PARAMETER( CopertinePath ) ) ;
}

QString configLS500::getPDFAppl()
{
    return getConfigParameter( configLS500::PARAMETER( PdfAppl ) ) ;
}

QString configLS500::getBookmarkPath()
{
    QString bk = getConfigParameter( configLS500::PARAMETER( BookmarkPath ) ) ;
    QString res ;

    if ( bk.isEmpty() )
    {
        res = QString( configLS500::DEFAULT( BookmarkPath ) ) ;
        open() ;
        this->setBookmarkPath( configLS500::DEFAULT( BookmarkPath ) );
        close() ;
    }
    else
        res = bk ;

    res.replace( QRegExp( "(^\\$HOME)" ) , QDir::homePath() ) ;
    return res ;
}

QString configLS500::getDVD()
{
    QString dvd = getConfigParameter( configLS500::PARAMETER( UseDvd ) ) ;
    QString res ;

    if ( dvd.isEmpty() )
        res = QString( configLS500::DEFAULT( UseDvd ) ) ;
    else
        res = dvd ;
    return res ;
}

QString configLS500::getBookmarkDumpPath()
{
    QString dump = getConfigParameter( configLS500::PARAMETER( BookmarkBkupPath ) ) ;
    QString res ;

    if ( dump.isEmpty() )
    {
        res = QString( configLS500::DEFAULT( BookmarkBkupPath ) ) ;
        open() ;
        this->setBookmarkDumpPath( res );
        close() ;
    }
    else
        res = dump ;
    return res ;
}

QString configLS500::getDictionaryPath()
{
    QString dump = getConfigParameter( configLS500::PARAMETER( DictionaryPath ) ) ;
    QString res ;

    if ( dump.isEmpty() )
    {
        res = QString( configLS500::DEFAULT( DictionaryPath ) ) ;
        open() ;
        this->setDictionaryPath( res );
        close() ;
    }
    else
        res = dump ;

    res.replace( QRegExp( "(^\\$HOME)" ) , QDir::homePath() ) ;
    return res ;
}

QString configLS500::getBookmarkBkUpFile()
{
    QString dump = getConfigParameter( configLS500::PARAMETER( BookmarkBkupFile ) ) ;
    QString res ;

    if ( dump.isEmpty() )
    {
        res = QString( configLS500::DEFAULT( BookmarkBkupFile ) ) ;
        open() ;
        this->setBookmarkBkUpFile( res );
        close() ;
    }
    else
        res = dump ;

    return res ;
}


QString configLS500::getBkUpCnt()
{
    QString dump = getConfigParameter( configLS500::PARAMETER( BookmarkBkupCnt ) ) ;
    QString res ;

    if ( dump.isEmpty() )
    {
        res = QString( configLS500::DEFAULT( BookmarkBkupCnt ) ) ;
        open() ;
        this->setBkUpCnt( res );
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
    file.open(  QIODevice::ReadOnly | QIODevice::Text ) ;

    while( !file.atEnd() ) {
        QString line = QString::fromUtf8( file.readLine() ) ;
        if ( line.size() > 0 ) {

            QRegExp reg( this->getLineRegex() ) ;
            int pos = reg.indexIn( line );
            QStringList list = reg.capturedTexts() ;

            list.takeFirst();

            if ( list.size() == 2 )
            {
                if (list[0] == name )
                    result = list[1] ;
            }
        }
    }

    file.close() ;

    int rs = result.size() ;

    if ( rs == 0 )
        return result ;

    result = result.remove( rs-1 , 1 ) ;
    
    return result ;
}

void configLS500::getAllParameters()
{
    QString result ;

    QFile file ;

    parameters.clear() ;

    file.setFileName( this->config_path );
    file.open(  QIODevice::ReadOnly | QIODevice::Text ) ;
    while ( !file.atEnd() ) {
        QString line ;
        line = QString::fromUtf8( file.readLine() ) ;

        if ( line.size() > 0 ) {

            QRegExp reg( this->getLineRegex() ) ;
            int pos = reg.indexIn( line );
            QStringList list = reg.capturedTexts() ;

            list.pop_front();
            if ( list.size() == 2 )
            {
                int ls = list[1].size() ;
                list[1] = list[1].remove( ls-1 , 1 ) ;
                parameters.insert( list[0] , list[1] ) ;
            }
        }
    }

    file.close() ;
}

void configLS500::writeAllParameters()
{
    QFile file ;
    file.setFileName( this->config_path );
    file.open(  QIODevice::WriteOnly | QIODevice::Text ) ;

    QList<QString> keys = parameters.keys() ;

    QTextStream out(&file) ;
    out.setCodec( "UTF-8" );

    for( QList<QString>::iterator it = keys.begin() ; it < keys.end() ; it++ )
    {
        QString line ;

        out << *it << SPACING << parameters[*it] << endl ;
        file.write( line.toUtf8().data() ) ;
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
    parameters.insert( configLS500::PARAMETER( DbDir ) , pr ) ;
}

void configLS500::setPDFPath1( QString pr )
{
    parameters.insert( configLS500::PARAMETER( ArtPath1 ) , pr ) ;
}

void configLS500::setPDFPath2( QString pr )
{
    parameters.insert( configLS500::PARAMETER( ArtPath2 ) , pr ) ;
}

void configLS500::setPDFAppl( QString pr )
{
    parameters.insert( configLS500::PARAMETER( PdfAppl ) , pr ) ;
}

void configLS500::setCopertinePath( QString pr )
{
    parameters.insert( configLS500::PARAMETER( CopertinePath ) , pr ) ;
}

void configLS500::setDVD( QString pr )
{
    parameters.insert( configLS500::PARAMETER( UseDvd ) , pr ) ;
}

void configLS500::setBookmarkDumpPath( QString pr )
{
    parameters.insert( configLS500::PARAMETER( BookmarkBkupPath ) , pr ) ;
}

void configLS500::setBookmarkBkUpFile( QString pr )
{
    parameters.insert( configLS500::PARAMETER( BookmarkBkupFile ) , pr ) ;
}

void configLS500::setBookmarkPath( QString pr )
{
    parameters.insert( configLS500::PARAMETER( BookmarkPath )  , pr ) ;
}

void configLS500::setBkUpCnt( QString pr )
{
    parameters.insert( configLS500::PARAMETER( BookmarkBkupCnt ) , pr ) ;
}

void configLS500::setDictionaryPath( QString pr )
{
    parameters.insert( configLS500::PARAMETER( DictionaryPath ) , pr ) ;
}

void configLS500::setParamenter( QString name , QString val )
{
    parameters.insert( name , val ) ;
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
