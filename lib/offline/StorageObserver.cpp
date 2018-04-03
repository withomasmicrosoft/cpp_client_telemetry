// Copyright (c) Microsoft. All rights reserved.

#include "StorageObserver.hpp"

namespace ARIASDK_NS_BEGIN {

    StorageObserver::StorageObserver(ITelemetrySystem& system, IOfflineStorage& offlineStorage)
        :
        m_system(system),
        m_offlineStorage(offlineStorage)
    {
    }

    StorageObserver::~StorageObserver()
    {
    }

    bool StorageObserver::handleStart()
    {
        m_offlineStorage.Initialize(*this);
        return true;
    }

    bool StorageObserver::handleStop()
    {
        m_offlineStorage.Shutdown();
        return true;
    }

    bool StorageObserver::handleStoreRecord(IncomingEventContextPtr const& ctx)
    {
        ctx->record.timestamp = PAL::getUtcSystemTimeMs();

        if (!m_offlineStorage.StoreRecord(ctx->record)) {
            // TODO: [MG] - add error callback for the case when record is not cached?
            storeRecordFailed(ctx);
            return false;
        }

        DispatchEvent(DebugEventType::EVT_CACHED);
        return true;
    }

    void StorageObserver::handleRetrieveEvents(EventsUploadContextPtr const& ctx)
    {
        auto consumer = [&ctx, this](StorageRecord&& record) -> bool {
            bool wantMore = true;
            retrievedEvent(ctx, std::move(record), wantMore);
            return wantMore;
        };

        // FIXME: [MG] - memory corruption here...
        if (!m_offlineStorage.GetAndReserveRecords(consumer, 120000, ctx->requestedMinLatency, ctx->requestedMaxCount))
        {
            ctx->fromMemory = m_offlineStorage.IsLastReadFromMemory();
            retrievalFailed(ctx);
        }
        else
        {
            ctx->fromMemory = m_offlineStorage.IsLastReadFromMemory();
            retrievalFinished(ctx);
        }
    }

    bool StorageObserver::handleDeleteRecords(EventsUploadContextPtr const& ctx)
    {
        HttpHeaders headers;
        if (ctx->httpResponse)
        {
            headers = ctx->httpResponse->GetHeaders();
        }
        std::vector<StorageRecordId> recordIds;
        for (auto item : ctx->recordIdsAndTenantIds)
        {
            recordIds.push_back(item.first);
        }
        m_offlineStorage.DeleteRecords(recordIds, headers, ctx->fromMemory);
        return true;
    }

    bool StorageObserver::handleReleaseRecords(EventsUploadContextPtr const& ctx)
    {
        HttpHeaders headers;
        if (ctx->httpResponse)
        {
            headers = ctx->httpResponse->GetHeaders();
        }
        std::vector<StorageRecordId> recordIds;
        for (auto item : ctx->recordIdsAndTenantIds)
        {
            recordIds.push_back(item.first);
        }
        m_offlineStorage.ReleaseRecords(recordIds, false, headers, ctx->fromMemory);
        return true;
    }

    bool StorageObserver::handleReleaseRecordsIncRetryCount(EventsUploadContextPtr const& ctx)
    {
        DispatchEvent(DebugEventType::EVT_SEND_RETRY);
        HttpHeaders headers;
        if (ctx->httpResponse)
        {
            headers = ctx->httpResponse->GetHeaders();
        }
        std::vector<StorageRecordId> recordIds;
        for (auto item : ctx->recordIdsAndTenantIds)
        {
            recordIds.push_back(item.first);
        }

        m_offlineStorage.ReleaseRecords(recordIds, true, headers, ctx->fromMemory);
        return true;
    }

    void StorageObserver::OnStorageOpened(std::string const& type)
    {
        StorageNotificationContext ctx;
        ctx.str = type;
        opened(&ctx);
    }

    void StorageObserver::OnStorageFailed(std::string const& reason)
    {
        StorageNotificationContext ctx;
        ctx.str = reason;
        failed(&ctx);
    }

    void StorageObserver::OnStorageTrimmed(std::map<std::string, size_t> const& numRecords)
    {
        StorageNotificationContext ctx;

        size_t overallCount = 0;
        for (auto records : numRecords)
        {
            ctx.countonTenant[records.first] = records.second;
            overallCount += records.second;
        }
        trimmed(&ctx);

        {
            DebugEvent evt;
            evt.type = EVT_DROPPED;
            evt.param1 = overallCount;
            evt.size = overallCount;
            DispatchEvent(evt);
        }
    }

    void StorageObserver::OnStorageRecordsDropped(std::map<std::string, size_t> const& numRecords)
    {
        StorageNotificationContext ctx;
        size_t overallCount = 0;
        for (auto records : numRecords)
        {
            ctx.countonTenant[records.first] = records.second;
            overallCount += records.second;
        }
        recordsDropped(&ctx);

        {
            DebugEvent evt;
            evt.type = EVT_DROPPED;
            evt.param1 = overallCount;
            evt.size = overallCount;
            DispatchEvent(evt);
        }
    }

    void StorageObserver::OnStorageRecordsRejected(std::map<std::string, size_t> const& numRecords)
    {
        StorageNotificationContext ctx;
        size_t overallCount = 0;
        for (auto records : numRecords)
        {
            ctx.countonTenant[records.first] = records.second;
            overallCount += records.second;
        }
        recordsRejected(&ctx);

        {
            DebugEvent evt;
            evt.type = EVT_REJECTED;
            evt.param1 = overallCount;
            evt.size = overallCount;
            DispatchEvent(evt);
        }
    }

} ARIASDK_NS_END
