#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GameManager.hpp>
#include <Geode/modify/GameStatsManager.hpp>

using namespace geode::prelude;

namespace {
bool garageOpenedFromPause = false;
bool gameplayFrozen = false;

void applyPlayerAppearance(PlayerObject* player) {
    if (!player) return;

    auto gameManager = GameManager::sharedState();
    IconType iconType = IconType::Cube;

    if (player->m_isShip) {
        iconType = IconType::Ship;
    } else if (player->m_isBall) {
        iconType = IconType::Ball;
    } else if (player->m_isBird) {
        iconType = IconType::Ufo;
    } else if (player->m_isDart) {
        iconType = IconType::Wave;
    } else if (player->m_isRobot) {
        iconType = IconType::Robot;
    } else if (player->m_isSpider) {
        iconType = IconType::Spider;
    } else if (player->m_isSwing) {
        iconType = IconType::Swing;
    }

    auto iconID = gameManager->activeIconForType(iconType);

    switch (iconType) {
        case IconType::Cube:
            player->updatePlayerFrame(iconID);
            break;
        case IconType::Ship:
            player->updatePlayerShipFrame(iconID);
            break;
        case IconType::Ball:
            player->updatePlayerRollFrame(iconID);
            break;
        case IconType::Ufo:
            player->updatePlayerBirdFrame(iconID);
            break;
        case IconType::Wave:
            player->updatePlayerDartFrame(iconID);
            break;
        case IconType::Robot:
            player->updatePlayerRobotFrame(iconID);
            break;
        case IconType::Spider:
            player->updatePlayerSpiderFrame(iconID);
            break;
        case IconType::Swing:
            player->updatePlayerSwingFrame(iconID);
            break;
        default:
            player->updatePlayerFrame(iconID);
            break;
    }

    player->setColor(gameManager->colorForIdx(gameManager->getPlayerColor()));
    player->setSecondColor(gameManager->colorForIdx(gameManager->getPlayerColor2()));
    player->updatePlayerGlow();
    player->updateGlowColor();
}

void returnToPauseMenu() {
    if (!garageOpenedFromPause) return;

    garageOpenedFromPause = false;
    CCDirector::get()->popScene();

    if (auto playLayer = PlayLayer::get()) {
        applyPlayerAppearance(playLayer->m_player1);
        applyPlayerAppearance(playLayer->m_player2);
    }
}
}

class $modify(PauseLayerHook, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto garageSprite = CCSprite::createWithSpriteFrameName("GJ_garageBtn_001.png");
        if (!garageSprite) return;

        garageSprite->setScale(0.8f);

        auto garageButton = CCMenuItemSpriteExtra::create(
            garageSprite,
            this,
            menu_selector(PauseLayerHook::openGarage)
        );
        garageButton->setID("dynamic-icon-garage-button");

        auto garageMenu = CCMenu::create();
        garageMenu->addChild(garageButton);
        garageMenu->setPosition({36.f, 36.f});
        garageMenu->setID("dynamic-icon-garage-menu");
        addChild(garageMenu, 100);
    }

    void openGarage(CCObject*) {
        garageOpenedFromPause = true;
        gameplayFrozen = true;

        auto garageScene = GJGarageLayer::scene();
        CCDirector::get()->pushScene(CCTransitionFade::create(0.25f, garageScene));
    }

    void onResume(CCObject* sender) {
        gameplayFrozen = false;
        PauseLayer::onResume(sender);
    }
};

class $modify(GarageLayerHook, GJGarageLayer) {
    void onBack(CCObject* sender) {
        if (garageOpenedFromPause) {
            returnToPauseMenu();
            return;
        }

        GJGarageLayer::onBack(sender);
    }

    void keyBackClicked() {
        if (garageOpenedFromPause) {
            returnToPauseMenu();
            return;
        }

        GJGarageLayer::keyBackClicked();
    }
};

class $modify(BaseGameLayerHook, GJBaseGameLayer) {
    void update(float dt) {
        if (gameplayFrozen && PlayLayer::get()) return;
        GJBaseGameLayer::update(dt);
    }
};

class $modify(GameManagerHook, GameManager) {
    bool isIconUnlocked(int iconID, IconType iconType) {
        return true;
    }

    bool isColorUnlocked(int colorID, UnlockType unlockType) {
        return true;
    }
};

class $modify(GameStatsManagerHook, GameStatsManager) {
    bool isItemUnlocked(UnlockType unlockType, int itemID) {
        if (unlockType == UnlockType::GJItem && itemID == 17) return true;
        return GameStatsManager::isItemUnlocked(unlockType, itemID);
    }
};

class $modify(PlayLayerHook, PlayLayer) {
    void resetLevel() {
        gameplayFrozen = false;
        PlayLayer::resetLevel();
    }

    void onQuit() {
        gameplayFrozen = false;
        garageOpenedFromPause = false;
        PlayLayer::onQuit();
    }
};
