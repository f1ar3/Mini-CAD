#include "PrimitiveDialog.h"

#include <QFormLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QLabel>

PrimitiveDialog::PrimitiveDialog(const QString& type, QWidget* parent)
    : QDialog(parent)
{
    QString typeName;
    if (type == "Box") typeName = tr("Параллелепипед");
    else if (type == "Cylinder") typeName = tr("Цилиндр");
    else if (type == "Sphere") typeName = tr("Сфера");
    else if (type == "Cone") typeName = tr("Конус");
    else if (type == "Torus") typeName = tr("Тор");
    else if (type == "Wedge") typeName = tr("Клин");
    else typeName = type;

    setWindowTitle(tr("Создание: %1").arg(typeName));

    auto* mainLayout = new QVBoxLayout(this);
    m_layout = new QFormLayout;
    mainLayout->addLayout(m_layout);

    setupFields(type);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttons);
}

void PrimitiveDialog::setupFields(const QString& type)
{
    if (type == "Box") {
        addField("Width (X)", tr("Ширина (X)"), 100.0);
        addField("Depth (Y)", tr("Глубина (Y)"), 60.0);
        addField("Height (Z)", tr("Высота (Z)"), 40.0);
    } else if (type == "Cylinder") {
        addField("Radius", tr("Радиус"), 30.0);
        addField("Height", tr("Высота"), 80.0);
    } else if (type == "Sphere") {
        addField("Radius", tr("Радиус"), 50.0);
    } else if (type == "Cone") {
        addField("Bottom Radius", tr("Нижний радиус"), 40.0);
        addField("Top Radius", tr("Верхний радиус"), 10.0);
        addField("Height", tr("Высота"), 70.0);
    } else if (type == "Torus") {
        addField("Major Radius", tr("Большой радиус"), 50.0);
        addField("Minor Radius", tr("Малый радиус"), 15.0);
    } else if (type == "Wedge") {
        addField("Width (X)", tr("Ширина (X)"), 100.0);
        addField("Depth (Y)", tr("Глубина (Y)"), 60.0);
        addField("Height (Z)", tr("Высота (Z)"), 40.0);
        addField("Top Width (Xmin)", tr("Верхняя ширина"), 20.0);
    }
}

void PrimitiveDialog::addField(const QString& key, const QString& label,
                                double defaultValue, double minVal, double maxVal)
{
    auto* spin = new QDoubleSpinBox(this);
    spin->setRange(minVal, maxVal);
    spin->setValue(defaultValue);
    spin->setDecimals(2);
    spin->setSuffix(tr(" мм"));
    m_layout->addRow(label + ":", spin);
    m_fields[key] = spin;
}

QMap<QString, double> PrimitiveDialog::parameters() const
{
    QMap<QString, double> params;
    for (auto it = m_fields.constBegin(); it != m_fields.constEnd(); ++it) {
        params[it.key()] = it.value()->value();
    }
    return params;
}

QMap<QString, double> PrimitiveDialog::getParameters(const QString& type, QWidget* parent)
{
    PrimitiveDialog dlg(type, parent);
    if (dlg.exec() == QDialog::Accepted) {
        return dlg.parameters();
    }
    return {};
}
