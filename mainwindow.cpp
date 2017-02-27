#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    buttons = {ui->form, ui->test, ui->grade, ui->year, ui->group, ui->task, ui->gender, ui->level};
    for (auto i = 0; i < 8; ++i) {
        sieve.push_back(std::string());
        menus.push_back(new QMenu());
        sets.push_back(std::set<std::string>());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    menus.clear(); sets.clear(); sieve.clear();
    results.clear(); ui->alternatives->clear();
    resultsPath.clear();
    for (auto i = 0; i < 8; ++i) {
        sieve.push_back(std::string());
        menus.push_back(new QMenu());
        sets.push_back(std::set<std::string>());
    }
    url = QFileDialog::getExistingDirectory(this, tr("Open File"),"~", QFileDialog::ShowDirsOnly);
    ui->dir_path->setText(url);
    loadFiles();
}

void MainWindow::changeText(QString qstr, int i) {
    buttons.at(i)->setText(qstr);
    std::string temp = '<' + qstr.toStdString() + '>';
    sieve.at(i) = temp;
}

void MainWindow::loadFiles() {
    QDirIterator it(url, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
        QString filePath = it.filePath();
        QFileInfo currentFile(filePath);
        if (!currentFile.isDir()) {
            parseFiles(it.filePath());
        }
    }
    for (auto i = 0; i < 8; ++i) {
        auto source = sets.at(i);
        auto destinat = menus.at(i);
        MenuAction * allChosen = new MenuAction("ALL", this);
        allChosen->index = i;
        connect(allChosen, &QAction::triggered, allChosen, &MenuAction::itemTriggered);
        connect(allChosen, SIGNAL(changeTextSig(QString, int)), this, SLOT(changeText(QString, int)));
        destinat->addAction(allChosen);
        auto button = buttons.at(i);
        for (auto && item : source) {
            MenuAction * currentAction = new MenuAction(item.c_str(), this);
            currentAction->index = i;
            connect(currentAction, &QAction::triggered, currentAction, &MenuAction::itemTriggered);
            connect(currentAction, SIGNAL(changeTextSig(QString, int)), this, SLOT(changeText(QString, int)));
            destinat->addAction(currentAction);
        }
        button->setMenu(destinat);
    }
}

void MainWindow::parseFiles(QString path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    QTextStream stream(&file);
//    while (!stream.atEnd()) {
        QString qline = stream.readLine();
        std::string line = qline.toStdString();
        bool flag = false;
        auto setnum = 0;
        std::string temp;
        if ((line.length() != 0) && (line[0] == '<')) {
            for (auto && i : line) {
                if (i == '<') {
                    flag = true;
                }
                else if (i == '>') {
                    flag = false;
                    sets.at(setnum).insert(temp);
                    temp = {};
                    ++setnum;
                }
                else {
                    if (flag == true) {
                        temp.push_back(i);
//                        qDebug() << temp.c_str();
                    }
                    else {
                        continue;
                    }
                }
            }
        }
//    }
}

void MainWindow::on_ok_clicked()
{
    results.clear(); resultsPath.clear();
    ui->results->clear();
    ui->alternatives->clear();
    for(auto && i : sieve) {
        if (i.length() == 0) {
            ui->results->setText("Please select all the boxes before pressing OK.");
            return;
        }
    }
    fillList();
}

void MainWindow::fillList() {
    QDirIterator it(url, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
        QString filePath = it.filePath();
        QFileInfo currentFile(filePath);
        if (!currentFile.isDir()) {
            auto flag = sieveFiles(it.filePath());
            if (flag) {
                results.push_back(it.fileName());
                resultsPath.push_back(it.filePath());
            }
        }
    }
    ui->alternatives->addItems(results);
}

bool MainWindow::sieveFiles(QString path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream stream(&file);
    QString qline = stream.readLine();
    std::string line = qline.toStdString();
    for (auto && i : sieve) {
//        qDebug() << line.c_str() << i.c_str();
        if (i == "<ALL>") { continue; }
        else if (line.find(i) == std::string::npos) { return false; }
    }
    return true;
}

void MainWindow::on_alternatives_itemClicked(QListWidgetItem *item)
{
    ui->results->clear();
    auto index = ui->alternatives->currentRow();
    auto filePath = resultsPath.at(index);
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    QString content = file.readAll();
    ui->results->setText(content);
}

void MainWindow::on_generate_clicked()
{
    if (resultsPath.size() == 0) {
        ui->results->setText("No result has been found");
        return;
    }
    else {
        QString dirName;
        for (auto && i : sieve) {
            if (i == "<ALL>") { continue; }
            std::string temp = i.substr(1, i.length() - 2) + '_';
            dirName += temp.c_str();
        }
        dirName = url + QDir::separator() + dirName;
        QDir dir(dirName);
        if (!dir.exists()) { dir.mkpath("."); }
        for (size_t i = 0; i < resultsPath.size(); ++i) {
            QFile::copy(resultsPath.at(i), dirName + QDir::separator() + results.at(i));
        }
    }
}
