/**
 * \mainpage API Summary
 *
 * The basic WPS API sequence can be summarized to the following calls:
 *
 * \li \c WPS_load()
 * \li \c WPS_set_key()
 * \li \c WPS_location()
 * \li \c WPS_unload()
 *
 * Periodic and IP location:
 *
 * \li \c WPS_periodic_location()
 * \li \c WPS_ip_location()
 *
 * Configuration:
 *
 * \li \c WPS_set_tiling()
 * \li \c WPS_set_tunable()
 *
 * Geofencing:
 *
 * \li \c WPS_geofence_set()
 * \li \c WPS_geofence_cancel()
 * \li \c WPS_geofence_cancel_all()
 *
 * Offline location:
 *
 * \li \c WPS_offline_token()
 * \li \c WPS_offline_location()
 *
 * \page license Limited Use License
 *
 * Copyright 2005-present Skyhook, Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted subject to the following:
 *
 * * Use and redistribution is subject to the Software License and Development
 * Agreement, available at
 * <a href="http://www.skyhookwireless.com">www.skyhookwireless.com</a>
 *
 * * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _WPSAPI_H_
#define _WPSAPI_H_

#if defined WPSAPI_EXTENSION && !defined WPSAPI_STATIC
#  error WPSAPI_EXTENSION can be used only with WPSAPI_STATIC
#endif

/**
 * \cond
 */

#if defined WPSAPI_STATIC && !defined WPSAPI_EXTENSION
#  define WPSAPI_EXPORT
#else
#  if __GNUC__ >= 4
#    define WPSAPI_EXPORT __attribute__((visibility("default")))
#  elif defined _WIN32
#    ifdef WPSAPI_INTERNAL
#      define WPSAPI_EXPORT __declspec(dllexport)
#    else
#      define WPSAPI_EXPORT __declspec(dllimport)
#    endif
#  else
#    define WPSAPI_EXPORT
#  endif
#endif

#ifdef _WIN32
#  define WPSAPI_CALL __cdecl
#else
#  define WPSAPI_CALL
#endif

/**
 * \endcond
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * WPS return codes.
 */
typedef enum
{
    /**
     * The call was successful.
     */
    WPS_OK = 0,

    /**
     * The \c WPSScanner.dll was not found.
     *
     * \deprecated This error code is no longer relevant.
     */
    WPS_ERROR_SCANNER_NOT_FOUND = 1,

    /**
     * No Wi-Fi adapter was detected.
     */
    WPS_ERROR_WIFI_NOT_AVAILABLE = 2,

    /**
     * No Wi-Fi reference points in range.
     */
    WPS_ERROR_NO_WIFI_IN_RANGE = 3,

    /**
     * User authentication failed.
     */
    WPS_ERROR_UNAUTHORIZED = 4,

    /**
     * The server is unavailable.
     */
    WPS_ERROR_SERVER_UNAVAILABLE = 5,

    /**
     * A location couldn't be determined.
     */
    WPS_ERROR_LOCATION_CANNOT_BE_DETERMINED = 6,

    /**
     * Proxy authentication failed.
     */
    WPS_ERROR_PROXY_UNAUTHORIZED = 7,

    /**
     * A file IO error occurred while reading the local file.
     *
     * \since 2.4
     */
    WPS_ERROR_FILE_IO = 8,

    /**
     * The local file has an invalid format.
     *
     * \since 2.4
     */
    WPS_ERROR_INVALID_FILE_FORMAT = 9,

    /**
     * Network operation timed out.
     *
     * \since 3.0
     */
    WPS_ERROR_TIMEOUT = 10,

    /**
     * Call cannot be completed at this time.
     *
     * \since 3.3
     */
    WPS_NOT_APPLICABLE = 11,

    /**
     * An error occurred when processing a geofence.
     * \see \c WPS_geofence_set()
     * \see \c WPS_geofence_cancel()
     *
     * \since 4.4
     */
    WPS_GEOFENCE_ERROR = 12,

    /**
     * Could not tune the location.
     *
     * \since 4.7
     */
    WPS_ERROR_NOT_TUNED = 13,

    /**
     * Could not sign the certified location.
     *
     * \since 4.9.1
     */
    WPS_ERROR_NOT_SIGNED = 14,

    /**
     * Location services are disabled in the system.
     *
     * \since 4.9.2
     */
    WPS_ERROR_LOCATION_SETTING_DISABLED = 15,

    /**
     * The application is not permitted to determine location.
     *
     * \since 5.0
     */
    WPS_ERROR_LOCATION_NOT_PERMITTED = 16,

    /**
     * Cannot allocate memory.
     *
     * \since 2.6.1
     */
    WPS_NOMEM = 98,

    /**
     * Some other error occurred.
     */
    WPS_ERROR = 99
} WPS_ReturnCode;

/**
 * \c WPS continuation.
 *
 * \since 2.6
 */
typedef enum
{
    WPS_STOP = 0,
    WPS_CONTINUE = 1
} WPS_Continuation;

/**
 * \c WPS_SimpleAuthentication is used to identify
 * the user with the server.
 */
typedef struct
{
    /**
     * the user's name, or unique identifier.
     */
    const char* username;

    /**
     * the authentication realm.
     */
    const char* realm;
} WPS_SimpleAuthentication;

/**
 * Street address lookup.
 *
 * \note The server returns as much information as requested,
 *       but is not required to fill in all the requested fields.
 *       \n
 *       Only the fields the server could reverse geocode are returned.
 */
typedef enum
{
    /**
     * no street address lookup is performed.
     */
    WPS_NO_STREET_ADDRESS_LOOKUP,

    /**
     * a limited address lookup is performed
     * to return, at most, city information.
     */
    WPS_LIMITED_STREET_ADDRESS_LOOKUP,

    /**
     * a full street address lookup is performed
     * returning the most specific street address.
     */
    WPS_FULL_STREET_ADDRESS_LOOKUP
} WPS_StreetAddressLookup;

/**
 * Name and code
 */
typedef struct
{
    /**
     * name
     */
    char* name;

    /**
     * 2-letter code
     */
    char code[3];
} WPS_NameCode;

/**
 * Street Address
 *
 * \note Some map data &copy;
 *       <a href="http://www.openstreetmap.org/">OpenStreetMap</a> contributors,
 *       <a href="http://creativecommons.org/licenses/by-sa/2.0/">CC-BY-SA</a>.
 */
