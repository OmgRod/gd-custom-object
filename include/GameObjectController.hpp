#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class GameObjectController {
public:
    struct Fields {
        GameObject* m_object;
        bool m_glowEnabled;
    };

    GameObjectController(GameObject* g)
        : m_fields(std::make_unique<Fields>(Fields{ g, false })) {}

    // These are for saving custom data for objects
    inline virtual std::map<uint32_t, std::string> onExport() {
        return std::map<uint32_t, std::string>();
    }

    inline virtual void onImport(std::map<uint32_t, std::string> data) {}

    // This happens when the object is reset, typically when the game resets
    inline virtual void onReset() {}

    // This happens when the object is "triggered", like a move trigger
    inline virtual void onTrigger(GJBaseGameLayer* gl) {}

    // This happens when the player collides with the object
    inline virtual void onCollide(float dt, PlayerObject* pl) {
        pl->collidedWithObject(dt, m_fields->m_object, {}, false);
    }

    // All setup goes here
    inline virtual void setup() {}

    inline bool isGlowEnabled() {
        return m_fields->m_glowEnabled;
    }

    // Simple getters and setters for the underlying GameObject
    inline GameObject* getObject() {
        return m_fields->m_object;
    }

    inline void setObject(GameObject* g) {
        m_fields->m_object = g;
    }

    // Setup custom sprite frame name
    inline void overrideSpriteFrame(std::string const& fname) {
        m_fields->m_object->m_parentMode = -2;

        auto cache = CCSpriteFrameCache::sharedSpriteFrameCache();
        auto frame = cache->spriteFrameByName(fname.c_str());

        m_fields->m_object->setDisplayFrame(frame);
        m_fields->m_object->m_lastSize = CCSizeMake( // m_objectSize
            frame->getRect().size.width,
            frame->getRect().size.height
        );

        m_fields->m_object->m_particleString = fname;
    }

    // Set custom texture
    inline void overrideTexture(CCTexture2D* texture) {
        m_fields->m_object->m_parentMode = -2;
        m_fields->m_object->setTexture(texture);
        m_fields->m_object->setTextureRect(CCRectMake(
            0, 0,
            texture->getContentSize().width,
            texture->getContentSize().height
        ));
        m_fields->m_object->m_lastSize = CCSizeMake( // m_objectSize
            texture->getContentSize().width,
            texture->getContentSize().height
        );
    }
	std::unique_ptr<Fields> m_fields;
};
