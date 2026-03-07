#include "MeasureDialog.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QGroupBox>

#include <BRepGProp.hxx>
#include <GProp_GProps.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>

MeasureDialog::MeasureDialog(const TopoDS_Shape& shape, const QString& name, QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Измерения: %1").arg(name));
    setMinimumWidth(320);

    auto* layout = new QVBoxLayout(this);
    auto* form = new QFormLayout();

    auto makeField = [this](const QString& text) {
        auto* edit = new QLineEdit(text, this);
        edit->setReadOnly(true);
        return edit;
    };

    // Volume
    GProp_GProps volumeProps;
    BRepGProp::VolumeProperties(shape, volumeProps);
    double volume = volumeProps.Mass();
    form->addRow(tr("Объём:"), makeField(QString::number(volume, 'f', 2) + tr(" мм³")));

    // Surface area
    GProp_GProps surfaceProps;
    BRepGProp::SurfaceProperties(shape, surfaceProps);
    double area = surfaceProps.Mass();
    form->addRow(tr("Площадь:"), makeField(QString::number(area, 'f', 2) + tr(" мм²")));

    // Bounding box
    Bnd_Box bbox;
    BRepBndLib::Add(shape, bbox);
    if (!bbox.IsVoid()) {
        double xmin, ymin, zmin, xmax, ymax, zmax;
        bbox.Get(xmin, ymin, zmin, xmax, ymax, zmax);

        double dx = xmax - xmin;
        double dy = ymax - ymin;
        double dz = zmax - zmin;

        form->addRow(tr("Габариты X:"), makeField(QString::number(dx, 'f', 2) + tr(" мм")));
        form->addRow(tr("Габариты Y:"), makeField(QString::number(dy, 'f', 2) + tr(" мм")));
        form->addRow(tr("Габариты Z:"), makeField(QString::number(dz, 'f', 2) + tr(" мм")));
    }

    layout->addLayout(form);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    layout->addWidget(buttons);
}

void MeasureDialog::show(const TopoDS_Shape& shape, const QString& name, QWidget* parent)
{
    MeasureDialog dlg(shape, name, parent);
    dlg.exec();
}
