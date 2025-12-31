# 擴充範例：如何添加新植物

這個文件展示如何向遊戲添加一個新的植物類型。我們將以「寒冰射手」(Ice Peashooter) 為例。

## 步驟 1: 創建標頭檔

在 `plant/` 目錄下創建 `IcePeashooter.h`：

```cpp
#ifndef ICEPEASHOOTER_H
#define ICEPEASHOOTER_H

#include "plant/Base.h"
#include "core/AnimationUnit.h"

namespace PVZ {
namespace Plant {

class IcePeashooter : public Base
{
    Q_OBJECT
public:
    static constexpr int SHOOT_INTERVAL_TICKS = 30;

public:
    IcePeashooter(Core::GameCore* core, int row, int col);
    ~IcePeashooter();

    void tick(Core::GameState state) override;

protected:
    const QList<QPixmap>& frames() const;
    const QHash<QString, Core::P2>& states() const;
    Core::AnimationUnit* aniUnit;

private:
    int shoot_ticks;
    int row;
    int col;
};

}; };

#endif // ICEPEASHOOTER_H
```

## 步驟 2: 實作原始檔

在 `plant/` 目錄下創建 `IcePeashooter.cpp`：

```cpp
#include "plant/IcePeashooter.h"
#include "core/GameCore.h"
#include "core/Bullet.h"

using namespace PVZ::Plant;

const QList<QPixmap>& IcePeashooter::frames() const {
    static const QList<QPixmap> data = []() {
        QList<QPixmap> list;
        // 使用藍色圖片代表寒冰射手
        QPixmap img(60, 60);
        img.fill(QColor(100, 150, 255));
        list.append(img);
        return list;
    }();
    return data;
}

const QHash<QString, PVZ::Core::P2>& IcePeashooter::states() const {
    static const QHash<QString, PVZ::Core::P2> data = []() {
        QHash<QString, PVZ::Core::P2> hash;
        hash.insert("Idle", PVZ::Core::P2(0, 0));
        return hash;
    }();
    return data;
}

IcePeashooter::IcePeashooter(Core::GameCore* core, int row, int col) :
    Base(core), shoot_ticks(0), row(row), col(col)
{
    zombieEatable = true;
    price = 175;  // 比普通豌豆射手貴
    health = 300;

    aniUnit = new Core::AnimationUnit(core, frames(), states());
    aniUnit->changeMotionState("Idle");
    
    // 定位植物
    qreal x = 250 + col * 100 + 25;
    qreal y = 80 + row * 100 + 25;
    aniUnit->setPosition(x, y);
}

IcePeashooter::~IcePeashooter() {
    delete aniUnit;
}

void IcePeashooter::tick(Core::GameState state) {
    if (state != Core::GameState::Progressing) return;
    
    aniUnit->update();

    // 定期發射寒冰豌豆
    if (++shoot_ticks >= SHOOT_INTERVAL_TICKS) {
        qreal x = 250 + col * 100 + 50;
        qreal y = 80 + row * 100 + 40;
        
        // 創建寒冰子彈（需要擴充 Bullet 類別支援）
        Core::Bullet* bullet = new Core::Bullet(core, x, y, row);
        // 這裡可以設定子彈為寒冰類型，讓它造成減速效果
        
        shoot_ticks = 0;
    }
}
```

## 步驟 3: 更新專案檔

在 `QT_Project_Plants_vs_Zombies_absver_1201.pro` 中添加：

```qmake
SOURCES += \
    ...
    plant/IcePeashooter.cpp

HEADERS += \
    ...
    plant/IcePeashooter.h
```

## 步驟 4: 在 GameCore 中註冊新植物

在 `core/GameCore.cpp` 的 `plantAt()` 方法中添加：

```cpp
#include "plant/IcePeashooter.h"

void GameCore::plantAt(QString type, int row, int col) {
    // ... 現有代碼 ...
    
    else if (type == "icepeashooter") {
        cost = 175;
        if (sun_count >= cost) {
            plant = new Plant::IcePeashooter(this, row, col);
        }
    }
    
    // ... 現有代碼 ...
}
```

## 步驟 5: 添加 UI 按鈕

在 `GameCore::GameCore()` 建構函數中添加按鈕：

```cpp
QPushButton* btnIcePeashooter = new QPushButton("Ice Peashooter (175)");
connect(btnIcePeashooter, &QPushButton::clicked, [this]() {
    if (sun_count >= 175) {
        plantAt("icepeashooter", 2, 3);
    }
});
topBar->addWidget(btnIcePeashooter);
```

## 進階功能：添加特殊效果

### 擴充子彈類別支援寒冰效果

可以創建一個 `IceBullet` 類別繼承 `Bullet`：

```cpp
// core/IceBullet.h
class IceBullet : public Bullet {
public:
    IceBullet(GameCore* core, qreal x, qreal y, int row) 
        : Bullet(core, x, y, row) {
        // 設定為寒冰傷害
        damage.type = Damage::Type::Normal;  // 可以添加 Ice 類型
        damage.value = 20;
        
        // 改變子彈顏色
        graphic->setBrush(QBrush(QColor(100, 150, 255)));
    }
    
    // 可以覆寫碰撞處理，添加減速效果
};
```

### 在碰撞檢測中處理減速效果

在 `GameCore::checkCollisions()` 中：

```cpp
// 檢測到寒冰子彈命中時
if (bullet->getDamage().type == Damage::Type::Ice) {
    zombie->setSlowness(true);  // 需要在 Zombie::Base 中添加此方法
}
```

## 更多擴充點

### 可以添加的植物特性

1. **生產型植物**（如向日葵）
   - 覆寫 `tick()` 方法
   - 定期調用 `core->spawnSun()`

2. **攻擊型植物**（如豌豆射手）
   - 在 `tick()` 中檢測殭屍
   - 創建子彈攻擊

3. **防禦型植物**（如堅果牆）
   - 設定高血量
   - 可以添加特殊防護效果

4. **特殊效果植物**
   - 爆炸植物（櫻桃炸彈）
   - 範圍攻擊
   - 即時效果

### 可以添加的殭屍特性

1. **高血量殭屍**
   - 增加 `health` 和 `protection` 值

2. **快速殭屍**
   - 增加 `speed` 值

3. **特殊能力殭屍**
   - 飛行（跳過植物）
   - 投擲（遠程攻擊）
   - 挖地（從後方出現）

## 測試新植物

1. 編譯專案
2. 運行遊戲
3. 點擊新添加的按鈕測試種植
4. 觀察植物行為是否正確
5. 測試與殭屍的互動

## 常見問題

**Q: 為什麼我的植物沒有顯示？**
A: 檢查 `AnimationUnit` 是否正確初始化，並調用了 `setPosition()`。

**Q: 如何添加動畫？**
A: 在 `frames()` 方法中添加多個 QPixmap，並在 `states()` 中定義幀範圍。

**Q: 如何讓植物攻擊特定目標？**
A: 在 `tick()` 中遍歷 `core->zombies` 列表，檢測同一行的殭屍。

**Q: 如何添加音效？**
A: 使用 Qt Multimedia，在適當時機播放音效檔案。

## 完整類別列表

以下是所有可以擴充的基類：

- `Plant::Base` - 所有植物的基類
- `Zombie::Base` - 所有殭屍的基類
- `Core::Bullet` - 投射物基類
- `Core::ObjectTickable` - 所有遊戲物件的基類

每個類別都有詳細的接口說明，請參考對應的標頭檔。
