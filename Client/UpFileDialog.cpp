/*======================================================
    > File Name: UpFileDialog.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年07月28日 星期二 21时09分41秒
 =======================================================*/

#include"upfiledialog.h"

UpFileDialog::UpFileDialog(QFileDialog* parent) : QFileDialog(parent)
{
    //设置当前目录
    this->setDirectory(".");
    //文件过滤器
    this->setFilter("*");
}
