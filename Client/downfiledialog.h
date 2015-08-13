/*======================================================
    > File Name: downfiledialog.h
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年07月28日 星期二 09时52分33秒
 =======================================================*/

#ifndef DOWNFILEDIALOG_H
#define DOWNFILEDIALOG_H

#include<QFileDialog>
#include<QDialog>
#include<QLabel>
#include<QLineEdit>
#include<QPushButton>

class DownFileDialog : public QDialog
{
    Q_OBJECT
public:
    DownFileDialog(QDialog* parent=0);
    ~DownFileDialog(){}
private slots:
    void LinePathShow(const QString &path);
    void DownLoadFile();
private:
    QFileDialog*    fileDialog;
    QLabel*         saveLabel;
    QLineEdit*      saveLineEdit;
    QPushButton*    saveButton;

};

#endif//DOWNFILEDIALOG_H

