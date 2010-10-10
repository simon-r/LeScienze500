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

#include "setname.h"
#include "ui_setname.h"

SetName::SetName(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetName)
{
    ui->setupUi(this);
    p_text = "" ;
}

SetName::~SetName()
{
    delete ui;
}

void SetName::open()
{
    ui->lineEdit->setText("");
    setModal( true ) ;
    show() ;
    exec() ;
}

void SetName::open( QString txt )
{
    ui->lineEdit->setText( txt );
    setModal( true ) ;
    show() ;
    exec() ;
}

void SetName::on_OK_clicked()
{
    p_text = ui->lineEdit->text() ;
    this->close() ;
}

void SetName::on_Close_clicked()
{
    p_text = "" ;
    this->close() ;
}
