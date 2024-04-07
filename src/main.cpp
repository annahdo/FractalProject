#include <SFML/Graphics.hpp>
#include <string>
#include "utils/event_manager.hpp"
#include "utils/binary_io.hpp"
#include "async_renderer.hpp"
#include "config.hpp"
#include <iostream>
int main()
{
    const sf::Vector2u window_size{1000, 1000};
    sf::RenderWindow window(sf::VideoMode(window_size.x, window_size.y), "Fractal", sf::Style::Default);
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);
    window.setKeyRepeatEnabled(false);

    sfev::EventManager event_manager{window, true};
    event_manager.addKeyReleasedCallback(sf::Keyboard::Escape, [&window](sfev::CstEv) {
        window.close();
    });
    event_manager.addEventCallback(sf::Event::Closed, [&window](sfev::CstEv) {
        window.close();
    });

    const auto                     zoom_factor = static_cast<Config::FloatType>(1.009);
    const auto                     speed       = static_cast<Config::FloatType>(1.0);
    auto                           zoom        = static_cast<Config::FloatType>(window.getSize().y) / 2;
    auto                           mouse_wheel_delta = static_cast<Config::FloatType>(0.0);  
    sf::Vector2<Config::FloatType> center      {0.0, 0.0};

    AsyncRenderer<Config::FloatType> renderer{window_size.x, window_size.y, zoom};

    bool zoom_in  = false;
    bool zoom_out = false;
    bool left     = false;
    bool right    = false;
    bool up       = false;
    bool down     = false;
    bool needs_update = true;

    event_manager.addEventCallback(sf::Event::MouseWheelScrolled, [&](sfev::CstEv event) { 
        mouse_wheel_delta = event.mouseWheelScroll.delta;
    });

    event_manager.addKeyPressedCallback(sf::Keyboard::A,      [&](sfev::CstEv) { zoom_in  = true;  });
    event_manager.addKeyPressedCallback(sf::Keyboard::E,      [&](sfev::CstEv) { zoom_out = true;  });
    event_manager.addKeyReleasedCallback(sf::Keyboard::A,     [&](sfev::CstEv) { zoom_in  = false; });
    event_manager.addKeyReleasedCallback(sf::Keyboard::E,     [&](sfev::CstEv) { zoom_out = false; });
    event_manager.addKeyPressedCallback(sf::Keyboard::Left,   [&](sfev::CstEv) { left     = true;  });
    event_manager.addKeyPressedCallback(sf::Keyboard::Right,  [&](sfev::CstEv) { right    = true;  });
    event_manager.addKeyPressedCallback(sf::Keyboard::Up,     [&](sfev::CstEv) { up       = true;  });
    event_manager.addKeyPressedCallback(sf::Keyboard::Down,   [&](sfev::CstEv) { down     = true;  });
    event_manager.addKeyReleasedCallback(sf::Keyboard::Left,  [&](sfev::CstEv) { left     = false; });
    event_manager.addKeyReleasedCallback(sf::Keyboard::Right, [&](sfev::CstEv) { right    = false; });
    event_manager.addKeyReleasedCallback(sf::Keyboard::Up,    [&](sfev::CstEv) { up       = false; });
    event_manager.addKeyReleasedCallback(sf::Keyboard::Down,  [&](sfev::CstEv) { down     = false; });

    event_manager.addKeyReleasedCallback(sf::Keyboard::W, [&](sfev::CstEv) {
        BinaryWriter writer{"center.bin"};
        writer.write(center);
    });
    event_manager.addKeyReleasedCallback(sf::Keyboard::R, [&](sfev::CstEv) {
        BinaryReader reader{"center.bin"};
        reader.readInto(center);
    });

    BinaryReader reader{"center.bin"};
    if (reader) {
        reader.readInto(center);
    }

    while (window.isOpen())
    {
        event_manager.processEvents();

        const Config::FloatType offset = speed / zoom;
        if (zoom_in) {
            zoom *= zoom_factor;  // Multiplying zoom by zoom_factor
        } else if (zoom_out) {
            zoom /= zoom_factor;  // Dividing zoom by zoom_factor
        } else {
            // zoom remains unchanged
        }
        if (mouse_wheel_delta != 0.0) {
            // print mouse_wheel_delta
            std::cout << "mouse_wheel_delta: " << mouse_wheel_delta << std::endl;
            zoom *= std::pow(zoom_factor, mouse_wheel_delta);
            mouse_wheel_delta = 0.0;
        }        // Auto zoom
        //zoom *= 1.0012;
        center.x += left ? -offset : (right ? offset : Config::FloatType{});
        center.y += up   ? -offset : (down  ? offset : Config::FloatType{});


        window.clear(sf::Color::Black);
        renderer.render(zoom, center, window);
        window.display();
    }

    return 0;
}
