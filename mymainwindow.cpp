#include "mymainwindow.h"
#include "ui_mymainwindow.h"
#include "sizedialog.h"

#include <QShortcut>
#include <QFileDialog>
#include <QColorDialog>
#include <QIcon>
#include <QWidgetAction>
#include <QAction>
#include <QtWidgets>

MyMainWindow::MyMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MyMainWindow)
{
    ui->setupUi(this);

    ui->m_gridLayout->setAlignment(Qt::AlignLeft);
    ui->m_hrzLayoutFirst->setAlignment(Qt::AlignLeft);
    ui->m_hrzLayoutMain->setAlignment(Qt::AlignLeft);

    ui->m_hrzLayoutMain_2->hide();

    m_scene = new paintScene();
    ui->m_graphView->setScene(m_scene);
    ui->m_graphView->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    //installEventFilter(ui->m_graphView);

    m_timer = new QTimer();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slotTimer()));
    m_timer->start(100);

    fileMenuFill();

    connect(this, SIGNAL(win_toolChanged(currentTool)), m_scene, SLOT(toolChanged(currentTool)));
    connect(this, SIGNAL(win_colorChanged(QColor)), m_scene, SLOT(colorChanged(QColor)));
    connect(this, SIGNAL(win_isZooming(bool)), m_scene, SLOT(isZooming(bool)));

    QShortcut* zoomingIn = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Equal), this);
    QShortcut* zoomingOut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Minus), this);
    connect(zoomingIn, &QShortcut::activated, this, &MyMainWindow::zoomIn);
    connect(zoomingOut, &QShortcut::activated, this, &MyMainWindow::zoomOut);

    brushButtonFill();
    figureButtonFill();
    eraserButtonFill();
    textButtonFill();

    connect(this, SIGNAL(win_figureChanged(currentFigureTool)), m_scene, SLOT(figureChanged(currentFigureTool)));
}

MyMainWindow::~MyMainWindow()
{
    delete ui;
}

void MyMainWindow::slotTimer()
{
    m_timer->stop();
    m_scene->setSceneRect(0, 0, ui->m_graphView->width() - 20, ui->m_graphView->height() - 20);
}

void MyMainWindow::zoomIn()
{
    emit win_isZooming(true);
    ui->m_graphView->scale(1.1, 1.1);
}

void MyMainWindow::zoomOut()
{
    emit win_isZooming(true);
    ui->m_graphView->scale(1 / 1.1, 1 / 1.1);
}

void MyMainWindow::saveFile()
{
    QString savingPath = QFileDialog::getSaveFileName(0, "Save File", "", "BMP Files (*.bmp) ;; JPEG (*.JPEG) ;; PNG (*.png)");
    QPixmap image = ui->m_graphView->grab();
    image.save(savingPath);
}

