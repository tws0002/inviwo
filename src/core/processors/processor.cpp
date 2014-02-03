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
 * Main file authors: Erik Sund�n, Timo Ropinski
 *
 *********************************************************************************/

#include <inviwo/core/processors/processor.h>
#include <inviwo/core/metadata/metadatafactory.h>
#include <inviwo/core/util/factory.h>

namespace inviwo {

ProcessorClassName(Processor, "Processor");
ProcessorCategory(Processor, "undefined");
ProcessorCodeState(Processor, CODE_STATE_EXPERIMENTAL);

Processor::Processor()
    : PropertyOwner(), ProcessorObservable()
    , processorWidget_(0)
    , identifier_("undefined")
    , initialized_(false) {
}

Processor::~Processor() {
    while (!metaData_.empty()) {
        delete metaData_.back();
        metaData_.pop_back();
    }
}

void Processor::addPort(Inport* port, std::string portDependencySet) {
    // TODO: check if port with same name has been added before
    port->setProcessor(this);
    inports_.push_back(port);
    portDependencySets_.insert(portDependencySet, port);
}

void Processor::addPort(Inport& port, std::string portDependencySet) {
    addPort(&port, portDependencySet);
}

void Processor::addPort(Outport* port, std::string portDependencySet) {
    // TODO: check if port with same name has been added before
    port->setProcessor(this);
    outports_.push_back(port);
    portDependencySets_.insert(portDependencySet, port);
}

void Processor::addPort(Outport& port, std::string portDependencySet) {
    addPort(&port, portDependencySet);
}

void Processor::setIdentifier(const std::string& identifier) {
    identifier_ = identifier;
}

std::string Processor::getIdentifier() const {
    return identifier_;
}

void Processor::setProcessorWidget(ProcessorWidget* processorWidget) {
    processorWidget_ = processorWidget;
}

ProcessorWidget* Processor::getProcessorWidget() const {
    return processorWidget_;
}

bool Processor::hasProcessorWidget() const {
    return (processorWidget_ != 0);
}

Port* Processor::getPort(std::string identifier) const {
    for (std::vector<Inport*>::const_iterator it = inports_.begin(); it != inports_.end(); ++it)
        if ((*it)->getIdentifier() == identifier)
            return (*it);

    for (std::vector<Outport*>::const_iterator it = outports_.begin(); it != outports_.end(); ++it)
        if ((*it)->getIdentifier() == identifier)
            return (*it);

    return NULL;
}

Inport* Processor::getInport(std::string identifier) const {
    for (std::vector<Inport*>::const_iterator it = inports_.begin(); it != inports_.end(); ++it)
        if ((*it)->getIdentifier() == identifier)
            return (*it);

    return NULL;
}

Outport* Processor::getOutport(std::string identifier) const {
    for (std::vector<Outport*>::const_iterator it = outports_.begin(); it != outports_.end(); ++it)
        if ((*it)->getIdentifier() == identifier)
            return (*it);

    return NULL;
}

const std::vector<Inport*>& Processor::getInports() const {
    return inports_;
}

const std::vector<Outport*>& Processor::getOutports() const {
    return outports_;
}

std::vector<Port*> Processor::getPortsByDependencySet(std::string portDependencySet) const {
    return portDependencySets_.getGroupedData(portDependencySet);
}

std::vector<std::string> Processor::getPortDependencySets() const {
    return portDependencySets_.getGroupKeys();
}

std::string Processor::getPortDependencySet(Port* port) const {
    return portDependencySets_.getKey(port);
}

bool Processor::allInportsConnected() const {
    for (std::vector<Inport*>::const_iterator it = inports_.begin(); it != inports_.end(); ++it)
        if (!(*it)->isConnected())
            return false;

    return true;
}

bool Processor::allInportsAreReady() const {
    for (std::vector<Inport*>::const_iterator it = inports_.begin(); it != inports_.end(); ++it)
        if (!(*it)->isReady())
            return false;

    return true;
}

void Processor::initialize() {
    for (std::vector<Inport*>::iterator it = inports_.begin(); it != inports_.end(); ++it)
        (*it)->initialize();

    for (std::vector<Outport*>::iterator it = outports_.begin(); it != outports_.end(); ++it)
        (*it)->initialize();

    initialized_ = true;
}

void Processor::deinitialize() {
    for (std::vector<Inport*>::iterator it = inports_.begin(); it != inports_.end(); ++it)
        (*it)->deinitialize();

    for (std::vector<Outport*>::iterator it = outports_.begin(); it != outports_.end(); ++it)
        (*it)->deinitialize();

    portDependencySets_.deinitialize();
    initialized_ = false;
}

bool Processor::isInitialized() const {
    return initialized_;
}

void Processor::invalidate(PropertyOwner::InvalidationLevel invalidationLevel, Property* modifiedProperty) {
    notifyObserversAboutPropertyChange(modifiedProperty);
    notifyObserversInvalidationBegin(this);
    PropertyOwner::invalidate(invalidationLevel);

    for (std::vector<Outport*>::iterator it = outports_.begin(); it != outports_.end(); ++it)
        (*it)->invalidate(PropertyOwner::INVALID_OUTPUT);

    notifyObserversInvalidationEnd(this);

    if (isEndProcessor())
        performEvaluateRequest();
}

bool Processor::isEndProcessor() const {
    return outports_.empty();
}

bool Processor::isReady() const {
    return allInportsAreReady();
}

void Processor::process() {
}


void Processor::addInteractionHandler(InteractionHandler* interactionHandler) {
    interactionHandlers_.push_back(interactionHandler);
}

void Processor::removeInteractionHandler(InteractionHandler* interactionHandler) {
    for (size_t i=0; i<interactionHandlers_.size(); i++)
        if (interactionHandlers_[i] == interactionHandler) {
            interactionHandlers_.erase(interactionHandlers_.begin()+i);
            i = interactionHandlers_.size();
        }
}

bool Processor::hasInteractionHandler() const {
    return !interactionHandlers_.empty();
}

std::vector<InteractionHandler*> Processor::getInteractionHandlers() const {
    return interactionHandlers_;
}

void Processor::invokeInteractionEvent(Event* event) {
    for (size_t i=0; i<interactionHandlers_.size(); i++)
        interactionHandlers_[i]->invokeEvent(event);
}


MetaData* Processor::getMetaData(std::string className) {
    MetaData* meta = 0;

    for (size_t i=0; i<metaData_.size(); i++) {
        if (metaData_[i]->getClassName()==className) {
            meta = metaData_[i];
            break;
        }
    }

    if (!meta) {
        meta = dynamic_cast<MetaData*>(MetaDataFactory::getRef().create(className));
        metaData_.push_back(meta);
    }

    return meta;
}

void Processor::serialize(IvwSerializer& s) const {
    s.serialize("type", getClassName(), true);
    s.serialize("identifier", identifier_, true);
    s.serialize("MetaDataList", metaData_, "MetaData") ;

    if (interactionHandlers_.size() != 0)
        s.serialize("InteractonHandlers", interactionHandlers_, "InteractionHandler");

    PropertyOwner::serialize(s);
}

void Processor::deserialize(IvwDeserializer& d) {
    std::string className;
    d.deserialize("type", className, true);
    d.deserialize("identifier", identifier_, true);
    d.deserialize("MetaDataList", metaData_, "MetaData") ;

    if (interactionHandlers_.size() != 0)
        d.deserialize("InteractonHandlers", interactionHandlers_, "InteractionHandler");

    PropertyOwner::deserialize(d);
}

void Processor::setValid() {
    PropertyOwner::setValid();

    for (std::vector<Outport*>::iterator it = outports_.begin(); it != outports_.end(); ++it)
        (*it)->setInvalidationLevel(VALID);
}

void Processor::performEvaluateRequest() {
    notifyObserversRequestEvaluate(this);
}

} // namespace
