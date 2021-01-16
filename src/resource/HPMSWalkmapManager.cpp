/*!
 * File HPMSWalkmapManager.cpp
 */

#include <resource/HPMSWalkmapManager.h>

hpms::WalkmapManager& hpms::WalkmapManager::GetSingleton()
{
    static WalkmapManager instance;
    return instance;
}

hpms::WalkmapManager* hpms::WalkmapManager::GetSingletonPtr()
{
    return &GetSingleton();
}

hpms::WalkmapManager::WalkmapManager()
{
    mResourceType = "Walkmap";
    mLoadOrder = 15;
    Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
}

hpms::WalkmapManager::~WalkmapManager()
{
    Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
}

hpms::WalkmapPtr hpms::WalkmapManager::Create(const std::string& name, const Ogre::String& group)
{
    WalkmapPtr wmPtr = std::static_pointer_cast<Walkmap>(getResourceByName(name));

    if (!wmPtr)
        wmPtr = std::static_pointer_cast<Walkmap>(createResource(name, group));

    wmPtr->load();
    return wmPtr;
}


