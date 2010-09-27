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
