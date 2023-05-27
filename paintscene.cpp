#include "paintscene.h"
#include <QGraphicsItem>

paintScene::paintScene(QObject *parent) : QGraphicsScene(parent)
{

}

paintScene::~paintScene()
{

}

void paintScene::toolChanged(currentTool newTool_)
{
    m_sceneCurrTool = newTool_;
}

void paintScene::figureChanged(currentFigureTool newFigure_)
{
    m_sceneCurrFigureTool = newFigure_;
}

void paintScene::colorChanged(QColor newColor_)
{
    m_sceneCurrColor = newColor_;
}

void paintScene::brushSizeChanged(int newBrushSize_)
{
    m_brushSize = newBrushSize_;
}

void paintScene::eraserSizeChanged(int newEraserSize_)
{
    m_eraserSize = newEraserSize_;
}

void paintScene::textChanged(QString newText_)
{
    m_textTool = newText_;
}

void paintScene::isZooming(bool zoomingFlag_)
{
    m_zoomingFlag = zoomingFlag_;
}

void paintScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!m_zoomingFlag)
    {
        switch(m_sceneCurrTool)
        {
        case currentTool::noneTool:
        {
            break;
        }
        case currentTool::pencil:
        {
            paintWithPencil(false, event);
            break;
        }
        case currentTool::brush:
        {
            paintWithBrush(false, event);
            break;
        }
        case currentTool::eraser:
        {
            paintWithEraser(event);
            break;
        }
        case currentTool::figure:
        {
            paintWithFigure(false, event);
            break;
        }
        case currentTool::text:
        {
            paintWithText(event);
            break;
        }
        }
    }
    else
    {
        m_zoomingFlag = false;
    }
}

void paintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(!m_zoomingFlag)
    {
        switch(m_sceneCurrTool)
        {
        case currentTool::noneTool:
        {
            break;
        }
        case currentTool::pencil:
        {
            paintWithPencil(true, event);
            break;
        }
        case currentTool::brush:
        {
            paintWithBrush(true, event);
            break;
        }
        case currentTool::eraser:
        {
            paintWithEraser(event);
            break;
        }
        case currentTool::figure:
        {
            paintWithFigure(true, event);
            break;
        }
        case currentTool::text:
        {
            break;
        }
        }
    }
    else
    {
        m_zoomingFlag = false;
    }
}

void paintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(!m_zoomingFlag)
    {
        if(m_sceneCurrTool==currentTool::figure)
        {
            paintWithFigure(false, event);
        }
    }
    else
    {
        m_zoomingFlag = false;
    }
}

void paintScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_sceneCurrTool == currentTool::figure)
        if(m_sceneCurrFigureTool == currentFigureTool::curve || m_sceneCurrFigureTool == currentFigureTool::polygon)
            paintWithFigure(false, event);
}

void paintScene::paintWithPencil(bool movedFlag, QGraphicsSceneMouseEvent *event)
{
    if(movedFlag == false)
    {
        addEllipse(event->scenePos().x(),
                   event->scenePos().y(),
                   1,
                   1,
                   QPen(Qt::NoPen),
                   QBrush(m_sceneCurrColor));

        m_previousPoint = event->scenePos();
    }
    else
    {
        addLine(m_previousPoint.x(),
                m_previousPoint.y(),
                event->scenePos().x(),
                event->scenePos().y(),
                QPen(m_sceneCurrColor, 1, Qt::SolidLine, Qt::RoundCap));

        m_previousPoint = event->scenePos();
    }
}

