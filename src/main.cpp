#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/GJGarageLayer.hpp>

using namespace geode::prelude;

namespace {
bool s_openedFromPause = false;
PlayLayer* s_frozenPlayLayer = nullptr;

void returnToPause() {
    if (!s_openedFromPause) return;

    s_openedFromPause = false;

    CCDirector::get()->popScene();

    if (s_frozenPlayLayer) {
        s_frozenPlayLayer->resumeSchedulerAndActions();
        s_frozenPlayLayer = nullptr;
    }

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
        // Freeze the complete gameplay node tree while the official garage is open.
        if (auto playLayer = PlayLayer::get()) {
            s_frozenPlayLayer = playLayer;
            s_frozenPlayLayer->pauseSchedulerAndActions();
        }

        s_openedFromPause = true;
        auto scene = GJGarageLayer::scene();
        CCDirector::get()->pushScene(CCTransitionFade::create(0.25f, scene));
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
