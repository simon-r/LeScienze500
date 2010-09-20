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

protected:
    void changeEvent(QEvent *e);

private:
    Ui::PreviewArticolo *ui;

private slots:
    void on_Chiudi_clicked();
};

#endif // PREVIEWARTICOLO_H
