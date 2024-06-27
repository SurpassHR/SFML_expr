#ifndef GAME_H
#define GAME_H

#include <memory>
#include <SFML/Graphics.hpp>
#include "entity_manager.h"
#include "entity.h"

struct WindowConfig {
    int
        WW, // Window Width
        WH, // Window Height
        FL  // Frame Limit
    ;
    bool
        FS  // Full Screen
    ;
};

struct PlayerConfig {
    int
        SR, // Shape Radius
        CR, // Collision Radius
        FR, // Fillcolor Red
        FG, // Fillcolor Green
        FB, // Fillcolor Blue
        OR, // Outlinecolor Red
        OG, // Outlinecolor Green
        OB, // Outlinecolor Blue
        OT, // Outline Thickness
        SV  // Shape Verticies
    ;
    float
        S   // Speed
    ;
};

struct EnemyConfig {
    int
        SR,   // Shape Radius
        CR,   // Collision Radius
        OR,   // Outlinecolor Red
        OG,   // Outlinecolor Green
        OB,   // Outlinecolor Blue
        OT,   // Outline Thickness
        VMIN, // Min Verticies
        VMAX, // Max Verticies
        L,    // (Small enemy)Lifespan
        SI    // Spawn Interval
    ;
    float
        SMIN, // Min Speed
        SMAX  // Max Speed
    ;
};

struct BulletConfig {
    int
        SR, // Shape Radius
        CR, // Collision Radius
        FR, // Fillcolor Red
        FG, // Fillcolor Green
        FB, // Fillcolor Blue
        OR, // Outlinecolor Red
        OG, // Outlinecolor Green
        OB, // Outlinecolor Blue
        OT, // Outline Thickness
        SV, // Shape Verticies
        L   // (Bullet)Lifespan
    ;
    float
        S
    ;
};

struct FontConfig {
    int
        FS, // Font Size
        FR, // Fontcolor Red
        FG, // Fontcolor Green
        FB  // Fontcolor Blue
    ;
    std::string
        FP // Font Path
    ;
};

class Game {
public:
    Game(const std::string &configPath) : m_configPath(configPath) {}
    ~Game() = default;
public:
    void update();
    // read config from file
    int init();
    int run();
private:
    int loadConfig();
    int applyConfig();
    void userInput();
private:
    std::shared_ptr<sf::RenderWindow> m_window;
    EntityManager m_entities;
    Entity m_player;
    bool m_paused{ false };
    bool m_running{ true };

    const std::string &m_configPath;

    WindowConfig m_windowConfig;
    FontConfig m_fontConfig;
    PlayerConfig m_playerConfig;
    EnemyConfig m_enemyConfig;
    BulletConfig m_bulletConfig;
};

#endif // GAME_H