#pragma once

#include <QWidget>
#include <QAbstractNativeEventFilter>
#include <QSet>
#include "xcb/xcb.h"
#include "xcb/xcb_keysyms.h"
#include "shortcut_keysequence.h"
#include "shortcut_exception.h"
#include "shortcut_global.h"


/*****************************************************************************/
struct ShortcutHotkeyData {
    xcb_keycode_t keyCode;
    int mods;
    bool operator == (const ShortcutHotkeyData& data) const {
        return data.keyCode == this->keyCode && data.mods == this->mods;
    }
};


/*****************************************************************************/
class SHORTCUT_EXPORT Shortcut : public QWidget, public QAbstractNativeEventFilter
{
    Q_OBJECT

public:
    explicit Shortcut(QWidget *parent=0);
    void registKey(const QString& keySeq, size_t id=1);
    void registKey(const ShortcutKeySequence& keySeq, size_t id=1);
    void unregistKey(size_t id = 1);
    void unregisterAllHotkeys();
    ~Shortcut();

protected:
    bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);
    bool event(xcb_generic_event_t *message);
    void registHotKey(const ShortcutKeySequence& keySeq, size_t id);
    void unregistLinuxHotkey(size_t id);

signals:
    void activated(size_t id);

private:
    QHash<size_t, ShortcutHotkeyData> registeredKey;
    xcb_connection_t* xcbConnection;
    xcb_window_t xcbWindow;
    xcb_key_symbols_t* xcbKeySymbols;
};
