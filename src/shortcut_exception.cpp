#include "shortcut_exception.h"

ShortcutException::ShortcutException(const QString& message) throw() : Message(message.toLocal8Bit()){}
ShortcutException::~ShortcutException() throw () {}
const char* ShortcutException::what() const throw ()
{
    return Message.data();
}
