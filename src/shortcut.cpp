#include <QApplication>
#include <QWindow>
#include <qpa/qplatformnativeinterface.h>
#include "shortcut.h"
#include "shortcut_hotkeymap.h"
#include "shortcut_global.h"


/*****************************************************************************/
Shortcut::Shortcut(QWidget *parent) : QWidget(parent)
{
    qApp->installNativeEventFilter(this);
    QWindow window;
    void* v = qApp->platformNativeInterface()->nativeResourceForWindow("connection", &window);
    xcbConnection = (xcb_connection_t*)v;
    xcbWindow = xcb_setup_roots_iterator(xcb_get_setup(xcbConnection)).data->root;
    xcbKeySymbols = xcb_key_symbols_alloc(xcbConnection);
}


/*****************************************************************************/
void Shortcut::registKey(const QString& keySeq, size_t id)
{
    registKey(ShortcutKeySequence(keySeq), id);
}


/*****************************************************************************/
void Shortcut::registKey(const ShortcutKeySequence& keySeq, size_t id)
{
    if (keySeq.Size() == 0) {
        throw ShortcutException("");  // TODO
    }
    if (registeredKey.find(id) != registeredKey.end()) {
        unregistKey(id);
    }
    registHotKey(keySeq, id);
}


/*****************************************************************************/
void Shortcut::unregistKey(size_t id)
{
    Q_ASSERT(registeredKey.find(id) != registeredKey.end() && "");  // TODO
    unregistLinuxHotkey(id);
    registeredKey.remove(id);
}


/*****************************************************************************/
void Shortcut::unregisterAllHotkeys()
{
    for (size_t id :registeredKey.keys()) {
        this->unregistKey(id);
    }
}


/*****************************************************************************/
Shortcut::~Shortcut()
{
    xcb_key_symbols_free(xcbKeySymbols);
}


/*****************************************************************************/
bool Shortcut::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType);
    Q_UNUSED(result);
    return event(static_cast<xcb_generic_event_t*>(message));
}


/*****************************************************************************/
bool Shortcut::event(xcb_generic_event_t *message)
{  // manage event here
    switch (message->response_type & ~0x80) {
        case XCB_KEY_PRESS: {
            xcb_key_press_event_t *pXcbKeyPressEvent = (xcb_key_press_event_t*)message;
            auto ind = registeredKey.key({pXcbKeyPressEvent->detail, (pXcbKeyPressEvent->state & ~XCB_MOD_MASK_2)});
            if (ind == 0) {
                return false;
            }
            emit activated(ind);
            return true;
        }
    }
    return false;
}


/*****************************************************************************/
void Shortcut::registHotKey(const ShortcutKeySequence &keySeq, size_t id)
{
    ShortcutHotkeyData data;
    ShortcutKeyData keyData = qtKeyToLinux(keySeq);

    xcb_keycode_t *keycode = xcb_key_symbols_get_keycode(xcbKeySymbols, keyData.key);

    data.keyCode = *keycode;
    data.mods = keyData.mods;

    xcb_grab_key(xcbConnection,
                 1,
                 xcbWindow,
                 data.mods,
                 data.keyCode,
                 XCB_GRAB_MODE_ASYNC,
                 XCB_GRAB_MODE_ASYNC);

    xcb_grab_key(xcbConnection,
                 1,
                 xcbWindow,
            data.mods | XCB_MOD_MASK_2,
                 data.keyCode,
                 XCB_GRAB_MODE_ASYNC,
                 XCB_GRAB_MODE_ASYNC);

    registeredKey.insert(id, data);
}


/*****************************************************************************/
void Shortcut::unregistLinuxHotkey(size_t id)
{
    ShortcutHotkeyData data = registeredKey.take(id);
    xcb_ungrab_key(xcbConnection, data.keyCode, xcbWindow, data.mods);
    xcb_ungrab_key(xcbConnection, data.keyCode, xcbWindow, data.mods | XCB_MOD_MASK_2);
}
