#include "translatedialog.h"
#include "ui_translatedialog.h"

TranslateDialog::TranslateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TranslateDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::clicked,
           [=](QAbstractButton * button)
            {
                float x, y;
                if(ui->buttonBox->button(QDialogButtonBox::Ok) == static_cast<QPushButton *>(button)) {  //判断按下的是否为"确定”按钮
                    if(!ui->lineEdit_x->text().isEmpty() && !ui->lineEdit_y->text().isEmpty())  //判断lineEdit是否为空
                    {
                        x = ui->lineEdit_x->text().toFloat();
                        y = ui->lineEdit_y->text().toFloat();
                        emit translate_x_y(x, y);
                    }
                }
            }
            );
}

TranslateDialog::~TranslateDialog()
{
    delete ui;
}

