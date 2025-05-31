#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

const float WIDTH = 800, HEIGHT = 600;
const float SCALE = 100; // Пикселей на метр
const float ORIGIN_X = WIDTH / 4, ORIGIN_Y = HEIGHT / 2;

// Параметры системы
const float mass = 1.0f;
const float k = 10.0f;
const float damping = 0.5f;
const float dt = 0.016f; // ~60 FPS

// Начальные условия
float pos = 0.5f;
float vel = 0.0f;
float time = 0.0f;

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Spring-Mass System");
    window.setFramerateLimit(60);

    // Создаем объекты для отрисовки
    sf::CircleShape ball(20);
    ball.setFillColor(sf::Color::Blue);
    ball.setOrigin(20, 20);

    sf::VertexArray spring(sf::LineStrip, 21);
    for (auto& point : spring) point.color = sf::Color::White;

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) return 1;
    sf::Text info("", font, 16);
    info.setPosition(10, 10);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Физическое обновление
        float force = -k * pos - damping * vel;
        float accel = force / mass;
        vel += accel * dt;
        pos += vel * dt;
        time += dt;

        // Отрисовка
        window.clear();

        // Пружина (простая волна)
        for (int i = 0; i < spring.getVertexCount(); i++) {
            float x = ORIGIN_X + i * (pos * SCALE) / (spring.getVertexCount() - 1);
            float y = ORIGIN_Y + 10 * sin(i * 0.3f);
            spring[i].position = sf::Vector2f(x, y);
        }
        window.draw(spring);

        // Шарик
        ball.setPosition(ORIGIN_X + pos * SCALE, ORIGIN_Y);
        window.draw(ball);

        // Информация
        info.setString(
            "Time: " + std::to_string(time) + " s\n" +
            "Position: " + std::to_string(pos) + " m\n" +
            "Velocity: " + std::to_string(vel) + " m/s"
        );
        window.draw(info);

        window.display();
    }

    return 0;
}