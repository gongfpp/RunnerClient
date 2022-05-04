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
        ui->statusBar->showMessage("数据库链接失败 "+data_base.hostName());
    }
    else{
        qDebug()<<"success";
        ui->statusBar->showMessage("数据库链接成功 "+data_base.hostName());
    }



    AllOrderSqlTableModel = new QSqlTableModel(Q_NULLPTR, data_base);
    UserOrderSqlTableModel = new QSqlTableModel(Q_NULLPTR, data_base);
    userQuery = QSqlQuery(data_base);
    data_base.transaction();

    int a;
//    UserID = ui->UserIdLineEdit->text();
    UserID = "666";
    ui->UserIdLineEdit->setText(UserID);
//    ui->label->setText(QString("用户id：%1").arg(UserID));
//    ui->UserIdLineEdit->hide();
    ui->groupBox_3->hide();
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
        ui->statusBar->showMessage("大厅订单数超过五个，请注意接单不要太多了哟",1000);
    }
}

void MainWindow::on_HistoryOrderButton_clicked()
{
    UserID = ui->UserIdLineEdit->text();
    UserOrderSqlTableModel->setTable("Orders");
    UserOrderSqlTableModel->setFilter(QString("accepterId='%1'").arg(UserID));//查询用户名
    UserOrderSqlTableModel->select();
    ui->UserSqlTable->setModel(UserOrderSqlTableModel);
    qDebug() << UserOrderSqlTableModel->rowCount();
    if(UserOrderSqlTableModel->rowCount() == 0){
        ui->statusBar->showMessage("这哥们当前没有已接订单捏",3000);
    }else{
        ui->statusBar->showMessage(QString("%2的哥们订单数量:%1").arg(UserOrderSqlTableModel->rowCount()).arg(UserID));
    }
}

void MainWindow::on_takeOrderButton_clicked()
{
    UserID = ui->UserIdLineEdit->text();
    QString orderID;
    orderID = QInputDialog::getText(this,"选择订单号码","输入订单ID");
    if("" == orderID)   return ;
    QString sql = QString("update %1 set %2 = %3 where %4 = %5;").arg("Orders").arg("accepterId")
            .arg(UserID).arg("orderId").arg(orderID);
    bool sqlOk = userQuery.exec(sql);
    if(!sqlOk){
        QSqlError error = userQuery.lastError();
        qDebug() << error.text();
        ui->statusBar->showMessage("Orders表出错",4000);
        data_base.rollback();
        return ;
    }


    QString sqlPostion = QString("update %1 set %1.%2 = %3 where %1.%4 = %5;").arg("Positions").arg("states")
            .arg("\'已接单\'").arg("orderId").arg(orderID);
    sqlOk = userQuery.exec(sqlPostion);
    if(!sqlOk){
        QSqlError error = userQuery.lastError();
        qDebug() << error.text();
        ui->statusBar->showMessage("Position表出错",4000);
        data_base.rollback();
        return ;
    }


    ui->statusBar->showMessage("接单成功",4000);
    data_base.commit();

}

void MainWindow::on_cancelOrderButton_clicked()
{

    int orderID;
    orderID = QInputDialog::getInt(this,"选择订单号码","输入订单ID");
    qDebug() << orderID;

    if(0 == orderID)   return ;
    QString sql = QString("update %1 set %2 = null where %4 = %5;").arg("Orders").arg("accepterId")
            .arg("orderId").arg(orderID);
    bool sqlOk = userQuery.exec(sql);

    if(!sqlOk){
        QSqlError error = userQuery.lastError();
        ui->statusBar->showMessage("order表退单失败",4000);
        qDebug() << error.text();
        data_base.rollback();
        return ;
    }


    sql = QString("update %1 set %2 = null where %4 = %5;").arg("Positions").arg("states").arg("\'新建 \'")
            .arg("orderId").arg(orderID);
    sqlOk = userQuery.exec(sql);
    if(!sqlOk){
        QSqlError error = userQuery.lastError();

        ui->statusBar->showMessage("Positions表退单失败",4000);
        qDebug() << error.text();
        data_base.rollback();
        return ;
    }

    ui->statusBar->showMessage("退单成功！不用卷了！",4000);
    data_base.commit();
}



void MainWindow::on_CreateOrderButton_clicked()
{

}

void MainWindow::on_DeleteOrderButton_clicked()
{

}
