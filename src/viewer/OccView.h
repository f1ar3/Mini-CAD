#ifndef OCCVIEW_H
#define OCCVIEW_H

#include <QWidget>

#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>

class OccView : public QWidget
{
    Q_OBJECT

public:
    explicit OccView(QWidget* parent = nullptr);
    ~OccView() override;

    Handle(AIS_InteractiveContext) context() const { return m_context; }
    Handle(V3d_View) view() const { return m_view; }

    void fitAll();
    void viewFront();
    void viewTop();
    void viewRight();
    void viewIso();

    QPaintEngine* paintEngine() const override { return nullptr; }

signals:
    void selectionChanged();

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    void initViewer();

    Handle(V3d_Viewer)            m_viewer;
    Handle(V3d_View)              m_view;
    Handle(AIS_InteractiveContext) m_context;

    enum class CurAction {
        Nothing,
        Rotation,
        Pan,
        Zoom
    };

    CurAction m_currentAction = CurAction::Nothing;
    QPoint    m_clickPos;
    bool      m_initialized = false;
};

#endif // OCCVIEW_H
