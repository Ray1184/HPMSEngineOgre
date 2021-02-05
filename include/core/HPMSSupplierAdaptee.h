/*!
 * File HPMSSupplierAdaptee.h
 */

#pragma once

#ifdef WIN32
#define HPMS_DLL DYNALO_DEMANGLE __declspec(dllexport)
#else
#define HPMS_DLL DYNALO_DEMANGLE
#endif

#if !defined(HPMS_CALL)
#ifdef _MSC_VER
#define HPMS_CALL __cdecl
#else
#define HPMS_CALL
#endif
#endif


#include <api/HPMSSupplierAdapter.h>
#include <core/HPMSCameraAdaptee.h>
#include <core/HPMSLightAdaptee.h>
#include <core/HPMSEntityAdaptee.h>
#include <core/HPMSSceneNodeAdaptee.h>
#include <vector>

namespace hpms
{
    class SupplierAdaptee : public hpms::SupplierAdapter, public AdapteeCommon
    {
    private:

        std::vector<EntityAdapter*> entities;
        SceneNodeAdapter* rootNode;
        CameraAdapter* camera;
        std::vector<BackgroundImageAdapter*> backgrounds;
        std::vector<OverlayImageAdapter*> overlays;
        std::vector<LightAdapter*> lights;

        void FreeItems();

    public:
        explicit SupplierAdaptee(hpms::OgreContext* ctx);

        virtual ~SupplierAdaptee();

        virtual EntityAdapter* CreateEntity(const std::string& path) override;

        virtual SceneNodeAdapter* GetRootSceneNode() override;

        virtual hpms::CameraAdapter* GetCamera() override;

        virtual hpms::LightAdapter* CreateLight(float r, float g, float b) override;

        virtual BackgroundImageAdapter*
        CreateBackgroundImage(const std::string& path, unsigned int width, unsigned int height) override;

        virtual OverlayImageAdapter*
        CreateOverlayImage(const std::string& path, unsigned int width, unsigned int height, unsigned int x,
                           unsigned int y, int zOrder) override;

        virtual void SetAmbientLight(const glm::vec3& rgb) override;

        virtual std::string GetImplName() override;

    };
}

hpms::OgreContext* ctx;

HPMS_DLL hpms::SupplierAdapter* HPMS_CALL CreateSupplier(hpms::WindowSettings& windowSettings);

HPMS_DLL void HPMS_CALL DestroySupplier(hpms::SupplierAdapter*& supplier);
