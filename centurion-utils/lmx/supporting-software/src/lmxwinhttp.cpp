//------------------------------------------------------------------------------
//                     Copyright (c) 2008 Codalogic Ltd.
//
// This file forms part of THE SUPPORTING SOFTWARE of Codalogic's LMX W3C
// Schema to C++ code generator product.  It may not be used independent of
// files legitimately generated and used by the LMX product.  See the LMX
// license agreement for further restrictions on the use of this file.
//
// Users may edit the contents of this file to suit their platform needs
// subject to the aforementioned restrictions remaining in effect on the
// derived work.
//
// THIS FILE AND THE SOFTWARE CONTAINED HEREIN IS PROVIDED 'AS IS' AND COMES
// WITH NO WARRANTIES OF ANY KIND.
//
// CODALOGIC LTD SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY ANYONE
// OR ANYTHING DUE TO THE USE OF THIS FILE HOWEVER THEY MAY BE CAUSED.
//
// For more information on the LMX compiler, go to:
//
//     http://www.xml2cpp.com
//
// Also see:
//
//     http://www.codalogic.com
//
// Documentation:
//      This modules uses Microsoft's WinHTTP code to provide HTTP
//      functionality for SOAP operations.  It is provided in source code form
//      to allow you to customise it to your needs.
//
// Version: 3.8
//      - First inclusion in LMX.
//------------------------------------------------------------------------------

#include <vector>

#include <windows.h>

// Allow winhttp.h to be used with vc6's and vc7's version of windows.h
#if defined( _MSC_VER ) && _MSC_VER <= 1200
typedef DWORD *DWORD_PTR;
#endif

#if defined( _MSC_VER ) && _MSC_VER <= 1310
#define __in
#define __out
#define __inout
#define __out_ecount(x)
#define __in_ecount(x)
#define __out_ecount_full_opt(x)
#endif

// winhttp.h is included in the VS2008 SDK.  For IDEs prior to VS2008,
// winhttp.h is available in the Windows Server 2003 SP1 Platform SDK
// which can be downloaded from:
//      http://www.microsoft.com/downloads/details.aspx?FamilyId=A55B6B43-E24F-4EA3-A93E-40C0EC4F68E5&displaylang=en
// When installed from this location, winhttp.h is located by default in:
//      C:\Program Files\Microsoft Platform SDK\Include
// and the project's properties need to be modified accordingly.
// WinHTTP is supported from Windows Server 2003, Windows XP SP1, and
// Windows 2000 Professional SP3 and later.  To operate on Windows
// operating systems prior to this an alternative HTTP solution is
// required, such as WinInet.
#include <winhttp.h>

#include "lmxsoap.h"

#include "lmxwinhttp.h"

