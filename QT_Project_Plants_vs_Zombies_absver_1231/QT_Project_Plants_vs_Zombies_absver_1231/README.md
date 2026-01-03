# Plants vs. Zombies - QT Project

這是一個使用QT框架實現的植物大戰殭屍遊戲專案。

## 專案結構

```
QT_Project_Plants_vs_Zombies_absver_1231/
├── entities/              # 基礎實體類別
│   ├── Plant.h/cpp       # 植物基類
│   └── Zombie.h/cpp      # 殭屍基類
├── plants/               # 植物實現
│   ├── Sunflower.h/cpp   # 太陽花
│   └── Peashooter.h/cpp  # 豌豆射手
├── zombies/              # 殭屍實現
│   └── NormalZombie.h/cpp # 普通殭屍
├── game/                 # 遊戲邏輯
│   ├── Game.h/cpp        # 遊戲控制器
│   └── PlantCell.h/cpp   # 植物格子
├── ui/                   # 用戶界面
│   └── mainwindow.h/cpp  # 主窗口
├── resources/            # 資源文件
│   └── images/          # 圖像資源
│       ├── sunflower.png
│       ├── peashooter.png
│       └── zombie.png
├── main.cpp              # 程序入口
├── resources.qrc         # Qt資源文件
└── *.pro                 # 專案文件
```

### 基礎類別 (Base Classes)

#### 1. Plant (植物基類)
**檔案**: `entities/Plant.h`, `entities/Plant.cpp`

植物的抽象基類，所有植物都繼承自此類。

**主要功能**:
- 管理植物的生命值 (health)
- 追蹤植物在網格中的位置 (position)
- 管理種植成本 (cost) 和冷卻時間 (cooldown)
- **圖像管理**: 支持PNG圖像加載和顯示
- 提供虛擬方法供子類實現：
  - `update(int tickCount)`: 每個tick更新植物狀態
  - `performAction()`: 執行植物特定動作
  - `getType()`: 返回植物類型

**信號 (Signals)**:
- `died()`: 當植物死亡時發出
- `actionPerformed()`: 當植物執行動作時發出
- `imageChanged()`: 當圖像變更時發出

#### 2. Zombie (殭屍基類)
**檔案**: `entities/Zombie.h`, `entities/Zombie.cpp`

殭屍的抽象基類，所有殭屍都繼承自此類。

**主要功能**:
- 管理殭屍的生命值 (health)
- 追蹤殭屍的位置 (position)
- 管理移動速度 (speed) 和攻擊傷害 (attackDamage)
- **圖像管理**: 支持PNG圖像加載和顯示
- 提供虛擬方法供子類實現：
  - `update(int tickCount)`: 每個tick更新殭屍狀態
  - `move()`: 移動殭屍
  - `attack()`: 攻擊植物
  - `getType()`: 返回殭屍類型

**信號 (Signals)**:
- `died()`: 當殭屍死亡時發出
- `reachedEnd()`: 當殭屍到達終點時發出
- `imageChanged()`: 當圖像變更時發出

#### 3. PlantCell (植物格子類)
**檔案**: `game/PlantCell.h`, `game/PlantCell.cpp`

代表遊戲網格中的一個格子，用於管理植物的放置。

**主要功能**:
- 管理格子的行列位置 (row, col)
- 追蹤格子是否被佔用 (occupied)
- 提供植物放置和移除功能
- 自動處理植物死亡事件

**方法**:
- `placePlant(Plant*)`: 在格子中放置植物
- `removePlant()`: 移除格子中的植物
- `update(int tickCount)`: 更新格子中的植物

#### 4. Game (遊戲類)
**檔案**: `Game.h`, `Game.cpp`

核心遊戲類，管理遊戲狀態和tick-based邏輯。

**主要功能**:
- 使用QTimer實現tick-based遊戲循環 (30 ticks/秒)
- 管理5x9的植物網格系統
- 管理陽光資源系統 (初始150點陽光)
- 處理植物和殭屍的生成、更新和碰撞
- 提供遊戲控制 (start, stop, pause, resume)

**遊戲循環**:
1. 更新所有植物
2. 更新所有殭屍
3. 檢查碰撞
4. 清理死亡實體

### 實現的植物

#### 1. Sunflower (太陽花)
**檔案**: `Sunflower.h`, `Sunflower.cpp`

**屬性**:
- 成本: 50陽光
- 生命值: 300
- 陽光產生間隔: 240 ticks (約8秒)
- 每次產生陽光: 25點

