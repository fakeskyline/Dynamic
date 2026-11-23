#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GameManager.hpp>
#include <Geode/modify/GameStatsManager.hpp>

using namespace geode::prelude;

namespace {
bool s_openedFromPause = false;
bool s_pauseFreezeActive = false;

void returnToPause() {
    if (!s_openedFromPause) return;

    s_openedFromPause = false;
    CCDirector::get()->popScene();

    // Keep the level frozen. The pause layer remains underneath the garage scene.
    // Gameplay is unfrozen only when the player explicitly presses Resume.
    if (auto playLayer = PlayLayer::get()) {
        auto gm = GameManager::sharedState();
        auto icon = gm->activeIconForType(gm->m_playerIconType);

        if (playLayer->m_player1) {
            playLayer->m_player1->updatePlayerFrame(icon);
        }
        if (playLayer->m_player2) {
            playLayer->m_player2->updatePlayerFrame(icon);
        }
    }
}
}

class $modify(DynamicPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto sprite = CCSprite::createWithSpriteFrameName("GJ_garageBtn_001.png");
        if (!sprite) return;
        sprite->setScale(0.8f);

        auto button = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            menu_selector(DynamicPauseLayer::openOfficialGarage)
        );
        button->setID("dynamic-icon-garage-button");

        auto menu = CCMenu::create();
        menu->addChild(button);
        menu->setPosition({36.f, 36.f});
        menu->setID("dynamic-icon-garage-menu");
        addChild(menu, 100);
    }

    void openOfficialGarage(CCObject*) {
        s_openedFromPause = true;
        s_pauseFreezeActive = true;

        auto scene = GJGarageLayer::scene();
        CCDirector::get()->pushScene(CCTransitionFade::create(0.25f, scene));
    }

    void onResume(CCObject* sender) {
        s_pauseFreezeActive = false;
        PauseLayer::onResume(sender);
    }
};

class $modify(DynamicGarageLayer, GJGarageLayer) {
    void onBack(CCObject* sender) {
        if (s_openedFromPause) {
            returnToPause();
            return;
        }
        GJGarageLayer::onBack(sender);
    }

    void keyBackClicked() {
        if (s_openedFromPause) {
            returnToPause();
            return;
        }
        GJGarageLayer::keyBackClicked();
    }
};

// Freeze the actual gameplay update loop. Pausing CCNode actions alone does not
// reliably stop GJBaseGameLayer physics while another scene is pushed.
class $modify(DynamicBaseGameLayer, GJBaseGameLayer) {
    void update(float dt) {
        if (s_pauseFreezeActive && PlayLayer::get()) return;
        GJBaseGameLayer::update(dt);
    }
};

class $modify(DynamicUnlockGameManager, GameManager) {
    bool isIconUnlocked(int iconID, IconType type) {
        return true;
    }

    bool isColorUnlocked(int colorID, UnlockType type) {
        return true;
    }
};

class $modify(DynamicUnlockStatsManager, GameStatsManager) {
    bool isItemUnlocked(UnlockType type, int itemID) {
        // Practice Music Sync is GJItem 17 in Geometry Dash.
        if (type == UnlockType::GJItem && itemID == 17) return true;
        return GameStatsManager::isItemUnlocked(type, itemID);
    }
};

class $modify(DynamicPlayLayer, PlayLayer) {
    void resetLevel() {
        s_pauseFreezeActive = false;
        PlayLayer::resetLevel();
    }

    void onQuit() {
        s_pauseFreezeActive = false;
        s_openedFromPause = false;
        PlayLayer::onQuit();
    }
};
