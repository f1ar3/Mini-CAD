#include "PropertyPanel.h"
#include "../model/Document.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QGroupBox>

PropertyPanel::PropertyPanel(QWidget* parent)
    : QWidget(parent)
{
    auto* mainLayout = new QVBoxLayout(this);

    m_nameLabel = new QLabel("-", this);
    m_typeLabel = new QLabel("-", this);

    auto* infoGroup = new QGroupBox(tr("Информация"), this);
    auto* infoLayout = new QFormLayout(infoGroup);
    infoLayout->addRow(tr("Имя:"), m_nameLabel);
    infoLayout->addRow(tr("Тип:"), m_typeLabel);
    mainLayout->addWidget(infoGroup);

    auto* paramGroup = new QGroupBox(tr("Параметры"), this);
    m_formLayout = new QFormLayout(paramGroup);
    mainLayout->addWidget(paramGroup);

    mainLayout->addStretch();
}

void PropertyPanel::setDocument(Document* doc)
{
    m_document = doc;
}

void PropertyPanel::showShape(int id)
{
    clearFields();

    if (!m_document) return;
    const ShapeEntry* entry = m_document->findShape(id);
    if (!entry) {
        clearPanel();
        return;
    }

    m_nameLabel->setText(entry->name);
    m_typeLabel->setText(entry->type);

    for (auto it = entry->params.constBegin(); it != entry->params.constEnd(); ++it) {
        auto* label = new QLabel(QString::number(it.value(), 'f', 2) + tr(" мм"), this);
        m_formLayout->addRow(it.key() + ":", label);
    }
}

void PropertyPanel::clearPanel()
{
    m_nameLabel->setText("-");
    m_typeLabel->setText("-");
    clearFields();
}

void PropertyPanel::clearFields()
{
    while (m_formLayout->rowCount() > 0) {
        m_formLayout->removeRow(0);
    }
}
