#include "mainwindow.h"

#include <QDebug>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

MainWindow::~MainWindow()
{
    qDebug() << "[DESTROY] MainWindow";
}

void MainWindow::setupUi()
{
    setObjectName("MainWindow");
    setWindowTitle("okp_lab2 — Qt Object Tree Demo");
    resize(560, 640);

    // Корневой layout окна
    m_rootLayout = new QVBoxLayout(this);

    // --- Виджет 1: заголовок -------------------------------------
    m_title = new LogLabel("Дерево объектов Qt: владелец и потомки", this);
    m_rootLayout->addWidget(m_title.data());

    // --- Виджет 2: поле ввода ------------------------------------
    m_input = new LogLineEdit(this);
    m_input->setPlaceholderText("Виджет-ребёнок главного окна");
    m_rootLayout->addWidget(m_input.data());

    // --- Виджет 3: статус ----------------------------------------
    m_status = new LogLabel("Статус: окно создано", this);
    m_status->setWordWrap(true);
    m_rootLayout->addWidget(m_status.data());

    // --- Виджет 4: панель-владелец -------------------------------
    m_panel = new LogPanel("Панель-владелец", this);
    m_rootLayout->addWidget(m_panel.data());

    m_panelLayout = new QVBoxLayout(m_panel.data());

    // Виджет 5: статическая кнопка внутри панели
    auto *staticButton = new LogButton("Статическая кнопка внутри панели",
                                       m_panel.data());
    m_panelLayout->addWidget(staticButton);

    // Виджет 6: перемещаемая кнопка
    m_movable = new LogButton("Перемещаемый виджет (в панели)",
                              m_panel.data());
    m_panelLayout->addWidget(m_movable.data());

    // --- Виджет 7: кнопка "Переместить" --------------------------
    m_moveButton = new LogButton("Переместить виджет: панель <-> окно", this);
    connect(m_moveButton.data(), &QPushButton::clicked,
            this, &MainWindow::moveMovableWidget);
    m_rootLayout->addWidget(m_moveButton.data());

    // --- Виджет 8: кнопка "Удалить панель" -----------------------
    m_deleteButton = new LogButton("Удалить панель (deleteLater)", this);
    connect(m_deleteButton.data(), &QPushButton::clicked,
            this, &MainWindow::deletePanel);
    m_rootLayout->addWidget(m_deleteButton.data());

    // --- Виджет 9: кнопка "Демо setParent(nullptr)" --------------
    m_detachButton = new LogButton("Демо setParent(nullptr)", this);
    connect(m_detachButton.data(), &QPushButton::clicked,
            this, &MainWindow::demoDetaching);
    m_rootLayout->addWidget(m_detachButton.data());

    m_rootLayout->addStretch();

    setStatus("Готово. Все виджеты созданы с родителями. "
              "Удаление родителя автоматически удалит детей.");
}

void MainWindow::setStatus(const QString &text)
{
    if (!m_status.isNull())
        m_status->setText(text);
}

// ----------------------------------------------------------------
// Перемещение виджета между панелью и главным окном.
// Демонстрирует смену владельца через setParent() и обязательное
// обновление layout.
// ----------------------------------------------------------------
void MainWindow::moveMovableWidget()
{
    if (m_movable.isNull()) {
        setStatus("Перемещаемый виджет уже уничтожен.");
        return;
    }

    if (m_panel.isNull()) {
        setStatus("Панель уже удалена. Вернуть виджет в неё невозможно.");
        return;
    }

    if (m_movable->parentWidget() == m_panel.data()) {
        // Из панели в окно
        if (!m_panelLayout.isNull())
            m_panelLayout->removeWidget(m_movable.data());

        m_movable->setParent(this);

        if (!m_rootLayout.isNull())
            m_rootLayout->addWidget(m_movable.data());

        m_movable->setText("Перемещаемый виджет (вне панели)");
        setStatus("setParent(this): главное окно стало новым владельцем. "
                  "Панель больше не удалит этот виджет автоматически.");
    } else {
        // Из окна обратно в панель
        if (!m_rootLayout.isNull())
            m_rootLayout->removeWidget(m_movable.data());

        m_movable->setParent(m_panel.data());

        if (!m_panelLayout.isNull())
            m_panelLayout->addWidget(m_movable.data());

        m_movable->setText("Перемещаемый виджет (в панели)");
        setStatus("Виджет снова стал ребёнком панели. "
                  "Теперь он будет удалён вместе с панелью.");
    }

    m_movable->show();
    qDebug() << "[SET_PARENT] Новый владелец виджета:"
             << m_movable->parentWidget()->objectName();
}

// ----------------------------------------------------------------
// Удаление панели через deleteLater().
// Если перемещаемая кнопка внутри — она удалится автоматически.
// ----------------------------------------------------------------
void MainWindow::deletePanel()
{
    if (m_panel.isNull()) {
        setStatus("Панель уже удалена.");
        return;
    }

    if (!m_movable.isNull() && m_movable->parentWidget() == m_panel.data()) {
        setStatus("Панель будет удалена. Перемещаемый виджет является её "
                  "ребёнком, поэтому будет уничтожен автоматически.");
    } else {
        setStatus("Панель будет удалена. Перемещаемый виджет не является её "
                  "ребёнком, поэтому останется жив.");
    }

    if (!m_deleteButton.isNull()) m_deleteButton->setEnabled(false);
    if (!m_moveButton.isNull())   m_moveButton->setEnabled(false);

    // Внутри слота используем deleteLater(), а не delete.
    m_panel->deleteLater();
    m_panel = nullptr;   // QPointer сам обнулился бы, но так нагляднее
}

// ----------------------------------------------------------------
// Демонстрация setParent(nullptr): ребёнок отвязывается от родителя
// и НЕ удаляется автоматически вместе с ним.
// ----------------------------------------------------------------
void MainWindow::demoDetaching()
{
    auto *tempOwner = new LogPanel("TempOwner", this);

    auto *child1 = new LogLabel("Ребёнок TempOwner", tempOwner);
    Q_UNUSED(child1);

    auto *child2 = new LogLabel("Отвязанный ребёнок TempOwner", tempOwner);

    qDebug() << "DEMO Количество детей tempOwner до detach:"
             << tempOwner->children().size();

    // Отвязываем child2 от родителя
    child2->setParent(nullptr);

    qDebug() << "DEMO Количество детей tempOwner после detach:"
             << tempOwner->children().size();

    // Удаляем владельца: child1 умрёт вместе с ним, child2 — нет
    delete tempOwner;

    // Отвязанный ребёнок теперь никем не владеет — удаляем вручную
    child2->deleteLater();
    child2->hide();

    setStatus("Демо setParent(nullptr) выполнено. Смотрите вывод в консоли: "
              "отвязанный ребёнок не был удалён вместе с владельцем "
              "и удалён вручную через deleteLater().");
}