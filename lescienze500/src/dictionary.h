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
