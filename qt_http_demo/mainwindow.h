#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QJsonArray>
#include <QJsonObject>
#include "apiclient.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    // GET tab
    void onGetPosts();
    void onGetPost();
    void onGetUsers();

    // POST tab
    void onCreatePost();

    // PUT tab
    void onUpdatePost();

    // DELETE tab
    void onDeletePost();

    // From ApiClient
    void onPostsReceived(const QJsonArray& posts);
    void onPostReceived(const QJsonObject& post);
    void onUsersReceived(const QJsonArray& users);
    void onPostCreated(const QJsonObject& post);
    void onPostUpdated(const QJsonObject& post);
    void onPostDeleted(int id);
    void onRequestStarted(const QString& desc);
    void onRequestFinished(int statusCode, qint64 elapsedMs);
    void onErrorOccurred(const QString& error);

private:
    void setupGetTab(QWidget* tab);
    void setupPostTab(QWidget* tab);
    void setupPutTab(QWidget* tab);
    void setupDeleteTab(QWidget* tab);
    void log(const QString& msg, const QString& color = "#000");
    void setStatus(const QString& msg, bool ok = true);

    ApiClient* m_client;

    // GET tab
    QTableWidget* m_postsTable;
    QSpinBox*     m_getIdSpin;
    QTextEdit*    m_singlePostView;
    QTableWidget* m_usersTable;

    // POST tab
    QLineEdit*    m_postTitle;
    QTextEdit*    m_postBody;
    QSpinBox*     m_postUserId;
    QTextEdit*    m_postResult;

    // PUT tab
    QSpinBox*     m_putId;
    QLineEdit*    m_putTitle;
    QTextEdit*    m_putBody;
    QTextEdit*    m_putResult;

    // DELETE tab
    QSpinBox*     m_deleteId;
    QTextEdit*    m_deleteResult;

    // Shared
    QTextEdit*    m_logView;
    QLabel*       m_statusBar;
};

#endif // MAINWINDOW_H
