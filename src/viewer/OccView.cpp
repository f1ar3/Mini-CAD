#include "OccView.h"

#include <QMouseEvent>
#include <QWheelEvent>

#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <V3d_AmbientLight.hxx>
#include <V3d_DirectionalLight.hxx>
#include <Graphic3d_GraphicDriver.hxx>
#include <Prs3d_DatumAspect.hxx>

#ifdef __APPLE__
#include <Cocoa_Window.hxx>
#endif

OccView::OccView(QWidget* parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_NativeWindow);
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setBackgroundRole(QPalette::NoRole);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

OccView::~OccView() = default;

void OccView::initViewer()
{
    if (m_initialized) return;

    Handle(Aspect_DisplayConnection) displayConn = new Aspect_DisplayConnection();
    Handle(OpenGl_GraphicDriver) driver = new OpenGl_GraphicDriver(displayConn);

    m_viewer = new V3d_Viewer(driver);
    m_viewer->SetDefaultLights();
    m_viewer->SetLightOn();

    m_context = new AIS_InteractiveContext(m_viewer);

    m_view = m_viewer->CreateView();

#ifdef __APPLE__
    NSView* nsview = reinterpret_cast<NSView*>(winId());
    Handle(Cocoa_Window) cocoaWin = new Cocoa_Window(nsview);
    m_view->SetWindow(cocoaWin);
    if (!cocoaWin->IsMapped()) {
        cocoaWin->Map();
    }
#endif

    m_view->SetBackgroundColor(Quantity_NOC_GRAY30);
    m_view->MustBeResized();
    m_view->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_WHITE, 0.1);

    m_context->SetDisplayMode(AIS_Shaded, Standard_True);

    m_initialized = true;
}

void OccView::fitAll()
{
    if (m_view.IsNull()) return;
    m_view->FitAll();
    m_view->ZFitAll();
    m_view->Redraw();
}

void OccView::viewFront()
{
    if (m_view.IsNull()) return;
    m_view->SetProj(V3d_Yneg);
    fitAll();
}

void OccView::viewTop()
{
    if (m_view.IsNull()) return;
    m_view->SetProj(V3d_Zpos);
    fitAll();
}

void OccView::viewRight()
{
    if (m_view.IsNull()) return;
    m_view->SetProj(V3d_Xpos);
    fitAll();
}

void OccView::viewIso()
{
    if (m_view.IsNull()) return;
    m_view->SetProj(V3d_XposYnegZpos);
    fitAll();
}

void OccView::paintEvent(QPaintEvent* /*event*/)
{
    if (!m_initialized) {
        initViewer();
    }
    if (!m_view.IsNull()) {
        m_view->Redraw();
    }
}

void OccView::resizeEvent(QResizeEvent* /*event*/)
{
    if (!m_view.IsNull()) {
        m_view->MustBeResized();
    }
}

void OccView::mousePressEvent(QMouseEvent* event)
{
    m_clickPos = event->pos();

    if (event->button() == Qt::RightButton) {
        if (event->modifiers() & Qt::ShiftModifier) {
            m_currentAction = CurAction::Zoom;
        } else {
            m_currentAction = CurAction::Rotation;
            m_view->StartRotation(event->pos().x(), event->pos().y());
        }
    } else if (event->button() == Qt::MiddleButton) {
        m_currentAction = CurAction::Pan;
    } else if (event->button() == Qt::LeftButton) {
        m_context->MoveTo(event->pos().x(), event->pos().y(), m_view, Standard_True);
        if (event->modifiers() & (Qt::MetaModifier | Qt::ShiftModifier)) {
            m_context->SelectDetected(AIS_SelectionScheme_XOR);
        } else {
            m_context->SelectDetected(AIS_SelectionScheme_Replace);
        }
        emit selectionChanged();
    }
}

void OccView::mouseMoveEvent(QMouseEvent* event)
{
    if (m_view.IsNull()) return;

    switch (m_currentAction) {
    case CurAction::Rotation:
        m_view->Rotation(event->pos().x(), event->pos().y());
        break;
    case CurAction::Pan:
        m_view->Pan(event->pos().x() - m_clickPos.x(),
                     m_clickPos.y() - event->pos().y());
        m_clickPos = event->pos();
        break;
    case CurAction::Zoom: {
        int dy = event->pos().y() - m_clickPos.y();
        m_view->Zoom(0, 0, 0, dy);
        m_clickPos = event->pos();
        break;
    }
    case CurAction::Nothing:
        m_context->MoveTo(event->pos().x(), event->pos().y(), m_view, Standard_True);
        break;
    }
}

void OccView::mouseReleaseEvent(QMouseEvent* /*event*/)
{
    m_currentAction = CurAction::Nothing;
}

void OccView::wheelEvent(QWheelEvent* event)
{
    if (m_view.IsNull()) return;

    Standard_Real delta = event->angleDelta().y();
    if (qAbs(delta) > 0) {
        Standard_Real factor = (delta > 0) ? 1.1 : 0.9;
        m_view->SetZoom(factor);
    }
    m_view->Redraw();
}
