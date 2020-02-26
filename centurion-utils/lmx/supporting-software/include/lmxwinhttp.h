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

#ifndef LMXWINHTTP_H
#define LMXWINHTTP_H

#include <string>

#include "lmxuser.h"

// Because WinHTTP is only supported on a limited number of platforms (i.e. post
// Windows Server 2003, Windows XP SP1, and Windows 2000 Professional SP3)
// it is not built into the default LMX DLLs.
#ifndef LMX_HDECL
#define LMX_HDECL
#endif

namespace lmx {

// Getting an HTTP status code like 404 is not considered a regular error in WinHTTP!
#define LMXWINHTTP_ERROR_HTTP_STATUS_NOT_OK -1000

class c_soap_base;

struct s_winhttp_error
{
    enum { E_INIT, E_MARSHAL, E_HTTP, E_UNMARSHAL, E_SUCCESS } phase;
    elmx_error lmx_error;
    int http_error;

    s_winhttp_error() : phase( E_INIT ), lmx_error( ELMX_OK ), http_error( 0 )
    {}
    bool is_ok() const { return phase == E_SUCCESS; }
};

class c_winhttp
{
public:
    static const size_t k_default_port;
    static const size_t k_default_http_port;
    static const size_t k_default_https_port;

private:
    // For a URI of http://example.com/service host=example.com and path=/service
    std::string host;
    size_t port;
    bool is_secure;
    int http_error_code;
    void *h_session;
    void *h_connect;

private:
    int p_get_reponse( void *ah_request, std::string *ap_response_xml );

public:
    LMX_HDECL c_winhttp( const std::string &ar_host,
                            size_t a_port = k_default_port,
                            bool a_is_secure = false );
    LMX_HDECL virtual ~c_winhttp();
    LMX_HDECL int soap_rpc( const std::string &ar_path,
                        const tlmx_uri_string &ar_soap_action,
                        const std::string &ar_request_xml,
                        std::string *ap_response_xml );
    LMX_HDECL s_winhttp_error soap_rpc(
                        const std::string &ar_path,
                        const c_soap_base &ar_request,
                        c_soap_base *ap_response );
    LMX_HDECL int rest_query(
                        const std::string &ar_path,
                        const std::string &ar_query,
                        std::string *ap_response_xml );
    LMX_HDECL int get_http_error_code() const { return http_error_code; }

    LMX_HDECL static const char *get_error_description( int a_error );

};

class c_secure_winhttp : public c_winhttp
{
public:
    LMX_HDECL c_secure_winhttp(
                    const std::string &ar_host,
                    size_t a_port = c_winhttp::k_default_https_port )
                    :
                    c_winhttp( ar_host, a_port, true )
    {}
};

// Include lmxsoap.h prior to including lmxwinhttp.h to enable this functionality
#ifdef LMX_SOAP_DEFINED
template< class Tin, class Tout>
s_winhttp_error simple_soap(
                    const std::string &ar_host,
                    size_t a_port,
                    const std::string &ar_path,
                    const tlmx_uri_string &ar_soap_action,
                    Tin *ap_in,
                    Tout **app_out = LMXNULL,   // Allows an empty SOAP body in the reply
                    bool a_is_secure = false )
{
    c_soap< Tin > l_soap_in;
    l_soap_in.do_delete_on_destruct( false );
    l_soap_in.bodies().push_back( ap_in );
    l_soap_in.set_soap_action( ar_soap_action );

    c_soap< Tout > l_soap_out;

    if( app_out )
        {
        *app_out = LMXNULL;
        l_soap_out.do_delete_on_destruct( false );  // Caller will own the result
        }

    c_winhttp l_winhttp( ar_host, a_port, a_is_secure );
    s_winhttp_error l_result = l_winhttp.soap_rpc( ar_path, l_soap_in, &l_soap_out );

    if( l_result.is_ok() && ! l_soap_out.bodies().empty() && app_out )
        *app_out = l_soap_out.bodies()[0];

    return l_result;
}

template< class Tin, class Tout>
s_winhttp_error simple_soap(
                    const std::string &ar_host,
                    const std::string &ar_path,
                    const tlmx_uri_string &ar_soap_action,
                    Tin *ap_in,
                    Tout **app_out = LMXNULL,   // Allows an empty SOAP body in the reply
                    bool a_is_secure = false )
{
    return simple_soap( ar_host, c_winhttp::k_default_port, ar_path,
                        ar_soap_action, ap_in, app_out,
                        a_is_secure );
}
#endif

} // End of namespace lmx

#endif // LMXWINHTTP_H

//------------------------------------------------------------------------------
//                     Copyright (c) 2008 Codalogic Ltd.
//------------------------------------------------------------------------------