namespace lmx {

const size_t c_winhttp::k_default_port = INTERNET_DEFAULT_PORT;
const size_t c_winhttp::k_default_http_port = INTERNET_DEFAULT_HTTP_PORT;
const size_t c_winhttp::k_default_https_port = INTERNET_DEFAULT_HTTPS_PORT;

c_winhttp::c_winhttp(
        const std::string &ar_host,
        size_t a_port /*=k_default_port*/,
        bool a_is_secure /*=false*/ )
        :
        host( ar_host ),
        port( a_port ),
        is_secure( a_is_secure ),
        http_error_code( 0 ),
        h_session( LMXNULL ),
        h_connect( LMXNULL )
{
    // Use WinHttpOpen to obtain a session handle.
    h_session = WinHttpOpen(
                        L"LMX WinHTTP/1.0",
                        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,  // Proxy info retrieved from registry
                        WINHTTP_NO_PROXY_NAME,
                        WINHTTP_NO_PROXY_BYPASS,
                        0 );            // dwFlags (e.g. WINHTTP_FLAG_ASYNC)

    // Specify an HTTP server.
    if( h_session )
        {
        std::wstring l_whost;
        h_connect = WinHttpConnect(
                        h_session,
                        convert( &l_whost, host ).c_str(),
                        (INTERNET_PORT)port,
                        0 );            // dwReserved
        }

    if( ! h_connect )   // Also captures ! h_session
        http_error_code = GetLastError();
}

c_winhttp::~c_winhttp()
{
    if( h_connect ) WinHttpCloseHandle( h_connect );
    if( h_session ) WinHttpCloseHandle( h_session );
}

BOOL c_winhttp::p_get_reponse( HINTERNET ah_request, std::string *ap_response_xml )
{
    // End the request.
    BOOL l_result = WinHttpReceiveResponse( ah_request, NULL );

    //get status code back from the response
    if( l_result )
        {
        DWORD l_status_code = 0;
        DWORD l_status_code_size = sizeof(l_status_code);

        l_result = WinHttpQueryHeaders(
                        ah_request,
                        WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
                        NULL,
                        &l_status_code,
                        &l_status_code_size,
                        WINHTTP_NO_HEADER_INDEX );

        if( l_status_code < HTTP_STATUS_OK || l_status_code > HTTP_STATUS_ACCEPTED )
            {
            http_error_code = LMXWINHTTP_ERROR_HTTP_STATUS_NOT_OK;
            l_result = 0;
            }
        }

    // Keep checking for data until there is nothing left.
    if( l_result )
        {
        DWORD l_size = 0;
        DWORD l_n_downloaded = 0;
        std::vector< char > l_buffer;

        do
            {
            // Check for available data.
            if( ! WinHttpQueryDataAvailable( ah_request, &l_size ) )
                {
                l_result = FALSE;
                break;
                }

            // Allocate space for the buffer.
            l_buffer.resize( l_size+1, 0 );

            // Read the data.
            if( ! WinHttpReadData(
                        ah_request,
                        (LPVOID)&l_buffer[0],
                        l_size,
                        &l_n_downloaded ) )
                {
                l_result = FALSE;
                break;
                };

            l_buffer.at( l_n_downloaded ) = '\0';

            *ap_response_xml += &l_buffer[0];

            } while( l_size > 0 );
        }

    return l_result;
}

int c_winhttp::soap_rpc(
            const std::string &ar_path,
            const tlmx_uri_string &ar_soap_action,
            const std::string &ar_request_xml,
            std::string *ap_response_xml )
{
    // WinHttpOpenRequest will add a / at the start of the path name if required.

    if( ! h_session )
        return http_error_code;

    http_error_code = 0;
    ap_response_xml->erase();

    BOOL l_result = FALSE;
    HINTERNET lh_request = NULL;

    // The error code for any errors occurring below is retrieved at the end of the routine.
    // -See Error_collect

    // Create an HTTP request handle.
    if( h_connect )
        {
        std::wstring l_wpath;
        lh_request = WinHttpOpenRequest(
                        h_connect,
                        L"POST",
                        convert( &l_wpath, ar_path ).c_str(),
                        NULL,               // pwszVersion (HTTP version)
                        WINHTTP_NO_REFERER,
                        WINHTTP_DEFAULT_ACCEPT_TYPES,
                        is_secure ? WINHTTP_FLAG_SECURE : 0 );  // dwFlags
        }

    // Add SOAP specific headers
    if( lh_request )
        l_result = WinHttpAddRequestHeaders(
                        lh_request,
                        L"Content-Type: text/xml; charset=\"utf-8\"",
                        static_cast<DWORD>(-1L),    // dwHeadersLength,
                        WINHTTP_ADDREQ_FLAG_ADD );  // dwModifiers

    if( l_result )
        {
        std::wstring l_soap_header( L"SOAPAction: " );
        std::wstring l_wsoap_action;
        l_soap_header += convert( &l_wsoap_action, ar_soap_action );
        l_result = WinHttpAddRequestHeaders(
                        lh_request,
                        l_soap_header.c_str(),
                        (DWORD)l_soap_header.size(),    // dwHeadersLength,
                        WINHTTP_ADDREQ_FLAG_ADD );      // dwModifiers
        }

    // Send a request.
    if( l_result )
        l_result = WinHttpSendRequest(
                        lh_request,
                        WINHTTP_NO_ADDITIONAL_HEADERS,  // pwszHeaders
                        0,                              // dwHeadersLength
                        (void *)ar_request_xml.c_str(), // lpOptional
                        (DWORD)ar_request_xml.size(),   // dwOptionalLength
                        (DWORD)ar_request_xml.size(),   // dwTotalLength
                        0 );                            // dwContext

    if( l_result )
        l_result = p_get_reponse( lh_request, ap_response_xml );

    // Collect any error code
    // Note Error_collect: This will capture any error code that is due to any errors above.
    if( ! l_result && http_error_code == 0 )
        http_error_code = GetLastError();

    // Close request handles.
    if( lh_request )
        WinHttpCloseHandle( lh_request );

    return http_error_code;
}

s_winhttp_error c_winhttp::soap_rpc(
                        const std::string &ar_path,
                        const c_soap_base &ar_request,
                        c_soap_base *ap_response )
{
    s_winhttp_error l_error;

    std::string l_request_xml;

    l_error.phase = s_winhttp_error::E_MARSHAL;
    l_error.lmx_error = ar_request.marshal( &l_request_xml );

    if( l_error.lmx_error != ELMX_OK )
        return l_error;

    std::string l_response_xml;
    l_error.phase = s_winhttp_error::E_HTTP;
    l_error.http_error = soap_rpc( ar_path, ar_request.get_soap_action(), l_request_xml, &l_response_xml );

    if( l_error.http_error != 0 )
        return l_error;

    l_error.phase = s_winhttp_error::E_UNMARSHAL;
    l_error.lmx_error = ap_response->unmarshal( l_response_xml );

    if( l_error.lmx_error != ELMX_OK )
        return l_error;

    l_error.phase = s_winhttp_error::E_SUCCESS;

    return l_error;
}

int c_winhttp::rest_query(
            const std::string &ar_path,
            const std::string &ar_query,
            std::string *ap_response_xml )
{
    // WinHttpOpenRequest will add a / at the start of the path name if required.

    if( ! h_session )
        return http_error_code;

    http_error_code = 0;
    ap_response_xml->erase();

    BOOL l_result = FALSE;
    HINTERNET lh_request = NULL;

    // The error code for any errors occurring below is retrieved at the end of the routine.
    // -See Error_collect

    // Create an HTTP request handle.
    if( h_connect )
        {
        std::string l_path_and_query( ar_path );
        l_path_and_query += "?";
        l_path_and_query += ar_query;
        std::wstring l_wpath_and_query;
        lh_request = WinHttpOpenRequest(
                        h_connect,
                        L"GET",
                        convert( &l_wpath_and_query, l_path_and_query ).c_str(),
                        NULL,               // pwszVersion (HTTP version)
                        WINHTTP_NO_REFERER,
                        WINHTTP_DEFAULT_ACCEPT_TYPES,
                        is_secure ? WINHTTP_FLAG_SECURE : 0 );  // dwFlags
        }

    // Send a request.
    if( lh_request )
        l_result = WinHttpSendRequest(
                        lh_request,
                        WINHTTP_NO_ADDITIONAL_HEADERS,  // pwszHeaders
                        0,                              // dwHeadersLength
                        WINHTTP_NO_REQUEST_DATA,        // lpOptional
                        0,                              // dwOptionalLength
                        0,                              // dwTotalLength
                        0 );                            // dwContext

    if( l_result )
        l_result = p_get_reponse( lh_request, ap_response_xml );

    // Collect any error code
    // Note Error_collect: This will capture any error code that is due to any errors above.
    if( ! l_result && http_error_code == 0 )
        http_error_code = GetLastError();

    // Close request handles.
    if( lh_request )
        WinHttpCloseHandle( lh_request );

    return http_error_code;
}

const char *c_winhttp::get_error_description( int a_error )
{
    switch( a_error )
        {
    case ERROR_WINHTTP_OUT_OF_HANDLES:            return "Out of handles";
    case ERROR_WINHTTP_TIMEOUT:                   return "Timeout";
    case ERROR_WINHTTP_INTERNAL_ERROR:            return "Internal error";
    case ERROR_WINHTTP_INVALID_URL:               return "Invalid url";
    case ERROR_WINHTTP_UNRECOGNIZED_SCHEME:       return "Unrecognized scheme";
    case ERROR_WINHTTP_NAME_NOT_RESOLVED:         return "Name not resolved";
    case ERROR_WINHTTP_INVALID_OPTION:            return "Invalid option";
    case ERROR_WINHTTP_OPTION_NOT_SETTABLE:       return "Option not settable";
    case ERROR_WINHTTP_SHUTDOWN:                  return "Shutdown";
    case ERROR_WINHTTP_LOGIN_FAILURE:             return "Login failure";
    case ERROR_WINHTTP_OPERATION_CANCELLED:       return "Operation cancelled";
    case ERROR_WINHTTP_INCORRECT_HANDLE_TYPE:     return "Incorrect handle type";
    case ERROR_WINHTTP_INCORRECT_HANDLE_STATE:    return "Incorrect handle state";
    case ERROR_WINHTTP_CANNOT_CONNECT:            return "Cannot connect";
    case ERROR_WINHTTP_CONNECTION_ERROR:          return "Connection error";
    case ERROR_WINHTTP_RESEND_REQUEST:            return "Resend request";
    case ERROR_WINHTTP_CLIENT_AUTH_CERT_NEEDED:   return "Client auth certificate needed";
    case ERROR_WINHTTP_CANNOT_CALL_BEFORE_OPEN:   return "Cannot call before open";
    case ERROR_WINHTTP_CANNOT_CALL_BEFORE_SEND:   return "Cannot call before send";
    case ERROR_WINHTTP_CANNOT_CALL_AFTER_SEND:    return "Cannot call after send";
    case ERROR_WINHTTP_CANNOT_CALL_AFTER_OPEN:    return "Cannot call after open";
    case ERROR_WINHTTP_HEADER_NOT_FOUND:          return "Header not found";
    case ERROR_WINHTTP_INVALID_SERVER_RESPONSE:   return "Invalid server response";
    case ERROR_WINHTTP_INVALID_QUERY_REQUEST:     return "Invalid query request";
    case ERROR_WINHTTP_HEADER_ALREADY_EXISTS:     return "Header already exists";
    case ERROR_WINHTTP_REDIRECT_FAILED:           return "Redirect failed";
    case ERROR_WINHTTP_AUTO_PROXY_SERVICE_ERROR:  return "Auto proxy service error";
    case ERROR_WINHTTP_BAD_AUTO_PROXY_SCRIPT:     return "Bad auto proxy script";
    case ERROR_WINHTTP_UNABLE_TO_DOWNLOAD_SCRIPT: return "Unable to download script";
    case ERROR_WINHTTP_NOT_INITIALIZED:           return "Not initialized";
    case ERROR_WINHTTP_SECURE_FAILURE:            return "Secure failure";
    case ERROR_WINHTTP_SECURE_CERT_DATE_INVALID:  return "Secure certificate date invalid";
    case ERROR_WINHTTP_SECURE_CERT_CN_INVALID:    return "Secure certificate CN invalid";
    case ERROR_WINHTTP_SECURE_INVALID_CA:         return "Secure invalid CA";
    case ERROR_WINHTTP_SECURE_CERT_REV_FAILED:    return "Secure certificate rev failed";
    case ERROR_WINHTTP_SECURE_CHANNEL_ERROR:      return "Secure channel error";
    case ERROR_WINHTTP_SECURE_INVALID_CERT:       return "Secure invalid certificate";
    case ERROR_WINHTTP_SECURE_CERT_REVOKED:       return "Secure certificate revoked";
    case ERROR_WINHTTP_SECURE_CERT_WRONG_USAGE:   return "Secure certificate wrong usage";
    case ERROR_WINHTTP_AUTODETECTION_FAILED:      return "Autodetection failed";
    case ERROR_WINHTTP_HEADER_COUNT_EXCEEDED:     return "Header count exceeded";
    case ERROR_WINHTTP_HEADER_SIZE_OVERFLOW:      return "Header size overflow";
    case ERROR_WINHTTP_CHUNKED_ENCODING_HEADER_SIZE_OVERFLOW: return "Chunked encoding header size overflow";
    case ERROR_WINHTTP_RESPONSE_DRAIN_OVERFLOW:   return "Response drain overflow";

    case LMXWINHTTP_ERROR_HTTP_STATUS_NOT_OK:     return "HTTP status not OK";

    case ERROR_NOT_ENOUGH_MEMORY:                 return "Not enough memory";
        }

    return "WinHTTP unknown error";
}

} // End of namespace lmx

//------------------------------------------------------------------------------
//                     Copyright (c) 2008 Codalogic Ltd.
//------------------------------------------------------------------------------
