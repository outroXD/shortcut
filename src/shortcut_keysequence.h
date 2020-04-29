#pragma once

#include <QObject>
#include <QString>
#include <QVector>
#include <QStringList>
#include <QKeyEvent>
#include "shortcut_exception.h"
#include "shortcut_global.h"


class SHORTCUT_EXPORT ShortcutKeySequence : public QObject
{
    Q_OBJECT

public:
    explicit ShortcutKeySequence(QObject *parent=0);
    explicit ShortcutKeySequence(const QString& str, QObject *parent=0);
    void fromString(const QString& str);
    QString toString();
    void addKey(int key);
    void addKey(const QString& key);
    void addModifiers(Qt::KeyboardModifiers mod);
    void addKey(const QKeyEvent* event);
    inline size_t Size() const {
        return keys.size();
    }
    inline int operator [] (size_t n) const {
        if ((int)n > keys.size()) {
            throw ShortcutException("");  // TODO
        }
        return keys[n];
    }
    QVector<int> getSimpleKeys() const;
    QVector<int> getModifiers() const;

private:
    QVector<int> keys;
};
