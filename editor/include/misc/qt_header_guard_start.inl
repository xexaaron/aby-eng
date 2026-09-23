/*
 * QT HEADER GUARD START
 *
 * Qt defines `emit` as a macro. Intel oneTBB also uses `emit` as an
 * identifier in its event-related code, which causes a macro collision
 * when <aby-eng/log.hpp> includes oneTBB.
 *
 * Any file that includes <aby-eng/log.hpp> while Qt is in scope should
 * include this file instead, followed by qt_header_guard_end.inl.
 */

#ifdef ABY_QT_HEADER_GUARD_START
#	error "'qt_header_guard_start.inl' was included without a matching 'qt_header_guard_end.inl'"
#else
#	define ABY_QT_HEADER_GUARD_START
#endif

#ifdef emit
#	define ABY_QT_SAVED_EMIT emit
#	undef emit
#	include <aby-eng/log.hpp>
#else
#	include <aby-eng/log.hpp>
#endif
