#include "networkleaderboardmanager.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCryptographicHash>
#include <QDebug>
#include <QElapsedTimer>

NetworkLeaderboardManager& NetworkLeaderboardManager::instance()
{
    static NetworkLeaderboardManager instance;
    return instance;
}

NetworkLeaderboardManager::NetworkLeaderboardManager()
    : QObject(nullptr)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_serverUrl("http://localhost:8080/api") // Default mock server URL //26.233.90.189
    , m_networkEnabled(true)
    , m_lastRequestTime(0)
{
    // Connect network manager signals
    connect(m_networkManager, &QNetworkAccessManager::finished,
            this, [](QNetworkReply *reply) {
                reply->deleteLater();
            });
}

NetworkLeaderboardManager::~NetworkLeaderboardManager()
{
}

void NetworkLeaderboardManager::setServerUrl(const QString &url)
{
    m_serverUrl = url;
    qDebug() << "Network leaderboard server URL set to:" << m_serverUrl;
}

void NetworkLeaderboardManager::submitScore(const PlayerScore &score)
{
    if (!m_networkEnabled) {
        emit scoreSubmitted(false, "Network is disabled");
        return;
    }
    
    // Rate limiting check
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    if (currentTime - m_lastRequestTime < MIN_REQUEST_INTERVAL_MS) {
        qDebug() << "Rate limit: Please wait before submitting another score";
        emit scoreSubmitted(false, "Rate limit exceeded. Please wait.");
        return;
    }
    m_lastRequestTime = currentTime;
    
    // Prepare JSON data
    QJsonObject scoreData = score.toJson();
    
    // Add checksum for basic validation
    QString checksum = calculateChecksum(scoreData);
    scoreData["checksum"] = checksum;
    
    QJsonDocument doc(scoreData);
    QByteArray data = doc.toJson();
    
    // Create network request
    QNetworkRequest request;
    request.setUrl(QUrl(m_serverUrl + "/submitScore"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setTransferTimeout(10000); // 10 second timeout
    
    qDebug() << "Submitting score to:" << request.url().toString();
    qDebug() << "Data:" << data;
    
    // Send POST request
    QNetworkReply *reply = m_networkManager->post(request, data);
    
    // Connect reply signals
    connect(reply, &QNetworkReply::finished, 
            this, &NetworkLeaderboardManager::onSubmitScoreFinished);
    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::errorOccurred),
            this, &NetworkLeaderboardManager::onNetworkError);
    connect(reply, &QNetworkReply::sslErrors,
            this, &NetworkLeaderboardManager::onSslErrors);
}

void NetworkLeaderboardManager::fetchLeaderboard(int levelId)
{
    if (!m_networkEnabled) {
        emit leaderboardReceived(QVector<PlayerScore>());
        return;
    }
    
    // Rate limiting check
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    if (currentTime - m_lastRequestTime < MIN_REQUEST_INTERVAL_MS) {
        qDebug() << "Rate limit: Please wait before fetching leaderboard again";
        return;
    }
    m_lastRequestTime = currentTime;
    
    // Build URL with query parameter
    QString urlStr = m_serverUrl + "/leaderboard";
    if (levelId > 0) {
        urlStr += "?levelId=" + QString::number(levelId);
    }
    
    QNetworkRequest request;
    request.setUrl(QUrl(urlStr));
    request.setTransferTimeout(10000); // 10 second timeout
    
    qDebug() << "Fetching leaderboard from:" << request.url().toString();
    
    // Send GET request
    QNetworkReply *reply = m_networkManager->get(request);
    
    // Connect reply signals
    connect(reply, &QNetworkReply::finished,
            this, &NetworkLeaderboardManager::onFetchLeaderboardFinished);
    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::errorOccurred),
            this, &NetworkLeaderboardManager::onNetworkError);
    connect(reply, &QNetworkReply::sslErrors,
            this, &NetworkLeaderboardManager::onSslErrors);
}

void NetworkLeaderboardManager::onSubmitScoreFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;
    
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(responseData);
        
        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            bool success = obj["success"].toBool(false);
            QString message = obj["message"].toString("Score submitted successfully");
            
            qDebug() << "Score submission response:" << success << message;
            emit scoreSubmitted(success, message);
        } else {
            qDebug() << "Score submitted (legacy response)";
            emit scoreSubmitted(true, "Score submitted successfully");
        }
    } else {
        QString errorMsg = "Failed to submit score: " + reply->errorString();
        qDebug() << errorMsg;
        emit scoreSubmitted(false, errorMsg);
    }
}

void NetworkLeaderboardManager::onFetchLeaderboardFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;
    
    QVector<PlayerScore> scores;
    
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(responseData);
        
        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            QJsonArray scoresArray = obj["scores"].toArray();
            
            for (const QJsonValue &value : scoresArray) {
                if (value.isObject()) {
                    PlayerScore score = PlayerScore::fromJson(value.toObject());
                    scores.append(score);
                }
            }
            
            qDebug() << "Fetched" << scores.size() << "scores from online leaderboard";
        }
    } else {
        qDebug() << "Failed to fetch leaderboard:" << reply->errorString();
        emit networkError("Failed to fetch leaderboard: " + reply->errorString());
    }
    
    emit leaderboardReceived(scores);
}

void NetworkLeaderboardManager::onNetworkError(QNetworkReply::NetworkError error)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;
    
    QString errorMsg = QString("Network error (%1): %2").arg(error).arg(reply->errorString());
    qDebug() << errorMsg;
    emit networkError(errorMsg);
}

void NetworkLeaderboardManager::onSslErrors(const QList<QSslError> &errors)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;
    
    qDebug() << "SSL Errors occurred:";
    for (const QSslError &error : errors) {
        qDebug() << "  -" << error.errorString();
    }
    
    // WARNING: In production, DO NOT ignore SSL errors blindly.
    // This is only for development/testing with self-signed certificates.
    // In production:
    // 1. Validate the certificate is the expected one
    // 2. Check specific error types (e.g., self-signed certificates)
    // 3. Prompt user for confirmation before ignoring errors
    // 4. Use proper CA-signed certificates
    
    // For now, only ignore for localhost/development
    QUrl url = reply->url();
    if (url.host() == "localhost" || url.host() == "127.0.0.1") {
        qDebug() << "Ignoring SSL errors for localhost";
        reply->ignoreSslErrors();
    } else {
        qDebug() << "SSL errors NOT ignored for production server";
        emit networkError("SSL certificate verification failed");
    }
}

QString NetworkLeaderboardManager::calculateChecksum(const QJsonObject &data) const
{
    // Create a basic checksum for data validation
    // NOTE: This is NOT secure for production use!
    // In production:
    // 1. Use server-side HMAC validation with a secret key that's not in the client
    // 2. Implement proper authentication (OAuth, JWT, API keys)
    // 3. Server must validate all scores for reasonableness
    // 4. Add timestamp and nonce to prevent replay attacks
    // 
    // This client-side checksum is only a deterrent, not real security.
    
    QJsonDocument doc(data);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
    
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(jsonData);
    // Using a weak "salt" here - in production, server should verify with HMAC
    hash.addData("PVZ_CLIENT_VALIDATION");
    
    return QString(hash.result().toHex());
}

bool NetworkLeaderboardManager::verifyChecksum(const QJsonObject &data, const QString &checksum) const
{
    QString calculated = calculateChecksum(data);
    return calculated == checksum;
}
