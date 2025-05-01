#ifndef __SPRITEPLUS_H__
#define __SPRITEPLUS_H__


#ifndef CC_DLL_DEFINED
    #define CC_DLL
#endif

#include <cocos2d.h>

#include "CCArray.hpp"

// This was originally reverse engineered and then ported and optimized to 4.0 by Tecro.

/// @brief A Following Sprite Object that can connect and disconnect like attachment blocks.
class SpritePlus : public cocos2d::Sprite {

    CCArray<cocos2d::Sprite> m_followers;
    SpritePlus* m_followingSprite;
    uint8_t m_spritePlusFlags;
public:
    enum {
        SPRITE_PLUS_NONE = 0,
        SPRITE_PLUS_HAS_FOLLOWER = 1,
        SPRITE_PLUS_PROPOGATE_FLIP_CHANGES = 2
    };

    SpritePlus(): 
        m_followingSprite(nullptr), 
        m_followers(0),
        m_spritePlusFlags(SPRITE_PLUS_NONE){
    }

    // @brief see if any followers were attached to the sprite
    virtual bool hasFollower(){
        return m_spritePlusFlags |= SPRITE_PLUS_HAS_FOLLOWER;
    }
    
    // @brief Sees if Fip Propagation has been enabled.
    virtual bool getFlipPropagation(){
        return m_spritePlusFlags & SPRITE_PLUS_PROPOGATE_FLIP_CHANGES;
    }
    
    // @brief Enables / Disables Followers and children from fipping over the X-Y Axis
    virtual void setFlipPropagation(bool val){
        if (val) {
            m_spritePlusFlags |= SPRITE_PLUS_PROPOGATE_FLIP_CHANGES;
        } else {
            m_spritePlusFlags &= (~SPRITE_PLUS_PROPOGATE_FLIP_CHANGES);
        }
    }

    // @brief the quad according the rotation, position, scale values.
    virtual void updateTransform() override {
        for (ssize_t i = 0; i < m_followers.count(); i++)
            m_followers[i]->updateTransform();
    }

    /// @brief Adds a follower to the sprite
    /// @param follower Something that will follow this sprite
    void addFollower(cocos2d::Sprite* follower){
        m_spritePlusFlags |= SPRITE_PLUS_HAS_FOLLOWER;
        m_followers.appendObject(follower);
    }
    
