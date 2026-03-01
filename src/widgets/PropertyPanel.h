#ifndef PROPERTYPANEL_H
#define PROPERTYPANEL_H

#include <QWidget>

class QFormLayout;
class QLabel;
class Document;

class PropertyPanel : public QWidget
{
    Q_OBJECT

public:
    explicit PropertyPanel(QWidget* parent = nullptr);

    void setDocument(Document* doc);
    void showShape(int id);
    void clearPanel();

private:
    void clearFields();

    Document* m_document = nullptr;
    QFormLayout* m_formLayout;
    QLabel* m_nameLabel;
    QLabel* m_typeLabel;
};

#endif // PROPERTYPANEL_H
