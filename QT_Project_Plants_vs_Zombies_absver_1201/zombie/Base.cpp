#include "zombie/Base.h"
#include "core/GameCore.h"

using namespace PVZ::Zombie;

Base::Base(Core::GameCore* core, int row) :
    Core::ObjectTickable(core),
    x(1300), 
    y(80 + row * 100),
    row(row),
    speed(0.5),
    health(200),
    attack_power(20),
    attack_interval(50),
    attack_ticks(0),
    poison(false),
    slowness(false),
    onfire(false),
    eating(false),
    aniUnit(nullptr)
{
    protection.type = Core::Protection::Type::Normal;
    protection.value = 0;
}

Base::~Base() {
    if (aniUnit) {
        delete aniUnit;
    }
}

void Base::tick(Core::GameState state) {
    if (state != Core::GameState::Progressing) return;
    if (health <= 0) return;
    
    // Move left if not eating
    if (!eating) {
        x -= speed * (slowness ? 0.5 : 1.0);
    }
    
    // Update animation if exists
    if (aniUnit) {
        aniUnit->update();
    }
    
    // Attack logic
    if (eating) {
        attack_ticks++;
        if (attack_ticks >= attack_interval) {
            // Deal damage to plant (this will be handled by GameCore collision detection)
            attack_ticks = 0;
        }
    }
}

void Base::takeDamage(const Core::Damage& damage) {
    int actual_damage = damage.value;
    
    // Apply protection
    if (protection.value > 0) {
        if (protection.value >= actual_damage) {
            protection.value -= actual_damage;
            return;
        } else {
            actual_damage -= protection.value;
            protection.value = 0;
        }
    }
    
    health -= actual_damage;
    
    // Apply status effects
    if (damage.type == Core::Damage::Type::Burn) {
        onfire = true;
    } else if (damage.type == Core::Damage::Type::Poison) {
        poison = true;
    }
}
