#include "PatternDialog.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QLabel>

PatternDialog::PatternDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Массив копий"));

    auto* layout = new QVBoxLayout(this);
    auto* form = new QFormLayout();

    m_typeCombo = new QComboBox(this);
    m_typeCombo->addItem(tr("Линейный"));
    m_typeCombo->addItem(tr("Круговой"));
    connect(m_typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PatternDialog::onTypeChanged);
    form->addRow(tr("Тип:"), m_typeCombo);

    m_axisCombo = new QComboBox(this);
    m_axisCombo->addItems({"X", "Y", "Z"});
    form->addRow(tr("Ось:"), m_axisCombo);

    m_stepSpin = new QDoubleSpinBox(this);
    m_stepSpin->setRange(0.1, 100000.0);
    m_stepSpin->setDecimals(2);
    m_stepSpin->setValue(50.0);
    m_stepSpin->setSuffix(tr(" мм"));
    form->addRow(tr("Шаг:"), m_stepSpin);

    m_countSpin = new QSpinBox(this);
    m_countSpin->setRange(2, 50);
    m_countSpin->setValue(3);
    form->addRow(tr("Количество:"), m_countSpin);

    layout->addLayout(form);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttons);
}

bool PatternDialog::isCircular() const
{
    return m_typeCombo->currentIndex() == 1;
}

int PatternDialog::axisIndex() const
{
    return m_axisCombo->currentIndex();
}

double PatternDialog::step() const
{
    return m_stepSpin->value();
}

int PatternDialog::count() const
{
    return m_countSpin->value();
}

void PatternDialog::onTypeChanged(int index)
{
    if (index == 1) { // Circular
        m_stepSpin->setSuffix(tr(" °"));
        m_stepSpin->setRange(0.1, 360.0);
        m_stepSpin->setValue(45.0);
    } else { // Linear
        m_stepSpin->setSuffix(tr(" мм"));
        m_stepSpin->setRange(0.1, 100000.0);
        m_stepSpin->setValue(50.0);
    }
}

bool PatternDialog::getParameters(bool& isCircular, int& axisIndex,
                                   double& step, int& count, QWidget* parent)
{
    PatternDialog dlg(parent);
    if (dlg.exec() != QDialog::Accepted) return false;

    isCircular = dlg.isCircular();
    axisIndex = dlg.axisIndex();
    step = dlg.step();
    count = dlg.count();
    return true;
}
