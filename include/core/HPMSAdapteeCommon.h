/*!
 * File HPMSAdapteeCommon.h
 */


#pragma once

#include <api/HPMSContextAdapter.h>
#include <common/HPMSUtils.h>

namespace hpms
{
    class AdapteeCommon
    {
    public:
        inline AdapteeCommon(ContextAdapter* ctx) : ctx(ctx)
        {}

        inline void Check() const
        {
            HPMS_ASSERT(ctx, "Context cannot be null.");
        }

        template <typename T>
        inline void Check(T* ptr) const
        {
            Check();
            HPMS_ASSERT(ptr, "Adaptee cannot be null.");
        }

    protected:
        ContextAdapter* ctx;

    };
}