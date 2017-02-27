#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QDebug>
#include <QMenu>
#include <QDirIterator>
#include <QFileInfo>
#include <QFileDialog>
#include <QAction>
#include <QPushButton>
#include <QStringList>
#include <QListWidgetItem>
#include <QDir>

#include <set>
#include <vector>
#include <string>

#include <menuaction.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void changeText(QString, int);
    void on_ok_clicked();
    void on_alternatives_itemClicked(QListWidgetItem *item);
    void on_generate_clicked();

private:
    Ui::MainWindow *ui;
    QString url;
    std::vector<QMenu *> menus;
    std::vector<std::set<std::string>> sets;
    std::vector<std::string> sieve;
    std::vector<QPushButton *> buttons;
    QStringList results;
    std::vector<QString> resultsPath;
    void loadFiles();
    void parseFiles(QString);
    void fillList();
    bool sieveFiles(QString);
};

#endif // MAINWINDOW_H
