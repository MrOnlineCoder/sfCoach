/*
    sf::Coach Extension
    Author: MrOnlineCoder
    github.com/MrOnlineCoder/sfCoach

    MIT License

    Copyright (c) 2017 MrOnlineCoder (Nikita Kogut)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/


#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>

using namespace std;
using namespace sf;

namespace sfCoach {

    /**
    * \struct Target
    * \brief This struct represents a target to be animated. It has own position, size, color and functions-setters.
    *
    */
    struct Target {
        //Transformable& transform;
        Color color;
        Color secondaryColor;
        std::function<void(sf::Vector2f)> setPosition;
        std::function<void(sf::Vector2f)> setSize;
        std::function<void(float)> setRotation;
        std::function<void(sf::Color)> setColor;
        std::function<void(sf::Color)> setSecondaryColor;
    };

    Target createTarget(sf::Sprite& sprite)
    {
        return Target
        {
            //sprite,
            sprite.getColor(),
            sprite.getColor(),
            [&] (sf::Vector2f pos)
            {
                sprite.setPosition(pos);
            },
            [&] (sf::Vector2f pos)
            {
               //Unsupported
            },
            [&] (float angle)
            {
                sprite.setRotation(angle);
            },
            [&] (sf::Color col)
            {
                sprite.setColor(col);
            },
            [&] (sf::Color col)
            {
                sprite.setColor(col);
            }
        };
    };

    Target createTarget(sf::CircleShape& shape)
    {
        return Target
        {
            //shape,
            shape.getFillColor(),
            shape.getOutlineColor(),
            [&] (sf::Vector2f pos)
            {
                shape.setPosition(pos);
            },
            [&] (sf::Vector2f pos)
            {
               shape.setRadius(pos.x);
            },
            [&] (float angle)
            {
               shape.setRotation(angle);
            },
            [&] (sf::Color col)
            {
                shape.setFillColor(col);
            },
            [&] (sf::Color col)
            {
                shape.setOutlineColor(col);
            },
        };
    }

    Target createTarget(sf::RectangleShape& shape)
    {
        return Target
        {
            //shape,
            shape.getFillColor(),
            shape.getOutlineColor(),
            [&] (sf::Vector2f pos)
            {
                shape.setPosition(pos);
            },
            [&] (sf::Vector2f pos)
            {
               shape.setSize(pos);
            },
            [&] (float angle)
            {
               shape.setRotation(angle);
            },
            [&] (sf::Color col)
            {
                shape.setFillColor(col);
            },
            [&] (sf::Color col)
            {
                shape.setOutlineColor(col);
            },
        };
    }

    Target createTarget(sf::Text& text)
    {
        return Target
        {
            //text,
            text.getFillColor(),
            text.getOutlineColor(),
            [&] (sf::Vector2f pos)
            {
                text.setPosition(pos);
            },
            [&] (sf::Vector2f pos)
            {
                text.setCharacterSize(pos.x);
            },
            [&] (float angle)
            {
                text.setRotation(angle);
            },
            [&] (sf::Color col)
            {
                text.setFillColor(col);
            },
            [&] (sf::Color col)
            {
                text.setOutlineColor(col);
            },
        };
    }


    /**
    * \class Animation
    * \brief The Animation itself. It just do some visual changes to a shape, text, sprite.
    */
    class Animation {
        public:

            /**
            * \fn virtual bool update()
            * \brief Updates the animation (does the logic of the animation)
            * \return Returns a boolean, which indicates should the animation be removed or not updated anymore.
            */
            virtual bool update() = 0;

    };

    /** \class DisplayAnimation
     *  \brief Toggles target visibility. "delay" is time delay before visibility change in seconds, show - should object be shown or hidden after delay.
     */

    class DisplayAnimation : public Animation {
        public:
            DisplayAnimation(Target target, float delay, bool show = false) {
                m_target = target;
                m_delay = delay;
                m_show = show;
            };
            bool update() {
                if (m_clock.getElapsedTime().asSeconds() >= m_delay) {
                    m_target.setColor(Color(255,255,255,m_show * 255));
                    return true;
                }

                return false;
            }

        private:
            Target m_target;
            Clock m_clock;
            float m_delay;
            bool m_show;
    };


    /**
     *  \class FadeAnimation
     *  \brief Fade animation. Fades in/out (fadeIn param) the target in time.
     */

    class FadeAnimation : public Animation {
        public:
            FadeAnimation(Target target, float time, bool fadeIn = false) {
                m_target = target;
                if (fadeIn) {
                    m_way = 1;
                } else {
                    m_way = -1;
                }

                m_color = m_target.color;
                m_step = (time * 1000) / 255; // Time is given is seconds, we get the timestep for 1 alpha point.
                m_steps = 0;
            };
            bool update() {
                if (!m_flag) {
                    sf::Color c(m_target.color.toInteger());
                    if (m_way == 1) {
                       c.a = 0;
                    } else {
                        c.a = 255;
                    }
                    m_target.setColor(c);
                    m_target.setSecondaryColor(c);
                    m_flag = true;
                }
                if (m_clock.getElapsedTime().asMilliseconds() >= m_step) {
                    m_color.a += m_way;
                    m_target.setColor(m_color);
                    m_target.setSecondaryColor(m_color);
                    m_clock.restart();
                    m_steps++;
                }

                return (m_steps >= 255);
            }

        private:
            Target m_target;
            Clock m_clock;
            float m_time;
            long m_step;
            int m_way;
            Color m_color;
            int m_steps;
            bool m_flag;
    };

    /**
     *  \class MoveAnimation
     *  \brief Smoothly moves the target by constant speed.
     */

    class MoveAnimation : public Animation {
        public:
            MoveAnimation(Target target, Vector2f start, Vector2f end, float speed) {
                m_target = target;
                m_factor = 0.0f;
                m_speed = speed;
                m_target.setPosition(start);
                m_start = start;
                m_end = end;
            };
            bool update() {
                float delta = m_clock.restart().asSeconds();
                m_factor += delta * m_speed;
                Vector2f newPos = m_start + (m_end - m_start) * m_factor;
                m_target.setPosition(newPos);

                return (m_factor >= 1.0f);
            }

        private:
            Target m_target;
            Clock m_clock;
            float m_factor;
            float m_speed;
            Vector2f m_start;
            Vector2f m_end;
    };

    class Coach {
        public:
            void playAnimation(Animation* animation) {
                m_animations.push_back(animation);
            }

            void stopAllAnimations() {
                m_animations.clear();
            }

            void update() {
                for(size_t i = 0; i < m_animations.size(); i++) {
                    if(m_animations[i]->update()) {
                        m_animations.erase(m_animations.begin()+i);
                    }
                 }
            }
        private:
            vector<Animation*> m_animations;
    };
}