typedef struct
{
    /**
     * street number
     */
    char* street_number;

    /**
     * A \c NULL terminated array of address line
     */
    char** address_line;

    /**
     * Represents the larger city layer which can be defined as
     * the urbanized region around major metropolitan areas.
     * \n
     * Unlike \c #city_sub, this city region is not specifically
     * delineated by jurisdictional boundaries but instead is influenced
     * by the access point density detected by Skyhook's systems.
     * \n
     * Specifically in areas where Skyhook has good postal code coverage,
     * \c city often resolves to the formal name of that postal
     * region. This is most typical in the U.S., Canada, Australia, and other
     * major European countries.
     *
     * \see city_sub
     */
    char* city;

    /**
     * postal code
     */
    char* postal_code;

    /**
     * county
     */
    char* county;

    /**
     * province
     */
    char* province;

    /**
     * state, includes state name and 2-letter code.
     */
    WPS_NameCode state;

    /**
     * region
     */
    char* region;

    /**
     * country, includes country name and 2-letter code.
     */
    WPS_NameCode country;

    /**
     * Represets the second city layer that usually includes smaller
     * township or municipal names found within the larger city region.
     * \n
     * Unlike \c #city, this geospatial layer is sourced
     * from open-source data without modification and it more closely
     * aligns with general jurisdictional and town boundaries.
     *
     * \see city
     */
    char* city_sub;
} WPS_StreetAddress;

/**
 * Calculated location type.
 *
 * \since 3.2
 */
typedef enum
{
    WPS_LOCATION_TYPE_2D,
    WPS_LOCATION_TYPE_3D
} WPS_LocationType;

/**
 * Geographic location
 */
typedef struct
{
    //@{
    /**
     * the calculated physical geographic location.
     */
    double latitude;
    double longitude;
    //@}

    /**
     * <em>horizontal positioning error</em> --
     * A calculated error estimate of the location result in meters.
     */
    double hpe;

    /**
     * The location score -- a number between 0 (very low score, highly
     * suspect location) and 10 (very high score, highly trusted location).
     *
     * \note only applies to certified locations, ie. \c hasScore is \c true.
     *
     * \since 4.8
     */
    double score;

    /**
     * The number of access-point used to calculate this location.
     *
     * \since 2.4
     */
    unsigned short nap;

    /**
     * A calculated estimate of speed in km/hr.
     *
     * A negative value is used to indicate an unknown speed.
     *
     * \since 2.4
     */
    double speed;

    /**
     * A calculated estimate of bearing as degree from north
     * clockwise (+90 is East).
     *
     * A negative value is used to indicate an unknown bearing.
     *
     * \since 2.4
     */
    double bearing;

    /**
     * physical street address,
     * only returned in the network-centric model
     * when the \c street_address_lookup parameter
     * is set to \c WPS_LIMITED_STREET_ADDRESS_LOOKUP
     * or \c WPS_FULL_STREET_ADDRESS_LOOKUP.
     */
    WPS_StreetAddress* street_address;

    /**
     * The number of cell tower used to calculate this location.
     *
     * \since 3.2
     */
    unsigned short ncell;

    /**
     * The number of unique location area codes used to calculate this location.
     *
     * \since 4.7
     */
    unsigned short nlac;

    /**
     * The number of satellite used to calculate this location.
     *
     * \since 3.2
     */
    unsigned short nsat;

    /**
     * The number of previous locations used to corroborate this location.
     *
     * \since 4.8
     */
    unsigned short historicalLocationCount;

    /**
     * Nonzero if the IP address of the client making the request was
     * used to confirm the location, or zero otherwise.
     *
     * \since 4.8
     */
    unsigned char withIP;

    /**
     * Nonzero if the score was calculated and zero otherwise.
     *
     * \since 4.8
     */
    unsigned char hasScore;

    /**
     * Identifies this location within a list of returned signed certified
     * locations.
     *
     * \note only applies to signed certified locations.
     *
     * \since 4.9.1
     */
    unsigned id;

    /**
     * IP Address as received by Skyhook's servers or NULL otherwise.
     * The presence of an IP address does not imply that \c withIP is \c true.
     *
     * \note only applies to certified locations, ie. \c hasScore is true.
     *
     * \since 4.9.1
     */
    char* ip;

    /**
     * Hash of the measurement data used in this certified location
     * or NULL otherwise.
     *
     * \note only applies to signed certified locations.
     *
     * \since 4.9.1
     */
    unsigned char* hash;

    /**
     * Length of \c hash.
     *
     * \note only applies to signed certified locations.
     *
     * \since 4.9.1
     */
    unsigned hashLength;

    /**
     * Signature computed by Skyhook's remote servers over the contents
     * of this location's fields or \c NULL otherwise.
     *
     * Signature of the WPS_Location serialized as follows:
     *
     * \code{.c}
void
WPS_CertifiedLocationCallback(void* arg,
                              WPS_ReturnCode code,
                              const WPS_Location** locations,
                              unsigned nlocations,
                              const void* reserved)
{
    for (int i = 0; i < nlocations; i++)
    {
        char buf[256];
        int nBytes =
            snprintf(buf,
                     sizeof(buf),
                     "%d/%d,%.6f,%.6f,%d,%.1f,%d,%d,%d,%d,%d,%d,%d,%ul,%s",
                     location[i]->id,
                     nlocations,
                     location[i]->latitude,
                     location[i]->longitude,
                     location[i]->hpe,
                     location[i]->score,
                     location[i]->nap,
                     location[i]->ncell,
                     location[i]->nlac,
                     location[i]->nsat,
                     location[i]->historicalLocationCount,
                     location[i]->withIP ? 1 : 0,
                     location[i]->timestamp,
                     location[i]->ip);
        memcpy(bug + nBytes, location[i]->hash, location[i]->hashLength);
        memcpy(buf + nBytes + saltLen, salt, saltLen);
        ...
    }
}
     * \endcode
     *
     * \par Skyhook public key:
     * \code{.c}
 -----BEGIN CERTIFICATE-----
 MIIDfDCCAmQCCQDs5/f7i+4vIjANBgkqhkiG9w0BAQUFADB/MQswCQYDVQQGEwJV
 UzELMAkGA1UECAwCTUExDzANBgNVBAcMBkJvc3RvbjEeMBwGA1UECgwVU2t5aG9v
 ayBXaXJlbGVzcyBJbmMuMRAwDgYDVQQLDAdTa3lob29rMSAwHgYDVQQDDBdhcGku
 c2t5aG9va3dpcmVsZXNzLmNvbTAgFw0xNDAyMDMxOTA2NDBaGA8zMDEzMDYwNjE5
 MDY0MFowfzELMAkGA1UEBhMCVVMxCzAJBgNVBAgMAk1BMQ8wDQYDVQQHDAZCb3N0
 b24xHjAcBgNVBAoMFVNreWhvb2sgV2lyZWxlc3MgSW5jLjEQMA4GA1UECwwHU2t5
 aG9vazEgMB4GA1UEAwwXYXBpLnNreWhvb2t3aXJlbGVzcy5jb20wggEiMA0GCSqG
 SIb3DQEBAQUAA4IBDwAwggEKAoIBAQC/N4Y/JXq5x1T3F5Q1pViI/b6sHPhrU3bd
 KNLSMkUb4ZPmwyde5ayPLSCAnS891Cmdn+MFolPNVlnu4/odDlIJNkBZxwqLyJjj
 UagIoU6AyRY0gWZh24abMjm8BeKC+8CFFW9DEL/3rZXSLAm+ViVVmEV0+lA2M9+D
 6i/L5Y47DkjsuU5B911uNzhOXS5qaW6WQOFBNtIMK8dEbFhfwMApxQOv4Pw998N7
 v+ylrDhUbeUOlW/mSXi5UEEVn1Q4cmiyiIlDejljIfCgQecPttEAmotNThwh8nCo
 KA6T0hdu6FL2bJmTrVbnz3ftOpuXLhCRTXMrsIICVizVijfxrhsBAgMBAAEwDQYJ
 KoZIhvcNAQEFBQADggEBABfwvRIyVm9UrBiT18Xj2WMJwC3hSvoGvxOVAfYFb0gO
 ciCwkMhhs+88F3CgQbXIILr7iaHoNZcH9zDmm8rZ2a8BMTVSkaF9b43xILTVwGmu
 zi36eWMcvCS1jE9cRhSYhpX6I+scRLC6IRshvuleTX9viE8LwASLvekKEFi8yg07
 8cFkJDRHXrqvYsG1hcDoKsW9ocr4ugj0annTUh38OMgAzVO0L2IFqKkb6+CuDPX7
 jJh+KwZau/0K6hqr+4aB6zwLinQ32nNJnI1UI3ufFbPU9g4V9nzXyHBDF93zaPWF
 R7IQB1FfB8DBgYeQUL9VJKSYUhogQUpSeqmQrwXbrgI=
 -----END CERTIFICATE-----
     * \endcode
     *
     * \see https://api.skyhookwireless.com/wps2/pubkey/skyhook-rsa-x509.cert
     *
     * \note only applies to signed certified locations.
     *
     * \since 4.9.1
     */
    unsigned char* signature;

    /**
     * Length of \c signature.
     *
     * \note only applies to signed certified locations.
     *
     * \since 4.9.1
     */
    unsigned signatureLength;

    /**
     * Timestamp (number of seconds since unix epoch) of this location. This
     * timestamp is calculated on Skyhook’s servers at the time the request
     * is received, and does not account for the network round-trip or the
     * local time of the device.
     *
     * \note only applies to certified locations, ie. \c hasScore is true
     *
     * \since 4.9.1
     */
    unsigned long timestamp;

    /**
     * A calculated altitude above mean sea level in meters.
     *
     * \since 3.2
     */
    double altitude;

    /**
     * Type of calculated location.
     *
     * \since 3.2
     */
    WPS_LocationType type;

    /**
     * Number of milliseconds elapsed since the time the
     * location was calculated
     *
     * \since 3.5
     */
    unsigned long age;
} WPS_Location;

