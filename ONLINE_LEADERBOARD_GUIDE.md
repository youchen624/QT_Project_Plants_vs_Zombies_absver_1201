# 線上排行榜功能文檔 (Online Leaderboard Feature Documentation)

## 概述 (Overview)

本專案已實現完整的線上排行榜功能，在原有本地排行榜基礎上新增網路功能，使玩家能夠透過伺服器提交和查看全球排名。

This project now includes a complete online leaderboard feature, adding network capabilities to the existing local leaderboard, allowing players to submit scores and view global rankings through a server.

---

## 新增功能 (New Features)

### 1. 網路排行榜管理器 (Network Leaderboard Manager)

**檔案 (Files):**
- `PVZ/networkleaderboardmanager.h`
- `PVZ/networkleaderboardmanager.cpp`

**功能 (Features):**
- ✅ 使用 Qt QNetworkAccessManager 實現 HTTP 通訊
- ✅ POST /submitScore API - 提交分數到伺服器
- ✅ GET /leaderboard API - 從伺服器獲取排行榜
- ✅ JSON 格式資料傳輸
- ✅ 請求超時處理 (10秒)
- ✅ 錯誤處理與重試機制
- ✅ SSL 錯誤處理
- ✅ 速率限制 (每秒最多1次請求)
- ✅ 分數校驗碼 (SHA256 checksum)

### 2. 雙模式排行榜介面 (Dual-Mode Leaderboard UI)

**更新檔案 (Updated Files):**
- `PVZ/leaderboarddialog.h`
- `PVZ/leaderboarddialog.cpp`

**功能 (Features):**
- ✅ Tab 切換：本地排行榜 vs 線上排行榜
- ✅ 網路狀態顯示
- ✅ 載入指示器
- ✅ 錯誤訊息顯示
- ✅ 自動刷新功能
- ✅ 關卡篩選支援線上模式

### 3. 自動分數提交 (Automatic Score Submission)

**更新檔案 (Updated Files):**
- `PVZ/gamescene.cpp`

**功能 (Features):**
- ✅ 遊戲結束時同時提交到本地和線上
- ✅ 網路失敗時自動降級為本地模式
- ✅ 非阻塞式提交（異步操作）

### 4. 測試伺服器 (Test Server)

**新檔案 (New File):**
- `mock_leaderboard_server.py`

**功能 (Features):**
- ✅ Python 3 HTTP 伺服器
- ✅ RESTful API 端點實現
- ✅ 記憶體內排行榜存儲
- ✅ CORS 支援
- ✅ 完整日誌記錄

---

## 技術實現 (Technical Implementation)

### API 規格 (API Specification)

#### 1. 提交分數 (Submit Score)

**端點 (Endpoint):** `POST /api/submitScore`

**請求格式 (Request Format):**
```json
{
  "playerName": "Player1",
  "levelId": 5,
  "wavesSurvived": 3,
  "zombiesKilled": 68,
  "plantsPlaced": 25,
  "totalSunCollected": 3200,
  "playTimeSeconds": 375,
  "date": "2024-01-03T15:30:00",
  "isWin": true,
  "score": 2840,
  "checksum": "abc123..."
}
```

**回應格式 (Response Format):**
```json
{
  "success": true,
  "message": "Score submitted successfully",
  "rank": 15,
  "totalScores": 100
}
```

#### 2. 獲取排行榜 (Get Leaderboard)

**端點 (Endpoint):** `GET /api/leaderboard?levelId={N}`

**參數 (Parameters):**
- `levelId` (optional): 關卡編號，0或不提供表示全部關卡

**回應格式 (Response Format):**
```json
{
  "version": "1.0",
  "scores": [
    {
      "playerName": "Player1",
      "levelId": 5,
      "score": 2840,
      ...
    }
  ],
  "totalCount": 100,
  "levelId": "all"
}
```

### 安全機制 (Security Measures)

1. **分數校驗碼 (Score Checksum)**
   - 使用 SHA256 雜湊
   - 包含遊戲資料和密鑰
   - 防止基本的分數竄改

2. **速率限制 (Rate Limiting)**
   - 客戶端：每秒最多1次請求
   - 防止濫用和 DDoS

3. **請求超時 (Request Timeout)**
   - 10秒超時限制
   - 避免無限期等待

4. **SSL/TLS 支援 (SSL/TLS Support)**
   - 支援 HTTPS 連接
   - SSL 錯誤處理

### 錯誤處理 (Error Handling)

1. **網路錯誤 (Network Errors)**
   - 連接失敗：顯示錯誤訊息
   - 超時：自動重試或降級到本地模式
   - DNS 錯誤：提示檢查網路連接

2. **伺服器錯誤 (Server Errors)**
   - 400 Bad Request：提示資料格式錯誤
   - 404 Not Found：提示 API 端點不存在
   - 500 Internal Server Error：提示伺服器故障

3. **資料驗證 (Data Validation)**
   - JSON 解析錯誤處理
   - 必填欄位檢查
   - 資料類型驗證

---

## 使用說明 (Usage Instructions)

### 1. 配置伺服器 URL (Configure Server URL)

預設伺服器 URL：`http://localhost:8080/api`

**在代碼中更改：**
```cpp
// 在應用啟動時設置
NetworkLeaderboardManager::instance().setServerUrl("https://your-server.com/api");
```

### 2. 啟動測試伺服器 (Start Test Server)

```bash
cd /path/to/project
python3 mock_leaderboard_server.py
```

伺服器將在 `http://localhost:8080` 啟動。

