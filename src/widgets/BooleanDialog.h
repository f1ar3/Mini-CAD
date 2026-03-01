#ifndef BOOLEANDIALOG_H
#define BOOLEANDIALOG_H

#include <QDialog>

class QComboBox;
class Document;
enum class BooleanType;

class BooleanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BooleanDialog(Document* doc, BooleanType type, QWidget* parent = nullptr);

    int firstShapeId() const;
    int secondShapeId() const;

    static bool getTwoShapes(Document* doc, BooleanType type,
                              int& id1, int& id2, QWidget* parent);

private:
    void populateCombo(QComboBox* combo, Document* doc);

    QComboBox* m_combo1;
    QComboBox* m_combo2;
};

#endif // BOOLEANDIALOG_H
