#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase> //sql驱动基础
#include <QSqlQuery>//sql查询相关
#include <QSqlError>//sql输出错误
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "QSqlTableModel"
#include "QMessageBox"
#include "QInputDialog"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_AllOrderButton_clicked();

    void on_HistoryOrderButton_clicked();

    void on_takeOrderButton_clicked();

    void on_cancelOrderButton_clicked();

    void on_CreateOrderButton_clicked();

    void on_DeleteOrderButton_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase data_base;
    QSqlTableModel *AllOrderSqlTableModel;
    QSqlTableModel *UserOrderSqlTableModel;
    QString UserID;
    QSqlQuery userQuery;

};

#endif // MAINWINDOW_H