/**
 * Geographic location based on
 * the IP address of the client making the request.
 *
 * \note This information is likely not accurate,
 *       but may give some indication as to the general location
 *       of the request and may provide some hints for the client
 *       software to act and react appropriately.
 */
typedef struct
{
    /**
     * the IP address of the client as received by the server.
     */
    char* ip;

    //@{
    /**
     * the estimated physical geographic location.
     */
    double latitude;
    double longitude;
    //@}

    /**
     * physical street address,
     * only returned when the \c street_address_lookup parameter
     * is set to \c WPS_LIMITED_STREET_ADDRESS_LOOKUP
     * or \c WPS_FULL_STREET_ADDRESS_LOOKUP.
     */
    WPS_StreetAddress* street_address;
} WPS_IPLocation;

/**
 * Return a string containing the version information
 * as <code>&lt;major&gt;.&lt;minor&gt;.&lt;revision&gt;.&lt;build&gt;</code>
 *
 * \return the version information
 *
 * \since 3.0
 */
WPSAPI_EXPORT const char* WPSAPI_CALL
WPS_version();

/**
 * Initializes WPS API.
 * Should be called before making other WPS API calls.
 *
 * \since 4.4
 *
 * \note This call is not mandatory. WPS will be initialized
 *       automatically upon the first API call.
 */
WPSAPI_EXPORT WPS_ReturnCode WPSAPI_CALL
WPS_load();

/**
 * Releases all resources consumed by WPS API and stops any background activity.
 * Must be called before the calling application or module is unloaded.
 *
 * \since 4.4
 */
WPSAPI_EXPORT void WPSAPI_CALL
WPS_unload();

/**
 * Requests geographic location based on observed Wi-Fi access points,
 * cell towers, and GPS signals.
 *
 * \param authentication must be \c NULL for new applications that
 *                       use \c WPS_set_key() or \c WPS_set_registration_user(),
 *                       otherwise set to the user's authentication information.
 * \param street_address_lookup request street address lookup
 *                              in addition to latitude/longitude lookup.
 * \param location pointer to return a \c WPS_Location struct.
 *                 \n
 *                 This pointer must be freed by calling \c WPS_free_location().
 *
 * \return a \c WPS_ReturnCode
 *
 * \par Sample Code:
 * \code{.c}
WPS_Location* location;
WPS_ReturnCode rc;

rc = WPS_location(NULL,
                  WPS_NO_STREET_ADDRESS_LOOKUP,
                  &location);
if (rc != WPS_OK)
{
    fprintf(stderr, "*** WPS_location failed (%d)!\n", rc);
}
else
{
    print_location(location);
    WPS_free_location(location);
}
 * \endcode
 */
WPSAPI_EXPORT WPS_ReturnCode WPSAPI_CALL
WPS_location(const WPS_SimpleAuthentication* authentication,
             WPS_StreetAddressLookup street_address_lookup,
             WPS_Location** location);

