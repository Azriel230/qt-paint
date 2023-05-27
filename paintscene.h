#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <QDebug>

enum class currentTool
{
    noneTool,
    pencil,
    brush,
    eraser,
    figure,
    text
};

enum class currentFigureTool
{
    noneFigure,
    line,
    curve,
    ellipse,
    rectangle,
    polygon
};

class paintScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit paintScene(QObject *parent = nullptr);
    ~paintScene();
    int getEraserSize();

public slots:
    void toolChanged(currentTool newTool_);
    void figureChanged(currentFigureTool newFigure_);
    void colorChanged(QColor color_);
    void brushSizeChanged(int brushSize_);
    void eraserSizeChanged(int eraserSize_);
    void textChanged(QString newText_);
    void isZooming(bool zoomingFlag_);

private:
    QPointF m_previousPoint;
    QColor m_sceneCurrColor;
    currentTool m_sceneCurrTool = currentTool::noneTool;
    currentFigureTool m_sceneCurrFigureTool = currentFigureTool::noneFigure;
    int m_brushSize = 1;
    int m_eraserSize = 1;
    QString m_textTool = "Enter your text";
    bool m_zoomingFlag;
    bool m_FigurePaintingFlag = false;

    QList<QPoint> m_polyPoints;
    QGraphicsItem* m_lastDrawn = nullptr;
    QGraphicsPolygonItem* m_lastPolygon = nullptr;

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    void paintWithPencil(bool movedFlag, QGraphicsSceneMouseEvent *event);
    void paintWithBrush(bool movedFlag, QGraphicsSceneMouseEvent *event);
    void paintWithFigure(bool movedFlag, QGraphicsSceneMouseEvent *event);
    void paintWithFigureLine(bool movedFlag, QGraphicsSceneMouseEvent *event);
    void paintWithFigureCurve(bool movedFlag, QGraphicsSceneMouseEvent *event);
    void paintWithFigureEllipse(bool movedFlag, QGraphicsSceneMouseEvent *event);
    void paintWithFigureRectangle(bool movedFlag, QGraphicsSceneMouseEvent *event);
    void paintWithFigurePolygon(bool movedFlag, QGraphicsSceneMouseEvent *event);
    void paintWithText(QGraphicsSceneMouseEvent *event);
    void paintWithEraser(QGraphicsSceneMouseEvent *event);
};



#endif // PAINTSCENE_H
