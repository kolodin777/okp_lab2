#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPointer>
#include <QBoxLayout>
#include <QWidget>

#include "widgets.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void moveMovableWidget();   // перемещение виджета между панелью и окном
    void deletePanel();         // удаление панели через deleteLater()
    void demoDetaching();       // демонстрация setParent(nullptr)

private:
    void setupUi();
    void setStatus(const QString &text);

    QPointer<QBoxLayout> m_rootLayout;
    QPointer<QBoxLayout> m_panelLayout;

    QPointer<LogLabel>    m_title;
    QPointer<LogLineEdit> m_input;
    QPointer<LogLabel>    m_status;
    QPointer<LogPanel>    m_panel;
    QPointer<LogButton>   m_movable;
    QPointer<LogButton>   m_moveButton;
    QPointer<LogButton>   m_deleteButton;
    QPointer<LogButton>   m_detachButton;
};

#endif // MAINWINDOW_H