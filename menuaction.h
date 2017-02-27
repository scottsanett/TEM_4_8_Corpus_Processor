#ifndef MENUACTION_H
#define MENUACTION_H

#include <QObject>
#include <QAction>
#include <QString>

class MenuAction : public QAction
{
    Q_OBJECT
public:
    explicit MenuAction(QString line, QObject * parent): QAction(line, parent) {}
    int index = 0;

signals:
    void changeTextSig(QString, int);

private:

public slots:
    void itemTriggered() {
        emit changeTextSig(this->text(), index);
    }
};

#endif // MENUACTION_H
