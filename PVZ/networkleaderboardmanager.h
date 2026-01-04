#ifndef NETWORKLEADERBOARDMANAGER_H
#define NETWORKLEADERBOARDMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "leaderboardmanager.h"

class NetworkLeaderboardManager : public QObject
{
    Q_OBJECT
public:
    static NetworkLeaderboardManager& instance();
    
    // Submit score to online server
    void submitScore(const PlayerScore &score);
    
    // Fetch leaderboard from online server
    void fetchLeaderboard(int levelId = 0); // 0 = all levels
    
    // Set server URL (default is mock server for testing)
    void setServerUrl(const QString &url);
    QString getServerUrl() const { return m_serverUrl; }
    
    // Check if network is enabled
    bool isNetworkEnabled() const { return m_networkEnabled; }
    void setNetworkEnabled(bool enabled) { m_networkEnabled = enabled; }
    
signals:
    void scoreSubmitted(bool success, const QString &message);
    void leaderboardReceived(const QVector<PlayerScore> &scores);
    void networkError(const QString &error);
    
private slots:
    void onSubmitScoreFinished();
    void onFetchLeaderboardFinished();
    void onNetworkError(QNetworkReply::NetworkError error);
    void onSslErrors(const QList<QSslError> &errors);
    
private:
    NetworkLeaderboardManager();
    ~NetworkLeaderboardManager();
    NetworkLeaderboardManager(const NetworkLeaderboardManager&) = delete;
    NetworkLeaderboardManager& operator=(const NetworkLeaderboardManager&) = delete;
    
    QString calculateChecksum(const QJsonObject &data) const;
    bool verifyChecksum(const QJsonObject &data, const QString &checksum) const;
    
    QNetworkAccessManager *m_networkManager;
    QString m_serverUrl;
    bool m_networkEnabled;
    qint64 m_lastRequestTime;
    static const int MIN_REQUEST_INTERVAL_MS = 1000; // Rate limiting: 1 request per second
};

#endif // NETWORKLEADERBOARDMANAGER_H
