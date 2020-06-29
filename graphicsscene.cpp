// SPDX-License-Identifier: BSD-3-Clause
#include "graphicsscene.h"
#include "edge.h"
#include "node.h"
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QLineF>
#include <QMessageBox>
#include <QDir>
#include <QtDebug>
#include <QPdfWriter>

GraphicsScene::GraphicsScene(QObject *parent)
    :QGraphicsScene::QGraphicsScene(parent)
{
    //qDebug() << "FOo";
}
GraphicsScene::~GraphicsScene()
{
}

void GraphicsScene::save()
{
    QString filter = "rico-qt cluster (*.json)";
    QDir examplePath = QDir::home();
    QString absolutePath = examplePath.absoluteFilePath("save.json");
    QString savefilename = QFileDialog::getSaveFileName(nullptr, tr("Save File"), absolutePath , filter, &filter);
    if (!savefilename.isEmpty())
    {
        QFile saveFile(savefilename);
        if (!saveFile.open(QIODevice::WriteOnly)) {
            qWarning("Couldn't open save2 file.");
        }
        QJsonObject sceneObject;
        writeJsonFromScene(sceneObject);
        saveFile.write(QJsonDocument(sceneObject).toJson());
        saveFile.close();
    }
}
void GraphicsScene::load()
{
    /* TODO */
    /* ensure that this is only run, when no nodes are defined and no edges are defined*/
    /* this means the current cluster is empty*/
    if ( edges.size()==0 && nodes.size() == 0)
    {
        QString loadfilename = QFileDialog::getOpenFileName(nullptr, tr("Open File"), QDir::homePath(), tr("qt-rico cluster (*.json)"));
        //only do something when a valid file name was returned
        if (!loadfilename.isEmpty())
        {
            QFile loadFile(loadfilename);
            if (!loadFile.open(QIODevice::ReadOnly)) {
                qWarning("Couldn't open save file.");
            }
            QByteArray loadData = loadFile.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(loadData);
            QJsonObject json=doc.object();
            int version=0;
            if (json.contains("version") && json["version"].isDouble())
                version = json["version"].toInt();
            //qDebug() << "Version:" << version;

            // Load Nodes
            QString nodesarrayname = "nodes";
            if (json.contains(nodesarrayname) && json[nodesarrayname].isArray()) {
                QJsonArray nodearray = json[nodesarrayname].toArray();
                for (int i = 0; i < nodearray.size(); ++i)
                {
                    Node *node = new Node(nodearray[i].toObject());
                    //qDebug() << node;
                    this->addItem(node);
                    nodes.push_back(node);
                }
            }
            // Load Edges
            QString edgesarrayname = "edges";
            if (json.contains(edgesarrayname) && json[edgesarrayname].isArray()) {
                QJsonArray edgearray = json[edgesarrayname].toArray();
                for (int i = 0; i < edgearray.size(); ++i)
                {
                    QJsonObject jsonedge = edgearray[i].toObject();
                    QString edgeIDname = "ID";
                    int ID;
                    if (jsonedge.contains(edgeIDname) && jsonedge[edgeIDname].isDouble())
                    {
                        ID = jsonedge[edgeIDname].toInt();
                    }
                    QString nodeAIDname = "nodeA-ID";
                    int nodeAID;
                    if (jsonedge.contains(nodeAIDname) && jsonedge[nodeAIDname].isDouble())
                    {
                        nodeAID = jsonedge[nodeAIDname].toInt();
                    }
                    QString nodeBIDname = "nodeB-ID";
                    int nodeBID;
                    if (jsonedge.contains(nodeBIDname) && jsonedge[nodeBIDname].isDouble())
                    {
                        nodeBID = jsonedge[nodeBIDname].toInt();
                    }
                    // Find nodes by ID from vector of nodes
                    std::vector<Node*>::iterator nit = nodes.begin();
                    Node *nodeA;
                    Node *nodeB;
                    while (nit != nodes.end())
                    {
                        if ((*nit)->ID == nodeAID)
                        {
                            nodeA = (*nit);
                        }
                        if ((*nit)->ID == nodeBID)
                        {
                            nodeB = (*nit);
                        }
                        ++nit;
                    }
                    Edge *edge = new Edge(nodeA, nodeB);
                    edge->ID = ID;
                    //qDebug() << edge;
                    this->addItem(edge);
                    edges.push_back(edge);
                }
            }
            loadFile.close();
        }
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("There are already nodes and/or edges present. Refusing to overwrite your work.");
        msgBox.exec();
    }
}


void GraphicsScene::writeJsonFromScene(QJsonObject &json)
{
    json["version"] = 1;
    //create array of all the nodes
    QJsonArray nodearray;
    std::vector<Node*>::iterator nit = nodes.begin();
    while (nit != nodes.end())
    {
        //get node json representation from each node
        nodearray.append((*nit)->returnJsonObj());
        ++nit;
    }
    //add node array to original QJsonObject
    json.insert(QString("nodes"),nodearray);

    //create array of all the edges
    QJsonArray edgearray;
    std::vector<Edge*>::iterator eit = edges.begin();
    while (eit != edges.end())
    {
        //get node json representation from each node
        edgearray.append((*eit)->returnJsonObj());
        ++eit;
    }
    //add node array to original QJsonObject
    json.insert(QString("edges"),edgearray);
}


void GraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
 //   qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
    QGraphicsItem * undercursor = this->itemAt(mouseEvent->scenePos(),QTransform() );
    bool ok;
    //check if we are called to modify a node
    if (undercursor != nullptr)
    {
        int itemtype = undercursor->type();
        //If it is a node
        if (itemtype==65537)
        {
            QString text = QInputDialog::getText(nullptr, tr("Modify node label"), tr("Node label:"), QLineEdit::Normal, "", &ok);
            if (ok && !text.isEmpty())
            {
                Node* node = (Node*) undercursor;
                node->nodelabel=text;
                update();
            }
        }
    }
    else
    {
        //this is called when we create a new node
        QString text = QInputDialog::getText(nullptr, tr("New node label"), tr("Node label:"), QLineEdit::Normal, "", &ok);
        if (ok && !text.isEmpty())
        {
            Node *node = new Node(mouseEvent->scenePos(), text);
            this->addItem(node);
            nodes.push_back(node);
        }
    }
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
//    qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
    QGraphicsScene::mouseMoveEvent(mouseEvent);
    update();
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    //qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
    //qDebug() << mouseEvent->button();
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
    update();
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    //qDebug() << mouseEvent->button();
    //qDebug() << mouseEvent->modifiers();
    //qDebug() << mouseEvent->flags();
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphicsScene::keyPressEvent(QKeyEvent *keyEvent)
{
    //qDebug() << keyEvent->key();
    QList<QGraphicsItem*> selList;
    //make a copy of the list
    selList.append(this->selectedItems());
    if (keyEvent->key() == Qt::Key_Delete || keyEvent->key() == Qt::Key_Backspace)
    {
        if ( selList.size() != 0)
        {
            for(int i=0;i<selList.size();i++)
            {
                qDebug()<< i << selList.size();
                QGraphicsItem * item = selList[i];
                int itemtype = item->type();
                //If it is a node
                if (itemtype==65537)
                {
                    //Look for all edges pointing to this node
                    //Delete all associated edges
                    int nodeID = ((Node*) item)->ID;
                    std::vector<Edge*>::iterator it = edges.begin();
                    while (it != edges.end())
                    {
                        if( nodeID  == (*it)->nodeA->ID || nodeID  == (*it)->nodeB->ID )
                        {
                            Edge* edge = (*it);
                            this->removeItem(edge);
                            // erase() invalidates it, using returned safe it
                            it = edges.erase(it);
                            delete edge;
                        }
                        else
                        {
                            ++it;
                        }
                    }
                    //Remove node from list of nodes
                    std::vector<Node*>::iterator nit = nodes.begin();
                    while (nit != nodes.end())
                    {
                        if( nodeID  == (*nit)->ID )
                        {
                            Node* node = (*nit);
                            this->removeItem(node);
                            // erase() invalidates it, using returned safe it
                            nit = nodes.erase(nit);
                            delete node;
                        }
                        else
                        {
                            ++nit;
                        }
                    }
                    update();
                }
                //If it is an edge
                else if (itemtype==65538)
                {
                    //lookup where it is in vector
                    std::vector<Edge*>::iterator it;
                    for(it=edges.begin();it!=edges.end();it++)
                    {
                        if( ((Edge*) item)->ID == (*it)->ID)
                        {
                            edges.erase(it);
                            this->removeItem(item);
                            delete item;
                            break;
                        }
                    }
                    update();
                }
            }
        }
    }
    if (keyEvent->key() == Qt::Key_E)
    {
        if ( selList.size() == 2)
        {
            // both are nodes/ellipsies
            Node *itemA = dynamic_cast<Node*>(selList.first());
            Node *itemB = dynamic_cast<Node*>(selList.last());
            if (itemA->type()==65537 && itemB->type()==65537)
            {
                Edge *e = new Edge(itemA, itemB);
                this->addItem(e);
                edges.push_back(e);
                update();
                this->clearSelection();
            }
        }
    }
    QGraphicsScene::keyPressEvent(keyEvent);
}

void GraphicsScene::print()
{
    QString filter = "Pdf file (*.pdf)";
    QDir examplePath = QDir::home();
    QString absolutePath = examplePath.absoluteFilePath("print.pdf");
    QString savefilename = QFileDialog::getSaveFileName(nullptr, tr("Save File"), absolutePath , filter, &filter);
    if (!savefilename.isEmpty())
    {
        QPdfWriter pdfwriter(savefilename);
        pdfwriter.setPageSize(QPageSize(QPageSize::A4));
        pdfwriter.setPageMargins(QMargins(30, 30, 30, 30));
        pdfwriter.setResolution(600);
        pdfwriter.setPageOrientation(QPageLayout::Landscape);
        QPainter painter(&pdfwriter);
        painter.setFont(QFont("Helvetica", 2));
        this->render(&painter);
        painter.end();
    }
}