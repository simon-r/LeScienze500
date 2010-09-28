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

#ifndef BROWSERCOPERTINEBASE_H
#define BROWSERCOPERTINEBASE_H

#include "querydata.h"
#include <QString>
#include <QStringList>
#include "querydb.h"


class BrowserCopertineBase
{
public:
    BrowserCopertineBase();

    void openBrowser() ;
    void openBrowser( QueryData &data ) ;
    void close() ;

    bool showAnno( const QString &anno ) ;
    bool showMeseAnno( const QString &mese , const QString &anno ) ;
    bool showRivista( const QString &numero ) ;

protected:

    bool fillListaAnni() ;

    virtual void appendAnnoGUI( QString anno ) = 0 ;

    void clear() ;


};

#endif // BROWSERCOPERTINEBASE_H
