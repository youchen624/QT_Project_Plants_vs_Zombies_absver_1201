#include "leaderboardmanager.h"
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <algorithm>

LeaderboardManager& LeaderboardManager::instance()
{
    static LeaderboardManager instance;
    return instance;
}

LeaderboardManager::LeaderboardManager()
{
    loadFromFile();
}

LeaderboardManager::~LeaderboardManager()
{
    saveToFile();
}

void LeaderboardManager::addScore(const PlayerScore &score)
{
    m_scores.append(score);
    
    // Sort by score in descending order
    std::sort(m_scores.begin(), m_scores.end(), [](const PlayerScore &a, const PlayerScore &b) {
        return a.calculateScore() > b.calculateScore();
    });
    
    // Keep only top 100 scores to prevent file from growing too large
    if (m_scores.size() > 100) {
        m_scores.resize(100);
    }
    
    saveToFile();
}

QVector<PlayerScore> LeaderboardManager::getTopScores(int count) const
{
    QVector<PlayerScore> topScores;
    int limit = qMin(count, m_scores.size());
    for (int i = 0; i < limit; i++) {
        topScores.append(m_scores[i]);
    }
    return topScores;
}

QVector<PlayerScore> LeaderboardManager::getScoresByLevel(int levelId, int count) const
{
    QVector<PlayerScore> levelScores;
    for (const PlayerScore &score : m_scores) {
        if (score.levelId == levelId) {
            levelScores.append(score);
        }
    }
    
    // Sort by score
    std::sort(levelScores.begin(), levelScores.end(), [](const PlayerScore &a, const PlayerScore &b) {
        return a.calculateScore() > b.calculateScore();
    });
    
    // Return top count
    int limit = qMin(count, levelScores.size());
    return levelScores.mid(0, limit);
}

QVector<PlayerScore> LeaderboardManager::getAllScores() const
{
    return m_scores;
}

QString LeaderboardManager::getFilePath() const
{
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(appDataPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    return appDataPath + "/leaderboard.json";
}

bool LeaderboardManager::saveToFile()
{
    QString filePath = getFilePath();
    QFile file(filePath);
    
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open leaderboard file for writing:" << filePath;
        return false;
    }
    
    QJsonArray scoresArray;
    for (const PlayerScore &score : m_scores) {
        scoresArray.append(score.toJson());
    }
    
    QJsonObject root;
    root["scores"] = scoresArray;
    root["version"] = "1.0";
    
    QJsonDocument doc(root);
    file.write(doc.toJson());
    file.close();
    
    qDebug() << "Leaderboard saved to:" << filePath;
    return true;
}

bool LeaderboardManager::loadFromFile()
{
    QString filePath = getFilePath();
    QFile file(filePath);
    
    if (!file.exists()) {
        qDebug() << "Leaderboard file does not exist yet:" << filePath;
        return true; // Not an error, just no data yet
    }
    
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open leaderboard file for reading:" << filePath;
        return false;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        qDebug() << "Invalid JSON in leaderboard file";
        return false;
    }
    
    QJsonObject root = doc.object();
    QJsonArray scoresArray = root["scores"].toArray();
    
    m_scores.clear();
    for (const QJsonValue &value : scoresArray) {
        if (value.isObject()) {
            m_scores.append(PlayerScore::fromJson(value.toObject()));
        }
    }
    
    qDebug() << "Loaded" << m_scores.size() << "scores from leaderboard";
    return true;
}
