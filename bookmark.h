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

#ifndef BOOKMARK_H
#define BOOKMARK_H

#include <QStringList>
#include <QList>
#include <queryresult.h>

class Bookmark
{
public:
    Bookmark();

    bool buildDataBase() ;

    QueryResult getFavorites() ;
    QueryResult getCategorie() ;
    QueryResult getStati() ;
    QueryResult getValutazioni() ;

    QueryResult getFavoritesByStato( QString IdStato ) ;
    QueryResult getFavoritesByCategoria( QString IdCategoria ) ;

    QueryResult getCommento( QString IdFavorito ) ;
    QueryResult getValutazione( QString IdFavorito ) ;
    QueryResult getStato( QString IdFavorito ) ;
    QueryResult getCategoria( QString IdFavorito ) ;

    QueryResult getSottoCategorie( QString IdCategoria ) ;
    QueryResult getMainCategoria( QString IdCategoria ) ;

private:

};

#endif // BOOKMARK_H
