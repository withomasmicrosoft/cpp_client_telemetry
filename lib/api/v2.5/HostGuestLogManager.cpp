// Copyright (c) Microsoft. All rights reserved.
#include "pal/PAL.hpp"
#include "HostGuestLogManager.hpp"
#include "LogController.hpp"
#include "LogManager.hpp"
#include "utils/Utils.hpp"

namespace ARIASDK_NS_BEGIN {

ARIASDK_LOG_INST_COMPONENT_CLASS(HostGuestLogManager, "EventsSDK.HostGuestLogManager", "Events telemetry client - HostGuestLogManager class");

HostGuestLogManager::HostGuestLogManager(LogConfiguration* config, bool wantController)
    :m_logController(nullptr)
{
    LOG_TRACE("New HostGuestLogManager instance");
    {
        std::lock_guard<std::mutex> lock(m_lock);
        if (EVTStatus::EVTStatus_OK == LogManager::Initialize(config, wantController) && wantController)
        {//create controller, wanted host and created as host
             m_logController = new LogController();
        }
        m_context.reset(new ContextFieldsProvider(nullptr));
    }
}

HostGuestLogManager::HostGuestLogManager(bool wantController)
    :m_logController(nullptr)
{
    LOG_TRACE("New HostGuestLogManager instance");
    {
        std::lock_guard<std::mutex> lock(m_lock);
        if (EVTStatus::EVTStatus_OK == LogManager::Initialize(nullptr, wantController) && wantController)
        {//create controller, wanted host and created as host
            m_logController = new LogController();
        }
        m_context.reset(new ContextFieldsProvider(nullptr));
    }
}

HostGuestLogManager::~HostGuestLogManager()
{
    LOG_INFO("destructor");
    {
        std::lock_guard<std::mutex> lock(m_lock);
        LogManager::FlushAndTeardown();
        for (auto& record : m_loggers) 
        {
            delete record.second;
        }
        m_loggers.clear();
        if (m_logController) delete m_logController;
    }
}

ISemanticContext& HostGuestLogManager::GetSemanticContext()
{
    return *m_context;
}

ILogController* HostGuestLogManager::GetLogController()
{ 
    return m_logController;
}

IAuthTokensController*  HostGuestLogManager::GetAuthTokensController()
{
    AuthTokensController* temp = LogManager::GetAuthTokensController();
    return temp;
}

/// <summary>
/// Set global context field - string
/// </summary>
/// <param name="name"></param>
/// <param name="value"></param>
/// <param name="piiKind"></param>
EVTStatus HostGuestLogManager::SetContext(std::string const& name, std::string const& value, PiiKind piiKind)
{
    LOG_TRACE("SetContext(\"%s\", ..., %u)", name.c_str(), piiKind);
    EventProperty prop(value, piiKind);
    m_context->setCustomField(name, prop);
    return EVTStatus::EVTStatus_OK;
}

/// <summary>
/// Set global context field - double
/// </summary>
/// <param name="name"></param>
/// <param name="value"></param>
/// <param name="piiKind"></param>
EVTStatus HostGuestLogManager::SetContext(const std::string& name, double value, PiiKind piiKind) 
{
    LOG_INFO("SetContext");
    EventProperty prop(value, piiKind);
    m_context->setCustomField(name, prop); 
    return EVTStatus::EVTStatus_OK;
}

/// <summary>
/// Set global context field - int64
/// </summary>
/// <param name="name"></param>
/// <param name="value"></param>
/// <param name="piiKind"></param>
EVTStatus HostGuestLogManager::SetContext(const std::string& name, int64_t value, PiiKind piiKind) {
    LOG_INFO("SetContext");
    EventProperty prop(value, piiKind);
    m_context->setCustomField(name, prop);
    return EVTStatus::EVTStatus_OK;
}

/// <summary>
/// Set global context field - boolean
/// </summary>
/// <param name="name"></param>
/// <param name="value"></param>
/// <param name="piiKind"></param>
EVTStatus HostGuestLogManager::SetContext(const std::string& name, bool value, PiiKind piiKind) {
    LOG_INFO("SetContext");
    EventProperty prop(value, piiKind);
    m_context->setCustomField(name, prop);
    return EVTStatus::EVTStatus_OK;
}

/// <summary>
/// Set global context field - date/time in .NET ticks
/// </summary>
/// <param name="name"></param>
/// <param name="value"></param>
/// <param name="piiKind"></param>
EVTStatus HostGuestLogManager::SetContext(const std::string& name, time_ticks_t value, PiiKind piiKind) {
    LOG_INFO("SetContext");
    EventProperty prop(value, piiKind);
    m_context->setCustomField(name, prop);
    return EVTStatus::EVTStatus_OK;
}

/// <summary>
/// Set global context field - GUID
/// </summary>
/// <param name="name"></param>
/// <param name="value"></param>
/// <param name="piiKind"></param>
EVTStatus HostGuestLogManager::SetContext(const std::string& name, GUID_t value, PiiKind piiKind) {
    LOG_INFO("SetContext");
    EventProperty prop(value, piiKind);
    m_context->setCustomField(name, prop);
    return EVTStatus::EVTStatus_OK;
}


ILogger* HostGuestLogManager::GetLogger(std::string const& tenantToken)
{
     LOG_TRACE("GetLogger(tenantId=\"%s\" ", tenantTokenToId(tenantToken).c_str());
     {
         std::string normalizedTenantToken = toLower(tenantToken);
         std::lock_guard<std::mutex> lock(m_lock);         

          auto& logger = m_loggers[normalizedTenantToken];
          if (!logger)
          {
              logger = new Logger(normalizedTenantToken, "", "", nullptr, m_context.get(), nullptr);
          }
          
          return logger;
     }
}
}}} // namespace Microsoft::Applications::Events 
