# Implementation Summary - Plants vs. Zombies QT Project

## 完成的工作

### 1. 基礎類別架構 ✓

#### Plant 基類 (Plant.h/cpp)
- 抽象基類，所有植物必須繼承
- 核心屬性：health, maxHealth, position, cost, cooldown
- 純虛擬方法：update(), performAction(), getType()
- 信號：died(), actionPerformed()
- 提供 takeDamage() 方法處理傷害

#### Zombie 基類 (Zombie.h/cpp)
- 抽象基類，所有殭屍必須繼承
- 核心屬性：health, maxHealth, position, speed, attackDamage
- 純虛擬方法：update(), move(), attack(), getType()
- 信號：died(), reachedEnd()
- 提供 takeDamage() 方法處理傷害

#### PlantCell 類 (PlantCell.h/cpp)
- 管理網格單元格
- 追蹤格子佔用狀態
- 處理植物放置和移除
- 自動管理植物生命週期
- 信號：plantPlaced(), plantRemoved()

#### Game 類 (Game.h/cpp)
- 核心遊戲管理器
- 使用 QTimer 實現 tick-based 循環（30 ticks/秒）
- 管理 5x9 網格系統
- 陽光資源管理（初始 150 點）
- 遊戲狀態控制：start(), stop(), pause(), resume()
- 完整的遊戲循環：
  1. updatePlants() - 更新所有植物
  2. updateZombies() - 更新所有殭屍
  3. checkCollisions() - 碰撞檢測
  4. cleanupDeadEntities() - 清理死亡實體

### 2. 實現的植物 ✓

#### Sunflower (太陽花)
```cpp
屬性：
- 成本：50 陽光
- 生命值：300
- 產生間隔：240 ticks (8秒)
- 產生量：25 陽光/次

行為：
- 定期自動產生陽光
- 發出 sunProduced(int) 信號
```

#### Peashooter (豌豆射手)
```cpp
屬性：
- 成本：100 陽光
- 生命值：300
- 射擊間隔：90 ticks (3秒)
- 傷害：20/發
- 射程：10 格

行為：
- 定期向同行殭屍射擊
- 發出 peaShot(int row, int damage) 信號
```

### 3. 實現的殭屍 ✓

#### NormalZombie (普通殭屍)
```cpp
屬性：
- 生命值：200
- 速度：1 像素/tick
- 攻擊傷害：20
- 攻擊冷卻：30 ticks (1秒)

行為：
- 從右側進入，向左移動
- 遇到植物時停止並攻擊
- 發出 attackingPlant(int row, int damage) 信號
- 到達左邊界時觸發遊戲結束
```

### 4. MainWindow 整合 ✓

- 創建並管理 Game 實例
- 提供遊戲控制按鈕
- 顯示遊戲狀態（陽光、tick）
- 測試功能：放置植物、生成殭屍

### 5. 專案配置 ✓

- 更新 .pro 文件包含所有新源文件
- 配置 C++17 標準
- 包含必要的 QT 模組（core, gui, widgets）

## 技術實現細節

### Tick-Based 系統
```cpp
// Game.cpp
tickRate = 30;  // 30 ticks per second
gameTimer->setInterval(1000 / tickRate);  // 33.33ms per tick
```

### 網格系統
```cpp
// 5 rows x 9 columns
gridRows = 5;
gridCols = 9;
QVector<QVector<PlantCell*>> grid;
```

### 信號槽連接
```cpp
// 自動連接陽光生產
Sunflower* sunflower = qobject_cast<Sunflower*>(plant);
if (sunflower) {
    connect(sunflower, &Sunflower::sunProduced, 
            game, &Game::onSunProduced);
}
```

### 碰撞檢測
```cpp
// 簡單的位置檢測
// 檢查殭屍是否到達植物所在列
if (zombieX <= plantX + 100 && zombieX >= plantX) {
    // 觸發攻擊
    normalZombie->setAttacking(true);
    plant->takeDamage(damage);
}
```

## 代碼結構

```
QT_Project_Plants_vs_Zombies_absver_1231/
├── Plant.h / Plant.cpp              (植物基類)
├── Zombie.h / Zombie.cpp            (殭屍基類)
├── PlantCell.h / PlantCell.cpp      (格子類)
├── Game.h / Game.cpp                (遊戲管理器)
├── Sunflower.h / Sunflower.cpp      (太陽花)
├── Peashooter.h / Peashooter.cpp    (豌豆射手)
├── NormalZombie.h / NormalZombie.cpp (普通殭屍)
├── mainwindow.h / mainwindow.cpp    (主窗口)
├── main.cpp                          (程序入口)
├── test_game.cpp                     (測試程序)
├── README.md                         (詳細文檔)
└── QT_Project_Plants_vs_Zombies_absver_1231.pro
```

## 設計模式

1. **繼承和多態**: Plant 和 Zombie 作為抽象基類
2. **組合模式**: Game 組合多個 PlantCell 和 Zombie
3. **觀察者模式**: 使用 QT 信號槽機制
4. **策略模式**: 不同植物/殭屍有不同的行為策略

## 測試驗證

創建了 test_game.cpp 用於驗證：
- 遊戲初始化
- 遊戲啟動/停止
- 植物放置
- 殭屍生成
- Tick 更新
- 信號發送

## 可擴展性

### 添加新植物範例
```cpp
class CherryBomb : public Plant
{
public:
    CherryBomb() {
        name = "Cherry Bomb";
        health = 300;
        cost = 150;
        cooldown = 1500;  // 50 seconds
    }
    
    void update(int tickCount) override {
        // 爆炸邏輯
    }
    
    void performAction() override {
        emit exploded();  // 自定義信號
    }
    
    QString getType() const override {
        return "CherryBomb";
    }
};
```

### 添加新殭屍範例
```cpp
class ConeheadZombie : public Zombie
{
public:
    ConeheadZombie(int row) {
        name = "Conehead Zombie";
        health = 640;  // 更高的生命值
        speed = 1;
        attackDamage = 20;
        position = QPoint(900, row);
    }
    
    // 實現必要的虛擬方法...
};
```

## 性能考量

- 使用 QVector 儲存網格，O(1) 訪問
- 使用 QList 儲存殭屍，支持動態添加/刪除
- 每個 tick 只更新活著的實體
- 自動清理死亡實體避免內存洩漏

## 內存管理

- 使用 QT 父子對象系統自動管理內存
- Plant 和 Zombie 的父對象設置為對應的容器
- 使用 deleteLater() 安全刪除 QObject
- 智能指針不需要，QT 的對象樹已足夠

## 線程安全

- 當前實現為單線程
- 所有操作在主線程的事件循環中執行
- 使用 QTimer 而非獨立線程

## 已知限制

1. 沒有圖形渲染（僅邏輯實現）
2. 沒有投射物系統（Peashooter 直接發出信號）
3. 碰撞檢測較簡單（基於位置比較）
4. 沒有關卡系統
5. 沒有保存/載入功能

## 下一步建議

1. 添加 QGraphicsView 實現視覺化
2. 實現投射物類別用於豌豆
3. 添加更多植物和殭屍類型
4. 實現波次系統
5. 添加音效和動畫
6. 實現不同難度的關卡

## 總結

✅ 完整實現了 tick-based 遊戲系統
✅ 創建了可擴展的類別架構
✅ 實現了基礎的植物和殭屍
✅ 整合到 QT MainWindow
✅ 提供了詳細的文檔

所有需求均已滿足，代碼結構清晰，易於擴展。
