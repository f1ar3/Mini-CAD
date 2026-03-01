#include "BooleanDialog.h"
#include "../model/Document.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QMessageBox>

BooleanDialog::BooleanDialog(Document* doc, BooleanType type, QWidget* parent)
    : QDialog(parent)
{
    QString typeName;
    switch (type) {
    case BooleanType::Fuse:   typeName = tr("Объединение"); break;
    case BooleanType::Cut:    typeName = tr("Вычитание"); break;
    case BooleanType::Common: typeName = tr("Пересечение"); break;
    }
    setWindowTitle(tr("Булева операция: %1").arg(typeName));

    auto* layout = new QVBoxLayout(this);

    auto* hint = new QLabel(this);
    if (type == BooleanType::Cut) {
        hint->setText(tr("Выберите базовую фигуру и фигуру-инструмент для вычитания."));
    } else {
        hint->setText(tr("Выберите две фигуры для булевой операции."));
    }
    hint->setWordWrap(true);
    layout->addWidget(hint);

    auto* form = new QFormLayout;
    m_combo1 = new QComboBox(this);
    m_combo2 = new QComboBox(this);
    populateCombo(m_combo1, doc);
    populateCombo(m_combo2, doc);

    if (type == BooleanType::Cut) {
        form->addRow(tr("Базовая фигура:"), m_combo1);
        form->addRow(tr("Инструмент (вычесть):"), m_combo2);
    } else {
        form->addRow(tr("Фигура 1:"), m_combo1);
        form->addRow(tr("Фигура 2:"), m_combo2);
    }

    if (m_combo2->count() > 1) {
        m_combo2->setCurrentIndex(1);
    }

    layout->addLayout(form);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, [this]() {
        if (firstShapeId() == secondShapeId()) {
            QMessageBox::warning(this, tr("Ошибка"),
                tr("Пожалуйста, выберите две разные фигуры."));
            return;
        }
        accept();
    });
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttons);
}

void BooleanDialog::populateCombo(QComboBox* combo, Document* doc)
{
    for (const auto& entry : doc->shapes()) {
        combo->addItem(QString("%1 [%2]").arg(entry.name, entry.type), entry.id);
    }
}

int BooleanDialog::firstShapeId() const
{
    return m_combo1->currentData().toInt();
}

int BooleanDialog::secondShapeId() const
{
    return m_combo2->currentData().toInt();
}

bool BooleanDialog::getTwoShapes(Document* doc, BooleanType type,
                                  int& id1, int& id2, QWidget* parent)
{
    if (doc->shapes().size() < 2) {
        QMessageBox::information(parent, tr("Булева операция"),
            tr("Для булевой операции необходимо минимум 2 фигуры."));
        return false;
    }

    BooleanDialog dlg(doc, type, parent);
    if (dlg.exec() == QDialog::Accepted) {
        id1 = dlg.firstShapeId();
        id2 = dlg.secondShapeId();
        return true;
    }
    return false;
}
