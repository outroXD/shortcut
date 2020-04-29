#include <QtCore>
#include "shortcut_keysequence.h"
#include "xcb/xcb.h"
#include "xcb/xcb_keysyms.h"
#include "X11/keysym.h"


/*****************************************************************************/
struct ShortcutKeyData {
    int key;
    int mods;
};


/*****************************************************************************/
inline ShortcutKeyData qtKeyToLinux(const ShortcutKeySequence &keySeq)
{
    ShortcutKeyData data = {0, 0};

    auto key = keySeq.getSimpleKeys();
    if (key.size() > 0) {
        data.key = key[0];
    } else {
        throw ShortcutException("");  // TODO
    }

    if (data.key >= Qt::Key_F1 && data.key <= Qt::Key_F35) {
        const size_t DIFF = Qt::Key_F1 - XK_F1;
        data.key -= DIFF;
    } else if (data.key >= Qt::Key_Space && data.key <= Qt::Key_QuoteLeft) {
    } else {
        throw ShortcutException("");  // TODO
    }

    auto mods = keySeq.getModifiers();
    for (auto i : mods) {
        if (i == Qt::Key_Shift) {
            data.mods |= XCB_MOD_MASK_SHIFT;
        } else if (i == Qt::Key_Control) {
            data.mods |= XCB_MOD_MASK_CONTROL;
        } else if (i == Qt::Key_Alt) {
            data.mods |= XCB_MOD_MASK_1;
        } else if (i == Qt::Key_Meta) {
            data.mods |= XCB_MOD_MASK_4;
        }
    }

    return data;
}
