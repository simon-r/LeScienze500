#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include <QUrl>

namespace Ui {
    class About;
}

class About : public QDialog
{
    Q_OBJECT

public:
    explicit About(QWidget *parent = 0);
    ~About();

private:
    Ui::About *ui;

public slots:
    void on_linkActivated( const QString& link ) ;
    void on_linkMessageClicked( const QUrl& url ) ;
};

#endif // ABOUT_H