/**
 * Callback routine for \c WPS_periodic_location().
 *
 * \param arg the \c arg passed to \c WPS_periodic_location().
 * \param code the \c WPS_ReturnCode of the last request.
 * \param location If \c code is \c WPS_OK points to a \c WPS_Location
 *                 \n
 *                 This pointer does \e not need to be freed.
 *
 * \return \c WPS_CONTINUE if \c WPS_periodic_location() should continue,
 *         \n
 *         \c WPS_STOP if \c WPS_periodic_location() should stop.
 *
 * \since 2.4
 *
 * \par Sample Code:
 * \code{.c}
WPS_Continuation
periodic_callback(void*,
                  WPS_ReturnCode code,
                  const WPS_Location* location,
                  const void*)
{
    if (code != WPS_OK)
    {
        fprintf(stderr, "*** failure (%d)!\n", code);
    }
    else
    {
        print_location(location);
    }

    return WPS_CONTINUE;
}
 * \endcode
 */
typedef WPS_Continuation
    (WPSAPI_CALL *WPS_LocationCallback)(void* arg,
                                        WPS_ReturnCode code,
                                        const WPS_Location* location,
                                        const void* reserved);

/**
 * Callback routine for \c WPS_certified_location().
 *
 * \param arg the \c arg passed to \c WPS_certified_location().
 * \param code the \c WPS_ReturnCode of the last request.
 * \param locations If \c code is \c WPS_OK points to an array of pointers to \c WPS_Location
 *                 \n
 *                 This pointer does \e not need to be freed.
 * \param locationsSize size of locations array
 *
 * \return \c WPS_CONTINUE if \c WPS_certified_location() should continue,
 *         \n
 *         \c WPS_STOP if \c WPS_certified_location() should stop.
 *
 * \since 4.8
 */
typedef WPS_Continuation
(WPSAPI_CALL *WPS_CertifiedLocationCallback)(void* arg,
                                             WPS_ReturnCode code,
                                             const WPS_Location** locations,
                                             unsigned nlocations,
                                             const void* reserved);

/**
 * Requests periodic geographic location based on observed Wi-Fi access points,
 * cell towers, and GPS signals.
 *
 * \param authentication must be \c NULL for new applications that
 *                       use \c WPS_set_key() or \c WPS_set_registration_user(),
 *                       otherwise set to the user's authentication information.
 * \param street_address_lookup request street address lookup
 *                              in addition to latitude/longitude lookup
 *                              \n
 *                              Note that street address lookup is only
 *                              performed when the location is determined
 *                              by the remote server (network-centric model),
 *                              not when the location is determined locally.
 * \param period maximum time in milliseconds between location reports.
 *               \n
 *               WPS will invoke the callback at least every period, either
 *               with a location update or with an error if no location can be
 *               determined. WPS may report location updates more often than
 *               the user period if a new or better location becomes available
 *               within a given period.
 *               \n
 *               This parameter is also used to specify the preferred Wi-Fi
 *               scan period, which WPS may override for optimal performance
 *               and power savings.
 * \param iterations number of times a location is to be reported.
 *                   \n
 *                   A value of zero indicates an unlimited number of
 *                   iterations.
 * \param callback the callback routine to report locations to.
 * \param arg an opaque parameter passed to the callback routine.
 * \return a \c WPS_ReturnCode
 *
 * \pre \c period must be strictly greater than 0.
 *
 * \note This call is blocking -- it will not return until a- an error
 *       occurs while setting up, or b- the number of iterations
 *       have been processed, or c- the \c callback returns \c WPS_STOP.
 *
 * \since 2.4
 *
 * \par Sample Code:
 * \code{.c}
WPS_ReturnCode rc;

rc = WPS_periodic_location(NULL,
                           WPS_NO_STREET_ADDRESS_LOOKUP,
                           1000,
                           0,
                           periodic_callback,
                           NULL);
if (rc != WPS_OK)
{
    fprintf(stderr, "*** WPS_periodic_location failed (%d)!\n", rc);
}
 * \endcode
 */
WPSAPI_EXPORT WPS_ReturnCode WPSAPI_CALL
WPS_periodic_location(const WPS_SimpleAuthentication* authentication,
                      WPS_StreetAddressLookup street_address_lookup,
                      unsigned long period,
                      unsigned iterations,
                      WPS_LocationCallback callback,
                      void* arg);

/**
 * Requests certified periodic geographic location based on
 * observed Wi-Fi access points, cell towers, and GPS signals.
 *
 * \param authentication must be \c NULL for new applications that
 *                       use \c WPS_set_key() or \c WPS_set_registration_user(),
 *                       otherwise set to the user's authentication information.
 * \param street_address_lookup request street address lookup
 *                              in addition to latitude/longitude lookup
 *                              \n
 *                              Note that street address lookup is only
 *                              performed when the location is determined
 *                              by the remote server (network-centric model),
 *                              not when the location is determined locally.
 * \param callback the callback routine to report locations to.
 * \param arg an opaque parameter passed to the callback routine.
 * \return a \c WPS_ReturnCode
 *
 * \note This call is blocking -- it will not return until a- an error
 *       occurs while setting up, or b- the number of iterations
 *       have been processed, or c- the \c callback returns \c WPS_STOP.
 *
 * \since 4.8
 */
WPSAPI_EXPORT WPS_ReturnCode WPSAPI_CALL
WPS_certified_location(const WPS_SimpleAuthentication* authentication,
                       WPS_StreetAddressLookup street_address_lookup,
                       WPS_CertifiedLocationCallback callback,
                       void* arg);

/**
 * Requests certified geographic location updates that are signed by
 * Skyhook's servers. The user may verify the signature of the returned
 * location using Skyhook's public certificate.
 *
 * \see \c signature in \c WPS_Location for details on the format of the signature.
 *
 * \see sample code for verifying the signature in the \c example/CertifiedSignatureTestJava
 *      and \c example/CertifiedSignatureTestPhp directories
 *
 * \param authentication must be \c NULL for new applications that
 *                       use \c WPS_set_key() or \c WPS_set_registration_user(),
 *                       otherwise set to the user's authentication information.
 * \param street_address_lookup request street address lookup
 *                              in addition to latitude/longitude lookup
 *                              \n
 *                              Note that street address lookup is only
 *                              performed when the location is determined
 *                              by the remote server (network-centric model),
 *                              not when the location is determined locally.
 * \param salt random data that is used as an additional input to the hash
 *             of the returned location.
 * \param saltLen length of the salt.
 * \param callback the callback routine to report locations to.
 * \param arg an opaque parameter passed to the callback routine.
 * \return a \c WPS_ReturnCode
 *
 * \note This call is blocking -- it will not return until a- an error
 *       occurs while setting up, or b- the number of iterations
 *       have been processed, or c- the \c callback returns \c WPS_STOP.
 * \note The \c salt is a mandatory parameter and will return \c WPS_ERROR
 *       if \c salt is \c NULL or \c saltLen is not greater than 0.
 * \note This call requires the device's local time to be correct.
 *       \c WPS_ERROR_NOT_SIGNED is returned to \c WPS_CertifiedLocationCallback
 *       if the local time is outside of the acceptable range.
 *
 * \since 4.9.1
 */
