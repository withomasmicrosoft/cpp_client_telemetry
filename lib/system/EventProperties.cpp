
#include "utils/Common.hpp"
#include "bond/generated/AriaProtocol_types.hpp"
#include "Aria/EventProperty.hpp"
#include "Aria/EventProperties.hpp"
#include <string>
#include <map>


using namespace std;
using namespace Microsoft::Applications::Telemetry;

namespace Microsoft {
    namespace Applications {
        namespace Telemetry {

            EventProperties::EventProperties(const std::string& name, const std::map<std::string, EventProperty> &properties) :
                EventProperties(name)
            {
                (*this) += properties;
            }

            EventProperties& EventProperties::operator+=(const std::map<std::string, EventProperty> &properties)
            {
                for (auto &kv : properties)
                {
                    auto key = kv.first;
                    auto val = kv.second;
					(*m_propertiesP)[key] = val;
                }
                return (*this);
            }

            EventProperties& EventProperties::operator=(const std::map<std::string, EventProperty> &properties)
            {
                m_propertiesP->clear();
                (*this) += properties;
                return (*this);
            }

            /**
             * \brief EventProperties constructor
             * \param name Event name - must not be empty!
             */

			EventProperties::EventProperties()
				: m_timestampInMillis(0LL)
				, m_eventPriority(EventPriority_Unspecified)
				, m_eventPolicyBitflags(0)
				, m_eventNameP(new std::string("EventProperties default constructor"))
				, m_eventTypeP(new std::string())
				, m_propertiesP(new std::map<std::string, EventProperty>())
			{
			}

            EventProperties::EventProperties(const string& name)
                : m_timestampInMillis(0LL)
                , m_eventPriority(EventPriority_Unspecified)
				, m_eventPolicyBitflags(0)
				, m_eventNameP(new std::string("EventProperties Named constructor"))
				, m_eventTypeP(new std::string())
				, m_propertiesP(new std::map<std::string, EventProperty>())
            {
                if (!name.empty())
                {
                    SetName(name);
                }
                else {
                    SetName("undefined");
                }
            }
			EventProperties::EventProperties(EventProperties const& copy)
			{
				m_eventNameP = new std::string(*(copy.m_eventNameP));
				m_eventTypeP = new std::string(*(copy.m_eventTypeP));
				m_propertiesP = new std::map<std::string, EventProperty>(*copy.m_propertiesP);
				m_eventPriority = copy.m_eventPriority;
				m_eventPolicyBitflags = copy.m_eventPolicyBitflags;
				m_timestampInMillis = copy.m_timestampInMillis;
                
                std::map<std::string, EventProperty>::iterator iter;
                for (iter = copy.m_propertiesP->begin(); iter != copy.m_propertiesP->end(); iter++)
                {
                    (*m_propertiesP)[iter->first] = iter->second;
                }
			}

			EventProperties& EventProperties::operator=(EventProperties const& copy)
			{
				m_eventNameP = new std::string(*(copy.m_eventNameP));
				m_eventTypeP = new std::string(*(copy.m_eventTypeP));
				m_propertiesP = new std::map<std::string, EventProperty>(*copy.m_propertiesP);
				m_eventPriority = copy.m_eventPriority;
				m_eventPolicyBitflags = copy.m_eventPolicyBitflags;
				m_timestampInMillis = copy.m_timestampInMillis;

                std::map<std::string, EventProperty>::iterator iter;
                for (iter = copy.m_propertiesP->begin(); iter != copy.m_propertiesP->end(); iter++)
                {
                    (*m_propertiesP)[iter->first] = iter->second;
                }

				return *this;
			}

			EventProperties::~EventProperties()
			{
				if (m_eventNameP) delete m_eventNameP;
				if (m_eventTypeP) delete m_eventTypeP;
				if (m_propertiesP) delete m_propertiesP;
			}

            /// <summary>
            /// EventProperties constructor using C++11 initializer list
            /// </summary>
            EventProperties::EventProperties(const std::string& name, std::initializer_list<std::pair<std::string const, EventProperty> > properties)
                : EventProperties(name)
            {
                (*this) = properties;
            }

            /// <summary>
            /// EventProperties assignment operator using C++11 initializer list
            /// </summary>
            EventProperties& EventProperties::operator=(std::initializer_list<std::pair<std::string const, EventProperty> > properties)
            {
                (*m_propertiesP).clear();

                for (auto &kv : properties)
                {
                    auto key = kv.first;
                    auto val = kv.second;
                   
					(*m_propertiesP)[key] = val;
                }

                return (*this);
            };

