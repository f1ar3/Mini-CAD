#ifndef MEASUREDIALOG_H
#define MEASUREDIALOG_H

#include <QDialog>
#include <TopoDS_Shape.hxx>

class MeasureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MeasureDialog(const TopoDS_Shape& shape, const QString& name, QWidget* parent = nullptr);

    static void show(const TopoDS_Shape& shape, const QString& name, QWidget* parent);
};

#endif // MEASUREDIALOG_H
