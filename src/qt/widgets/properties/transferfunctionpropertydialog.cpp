/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Link�ping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Viktor Axelsson
 *
 **********************************************************************/

#include <inviwo/qt/widgets/properties/transferfunctionpropertydialog.h>
#include <QFileDialog>

namespace inviwo {

TransferFunctionPropertyDialog::TransferFunctionPropertyDialog(TransferFunctionProperty* tfProperty, QWidget* parent)
    : InviwoDockWidget(tr("Transfer Function"), parent)
    , tfProperty_(tfProperty)
    , tfPixmap_(NULL)
{
    setObjectName("Transfer Function");
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    generateWidget();

    if(!tfProperty_->getVolumeInport()){
        chkShowHistogram_->setVisible(false);
    }

    gradient_ = new QLinearGradient(0,0,100,20);
    updateTFPreview();

    updateFromProperty();
}

TransferFunctionPropertyDialog::~TransferFunctionPropertyDialog() {
    delete tfPixmap_;
    delete tfEditor_;
    delete colorWheel_;
    delete gradient_;
    delete colorDialog_;
}

void TransferFunctionPropertyDialog::generateWidget() {
    vec2 minEditorDims = vec2(255.0f, 100.0f);
    arrayWidth_ = 256; //TODO: set based on data size bit depth
    arrayHeight_ = 256;

    tfEditorView_ = new TransferFunctionEditorView(tfProperty_);
    //VoidObserver::addObservation(tfProperty_);
    tfProperty_->addObserver(tfEditorView_);
    // put origin to bottom left corner
    tfEditorView_->scale(1.0, -1.0);
    tfEditorView_->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    tfEditorView_->setMinimumSize(minEditorDims.x, minEditorDims.y);
    tfEditorView_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tfEditorView_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    tfEditor_ = new TransferFunctionEditor(&tfProperty_->get(), tfEditorView_);
    connect(tfEditor_, SIGNAL(doubleClick()), this, SLOT(showColorDialog()));
    connect(tfEditor_, SIGNAL(selectionChanged()), this, SLOT(updateColorWheel()));
    connect(tfEditor_, SIGNAL(controlPointsChanged()), this, SLOT(updateTransferFunction()));
    tfEditorView_->setScene(tfEditor_);

    zoomVSlider_ = new RangeSliderQt(Qt::Vertical, this);
    zoomVSlider_->setRange(0, 100);
    zoomVSlider_->setValue(static_cast<int>(tfProperty_->getZoomV().x*100), static_cast<int>(tfProperty_->getZoomV().y*100));
    connect(zoomVSlider_, SIGNAL(valuesChanged(int, int)), tfEditorView_, SLOT(zoomVertically(int, int)));

    zoomHSlider_ = new RangeSliderQt(Qt::Horizontal, this);
    zoomHSlider_->setRange(0, 100);
    zoomHSlider_->setValue(static_cast<int>(tfProperty_->getZoomH().x*100), static_cast<int>(tfProperty_->getZoomH().y*100));
    connect(zoomHSlider_, SIGNAL(valuesChanged(int, int)), tfEditorView_, SLOT(zoomHorizontally(int, int)));

    maskSlider_ = new RangeSliderQt(Qt::Horizontal, this);
    maskSlider_->setRange(0, 100);
    maskSlider_->setValue(static_cast<int>(tfProperty_->getMask().x*100), static_cast<int>(tfProperty_->getMask().y*100));
    connect(maskSlider_, SIGNAL(valuesChanged(int, int)), this, SLOT(changeMask(int, int)));

    colorChange_ = false;
    colorWheel_ = new ColorWheel();
    connect(colorWheel_, SIGNAL(colorChange(QColor)), this, SLOT(setPointColor()));

    btnClearTF_ = new QPushButton("Reset transfer function");
    connect(btnClearTF_, SIGNAL(clicked()), tfEditor_, SLOT(resetTransferFunction()));

    btnImportTF_ = new QPushButton("Import transfer function");
    connect(btnImportTF_, SIGNAL(clicked()), this, SLOT(importTransferFunction()));

    btnExportTF_ = new QPushButton("Export transfer function");
    connect(btnExportTF_, SIGNAL(clicked()), this, SLOT(exportTransferFunction()));

    colorDialog_ = new QColorDialog();
    colorDialog_->setWindowFlags(Qt::WindowStaysOnTopHint);
    connect(colorDialog_, SIGNAL(currentColorChanged(QColor)), this, SLOT(setPointColorDialog()));

    tfPreview_ = new QLabel();
    tfPreview_->setMinimumSize(1,1);

    cmbInterpolation_ = new QComboBox();
    cmbInterpolation_->addItem("linear interpolation");
    cmbInterpolation_->addItem("cubic interpolation");
    cmbInterpolation_->setCurrentIndex(tfProperty_->get().getInterpolationType());
    connect(cmbInterpolation_, SIGNAL(currentIndexChanged(int)), this, SLOT(switchInterpolationType(int)));

    chkShowHistogram_ = new QCheckBox("Show Histogram");
    chkShowHistogram_->setChecked(tfProperty_->getShowHistogram());
    connect(chkShowHistogram_, SIGNAL(toggled(bool)), this, SLOT(showHistogram(bool)));

    QFrame* leftPanel = new QFrame(this);
    QGridLayout* leftLayout = new QGridLayout();
    leftLayout->addWidget(zoomVSlider_,  0, 0);
    leftLayout->addWidget(tfEditorView_, 0, 1);
    leftLayout->addWidget(zoomHSlider_,  1, 1);
    leftLayout->addWidget(tfPreview_,    2, 1);
    leftLayout->addWidget(maskSlider_,   3, 1);
    leftPanel->setLayout(leftLayout);

    QFrame* rightPanel = new QFrame(this);
    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->setAlignment(Qt::AlignTop);
    rightLayout->addWidget(colorWheel_);
    rightLayout->addWidget(btnClearTF_);
    rightLayout->addWidget(btnImportTF_);
    rightLayout->addWidget(btnExportTF_);
    rightLayout->addWidget(cmbInterpolation_);
    rightLayout->addWidget(chkShowHistogram_);
    rightPanel->setLayout(rightLayout);

    QFrame* mainPanel = new QFrame(this);
    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->addWidget(leftPanel);
    mainLayout->addWidget(rightPanel);
    mainPanel->setLayout(mainLayout);
    setWidget(mainPanel);
}

void TransferFunctionPropertyDialog::switchInterpolationType(int interpolationType) {
    if (interpolationType == 0) tfProperty_->get().setInterpolationType(TransferFunction::InterpolationLinear);
    else tfProperty_->get().setInterpolationType(TransferFunction::InterpolationCubic);
    tfEditor_->redrawConnections();
}

void TransferFunctionPropertyDialog::updateTFPreview() {
    int gradientWidth = tfPreview_->width();
    gradient_->setFinalStop(gradientWidth, 0);
    if(tfPixmap_)
        delete tfPixmap_;
    tfPixmap_ = new QPixmap(gradientWidth, 20);
    QPainter tfPainter(tfPixmap_);
    tfPainter.fillRect(0, 0, gradientWidth, 20, *gradient_);
    // draw masking indicators
    if (tfProperty_->getMask().x > 0.0f) {
        tfPainter.fillRect(0, 0, static_cast<int>(tfProperty_->getMask().x*gradientWidth), 20, QColor(25,25,25,100));
        tfPainter.drawLine(static_cast<int>(tfProperty_->getMask().x*gradientWidth), 0, static_cast<int>(tfProperty_->getMask().x*gradientWidth), 20);
    }
    if (tfProperty_->getMask().y < 1.0f) {
        tfPainter.fillRect(static_cast<int>(tfProperty_->getMask().y*gradientWidth), 0, 
                           static_cast<int>(gradientWidth-(tfProperty_->getMask().y*gradientWidth)+10), 20, QColor(25,25,25,150));
        tfPainter.drawLine(static_cast<int>(tfProperty_->getMask().y*gradientWidth), 0, static_cast<int>(tfProperty_->getMask().y*gradientWidth), 20);
    }
    tfPreview_->setPixmap(*tfPixmap_);
}

void TransferFunctionPropertyDialog::updateFromProperty() {
    TransferFunction& transFunc = tfProperty_->get();
    QVector<QGradientStop> gradientStops;
    for (size_t i=0; i<transFunc.getNumDataPoints(); i++) {
        TransferFunctionDataPoint* curPoint = transFunc.getPoint(static_cast<int>(i));
        vec4 curColor = curPoint->getRGBA();
        gradientStops.append(QGradientStop(curPoint->getPos().x,
                                           QColor::fromRgbF(curColor.r, curColor.g, curColor.b, 1.0)));
    }
    gradient_->setStops(gradientStops);
    updateTFPreview();
}


void TransferFunctionPropertyDialog::updateColorWheel() {
    QList<QGraphicsItem *> selection = tfEditor_->selectedItems();
    if (selection.size()== 1 && dynamic_cast<TransferFunctionEditorControlPoint*>(selection.at(0))) {
        vec4 pointColor = dynamic_cast<TransferFunctionEditorControlPoint*>(selection.at(0))->getPoint()->getRGBA();
        colorWheel_->setColor(QColor(pointColor.r*255.0, pointColor.g*255.0, pointColor.b*255.0, pointColor.a*255.0));
        if (!colorChange_)
            colorDialog_->setCurrentColor(QColor(pointColor.r*255.0, pointColor.g*255.0, pointColor.b*255.0, pointColor.a*255.0));
    }
}

void TransferFunctionPropertyDialog::showColorDialog() {
    QList<QGraphicsItem *> selection = tfEditor_->selectedItems();
    if (selection.size()==1 && dynamic_cast<TransferFunctionEditorControlPoint*>(selection.at(0)))
        colorDialog_->show();
}


void TransferFunctionPropertyDialog::setPointColor() {
    QColor color = colorWheel_->color();
    setPointColor(color);
}

void TransferFunctionPropertyDialog::setPointColorDialog() {
    QColor color = colorDialog_->currentColor();
    setPointColor(color);
    colorChange_ = true;
    updateColorWheel();
    colorChange_ = false;
}

void TransferFunctionPropertyDialog::setPointColor(QColor color) {
    QList<QGraphicsItem *> selection = tfEditor_->selectedItems();
    for (int i=0; i< selection.size(); i++) {
        if (dynamic_cast<TransferFunctionEditorControlPoint*>(selection.at(i))) {
            vec3 newRgb = vec3(color.redF(),color.greenF(),color.blueF());
            dynamic_cast<TransferFunctionEditorControlPoint*>(selection.at(i))->getPoint()->setRGB(newRgb);
        }
    }

    updateFromProperty();
    tfEditorView_->update();
    tfProperty_->get().calcTransferValues();
    tfProperty_->propertyModified();
}

void TransferFunctionPropertyDialog::updateTransferFunction() {
    tfProperty_->get().calcTransferValues();
    updateFromProperty();
    tfProperty_->propertyModified();
}

void TransferFunctionPropertyDialog::changeMask(int maskMin, int maskMax) {
    float maskMinF = static_cast<float>(maskMin)/100.0;
    float maskMaxF = static_cast<float>(maskMax)/100.0;
    tfProperty_->setMask(maskMinF, maskMaxF);
    tfEditorView_->setMask(maskMinF, maskMaxF);
    updateTFPreview();
    tfEditorView_->update();
    tfProperty_->get().calcTransferValues();
    tfProperty_->propertyModified();
}

void TransferFunctionPropertyDialog::importTransferFunction() {
    QFileDialog importFileDialog(this, QString::fromStdString("Import transfer function"));
    importFileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    importFileDialog.setFileMode(QFileDialog::ExistingFile);
    importFileDialog.setNameFilter("*.itf");
    if (importFileDialog.exec()) {
        QString file = importFileDialog.selectedFiles().at(0);
        IvwDeserializer deserializer(file.toLocal8Bit().constData());
        tfProperty_->deserialize(deserializer);
        tfEditor_->recalculateControlPoints();
        updateFromProperty();
    }
}

void TransferFunctionPropertyDialog::exportTransferFunction() {
    QFileDialog exportFileDialog(this, QString::fromStdString("Export transfer function"));
    exportFileDialog.setAcceptMode(QFileDialog::AcceptSave);
    exportFileDialog.setFileMode(QFileDialog::AnyFile);
    exportFileDialog.setNameFilter("*.itf");
    if (exportFileDialog.exec()) {
        std::string file = exportFileDialog.selectedFiles().at(0).toStdString();
        std::string extension = URLParser::getFileExtension(file);
        if (extension == "") file.append(".itf");
        else if (extension != "itf") URLParser::replaceFileExtension(file, "itf");
        IvwSerializer serializer(file);
        tfProperty_->serialize(serializer);
        serializer.writeFile();
    }
}

void TransferFunctionPropertyDialog::showHistogram(bool show){
    tfProperty_->setShowHistogram(show);
    tfEditorView_->setShowHistogram(show);
}

} // namespace