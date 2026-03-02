#include "MainWindow.h"
#include "../src/viewer/OccView.h"
#include "../src/model/Document.h"
#include "../src/io/FileIO.h"
#include "../src/widgets/PrimitiveDialog.h"
#include "../src/widgets/TransformDialog.h"
#include "../src/widgets/BooleanDialog.h"
#include "../src/widgets/ModelTreeWidget.h"
#include "../src/widgets/PropertyPanel.h"

#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QDockWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QApplication>
#include <QStyle>
#include <QFileInfo>


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Мини-САПР"));
    resize(1280, 800);

    m_occView = new OccView(this);
    setCentralWidget(m_occView);

    createActions();
    createMenus();
    createToolBars();
    createDockWidgets();

    statusBar()->showMessage(tr("Готово. Создавайте фигуры через меню Моделирование или панель инструментов."));

    QMetaObject::invokeMethod(this, &MainWindow::initDocument, Qt::QueuedConnection);
}

MainWindow::~MainWindow() = default;

void MainWindow::initDocument()
{
    if (m_occView->context().IsNull()) {
        m_occView->repaint();
    }
    m_document = new Document(m_occView->context(), this);

    m_modelTree->setDocument(m_document);
    m_propertyPanel->setDocument(m_document);

    connect(m_document, &Document::selectionChanged, this, &MainWindow::onSelectionChanged);
    connect(m_occView, &OccView::selectionChanged, this, &MainWindow::onViewerSelectionChanged);

    // Undo / Redo
    connect(m_actUndo, &QAction::triggered, this, [this]() {
        if (!m_document) return;
        m_document->undo();
        m_occView->fitAll();
        statusBar()->showMessage(tr("Отменено"));
    });
    connect(m_actRedo, &QAction::triggered, this, [this]() {
        if (!m_document) return;
        m_document->redo();
        m_occView->fitAll();
        statusBar()->showMessage(tr("Повторено"));
    });
    connect(m_document, &Document::undoRedoChanged, this, [this]() {
        m_actUndo->setEnabled(m_document->canUndo());
        m_actRedo->setEnabled(m_document->canRedo());

        QString undoTip = m_document->canUndo()
            ? tr("Отменить: %1").arg(m_document->undoDescription())
            : tr("Отменить");
        QString redoTip = m_document->canRedo()
            ? tr("Повторить: %1").arg(m_document->redoDescription())
            : tr("Повторить");
        m_actUndo->setToolTip(undoTip);
        m_actRedo->setToolTip(redoTip);
    });
}

// ============================================================
//  Действия (Actions)
// ============================================================

