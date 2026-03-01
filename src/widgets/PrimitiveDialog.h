#ifndef PRIMITIVEDIALOG_H
#define PRIMITIVEDIALOG_H

#include <QDialog>
#include <QMap>
#include <QString>
#include <QDoubleSpinBox>

class QFormLayout;

class PrimitiveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PrimitiveDialog(const QString& type, QWidget* parent = nullptr);

    QMap<QString, double> parameters() const;

    static QMap<QString, double> getParameters(const QString& type, QWidget* parent);

private:
    void setupFields(const QString& type);
    void addField(const QString& key, const QString& label, double defaultValue, double minVal = 0.1, double maxVal = 10000.0);

    QFormLayout* m_layout;
    QMap<QString, QDoubleSpinBox*> m_fields;
};

#endif // PRIMITIVEDIALOG_H