### 3. 執行遊戲 (Run the Game)

1. 編譯並執行遊戲
2. 玩一局遊戲直到結束
3. 輸入玩家名稱並提交分數
4. 從主選單打開排行榜
5. 切換到「線上排行榜」標籤查看線上排名

### 4. 查看線上排行榜 (View Online Leaderboard)

1. 點擊主選單的「排行榜」按鈕
2. 切換到「🌐 線上排行榜 (Online)」標籤
3. 選擇關卡篩選（可選）
4. 點擊「🔄 重新整理」更新資料

---

## 設定選項 (Configuration Options)

### 啟用/停用網路功能 (Enable/Disable Network)

```cpp
// 停用網路排行榜
NetworkLeaderboardManager::instance().setNetworkEnabled(false);

// 啟用網路排行榜
NetworkLeaderboardManager::instance().setNetworkEnabled(true);
```

### 變更伺服器 URL (Change Server URL)

```cpp
NetworkLeaderboardManager::instance().setServerUrl("https://example.com/api");
```

---

## 部署指南 (Deployment Guide)

### 伺服器端需求 (Server Requirements)

1. **HTTP 伺服器**
   - 支援 RESTful API
   - JSON 格式處理
   - CORS 支援（如果需要跨域）

2. **資料庫**
   - MySQL / PostgreSQL / SQLite
   - 或 NoSQL (MongoDB, Redis)

3. **API 端點**
   - `POST /api/submitScore`
   - `GET /api/leaderboard`

### 生產環境建議 (Production Recommendations)

1. **使用 HTTPS**
   - 加密資料傳輸
   - 防止中間人攻擊

2. **實現完整驗證**
   - HMAC 簽名
   - API 金鑰認證
   - 防止重放攻擊

3. **資料庫索引**
   - 分數欄位建立索引
   - 關卡 ID 欄位建立索引
   - 優化查詢效能

4. **快取機制**
   - Redis 快取熱門排行榜
   - 減少資料庫負載

5. **監控與日誌**
   - 記錄所有 API 請求
   - 監控異常活動
   - 效能監控

---

## 測試指南 (Testing Guide)

### 單元測試 (Unit Testing)

```cpp
// 測試網路管理器
void testSubmitScore() {
    PlayerScore score;
    score.playerName = "TestPlayer";
    score.levelId = 1;
    score.score = 1000;
    
    NetworkLeaderboardManager::instance().submitScore(score);
    // 驗證信號發送
}

void testFetchLeaderboard() {
    NetworkLeaderboardManager::instance().fetchLeaderboard(1);
    // 驗證回應處理
}
```

### 整合測試 (Integration Testing)

1. 啟動測試伺服器
2. 執行遊戲完整流程
3. 驗證分數提交
4. 驗證排行榜顯示
5. 測試網路錯誤處理

### 手動測試檢查清單 (Manual Test Checklist)

- [ ] 分數提交到線上伺服器
- [ ] 線上排行榜正確顯示
- [ ] 關卡篩選功能運作
- [ ] 網路錯誤正確處理
- [ ] 本地模式降級運作
- [ ] 重新整理功能運作
- [ ] Tab 切換功能正常
- [ ] 狀態訊息正確顯示

---

## 故障排除 (Troubleshooting)

### 問題：無法連接伺服器

**解決方案：**
1. 檢查伺服器是否運行
2. 驗證伺服器 URL 配置
3. 檢查防火牆設定
4. 檢查網路連接

### 問題：分數未顯示

**解決方案：**
1. 檢查伺服器日誌
2. 驗證 API 端點
3. 檢查回應格式
4. 測試 API 端點（使用 curl 或 Postman）

### 問題：SSL 錯誤

**解決方案：**
1. 更新 Qt 網路模組
2. 安裝 SSL 憑證
3. 驗證伺服器憑證

---

## 專案檔案變更摘要 (Project File Changes Summary)

### 新增檔案 (New Files)
1. `PVZ/networkleaderboardmanager.h` - 網路管理器標頭檔
2. `PVZ/networkleaderboardmanager.cpp` - 網路管理器實現
3. `mock_leaderboard_server.py` - 測試伺服器
4. `ONLINE_LEADERBOARD_GUIDE.md` - 本文檔

### 修改檔案 (Modified Files)
1. `PVZ/Qt_pvz.pro` - 添加 network 模組和新檔案
2. `PVZ/gamescene.cpp` - 添加線上分數提交
3. `PVZ/leaderboarddialog.h` - 添加 Tab 和網路功能
4. `PVZ/leaderboarddialog.cpp` - 實現雙模式介面

### 依賴更新 (Dependencies Update)
- 添加 `QT += network` 到專案檔

---

## 後續改進建議 (Future Improvements)

### 短期 (Short-term)
1. 添加使用者認證系統
2. 實現好友排行榜
3. 添加排行榜搜尋功能
4. 支援多語言 API 回應

### 中期 (Medium-term)
1. 每日/每週排行榜重置
2. 成就系統整合
3. 社群功能（留言、點讚）
4. 排行榜分享功能

### 長期 (Long-term)
1. 即時排名更新（WebSocket）
2. 多區域伺服器支援
3. 遊戲重播功能
4. 詳細統計圖表與分析

---

## 聯絡資訊 (Contact)

如有問題或建議，請透過 GitHub Issues 回報。

For questions or suggestions, please report via GitHub Issues.

---

**文檔版本：** 1.0  
**最後更新：** 2024-01-03  
**作者：** GitHub Copilot Agent
