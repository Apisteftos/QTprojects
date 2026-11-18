#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QJsonDocument>
#include <QDateTime>
#include <QSplitter>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Qt HTTP REST Demo — QNetworkAccessManager");
    setMinimumSize(800, 600);

    m_client = new ApiClient(this);

    connect(m_client, &ApiClient::postsReceived,
            this,     &MainWindow::onPostsReceived);
    connect(m_client, &ApiClient::postReceived,
            this,     &MainWindow::onPostReceived);
    connect(m_client, &ApiClient::usersReceived,
            this,     &MainWindow::onUsersReceived);
    connect(m_client, &ApiClient::postCreated,
            this,     &MainWindow::onPostCreated);
    connect(m_client, &ApiClient::postUpdated,
            this,     &MainWindow::onPostUpdated);
    connect(m_client, &ApiClient::postDeleted,
            this,     &MainWindow::onPostDeleted);
    connect(m_client, &ApiClient::requestStarted,
            this,     &MainWindow::onRequestStarted);
    connect(m_client, &ApiClient::requestFinished,
            this,     &MainWindow::onRequestFinished);
    connect(m_client, &ApiClient::errorOccurred,
            this,     &MainWindow::onErrorOccurred);

    QWidget* central = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);
    mainLayout->setSpacing(6);
    mainLayout->setContentsMargins(8, 8, 8, 8);

    // ── API info bar ──────────────────────────────────────
    QLabel* apiLabel = new QLabel(
        "API: <b>https://jsonplaceholder.typicode.com</b> "
        "— free fake REST API for testing", this);
    apiLabel->setStyleSheet("color: #555; padding: 4px;");
    mainLayout->addWidget(apiLabel);

    // ── Tabs ──────────────────────────────────────────────
    QTabWidget* tabs = new QTabWidget(this);

    QWidget* getTab    = new QWidget();
    QWidget* postTab   = new QWidget();
    QWidget* putTab    = new QWidget();
    QWidget* deleteTab = new QWidget();

    setupGetTab(getTab);
    setupPostTab(postTab);
    setupPutTab(putTab);
    setupDeleteTab(deleteTab);

    tabs->addTab(getTab,    "GET — Read");
    tabs->addTab(postTab,   "POST — Create");
    tabs->addTab(putTab,    "PUT — Update");
    tabs->addTab(deleteTab, "DELETE — Remove");

    mainLayout->addWidget(tabs);

    // ── Log ───────────────────────────────────────────────
    QGroupBox* logGroup = new QGroupBox("Request Log", this);
    QVBoxLayout* logLayout = new QVBoxLayout(logGroup);
    m_logView = new QTextEdit(this);
    m_logView->setReadOnly(true);
    m_logView->setMaximumHeight(100);
    m_logView->setStyleSheet("font-family: monospace; font-size: 11px;");
    logLayout->addWidget(m_logView);
    mainLayout->addWidget(logGroup);

    // ── Status bar ────────────────────────────────────────
    m_statusBar = new QLabel("Ready", this);
    m_statusBar->setStyleSheet("padding: 2px 8px; background: #f0f0f0;");
    mainLayout->addWidget(m_statusBar);

    setCentralWidget(central);
}

// ── GET tab ───────────────────────────────────────────────
void MainWindow::setupGetTab(QWidget* tab) {
    QVBoxLayout* layout = new QVBoxLayout(tab);

    // All posts
    QGroupBox* allGroup = new QGroupBox("GET /posts — all posts", tab);
    QVBoxLayout* allLayout = new QVBoxLayout(allGroup);
    QPushButton* btnGetAll = new QPushButton("Load All Posts", tab);
    allLayout->addWidget(btnGetAll);
    m_postsTable = new QTableWidget(0, 3, tab);
    m_postsTable->setHorizontalHeaderLabels({"ID", "User ID", "Title"});
    m_postsTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    m_postsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_postsTable->setMaximumHeight(200);
    allLayout->addWidget(m_postsTable);
    layout->addWidget(allGroup);

    // Single post
    QGroupBox* singleGroup = new QGroupBox("GET /posts/{id} — single post", tab);
    QHBoxLayout* singleLayout = new QHBoxLayout(singleGroup);
    singleLayout->addWidget(new QLabel("Post ID:", tab));
    m_getIdSpin = new QSpinBox(tab);
    m_getIdSpin->setRange(1, 100);
    m_getIdSpin->setValue(1);
    singleLayout->addWidget(m_getIdSpin);
    QPushButton* btnGetOne = new QPushButton("Get Post", tab);
    singleLayout->addWidget(btnGetOne);
    m_singlePostView = new QTextEdit(tab);
    m_singlePostView->setReadOnly(true);
    m_singlePostView->setMaximumHeight(80);
    QVBoxLayout* singleOuter = new QVBoxLayout();
    singleOuter->addLayout(singleLayout);
    singleOuter->addWidget(m_singlePostView);
    // Replace horizontal with vertical
    QWidget* singleWidget = new QWidget(tab);
    singleWidget->setLayout(singleOuter);
    layout->addWidget(singleGroup);

    // Users
    QGroupBox* usersGroup = new QGroupBox("GET /users — all users", tab);
    QVBoxLayout* usersLayout = new QVBoxLayout(usersGroup);
    QPushButton* btnGetUsers = new QPushButton("Load Users", tab);
    usersLayout->addWidget(btnGetUsers);
    m_usersTable = new QTableWidget(0, 3, tab);
    m_usersTable->setHorizontalHeaderLabels({"ID", "Name", "Email"});
    m_usersTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_usersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_usersTable->setMaximumHeight(150);
    usersLayout->addWidget(m_usersTable);
    layout->addWidget(usersGroup);

    connect(btnGetAll,   &QPushButton::clicked, this, &MainWindow::onGetPosts);
    connect(btnGetOne,   &QPushButton::clicked, this, &MainWindow::onGetPost);
    connect(btnGetUsers, &QPushButton::clicked, this, &MainWindow::onGetUsers);
}

