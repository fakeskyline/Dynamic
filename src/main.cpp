#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/SimplePlayer.hpp>
using namespace geode::prelude;

namespace {
enum class Mode { Cube, Ship, Ball, Ufo, Wave, Robot, Spider, Swing, Jetpack };
constexpr char const* NAMES[] = {"Cube","Ship","Ball","UFO","Wave","Robot","Spider","Swing","Jetpack"};
constexpr int COUNT = 9, PER_PAGE = 20;

IconType typeOf(Mode m) {
    switch (m) {
        case Mode::Cube: return IconType::Cube;
        case Mode::Ship: return IconType::Ship;
        case Mode::Ball: return IconType::Ball;
        case Mode::Ufo: return IconType::Ufo;
        case Mode::Wave: return IconType::Wave;
        case Mode::Robot: return IconType::Robot;
        case Mode::Spider: return IconType::Spider;
        case Mode::Swing: return IconType::Swing;
        case Mode::Jetpack: return IconType::Jetpack;
    }
    return IconType::Cube;
}
int limitOf(Mode m) {
    constexpr int limits[] = {484,169,118,149,96,68,69,43,5};
    return limits[static_cast<int>(m)];
}
int selected(Mode m) {
    auto g = GameManager::sharedState();
    switch (m) {
        case Mode::Cube: return g->m_playerFrame;
        case Mode::Ship: return g->m_playerShip;
        case Mode::Ball: return g->m_playerBall;
        case Mode::Ufo: return g->m_playerBird;
        case Mode::Wave: return g->m_playerDart;
        case Mode::Robot: return g->m_playerRobot;
        case Mode::Spider: return g->m_playerSpider;
        case Mode::Swing: return g->m_playerSwing;
        case Mode::Jetpack: return g->m_playerJetpack;
    }
    return 1;
}
void apply(Mode m, int id) {
    auto g = GameManager::sharedState();
    switch (m) {
        case Mode::Cube: g->setPlayerFrame(id); break;
        case Mode::Ship: g->setPlayerShip(id); break;
        case Mode::Ball: g->setPlayerBall(id); break;
        case Mode::Ufo: g->setPlayerBird(id); break;
        case Mode::Wave: g->setPlayerDart(id); break;
        case Mode::Robot: g->setPlayerRobot(id); break;
        case Mode::Spider: g->setPlayerSpider(id); break;
        case Mode::Swing: g->setPlayerSwing(id); break;
        case Mode::Jetpack: g->setPlayerJetpack(id); break;
    }
}

class EditorLayer : public CCLayerColor {
    Mode m_mode = Mode::Cube;
    int m_page = 0;
    CCMenu* m_grid = nullptr;
    CCLabelBMFont* m_info = nullptr;
    CCLabelBMFont* m_pageText = nullptr;

    bool init() {
        auto s = CCDirector::sharedDirector()->getWinSize();
        if (!CCLayerColor::initWithColor({0,0,0,205})) return false;
        auto panel = CCLayerColor::create({45,49,62,255}, 330.f, 245.f);
        panel->setPosition({(s.width-330.f)/2.f,(s.height-245.f)/2.f});
        addChild(panel,1);

        auto title = CCLabelBMFont::create("DYNAMIC ICON", "bigFont.fnt");
        title->setScale(.55f);
        title->setPosition({s.width/2.f,panel->getPositionY()+222.f});
        addChild(title,2);

        auto close = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("X",30,true,"goldFont.fnt","GJ_button_01.png",28.f,1.f),
            this, menu_selector(EditorLayer::onClose));
        auto closeMenu = CCMenu::create();
        closeMenu->addChild(close);
        closeMenu->setPosition({panel->getPositionX()+310.f,panel->getPositionY()+222.f});
        addChild(closeMenu,3);

        auto modes = CCMenu::create();
        modes->setPosition({s.width/2.f,panel->getPositionY()+190.f});
        for (int i=0;i<COUNT;i++) {
            auto label=CCLabelBMFont::create(NAMES[i],"goldFont.fnt");
            label->setScale(.34f);
            auto item=CCMenuItemSpriteExtra::create(label,this,menu_selector(EditorLayer::onMode));
            item->setTag(i);
            item->setPosition({-143.f+(i%5)*72.f,i<5?0.f:-25.f});
            modes->addChild(item);
        }
        addChild(modes,2);

