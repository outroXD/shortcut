#pragma once

#include <exception>
#include <QString>
#include <QByteArray>
#include "shortcut_global.h"


class SHORTCUT_EXPORT ShortcutException : public std::exception
{
public:
    ShortcutException(const QString& message) throw();
    const char* what() const throw();
    ~ShortcutException() throw ();

private:
    QByteArray Message;
};
