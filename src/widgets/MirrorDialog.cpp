#include "MirrorDialog.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QDialogButtonBox>

MirrorDialog::MirrorDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Зеркальное отражение"));

    auto* layout = new QVBoxLayout(this);
    auto* form = new QFormLayout();

    m_planeCombo = new QComboBox(this);
    m_planeCombo->addItem(tr("XY (отражение по Z)"));
    m_planeCombo->addItem(tr("XZ (отражение по Y)"));
    m_planeCombo->addItem(tr("YZ (отражение по X)"));
    form->addRow(tr("Плоскость:"), m_planeCombo);

    layout->addLayout(form);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttons);
}

int MirrorDialog::planeIndex() const
{
    return m_planeCombo->currentIndex();
}

int MirrorDialog::getPlane(QWidget* parent)
{
    MirrorDialog dlg(parent);
    if (dlg.exec() != QDialog::Accepted) return -1;
    return dlg.planeIndex();
}
