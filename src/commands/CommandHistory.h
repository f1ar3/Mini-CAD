#ifndef COMMANDHISTORY_H
#define COMMANDHISTORY_H

#include <QList>
#include <QMap>
#include <QString>

#include <TopoDS_Shape.hxx>

struct Snapshot {
    QString description;

    struct ShapeData {
        int id;
        QString name;
        QString type;
        QMap<QString, double> params;
        TopoDS_Shape topoShape;
    };

    QList<ShapeData> shapes;
    int nextId = 1;
    QMap<QString, int> nameCounters;
};

class CommandHistory
{
public:
    void pushUndo(const Snapshot& snapshot);
    void pushRedo(const Snapshot& snapshot);

    bool canUndo() const;
    bool canRedo() const;

    Snapshot popUndo();
    Snapshot popRedo();

    void clearRedo();
    void clearAll();

    QString undoDescription() const;
    QString redoDescription() const;

private:
    static const int MAX_HISTORY = 50;

    QList<Snapshot> m_undoStack;
    QList<Snapshot> m_redoStack;
};

#endif // COMMANDHISTORY_H
