# 線上排行榜快速開始指南 (Online Leaderboard Quick Start)

## 功能概述 (Feature Overview)

本專案實現了完整的線上排行榜系統，包含：
- 🌐 網路 API 客戶端（使用 Qt QNetworkAccessManager）
- 📊 本地與線上雙模式排行榜介面
- 🔒 基本安全措施（校驗碼、速率限制）
- 🧪 Python 測試伺服器

This project implements a complete online leaderboard system, including:
- 🌐 Network API client (using Qt QNetworkAccessManager)
- 📊 Local and online dual-mode leaderboard interface
- 🔒 Basic security measures (checksum, rate limiting)
- 🧪 Python test server

---

## 快速測試 (Quick Test)

### 步驟 1: 啟動測試伺服器 (Step 1: Start Test Server)

```bash
cd /path/to/QT_Project_Plants_vs_Zombies_absver_1201
python3 mock_leaderboard_server.py
```

伺服器將在 `http://localhost:8080/api` 啟動。

Server will start at `http://localhost:8080/api`.

### 步驟 2: 測試 API (Step 2: Test API)

**提交分數測試 (Submit Score Test):**
```bash
curl -X POST http://localhost:8080/api/submitScore \
  -H "Content-Type: application/json" \
  -d '{
    "playerName": "TestPlayer",
    "levelId": 1,
    "wavesSurvived": 3,
    "zombiesKilled": 50,
    "plantsPlaced": 20,
    "totalSunCollected": 2000,
    "playTimeSeconds": 300,
    "date": "2024-01-03T15:30:00",
    "isWin": true,
    "score": 2000
  }'
```

**獲取排行榜測試 (Get Leaderboard Test):**
```bash
curl http://localhost:8080/api/leaderboard
```

**按關卡獲取 (Get by Level):**
```bash
curl http://localhost:8080/api/leaderboard?levelId=1
```

### 步驟 3: 編譯遊戲 (Step 3: Build Game)

```bash
cd PVZ
qmake Qt_pvz.pro
make
```

### 步驟 4: 執行遊戲 (Step 4: Run Game)

1. 確保測試伺服器正在運行
2. 啟動遊戲
3. 玩一局遊戲直到結束
4. 輸入玩家名稱提交分數
5. 查看主選單的排行榜

Ensure test server is running, start the game, play until end, submit score, view leaderboard.

---

## 配置說明 (Configuration)

### 伺服器 URL 設定 (Server URL Configuration)

編輯 `PVZ/network_config.json`:
```json
{
  "serverUrl": "http://localhost:8080/api",
  "networkEnabled": true,
  "requestTimeout": 10000,
  "rateLimit": 1000
}
```

或在代碼中設定 (Or set in code):
```cpp
NetworkLeaderboardManager::instance().setServerUrl("http://your-server.com/api");
```

### 停用網路功能 (Disable Network)

```cpp
NetworkLeaderboardManager::instance().setNetworkEnabled(false);
```

---

## API 端點說明 (API Endpoints)

### POST /api/submitScore

提交玩家分數到伺服器。

**請求格式:**
```json
{
  "playerName": "string",
  "levelId": number,
  "wavesSurvived": number,
  "zombiesKilled": number,
  "plantsPlaced": number,
  "totalSunCollected": number,
  "playTimeSeconds": number,
  "date": "ISO8601 string",
  "isWin": boolean,
  "score": number,
  "checksum": "string"
}
```

**回應:**
```json
{
  "success": boolean,
  "message": "string",
  "rank": number,
  "totalScores": number
}
```

### GET /api/leaderboard

獲取排行榜資料。

**查詢參數:**
- `levelId` (optional): 關卡編號，0或不提供表示全部關卡

**回應:**
```json
{
  "version": "1.0",
  "scores": [PlayerScore],
  "totalCount": number,
  "levelId": "all" | number
}
```

---

## 架構說明 (Architecture)

