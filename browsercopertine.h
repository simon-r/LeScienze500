#ifndef BROWSERCOPERTINE_H
#define BROWSERCOPERTINE_H

#include <QDialog>

namespace Ui {
    class BrowserCopertine;
}

class BrowserCopertine : public QDialog
{
    Q_OBJECT
public:
    BrowserCopertine(QWidget *parent = 0);
    ~BrowserCopertine();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::BrowserCopertine *ui;

};

#endif // BROWSERCOPERTINE_H
