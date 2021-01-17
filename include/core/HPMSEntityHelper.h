/*!
 * File HPMSEntityHelper.h
 */


#pragma once

#include <Ogre.h>

namespace hpms
{
    class EntityHelper
    {


    public:
        inline static void SetWriteDepthOnly(Ogre::Entity* entity)
        {
            auto material = entity->getSubEntity(0)->getMaterial();
            material->getTechnique(0)->getPass(0)->setColourWriteEnabled(false);
        }


    };
}