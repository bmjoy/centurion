#include "lmxuser.h"    /* Outside include guards.  Depending on build, lmxuser.h may include lmxregex.h */

#ifndef LMXREGEX_H
#define LMXREGEX_H

#include <string>

namespace lmx {

/**
 * xmlRegexpPtr:
 *
 * A libxml regular expression, they can actually be far more complex
 * than the POSIX regex expressions.
 */
typedef struct _xmlRegexp xmlRegexp;
typedef xmlRegexp * xmlRegexpPtr;

class c_regex
{
private:
    xmlRegexpPtr mp_regex;

public:
    LMX_RDECL c_regex( const std::string & r_pattern );
    LMX_RDECL bool is_compiled() const
    {
        return mp_regex != LMXNULL;
    }
    LMX_RDECL bool is_match( const std::string & r_s ) const;
    LMX_RDECL virtual ~c_regex();
};

} // End of namepsace lmx

#endif /* LMXREGEX_H */
