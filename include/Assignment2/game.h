#ifndef GAME_H
#define GAME_H

#include <memory>
#include <SFML/Graphics.hpp>
#include "entity_manager.h"
#include "entity.h"

const short COLOR_MIN = 0;
const short COLOR_MAX = 255;

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
        FR,   // Fillcolor Red
        FG,   // Fillcolor Green
        FB,   // Fillcolor Blue
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
        FP  // Font Path
    ;
};

class Game {
public:
    Game(const std::string &configPath) : m_configPath(configPath) {}
    ~Game() = default;
public:
    // read config from file
    int init();
    int run();
private:
    // systems
    void userInput();
    void movement();
    void collision();
    void lifespan();
    void update();
    void render();

    // assistant func
    void spawner();
    void playerSpawner();
    void enemySpawner();
    void smallEnemySpawner(const std::shared_ptr<Entity> enemy);
    void bulletSpawner(const Vec2 &mPos);
    sf::Color randColorGenerator();

    int loadConfig();
    int applyConfig();

    void procKeyPressed(sf::Keyboard::Key key);
    void procKeyReleased(sf::Keyboard::Key key);

    void procPlayerMovement();
    void procEntityMovement();

    void procBoundaryPlayerCollision();
    void procBoundaryEnemyCollision();
    void procEnemiesCollision();
    void procBulletEnemyCollision();

    bool isCollision(std::shared_ptr<Entity> e1, std::shared_ptr<Entity> e2);

    void decTransparencyByLeftLife(const std::shared_ptr<Entity> e, unsigned long long leftFrame);

    // collision debug tool
    std::shared_ptr<Entity> debugSpawner();
private:
    std::shared_ptr<sf::RenderWindow> m_window;
    EntityManager m_entities;
    std::shared_ptr<Entity> m_player;
    bool m_paused{ false };
    bool m_running{ true };
    long long unsigned m_currentFrame{ 0 };
    std::shared_ptr<sf::Text> m_scoreDisp;
    std::shared_ptr<sf::Font> m_font;

    const std::string &m_configPath;

    WindowConfig m_windowConfig;
    FontConfig m_fontConfig;
    PlayerConfig m_playerConfig;
    EnemyConfig m_enemyConfig;
    BulletConfig m_bulletConfig;
};

#endif // GAME_H