**功能**:
定期產生陽光資源，是遊戲中的資源生產單位。

#### 2. Peashooter (豌豆射手)
**檔案**: `Peashooter.h`, `Peashooter.cpp`

**屬性**:
- 成本: 100陽光
- 生命值: 300
- 射擊間隔: 90 ticks (約3秒)
- 每發豌豆傷害: 20
- 射程: 10格

**功能**:
定期向同一行的殭屍發射豌豆，是基礎的攻擊單位。

### 實現的殭屍

#### 1. NormalZombie (普通殭屍)
**檔案**: `NormalZombie.h`, `NormalZombie.cpp`

**屬性**:
- 生命值: 200
- 移動速度: 1像素/tick
- 攻擊傷害: 20
- 攻擊冷卻: 30 ticks (約1秒)

**行為**:
1. 從右側進入遊戲場景
2. 持續向左移動
3. 遇到植物時停止並攻擊
4. 到達左邊界時遊戲結束

## 遊戲機制

### Tick系統
- 遊戲使用tick-based更新系統
- 默認tick rate: 30 ticks/秒
- 所有時間相關的邏輯都基於tick計數

### 網格系統
- 5行 x 9列的遊戲網格
- 每個格子可以放置一個植物
- 格子負責管理其中的植物生命週期

### 資源系統
- 使用陽光作為遊戲貨幣
- 初始陽光: 150點
- 通過太陽花產生更多陽光
- 種植植物需要消耗陽光

### 圖像系統
- 所有遊戲物件都支持PNG圖像
- 圖像通過Qt資源系統 (QRC) 管理
- 每個實體都有 `getImage()` 和 `setImage()` 方法
- 支持透明度 (RGBA)
- 默認圖像路徑：
  - 太陽花: `:/images/sunflower.png`
  - 豌豆射手: `:/images/peashooter.png`
  - 殭屍: `:/images/zombie.png`

### 碰撞檢測
- 遊戲自動檢測殭屍與植物的碰撞
- 殭屍到達植物位置時會停止並攻擊
- 植物死亡後殭屍繼續前進

## 使用方法

### 編譯專案

需要QT開發環境 (QT5或QT6):

```bash
qmake QT_Project_Plants_vs_Zombies_absver_1231.pro
make
```

### 運行程式

```bash
./QT_Project_Plants_vs_Zombies_absver_1231
```

### 測試遊戲邏輯

專案包含一個測試程式 `test_game.cpp`，可以驗證基礎遊戲邏輯。

## MainWindow集成

MainWindow提供簡單的UI界面：

**控制按鈕**:
- Start Game: 開始遊戲
- Pause/Resume: 暫停/恢復遊戲
- Test: Place Plants: 測試放置植物
- Test: Spawn Zombies: 測試生成殭屍

**狀態顯示**:
- 遊戲狀態
- 當前陽光數量
- 當前tick計數

## 擴展性

### 添加新植物
1. 創建繼承自`Plant`的新類
2. 實現虛擬方法: `update()`, `performAction()`, `getType()`
3. 在構造函數中設置植物屬性
4. 根據需要發出信號

### 添加新殭屍
1. 創建繼承自`Zombie`的新類
2. 實現虛擬方法: `update()`, `move()`, `attack()`, `getType()`
3. 在構造函數中設置殭屍屬性
4. 根據需要發出信號

## 技術特點

- **面向對象設計**: 使用繼承和多態實現可擴展的遊戲實體系統
- **信號槽機制**: 充分利用QT的信號槽機制實現事件通信
- **Tick-based更新**: 使用固定時間步長的遊戲循環確保一致性
- **網格管理**: 智能的網格系統管理植物放置和碰撞檢測
- **資源管理**: 完善的內存管理，使用QT的父子對象系統

## 類別關係圖

```
QObject
  ├── Plant (抽象基類)
  │   ├── Sunflower (太陽花)
  │   └── Peashooter (豌豆射手)
  ├── Zombie (抽象基類)
  │   └── NormalZombie (普通殭屍)
  ├── PlantCell (植物格子)
  └── Game (遊戲管理器)

QMainWindow
  └── MainWindow (主窗口)
```

## 未來改進方向

1. 添加圖形渲染系統
2. 實現更多植物和殭屍類型
3. 添加子彈/投射物系統
4. 實現關卡系統
5. 添加音效和動畫
6. 保存/載入遊戲進度
7. 添加更複雜的AI行為

## 作者

這是一個基礎實現，展示了如何使用QT框架構建tick-based的遊戲系統。