WPSAPI_EXPORT WPS_ReturnCode WPSAPI_CALL
WPS_signed_certified_location(const WPS_SimpleAuthentication* authentication,
                              WPS_StreetAddressLookup street_address_lookup,
                              const unsigned char* salt,
                              unsigned saltLen,
                              WPS_CertifiedLocationCallback callback,
                              void* arg);

/**
 * Request geographic location information based on
 * the IP address of the client making the request.
 *
 * \note  This information is likely not accurate,
 *        but may give some indication as to the general location
 *        of the request and may provide some hints for the client
 *        software to act and react appropriately.
 * \par
 *        \c WPS_ip_location() requires network connectivity.
 *
 * \param authentication must be \c NULL for new applications that
 *                       use \c WPS_set_key() or \c WPS_set_registration_user(),
 *                       otherwise set to the user's authentication information.
 * \param street_address_lookup request street address lookup
 *                              in addition to lat/long lookup.
 * \param location pointer to return a \c WPS_IPLocation struct.
 *                 \n
 *                 This pointer must be freed by calling \c WPS_free_ip_location()
 *
 * \return a \c WPS_ReturnCode
 *
 * \par Sample Code:
 * \code{.c}
WPS_IPLocation* location;
WPS_ReturnCode rc;

rc = WPS_ip_location(NULL,
                     WPS_NO_STREET_ADDRESS_LOOKUP,
                     &location);
if (rc != WPS_OK)
{
    fprintf(stderr, "*** WPS_ip_location failed (%d)!\n", rc);
}
else
{
    print_ip_location(location);
    WPS_free_ip_location(location);
}
 * \endcode
 */
WPSAPI_EXPORT WPS_ReturnCode WPSAPI_CALL
WPS_ip_location(const WPS_SimpleAuthentication* authentication,
                WPS_StreetAddressLookup street_address_lookup,
                WPS_IPLocation** location);

/**
 * Retrieve a secure offline token based on the latest observed
 * Wi-Fi access points and cell towers. This token can be saved and
 * converted to a location later by calling \c WPS_offline_location().
 *
 * \param authentication must be \c NULL for new applications that
 *                       use \c WPS_set_key() or \c WPS_set_registration_user(),
 *                       otherwise set to the user's authentication information.
 * \param key a key used to encrypt the token.
 *            \n
 *            The longer the key the more effective the encryption.
 *            It is recommended to have a key of at least 16 bytes.
 * \param key_length the length of the key.
 * \param token a pointer to receive the token.
 *              \n
 *              This pointer must be freed by calling
 *              \c WPS_free_offline_token().
 * \param token_size a pointer to receive the size of \c token.
 *
 * \return a \c WPS_ReturnCode
 *
 * \note A sensible time to call \c WPS_offline_token() is after either
 *       \c WPS_location() failed with \c WPS_ERROR_SERVER_UNAVAILABLE
 *       or \c WPS_ERROR_LOCATION_CANNOT_BE_DETERMINED, or from the
 *       \c WPS_periodic_location() callback again with a
 *       \c WPS_ERROR_SERVER_UNAVAILABLE or \c WPS_ERROR_LOCATION_CANNOT_BE_DETERMINED
 *       error code.
 * \par
 *       Offline tokens are only valid for 90 days after they are generated.
 *       Attempting to redeem a token more than 90 days old will result in an error.
 *
 * \see \c WPS_offline_location()
 * \see \c WPS_free_offline_token()
 *
 * \since 4.5
 *
 * \par Sample Code:
 * \code{.c}
WPS_ReturnCode rc;
unsigned char key[16];
unsigned char* token;
unsigned token_size;

// generate the key
// ...

rc = WPS_offline_token(NULL,
                       key,
                       sizeof(key),
                       &token,
                       &token_size);
if (rc != WPS_OK)
{
    fprintf(stderr, "*** WPS_offline_token failed (%d)!\n", rc);
}
else
{
    // save the key and token in persistent storage
    // ...

    WPS_free_offline_token(token);
}
 *
 * \endcode
 */
WPSAPI_EXPORT WPS_ReturnCode WPSAPI_CALL
WPS_offline_token(const WPS_SimpleAuthentication* authentication,
                  const unsigned char* key,
                  unsigned key_length,
                  unsigned char** token,
                  unsigned* token_size);

/**
 * Request a geographic location from a token collected
 * at a different time.
 *
 * \param authentication must be \c NULL for new applications that
 *                       use \c WPS_set_key() or \c WPS_set_registration_user(),
 *                       otherwise set to the user's authentication information.
 * \param key a key used to encrypt the \c token.
 * \param key_length the length of the key used to encrypt the \c token
 * \param token the token to be converted to a location
 * \param token_size the size of the \c token
 * \param location pointer to return a \c WPS_Location struct.
 *                 \n
 *                 This pointer should be freed by calling \c WPS_free_location()
 *
 * \return a \c WPS_ReturnCode
 *
 * \note A different \c username can be used to replay a token than
 *       the \c username used to collect the token, however both users
 *       must belong to the same \c realm.
 * \par
 *       Offline tokens are only valid for 90 days after they are generated.
 *       Attempting to redeem a token more than 90 days old will result in an error.
 * \par
 *       The same key used to encrypt the token in \c WPS_offline_token()
 *       must be provided to \c WPS_offline_location().
 * \par
 *       Although a token can be collected when a location has been
 *       calculated \c WPS_offline_location() is not guaranteed
 *       to return a location with that same token.
 *
 * \see \c WPS_offline_token()
 *
 * \since 4.5
 *
 * \par Sample Code:
 * \code{.c}
WPS_Location* location;
WPS_ReturnCode rc;
unsigned char key[16];
unsigned char* token;
unsigned token_size;

// restore the key and token stored earlier
// ...

rc = WPS_offline_location(NULL,
                          key,
                          sizeof(key),
                          token,
                          token_size,
                          &location);
if (rc != WPS_OK)
{
    fprintf(stderr, "*** WPS_offline_location failed (%d)!\n", rc);
}
else
{
    print_location(location);
    WPS_free_location(location);
}
 * \endcode
 */
WPSAPI_EXPORT WPS_ReturnCode WPSAPI_CALL
WPS_offline_location(const WPS_SimpleAuthentication* authentication,
                     const unsigned char* key,
                     unsigned key_length,
                     const unsigned char* token,
                     unsigned token_size,
                     WPS_Location** location);

