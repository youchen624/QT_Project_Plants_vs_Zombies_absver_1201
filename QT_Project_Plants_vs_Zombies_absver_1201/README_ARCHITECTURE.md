# 植物大戰殭屍 - 類別架構說明文件

## 專案概述
這是一個使用 Qt C++ 開發的植物大戰殭屍遊戲。本文件說明遊戲的類別架構，以便未來擴充新角色、特性或功能。

## 核心架構設計

### 1. 基礎類別層級

```
QObject
  └── ObjectTickable (core/ObjectTickable.h)
      ├── Plant::Base (plant/Base.h)
      │   ├── Sunflower (plant/Sunflower.h) - 向日葵
      │   ├── Peashooter (plant/Peashooter.h) - 豌豆射手
      │   └── Wallnut (plant/Wallnut.h) - 堅果牆
      │
      ├── Zombie::Base (zombie/Base.h)
      │   ├── General (zombie/General.h) - 普通殭屍
      │   └── Conehead (zombie/Conehead.h) - 路障殭屍
      │
      ├── Bullet (core/Bullet.h) - 子彈/投射物
      └── Sun (core/Sun.h) - 陽光
```

### 2. 核心系統類別

- **GameCore** (core/GameCore.h) - 遊戲主控制器
  - 管理遊戲狀態
  - 控制遊戲循環 (tick system)
  - 處理碰撞檢測
  - 管理所有遊戲物件

- **AnimationUnit** (core/AnimationUnit.h) - 動畫系統
  - 管理精靈圖動畫
  - 支援多個動作狀態

- **Ground** (core/GameCore.h) - 遊戲場地
  - 顯示背景
  - 管理網格系統

## 如何擴充新功能

### 新增植物類型

1. 在 `plant/` 目錄下創建新的 `.h` 和 `.cpp` 檔案
2. 繼承 `Plant::Base` 類別
3. 實作必要的方法：

```cpp
// plant/NewPlant.h
#ifndef NEWPLANT_H
#define NEWPLANT_H

#include "plant/Base.h"
#include "core/AnimationUnit.h"

namespace PVZ {
namespace Plant {

class NewPlant : public Base
{
    Q_OBJECT
public:
    NewPlant(Core::GameCore* core, int row, int col);
    ~NewPlant();

    void tick(Core::GameState state) override;

protected:
    const QList<QPixmap>& frames() const;
    const QHash<QString, Core::P2>& states() const;
    Core::AnimationUnit* aniUnit;

private:
    // 你的自定義屬性
    int row, col;
};

}; };

#endif
```

4. 在 `.cpp` 檔案中實作：
   - 設定 `price`（價格）
   - 設定 `health`（血量）
   - 設定 `zombieEatable`（是否可被殭屍吃）
   - 實作 `tick()` 方法處理每幀邏輯

5. 在 `.pro` 檔案中添加新檔案

6. 在 `GameCore::plantAt()` 中添加新植物的創建邏輯

### 新增殭屍類型

1. 在 `zombie/` 目錄下創建新的 `.h` 和 `.cpp` 檔案
2. 繼承 `Zombie::Base` 類別
3. 可以自定義以下屬性：
   - `health` - 血量
   - `speed` - 移動速度
   - `attack_power` - 攻擊力
   - `protection` - 防護值和類型

```cpp
// zombie/NewZombie.h
#ifndef ZOMBIE_NEWZOMBIE_H
#define ZOMBIE_NEWZOMBIE_H

#include "zombie/Base.h"

namespace PVZ {
namespace Zombie {

class NewZombie : public Base
{
    Q_OBJECT
public:
    NewZombie(Core::GameCore* core, int row);
    ~NewZombie();

protected:
    const QList<QPixmap>& frames() const;
    const QHash<QString, Core::P2>& states() const;
};

}; };

#endif
```

4. 在 `GameCore::spawnZombie()` 中添加新殭屍的創建邏輯

### 新增投射物類型

1. 可以直接修改 `Bullet` 類別，或創建新的投射物類別
2. 繼承 `ObjectTickable`
3. 實作移動、碰撞邏輯
4. 設定傷害類型和數值

### 新增特殊效果

在 `Global.h` 中定義了以下系統：

**傷害類型** (`Damage::Type`):
- Normal - 普通傷害
- Explode - 爆炸傷害
- Bite - 啃咬傷害
- Burn - 燃燒傷害
- Poison - 毒素傷害
- Drown - 溺水傷害
- Force - 原力傷害

**防護類型** (`Protection::Type`):
- Normal - 普通防護
- Plastic - 塑膠防護
- Metal - 金屬防護

## 遊戲系統詳解

### Tick 系統
- 遊戲以 50ms 為一個 tick (定義於 `TICK_PER_MS`)
- 所有繼承 `ObjectTickable` 的物件都會在每個 tick 被更新
- 實作 `tick(GameState state)` 方法來定義物件行為

### 網格系統
- 5 行 9 列的網格 (`GRID_ROWS`, `GRID_COLS`)
- 起始位置: (250, 80)
- 每格大小: 100x100 像素

### 碰撞檢測
在 `GameCore::checkCollisions()` 中處理：
- 子彈與殭屍的碰撞
- 殭屍與植物的碰撞
- 殭屍到達終點的檢測

### 陽光系統
- 起始陽光: 150
- 天空掉落陽光: 每 300 ticks (15 秒)
- 向日葵生產陽光: 每 500 ticks (25 秒)，產值 25

### 殭屍生成
- 每 200 ticks (10 秒) 生成一波殭屍
- 數量隨遊戲時間增加
- 隨機選擇行數和類型

## 擴充建議

### 容易添加的功能：
1. **新植物種類**
   - 寒冰射手（減速效果）
   - 櫻桃炸彈（爆炸傷害）
   - 大嘴花（吞食殭屍）

2. **新殭屍種類**
   - 鐵桶殭屍（更高防護）
   - 旗幟殭屍（提升速度）
   - 投石車殭屍（遠程攻擊）

3. **新遊戲機制**
   - 植物升級系統
   - 關卡系統
   - 成就系統
   - 商店系統

### 需要較多工作的功能：
1. **完整動畫系統**
   - 需要準備精靈圖資源
   - 實作多幀動畫切換

2. **音效系統**
   - 使用 Qt Multimedia
   - 添加背景音樂和音效

3. **存檔系統**
   - 使用 QSettings 或 JSON
   - 儲存進度和設定

4. **網路多人模式**
   - 使用 Qt Network
   - 實作伺服器/客戶端架構

## 編譯和執行

需要 Qt 5 或 Qt 6 環境：

```bash
qmake QT_Project_Plants_vs_Zombies_absver_1201.pro
make
./QT_Project_Plants_vs_Zombies_absver_1201
```

## 目前已實作的功能

✅ 完整的類別繼承體系
✅ 植物系統（向日葵、豌豆射手、堅果牆）
✅ 殭屍系統（普通殭屍、路障殭屍）
✅ 子彈系統
✅ 陽光系統
✅ 網格種植系統
✅ 碰撞檢測
✅ 遊戲循環和狀態管理
✅ 基礎 UI（陽光計數、植物按鈕）

## 待完善的功能

⏳ 陽光點擊收集
⏳ 植物和殭屍的完整動畫
⏳ 音效和背景音樂
⏳ 遊戲勝利條件
⏳ 關卡系統
⏳ 存檔功能
⏳ 更多植物和殭屍類型
