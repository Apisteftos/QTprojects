#include "apiclient.h"
#include <QElapsedTimer>

const QString ApiClient::BASE_URL = "https://jsonplaceholder.typicode.com";

ApiClient::ApiClient(QObject* parent)
    : QObject(parent)
{
    // One QNetworkAccessManager per application — reuse it
    m_nam = new QNetworkAccessManager(this);
}

// ── Helper: build a request with common headers ────────────
QNetworkRequest ApiClient::buildRequest(const QString& path) const {
    QNetworkRequest request(QUrl(BASE_URL + path));
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/json");
    // Set a reasonable timeout
    request.setTransferTimeout(10000); // 10 seconds
    return request;
}

// ── Helper: parse JSON object from reply ──────────────────
QJsonObject ApiClient::parseReply(QNetworkReply* reply, bool* ok) {
    *ok = false;
    if (reply->error() != QNetworkReply::NoError) {
        emit errorOccurred(reply->errorString());
        reply->deleteLater();
        return {};
    }
    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    reply->deleteLater();
    if (!doc.isObject()) {
        emit errorOccurred("Response is not a JSON object");
        return {};
    }
    *ok = true;
    return doc.object();
}

// ── Helper: parse JSON array from reply ───────────────────
QJsonArray ApiClient::parseArrayReply(QNetworkReply* reply, bool* ok) {
    *ok = false;
    if (reply->error() != QNetworkReply::NoError) {
        emit errorOccurred(reply->errorString());
        reply->deleteLater();
        return {};
    }
    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    reply->deleteLater();
    if (!doc.isArray()) {
        emit errorOccurred("Response is not a JSON array");
        return {};
    }
    *ok = true;
    return doc.array();
}

// ── GET /posts ─────────────────────────────────────────────
void ApiClient::getPosts() {
    emit requestStarted("GET /posts");
    QNetworkReply* reply = m_nam->get(buildRequest("/posts"));

    connect(reply, &QNetworkReply::finished,
            this,  [this, reply]() {
        onGetPostsReply(reply);
    });
}

void ApiClient::onGetPostsReply(QNetworkReply* reply) {
    int status = reply->attribute(
        QNetworkRequest::HttpStatusCodeAttribute).toInt();
    bool ok;
    QJsonArray posts = parseArrayReply(reply, &ok);
    emit requestFinished(status, 0);
    if (ok) emit postsReceived(posts);
}

// ── GET /posts/{id} ────────────────────────────────────────
void ApiClient::getPost(int id) {
    emit requestStarted(QString("GET /posts/%1").arg(id));
    QNetworkReply* reply = m_nam->get(
        buildRequest(QString("/posts/%1").arg(id)));

    connect(reply, &QNetworkReply::finished,
            this,  [this, reply, id]() {
        onGetPostReply(reply, id);
    });
}

void ApiClient::onGetPostReply(QNetworkReply* reply, int id) {
    Q_UNUSED(id)
    int status = reply->attribute(
        QNetworkRequest::HttpStatusCodeAttribute).toInt();
    bool ok;
    QJsonObject post = parseReply(reply, &ok);
    emit requestFinished(status, 0);
    if (ok) emit postReceived(post);
}

// ── GET /users ─────────────────────────────────────────────
void ApiClient::getUsers() {
    emit requestStarted("GET /users");
    QNetworkReply* reply = m_nam->get(buildRequest("/users"));

    connect(reply, &QNetworkReply::finished,
            this,  [this, reply]() {
        onGetUsersReply(reply);
    });
}

void ApiClient::onGetUsersReply(QNetworkReply* reply) {
    int status = reply->attribute(
        QNetworkRequest::HttpStatusCodeAttribute).toInt();
    bool ok;
    QJsonArray users = parseArrayReply(reply, &ok);
    emit requestFinished(status, 0);
    if (ok) emit usersReceived(users);
}

// ── POST /posts ────────────────────────────────────────────
void ApiClient::createPost(const QString& title,
                           const QString& body,
                           int userId) {
    emit requestStarted("POST /posts");

    // Build JSON body
    QJsonObject payload;
    payload["title"]  = title;
    payload["body"]   = body;
    payload["userId"] = userId;
    QByteArray data = QJsonDocument(payload).toJson(QJsonDocument::Compact);

    QNetworkReply* reply = m_nam->post(buildRequest("/posts"), data);

    connect(reply, &QNetworkReply::finished,
            this,  [this, reply]() {
        onCreatePostReply(reply);
    });
}

void ApiClient::onCreatePostReply(QNetworkReply* reply) {
    int status = reply->attribute(
        QNetworkRequest::HttpStatusCodeAttribute).toInt();
    bool ok;
    QJsonObject post = parseReply(reply, &ok);
    emit requestFinished(status, 0);
    if (ok) emit postCreated(post);
}

// ── PUT /posts/{id} ────────────────────────────────────────
void ApiClient::updatePost(int id,
                           const QString& title,
                           const QString& body) {
    emit requestStarted(QString("PUT /posts/%1").arg(id));

    QJsonObject payload;
    payload["id"]     = id;
    payload["title"]  = title;
    payload["body"]   = body;
    payload["userId"] = 1;
    QByteArray data = QJsonDocument(payload).toJson(QJsonDocument::Compact);

    QNetworkReply* reply = m_nam->put(
        buildRequest(QString("/posts/%1").arg(id)), data);

    connect(reply, &QNetworkReply::finished,
            this,  [this, reply]() {
        onUpdatePostReply(reply);
    });
}

void ApiClient::onUpdatePostReply(QNetworkReply* reply) {
    int status = reply->attribute(
        QNetworkRequest::HttpStatusCodeAttribute).toInt();
    bool ok;
    QJsonObject post = parseReply(reply, &ok);
    emit requestFinished(status, 0);
    if (ok) emit postUpdated(post);
}

// ── DELETE /posts/{id} ─────────────────────────────────────
void ApiClient::deletePost(int id) {
    emit requestStarted(QString("DELETE /posts/%1").arg(id));

    QNetworkReply* reply = m_nam->deleteResource(
        buildRequest(QString("/posts/%1").arg(id)));

    connect(reply, &QNetworkReply::finished,
            this,  [this, reply, id]() {
        onDeletePostReply(reply, id);
    });
}

void ApiClient::onDeletePostReply(QNetworkReply* reply, int id) {
    int status = reply->attribute(
        QNetworkRequest::HttpStatusCodeAttribute).toInt();
    reply->deleteLater();
    emit requestFinished(status, 0);
    if (status == 200) emit postDeleted(id);
}
