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

#include "previewarticolo.h"
#include "ui_previewarticolo.h"

PreviewArticolo::PreviewArticolo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreviewArticolo)
{
    ui->setupUi(this);
}

PreviewArticolo::~PreviewArticolo()
{
    delete ui;
}

void PreviewArticolo::changeEvent(QEvent *e)
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

void PreviewArticolo::setHtml( QString &testo )
{
    ui->Testo->setHtml( testo );
}

void PreviewArticolo::on_Chiudi_clicked()
{
    this->setVisible( false );
}
