 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Main file authors: Timo Ropinski, Sathish Kottravel
 *
 *********************************************************************************/

#include <inviwo/qt/widgets/processors/processorwidgetqt.h>

#include <QApplication>
#include <QDesktopWidget>
#include <QResizeEvent>
#include <QMoveEvent>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

ProcessorWidgetQt::ProcessorWidgetQt()
    : QWidget(NULL), ProcessorWidget()
{
    QWidget::move(0, 0);
    QWidget::resize(32, 32);
    QWidget::setVisible(false);
}

ProcessorWidgetQt::~ProcessorWidgetQt() {}

void ProcessorWidgetQt::initialize() {
    ProcessorWidget::initialize();
    ivec2 pos = ProcessorWidget::getPositionMetaData();
    // check if geometry is on screen and alter otherwise
    QDesktopWidget* desktop = QApplication::desktop();
    QRect wholeScreenGeometry = desktop->screenGeometry(0);

    for (int i=1; i<desktop->screenCount(); i++)
        wholeScreenGeometry = wholeScreenGeometry.united(desktop->screenGeometry(i));

    wholeScreenGeometry.setRect(wholeScreenGeometry.x()-10, wholeScreenGeometry.y()-10,
                                wholeScreenGeometry.width()+20, wholeScreenGeometry.height()+20);
    QPoint bottomRight = QPoint(pos.x+this->width(), pos.y+this->height());

    if (!wholeScreenGeometry.contains(QPoint(pos.x, pos.y)) || !wholeScreenGeometry.contains(bottomRight))
        QWidget::move(QPoint(0,0));
    else
        QWidget::move(pos.x, pos.y);
}

void ProcessorWidgetQt::deinitialize() {
    ProcessorWidget::deinitialize();
}

void ProcessorWidgetQt::setVisible(bool visible) {
    ProcessorWidget::setVisible(visible);
    QWidget::setVisible(visible);
}

void ProcessorWidgetQt::show() {
    ProcessorWidget::show();
    QWidget::show();
}

void ProcessorWidgetQt::hide() {
    ProcessorWidget::hide();
    QWidget::hide();
}

void ProcessorWidgetQt::move(ivec2 pos) {
    ProcessorWidget::move(pos);
    QWidget::move(pos.x, pos.y);
}

void ProcessorWidgetQt::setDimension(ivec2 dimensions) {
    ProcessorWidget::setDimension(dimensions);
    QWidget::resize(dimensions.x, dimensions.y);
}

void ProcessorWidgetQt::resizeEvent(QResizeEvent* event) {
    ProcessorWidget::setDimension(ivec2(event->size().width(), event->size().height()));
    QWidget::resizeEvent(event);
}

void ProcessorWidgetQt::showEvent(QShowEvent* event) {
    ProcessorWidget::show();
}

void ProcessorWidgetQt::closeEvent(QCloseEvent* event) {
    ProcessorWidget::hide();
}

void ProcessorWidgetQt::moveEvent(QMoveEvent* event) {
    ProcessorWidget::move(ivec2(event->pos().x(), event->pos().y()));
    QWidget::moveEvent(event);
}

} // namespace
