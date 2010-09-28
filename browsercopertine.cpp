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

#include "browsercopertine.h"
#include "ui_browsercopertine.h"

BrowserCopertine::BrowserCopertine(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::BrowserCopertine)
{
    ui->setupUi(this);
}

BrowserCopertine::~BrowserCopertine()
{
    delete ui ;
}

void BrowserCopertine::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void BrowserCopertine::appendAnnoGUI( QString anno )
{
    ui->ListaAnni->addItem( anno );
}

void BrowserCopertine::clearListaAnni()
{
    ui->ListaAnni->clear();
}

void BrowserCopertine::openBrowser()
{
    this->fillListaAnni() ;

    this->setModal(true);
    this->setFocus();
    this->show();
}
