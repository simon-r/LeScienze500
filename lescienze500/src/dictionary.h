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
    QHash< int , QPair<int,int> > articoloid_cnt ;
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
