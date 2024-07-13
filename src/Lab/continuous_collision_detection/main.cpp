#include "main.h"
#include "vec2.h"
#include "utils.h"
#include <SFML/Window/Event.hpp>

void Entity::init(EntityShape shape, const void *para)
{
    switch (shape) {
        case EntityShape::CIRCLE_SHAPE: {
            m_circle = std::make_shared<sf::CircleShape>();
            CirclePara *cirPara = reinterpret_cast<CirclePara *>(const_cast<void *>(para));
            initCircle(m_circle, *cirPara);
            break;
        }
        case EntityShape::RECT_SHAPE: {
            m_rect = std::make_shared<sf::RectangleShape>();
            RectPara *rectPara = reinterpret_cast<RectPara *>(const_cast<void *>(para));
            initRect(m_rect, *rectPara);
            break;
        }
        default:
            LOG("invalid shape err");
            break;
    }
    m_shape = shape;
}

std::shared_ptr<sf::Shape> Entity::shape()
{
    switch (m_shape) {
        case EntityShape::CIRCLE_SHAPE:
            return m_circle;
        case EntityShape::RECT_SHAPE:
            return m_rect;
        default:
            break;
    }
    return nullptr;
}

void Entity::update()
{
    shape()->setPosition(pos.vec2f());
}

void Entity::initCircle(std::shared_ptr<sf::CircleShape> e, const CirclePara &para)
{
    if (e == nullptr) {
        LOG("nullptr err");
        return;
    }
    e->setRadius(para.r);
    e->setPosition(para.pos.vec2f());
    e->setOrigin(para.origin.vec2f());
    e->setFillColor(para.fColor);
    e->setOutlineColor(para.oColor);
    e->setOutlineThickness(para.oThick);
    pos = para.pos;
    r = para.r;
}

void Entity::initRect(std::shared_ptr<sf::RectangleShape> e, const RectPara &para)
{
    if (e == nullptr) {
        LOG("nullptr err");
        return;
    }
    e->setSize(para.size.vec2f());
    e->setPosition(para.pos.vec2f());
    e->setOrigin(para.origin.vec2f());
    e->setFillColor(para.fColor);
    e->setOutlineColor(para.oColor);
    e->setOutlineThickness(para.oThick);
    pos = para.pos;
    size = para.size;
}

int Lab::init()
{
    m_window = std::make_shared<sf::RenderWindow>(sf::VideoMode(1366, 768), "continuous collision detection");
    m_window->setFramerateLimit(m_frameLimit);

    RectPara rectPara;
    CirclePara cirPara;

    std::shared_ptr<Entity> rect1 = std::make_shared<Entity>(1);
    rectPara.pos = { 100, 130 };
    rect1->init(EntityShape::RECT_SHAPE, &rectPara);
    std::shared_ptr<Entity> cir11 = std::make_shared<Entity>(1000);
    cirPara.pos = rectPara.pos + Vec2(cirPara.r, cirPara.r);
    cirPara.pos.print("cir11");
    cir11->init(EntityShape::CIRCLE_SHAPE, &cirPara);

    m_entities["rect1"] = rect1;
    m_entitiesSeq.push_back(rect1);
    m_entities["cir11"] = cir11;
    m_entitiesSeq.push_back(cir11);

    std::shared_ptr<Entity> rect2 =std::make_shared<Entity>(1);
    rectPara.pos = { 760, 130 };
    rect2->init(EntityShape::RECT_SHAPE, &rectPara);
    std::shared_ptr<Entity> cir21 = std::make_shared<Entity>(1000);
    cirPara.pos = rectPara.pos + Vec2(cirPara.r, cirPara.r);
    cirPara.pos.print("cir21");
    cir21->init(EntityShape::CIRCLE_SHAPE, &cirPara);

    m_entities["rect2"] = rect2;
    m_entitiesSeq.push_back(rect2);
    m_entities["cir21"] = cir21;
    m_entitiesSeq.push_back(cir21);

    std::sort(m_entitiesSeq.begin(), m_entitiesSeq.end(),
        [](const auto &a, const auto &b){
            return a->zIndex() < b->zIndex();
    });

    return 0;
}

void Lab::labNormalCollisionDetect()
{
    Entity lab = *m_entities["rect1"];

    double left = lab.shape()->getPosition().x;
    double width = lab.size.x;
    double top = lab.shape()->getPosition().y;
    double height = lab.size.y;

    double right = left + width;
    double bottom = top + height;

    // LOG("%f %f %f %f", top, left, bottom, right);

    auto &cir11 = *m_entities["cir11"];

    bool horiColl = (cir11.pos.x - cir11.r < left) || (cir11.pos.x + cir11.r > right);
    bool vertColl = (cir11.pos.y - cir11.r < top) || (cir11.pos.y + cir11.r > bottom);

    // LOG("pos[%f, %f], r[%f]", cir11.pos.x, cir11.pos.y, cir11.r);

    cir11.velo.x *= horiColl ? -1 : 1;
    cir11.velo.y *= vertColl ? -1 : 1;
}

