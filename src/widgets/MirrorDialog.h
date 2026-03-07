#ifndef MIRRORDIALOG_H
#define MIRRORDIALOG_H

#include <QDialog>

class QComboBox;

class MirrorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MirrorDialog(QWidget* parent = nullptr);

    int planeIndex() const;

    static int getPlane(QWidget* parent);

private:
    QComboBox* m_planeCombo = nullptr;
};

#endif // MIRRORDIALOG_H
