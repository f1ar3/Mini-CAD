#include "ModelTreeWidget.h"
#include "../model/Document.h"

#include <QMenu>
#include <QInputDialog>

ModelTreeWidget::ModelTreeWidget(QWidget* parent)
    : QTreeWidget(parent)
{
    setHeaderLabel(tr("Дерево модели"));
    setSelectionMode(QAbstractItemView::SingleSelection);
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, &QTreeWidget::itemClicked, this, &ModelTreeWidget::onItemClicked);
    connect(this, &QTreeWidget::customContextMenuRequested, this, &ModelTreeWidget::onCustomContextMenu);
}

void ModelTreeWidget::setDocument(Document* doc)
{
    m_document = doc;
    if (m_document) {
        connect(m_document, &Document::modelChanged, this, &ModelTreeWidget::rebuild);
    }
}

void ModelTreeWidget::rebuild()
{
    clear();
    if (!m_document) return;

    for (const auto& entry : m_document->shapes()) {
        auto* item = new QTreeWidgetItem(this);
        item->setText(0, QString("%1 [%2]").arg(entry.name, entry.type));
        item->setData(0, Qt::UserRole, entry.id);
    }
}

void ModelTreeWidget::selectShapeById(int id)
{
    for (int i = 0; i < topLevelItemCount(); ++i) {
        auto* item = topLevelItem(i);
        if (item->data(0, Qt::UserRole).toInt() == id) {
            setCurrentItem(item);
            return;
        }
    }
    clearSelection();
}

void ModelTreeWidget::onItemClicked(QTreeWidgetItem* item, int /*column*/)
{
    if (!item) return;
    int id = item->data(0, Qt::UserRole).toInt();
    emit shapeSelected(id);
}

void ModelTreeWidget::onCustomContextMenu(const QPoint& pos)
{
    auto* item = itemAt(pos);
    if (!item || !m_document) return;

    int id = item->data(0, Qt::UserRole).toInt();

    QMenu menu(this);

    QAction* renameAct = menu.addAction(tr("Переименовать..."));
    QAction* deleteAct = menu.addAction(tr("Удалить"));

    QAction* chosen = menu.exec(mapToGlobal(pos));

    if (chosen == deleteAct) {
        emit deleteRequested(id);
    } else if (chosen == renameAct) {
        ShapeEntry* entry = m_document->findShape(id);
        if (!entry) return;
        bool ok;
        QString newName = QInputDialog::getText(this, tr("Переименовать"),
                                                 tr("Новое имя:"),
                                                 QLineEdit::Normal, entry->name, &ok);
        if (ok && !newName.isEmpty()) {
            entry->name = newName;
            rebuild();
        }
    }
}
