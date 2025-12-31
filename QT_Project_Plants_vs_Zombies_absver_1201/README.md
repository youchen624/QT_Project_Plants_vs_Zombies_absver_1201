# Plants vs. Zombies - Qt C++ Implementation

A Qt-based Plants vs. Zombies game implementation with a well-organized class structure designed for easy extension.

## 🎮 Features

### Implemented
- ✅ Complete object-oriented class hierarchy
- ✅ Plant system (Sunflower, Peashooter, Wallnut)
- ✅ Zombie system (Regular Zombie, Conehead Zombie)
- ✅ Bullet/Projectile system
- ✅ Sun collection system (click to collect)
- ✅ Grid-based planting system (5 rows × 9 columns)
- ✅ Collision detection (bullets vs zombies, zombies vs plants)
- ✅ Game loop and state management
- ✅ Basic UI (sun counter, plant buttons)
- ✅ Plant damage and death mechanics
- ✅ Zombie wave spawning with increasing difficulty

### In Progress / Planned
- ⏳ Complete sprite animations
- ⏳ Sound effects and background music
- ⏳ Victory conditions and level system
- ⏳ Save/load game functionality
- ⏳ More plant and zombie types

## 🏗️ Architecture

### Class Hierarchy

```
QObject
  └── ObjectTickable
      ├── Plant::Base
      │   ├── Sunflower (produces sun)
      │   ├── Peashooter (shoots peas)
      │   └── Wallnut (high defense)
      │
      ├── Zombie::Base
      │   ├── General (basic zombie)
      │   └── Conehead (armored zombie)
      │
      ├── Bullet (projectiles)
      └── Sun (collectible sun)
```

### Core Components

- **GameCore** - Main game controller managing game loop, objects, and collision detection
- **ObjectTickable** - Base class for all game objects with tick-based updates
- **AnimationUnit** - Handles sprite animation for all visual objects
- **Global** - Defines damage types, protection types, and other game data structures

## 🚀 Getting Started

### Prerequisites
- Qt 5 or Qt 6
- C++17 compiler
- qmake or CMake

### Building

```bash
cd QT_Project_Plants_vs_Zombies_absver_1201
qmake QT_Project_Plants_vs_Zombies_absver_1201.pro
make
./QT_Project_Plants_vs_Zombies_absver_1201
```

### Game Controls
- **Click plant buttons** to select a plant type
- **Click on grid** to plant (future feature)
- **Click on suns** to collect them
- Starting sun: 150

## 📚 Documentation

- **[Architecture Guide](README_ARCHITECTURE.md)** - Detailed class structure (Chinese)
- **[Class Diagram](CLASS_DIAGRAM.md)** - Visual class relationships and flow (Chinese)
- **[Extension Example](EXTENSION_EXAMPLE.md)** - How to add new plants/zombies (Chinese)

## 🔧 Extending the Game

### Adding a New Plant

1. Create header and implementation files in `plant/` directory
2. Inherit from `Plant::Base`
3. Implement required methods:
   - Constructor with row/col parameters
   - `tick(GameState)` for per-frame logic
   - `frames()` and `states()` for animation
4. Register in `GameCore::plantAt()`
5. Add UI button in `GameCore` constructor

Example structure:
```cpp
class MyPlant : public Plant::Base {
    Q_OBJECT
public:
    MyPlant(Core::GameCore* core, int row, int col);
    void tick(Core::GameState state) override;
protected:
    Core::AnimationUnit* aniUnit;
    // Your custom properties
};
```

### Adding a New Zombie

1. Create files in `zombie/` directory
2. Inherit from `Zombie::Base`
3. Set properties (health, speed, protection)
4. Register in `GameCore::spawnZombie()`

See [EXTENSION_EXAMPLE.md](EXTENSION_EXAMPLE.md) for detailed tutorial.

## 🎯 Game Mechanics

### Plants
- **Sunflower** (50 sun) - Produces sun every 25 seconds
- **Peashooter** (100 sun) - Shoots peas that deal 20 damage
- **Wallnut** (50 sun) - High defense with 4000 HP

### Zombies
- **General Zombie** - 200 HP, basic zombie
- **Conehead Zombie** - 370 HP + 370 armor

### Systems
- **Tick Rate**: 50ms per tick
- **Grid**: 5 rows × 9 columns, starting at (250, 80), 100px per cell
- **Sun Production**: Sky drops every 15 seconds, Sunflower produces every 25 seconds
- **Zombie Spawning**: Wave every 10 seconds, difficulty increases over time

## 🧪 Code Quality

The codebase follows these principles:
- **Single Responsibility**: Each class has a clear purpose
- **Open/Closed**: Easy to extend with new types without modifying existing code
- **Dependency Injection**: GameCore reference passed to all objects
- **Signal/Slot Pattern**: Qt's signal-slot for game loop synchronization

## 📝 Project Structure

```
QT_Project_Plants_vs_Zombies_absver_1201/
├── core/                   # Core game systems
│   ├── GameCore.h/cpp     # Main game controller
│   ├── ObjectTickable.h/cpp # Base game object
│   ├── AnimationUnit.h/cpp # Animation system
│   ├── Bullet.h/cpp       # Projectile system
│   ├── Sun.h/cpp          # Sun collectible
│   └── Global.h/cpp       # Global definitions
├── plant/                 # Plant implementations
│   ├── Base.h/cpp         # Plant base class
│   ├── Sunflower.h/cpp    # Sunflower plant
│   ├── Peashooter.h/cpp   # Peashooter plant
│   └── Wallnut.h/cpp      # Wallnut plant
├── zombie/                # Zombie implementations
│   ├── Base.h/cpp         # Zombie base class
│   ├── General.h/cpp      # Regular zombie
│   └── Conehead.h/cpp     # Conehead zombie
├── .resources/            # Game assets
│   └── img/              # Images
├── main.cpp              # Entry point
└── *.pro                 # Qt project file
```

## 🤝 Contributing

This is a learning project showcasing game architecture. Feel free to:
- Add more plant and zombie types
- Improve graphics and animations
- Add sound effects
- Implement level system
- Optimize performance

## 📄 License

This project is for educational purposes.

## 🙏 Acknowledgments

- Original Plants vs. Zombies by PopCap Games
- Qt Framework for the GUI and game loop
