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

#define PDF_APPL "pdf_appl"
#define PDF_APPL_V "gv"

#define DB_DIR "db_dir"
#define DB_DIR_V "/opt/LeScienze500/LeScienze.db"

#define ART_PATH_1 "art_pdf_path1"
#define ART_PATH_1_V "/media/LESCIENZE/articoli/"

#define ART_PATH_2 "art_pdf_path2"
#define ART_PATH_2_V "/media/LESCIENZE/articoli/"

#define COPERTINE_P "copertine_path"
#define COPERTINE_P_V "/opt/LeScienze500/copertine/"

#define USE_DVD "dvd"
#define USE_DVD_V "no"

#define BOOKMARK_PATH "bookmark"
#define BOOKMARK_PATH_V "$HOME/.config/LeScienze500/bookmark_ls500.db"

class configLS500
{
public:
    configLS500();

    QString getDBPath() ;
    QString getPDFPath1() ;
    QString getPDFPath2() ;
    QString getPDFAppl() ;
    QString getCopertinePath() ;
    QString getDVD() ;
    QString getBookmarkPath() ;

    void open() ;
    void setDBPath( QString pr ) ;
    void setPDFPath1( QString pr ) ;
    void setPDFPath2( QString pr ) ;
    void setPDFAppl( QString pr ) ;
    void setCopertinePath( QString pr ) ;
    void setDVD( QString pr ) ;
    void setBookmarkPath( QString pr ) ;
    void close() ;

private:

    QString getConfigParameter( QString name ) ;

    void getAllParameters() ;
    void writeAllParameters() ;

    void initConfig() ;
    void resetToDefault() ;
    void writeDfaultFile( QFile *file ) ;

    QHash<QString,QString> parameters ;

    QString config_path ;
    QString config_dir ;
};

#endif // CONFIGLS500_H
