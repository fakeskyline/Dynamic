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

void applyCurrentLoadout(PlayerObject* player) {
    if (!player) return;

    auto gm = GameManager::sharedState();
    auto type = gm->m_playerIconType;
    auto icon = gm->activeIconForType(type);

    switch (type) {
        case IconType::Cube:   player->updatePlayerFrame(icon); break;
        case IconType::Ship:   player->updatePlayerShipFrame(icon); break;
        case IconType::Ball:   player->updatePlayerRollFrame(icon); break;
        case IconType::Ufo:    player->updatePlayerBirdFrame(icon); break;
        case IconType::Wave:   player->updatePlayerDartFrame(icon); break;
        case IconType::Robot:  player->updatePlayerRobotFrame(icon); break;
        case IconType::Spider: player->updatePlayerSpiderFrame(icon); break;
        case IconType::Swing:  player->updatePlayerSwingFrame(icon); break;
        default:               player->updatePlayerFrame(icon); break;
    }

    player->setColor(gm->colorForIdx(gm->getPlayerColor()));
    player->setSecondColor(gm->colorForIdx(gm->getPlayerColor2()));
    player->updatePlayerGlow();
    player->updateGlowColor();
}

void returnToPause() {
    if (!s_openedFromPause) return;

    s_openedFromPause = false;
    CCDirector::get()->popScene();

    // Apply the garage's saved selection to both live players without resuming gameplay.
    if (auto playLayer = PlayLayer::get()) {
        applyCurrentLoadout(playLayer->m_player1);
        applyCurrentLoadout(playLayer->m_player2);
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

// Freeze the actual gameplay update loop while the official garage is open.
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
