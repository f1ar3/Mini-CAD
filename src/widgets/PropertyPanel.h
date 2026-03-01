#ifndef PROPERTYPANEL_H
#define PROPERTYPANEL_H

#include <QWidget>
#include <QMap>

class QFormLayout;
class QLabel;
class QLineEdit;
class QPushButton;
class QDoubleSpinBox;
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
    void clearParamFields();
    void onNameChanged();
    void onColorClicked();
    void onParamChanged();
    void updateColorButton(const QColor& color);

    static QStringList editableTypes();

    Document* m_document = nullptr;
    int m_currentShapeId = -1;
    bool m_updatingUI = false;

    QFormLayout* m_formLayout;
    QLineEdit* m_nameEdit;
    QLabel* m_typeLabel;
    QPushButton* m_colorButton;

    QMap<QString, QDoubleSpinBox*> m_paramSpinBoxes;
};

#endif // PROPERTYPANEL_H