void MainWindow::createActions()
{
    auto* style = QApplication::style();

    // --- Файл ---
    m_actNew = new QAction(style->standardIcon(QStyle::SP_FileIcon), tr("Новый"), this);
    m_actNew->setShortcut(QKeySequence::New);
    m_actNew->setToolTip(tr("Создать новый пустой документ (Cmd+N)"));
    connect(m_actNew, &QAction::triggered, this, &MainWindow::onNewDocument);

    m_actOpen = new QAction(style->standardIcon(QStyle::SP_DialogOpenButton), tr("Открыть..."), this);
    m_actOpen->setShortcut(QKeySequence::Open);
    m_actOpen->setToolTip(tr("Импортировать файл STEP/IGES/BRep (Cmd+O)"));
    connect(m_actOpen, &QAction::triggered, this, &MainWindow::onOpenFile);

    m_actSaveAs = new QAction(style->standardIcon(QStyle::SP_DialogSaveButton), tr("Сохранить как..."), this);
    m_actSaveAs->setShortcut(QKeySequence::SaveAs);
    m_actSaveAs->setToolTip(tr("Экспортировать в STEP/IGES/BRep (Cmd+Shift+S)"));
    connect(m_actSaveAs, &QAction::triggered, this, &MainWindow::onSaveAs);

    m_actExit = new QAction(tr("Выход"), this);
    m_actExit->setShortcut(QKeySequence::Quit);
    connect(m_actExit, &QAction::triggered, this, &QWidget::close);

    // --- Правка ---
    m_actUndo = new QAction(style->standardIcon(QStyle::SP_ArrowBack), tr("Отменить"), this);
    m_actUndo->setShortcut(QKeySequence::Undo);
    m_actUndo->setEnabled(false);

    m_actRedo = new QAction(style->standardIcon(QStyle::SP_ArrowForward), tr("Повторить"), this);
    m_actRedo->setShortcut(QKeySequence::Redo);
    m_actRedo->setEnabled(false);

    m_actDelete = new QAction(style->standardIcon(QStyle::SP_TrashIcon), tr("Удалить"), this);
    m_actDelete->setShortcut(QKeySequence::Delete);
    m_actDelete->setToolTip(tr("Удалить выделенную фигуру (Del)"));
    connect(m_actDelete, &QAction::triggered, this, &MainWindow::onDeleteSelected);

    // --- Вид ---
    m_actFitAll = new QAction(tr("Показать всё"), this);
    m_actFitAll->setShortcut(tr("F"));
    m_actFitAll->setToolTip(tr("Вписать все фигуры в окно (F)"));
    connect(m_actFitAll, &QAction::triggered, this, &MainWindow::onViewFitAll);

    m_actViewFront = new QAction(tr("Спереди"), this);
    m_actViewFront->setShortcut(tr("1"));
    connect(m_actViewFront, &QAction::triggered, this, &MainWindow::onViewFront);

    m_actViewTop = new QAction(tr("Сверху"), this);
    m_actViewTop->setShortcut(tr("2"));
    connect(m_actViewTop, &QAction::triggered, this, &MainWindow::onViewTop);

    m_actViewRight = new QAction(tr("Справа"), this);
    m_actViewRight->setShortcut(tr("3"));
    connect(m_actViewRight, &QAction::triggered, this, &MainWindow::onViewRight);

    m_actViewIso = new QAction(tr("Изометрия"), this);
    m_actViewIso->setShortcut(tr("0"));
    m_actViewIso->setToolTip(tr("Изометрический вид (0)"));
    connect(m_actViewIso, &QAction::triggered, this, &MainWindow::onViewIso);

    // --- Примитивы ---
    m_actBox = new QAction(tr("Параллелепипед"), this);
    connect(m_actBox, &QAction::triggered, this, &MainWindow::onCreateBox);

    m_actCylinder = new QAction(tr("Цилиндр"), this);
    connect(m_actCylinder, &QAction::triggered, this, &MainWindow::onCreateCylinder);

    m_actSphere = new QAction(tr("Сфера"), this);
    connect(m_actSphere, &QAction::triggered, this, &MainWindow::onCreateSphere);

    m_actCone = new QAction(tr("Конус"), this);
    connect(m_actCone, &QAction::triggered, this, &MainWindow::onCreateCone);

    m_actTorus = new QAction(tr("Тор"), this);
    connect(m_actTorus, &QAction::triggered, this, &MainWindow::onCreateTorus);

    m_actWedge = new QAction(tr("Клин"), this);
    connect(m_actWedge, &QAction::triggered, this, &MainWindow::onCreateWedge);

    // --- Трансформации ---
    m_actTranslate = new QAction(tr("Переместить..."), this);
    m_actTranslate->setShortcut(tr("G"));
    m_actTranslate->setToolTip(tr("Переместить фигуру (G)"));
    connect(m_actTranslate, &QAction::triggered, this, &MainWindow::onTranslate);

    m_actRotate = new QAction(tr("Повернуть..."), this);
    m_actRotate->setShortcut(tr("R"));
    m_actRotate->setToolTip(tr("Повернуть фигуру (R)"));
    connect(m_actRotate, &QAction::triggered, this, &MainWindow::onRotate);

    m_actScale = new QAction(tr("Масштабировать..."), this);
    m_actScale->setShortcut(tr("S"));
    m_actScale->setToolTip(tr("Масштабировать фигуру (S)"));
    connect(m_actScale, &QAction::triggered, this, &MainWindow::onScale);

    // --- Булевы операции ---
    m_actFuse = new QAction(tr("Объединение"), this);
    m_actFuse->setToolTip(tr("Объединить две фигуры в одну"));
    connect(m_actFuse, &QAction::triggered, this, &MainWindow::onBoolFuse);

    m_actCut = new QAction(tr("Вычитание"), this);
    m_actCut->setToolTip(tr("Вычесть одну фигуру из другой"));
    connect(m_actCut, &QAction::triggered, this, &MainWindow::onBoolCut);

    m_actCommon = new QAction(tr("Пересечение"), this);
    m_actCommon->setToolTip(tr("Оставить только пересечение двух фигур"));
    connect(m_actCommon, &QAction::triggered, this, &MainWindow::onBoolCommon);

    // --- Скругление / Фаска ---
    m_actFillet = new QAction(tr("Скругление..."), this);
    m_actFillet->setToolTip(tr("Скруглить все рёбра выделенной фигуры"));
    connect(m_actFillet, &QAction::triggered, this, &MainWindow::onFillet);

    m_actChamfer = new QAction(tr("Фаска..."), this);
    m_actChamfer->setToolTip(tr("Снять фаску со всех рёбер выделенной фигуры"));
    connect(m_actChamfer, &QAction::triggered, this, &MainWindow::onChamfer);
}

