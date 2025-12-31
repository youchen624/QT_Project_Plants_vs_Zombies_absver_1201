# 類別關係圖 (Class Diagram)

## 核心類別繼承關係

```
┌─────────────────────────┐
│      QMainWindow        │
└───────────┬─────────────┘
            │
            │ inherits
            ▼
┌─────────────────────────┐
│       GameCore          │◄──────────────┐
│  - state: GameState     │               │ manages
│  - m_scene: Scene*      │               │
│  - m_view: View*        │               │
│  - plants[][]           │               │
│  - zombies: List        │               │
│  - bullets: List        │               │
│  - suns: List           │               │
│  + plantAt()            │               │
│  + spawnZombie()        │               │
│  + spawnSun()           │               │
│  + checkCollisions()    │               │
└─────────────────────────┘               │
                                          │
┌─────────────────────────┐               │
│       QObject           │               │
└───────────┬─────────────┘               │
            │                             │
            │ inherits                    │
            ▼                             │
┌─────────────────────────┐               │
│    ObjectTickable       │◄──────────────┤
│  # core: GameCore*      │               │
│  # pause: bool          │               │
│  + tick(state)          │               │
└───────────┬─────────────┘               │
            │                             │
            │                             │
      ┌─────┴─────────┬──────────┬────────┴─────┐
      │               │          │              │
      ▼               ▼          ▼              ▼
┌──────────┐   ┌──────────┐  ┌────────┐   ┌────────┐
│Plant::   │   │Zombie::  │  │ Bullet │   │  Sun   │
│Base      │   │Base      │  │        │   │        │
└────┬─────┘   └────┬─────┘  └────────┘   └────────┘
     │              │
     │              │
 ┌───┴──────┬───────┴────┬────────┐   ┌────────┬─────────┐
 │          │            │        │   │        │         │
 ▼          ▼            ▼        ▼   ▼        ▼         ▼
┌──────┐ ┌───────┐ ┌─────────┐ ┌─────┐ ┌────────┐ ┌──────────┐
│Sun-  │ │Pea-   │ │Wall-    │ │Gene-│ │Cone-   │ │More...   │
│flower│ │shooter│ │nut      │ │ral  │ │head    │ │          │
└──────┘ └───────┘ └─────────┘ └─────┘ └────────┘ └──────────┘
```

## 類別職責說明

### GameCore (遊戲核心控制器)
**職責:**
- 管理遊戲主循環 (game loop)
- 維護遊戲狀態 (進行中/暫停/結束)
- 管理所有遊戲物件的生命週期
- 處理碰撞檢測
- 管理 UI 元件

**關鍵方法:**
- `gameTick()` - 每個時間週期被調用
- `plantAt(type, row, col)` - 在指定位置種植植物
- `spawnZombie(type, row)` - 生成殭屍
- `checkCollisions()` - 檢查並處理碰撞

### ObjectTickable (可更新物件基類)
**職責:**
- 所有遊戲物件的共同基類
- 提供 tick 機制，讓物件在每個時間週期更新
- 自動連接到 GameCore 的信號

**關鍵屬性:**
- `core` - 指向 GameCore 的指標
- `pause` - 是否暫停

**關鍵方法:**
- `tick(GameState)` - 虛擬方法，子類實作更新邏輯

### Plant::Base (植物基類)
**職責:**
- 所有植物的共同基類
- 定義植物的基本屬性和行為

**關鍵屬性:**
- `health` - 生命值
- `price` - 種植成本
- `zombieEatable` - 是否可被殭屍啃食

**關鍵方法:**
- `takeDamage(amount)` - 受到傷害
- `isDead()` - 是否死亡

### 具體植物類別

#### Sunflower (向日葵)
- **特性:** 生產陽光
- **價格:** 50
- **血量:** 300
- **功能:** 每 500 ticks 生產 25 陽光

#### Peashooter (豌豆射手)
- **特性:** 發射豌豆攻擊殭屍
- **價格:** 100
- **血量:** 300
- **功能:** 每 30 ticks 發射一顆豌豆

#### Wallnut (堅果牆)
- **特性:** 高血量防禦
- **價格:** 50
- **血量:** 4000
- **功能:** 阻擋殭屍前進

