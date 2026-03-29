#ifndef APICLIENT_H
#define APICLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>

// ============================================================
// ApiClient — MODEL layer
//
// Wraps QNetworkAccessManager for HTTP REST operations.
//
// QNetworkAccessManager (QNAM):
//   - Qt's HTTP client — handles GET, POST, PUT, DELETE
//   - Asynchronous — never blocks the UI thread
//   - One QNAM per application (reuse it, don't create new ones)
//   - Returns QNetworkReply* — connect its signals for results
//
// REST API used: https://jsonplaceholder.typicode.com
//   Free fake REST API for testing — no auth, no signup
//   Endpoints:
//     GET  /posts          → list of posts
//     GET  /posts/{id}     → single post
//     POST /posts          → create post (returns fake id)
//     PUT  /posts/{id}     → update post
//     DELETE /posts/{id}   → delete post
//     GET  /users          → list of users
// ============================================================
class ApiClient : public QObject
{
    Q_OBJECT

public:
    explicit ApiClient(QObject* parent = nullptr);

public slots:
    // GET requests
    void getPosts();
    void getPost(int id);
    void getUsers();

    // POST — create
    void createPost(const QString& title,
                    const QString& body,
                    int userId);

    // PUT — full update
    void updatePost(int id,
                    const QString& title,
                    const QString& body);

    // DELETE
    void deletePost(int id);

signals:
    // Success signals
    void postsReceived(const QJsonArray& posts);
    void postReceived(const QJsonObject& post);
    void usersReceived(const QJsonArray& users);
    void postCreated(const QJsonObject& post);
    void postUpdated(const QJsonObject& post);
    void postDeleted(int id);

    // Status signals
    void requestStarted(const QString& description);
    void requestFinished(int statusCode, qint64 elapsedMs);
    void errorOccurred(const QString& error);

private slots:
    void onGetPostsReply(QNetworkReply* reply);
    void onGetPostReply(QNetworkReply* reply, int id);
    void onGetUsersReply(QNetworkReply* reply);
    void onCreatePostReply(QNetworkReply* reply);
    void onUpdatePostReply(QNetworkReply* reply);
    void onDeletePostReply(QNetworkReply* reply, int id);

private:
    QNetworkRequest buildRequest(const QString& path) const;
    QJsonObject     parseReply(QNetworkReply* reply, bool* ok);
    QJsonArray      parseArrayReply(QNetworkReply* reply, bool* ok);

    QNetworkAccessManager* m_nam;
    static const QString   BASE_URL;
};

#endif // APICLIENT_H
