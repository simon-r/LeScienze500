#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QString>

class Dictionary
{
public:
    Dictionary();

    bool initDictionary() ;
    bool buildDictionary() ;
    bool addText( QString text , QString id_art ) ;
    bool exists() ;

private:

};

#endif // DICTIONARY_H
