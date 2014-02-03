 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
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
 * Main file authors: Daniel J�nsson, Timo Ropinski
 *
 *********************************************************************************/

#include <inviwo/core/ports/multiinport.h>

namespace inviwo {

MultiInport::MultiInport(std::string identifier)
    : Inport(identifier), inports_(new InportSet()) {
}

void MultiInport::setProcessorHelper(Port* port, Processor* processor) {
    port->setProcessor(processor);
}

MultiInport::~MultiInport() {
    InportSet::iterator it = inports_->begin();
    InportSet::iterator endIt = inports_->end();

    for (; it != endIt; ++it)
        delete *it;

    delete inports_;
    inports_ = NULL;
}

PropertyOwner::InvalidationLevel MultiInport::getInvalidationLevel() const {
    InportSet::const_iterator it = inports_->begin();
    InportSet::const_iterator endIt = inports_->end();
    PropertyOwner::InvalidationLevel maxInvalidationLevel(PropertyOwner::VALID);

    for (; it != endIt; ++it)
        maxInvalidationLevel = std::max(maxInvalidationLevel, (*it)->getInvalidationLevel());

    return maxInvalidationLevel;
}

void MultiInport::setInvalidationLevel(PropertyOwner::InvalidationLevel invalidationLevel) {
    InportSet::iterator it = inports_->begin();
    InportSet::iterator endIt = inports_->end();

    for (; it != endIt; ++it)
        (*it)->setInvalidationLevel(invalidationLevel);
}

bool MultiInport::isConnectedTo(Outport* outport) const {
    InportSet::const_iterator it = inports_->begin();
    InportSet::const_iterator endIt = inports_->end();

    for (; it != endIt; ++it) {
        if ((*it)->isConnectedTo(outport))
            return true;
    }

    return false;
}




std::vector<Outport*> MultiInport::getConnectedOutports() const {
    InportSet::const_iterator it = inports_->begin();
    InportSet::const_iterator endIt = inports_->end();
    std::vector<Outport*> connectedOutports;

    for (; it != endIt; ++it) {
        std::vector<Outport*> inportConnectedOutports = (*it)->getConnectedOutports();
        std::vector<Outport*>::const_iterator outportIt = inportConnectedOutports.begin();
        std::vector<Outport*>::const_iterator outportEndIt = inportConnectedOutports.end();

        for (; outportIt != outportEndIt; ++outportIt)
            connectedOutports.push_back(*outportIt);
    }

    return connectedOutports;
}

Outport* MultiInport::getConnectedOutport() const {
    if (isConnected())
        return (*(inports_->begin()))->getConnectedOutport();
    else
        return NULL;
}

void MultiInport::invalidate(PropertyOwner::InvalidationLevel invalidationLevel) {
    InportSet::iterator it = inports_->begin();
    InportSet::iterator endIt = inports_->end();

    for (; it != endIt; ++it)
        (*it)->invalidate(invalidationLevel);
}

} // namespace
