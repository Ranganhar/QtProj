#include "mainwindow.h"
#include <qevent.h>

GraphicsView::GraphicsView(QGraphicsScene *scene, QWidget *parent = nullptr)
    : QGraphicsView(scene, parent),
      currentComponent(nullptr),
      currentWire(nullptr),
      isPlacingComponent(false),
      isPlacingWire(false) {}

void GraphicsView::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    if (isPlacingComponent) {
      // 放置元件
      Component *newComponent = new Resistor();
      scene()->addItem(newComponent);
      newComponent->setPos(mapToScene(event->pos()));
      isPlacingComponent = false;
    } else if (isPlacingWire) {
      // 开始放置连接线
      if (currentWire == nullptr) {
        currentWire = new Wire();
        scene()->addItem(currentWire);
        currentWire->setPos(mapToScene(event->pos()));
      }
    }
  }
}
void GraphicsView::mouseMoveEvent(QMouseEvent *event) {
  if (isPlacingWire && currentWire != nullptr) {
    currentWire->updateGeometry();
  }
}
void GraphicsView::mouseReleaseEvent(QMouseEvent *event) {
  if (isPlacingWire && currentWire != nullptr) {
    currentWire->updateGeometry();
    isPlacingWire = false;
  }
}
void GraphicsView::placeComponent() { isPlacingComponent = true; }
void GraphicsView::placeWire() { isPlacingWire = true; }
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  InitToolBar();
  InitGraph();
}

MainWindow::~MainWindow() {}

void MainWindow::InitToolBar() {
  menuToolBar = menuBar()->addMenu(tr("&ToolBar"));
  menuTest_put = menuBar()->addMenu(tr("&Put"));
  menuTest_file = menuBar()->addMenu(tr("&File"));

  toolBar_put = new QToolBar(tr("&put"), this);
  toolBar_file = new QToolBar(tr("&file"), this);

  actionTest_put = new QAction(tr("&Put tool"), this);
  actionTest_file = new QAction(tr("&File tool"), this);
  action_Open = new QAction(tr("&Open"), this);
  action_Save = new QAction(tr("&Save"), this);
  // add ToolBar act
  menuToolBar->addAction(actionTest_put);
  menuToolBar->addAction(actionTest_file);
  // add File act
  menuTest_file->addAction(action_Open);
  menuTest_file->addAction(action_Save);

  put = new QAction(tr("Put"), this);
  file = new QAction(tr("File"), this);
  toolBar_put->addAction(put);
  toolBar_file->addAction(file);
  // bind Put and File bar
  connect(actionTest_put, &QAction::triggered, this,
          [=]() { this->showToolBar(toolBar_put); });
  connect(actionTest_file, &QAction::triggered, this,
          [=]() { this->showToolBar(toolBar_file); });
  // bind open and save bar
  connect(action_Open, &QAction::triggered, this,
          &MainWindow::open_file_triggered);
  connect(action_Save, &QAction::triggered, this,
          &MainWindow::save_file_triggered);

  connect(put, &QAction::triggered, this, &MainWindow::on_put_triggered);
  connect(file, &QAction::triggered, this, &MainWindow::on_file_triggered);

  this->addToolBar(Qt::LeftToolBarArea, toolBar_put);
  this->addToolBar(Qt::LeftToolBarArea, toolBar_file);

  resize(1000, 800);
}
void MainWindow::showToolBar(QToolBar *toolBar) {
  foreach (QToolBar *bar, findChildren<QToolBar *>()) {
    bar->setVisible(bar == toolBar);
  }
}

void MainWindow::InitGraph() {
  scene = new QGraphicsScene(this);
  scene->setSceneRect(-100, -100, 400, 400);
  view = new GraphicsView(scene, this);
  view->setRenderHint(QPainter::Antialiasing);
  view->setFixedSize(800, 600);

  setCentralWidget(view);
}
void MainWindow::on_put_triggered() {
  qDebug() << "Test put";
  RectangleItem *item = new RectangleItem();
  scene->addItem(item);
  item->setPos(QPointF(100, 100));
}

void MainWindow::open_file_triggered() {
  qDebug() << "Open Test";
  auto file_name = QFileDialog::getOpenFileName(
      this, tr("Open File"), "", tr("All Files (*);;Text Files (*.txt)"));
  if(!file_name.isEmpty()){
    QFile target(file_name);
    //TODO实现打开文件后的操作
  }    
}

void MainWindow::save_file_triggered() { qDebug() << "Save Test"; }

void MainWindow::on_file_triggered() { qDebug() << "Test file"; }