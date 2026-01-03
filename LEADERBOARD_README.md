# 排行榜功能 (Leaderboard Feature)

## 功能概述 (Overview)

本項目新增了排行榜功能，可以記錄和查看不同玩家的遊戲戰績。

The leaderboard feature allows players to record and view game statistics and rankings.

## 功能特點 (Features)

### 遊戲統計追蹤 (Game Statistics Tracking)
- 玩家名稱 (Player Name)
- 關卡編號 (Level ID)
- 勝利/失敗狀態 (Win/Loss Status)
- 存活波數 (Waves Survived)
- 擊殺殭屍數 (Zombies Killed)
- 種植植物數 (Plants Placed)
- 收集陽光總數 (Total Sun Collected)
- 遊戲時長 (Play Time)
- 遊戲日期時間 (Date/Time)

### 分數計算 (Score Calculation)
分數基於多個因素計算：
- 存活波數 × 100
- 擊殺殭屍 × 10
- 種植植物 × 5
- 收集陽光 ÷ 10
- 勝利獎勵：+1000
- 速通獎勵（<5分鐘）：+500
- 速通獎勵（<10分鐘）：+300

Score is calculated based on multiple factors:
- Waves survived × 100
- Zombies killed × 10
- Plants placed × 5
- Sun collected ÷ 10
- Win bonus: +1000
- Speed bonus (<5 min): +500
- Speed bonus (<10 min): +300

### 排行榜界面 (Leaderboard UI)
- 查看全部關卡排行 (View all levels)
- 按關卡篩選 (Filter by level)
- 顯示前50名 (Top 50 rankings)
- 彩色排名標記（金、銀、銅） (Colored rank badges)
- 重新整理功能 (Refresh functionality)

### 數據持久化 (Data Persistence)
- 使用JSON格式儲存 (Stored in JSON format)
- 自動保存到用戶應用數據目錄 (Auto-saved to user AppData folder)
- 最多保存100條記錄 (Maximum 100 records stored)

## 使用方法 (Usage)

### 查看排行榜 (View Leaderboard)
1. 從主選單點擊「排行榜」按鈕 (Click "Leaderboard" button from main menu)
2. 使用下拉選單篩選特定關卡 (Use dropdown to filter specific levels)
3. 點擊「重新整理」更新排行榜 (Click "Refresh" to reload)

### 提交分數 (Submit Score)
1. 完成遊戲（勝利或失敗） (Complete a game - win or lose)
2. 輸入玩家名稱（最多20字符） (Enter player name - max 20 characters)
3. 點擊「確定」提交，或「跳過」不記錄 (Click "OK" to submit or "Skip" to ignore)

## 技術實現 (Technical Implementation)

### 新增檔案 (New Files)
- `leaderboardmanager.h/cpp` - 排行榜管理器（單例模式）
- `leaderboarddialog.h/cpp` - 排行榜顯示界面
- `playernamedialog.h/cpp` - 玩家名稱輸入對話框

### 修改檔案 (Modified Files)
- `gamescene.h/cpp` - 添加統計追蹤和分數提交
- `mainmenu.h/cpp` - 添加排行榜按鈕
- `qvz.h/cpp` - 連接排行榜功能
- `Qt_pvz.pro` - 更新專案配置

### 數據存儲位置 (Data Storage Location)
排行榜數據儲存在：
`%APPDATA%/[AppName]/leaderboard.json` (Windows)
`~/.local/share/[AppName]/leaderboard.json` (Linux)
`~/Library/Application Support/[AppName]/leaderboard.json` (macOS)

## 未來改進建議 (Future Improvements)
- 在線排行榜（需要伺服器） (Online leaderboard - requires server)
- 好友排行榜 (Friends leaderboard)
- 每日/每週排行榜 (Daily/Weekly rankings)
- 成就系統整合 (Achievement system integration)
- 更多統計圖表 (More statistical charts)
