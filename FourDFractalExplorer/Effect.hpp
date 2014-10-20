#ifndef EFFECT_HPP
#define EFFECT_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <cassert>
#include <string>
#include <cmath>
#include <iostream>

#ifndef PI
#define PI 3.14159265
#endif

#define fmin(a,b) ((a)<(b)?(a):(b))
#define fmax(a,b) ((a)>(b)?(a):(b))

////////////////////////////////////////////////////////////
// Base class for effects
////////////////////////////////////////////////////////////
class Effect : public sf::Drawable
{
public :

    virtual ~Effect()
    {
    }

	 
    const std::string& getName() const
    {
        return m_name;
    }

    void load()
    {
        m_isLoaded = sf::Shader::isAvailable() && onLoad();
        frame = sf::Vector3f(0.0, 0.0, 4.0);
        m_logShading = true;
        m_almond = false;
        m_coloring = sf::Vector3f(0.0, 0.0, 0.0);
    }

    void update()
    {
        if (m_isLoaded)
            onUpdate();
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if (m_isLoaded)
        {
            onDraw(target, states);
        }
    }

    void mouseButtonPressed(sf::Event event)
    {
        if (m_isLoaded)
            onMouseButtonPress(event);
    }
    
    void mouseButtonReleased(sf::Event event)
    {
        if (m_isLoaded)
            onMouseButtonRelease(event);
    }

    void mouseMoved(sf::Event event)
    {
        if (m_isLoaded)
            onMouseMove(event);
    }

    void mouseScrolled(sf::Event event)
    {
        if (m_isLoaded)
            onMouseScroll(event);
    }

    // Getters and setters

    bool isInteracting()
    {
        return m_interacting;
    }

    bool getLogShading()
    {
        return m_logShading;
    }

    void setLogShading(bool logShading)
    {
        m_logShading = logShading;
    }

    void setFrame(sf::Vector3f newFrame)
    {
        frame = sf::Vector3f(newFrame.x, newFrame.y, newFrame.z);
    }

    void setIterationScaling(bool scale)
    {
        m_iterationsScaing = scale;        
    }

    sf::Vector3f getFrame()
    {
        return frame;
    }

    void set_almond(bool almond)
    {
        m_almond = almond;
    }

    void setColoring(sf::Vector3f coeff)
    {
        m_coloring = coeff;
    }

    void setEmulated(bool emulated)
    {
        m_emulated = emulated;
    }

    sf::Vector3f getFrame(int left, int right, int width)
    {
        // How convienent!
        float Zoom = frame.z;

        float mx = left-1.0f;
        float my = right-1.0f;

        float centerX = mx + width/2.0f;
        float centerY = my + width/2.0f;

        float centerA = ((centerX)*Zoom)/960.0f - Zoom/2.0f - frame.x;
        float centerB = ((960.0f-centerY)*Zoom)/960.0f - Zoom/2.0f - frame.y;

        float newZoom = fabs((width/960.0f)*Zoom);

        return sf::Vector3f(-centerA, -centerB, newZoom);
    }

    // Mouse event handlers
    void onMouseButtonPress(sf::Event event)
    {
        if (event.mouseButton.button == sf::Mouse::Right)
        {
            m_zoomBox.setPosition((float)event.mouseButton.x,(float)event.mouseButton.y);
            m_zoomBox.setSize(sf::Vector2f(0.f,0.f));
            m_zooming = true;
        }
        else if (event.mouseButton.button == sf::Mouse::Middle)
        {
            m_mouseDragCenter = sf::Vector2f((float)event.mouseButton.x, 
                                              (float)event.mouseButton.y);
            m_panning = true;
        }
    }

    void onMouseMove(sf::Event event)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            sf::Vector2f position = m_zoomBox.getPosition();
            float newSize = fmin(fmax(
                                event.mouseMove.x-position.x, 
                                 event.mouseMove.y-position.y),960.0f);

            m_zoomBox.setSize(sf::Vector2f(newSize,newSize));
        } 
        else if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)&& m_panning)
        {
            float distance = sqrt(pow(
                fabs(m_mouseDragCenter.x - event.mouseMove.x), 2.0f) + 
                pow(fabs(m_mouseDragCenter.y - event.mouseMove.y), 2.0f));

            // velocity = distance / 10000.0 * Zoom;

            m_panVelocity = ((distance + 1) * frame.z)/50000.f;

            m_panAngle =(float)( 2*PI - atan2 (m_mouseDragCenter.y - 
                                        event.mouseMove.y,
                                       m_mouseDragCenter.x - 
                                        event.mouseMove.x));
        }
    }

    void onMouseScroll(sf::Event event)
    {
        if (event.mouseWheel.delta < 0)
            frame.z *= 1.05f;
        else
            frame.z /= 1.05f;
    }

protected :

    Effect(const std::string& name) :
    m_name(name),
    m_isLoaded(false),
    m_emulated(false),
    m_panning(false),
    m_zooming(false)
    {
        m_zoomBox.setFillColor(sf::Color::Transparent);
        m_zoomBox.setOutlineColor(sf::Color::Green);
        m_zoomBox.setOutlineThickness(2);
    }


    // Current viewport for this fractal, has a center (X,Y) and a zoom (Z)
    sf::Vector3f frame;

    sf::Vector3f m_coloring;

    sf::RectangleShape m_zoomBox;    
    sf::Vector2f m_mouseDragCenter;

    bool m_logShading;
    bool m_almond;
    bool m_iterationsScaing;
    bool m_emulated;

    bool m_interacting;
    bool m_panning;
    bool m_zooming;
    float m_panVelocity;
    float m_panAngle;


    // Virtual functions to be implemented in derived effects
    virtual bool onLoad() = 0;
    virtual void onUpdate() = 0;
    virtual void onDraw(sf::RenderTarget& target, 
                         sf::RenderStates states) const = 0;
    virtual void onMouseButtonRelease(sf::Event event) = 0;

private :

    std::string m_name;
    bool m_isLoaded;

};

#endif // EFFECT_HPP