void paintScene::paintWithBrush(bool movedFlag, QGraphicsSceneMouseEvent *event)
{
    if(movedFlag == false)
    {
        addEllipse(event->scenePos().x() - m_brushSize / 2,
                   event->scenePos().y() - m_brushSize / 2,
                   m_brushSize,
                   m_brushSize,
                   QPen(Qt::NoPen),
                   QBrush(m_sceneCurrColor));

        m_previousPoint = event->scenePos();
    }
    else
    {
        addLine(m_previousPoint.x(),
                m_previousPoint.y(),
                event->scenePos().x(),
                event->scenePos().y(),
                QPen(m_sceneCurrColor, m_brushSize, Qt::SolidLine, Qt::RoundCap));

        m_previousPoint = event->scenePos();
    }
}

void paintScene::paintWithFigure(bool movedFlag, QGraphicsSceneMouseEvent *event)
{
    switch(m_sceneCurrFigureTool)
    {
    case currentFigureTool::noneFigure:
    {
        break;
    }
    case currentFigureTool::line:
    {
        paintWithFigureLine(movedFlag, event);
        break;
    }
    case currentFigureTool::curve:
    {
        paintWithFigureCurve(movedFlag, event);
        break;
    }
    case currentFigureTool::ellipse:
    {
        paintWithFigureEllipse(movedFlag, event);
        break;
    }
    case currentFigureTool::rectangle:
    {
        paintWithFigureRectangle(movedFlag, event);
        break;
    }
    case currentFigureTool::polygon:
    {
        paintWithFigurePolygon(movedFlag, event);
        break;
    }
    }
}

void paintScene::paintWithFigureLine(bool movedFlag, QGraphicsSceneMouseEvent *event)
{
    if(movedFlag == false)
    {
        if(m_FigurePaintingFlag == false)
        {
            m_FigurePaintingFlag = true;
            m_previousPoint = event->scenePos();
        }
        else
        {
            m_FigurePaintingFlag = false;
            m_lastDrawn = nullptr;
            m_previousPoint = QPointF();
        }
    }
    else
    {
        if(m_lastDrawn != nullptr)
            removeItem(m_lastDrawn);

        m_lastDrawn = addLine(m_previousPoint.x(),
                              m_previousPoint.y(),
                              event->scenePos().x(),
                              event->scenePos().y(),
                              QPen(m_sceneCurrColor, 2, Qt::SolidLine, Qt::RoundCap));
    }
}

void paintScene::paintWithFigureCurve(bool movedFlag, QGraphicsSceneMouseEvent *event)
{
    if(movedFlag == false && event->type() != QEvent::GraphicsSceneMouseRelease)
    {
        if(m_FigurePaintingFlag == false)
        {
            m_FigurePaintingFlag = true;
            m_previousPoint = event->scenePos();
        }
        else if(event->type() == QEvent::GraphicsSceneMouseDoubleClick)
        {
            m_FigurePaintingFlag = false;
            m_lastDrawn = nullptr;
            //m_previousPoint = QPointF();
        }
    }
    else
    {
        if(m_lastDrawn == nullptr)
            removeItem(m_lastDrawn);

        m_lastDrawn = addLine(m_previousPoint.x(),
                              m_previousPoint.y(),
                              event->scenePos().x(),
                              event->scenePos().y(),
                              QPen(m_sceneCurrColor,2,Qt::SolidLine,Qt::RoundCap));
        m_previousPoint = event->scenePos();
    }
}

void paintScene::paintWithFigureEllipse(bool movedFlag, QGraphicsSceneMouseEvent *event)
{
    if(movedFlag == false)
    {
        if(m_FigurePaintingFlag == false)
        {
            m_FigurePaintingFlag = true;
            m_previousPoint = event->scenePos();
        }
        else
        {
            m_FigurePaintingFlag = false;
            m_lastDrawn = nullptr;
            m_previousPoint = QPointF();
        }
    }
    else
    {
        if(m_lastDrawn != nullptr)
            removeItem(m_lastDrawn);

        QPointF topLeftPoint(qMin<qreal>(m_previousPoint.x(), event->scenePos().x()),
                             qMin<qreal>(m_previousPoint.y(), event->scenePos().y()));
        QPointF bottomRightPoint(qMax<qreal>(m_previousPoint.x(), event->scenePos().x()),
                                 qMax<qreal>(m_previousPoint.y(), event->scenePos().y()));
        m_lastDrawn = addEllipse(QRectF(topLeftPoint, bottomRightPoint),
                                 QPen(m_sceneCurrColor, 3, Qt::SolidLine, Qt::RoundCap));
    }
}

