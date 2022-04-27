#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    data_base = QSqlDatabase::addDatabase("QMYSQL");//添加驱动
    data_base.setHostName("121.37.186.99");  //设置主机地址
    data_base.setPort(3306);  //mysql设置端口
    data_base.setDatabaseName("test");  //设置数据库名称
    data_base.setUserName("public");  //设置用户名
    data_base.setPassword("test");  //设置密码
    if(!data_base.open())//打开数据库
    {
        qDebug()<<"connect failed";
        qDebug() << data_base.lastError();//.databaseText()  输出错误信息
    }
    else
        qDebug()<<"success";
    AllOrderSqlTableModel = new QSqlTableModel(Q_NULLPTR, data_base);
    UserOrderSqlTableModel = new QSqlTableModel(Q_NULLPTR, data_base);



//    UserID = ui->UserIdLineEdit->text();
    UserID = "666";
    ui->UserIdLineEdit->setText(UserID);
}

MainWindow::~MainWindow()
{
    data_base.close();//关闭数据库
    delete ui;
}

void MainWindow::on_AllOrderButton_clicked()
{
    AllOrderSqlTableModel->setTable("Orders");
//    AllOrderSqlTableModel->setFilter(QString("accepterId='%1'").arg("NULL"));
    AllOrderSqlTableModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    AllOrderSqlTableModel->select();
    ui->AllSqlTable->setModel(AllOrderSqlTableModel);
    qDebug() << UserOrderSqlTableModel->rowCount() ;
    if(UserOrderSqlTableModel->rowCount() > 5){
        ui->statusBar->showMessage("大厅订单数超过五个，请注意接单不要太多了哟",10000);
    }
}

void MainWindow::on_HistoryOrderButton_clicked()
{

    UserOrderSqlTableModel->setTable("Orders");
    UserOrderSqlTableModel->setFilter(QString("accepterId='%1'").arg(UserID));//查询用户名
    UserOrderSqlTableModel->select();
    ui->UserSqlTable->setModel(UserOrderSqlTableModel);

    if(UserOrderSqlTableModel->rowCount() == 0){
        ui->statusBar->showMessage("您当前没有已接订单捏",10000);
    }
}
