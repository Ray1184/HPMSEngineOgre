/*!
 * File HPMSDepthGeometry.h
 */


#pragma once

#include <Ogre.h>
#include <common/HPMSNamedWrapper.h>
#include <common/HPMSUtils.h>

namespace hpms
{
    class DepthGeometry : public Ogre::MovableObject, public hpms::Object
    {
    private:
        hpms::NamedWrapper<Ogre::Entity>* entityRef;

    public:
        inline DepthGeometry(const std::string& depthGeometryPath, OgreContext* ctx)
        {
            auto* iEntityRef = ctx->GetSceneManager()->createEntity(depthGeometryPath);
            entityRef = hpms::SafeNew<NamedWrapper<Ogre::Entity>>(iEntityRef, "Ogre::Entity");
            auto material = Ogre::MaterialManager::getSingleton().getByName("R25D/DepthOnly");
            entityRef->GetWrappedObject()->setMaterial(material);
        }

        inline virtual ~DepthGeometry()
        {
            hpms::SafeDelete(entityRef);
        }

        inline const Ogre::String& getMovableType(void) const override
        {
            return entityRef->GetWrappedObject()->getMovableType();
        }

        inline const Ogre::AxisAlignedBox& getBoundingBox() const override
        {
            return entityRef->GetWrappedObject()->getBoundingBox();
        }

        inline Ogre::Real getBoundingRadius() const override
        {
            return entityRef->GetWrappedObject()->getBoundingRadius();
        }

        inline void _updateRenderQueue(Ogre::RenderQueue* queue) override
        {
            entityRef->GetWrappedObject()->_updateRenderQueue(queue);
        }

        inline void visitRenderables(Ogre::Renderable::Visitor* visitor, bool debugRenderables) override
        {
            entityRef->GetWrappedObject()->visitRenderables(visitor, debugRenderables);
        }

        inline const std::string Name() const override
        {
            return "DepthGeometry";
        }
    };
}