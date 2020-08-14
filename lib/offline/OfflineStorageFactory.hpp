// Copyright (c) Microsoft. All rights reserved.
#ifndef OFFLINESTORAGEFACTORY_HPP
#define OFFLINESTORAGEFACTORY_HPP

#ifdef HAVE_MAT_STORAGE

#include "IOfflineStorage.hpp"
#include "pal/PAL.hpp"

namespace ARIASDK_NS_BEGIN
{
    class OfflineStorageFactory
    {
       public:
        static std::shared_ptr<IOfflineStorage> Create(ILogManager& logManager, IRuntimeConfig& runtimeConfig);
    };
}
ARIASDK_NS_END

#ifdef USE_ROOM
#include "offline/OfflineStorage_Room.hpp"
#else
#include "offline/OfflineStorage_SQLite.hpp"
#endif

#endif // HAVE_MAT_STORAGE

#endif  // HTTPCLIENTFACTORY_HPP