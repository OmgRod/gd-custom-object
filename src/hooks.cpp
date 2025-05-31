#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/CCSprite.hpp>
#include <GameObjectFactory.hpp>

using namespace geode::prelude;

class $modify(GameObjectHook, GameObject) {
    static GameObject* createWithKey(int object_id) {
        auto ctr = GameObjectFactory::get();

        if (auto generator = ctr->fetchGenerator(object_id)) {
            auto obj = GameObject::createWithKey(object_id);
            if (!obj) {
                obj = GameObject::createWithFrame(ctr->fetchTexture(object_id).c_str());
            }
            ctr->addContainer(obj->m_uniqueID, generator.value()(obj));

            return obj;
        } else {
            return GameObject::createWithKey(object_id);
        }
    }

    // void triggerObject(GJBaseGameLayer* gl, int p1, std::vector<int> const* p2) {
    //     if (auto mg = GameObjectFactory::get()->fetch(this)) {
    //         mg.value()->onTrigger(gl);
    //     } else {
    //         GameObject::triggerObject(gl, p1, p2);
    //     }
    // }

    void resetObject() {
        if (auto mg = GameObjectFactory::get()->fetch(this)) {
            mg.value()->onReset();
        }

        GameObject::resetObject();
    }

    gd::string getSaveString(GJBaseGameLayer* gl) {
        std::string dat(GameObject::getSaveString(gl));

        if (auto mg = GameObjectFactory::get()->fetch(this)) {
            for (auto[k, v]: mg.value()->onExport()) {
                dat += "," + std::to_string(k + 1000) + "," + v;
            }
        }

        return gd::string(dat);
    }

    static GameObject* objectFromString(gd::string a, bool b) {
        auto object = objectFromString(a, b);
        if (!object) return object;

        if (auto mg = GameObjectFactory::get()->fetch(object)) {
            std::map < uint32_t, std::string > m;
            std::string tmp;
            std::string tmp2;
            bool even = false;

            std::stringstream items;
            items << std::string(a);
            while (std::getline(items, tmp, ',')) {
                if (!even)
                    tmp2 = std::move(tmp);
                else {
                    int key = stoi(tmp2);
                    if (key > 1000) {
                        m[key - 1000] = tmp;
                    }
                }
                even = !even;
            }

            mg.value()->onImport(m);
        }

        return object;
    }

    void addGlow(gd::string p0) {
        if (auto mg = GameObjectFactory::get()->fetch(this)) {
            if (!mg.value()->isGlowEnabled())
                return;
        }

        GameObject::addGlow(p0);
    }

    void customSetup() {
        GameObject::customSetup();
        if (auto mg = GameObjectFactory::get()->fetch(this)) {
            mg.value()->setup();
        }
    }
};

class $modify(PlayerObject) {
    void collidedWithObject(float dt, GameObject* obj, CCRect r, bool p3) {
        if (auto mg = GameObjectFactory::get()->fetch(obj)) {
            mg.value()->onCollide(dt, this);
        } else {
            PlayerObject::collidedWithObject(dt, obj, r, p3);
        }
    }
};

class $modify(GJBaseGameLayer) {
	struct Fields {
		CCDictionaryExt <int, CCNode> m_effectLayerMap;
	};

    CCNode* parentForZLayer(int zLayer, bool detailChannel, int batchLayer, int p3) {
        auto elm = m_fields->m_effectLayerMap;

        if (batchLayer == -2) { // -2 is the effect layer
            if (elm.size() == 0) {
                for (int b: {
                        -3,
                        -1,
                        1,
                        3,
                        5,
                        7,
                        9,
                        11
                    }) {
                    auto node = CCNode::create();
                    elm[b] = node;

                    m_objectLayer->addChild(node, b);
                }
            }
            return elm[zLayer];
        } else {
            return GJBaseGameLayer::parentForZLayer(zLayer, detailChannel, batchLayer, p3);
        }
    }
};

class $modify(CCSprite) {
    void setDisplayFrame(CCSpriteFrame* f) {
        if (f == nullptr) {
            return;
        }

        CCSprite::setDisplayFrame(f);
    }
};