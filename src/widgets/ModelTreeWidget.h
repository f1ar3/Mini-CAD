#ifndef MODELTREEWIDGET_H
#define MODELTREEWIDGET_H

#include <QTreeWidget>

class Document;

class ModelTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    explicit ModelTreeWidget(QWidget* parent = nullptr);

    void setDocument(Document* doc);
    void rebuild();
    void selectShapeById(int id);

signals:
    void shapeSelected(int id);
    void deleteRequested(int id);

private slots:
    void onItemClicked(QTreeWidgetItem* item, int column);
    void onCustomContextMenu(const QPoint& pos);

private:
    Document* m_document = nullptr;
};

#endif // MODELTREEWIDGET_H
