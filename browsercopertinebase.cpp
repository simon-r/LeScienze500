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

#include "browsercopertinebase.h"


BrowserCopertineBase::BrowserCopertineBase()
{
}

bool BrowserCopertineBase::fillListaAnni()
{
    QueryDB db ;
    QueryResult lista = db.getAnni() ;

    if ( lista.empty() )
        return false ;

    QString col_name = lista.getFirstColumnName() ;

    QueryResult::iterator it;

    for( it = lista.begin() ; it < lista.end() ; it++ )
    {
        this->appendAnnoGUI( lista.getField( col_name , it ) );
    }
    return true ;
}
