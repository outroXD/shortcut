#include "shortcut_keysequence.h"
#include <QDebug>


/*****************************************************************************/
ShortcutKeySequence::ShortcutKeySequence(QObject *parent) : QObject(parent){}


/*****************************************************************************/
ShortcutKeySequence::ShortcutKeySequence(const QString& str, QObject *parent) : QObject(parent)
{
    fromString(str);
}


/*****************************************************************************/
bool isModifier(int key)
{
    return (key == Qt::Key_Shift || key == Qt::Key_Control || key == Qt::Key_Alt || key == Qt::Key_Meta);
}


/*****************************************************************************/
static QString keyToStr(int key)
{
    if (key == Qt::Key_Shift) {
        return "Shift";
    }
    if (key == Qt::Key_Control) {
        return "Ctrl";
    }
    if (key == Qt::Key_Alt) {
        return "Alt";
    }
    if (key == Qt::Key_Meta) {
        return "Meta";
    }
    QKeySequence seq(key);
    return seq.toString();
}


/*****************************************************************************/
void ShortcutKeySequence::fromString(const QString& str)
{
    QStringList keys = str.split('+');
    for (int i = 0; i < keys.size(); i++) {
        addKey(keys[i]);
    }
}


/*****************************************************************************/
QString ShortcutKeySequence::toString()
{
    QVector<int> simpleKeys = getSimpleKeys();
    QVector<int> modifiers = getModifiers();
    QStringList result;
    for (int i = 0; i < modifiers.size(); i++) {
        result.push_back(keyToStr(modifiers[i]));
    }
    for (int i = 0; i < simpleKeys.size(); i++) {
        result.push_back(keyToStr(simpleKeys[i]));
    }
    return result.join('+');
}


/*****************************************************************************/
QVector<int> ShortcutKeySequence::getSimpleKeys() const
{
    QVector<int> result;
    for (int i = 0; i < keys.size(); i++) {
        if (!isModifier(keys[i])) {
            result.push_back(keys[i]);
        }
    }
    return result;
}


/*****************************************************************************/
QVector<int> ShortcutKeySequence::getModifiers() const
{
    QVector<int> result;
    for (int i = 0; i < keys.size(); i++) {
        if (isModifier(keys[i])) {
            result.push_back(keys[i]);
        }
    }
    return result;
}


/*****************************************************************************/
void ShortcutKeySequence::addModifiers(Qt::KeyboardModifiers mod)
{
    if (mod == Qt::NoModifier) {
        return;
    }
    if (mod & Qt::ShiftModifier) {
        addKey(Qt::Key_Shift);
    }
    if (mod & Qt::ControlModifier) {
        addKey(Qt::Key_Control);
    }
    if (mod & Qt::AltModifier) {
        addKey(Qt::Key_Alt);
    }
    if (mod & Qt::MetaModifier) {
        addKey(Qt::Key_Meta);
    }
}


/*****************************************************************************/
void ShortcutKeySequence::addKey(const QString& key)
{
    if (key.contains("+") || key.contains(",")) {
        throw ShortcutException("");  // TODO
    }

    QString mod = key.toLower();
    if (mod == "alt") {
        addKey(Qt::Key_Alt);
        return;
    }
    if (mod == "shift" || mod == "shft") {
        addKey(Qt::Key_Shift);
        return;
    }
    if (mod == "control" || mod == "ctrl") {
        addKey(Qt::Key_Control);
        return;
    }
    if (mod == "win" || mod == "meta") {
        addKey(Qt::Key_Meta);
        return;
    }
    QKeySequence qKeySequence(key);
    if (qKeySequence.count() != 1) {
        throw ShortcutException("");  // TODO
    }
    addKey(qKeySequence[0]);
}


/*****************************************************************************/
void ShortcutKeySequence::addKey(int key)
{
    if (key <= 0) {
        return;
    }
    for (int i = 0; i < keys.size(); i++) {
        if (keys[i] == key) {
            return;
        }
    }
    keys.push_back(key);
}


/*****************************************************************************/
void ShortcutKeySequence::addKey(const QKeyEvent* event)
{
    addKey(event->key());
    addModifiers(event->modifiers());
}