// ============================================================
//  Меню
// ============================================================

void MainWindow::createMenus()
{
    // Файл
    QMenu* fileMenu = menuBar()->addMenu(tr("&Файл"));
    fileMenu->addAction(m_actNew);
    fileMenu->addAction(m_actOpen);
    fileMenu->addAction(m_actSaveAs);
    fileMenu->addSeparator();
    fileMenu->addAction(m_actExit);

    // Правка
    QMenu* editMenu = menuBar()->addMenu(tr("&Правка"));
    editMenu->addAction(m_actUndo);
    editMenu->addAction(m_actRedo);
    editMenu->addSeparator();
    editMenu->addAction(m_actDelete);

    // Вид
    QMenu* viewMenu = menuBar()->addMenu(tr("&Вид"));
    viewMenu->addAction(m_actFitAll);
    viewMenu->addSeparator();
    viewMenu->addAction(m_actViewFront);
    viewMenu->addAction(m_actViewTop);
    viewMenu->addAction(m_actViewRight);
    viewMenu->addAction(m_actViewIso);

    // Моделирование
    QMenu* modelMenu = menuBar()->addMenu(tr("&Моделирование"));
    modelMenu->addAction(m_actBox);
    modelMenu->addAction(m_actCylinder);
    modelMenu->addAction(m_actSphere);
    modelMenu->addAction(m_actCone);
    modelMenu->addAction(m_actTorus);
    modelMenu->addAction(m_actWedge);
    modelMenu->addSeparator();
    modelMenu->addAction(m_actFuse);
    modelMenu->addAction(m_actCut);
    modelMenu->addAction(m_actCommon);
    modelMenu->addSeparator();
    modelMenu->addAction(m_actFillet);
    modelMenu->addAction(m_actChamfer);

    // Трансформация
    QMenu* transformMenu = menuBar()->addMenu(tr("&Трансформация"));
    transformMenu->addAction(m_actTranslate);
    transformMenu->addAction(m_actRotate);
    transformMenu->addAction(m_actScale);
}

// ============================================================
//  Панели инструментов
// ============================================================

void MainWindow::createToolBars()
{
    // Файл
    QToolBar* fileBar = addToolBar(tr("Файл"));
    fileBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    fileBar->addAction(m_actNew);
    fileBar->addAction(m_actOpen);
    fileBar->addAction(m_actSaveAs);

    // Вид
    QToolBar* viewBar = addToolBar(tr("Вид"));
    viewBar->addAction(m_actFitAll);
    viewBar->addAction(m_actViewFront);
    viewBar->addAction(m_actViewTop);
    viewBar->addAction(m_actViewRight);
    viewBar->addAction(m_actViewIso);

    // Примитивы
    addToolBarBreak();
    QToolBar* primBar = addToolBar(tr("Примитивы"));
    primBar->setToolButtonStyle(Qt::ToolButtonTextOnly);
    primBar->addAction(m_actBox);
    primBar->addAction(m_actCylinder);
    primBar->addAction(m_actSphere);
    primBar->addAction(m_actCone);
    primBar->addAction(m_actTorus);
    primBar->addAction(m_actWedge);

    // Булевы
    QToolBar* boolBar = addToolBar(tr("Булевы"));
    boolBar->setToolButtonStyle(Qt::ToolButtonTextOnly);
    boolBar->addAction(m_actFuse);
    boolBar->addAction(m_actCut);
    boolBar->addAction(m_actCommon);

    // Операции
    QToolBar* opsBar = addToolBar(tr("Операции"));
    opsBar->setToolButtonStyle(Qt::ToolButtonTextOnly);
    opsBar->addAction(m_actFillet);
    opsBar->addAction(m_actChamfer);

    // Трансформация
    QToolBar* transBar = addToolBar(tr("Трансформация"));
    transBar->setToolButtonStyle(Qt::ToolButtonTextOnly);
    transBar->addAction(m_actTranslate);
    transBar->addAction(m_actRotate);
    transBar->addAction(m_actScale);
    transBar->addSeparator();
    transBar->addAction(m_actDelete);
}