            /// <summary>
            /// Set the Epoch unix timestamp in milliseconds of the event. 
            /// This will override default timestamp generated by telemetry system.
            /// <param name="timestampInEpochMillis">Unix timestamp in milliseconds since 00:00:00 
            /// Coordinated Universal Time (UTC), 1 January 1970 not counting leap seconds</param>
            /// </summary>
            void EventProperties::SetTimestamp(const int64_t timestampInEpochMillis)
            {
                m_timestampInMillis = timestampInEpochMillis;
            }

            /// <summary>
            /// Returns the timestamp of the event.
            /// If this was not set explicitly by calling SetTimestamp, it will return 0 by default.
            /// </summary>
            int64_t EventProperties::GetTimestamp() const
            {
                return m_timestampInMillis;
            }

            /// <summary>
            /// Set transmit priority of this event
            /// Default transmit priority will be used if none specified 
            /// </summary>
            void EventProperties::SetPriority(EventPriority priority)
            {
                m_eventPriority = priority;
            }

            /// <summary>
            /// Get transmit priority of this event
            /// Default transmit priority will be used if none specified 
            /// </summary>
            EventPriority EventProperties::GetPriority() const
            {
                EventPriority result = m_eventPriority;
                return result;
            }

			/// <summary>
			///  Specify Policy Bit flags for UTC usage of an event.
			/// </summary>
			void EventProperties::SetPolicyBitFlags(uint64_t policyBitFlags)
			{
				m_eventPolicyBitflags = policyBitFlags;
			}

			/// <summary>
			/// Get the Policy bit flags for UTC usage of the event.
			/// </summary>
			uint64_t EventProperties::GetPolicyBitFlags() const
			{
				return m_eventPolicyBitflags;
			}

            /// <summary>
            /// Set name of this event
            /// Default name will be used if none specified (e.g. for LogPageView, name = "PageView")
            /// </summary>
            bool EventProperties::SetName(const string& name)
            {
                std::string m_eventName;
                // Normalize the name of EventProperties
                m_eventName = toLower(name);
                m_eventName = sanitizeIdentifier(m_eventName);
                bool isValidEventName = validateEventName(m_eventName);
                if (!isValidEventName) {
                    return false;
                }
				this->m_eventNameP->assign(m_eventName);
                return true;
            }

            /// <summary>
            /// Returns the name for this event. 
            /// If this was not set explicitly by calling SetName, it will return an empty string.
            /// </summary>
            const string& EventProperties::GetName() const
            {
                return *m_eventNameP;
            }

            /// <summary>
            /// Specify the Base Type of an event. This field is populated in Records.Type
            /// </summary>
            bool EventProperties::SetType(const string& recordType)
            {
                std::string m_eventType;
                // Normalize the type of EventProperties
                m_eventType = toLower(recordType);
                m_eventType = sanitizeIdentifier(m_eventType);
                bool isValidEventType = validateEventName(m_eventType);
                if (!isValidEventType) {
                    return false;
                }
				this->m_eventTypeP->assign(m_eventType);
                return true;
            }

            /// <summary>
            /// Returns the Base Type for this event. 
            /// If this was not set explicitly by calling SetType, it will return an empty string.
            /// </summary>
            const string& EventProperties::GetType() const
            {
                return *m_eventTypeP;
            }

            /// <summary>
            /// Specify a property of an event
            /// It creates a new property if none exists or overwrites an existing one
            /// <param name='name'>Name of the property</param>
            /// <param name='value'>String value of the property</param>
            /// <param name='piiKind'>PIIKind of the property</param>
            /// </summary>
            void EventProperties::SetProperty(const string& name, EventProperty prop)
            {
                bool isValidPropertyName = validatePropertyName(name);
                if (!isValidPropertyName)
                {
                    // FIXME: add a callback for the case where we reject properties as invalid
                    return;
                }
               
				(*m_propertiesP)[name] = prop;
            }