```
┌─────────────────────────────────────────────────┐
│                   Qt Game Client                │
├─────────────────────────────────────────────────┤
│  GameScene          │  LeaderboardDialog        │
│  - 追蹤遊戲統計      │  - 本地排行榜             │
│  - 提交分數          │  - 線上排行榜             │
└──────────┬──────────┴───────────┬───────────────┘
           │                      │
           ▼                      ▼
┌─────────────────────┐  ┌─────────────────────┐
│ LeaderboardManager  │  │NetworkLeaderboard   │
│ - 本地 JSON 存儲     │  │Manager              │
│ - 分數計算           │  │- HTTP 請求           │
│                     │  │- JSON 處理           │
└─────────────────────┘  └──────────┬───────────┘
                                    │
                         HTTP/HTTPS │
                                    ▼
                         ┌──────────────────────┐
                         │   Server (REST API)  │
                         │  - POST /submitScore │
                         │  - GET /leaderboard  │
                         └──────────┬───────────┘
                                    │
                                    ▼
                         ┌──────────────────────┐
                         │   Database           │
                         │   (MySQL/SQLite/etc) │
                         └──────────────────────┘
```

---

## 安全考量 (Security Considerations)

### 目前實現 (Current Implementation)

1. ✅ **SHA256 校驗碼** - 基本的資料完整性驗證
2. ✅ **速率限制** - 客戶端每秒最多 1 次請求
3. ✅ **請求超時** - 10 秒超時保護
4. ✅ **SSL/TLS 支援** - 可使用 HTTPS

### 生產環境建議 (Production Recommendations)

1. ⚠️ **HMAC 認證** - 使用伺服器共享密鑰
2. ⚠️ **API 金鑰** - 每個客戶端唯一識別
3. ⚠️ **防重放攻擊** - 添加時間戳和 nonce
4. ⚠️ **伺服器端驗證** - 驗證所有分數合理性
5. ⚠️ **IP 速率限制** - 伺服器端限流

---

## 故障排除 (Troubleshooting)

### Q1: 無法連接伺服器

**檢查清單:**
- [ ] 伺服器是否正在運行？
- [ ] URL 設定是否正確？
- [ ] 防火牆是否阻擋？
- [ ] 網路連接是否正常？

### Q2: 編譯錯誤

**可能原因:**
- Qt network 模組未安裝
- Qt 版本不相容

**解決方案:**
```bash
# Ubuntu/Debian
sudo apt-get install qt5-network-dev

# macOS
brew install qt5
```

### Q3: 分數未顯示在線上排行榜

**檢查:**
1. 查看伺服器日誌
2. 使用 curl 測試 API
3. 檢查網路請求是否成功
4. 驗證 JSON 格式

---

## 開發與部署 (Development & Deployment)

### 本地開發 (Local Development)

1. 使用 `mock_leaderboard_server.py` 進行測試
2. 在 `network_config.json` 設定為 `localhost`
3. 啟用詳細日誌記錄

### 部署到生產環境 (Production Deployment)

1. **設定真實伺服器:**
   - 部署完整的 RESTful API
   - 連接到持久化資料庫
   - 配置 HTTPS

2. **更新客戶端配置:**
   ```json
   {
     "serverUrl": "https://api.yourserver.com/pvz",
     "networkEnabled": true,
     "requestTimeout": 15000
   }
   ```

3. **監控與維護:**
   - 設定日誌系統
   - 監控 API 效能
   - 定期備份資料庫

---

## 相關文件 (Related Documentation)

- `ONLINE_LEADERBOARD_GUIDE.md` - 詳細技術文檔
- `LEADERBOARD_README.md` - 本地排行榜功能說明
- `LEADERBOARD_SUMMARY.md` - 功能摘要
- `LEADERBOARD_TEST_PLAN.md` - 測試計劃

---

## 貢獻 (Contributing)

歡迎提交 Pull Request 或 Issue！

Welcome to submit Pull Requests or Issues!

---

**版本:** 1.0  
**日期:** 2024-01-03  
**維護者:** GitHub Copilot Agent