// ============================================================
//  Док-панели
// ============================================================

void MainWindow::createDockWidgets()
{
    QDockWidget* treeDock = new QDockWidget(tr("Дерево модели"), this);
    treeDock->setMinimumWidth(200);
    m_modelTree = new ModelTreeWidget(treeDock);
    treeDock->setWidget(m_modelTree);
    addDockWidget(Qt::LeftDockWidgetArea, treeDock);

    connect(m_modelTree, &ModelTreeWidget::shapeSelected, this, &MainWindow::onTreeShapeSelected);
    connect(m_modelTree, &ModelTreeWidget::deleteRequested, this, [this](int id) {
        if (!m_document) return;
        m_document->removeShape(id);
        m_propertyPanel->clearPanel();
        statusBar()->showMessage(tr("Фигура удалена"));
    });

    QDockWidget* propDock = new QDockWidget(tr("Свойства"), this);
    propDock->setMinimumWidth(200);
    m_propertyPanel = new PropertyPanel(propDock);
    propDock->setWidget(m_propertyPanel);
    addDockWidget(Qt::LeftDockWidgetArea, propDock);
}

// ============================================================
//  Вспомогательные методы
// ============================================================

int MainWindow::requireSelectedShape()
{
    if (!m_document) return -1;
    int id = m_document->selectedShapeId();
    if (id < 0) {
        QMessageBox::information(this, tr("Нет выделения"),
            tr("Сначала выделите фигуру, кликнув на неё в 3D-виде или в дереве модели."));
    }
    return id;
}

void MainWindow::createPrimitive(const QString& type)
{
    if (!m_document) return;

    auto params = PrimitiveDialog::getParameters(type, this);
    if (params.isEmpty()) return;

    TopoDS_Shape shape = Document::rebuildShape(type, params);
    if (shape.IsNull()) return;

    m_document->addShape(type, params, shape);
    m_occView->fitAll();
    statusBar()->showMessage(tr("Создан: %1").arg(type));
}

void MainWindow::doBooleanOp(int boolType)
{
    if (!m_document) return;

    auto bt = static_cast<BooleanType>(boolType);
    int id1, id2;
    if (!BooleanDialog::getTwoShapes(m_document, bt, id1, id2, this)) return;

    int result = m_document->booleanOperation(id1, id2, bt);
    if (result < 0) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Булева операция не удалась."));
    } else {
        m_occView->fitAll();
        statusBar()->showMessage(tr("Булева операция выполнена"));
    }
}

// ============================================================
//  Файл
// ============================================================

void MainWindow::onNewDocument()
{
    if (!m_document) return;
    m_document->clearAll();
    m_propertyPanel->clearPanel();
    statusBar()->showMessage(tr("Новый документ"));
}

void MainWindow::onOpenFile()
{
    QString path = QFileDialog::getOpenFileName(
        this, tr("Импорт файла"), QString(), FileIO::supportedImportFormats());
    if (path.isEmpty()) return;

    if (!m_document) return;
    m_document->importFile(path);
    m_occView->fitAll();
    statusBar()->showMessage(tr("Импортирован: %1").arg(QFileInfo(path).fileName()));
}

void MainWindow::onSaveAs()
{
    if (!m_document || m_document->shapes().isEmpty()) {
        QMessageBox::warning(this, tr("Предупреждение"), tr("Нет фигур для экспорта."));
        return;
    }

    QString path = QFileDialog::getSaveFileName(
        this, tr("Экспорт файла"), QString(), FileIO::supportedExportFormats());
    if (path.isEmpty()) return;

    m_document->exportFile(path);
    statusBar()->showMessage(tr("Экспортирован: %1").arg(QFileInfo(path).fileName()));
}

// ============================================================
//  Вид
// ============================================================

void MainWindow::onViewFitAll() { m_occView->fitAll(); }
void MainWindow::onViewFront() { m_occView->viewFront(); }
void MainWindow::onViewTop()   { m_occView->viewTop(); }
void MainWindow::onViewRight() { m_occView->viewRight(); }
void MainWindow::onViewIso()   { m_occView->viewIso(); }

// ============================================================
//  Примитивы
// ============================================================

