#ifndef ZOMBIETYPES_H
#define ZOMBIETYPES_H

#include "zombiebase.h"

// Normal zombie
class NormalZombie : public Zombie
{
    Q_OBJECT
public:
    explicit NormalZombie(QObject *parent = nullptr);
    void update(float deltaTime) override;
    void render(QPainter *painter) override;
};

// Fast zombie
class FastZombie : public Zombie
{
    Q_OBJECT
public:
    explicit FastZombie(QObject *parent = nullptr);
    void update(float deltaTime) override;
    void render(QPainter *painter) override;
};

// Armored zombie (high health)
class ArmoredZombie : public Zombie
{
    Q_OBJECT
public:
    explicit ArmoredZombie(QObject *parent = nullptr);
    void update(float deltaTime) override;
    void render(QPainter *painter) override;
};

// Flag zombie (appears in large waves)
class FlagZombie : public Zombie
{
    Q_OBJECT
public:
    explicit FlagZombie(QObject *parent = nullptr);
    void update(float deltaTime) override;
    void render(QPainter *painter) override;
};

// Cone Head zombie (more health than normal)
class ConeHeadZombie : public Zombie
{
    Q_OBJECT
public:
    explicit ConeHeadZombie(QObject *parent = nullptr);
    void update(float deltaTime) override;
    void render(QPainter *painter) override;
};

// Car zombie (drives a car, crushes plants)
class CarZombie : public Zombie
{
    Q_OBJECT
public:
    explicit CarZombie(QObject *parent = nullptr);
    void update(float deltaTime) override;
    void render(QPainter *painter) override;
    
    bool isCrushing() const { return m_isCrushing; }

private:
    bool m_isCrushing;
};

// Pole Vaulting zombie (jumps over one plant)
class PoleVaultingZombie : public Zombie
{
    Q_OBJECT
public:
    explicit PoleVaultingZombie(QObject *parent = nullptr);
    void update(float deltaTime) override;
    void render(QPainter *painter) override;
    
    bool canVault() const override { return !m_hasVaulted; }
    bool hasVaulted() const override { return m_hasVaulted; }
    void setVaulted(bool vaulted) override { m_hasVaulted = vaulted; }
    void startVault() { m_isVaulting = true; m_hasVaulted = true; }

private:
    bool m_hasVaulted;
    bool m_isVaulting;
    float m_vaultTimer;
    float m_vaultDuration;
};

#endif // ZOMBIETYPES_H