void MyMainWindow::loadFile()
{
    QString pathToImage = QFileDialog::getOpenFileName(0, "Open File", "", "BMP Files (*.bmp) ;; JPEG (*.JPEG) ;; PNG (*.png)");
    QPixmap image(pathToImage);

    if (image.size().rheight() == ui->m_graphView->size().rheight() || image.size().rwidth() == ui->m_graphView->size().rwidth())
    {
        QSize imageSize = image.size();
        imageSize.scale(ui->m_graphView->size(), Qt::KeepAspectRatio);
        QPixmap scaledImage = image.scaled(imageSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        m_scene->addPixmap(scaledImage);
    }
    else
        m_scene->addPixmap(image);
}

void MyMainWindow::createNewCanvas()
{
    sizeDialog* newSizeDialog = new sizeDialog;
    if(newSizeDialog->exec() == QDialog::Accepted)
    {
        QString new_widht = newSizeDialog->get_widht();
        QString new_height = newSizeDialog->get_height();

        ui->m_graphView->scene()->clear();
        ui->m_graphView->resize(new_widht.toInt(), new_height.toInt());
        QRect rectScene = ui->m_graphView->rect();
        rectScene.setWidth(rectScene.width() - 20);
        rectScene.setHeight(rectScene.height() - 20);
        ui->m_graphView->scene()->setSceneRect(rectScene);
    }
    delete newSizeDialog;
}

void MyMainWindow::figureChangedLine()
{
    emit win_figureChanged(currentFigureTool::line);
}

void MyMainWindow::figureChangedCurve()
{
    emit win_figureChanged(currentFigureTool::curve);
}

void MyMainWindow::figureChangedEllipse()
{
    emit win_figureChanged(currentFigureTool::ellipse);
}

void MyMainWindow::figureChangedRectangle()
{
    emit win_figureChanged(currentFigureTool::rectangle);
}

void MyMainWindow::figureChangedPolygon()
{
    emit win_figureChanged(currentFigureTool::polygon);
}

void MyMainWindow::on_m_pbMainMenu_clicked(bool flagActivated_)
{
    if(flagActivated_)
        ui->m_hrzLayoutMain_2->show();
    else
        ui->m_hrzLayoutMain_2->hide();

}

void MyMainWindow::on_m_pbPencilTool_clicked()
{
    m_currTool = currentTool::pencil;
    emit win_toolChanged(m_currTool);
    QCursor newCursor(QPixmap(":/iconsForPaint/pencil.png").scaled(32, 32), 0,32);
    setCursor(newCursor);
}

void MyMainWindow::on_m_pbBrushTool_clicked()
{
    m_currTool = currentTool::brush;
    emit win_toolChanged(m_currTool);
    QCursor newCursor(QPixmap(":/iconsForPaint/brush.png").scaled(32, 32), 0,32);
    setCursor(newCursor);
}

void MyMainWindow::on_m_pbTextTool_clicked()
{
    m_currTool = currentTool::text;
    emit win_toolChanged(m_currTool);
    //QCursor newCursor(QPixmap(":/iconsForPaint/text.png").scaled(16, 16), 0,32);
    setCursor(Qt::IBeamCursor);
}

void MyMainWindow::on_m_pbEraserTool_clicked()
{
    m_currTool = currentTool::eraser;
    emit win_toolChanged(m_currTool);
    int newEraserSize = m_scene->getEraserSize();
    QCursor newCursor(QPixmap(":/iconsForPaint/redRectangleEraser.png").scaled(newEraserSize, newEraserSize), newEraserSize/2, newEraserSize/2);
    setCursor(newCursor);
}

void MyMainWindow::on_m_pbFigureTool_clicked()
{
    m_currTool = currentTool::figure;
    emit win_toolChanged(m_currTool);
    setCursor(Qt::ArrowCursor);
}

void MyMainWindow::on_m_pbColorTool_clicked()
{
    m_currColor = QColorDialog::getColor(QColorConstants::Blue);
    emit win_colorChanged(m_currColor);
}

void MyMainWindow::resizeEvent(QResizeEvent *event)
{
    m_timer->start(100);
    QWidget::resizeEvent(event);
}

/*
bool MyMainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::Resize)
    {
        if(obj == ui->m_graphView)
            return false;
        else
        {
            resizeEvent(static_cast<QResizeEvent*>(event));
            return true;
        }
    }
    else
    {
        return QWidget::eventFilter(obj, event);
    }
}
*/

void MyMainWindow::fileMenuFill()
{
    ui->m_pbFileMenu->setMenu(new QMenu("&File"));

    auto actCreateNewFile = new QAction(QIcon(":/iconsForPaint/createNewFile.png"), "&Create new canvas", this);
    actCreateNewFile->setShortcut(QKeySequence::New);
    connect(actCreateNewFile, SIGNAL(triggered()), this, SLOT(createNewCanvas()));

    auto actLoadFile = new QAction(QIcon(":/iconsForPaint/loadFile.png"), "&Load file", this);
    actLoadFile->setShortcut(QKeySequence::Open);
    connect(actLoadFile, SIGNAL(triggered()), this, SLOT(loadFile()));

    auto actSaveFile = new QAction(QIcon(":/iconsForPaint/saveFile.png"), "&Save file", this);
    actSaveFile->setShortcut(QKeySequence::Save);
    connect(actSaveFile, SIGNAL(triggered()), this, SLOT(saveFile()));

    ui->m_pbFileMenu->menu()->addAction(actCreateNewFile);
    ui->m_pbFileMenu->menu()->addAction(actLoadFile);
    ui->m_pbFileMenu->menu()->addAction(actSaveFile);
}

void MyMainWindow::brushButtonFill()
{
    QSlider* brushSize = new QSlider(Qt::Orientation::Horizontal);
    brushSize->setMinimum(1);
    brushSize->setMaximum(100);
    brushSize->setBaseSize(512, 10);

    QLabel* brushSizeLabel = new QLabel(QString::number(brushSize->value()));
    brushSizeLabel->setBuddy(brushSize);
    connect(brushSize, SIGNAL(valueChanged(int)), brushSizeLabel, SLOT(setNum(int)));

    QVBoxLayout* brushLayout = new QVBoxLayout();
    brushLayout->addWidget(brushSizeLabel);
    brushLayout->addWidget(brushSize);

    QWidget* brushLayoutWidget = new QWidget();
    brushLayoutWidget->setLayout(brushLayout);

    QWidgetAction* brushSizePicker = new QWidgetAction(ui->m_pbBrushTool);
    brushSizePicker->setDefaultWidget(brushLayoutWidget);
    ui->m_pbBrushTool->setMenu(new QMenu());
    ui->m_pbBrushTool->menu()->addAction(brushSizePicker);

    connect(ui->m_pbBrushTool->menu(), SIGNAL(aboutToShow()), this, SLOT(on_m_pbBrushTool_clicked()));
    connect(brushSize, SIGNAL(valueChanged(int)), m_scene, SLOT(brushSizeChanged(int)));
}

void MyMainWindow::eraserButtonFill()
{
    QSlider* eraserSize = new QSlider(Qt::Orientation::Horizontal);
    eraserSize->setMinimum(1);
    eraserSize->setMaximum(100);
    eraserSize->setBaseSize(512, 10);

    QLabel* eraserSizeLabel = new QLabel(QString::number(eraserSize->value()));
    eraserSizeLabel->setBuddy(eraserSize);
    connect(eraserSize, SIGNAL(valueChanged(int)), eraserSizeLabel, SLOT(setNum(int)));

    QVBoxLayout* eraserLayout = new QVBoxLayout();
    eraserLayout->addWidget(eraserSizeLabel);
    eraserLayout->addWidget(eraserSize);

    QWidget* eraserLayoutWidget = new QWidget();
    eraserLayoutWidget->setLayout(eraserLayout);

    QWidgetAction* eraserSizePicker = new QWidgetAction(ui->m_pbBrushTool);
    eraserSizePicker->setDefaultWidget(eraserLayoutWidget);
    ui->m_pbEraserTool->setMenu(new QMenu());
    ui->m_pbEraserTool->menu()->addAction(eraserSizePicker);

    connect(ui->m_pbEraserTool->menu(), SIGNAL(aboutToShow()), this, SLOT(on_m_pbEraserTool_clicked()));
    connect(eraserSize, SIGNAL(valueChanged(int)), m_scene, SLOT(eraserSizeChanged(int)));
}

void MyMainWindow::textButtonFill()
{
    QLineEdit* textLineEdit = new QLineEdit;
    textLineEdit->setBaseSize(512,10);
    textLineEdit->setText("Enter Your Text");

    QVBoxLayout* textLayout = new QVBoxLayout;
    textLayout->addWidget(textLineEdit);

    QWidget* textWidget = new QWidget;
    textWidget->setLayout(textLayout);

    QWidgetAction* textInput = new QWidgetAction(ui->m_pbTextTool);
    textInput->setDefaultWidget(textWidget);
    ui->m_pbTextTool->setMenu(new QMenu());
    ui->m_pbTextTool->menu()->addAction(textInput);

    connect(ui->m_pbTextTool->menu(), SIGNAL(aboutToShow()), this, SLOT(on_m_pbTextTool_clicked()));
    connect(textLineEdit, SIGNAL(textChanged(QString)), m_scene, SLOT(textChanged(QString)));
}

void MyMainWindow::figureButtonFill()
{
    ui->m_pbFigureTool->setMenu(new QMenu);
    
    auto actLine = new QAction(QIcon(":/iconsForPaint/line.png"), "&Line", this);
    connect(actLine, SIGNAL(triggered()), this, SLOT(figureChangedLine()));
    
    auto actCurve = new QAction(QIcon(":/iconsForPaint/curve.png"), "&Curve", this);
    connect(actCurve, SIGNAL(triggered()), this, SLOT(figureChangedCurve()));
    
    auto actEllipse = new QAction(QIcon(":/iconsForPaint/ellipse.png"), "&Ellipse", this);
    connect(actEllipse, SIGNAL(triggered()), this, SLOT(figureChangedEllipse()));
    
    auto actRectangle = new QAction(QIcon(":/iconsForPaint/rectangle.png"), "&Rectangle", this);
    connect(actRectangle, SIGNAL(triggered()), this, SLOT(figureChangedRectangle()));
    
    auto actPolygon = new QAction(QIcon(":/iconsForPaint/polygon.png"), "&Polygon", this);
    connect(actPolygon, SIGNAL(triggered()), this, SLOT(figureChangedPolygon()));
    
    QList<QAction*> listActions;
    listActions << actLine << actCurve << actEllipse << actRectangle << actPolygon;

    ui->m_pbFigureTool->menu()->addActions(listActions);
    connect(ui->m_pbFigureTool->menu(), SIGNAL(aboutToShow()), this, SLOT(on_m_pbFigureTool_clicked()));
}