// ── POST tab ──────────────────────────────────────────────
void MainWindow::setupPostTab(QWidget* tab) {
    QVBoxLayout* layout = new QVBoxLayout(tab);
    QGroupBox* group = new QGroupBox("POST /posts — create a new post", tab);
    QVBoxLayout* gl = new QVBoxLayout(group);

    QHBoxLayout* row1 = new QHBoxLayout();
    row1->addWidget(new QLabel("Title:", tab));
    m_postTitle = new QLineEdit(tab);
    m_postTitle->setText("My new post title");
    row1->addWidget(m_postTitle);
    row1->addWidget(new QLabel("User ID:", tab));
    m_postUserId = new QSpinBox(tab);
    m_postUserId->setRange(1, 10);
    m_postUserId->setValue(1);
    row1->addWidget(m_postUserId);
    gl->addLayout(row1);

    gl->addWidget(new QLabel("Body:", tab));
    m_postBody = new QTextEdit(tab);
    m_postBody->setPlainText("This is the body of my new post.");
    m_postBody->setMaximumHeight(80);
    gl->addWidget(m_postBody);

    QPushButton* btnCreate = new QPushButton("Send POST Request", tab);
    btnCreate->setStyleSheet("background: #2E75B6; color: white; padding: 6px;");
    gl->addWidget(btnCreate);

    gl->addWidget(new QLabel("Response:", tab));
    m_postResult = new QTextEdit(tab);
    m_postResult->setReadOnly(true);
    gl->addWidget(m_postResult);

    layout->addWidget(group);
    connect(btnCreate, &QPushButton::clicked, this, &MainWindow::onCreatePost);
}

// ── PUT tab ───────────────────────────────────────────────
void MainWindow::setupPutTab(QWidget* tab) {
    QVBoxLayout* layout = new QVBoxLayout(tab);
    QGroupBox* group = new QGroupBox("PUT /posts/{id} — update a post", tab);
    QVBoxLayout* gl = new QVBoxLayout(group);

    QHBoxLayout* row1 = new QHBoxLayout();
    row1->addWidget(new QLabel("Post ID:", tab));
    m_putId = new QSpinBox(tab);
    m_putId->setRange(1, 100);
    m_putId->setValue(1);
    row1->addWidget(m_putId);
    gl->addLayout(row1);

    gl->addWidget(new QLabel("New Title:", tab));
    m_putTitle = new QLineEdit(tab);
    m_putTitle->setText("Updated title");
    gl->addWidget(m_putTitle);

    gl->addWidget(new QLabel("New Body:", tab));
    m_putBody = new QTextEdit(tab);
    m_putBody->setPlainText("Updated body content.");
    m_putBody->setMaximumHeight(80);
    gl->addWidget(m_putBody);

    QPushButton* btnUpdate = new QPushButton("Send PUT Request", tab);
    btnUpdate->setStyleSheet("background: #cc6622; color: white; padding: 6px;");
    gl->addWidget(btnUpdate);

    gl->addWidget(new QLabel("Response:", tab));
    m_putResult = new QTextEdit(tab);
    m_putResult->setReadOnly(true);
    gl->addWidget(m_putResult);

    layout->addWidget(group);
    connect(btnUpdate, &QPushButton::clicked, this, &MainWindow::onUpdatePost);
}

// ── DELETE tab ────────────────────────────────────────────
void MainWindow::setupDeleteTab(QWidget* tab) {
    QVBoxLayout* layout = new QVBoxLayout(tab);
    QGroupBox* group = new QGroupBox("DELETE /posts/{id} — delete a post", tab);
    QVBoxLayout* gl = new QVBoxLayout(group);

    QHBoxLayout* row = new QHBoxLayout();
    row->addWidget(new QLabel("Post ID to delete:", tab));
    m_deleteId = new QSpinBox(tab);
    m_deleteId->setRange(1, 100);
    m_deleteId->setValue(1);
    row->addWidget(m_deleteId);
    QPushButton* btnDelete = new QPushButton("Send DELETE Request", tab);
    btnDelete->setStyleSheet("background: #cc2222; color: white; padding: 6px;");
    row->addWidget(btnDelete);
    gl->addLayout(row);

    gl->addWidget(new QLabel("Response:", tab));
    m_deleteResult = new QTextEdit(tab);
    m_deleteResult->setReadOnly(true);
    gl->addWidget(m_deleteResult);

    layout->addWidget(group);
    layout->addStretch();
    connect(btnDelete, &QPushButton::clicked, this, &MainWindow::onDeletePost);
}

