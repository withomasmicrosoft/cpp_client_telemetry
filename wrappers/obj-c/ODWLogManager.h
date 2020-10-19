#include "objc_begin.h"
#import "ODWLogger.h"

NS_ASSUME_NONNULL_BEGIN

/*!
 @brief The ODWLogManager class manages the telemetry lgging system.
*/
@interface ODWLogManager : NSObject

/*!
 @enum ODWTransmissionProfile
 @brief The <b>ODWTransmissionProfile</b> enumeration contains a set of values that specify how efficiently telemetry events are sent.
*/typedef NS_ENUM(NSInteger, ODWTransmissionProfile)
{
    ODWRealTime = 0,
    ODWNearRealTime = 1,
    ODWBestEffort = 2
};

/*!
 @brief Initializes the telemetry logging system with the default cofiguration, using the specified tenant token.
 @param tenantToken A string that contains the tenant token.
 @return An ODWLogger instance
 */
+(nullable ODWLogger *)initForTenant:(nonnull NSString *)tenantToken;

/*!
@brief Initializes the telemetry logging system with the specified tenant token and custom cofiguration.
@param tenantToken A string that contains the tenant token.
@param config A Dictionary that contains a custom configuration.
@return An ODWLogger instance
*/
+(nullable ODWLogger *)initForTenant:(nonnull NSString *)tenantToken withConfig:(nullable NSDictionary *)config;

/*!
 @brief Retrieves a new instance of ODWLogger for logging telemetry events. It requires to previously initialize LogManager.
 @param tenantToken A string that contains the tenant token.
 @return An ODWLogger instance
 */
+(nullable ODWLogger *)loggerWithTenant:(NSString *)tenantToken;

/*!
 @brief Retrieves a new instance of ODWLogger for logging telemetry events. It requires to previously initialize LogManager.
 @param tenantToken A string that contains the tenant token.
 @param source A string that contains the name of the source of events.
 @return An ODWLogger pointer that points to the logger for the specified tenantToken and source.
 */
+(nullable ODWLogger *)loggerWithTenant:(NSString *)tenantToken
                  source:(NSString *)source;

/*!
 @brief Retrieves a new instance of ODWLogger for logging telemetry events. It requires to previously initialize LogManager.
 @param source A string that contains the name of the source of events sent by this logger instance.
 @return An ODWLogger instance that points to the logger for source.
 */
+(nullable ODWLogger *)loggerForSource:(NSString *)source;

/*!
 @brief Attempts to send any pending telemetry events that are currently cached either in memory, or on disk. Use this method if your event can't wait for automatic timed upload
 */
+(void)uploadNow;

/*!
 @brief Flushes pending telemetry events from memory to disk (to reduce possible data loss).
 */
+(void)flush;

/*!
 @brief Sets the transmit profile for event transmission.
 @details A transmit profile is a collection of hardware and system settings (like network connectivity, power state, etc.)
 that determines how efficiently telemetry events are transmitted.
 @param profile The transmit profile to set&mdash;as one of the ::ODWTransmissionProfile enumeration values.
 */
+(void)setTransmissionProfile:(ODWTransmissionProfile)profile;

/*!
 @brief Pauses the transmission of telemetry events to the data collector.
 @details While paused, events continue to be queued on the client side&mdash;cached either in memory or on disk.
 */
+(void)pauseTransmission;

/*!
 @brief Resumes the transmission of telemetry events to the data collector.
 */
+(void)resumeTransmission;

/*!
 @brief Flushes pending telemetry events from memory to disk, and tears-down the telemetry logging system.
 */
+(void)flushAndTeardown;

/*!
 @brief Resets the transmit profiles to contain only default profiles.
 */
+(void)resetTransmitProfiles;

/*!
 @brief Adds (or overrides) a property of the context for the telemetry logging system.
 @details Context information applies to events generated by all ODWLogger instances;unless it is overwritten on a particular ODWLogger instance.
 @param name A string that contains the name of the property.
 @param value A string that contains the property value.
 */
+(void)setContextWithName:(nonnull NSString*)name
              stringValue:(nonnull NSString*)value;

/*!
 @brief Adds (or overrides) a property of the context for the telemetry logging system, and tags it with its PII kind.
 @details Context information applies to events generated by all ODWLogger instances;unless it is overwritten on a particular ODWLogger instance.
 @param name A string that contains the name of the property.
 @param value A string that contains the value of the property.
 @param piiKind The kind of personal identifiable information (PII), as one of the ::ODWPiiKind enumeration values.
 */
+(void)setContextWithName:(nonnull NSString*)name
              stringValue:(nonnull NSString*)value
                  piiKind:(enum ODWPiiKind)piiKind;
@end

NS_ASSUME_NONNULL_END
#include "objc_end.h"