void MainWindow::onCreateBox()      { createPrimitive("Box"); }
void MainWindow::onCreateCylinder() { createPrimitive("Cylinder"); }
void MainWindow::onCreateSphere()   { createPrimitive("Sphere"); }
void MainWindow::onCreateCone()     { createPrimitive("Cone"); }
void MainWindow::onCreateTorus()    { createPrimitive("Torus"); }
void MainWindow::onCreateWedge()    { createPrimitive("Wedge"); }


// ============================================================
//  Трансформации
// ============================================================

void MainWindow::onTranslate()
{
    int id = requireSelectedShape();
    if (id < 0) return;

    double dx, dy, dz;
    if (TransformDialog::getTranslation(dx, dy, dz, this)) {
        m_document->translateShape(id, dx, dy, dz);
        m_occView->fitAll();
        statusBar()->showMessage(tr("Перемещено на (%1, %2, %3)").arg(dx).arg(dy).arg(dz));
    }
}

void MainWindow::onRotate()
{
    int id = requireSelectedShape();
    if (id < 0) return;

    int axis;
    double angle;
    if (TransformDialog::getRotation(axis, angle, this)) {
        m_document->rotateShape(id, axis, angle);
        m_occView->fitAll();
        QString axisName = (axis == 0) ? "X" : (axis == 1) ? "Y" : "Z";
        statusBar()->showMessage(tr("Повёрнуто на %1 град. вокруг %2").arg(angle).arg(axisName));
    }
}

void MainWindow::onScale()
{
    int id = requireSelectedShape();
    if (id < 0) return;

    double factor;
    if (TransformDialog::getScale(factor, this)) {
        m_document->scaleShape(id, factor);
        m_occView->fitAll();
        statusBar()->showMessage(tr("Масштабировано на %1").arg(factor));
    }
}

// ============================================================
//  Булевы операции
// ============================================================

void MainWindow::onBoolFuse()   { doBooleanOp(static_cast<int>(BooleanType::Fuse)); }
void MainWindow::onBoolCut()    { doBooleanOp(static_cast<int>(BooleanType::Cut)); }
void MainWindow::onBoolCommon() { doBooleanOp(static_cast<int>(BooleanType::Common)); }

// ============================================================
//  Скругление / Фаска
// ============================================================

void MainWindow::onFillet()
{
    int id = requireSelectedShape();
    if (id < 0) return;

    bool ok;
    double radius = QInputDialog::getDouble(this, tr("Скругление"),
        tr("Радиус скругления (мм):"), 5.0, 0.1, 10000.0, 2, &ok);
    if (!ok) return;

    int result = m_document->filletShape(id, radius);
    if (result < 0) {
        QMessageBox::warning(this, tr("Ошибка скругления"),
            tr("Скругление не удалось. Возможно, радиус слишком велик для рёбер фигуры."));
    } else {
        m_occView->fitAll();
        statusBar()->showMessage(tr("Скругление применено (R=%1)").arg(radius));
    }
}

void MainWindow::onChamfer()
{
    int id = requireSelectedShape();
    if (id < 0) return;

    bool ok;
    double dist = QInputDialog::getDouble(this, tr("Фаска"),
        tr("Расстояние фаски (мм):"), 3.0, 0.1, 10000.0, 2, &ok);
    if (!ok) return;

    int result = m_document->chamferShape(id, dist);
    if (result < 0) {
        QMessageBox::warning(this, tr("Ошибка фаски"),
            tr("Фаска не удалась. Возможно, расстояние слишком велико для рёбер фигуры."));
    } else {
        m_occView->fitAll();
        statusBar()->showMessage(tr("Фаска применена (D=%1)").arg(dist));
    }
}

// ============================================================
//  Удаление
// ============================================================

void MainWindow::onDeleteSelected()
{
    int id = requireSelectedShape();
    if (id < 0) return;

    m_document->removeShape(id);
    m_propertyPanel->clearPanel();
    statusBar()->showMessage(tr("Фигура удалена"));
}

// ============================================================
//  Синхронизация выделения
// ============================================================

void MainWindow::onViewerSelectionChanged()
{
    if (!m_document) return;
    m_document->syncSelectionFromViewer();
}

void MainWindow::onTreeShapeSelected(int id)
{
    if (!m_document) return;

    ShapeEntry* entry = m_document->findShape(id);
    if (entry) {
        m_document->context()->ClearSelected(Standard_False);
        m_document->context()->AddOrRemoveSelected(entry->aisShape, Standard_True);
    }

    onSelectionChanged(id);
}

void MainWindow::onSelectionChanged(int id)
{
    m_modelTree->selectShapeById(id);
    m_propertyPanel->showShape(id);
}
