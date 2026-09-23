/*
 * QT HEADER GUARD END
 *
 * Restores Qt's `emit` macro if it was defined when
 * qt_header_guard_start.inl was included.
 *
 * See qt_header_guard_start.inl for details.
 */

#ifndef ABY_QT_HEADER_GUARD_START
#	error "this file should not be included unless 'qt_header_guard_start.inl' was included"
#endif

#ifdef ABY_QT_SAVED_EMIT
#	define emit ABY_QT_SAVED_EMIT
#	undef ABY_QT_SAVED_EMIT
#endif

#undef ABY_QT_HEADER_GUARD_START