### Zombie::Base (殭屍基類)
**職責:**
- 所有殭屍的共同基類
- 定義殭屍的移動、攻擊行為

**關鍵屬性:**
- `health` - 生命值
- `speed` - 移動速度
- `attack_power` - 攻擊力
- `protection` - 防護值
- `eating` - 是否正在進食
- `poison`, `slowness`, `onfire` - 狀態效果

**關鍵方法:**
- `takeDamage(Damage)` - 受到傷害
- `tick(GameState)` - 更新位置和狀態

### 具體殭屍類別

#### General (普通殭屍)
- **血量:** 200
- **速度:** 0.5
- **特性:** 基礎殭屍

#### Conehead (路障殭屍)
- **血量:** 370
- **速度:** 0.5
- **防護:** 370 (塑膠)
- **特性:** 有額外防護

### Bullet (子彈/投射物)
**職責:**
- 植物發射的攻擊投射物
- 移動並檢測碰撞

**關鍵屬性:**
- `x, y` - 位置
- `row` - 所在行
- `speed` - 移動速度
- `damage` - 傷害值
- `active` - 是否活躍

### Sun (陽光)
**職責:**
- 遊戲貨幣
- 可從天空掉落或由向日葵生產

**關鍵屬性:**
- `x, y` - 位置
- `value` - 陽光值 (25)
- `lifetime` - 存在時間
- `collectable` - 是否可收集

### AnimationUnit (動畫單元)
**職責:**
- 管理精靈圖動畫
- 支援多個動作狀態

**關鍵方法:**
- `changeMotionState(state)` - 切換動作
- `update()` - 更新動畫幀

## 資料結構

### GameState (遊戲狀態)
```cpp
enum class GameState {
    Pause = 0,           // 暫停
    Progressing,         // 進行中
    GameOver_lose,       // 失敗
    GameOver_win         // 勝利
};
```

### Damage (傷害)
```cpp
struct Damage {
    quint64 value;       // 傷害值
    enum class Type {
        Normal,          // 普通
        Explode,         // 爆炸
        Bite,            // 啃咬
        Burn,            // 燃燒
        Poison,          // 毒素
        Drown,           // 溺水
        Force            // 原力
    } type;
};
```

### Protection (防護)
```cpp
struct Protection {
    enum class Type {
        Normal,          // 普通
        Plastic,         // 塑膠
        Metal            // 金屬
    } type;
    quint64 value;       // 防護值
};
```

## 遊戲循環流程

```
GameCore 初始化
    │
    ├─→ 創建場景和視圖
    ├─→ 初始化網格
    ├─→ 設置 UI
    └─→ 啟動計時器 (50ms/tick)
          │
          ▼
    ┌─────────────────┐
    │   gameTick()    │◄───┐
    └────────┬────────┘    │
             │              │
             ├─→ emit coreTick()
             │   (所有 ObjectTickable 更新)
             │
             ├─→ 生成殭屍波次
             │
             ├─→ 生成天空陽光
             │
             ├─→ checkCollisions()
             │   ├─→ 子彈 vs 殭屍
             │   ├─→ 殭屍 vs 植物
             │   └─→ 殭屍到達終點
             │
             └─→ cleanupDeadObjects()
                 ├─→ 移除死亡殭屍
                 ├─→ 移除失效子彈
                 └─→ 移除收集的陽光
                       │
                       └───────────────┘
```

## 擴展點 (Extension Points)

### 1. 新增植物
- 繼承 `Plant::Base`
- 實作 `tick()` 方法
- 在 `GameCore::plantAt()` 添加創建邏輯

### 2. 新增殭屍
- 繼承 `Zombie::Base`
- 設置屬性（血量、速度、防護）
- 在 `GameCore::spawnZombie()` 添加創建邏輯

### 3. 新增投射物
- 繼承 `ObjectTickable` 或創建新的基類
- 實作移動和碰撞邏輯
- 在植物的 `tick()` 中創建

### 4. 新增特殊效果
- 在 `Damage::Type` 或 `Protection::Type` 添加新類型
- 在 `Zombie::Base::takeDamage()` 中處理新效果

### 5. 新增遊戲機制
- 在 `GameCore` 中添加新的管理方法
- 創建新的類別處理特定功能
- 整合到遊戲循環中
