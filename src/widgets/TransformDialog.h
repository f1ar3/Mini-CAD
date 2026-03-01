#ifndef TRANSFORMDIALOG_H
#define TRANSFORMDIALOG_H

#include <QDialog>

class QDoubleSpinBox;
class QComboBox;

class TransformDialog : public QDialog
{
    Q_OBJECT

public:
    enum class Mode { Translate, Rotate, Scale };

    explicit TransformDialog(Mode mode, QWidget* parent = nullptr);

    // Translate
    double dx() const;
    double dy() const;
    double dz() const;

    // Rotate
    int axisIndex() const;    // 0=X, 1=Y, 2=Z
    double angleDeg() const;

    // Scale
    double scaleFactor() const;

    static bool getTranslation(double& dx, double& dy, double& dz, QWidget* parent);
    static bool getRotation(int& axis, double& angle, QWidget* parent);
    static bool getScale(double& factor, QWidget* parent);

private:
    Mode m_mode;
    QDoubleSpinBox* m_spinX = nullptr;
    QDoubleSpinBox* m_spinY = nullptr;
    QDoubleSpinBox* m_spinZ = nullptr;
    QComboBox*      m_axisCombo = nullptr;
    QDoubleSpinBox* m_angleSpin = nullptr;
    QDoubleSpinBox* m_scaleSpin = nullptr;
};

#endif // TRANSFORMDIALOG_H
