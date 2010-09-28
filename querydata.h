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

#ifndef QUERYDATA_H
#define QUERYDATA_H

#include <QObject>
#include <QString>
#include <QStringList>

class QueryData : public QObject
{
public:
    QueryData();

    enum Logical { QEUERY_AND , QUERY_OR }  ;

    void setParoleChiaveTitolo( const QString &pct );

    void setParoleChiaveAbstract( const QString &pca );

    void setParoleChiaveTesto( const QString &pct );

    void setCategorie( const QStringList &cat );

    void setRubriche( const QStringList &ru ) ;

    void setAnni( const QStringList &a ) ;

    void setAutori( const QStringList aut ) ;



    QString getParoleChiaveTitolo();

    QString getParoleChiaveAbstract();

    QString getParoleChiaveTesto();

    QStringList getCategorie();

    QStringList getRubriche() ;

    QStringList getAnni() ;

    QStringList getAutori() ;

private:

    QString parole_chiave_titolo ;
    QString parole_chiave_abstract ;
    QString parole_chiave_testo ;

    QStringList categorie ;
    QStringList rubriche ;
    QStringList anni ;
    QStringList autori ;

    QueryData::Logical g_logical ;
};

#endif // QUERYDATA_H