/**
 * Free a token returned by \c WPS_offline_token().
 *
 * \since 4.5
 */
WPSAPI_EXPORT void WPSAPI_CALL
WPS_free_offline_token(unsigned char* token);

/**
 * Free a \c WPS_Location struct returned by \c WPS_location().
 */
WPSAPI_EXPORT void WPSAPI_CALL
WPS_free_location(WPS_Location*);

/**
 * Free a \c WPS_IPLocation struct returned by \c WPS_ip_location().
 */
WPSAPI_EXPORT void WPSAPI_CALL
WPS_free_ip_location(WPS_IPLocation*);

/**
 * Setup a proxy server.
 *
 * \param address the internet address of the proxy server.
 * \param port the TCP port number of the proxy server.
 * \param user the username to authenticate with the proxy server.
 * \param password the password to authentication with the proxy server.
 *
 * \note \c user and \c password are optional and can be set to \c NULL
 *       for un-authenticated proxy servers.
 *
 * \return a \c WPS_ReturnCode
 */
WPSAPI_EXPORT void WPSAPI_CALL
WPS_set_proxy(const char* address,
              int port,
              const char* user,
              const char* password);

/**
 * Overwrite the WPS server's url from its default value.
 *
 * \param url the url to the server.
 *            \n
 *            A value of \c NULL turns off remote determination of location.
 */
WPSAPI_EXPORT void WPSAPI_CALL
WPS_set_server_url(const char* url);

/**
 * Overwrite the default user agent of the requests going to the server.
 *
 * \param ua the user agent string.
 *
 * \since 2.7
 */
WPSAPI_EXPORT void WPSAPI_CALL
WPS_set_user_agent(const char* ua);

/**
 * Set the path to local files so location determination can be performed
 * locally.
 *
 * \param paths an array (terminated by \c NULL) of complete path to local files.
 *              \n
 *              This array replaces all previous values.
 *              \n
 *              Use \c NULL to clear and release local files.
 *
 * \return a \c WPS_ReturnCode
 *
 * \deprecated Replaced by the tiling model.
 *
 * \since 2.4
 *
 * \par Sample Code:
 * \code{.c}
const char* paths[] = { "myfile1", "myfile2", NULL };
WPS_ReturnCode rc = WPS_set_local_files_path(paths);
if (rc != WPS_OK)
{
    fprintf(stderr, "*** WPS_set_local_files_path failed (%d)!\n", rc);
}
 * \endcode
 */
WPSAPI_EXPORT WPS_ReturnCode WPSAPI_CALL
WPS_set_local_files_path(const char** paths);

/**
 * Setup the tier2 area.
 *
 * \param dirpath the path to a directory to store the tier2 files.
 * \param size the approximate max total size of the tier2 files,
 *             in kilobytes.
 *
 * \return a \c WPS_ReturnCode
 *
 * \deprecated This call is no longer relevant.
 *
 * \since 2.6
 */
WPSAPI_EXPORT WPS_ReturnCode WPSAPI_CALL
WPS_set_tier2_area(const char* dirpath, unsigned size);

/**
 * Callback routine for \c WPS_set_tiling().
 *
 * \param arg the \c arg passed to \c WPS_set_tiling().
 * \param tileNumber the number of the tile to be downloaded next.
 * \param tileTotal the total number of tiles to be downlaoded
 *                  (in this session).
 *
 * \note \c tileNumber ranges from 0 to \c tileTotal (not inclusive),
 *       but if a tile has already been downloaded it will be skipped
 *       and the callback will not be called for that tile.
 *
 * \return \c WPS_CONTINUE if \c WPS should continue downloading tiles,
 *         \n
 *         \c WPS_STOP if \c WPS should stop downloading tiles.
 *
 * \since 2.6
 *
 * \par Sample Code:
 * \code{.c}
WPS_Continuation
tiling_callback(void*,
                unsigned tileNumber,
                unsigned tileTotal)
{
    printf("downloading tile %d/%d...\n", tileNumber, tileTotal);
    return WPS_CONTINUE;
}
 * \endcode
 */
typedef WPS_Continuation
    (WPSAPI_CALL *WPS_TilingCallback)(void* arg,
                                      unsigned tileNumber,
                                      unsigned tileTotal);

/**
 * Setup tiling so location determination can be performed
 * locally.
 * \n
 * Tiles are managed automatically (download of new tiles when
 * required and remove of old tiles) by \c WPS_location and
 * \c WPS_periodic_location.
 *
 * \param authentication must be \c NULL for new applications that
 *                       use \c WPS_set_key() or \c WPS_set_registration_user(),
 *                       otherwise set to the user's authentication information.
 * \param dirpath the path to a directory to store tiles.
 *        \n
 *        \c dirpath must end with a directory separator character.
 *        (i.e. '\' on Windows platforms and '/' on Unix platforms)
 * \param maxDataSizePerSession the maximum size of data downloaded
 *                              per session, in bytes.
 *                              \n
 *                              A value of 0 disables
 *                              any further tile downloads.
 * \param maxDataSizeTotal the maximum size of all stored tiles, in bytes.
 *                         \n
 *                         A value of 0 causes all stored tiles to be deleted.
 * \param callback the callback function to control the tile download.
 *                 By default, all tiles are downloaded.
 * \param arg the opaque parameter to pass to the callback function.
 *
 * \note Tiles are typically less then 50KB in size, so to download an area
 *       of 3x3 tiles for each session you would set \c maxDataSizePerSession
 *       to 450KB, i.e. 460,800.
 * \par
 *       It is recommended that \c maxDataSizePerSession be
 *       a factor of 2 - 10 smaller than \c maxDataSizeTotal,
 *       so that tiles from several areas can be cached.
 * \par
 *       Applications should not attempt to access tiles directly,
 *       in particular to delete them.
 *
 * \return a \c WPS_ReturnCode
 *
 * \since 3.3
 *
 * \par Sample Code:
 * \code{.c}
WPS_ReturnCode rc;

rc = WPS_set_tiling(NULL,
                    "/tiles/",
                    450 * 1024, // 450KB
                    2 * 1024 * 1024, // 2MB
                    NULL,
                    NULL);
if (rc != WPS_OK)
{
    fprintf(stderr, "*** WPS_set_tiling failed (%d)!\n", rc);
}
 * \endcode
 */
WPSAPI_EXPORT WPS_ReturnCode WPSAPI_CALL
WPS_set_tiling(const WPS_SimpleAuthentication* authentication,
               const char* dirpath,
               unsigned maxDataSizePerSession,
               unsigned maxDataSizeTotal,
               WPS_TilingCallback callback,
               void* arg);

