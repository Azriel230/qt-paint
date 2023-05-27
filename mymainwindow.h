
#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QGraphicsView>
#include <QTimer>
#include <QResizeEvent>

#include <paintscene.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MyMainWindow; }
QT_END_NAMESPACE

class MyMainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MyMainWindow(QWidget *parent = nullptr);
    ~MyMainWindow();

public slots:
    void zoomIn();
    void zoomOut();

signals:
    void win_toolChanged(currentTool newTool_);
    void win_colorChanged(QColor newColor_);
    void win_isZooming(bool zoomingFlag_);
    void win_figureChanged(currentFigureTool newFigure_);


private slots:
    void slotTimer();

    //Work with file
    void saveFile();
    void loadFile();
    void createNewCanvas();

    //Work with figures
    void figureChangedLine();
    void figureChangedCurve();
    void figureChangedEllipse();
    void figureChangedRectangle();
    void figureChangedPolygon();

    //Buttons on winForm
    void on_m_pbMainMenu_clicked(bool flagActivated_);
    void on_m_pbPencilTool_clicked();
    void on_m_pbBrushTool_clicked();
    void on_m_pbTextTool_clicked();
    void on_m_pbEraserTool_clicked();
    void on_m_pbFigureTool_clicked();
    void on_m_pbColorTool_clicked();

private:
    Ui::MyMainWindow *ui;
    QTimer *m_timer;
    QColor m_currColor;
    currentTool m_currTool = currentTool::noneTool;
    paintScene *m_scene;

private:
    void resizeEvent(QResizeEvent *event);
    //bool eventFilter(QObject *obj, QEvent* event);
    void fileMenuFill();
    void brushButtonFill();
    void eraserButtonFill();
    void textButtonFill();
    void figureButtonFill();
};

#endif // MYMAINWINDOW_H
