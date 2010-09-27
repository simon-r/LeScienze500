#ifndef BROWSERCOPERTINEBASE_H
#define BROWSERCOPERTINEBASE_H

#include "querydata.h"

class BrowserCopertineBase
{
public:
    BrowserCopertineBase();

    void OpenBrowser() ;
    void OpenBrowser( QueryData &data ) ;
};

#endif // BROWSERCOPERTINEBASE_H
