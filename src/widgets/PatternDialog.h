#ifndef PATTERNDIALOG_H
#define PATTERNDIALOG_H

#include <QDialog>

class QComboBox;
class QSpinBox;
class QDoubleSpinBox;

class PatternDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PatternDialog(QWidget* parent = nullptr);

    bool isCircular() const;
    int axisIndex() const;
    double step() const;
    int count() const;

    static bool getParameters(bool& isCircular, int& axisIndex,
                              double& step, int& count, QWidget* parent);

private slots:
    void onTypeChanged(int index);

private:
    QComboBox* m_typeCombo = nullptr;
    QComboBox* m_axisCombo = nullptr;
    QDoubleSpinBox* m_stepSpin = nullptr;
    QSpinBox* m_countSpin = nullptr;
};

#endif // PATTERNDIALOG_H
