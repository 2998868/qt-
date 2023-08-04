#ifndef TRANSLATEDIALOG_H
#define TRANSLATEDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
namespace Ui {
class TranslateDialog;
}

class TranslateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TranslateDialog(QWidget *parent = nullptr);
    ~TranslateDialog();

private:
    Ui::TranslateDialog *ui;

signals:
    void translate_x_y(float x,float y);
};

#endif // TRANSLATEDIALOG_H
