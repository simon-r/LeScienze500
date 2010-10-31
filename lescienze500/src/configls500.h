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


#ifndef CONFIGLS500_H
#define CONFIGLS500_H

#include<qstring.h>
#include<qhash.h>
#include <QFile>
#include <QDir>


#define SPACING "\t"
#define ENDL "\n"

class configLS500
{
public:

    enum  parName { Spacing , Endl , PdfAppl , DbDir , ArtPath1 , ArtPath2 , CopertinePath , UseDvd , BookmarkPath , BookmarkBkupPath , BookmarkBkupFile , BookmarkBkupCnt } ;

    static QString DEFAULT( parName pn ) ;
    static QString PARAMETER( parName pn ) ;

    configLS500();

    QString getDBPath() ;
    QString getPDFPath1() ;
    QString getPDFPath2() ;
    QString getPDFAppl() ;
    QString getCopertinePath() ;
    QString getDVD() ;
    QString getBookmarkPath() ;
    QString getBookmarkDumpPath() ;
    QString getBookmarkBkUpFile() ;
    QString getBkUpCnt() ;

    QString getParameter( QString name ) ;

    void open() ;
    bool isOpen() { return is_open ; }
    void setDBPath( QString pr ) ;
    void setPDFPath1( QString pr ) ;
    void setPDFPath2( QString pr ) ;
    void setPDFAppl( QString pr ) ;
    void setCopertinePath( QString pr ) ;
    void setDVD( QString pr ) ;
    void setBookmarkPath( QString pr ) ;
    void setBookmarkDumpPath( QString pr ) ;
    void setBookmarkBkUpFile( QString pr ) ;
    void setBkUpCnt( QString pr ) ;

    void setParamenter( QString name , QString val ) ;

    void close() ;

    QString getConfigDir() { return this->config_dir ; }



private:

    QString getConfigParameter( QString name ) ;

    void getAllParameters() ;
    void writeAllParameters() ;

    void initConfig() ;
    void resetToDefault() ;
    void writeDfaultFile() ;

    QString getLineRegex() { return QString ( "([\\w]+)\\t([\\w\\W]+)" ) ; }

    QHash<QString,QString> parameters ;

    QString config_path ;
    QString config_dir ;

    bool is_open ;
};

#endif // CONFIGLS500_H
