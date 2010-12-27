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

#include <QtGui/QApplication>
#include "lescienze500.h"
#include "configls500.h"
#include "bookmark.h"
#include <QDebug>
#include "querydb.h"
#include <QtPlugin>
#include <QCoreApplication>

int main(int argc, char *argv[])
{


    Bookmark bk ;
    if ( bk.initBookmark() )
    {
        qDebug() << "Ok" ;
    }
    else
    {
        qDebug() << "new bookmark" ;
    }

    configLS500 cfg ;

    QApplication a(argc, argv);
    LeScienze500 w;

#ifdef Q_WS_WIN
    qApp->addLibraryPath( qApp->applicationDirPath() ) ;
    qDebug() <<  qApp->applicationDirPath() ;
#endif

    w.show();
    return a.exec();
}
