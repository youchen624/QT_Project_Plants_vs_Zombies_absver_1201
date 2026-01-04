#ifndef LEADERBOARDMANAGER_H
#define LEADERBOARDMANAGER_H

#include <QString>
#include <QVector>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

struct PlayerScore {
    QString playerName;
    int levelId;
    int wavesSurvived;
    int zombiesKilled;
    int plantsPlaced;
    int totalSunCollected;
    qint64 playTimeSeconds;
    QDateTime date;
    bool isWin;
    
    // Calculate a score based on multiple factors
    int calculateScore() const {
        int score = 0;
        score += wavesSurvived * 100;
        score += zombiesKilled * 10;
        score += plantsPlaced * 5;
        score += totalSunCollected / 10;
        if (isWin) {
            score += 1000;
            // Bonus for faster completion
            if (playTimeSeconds < 300) score += 500;
            else if (playTimeSeconds < 600) score += 300;
        }
        return score;
    }
    
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["playerName"] = playerName;
        obj["levelId"] = levelId;
        obj["wavesSurvived"] = wavesSurvived;
        obj["zombiesKilled"] = zombiesKilled;
        obj["plantsPlaced"] = plantsPlaced;
        obj["totalSunCollected"] = totalSunCollected;
        obj["playTimeSeconds"] = static_cast<qint64>(playTimeSeconds);
        obj["date"] = date.toString(Qt::ISODate);
        obj["isWin"] = isWin;
        obj["score"] = calculateScore();
        return obj;
    }
    
    static PlayerScore fromJson(const QJsonObject &obj) {
        PlayerScore score;
        score.playerName = obj["playerName"].toString();
        score.levelId = obj["levelId"].toInt();
        score.wavesSurvived = obj["wavesSurvived"].toInt();
        score.zombiesKilled = obj["zombiesKilled"].toInt();
        score.plantsPlaced = obj["plantsPlaced"].toInt();
        score.totalSunCollected = obj["totalSunCollected"].toInt();
        score.playTimeSeconds = obj["playTimeSeconds"].toVariant().toLongLong();
        score.date = QDateTime::fromString(obj["date"].toString(), Qt::ISODate);
        score.isWin = obj["isWin"].toBool();
        return score;
    }
};

class LeaderboardManager
{
public:
    static LeaderboardManager& instance();
    
    void addScore(const PlayerScore &score);
    QVector<PlayerScore> getTopScores(int count = 10) const;
    QVector<PlayerScore> getScoresByLevel(int levelId, int count = 10) const;
    QVector<PlayerScore> getAllScores() const;
    
    bool saveToFile();
    bool loadFromFile();
    
private:
    LeaderboardManager();
    ~LeaderboardManager();
    LeaderboardManager(const LeaderboardManager&) = delete;
    LeaderboardManager& operator=(const LeaderboardManager&) = delete;
    
    QString getFilePath() const;
    
    QVector<PlayerScore> m_scores;
};

#endif // LEADERBOARDMANAGER_H