/**
 * \deprecated Replaced by \c WPS_set_tiling()
 *
 * \since 2.6
 */
WPSAPI_EXPORT WPS_ReturnCode WPSAPI_CALL
WPS_tiling(const char* dirpath,
           unsigned maxDataSizePerSession,
           unsigned maxDataSizeTotal,
           WPS_TilingCallback callback,
           void* arg);

/**
 * Set the user's API key.
 *
 * \param key an existing user's API key.
 *
 * This is the preferred method to authenticate for new applications.
 * \n
 * Make sure to call this method before making any other calls,
 * such as during initialization of the application.
 * \n
 * After this call you should pass \c NULL as \c authentication
 * to other calls like \c WPS_location.
 *
 * \note Must not be used in conjunction with \c WPS_set_registration_user()
 *
 * \since 4.9
 */
WPSAPI_EXPORT WPS_ReturnCode WPSAPI_CALL
WPS_set_key(const char* key);

/**
 * Set the user's authentication token.
 *
 * \param key an existing user's authentication token.
 *
 * This is an authentication method alternative to the API key
 * based authentication.
 * \n
 * Make sure to call this method before making any other calls,
 * such as during initialization of the application.
 * \n
 * After this call you should pass \c NULL as \c authentication
 * to other calls like \c WPS_location.
 *
 * \note Must not be used in conjunction with \c WPS_set_key()
 *       or \c WPS_set_registration_user()
 *
 * \since 4.9.8
 */
WPSAPI_EXPORT WPS_ReturnCode WPSAPI_CALL
WPS_set_auth_token(const char* token);

/**
 * Set the user's authentication information.
 *
 * \deprecated Replaced by \c WPS_set_key()
 *
 * \param authentication an existing user's authentication information.
 *
 * Make sure to call this method before making any other calls,
 * such as during initialization of the application.
 * \n
 * After this call you should pass \c NULL as \c authentication
 * to other calls like \c WPS_location.
 *
 * \note Must not be used in conjunction with \c WPS_set_key()
 *
 * \since 4.8
 */
WPSAPI_EXPORT WPS_ReturnCode WPSAPI_CALL
WPS_set_registration_user(const WPS_SimpleAuthentication* authentication);

/**
 * Register a new user.
 *
 * \deprecated Replaced by \c WPS_set_key()
 *
 * \param authentication an existing user's authentication information.
 * \param new_authentication the new user's authentication information
 *                           or \c NULL to trigger auto-registration.
 *
 * \return a \c WPS_ReturnCode
 *
 * \since 2.3.1
 *
 * \par Sample Code:
 * New applications are encouraged to simply use the auto-registration feature
 * by calling \c WPS_register_user(), maybe during initialization of the
 * application. For example:
 * \code{.c}
    // info from Skyhook
    #define WPS_USERNAME "myusername"
    #define WPS_REALM "myrealm"

    WPS_SimpleAuthentication wpsUser;
    WPS_ReturnCode rc;

    void wpsInit()
    {
        wpsUser.username = WPS_USERNAME;
        wpsUser.realm = WPS_REALM;

        while ((rc = WPS_register_user(&wpsUser, NULL)) != WPS_OK)
        {
             // registration failed... retry

            sleep(5 + rand() * 5); // some randomness
        }
    }
 * \endcode
 * Applications that still want to control how usernames are generated can
 * continue to do so as they did before since version 4.2 is backward compatible
 * with 4.1. For example:
 * \code{.c}
    // info from Skyhook
    #define WPS_USERNAME "myusername"
    #define WPS_REALM "myrealm"

    WPS_SimpleAuthentication wpsUser;

    void saveUsername(const char*); // save registered username
    const char* readUsername(); // read registered username, or NULL if not found
    const char* generateUsername(); // your own generated username

    void wpsInit()
    {
        WPS_SimpleAuthentication wpsReg;
        WPS_ReturnCode rc;

        const char* savedUsername = readUsername(); // retrieve previously registered username
        const char* generatedUsername = generateUsername();

        wpsUser.username = generatedUsername;
        wpsUser.realm = WPS_REALM;

        if (savedUsername != NULL && strcmp(savedUsername, generatedUsername) == 0)
        {
            // user already registered (and match generated username)
            return;
        }

        wpsReg.username = WPS_USERNAME;
        wpsReg.realm = WPS_REALM;

        while ((rc = WPS_register_user(&wpsReg, &wpsUser) != WPS_OK)
        {
             // registration failed... retry

            sleep(5 + rand() * 5); // some randomness
        }

        saveUsername(generatedusername); // save registered username
    }
 * \endcode
 * Note that the generated username should be stable -- ie. remain constant
 * over time, every time it is generated -- to avoid unneccesary
 * re-registrations.
 * Care should be taken when generating a username to protect the privacy
 * of the user, and should not contain any personally identifiable information.
 * This can be achieved, for example, by hashing a unique identifier of the
 * device, like the IMEI, or the MAC, or some other hardware ID.
 * \par
 * Once registration is complete \c wpsUser can be used in any \c WPS_* calls.
 * For example:
 * \code{.c}
    WPS_Location* location;
    WPS_ReturnCode rc = WPS_location(&wpsUser,
                                     WPS_NO_STREET_ADDRESS_LOOKUP,
                                     &location);
 * \endcode
 */
WPSAPI_EXPORT WPS_ReturnCode WPSAPI_CALL
WPS_register_user(const WPS_SimpleAuthentication* authentication,
                  const WPS_SimpleAuthentication* new_authentication);

/**
 * Tune the last location returned.
 *
 * \param authentication an existing user's authentication information.
 * \param location the tuned location.
 *
 * \return a \c WPS_ReturnCode
 *
 * \note The device must be at the location specified in
 *       \c location during the entire duration of this call. Note
 *       that pure GPS locations will not reflect any tuning.
 * \par
 *       Tuning is not an effective strategy for trying to make small
 *       adjustments to a location.  It is best used for adding to our
 *       coverage or fixing results that are off by hundreds of meters.
 *
 * \since 3.0
 */
WPSAPI_EXPORT WPS_ReturnCode WPSAPI_CALL
WPS_tune_location(const WPS_SimpleAuthentication* authentication,
                  const WPS_Location* location);

/**
 * Geofence type.
 *
 * \since 4.4
 */
