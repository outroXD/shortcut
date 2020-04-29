#ifndef SHORTCUT_GLOBAL_H
#define SHORTCUT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SHORTCUT_LIBRARY)
#  define SHORTCUT_EXPORT Q_DECL_EXPORT
#else
#  define SHORTCUT_EXPORT Q_DECL_IMPORT
#endif

#endif // SHORTCUT_GLOBAL_H
