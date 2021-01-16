/*!
 * File HPMSLuaScriptManager.cpp
 */

#include <resource/HPMSLuaScriptManager.h>

hpms::LuaScriptManager& hpms::LuaScriptManager::GetSingleton()
{
    static LuaScriptManager instance;
    return instance;
}

hpms::LuaScriptManager* hpms::LuaScriptManager::GetSingletonPtr()
{
    return &GetSingleton();
}

hpms::LuaScriptManager::LuaScriptManager()
{
    mResourceType = "LuaScript";
    mLoadOrder = 10;
    Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
}

hpms::LuaScriptManager::~LuaScriptManager()
{
    Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
}

hpms::LuaScriptPtr hpms::LuaScriptManager::Create(const std::string& name, const Ogre::String& group)
{
    LuaScriptPtr luaPtr = std::static_pointer_cast<LuaScript>(getResourceByName(name));

    if (!luaPtr)
        luaPtr = std::static_pointer_cast<LuaScript>(createResource(name, group));

    luaPtr->load();
    return luaPtr;
}