            //
            void EventProperties::SetProperty(const std::string& name, char const*  value, PiiKind piiKind) { SetProperty(name, EventProperty(value, piiKind)); }
            void EventProperties::SetProperty(const std::string& name, std::string  value, PiiKind piiKind) { SetProperty(name, EventProperty(value, piiKind)); }
            void EventProperties::SetProperty(const std::string& name, double       value, PiiKind piiKind) { SetProperty(name, EventProperty(value, piiKind)); }
            void EventProperties::SetProperty(const std::string& name, int64_t      value, PiiKind piiKind) { SetProperty(name, EventProperty(value, piiKind)); }
            void EventProperties::SetProperty(const std::string& name, bool         value, PiiKind piiKind) { SetProperty(name, EventProperty(value, piiKind)); }
            void EventProperties::SetProperty(const std::string& name, time_ticks_t value, PiiKind piiKind) { SetProperty(name, EventProperty(value, piiKind)); }
            void EventProperties::SetProperty(const std::string& name, GUID_t       value, PiiKind piiKind) { SetProperty(name, EventProperty(value, piiKind)); }

            const map<string, EventProperty>& EventProperties::GetProperties() const
            {
                return (*m_propertiesP);
            }

            /// <summary>
            /// Get Pii properties map
            /// </summary>
            /// <returns></returns>
            const map<string, pair<string, PiiKind> > EventProperties::GetPiiProperties() const
            {
				std::map<string, pair<string, PiiKind> > pIIExtensions;
				for (auto &kv : (*m_propertiesP))
				{
					auto k = kv.first;
					auto v = kv.second;
					if (v.piiKind != PiiKind_None)
					{
						pIIExtensions[k] = std::pair<string, PiiKind>(v.to_string(), v.piiKind);
					}
				}
                
				return pIIExtensions;
            }
			        
            /// <summary>
            /// GUID_t constructor that accepts string
            /// </summary>
            /// <param name="guid_string"></param>
            GUID_t::GUID_t(const char* guid_string)
            {
                char *str = (char *)(guid_string);
                // Skip curly brace
                if (str[0] == '{') {
                    str++;
                }
                // Convert to set of integer values
                sscanf_s(str,
                    "%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX",
                    &Data1, &Data2, &Data3,
                    &Data4[0],
                    &Data4[1],
                    &Data4[2],
                    &Data4[3],
                    &Data4[4],
                    &Data4[5],
                    &Data4[6],
                    &Data4[7]);
            }

            GUID_t::GUID_t(const uint8_t guid_bytes[16], bool bigEndian)
            {
                if (bigEndian) {
                    /* Use big endian - human-readable */
                    // Part 1
                    Data1 = guid_bytes[3];
                    Data1 |= ((uint32_t)(guid_bytes[2])) << 8;
                    Data1 |= ((uint32_t)(guid_bytes[1])) << 16;
                    Data1 |= ((uint32_t)(guid_bytes[0])) << 24;
                    // Part 2
                    Data2 = guid_bytes[5];
                    Data2 |= ((uint16_t)(guid_bytes[4])) << 8;
                    // Part 3
                    Data3 = guid_bytes[7];
                    Data3 |= ((uint16_t)(guid_bytes[6])) << 8;
                }
                else
                {
                    /* Use little endian - the same order as .NET C# Guid() class uses */
                    // Part 1
                    Data1 = guid_bytes[0];
                    Data1 |= ((uint32_t)(guid_bytes[1])) << 8;
                    Data1 |= ((uint32_t)(guid_bytes[2])) << 16;
                    Data1 |= ((uint32_t)(guid_bytes[3])) << 24;
                    // Part 2
                    Data2 = guid_bytes[4];
                    Data2 |= ((uint16_t)(guid_bytes[5])) << 8;
                    // Part 3
                    Data3 = guid_bytes[6];
                    Data3 |= ((uint16_t)(guid_bytes[7])) << 8;
                }
                // Part 4
                for (size_t i = 0; i < 8; i++)
                {
                    Data4[i] = guid_bytes[8 + i];
                }
            }

            void GUID_t::to_bytes(uint8_t(&guid_bytes)[16])
            {
                // Part 1
                guid_bytes[0] = (uint8_t)((Data1) & 0xFF);
                guid_bytes[1] = (uint8_t)((Data1 >> 8) & 0xFF);
                guid_bytes[2] = (uint8_t)((Data1 >> 16) & 0xFF);
                guid_bytes[3] = (uint8_t)((Data1 >> 24) & 0xFF);
                // Part 2
                guid_bytes[4] = (uint8_t)((Data2) & 0xFF);
                guid_bytes[5] = (uint8_t)((Data2 >> 8) & 0xFF);
                // Part 3
                guid_bytes[6] = (uint8_t)((Data3) & 0xFF);
                guid_bytes[7] = (uint8_t)((Data3 >> 8) & 0xFF);
                // Part 4
                for (size_t i = 0; i < 8; i++)
                {
                    guid_bytes[8 + i] = Data4[i];
                }
            }

        }
    }
}
