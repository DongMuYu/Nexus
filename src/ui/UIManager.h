#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <memory>
#include <SFML/Graphics.hpp>
#include "../particles/CoreParticle.h"
#include "../utils/Math/Vector2.h"
#include "../particles/ParticleSystem.h"

class UIManager {
private:
    std::shared_ptr<CoreParticle> selectedParticle;
    bool fontLoaded;
    bool showStats;
    bool showHelp;
    sf::Font font;
    sf::Font defaultFont;
    sf::Text helpText;
    sf::Text statsText;
    sf::Text particleInfoText;
    sf::RenderWindow& window;
    ParticleSystem& particleSystem;
    
    void initializeUIElements();
    void updateStats();
    void updateParticleInfo();
    
public:
    UIManager(sf::RenderWindow& window, ParticleSystem& ps);
    ~UIManager();
    
    void handleEvent(const sf::Event& event);
    void update(float deltaTime = 0.0f);
    void render(sf::RenderWindow& window);
    
    void selectParticleAtPosition(sf::Vector2f position);
    void renderAdditionalUI(sf::RenderWindow& window);
    
    void setShowHelp(bool show);
    bool getShowHelp() const;
    
    void setShowStats(bool show);
    bool getShowStats() const;
    
    void setSelectedParticle(std::shared_ptr<CoreParticle> particle);
    std::shared_ptr<CoreParticle> getSelectedParticle() const;
    
    bool isFontLoaded() const;
};

#endif // UI_MANAGER_H