void paintScene::paintWithFigureRectangle(bool movedFlag, QGraphicsSceneMouseEvent *event)
{
    if(movedFlag == false)
    {
        if(m_FigurePaintingFlag == false)
        {
            m_FigurePaintingFlag = true;
            m_previousPoint = event->scenePos();
        }
        else
        {
            m_FigurePaintingFlag = false;
            m_lastDrawn = nullptr;
            m_previousPoint = QPointF();
        }
    }
    else
    {
        if(m_lastDrawn != nullptr)
            removeItem(m_lastDrawn);

        QPointF topLeftPoint(qMin<qreal>(m_previousPoint.x(), event->scenePos().x()),
                             qMin<qreal>(m_previousPoint.y(), event->scenePos().y()));
        QPointF bottomRightPoint(qMax<qreal>(m_previousPoint.x(), event->scenePos().x()),
                                 qMax<qreal>(m_previousPoint.y(), event->scenePos().y()));
        m_lastDrawn = addRect(QRectF(topLeftPoint, bottomRightPoint),
                              QPen(m_sceneCurrColor, 3, Qt::SolidLine, Qt::RoundCap));
    }
}

void paintScene::paintWithFigurePolygon(bool movedFlag, QGraphicsSceneMouseEvent *event)
{
    if(movedFlag == false && event->type() != QEvent::GraphicsSceneMouseRelease)
    {
        if(m_FigurePaintingFlag == false)
        {
            m_FigurePaintingFlag = true;
            m_previousPoint = event->scenePos();

            m_polyPoints << event->scenePos().toPoint();
            m_lastDrawn = addPolygon(QPolygon(m_polyPoints),
                                     QPen(m_sceneCurrColor, 3, Qt::SolidLine, Qt::RoundCap));
        }
        else if(event->type() == QEvent::GraphicsSceneMouseDoubleClick)
        {
            m_FigurePaintingFlag = false;
            m_lastDrawn = nullptr;
            m_previousPoint = QPointF();
            m_polyPoints.clear();
        }
        else
        {
            if(m_lastDrawn != nullptr)
                removeItem(m_lastDrawn);

            m_polyPoints << event->scenePos().toPoint();
            m_lastDrawn = addPolygon(QPolygon(m_polyPoints),
                                     QPen(m_sceneCurrColor, 3, Qt::SolidLine, Qt::RoundCap));
        }
    }
}

void paintScene::paintWithText(QGraphicsSceneMouseEvent *event)
{
    if(!m_textTool.isNull())
    {
        QGraphicsSimpleTextItem *newTextItem = addSimpleText(m_textTool);
        newTextItem->setPos(event->scenePos());
    }
}

void paintScene::paintWithEraser(QGraphicsSceneMouseEvent *event)
{
    QPointF topLeftPoint = event->scenePos();
    topLeftPoint.setX(topLeftPoint.x() - m_eraserSize / 2);
    topLeftPoint.setY(topLeftPoint.y() - m_eraserSize / 2);

    QPointF bottomRightPoint = event->scenePos();
    bottomRightPoint.setX(bottomRightPoint.x() + m_eraserSize / 2);
    bottomRightPoint.setY(bottomRightPoint.y() + m_eraserSize / 2);

    QPolygonF erasingArea(QRectF(topLeftPoint, bottomRightPoint));

    auto listObjectsOnScene = items(erasingArea);
    for(auto it : listObjectsOnScene)
    {
        removeItem(it);
        delete it;
    }
}

int paintScene::getEraserSize()
{
    return m_eraserSize;
}