void Lab::labContinuousCollisionDetect()
{
    Entity lab = *m_entities["rect2"];

    double left = lab.shape()->getPosition().x;
    double width = lab.size.x;
    double top = lab.shape()->getPosition().y;
    double height = lab.size.y;

    double right = left + width;
    double bottom = top + height;

    auto &cir21 = *m_entities["cir21"];

    bool topOverlap = (cir21.pos.y - cir21.r < top);
    bool leftOverlap = (cir21.pos.x - cir21.r < left);
    bool bottomOverlap = (cir21.pos.y + cir21.r > bottom);
    bool rightOverlap = (cir21.pos.x + cir21.r > right);

    auto &pos = cir21.pos;
    auto &velo = cir21.velo;
    // auto &acc = cir21.acc;
    // auto &lastPos = cir21.lastPos;

    // double x0 = lastPos.x;
    // double y0 = lastPos.y;
    double x1 = pos.x;
    double y1 = pos.y;

    // x(0) = a * x0 + b
    // x(1) = a * x1 + b
    // -> x(t) = (x1 - x0) * t + x0
    // same with y:
    // -> y(t) = (y1 - y0) * t + y0

    // find t when x(t) + r = right
    // (x1 - x0) * t + x0 + r = right
    // t = (right - x0 - r) / (x1 - x0)
    // x(t) = (x1 - x0) * (right - x0 - r) / (x1 - x0) + x0 = right - r
    if (rightOverlap) {
        pos.x = right - cir21.r;
        pos.x = 2.0f * pos.x - x1;
        pos.y = pos.y;
        velo.x *= -1.0f;
    }

    // find t when x(t) - r = left
    // (x1 - x0) * t + x0 - r = left
    // t = (left - x0 + r) / (x1 - x0)
    // x(t) = (x1 - x0) * (left - x0 + r) / (x1 - x0) + x0 = left + r
    // x'(1) = x(t) + (x(t) - x(1))
    if (leftOverlap) {
        pos.x = left + cir21.r;
        pos.x = 2.0f * pos.x - x1;
        pos.y = pos.y;
        velo.x *= -1.0f;
    }

    // find t when y(t) - r = top
    // (y1 - y0) * t + y0 - r = top
    // t = (top - y0 + r) / (y1 - y0)
    // y(t) = (y1 - y0) * (top - y0 + r) / (y1 - y0) + y0 = top + r
    // y'(1) = y(t) - (y(1) - y(t)) = 2 * y(t) - y(1)
    if (topOverlap) {
        pos.y = top + cir21.r;
        pos.y = 2.0f * pos.y - y1;
        pos.x = pos.x;
        velo.y *= -1.0f;
    }

    // find t when y(t) + r = bottom
    // (y1 - y0) * t + y0 + r = bottom
    // t = (bottom - y0 - r) / (y1 - y0)
    // y(t) = (y1 - y0) * (bottom - y0 - r) / (y1 - y0) + y0 = bottomm - r
    // y'(1) = y(t) + (y(t) - y(1)) = 2 * y(t) - y(1)
    if (bottomOverlap) {
        pos.y = bottom - cir21.r;
        pos.y = 2.0f * pos.y - y1;
        pos.x = pos.x;
        velo.y *= -1.0f;
    }
}

void Lab::updateCorrection()
{
    for (auto &iter : m_entities) {
        iter.second->update();
    }
}

int Lab::run()
{
    if (m_window == nullptr) {
        return 1;
    }
    while (m_isRunning) {
        procEvent();
        movement();
        logic();
        render();
    }
    return 0;
}

void Lab::procEvent()
{
    sf::Event event;
    while (m_window->pollEvent(event)) {
        switch (event.type) {
            case sf::Event::EventType::KeyPressed:
                procKeypress(event.key);
                break;
            case sf::Event::Closed:
                m_isRunning = false;
                break;
            default:
                break;
        }
    }
}

void Lab::procKeypress(sf::Event::KeyEvent kEvent)
{
    switch (kEvent.code) {
        case sf::Keyboard::Key::Q:
            m_isRunning = false;
        default:
            break;
    }
}

void Lab::logic()
{
    if (!m_init) {
        initLogic();
        m_init = true;
    }

    labNormalCollisionDetect();
    labContinuousCollisionDetect();
    updateCorrection();
}

void Lab::initLogic()
{
    auto cir11 = m_entities["cir11"];
    // cir11->acc = { 0, 5.0f };
    cir11->velo = { 5000.0f, 5000.0f };
    cir11->pos.x = 365.0f;

    auto cir21 = m_entities["cir21"];
    // cir21->acc = { 0, 5.0f };
    cir21->velo = { 5000.0f, 5000.0f };
    cir21->pos.x = 1025.0f;
}

void Lab::movement()
{
    for (auto &iter : m_entities) {
        auto shape = iter.second->shape();
        if (shape != nullptr) {
            auto &pos = iter.second->pos;
            auto &velo = iter.second->velo;
            auto &acc = iter.second->acc;
            auto &lastPos = iter.second->lastPos;

            // Δv = aΔt
            Vec2 deltaV = acc * m_deltaT;
            velo += deltaV;
            // Δd = 1/2 vΔt
            Vec2 deltaD = velo * m_deltaT * 0.5f;
            pos += deltaD;

            lastPos = pos;
        }
    }
}

void Lab::render()
{
    m_window->clear(sf::Color::Black);
    for (auto &iter : m_entitiesSeq) {
        if (iter != nullptr) {
            auto shape = iter->shape();
            m_window->draw(*shape);
        }
    }
    m_window->display();
}

int main(int argc, char* argv[])
{
    Lab l(60);
    l.init();
    return l.run();
}