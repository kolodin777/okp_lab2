#ifndef WIDGETS_H
#define WIDGETS_H

#include <QDebug>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

// ============================================================
// Производные классы виджетов, которые логируют своё создание
// и уничтожение через qDebug(). Это позволяет наглядно увидеть
// работу дерева объектов QObject.
// ============================================================

class LogLabel : public QLabel
{
public:
    explicit LogLabel(const QString &text, QWidget *parent = nullptr)
        : QLabel(text, parent)
    {
        qDebug() << "[CREATE] LogLabel" << text;
    }

    ~LogLabel() override
    {
        qDebug() << "[DESTROY] LogLabel" << text();
    }
};

class LogLineEdit : public QLineEdit
{
public:
    explicit LogLineEdit(QWidget *parent = nullptr)
        : QLineEdit(parent)
    {
        qDebug() << "[CREATE] LogLineEdit";
    }

    ~LogLineEdit() override
    {
        qDebug() << "[DESTROY] LogLineEdit";
    }
};

class LogButton : public QPushButton
{
public:
    explicit LogButton(const QString &text, QWidget *parent = nullptr)
        : QPushButton(text, parent)
    {
        qDebug() << "[CREATE] LogButton" << text;
    }

    ~LogButton() override
    {
        qDebug() << "[DESTROY] LogButton" << text();
    }
};

class LogPanel : public QFrame
{
public:
    explicit LogPanel(const QString &name, QWidget *parent = nullptr)
        : QFrame(parent)
    {
        setObjectName(name);
        setFrameShape(QFrame::StyledPanel);
        qDebug() << "[CREATE] LogPanel" << name;
    }

    ~LogPanel() override
    {
        qDebug() << "[DESTROY] LogPanel" << objectName();
    }
};

#endif // WIDGETS_H