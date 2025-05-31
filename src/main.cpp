#include <Geode/Geode.hpp>
#include <GameObjectFactory.hpp>

using namespace geode::prelude;

// Create new object
class TestObject: public GameObjectController {
    public: TestObject(GameObject* g): GameObjectController(g) {}

    // What happens when the object is "triggered"
    void onTrigger(GJBaseGameLayer* gl) override {
        auto r = CCScaleTo::create(1.0, 1.25f, 0.75f);
        gl->m_objectLayer->runAction(r);
    }

    void setup() override {
        // Disable glow because there is no glow texture for this sprite
        m_fields->m_glowEnabled = false;

        // Set custom texture for this object
        overrideSpriteFrame("GJ_arrow_01_001.png");

        // Touch-triggered object with the Modifier type. This object will run onTrigger when collided with
        // m_fields->m_object->m_touchTriggered = true;
        m_fields->m_object->m_objectType = GameObjectType::Modifier;
    }
};

$on_mod(Loaded) {
    // Register our custom object with an Object ID of 1. This overrides the existing object.
    GameObjectFactory::get()->add(1, [](GameObject* g) {
        return new TestObject(g);
    });
}