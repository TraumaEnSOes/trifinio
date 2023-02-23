#ifndef LIBTRI_DETAILS_TRI_ASSERT_HPP
#define LIBTRI_DETAILS_TRI_ASSERT_HPP

#ifdef NDEBUG
#include <sstream>
#include <stdexcept>

namespace tri {

namespace details {

class AssertionFailure : public std::runtime_error {
};

inline void Assert( bool cond, std::string expr, const char *file, const char *func, int line ) {
    if( !c ) {
        std::stringstream ss;
        ss << "assertion " << expr << " in " << file << ':' << line << " (" << func << ") failed";
        throw AssertionFailure( ss.str( ) );
    }
}

} // namespace details.

} // namespace tri.

#define triassert( x ) ::tri::details::Assert( (x), #x, __FILE__, __func__, __LINE__ )
#define LIBTRI_NOEXCEPT
#else
#define triassert( x ) (void)0
#define LIBTRI_NOEXCEPT noexcept
#endif

#endif
