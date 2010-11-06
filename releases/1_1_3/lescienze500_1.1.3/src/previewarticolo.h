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

#ifndef PREVIEWARTICOLO_H
#define PREVIEWARTICOLO_H

#include <QDialog>
#include <QString>


namespace Ui {
    class PreviewArticolo;
}

class PreviewArticolo : public QDialog {
    Q_OBJECT
public:
    PreviewArticolo(QWidget *parent = 0);
    ~PreviewArticolo();

    void setHtml( QString &testo ) ;

    void setWinTitle( QString wint )
    {
        this->setWindowTitle( wint );
    }

protected:
    void changeEvent(QEvent *e);

private:
    Ui::PreviewArticolo *ui;

private slots:
    void on_Chiudi_clicked();
};

#endif // PREVIEWARTICOLO_H
