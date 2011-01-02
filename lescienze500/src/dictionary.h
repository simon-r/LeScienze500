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

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QString>
#include <QHash>
#include <QPair>
#include <QList>
#include <QObject>

// select * from words where length(word) > 3 and cnt > 200 and id in ( select idword from Word_Categoria where idcategoria in (select id from categorie where categoria like "biologia") ) order by cnt ;
// select categoria from categorie where id in ( select idcategoria from Word_Categoria where idword in ( select id from words where word like "mantide" ) );
// select categoria , cnt from categorie , Word_Categoria where categorie.id = Word_Categoria.IdCategoria and categorie.id in ( select idcategoria from Word_Categoria where idword in ( select id from words where word like "energia" ) ) group by categoria ;
// select id , categoria from categorie where id in ( select idcategoria from Word_Categoria where idword in ( select id from words where word like "magnete" ) ) group by categoria ;


class WordInfo
{
public:
    WordInfo() { cnt = 0 ; }
    QString word ;
    int cnt ;
    QHash< int , QPair<int,int> > year_cnt ;
    QHash< int , QPair<int,int> > categoriaid_cnt ;
 //   QHash< int , QPair<int,int> > articoloid_cnt ;
};

class Dictionary : public QObject
{
    Q_OBJECT
public:
    Dictionary();

    bool initDictionary() ;
    bool buildDictionary() ;

    bool addText( QString text , QString id_art ) ;
    bool fillDataBase() ;
    bool exists() ;

    bool addYears() ;
    bool addCategorie() ;
    bool addIdArticoli() ;
private:

    QHash<QString,WordInfo> dictionary ;

signals:
    void sig_progress( int cnt , int total ) ;

};

#endif // DICTIONARY_H
