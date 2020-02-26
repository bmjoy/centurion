//------------------------------------------------------------------------------
//                     Copyright (c) 2014 Codalogic Ltd.
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
// Version: 6.5
//      - First inclusion in LMX.
//------------------------------------------------------------------------------

#include "lmxparse.h"

//------------------------------------------------------------------------------
//                   class c_external_transcoder_ascii_extended
//                 A number of encodings are extensions of ASCII
//------------------------------------------------------------------------------

class c_external_transcoder_ascii_extended : public lmx::c_external_transcoder
{
private:
    const char * const * mpp_extended_table;

public:
    c_external_transcoder_ascii_extended(
                    lmx::c_read & r_reader_in,
                    lmx::c_error & r_error_in,
                    const char * const cp_extended_table_in[] );

    virtual lmx::c_external_transcoder::s_result read();
};

//------------------------------------------------------------------------------
//                   class c_external_transcoder_win_1252
//                  Windows-1252 is an extension of ASCII
//------------------------------------------------------------------------------

class c_external_transcoder_win_1252 : public c_external_transcoder_ascii_extended
{
public:
    c_external_transcoder_win_1252(
                    lmx::c_read & r_reader_in,
                    lmx::c_error & r_error_in );
};

//------------------------------------------------------------------------------
//                   class c_external_transcoder_factory_example
//                     Factory class for selecting transcoders
//------------------------------------------------------------------------------

class c_external_transcoder_factory_example : public lmx::c_external_transcoder_factory
{
public:
    virtual lmx::c_external_transcoder * get_transcoder(
                        lmx::c_read & r_reader_in,
                        lmx::c_error & r_error_in,
                        lmx::c_get_as_utf8::e_encoding_mode initial_encoding_mode_in,
                        const char * p_encoding_name_in ) const;
};

//------------------------------------------------------------------------------
//                                 Prototypes
//------------------------------------------------------------------------------

void lmx_external_transcoder_setup_example();

//------------------------------------------------------------------------------
//                     Copyright (c) 2014 Codalogic Ltd.
//------------------------------------------------------------------------------