    /// @brief Creates a sprite by a given sprite frame 
    /// @param frame the frame to give to the specific sprite
    /// @return SpritePlus object and returns null if initialization fails
    static SpritePlus* createWithSpriteFrame(cocos2d::SpriteFrame* frame){
        SpritePlus* pRet = new SpritePlus;
        if ((frame != nullptr) && pRet->initWithSpriteFrame(frame)){
            pRet->autorelease();
            return pRet;
        }
        CC_SAFE_DELETE(pRet);
        return nullptr;
    }

    
    /// @brief Creates a sprite by a given sprite's frame name
    /// @param frame the frame or filename to give to the specific sprite
    /// @return SpritePlus object and returns null if initialization fails
    static SpritePlus* createWithSpriteFrameName(const std::string &frameName){
        return createWithSpriteFrame(cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
    }


    /// @brief attaches a sprite to the sprite. and other way around
    /// you can think of this as attaching 2 lego blocks together.  
    /// @param sprite the sprite to attach to the main sprite and vice versa
    void followSprite(SpritePlus* sprite){
        m_followingSprite = sprite;
        sprite->addFollower(sprite);
    }
    
    /// @brief Obtains the first following sprite if it has a follower on hand
    /// @return nullptr if there is no follower avalible
    SpritePlus* getFollower(){
        return (m_spritePlusFlags & SPRITE_PLUS_HAS_FOLLOWER) ? reinterpret_cast<SpritePlus*>(m_followers[0]) : nullptr;
    }
    
    /// @brief Removes a specific following sprite from this sprite.
    /// @param sprite follower sprite that will be removed.
    void removeFollower(cocos2d::Sprite* sprite){
        if (m_followers.count()){
            m_followers.removeObject(sprite);
            if (!m_followers.count())
                m_spritePlusFlags &= (~SPRITE_PLUS_HAS_FOLLOWER);
        }
    }
    
    /// @brief Stops this child sprite from following it's given parent 
    void stopFollow(){
        if (m_followingSprite != nullptr) {
            m_followingSprite->removeFollower(this);
        }
    }
    
    /// @brief sets X's scale on itself and it's followers
    /// @param fScaleX the scale of X to set
    virtual void setScaleX(float fScaleX){
        cocos2d::Sprite::setScaleX(fScaleX);
        if (m_spritePlusFlags & SPRITE_PLUS_HAS_FOLLOWER){
            for (ssize_t i = 0; i < m_followers.count(); i++){
                m_followers[i]->setScaleX(fScaleX);
            }
        }
    }

    /// @brief sets Y's scale on itself and it's followers
    /// @param fScaleY the scale of Y to set
    virtual void setScaleY(float fScaleY){
        cocos2d::Sprite::setScaleY(fScaleY);
        if (m_spritePlusFlags & SPRITE_PLUS_HAS_FOLLOWER){
            for (ssize_t i = 0; i < m_followers.count(); i++){
                m_followers[i]->setScaleY(fScaleY);
            }
        }
    }
    
    /// @brief sets the Scale of itself and it's followers 
    /// @param fScale the scale value to set
    virtual void setScale(float fScale){
        cocos2d::Sprite::setScale(fScale);
        if (m_spritePlusFlags & SPRITE_PLUS_HAS_FOLLOWER){
            for (ssize_t i = 0; i < m_followers.count(); i++){
                m_followers[i]->setScale(fScale);
            }
        }
    }
    
    /// @brief Sets the position where the sprite will be at and it's followers
    /// @param pos the position to place to the sprite and it's followers
    virtual void setPosition(cocos2d::Vec2 const& pos){
        cocos2d::Sprite::setPosition(pos);
        if (m_spritePlusFlags & SPRITE_PLUS_HAS_FOLLOWER){
            for (ssize_t i = 0; i < m_followers.count(); i++){
                m_followers[i]->setPosition(pos);
            }
        }
    }
    
    /// @brief Sets the position where the sprite will be at and it's followers
    /// @param x the X position to place to the sprite and it's followers
    /// @param y the Y position to place to the sprite and it's followers
    virtual void setPosition(float x, float y){
        cocos2d::Sprite::setPosition(x, y);
        if (m_spritePlusFlags & SPRITE_PLUS_HAS_FOLLOWER){
            for (ssize_t i = 0; i < m_followers.count(); i++){
                m_followers[i]->setPosition(x, y);
            }
        }
    }
    
    /// @brief sets the sprite's given rotation and it's followers
    /// @param fRotation the rotation value to set
    virtual void setRotation(float fRotation){
        cocos2d::Sprite::setRotation(fRotation);
        if (m_spritePlusFlags & SPRITE_PLUS_HAS_FOLLOWER){
            for (ssize_t i = 0; i < m_followers.count(); i++){
                m_followers[i]->setRotation(fRotation);
            }
        }
    }

    /// @brief sets the sprite's given rotation of X and it's followers
    /// @param fRotationX the rotation of X to set.
    virtual void setRotationSkewX(float fRotationX){
        cocos2d::Sprite::setRotationSkewX(fRotationX);
        if (m_spritePlusFlags & SPRITE_PLUS_HAS_FOLLOWER){
            for (ssize_t i = 0; i < m_followers.count(); i++){
                m_followers[i]->setRotationSkewX(fRotationX);
            }
        }
    }
    
    /// @brief sets the sprite's given rotation of Y and it's followers
    /// @param fRotationY the roation of Y to set 
    virtual void setRotationSkewY(float fRotationY){
        cocos2d::Sprite::setRotationSkewY(fRotationY);
        if (m_spritePlusFlags & SPRITE_PLUS_HAS_FOLLOWER){
            for (ssize_t i = 0; i < m_followers.count(); i++){
                m_followers[i]->setRotationSkewX(fRotationY);
            }
        }
    }

    /// @brief initalizes the sprite using a texture
    /// @param texture the texture to initalize the sprite with 
    /// @return true if initalization succeeded.
    virtual bool initWithTexture(cocos2d::Texture2D* texture){
        return cocos2d::Sprite::initWithTexture(texture);
    }

    /// @brief initalizes the sprite with a frame name 
    /// @param frame the frame to initalize the sprite with 
    /// @return true if initalization succeeded 
    virtual bool initWithSpriteFrameName(const std::string& frame){
        return cocos2d::Sprite::initWithSpriteFrameName(frame);
    }

    /// @brief sets flipX to itself and it's followers
    /// @param flipX the direction that the sprite should be flipped to
    virtual void setFlippedX(bool flipX){
        cocos2d::Sprite::setFlippedX(flipX);
        
        if (getFlipPropagation() && (_parent != nullptr)){
            auto children = _parent->getChildren();
            for (ssize_t i = 0; i < _parent->getChildrenCount(); i++){
                (reinterpret_cast<cocos2d::Sprite*>(children.at(i)))->setFlippedX(flipX);
            }
        }
        if (m_spritePlusFlags & SPRITE_PLUS_HAS_FOLLOWER){
            for (ssize_t i = 0; i < m_followers.count(); i++){
                m_followers[i]->setFlippedX(flipX);
            }
        }
    }
    
    /// @brief sets flipY to itself and it's followers
    /// @param flipY the direction that the sprite should be flipped to
    virtual void setFlippedY(bool flipY){
        cocos2d::Sprite::setFlippedY(flipY);
        if (getFlipPropagation() && (_parent != nullptr)){
            auto children = _parent->getChildren();
            for (ssize_t i = 0; i < _parent->getChildrenCount(); i++){
                (reinterpret_cast<cocos2d::Sprite*>(children.at(i)))->setFlippedY(flipY);
            }
        }
        if (m_spritePlusFlags & SPRITE_PLUS_HAS_FOLLOWER){
            for (ssize_t i = 0; i < m_followers.count(); i++){
                m_followers[i]->setFlippedY(flipY);
            }
        }
    }
};




#endif // __SPRITEPLUS_H__
