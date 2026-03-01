#include "TransformDialog.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QComboBox>

TransformDialog::TransformDialog(Mode mode, QWidget* parent)
    : QDialog(parent)
    , m_mode(mode)
{
    auto* mainLayout = new QVBoxLayout(this);
    auto* form = new QFormLayout;
    mainLayout->addLayout(form);

    switch (mode) {
    case Mode::Translate:
        setWindowTitle(tr("Перемещение фигуры"));
        m_spinX = new QDoubleSpinBox(this);
        m_spinY = new QDoubleSpinBox(this);
        m_spinZ = new QDoubleSpinBox(this);
        for (auto* s : {m_spinX, m_spinY, m_spinZ}) {
            s->setRange(-10000, 10000);
            s->setValue(0);
            s->setDecimals(2);
            s->setSuffix(tr(" мм"));
        }
        form->addRow(tr("X:"), m_spinX);
        form->addRow(tr("Y:"), m_spinY);
        form->addRow(tr("Z:"), m_spinZ);
        break;

    case Mode::Rotate:
        setWindowTitle(tr("Вращение фигуры"));
        m_axisCombo = new QComboBox(this);
        m_axisCombo->addItems({"X", "Y", "Z"});
        m_angleSpin = new QDoubleSpinBox(this);
        m_angleSpin->setRange(-360, 360);
        m_angleSpin->setValue(45);
        m_angleSpin->setDecimals(1);
        m_angleSpin->setSuffix(QString::fromUtf8("\u00B0"));
        form->addRow(tr("Ось:"), m_axisCombo);
        form->addRow(tr("Угол:"), m_angleSpin);
        break;

    case Mode::Scale:
        setWindowTitle(tr("Масштабирование фигуры"));
        m_scaleSpin = new QDoubleSpinBox(this);
        m_scaleSpin->setRange(0.01, 100.0);
        m_scaleSpin->setValue(1.0);
        m_scaleSpin->setDecimals(3);
        m_scaleSpin->setSingleStep(0.1);
        form->addRow(tr("Коэффициент:"), m_scaleSpin);
        break;
    }

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttons);
}

double TransformDialog::dx() const { return m_spinX ? m_spinX->value() : 0; }
double TransformDialog::dy() const { return m_spinY ? m_spinY->value() : 0; }
double TransformDialog::dz() const { return m_spinZ ? m_spinZ->value() : 0; }

int TransformDialog::axisIndex() const { return m_axisCombo ? m_axisCombo->currentIndex() : 0; }
double TransformDialog::angleDeg() const { return m_angleSpin ? m_angleSpin->value() : 0; }
double TransformDialog::scaleFactor() const { return m_scaleSpin ? m_scaleSpin->value() : 1.0; }

bool TransformDialog::getTranslation(double& dx, double& dy, double& dz, QWidget* parent)
{
    TransformDialog dlg(Mode::Translate, parent);
    if (dlg.exec() == QDialog::Accepted) {
        dx = dlg.dx();
        dy = dlg.dy();
        dz = dlg.dz();
        return true;
    }
    return false;
}

bool TransformDialog::getRotation(int& axis, double& angle, QWidget* parent)
{
    TransformDialog dlg(Mode::Rotate, parent);
    if (dlg.exec() == QDialog::Accepted) {
        axis = dlg.axisIndex();
        angle = dlg.angleDeg();
        return true;
    }
    return false;
}

bool TransformDialog::getScale(double& factor, QWidget* parent)
{
    TransformDialog dlg(Mode::Scale, parent);
    if (dlg.exec() == QDialog::Accepted) {
        factor = dlg.scaleFactor();
        return true;
    }
    return false;
}