typedef enum
{
    /**
     * identify a geofence that triggers when the user enters the defined zone
     */
    WPS_GEOFENCE_ENTER,

    /**
     * identify a geofence that triggers when the user leaves the defined zone
     */
    WPS_GEOFENCE_LEAVE,

    /**
      * identify a geofence that triggers when the user's location starts within or
      * enters the defined zone
      *
      * \since 4.8
      */
    WPS_GEOFENCE_INSIDE,

    /**
      * identify a geofence that triggers when the user's location starts outside of
      * or leaves the defined zone
      *
      * \since 4.8
      */
    WPS_GEOFENCE_OUTSIDE
} WPS_GeoFenceType;

/**
 * Geofence.
 *
 * \since 4.4
 */
typedef struct
{
    /**
     * Store the size of \c WPS_GeoFence, and must be initialized
     * to \c sizeof(WPS_GeoFence).
     * \n
     * Used as a versioning mechanism.
     *
     * \par Sample Code:
     * \code{.c}
WPS_GeoFence geofence;
geofence.size = sizeof(WPS_GeoFence);
     * \endcode
     */
    unsigned size;

    //@{
    /**
     * The center of the geofence zone
     */
    double latitude;
    double longitude;
    //@}

    /**
     * The radius of the geofence zone, in meters
     */
    unsigned radius;

    /**
     * The type of the geofence
     */
    WPS_GeoFenceType type;

    /**
     * The desired Wi-Fi period when the device is outside
     * a \c WPS_GEOFENCE_ENTER geofence,
     * or is inside a \c WPS_GEOFENCE_LEAVE geofence.
     * \n
     * A value of \c 0 can be used if no preferred period is desired
     * (for this geofence).
     */
    unsigned long period;
} WPS_GeoFence;

/**
 * Geofence handle.
 *
 * \since 4.4
 */
typedef void* WPS_GeoFenceHandle;

/**
 * Callback routine for \c WPS_geofence_set().
 *
 * For a \c WPS_GEOFENCE_ENTER geofence, this callback will be called whenever
 * the user enters the geofence (but only once for as long as the user
 * remains inside the geofence).
 * For a \c WPS_GEOFENCE_LEAVE geofence, this callback will be called whenever
 * the user leaves the geofence (but only once for as long as the user
 * remains outside the geofence).
 *
 * \param arg the \c arg passed to \c WPS_geofence_set().
 * \param geofence the geofence that triggered this callback.
 * \param location the actual calculated location.
 * \return \c WPS_CONTINUE if \c WPS_periodic_location() should continue,
 *         \n
 *         \c WPS_STOP if \c WPS_periodic_location() should stop (and no
 *         more callback will be invoked).
 *
 * \since 4.4
 *
 * \par Sample Code:
 * \code{.c}
WPS_Continuation
geofence_callback(void*,
                  const WPS_GeoFence* geofence,
                  const WPS_Location* location,
                  const void*)
{
    if (geofence->type == WPS_GEOFENCE_ENTER)
    {
        printf("entering");
    }
    else // geofence->type == WPS_GEOFENCE_LEAVE
    {
        printf("leaving");
    }
    printf(" geofence (%.6f, %.6f +/-%um) at %.6f, %.6f +/-%.0fm\n",
           geofence->latitude,
           geofence->longitude,
           geofence->radius,
           location->latitude,
           location->longitude,
           location->hpe);

    return WPS_CONTINUE;
}
 * \endcode
 */
typedef WPS_Continuation
    (WPSAPI_CALL *WPS_GeoFenceCallback)(void* arg,
                                        const WPS_GeoFence* geofence,
                                        const WPS_Location* location,
                                        const void* reserved);

/**
 * Define a new geofence.
 *
 * \param geofence the geofence to monitor.
 * \param callback the \c WPS_GeoFenceCallback to invoke when this geofence is triggered.
 * \param arg an opaque parameter passed to the \c WPS_GeoFenceCallback.
 * \param handle receives \c WPS_GeoFenceHandle which can be used to cancel this geofence.

 * \return a \c WPS_ReturnCode.
 *
 * \see WPS_geofence_cancel()
 *
 * \since 4.4
 *
 * \par Sample Code:
 * \code{.c}
WPS_GeoFence geofence;
WPS_GeoFenceHandle handle;
WPS_ReturnCode rc;

geofence.size = sizeof(WPS_GeoFence);
geofence.latitude = 42.1234;
geofence.longitude = -71.5678;
geofence.radius = 100;
geofence.type = WPS_GEOFENCE_ENTER;
geofence.period = 0;

rc = WPS_geofence_set(&geofence,
                      geofence_callback,
                      NULL,
                      &handle);
if (rc != WPS_OK)
{
    fprintf(stderr, "*** WPS_geofence_set failed (%d)!\n", rc);
}

rc = WPS_periodic_location(NULL,
                           WPS_NO_STREET_ADDRESS_LOOKUP,
                           60 * 60 * 1000,
                           0,
                           NULL,
                           NULL);
if (rc != WPS_OK)
{
    fprintf(stderr, "*** WPS_periodic_location failed (%d)!\n", rc);
}

rc = WPS_geofence_cancel(handle);
if (rc != WPS_OK)
{
    fprintf(stderr, "*** WPS_geofence_cancel failed (%d)!\n", rc);
}

rc = WPS_geofence_cancel_all();
if (rc != WPS_OK)
{
    fprintf(stderr, "*** WPS_geofence_cancel_all failed (%d)!\n", rc);
}
 * \endcode
 */
WPSAPI_EXPORT WPS_ReturnCode WPSAPI_CALL
WPS_geofence_set(const WPS_GeoFence* geofence,
                 WPS_GeoFenceCallback callback,
                 void* arg,
                 WPS_GeoFenceHandle* handle);

/**
 * Cancel a geofence.
 *
 * \param handle the handle of the geofence to cancel.
 * \return a \c WPS_ReturnCode. \c WPS_GEOFENCE_ERROR indicates
 *         the geofence for this handle isn't defined.
 *
 * \see \c WPS_geofence_set()
 *
 * \since 4.4
 */
WPSAPI_EXPORT WPS_ReturnCode WPSAPI_CALL
WPS_geofence_cancel(WPS_GeoFenceHandle handle);

/**
 * Cancel all geofences.
 *
 * \return a \c WPS_ReturnCode
 *
 * \see \c WPS_geofence_set()
 *
 * \since 4.4
 */
WPSAPI_EXPORT WPS_ReturnCode WPSAPI_CALL
WPS_geofence_cancel_all();

/**
 * Set a tunable value.
 *
 * \return a \c WPS_ReturnCode
 *
 * \since 4.9.5
 */
WPSAPI_EXPORT WPS_ReturnCode WPSAPI_CALL
WPS_set_tunable(const char* key, const char* value);

#ifdef __cplusplus
}
#endif

#endif // _WPSAPI_H_