        m_info=CCLabelBMFont::create("Cube","bigFont.fnt");
        m_info->setScale(.42f);
        m_info->setPosition({s.width/2.f,panel->getPositionY()+150.f});
        addChild(m_info,2);

        auto left=CCMenuItemSpriteExtra::create(ButtonSprite::create("<",30,true,"goldFont.fnt","GJ_button_01.png",28.f,1.f),this,menu_selector(EditorLayer::onPrev));
        auto right=CCMenuItemSpriteExtra::create(ButtonSprite::create(">",30,true,"goldFont.fnt","GJ_button_01.png",28.f,1.f),this,menu_selector(EditorLayer::onNext));
        auto pages=CCMenu::create();
        pages->addChild(left); pages->addChild(right);
        left->setPosition({-65.f,0.f}); right->setPosition({65.f,0.f});
        pages->setPosition({s.width/2.f,panel->getPositionY()+24.f});
        addChild(pages,2);
        m_pageText=CCLabelBMFont::create("1 / 1","goldFont.fnt");
        m_pageText->setScale(.38f);
        m_pageText->setPosition({s.width/2.f,panel->getPositionY()+24.f});
        addChild(m_pageText,2);
        refresh();
        return true;
    }
    void refresh() {
        if (m_grid) { m_grid->removeFromParentAndCleanup(true); m_grid=nullptr; }
        auto s=CCDirector::sharedDirector()->getWinSize();
        auto gm=GameManager::sharedState();
        int max=limitOf(m_mode), pages=(max+PER_PAGE-1)/PER_PAGE;
        m_page=std::max(0,std::min(m_page,pages-1));
        m_pageText->setString(fmt::format("{} / {}",m_page+1,pages).c_str());
        m_info->setString(fmt::format("{}  •  Selected: {}",NAMES[static_cast<int>(m_mode)],selected(m_mode)).c_str());
        m_grid=CCMenu::create();
        m_grid->setPosition({s.width/2.f,s.height/2.f-8.f});
        for (int slot=0;slot<PER_PAGE;slot++) {
            int id=m_page*PER_PAGE+slot+1;
            if (id>max || !gm->isIconUnlocked(id,typeOf(m_mode))) continue;
            auto sprite=SimplePlayer::create(id,typeOf(m_mode));
            if (!sprite) continue;
            sprite->setScale(.58f);
            auto item=CCMenuItemSpriteExtra::create(sprite,this,menu_selector(EditorLayer::onSelect));
            item->setTag(id);
            item->setPosition({-126.f+(slot%5)*63.f,70.f-(slot/5)*43.f});
            m_grid->addChild(item);
        }
        addChild(m_grid,4);
    }
    void onMode(CCObject* o) { m_mode=static_cast<Mode>(o->getTag()); m_page=0; refresh(); }
    void onSelect(CCObject* o) { apply(m_mode,o->getTag()); refresh(); }
    void onPrev(CCObject*) { if(m_page>0)--m_page; refresh(); }
    void onNext(CCObject*) { if(m_page+1<(limitOf(m_mode)+PER_PAGE-1)/PER_PAGE)++m_page; refresh(); }
    void onClose(CCObject*) { removeFromParentAndCleanup(true); }
public:
    static EditorLayer* create() {
        auto p=new EditorLayer();
        if(p && p->init()){p->autorelease();return p;}
        CC_SAFE_DELETE(p); return nullptr;
    }
};

class $modify(DynamicPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();
        auto sprite=ButtonSprite::create("ICONS",50,true,"goldFont.fnt","GJ_button_01.png",30.f,.8f);
        auto item=CCMenuItemSpriteExtra::create(sprite,this,menu_selector(DynamicPauseLayer::openEditor));
        auto menu=CCMenu::create(); menu->addChild(item);
        menu->setPosition({CCDirector::sharedDirector()->getWinSize().width/2.f,38.f});
        addChild(menu,100);
    }
    void openEditor(CCObject*) {
        if(auto editor=EditorLayer::create()) addChild(editor,1000);
    }
};
}