// ── Button handlers ───────────────────────────────────────
void MainWindow::onGetPosts()   { m_client->getPosts(); }
void MainWindow::onGetPost()    { m_client->getPost(m_getIdSpin->value()); }
void MainWindow::onGetUsers()   { m_client->getUsers(); }

void MainWindow::onCreatePost() {
    m_client->createPost(m_postTitle->text(),
                         m_postBody->toPlainText(),
                         m_postUserId->value());
}

void MainWindow::onUpdatePost() {
    m_client->updatePost(m_putId->value(),
                         m_putTitle->text(),
                         m_putBody->toPlainText());
}

void MainWindow::onDeletePost() {
    m_client->deletePost(m_deleteId->value());
}

// ── ApiClient response handlers ───────────────────────────
void MainWindow::onPostsReceived(const QJsonArray& posts) {
    m_postsTable->setRowCount(0);
    for (const QJsonValue& v : posts) {
        QJsonObject p = v.toObject();
        int row = m_postsTable->rowCount();
        m_postsTable->insertRow(row);
        m_postsTable->setItem(row, 0, new QTableWidgetItem(
            QString::number(p["id"].toInt())));
        m_postsTable->setItem(row, 1, new QTableWidgetItem(
            QString::number(p["userId"].toInt())));
        m_postsTable->setItem(row, 2, new QTableWidgetItem(
            p["title"].toString()));
    }
    log(QString("Loaded %1 posts").arg(posts.size()));
}

void MainWindow::onPostReceived(const QJsonObject& post) {
    m_singlePostView->setPlainText(
        QString("ID: %1\nTitle: %2\nBody: %3")
        .arg(post["id"].toInt())
        .arg(post["title"].toString())
        .arg(post["body"].toString()));
}

void MainWindow::onUsersReceived(const QJsonArray& users) {
    m_usersTable->setRowCount(0);
    for (const QJsonValue& v : users) {
        QJsonObject u = v.toObject();
        int row = m_usersTable->rowCount();
        m_usersTable->insertRow(row);
        m_usersTable->setItem(row, 0, new QTableWidgetItem(
            QString::number(u["id"].toInt())));
        m_usersTable->setItem(row, 1, new QTableWidgetItem(
            u["name"].toString()));
        m_usersTable->setItem(row, 2, new QTableWidgetItem(
            u["email"].toString()));
    }
    log(QString("Loaded %1 users").arg(users.size()));
}

void MainWindow::onPostCreated(const QJsonObject& post) {
    QString pretty = QJsonDocument(post)
        .toJson(QJsonDocument::Indented);
    m_postResult->setPlainText(pretty);
    log(QString("Created post — server assigned ID: %1")
        .arg(post["id"].toInt()));
}

void MainWindow::onPostUpdated(const QJsonObject& post) {
    QString pretty = QJsonDocument(post)
        .toJson(QJsonDocument::Indented);
    m_putResult->setPlainText(pretty);
    log("Post updated successfully");
}

void MainWindow::onPostDeleted(int id) {
    m_deleteResult->setPlainText(
        QString("Post %1 deleted successfully.\n"
                "Server returned: {} (empty object)").arg(id));
    log(QString("Deleted post %1").arg(id));
}

void MainWindow::onRequestStarted(const QString& desc) {
    setStatus(QString("→ %1").arg(desc));
    log(QString("→ %1").arg(desc), "#2266cc");
}

void MainWindow::onRequestFinished(int statusCode, qint64 elapsedMs) {
    Q_UNUSED(elapsedMs)
    bool ok = (statusCode >= 200 && statusCode < 300);
    setStatus(QString("HTTP %1 %2")
        .arg(statusCode)
        .arg(ok ? "OK" : "Error"), ok);
    log(QString("← HTTP %1").arg(statusCode),
        ok ? "#228822" : "#cc2222");
}

void MainWindow::onErrorOccurred(const QString& error) {
    setStatus("Error: " + error, false);
    log("Error: " + error, "#cc2222");
}

void MainWindow::log(const QString& msg, const QString& color) {
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
    m_logView->append(
        QString("<span style='color:#888'>[%1]</span> "
                "<span style='color:%2'>%3</span>")
        .arg(time, color, msg));
}

void MainWindow::setStatus(const QString& msg, bool ok) {
    m_statusBar->setText(msg);
    m_statusBar->setStyleSheet(
        QString("padding: 2px 8px; background: %1; color: %2;")
        .arg(ok ? "#e8f5e9" : "#ffebee")
        .arg(ok ? "#1b5e20" : "#b71c1c"));
}
