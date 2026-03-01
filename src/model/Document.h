#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QString>
#include <QColor>

#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <TopoDS_Shape.hxx>
#include <gp_Trsf.hxx>

#include "../commands/CommandHistory.h"

struct ShapeEntry {
    int id;
    QString name;
    QString type;
    QMap<QString, double> params;
    QColor color = QColor(180, 180, 220);
    Handle(AIS_Shape) aisShape;
    TopoDS_Shape topoShape;
};

enum class BooleanType { Fuse, Cut, Common };

class Document : public QObject
{
    Q_OBJECT

public:
    explicit Document(Handle(AIS_InteractiveContext) context, QObject* parent = nullptr);
    ~Document() override;

    // Shape management
    int addShape(const QString& type, const QMap<QString, double>& params,
                 const TopoDS_Shape& shape);
    int addImportedShape(const QString& name, const TopoDS_Shape& shape);
    void removeShape(int id);
    void clearAll();

    // Queries
    const QList<ShapeEntry>& shapes() const { return m_shapes; }
    ShapeEntry* findShape(int id);
    const ShapeEntry* findShape(int id) const;
    int selectedShapeId() const;
    QList<int> selectedShapeIds() const;

    // Transformations
    void translateShape(int id, double dx, double dy, double dz);
    void rotateShape(int id, int axisIndex, double angleDeg);
    void scaleShape(int id, double factor);

    // Boolean operations
    int booleanOperation(int id1, int id2, BooleanType type);

    // Parametric editing
    void updateShapeParams(int id, const QMap<QString, double>& newParams);
    void setShapeColor(int id, const QColor& color);
    void renameShape(int id, const QString& newName);
    static TopoDS_Shape rebuildShape(const QString& type, const QMap<QString, double>& params);

    // Fillet / Chamfer
    int filletShape(int id, double radius);
    int chamferShape(int id, double distance);

    // Undo / Redo
    void undo();
    void redo();
    bool canUndo() const;
    bool canRedo() const;
    QString undoDescription() const;
    QString redoDescription() const;

    // Selection sync from 3D viewer
    void syncSelectionFromViewer();

    // I/O
    void importFile(const QString& path);
    void exportFile(const QString& path);

    Handle(AIS_InteractiveContext) context() const { return m_context; }

signals:
    void shapeAdded(int id);
    void shapeRemoved(int id);
    void selectionChanged(int id);
    void modelChanged();
    void undoRedoChanged();

private:
    Snapshot makeSnapshot(const QString& description) const;
    void saveSnapshot(const QString& description);
    void restoreSnapshot(const Snapshot& snapshot);

    TopoDS_Shape buildCompound() const;
    QString generateName(const QString& type);

    Handle(AIS_InteractiveContext) m_context;
    QList<ShapeEntry> m_shapes;
    int m_nextId = 1;
    QMap<QString, int> m_nameCounters;
    CommandHistory m_history;
};

#endif // DOCUMENT_H
