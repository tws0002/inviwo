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
 * Main file authors: Erik Sund�n, Peter Steneteg
 *
 *********************************************************************************/

#include <inviwo/core/util/formats.h>
#include <inviwo/core/datastructures/datarepresentation.h>

namespace inviwo {

DataRepresentation::DataRepresentation()
    : dataFormatBase_(DataUINT8::get()) {}
DataRepresentation::DataRepresentation(const DataFormatBase* format)
    : dataFormatBase_(format) {}
DataRepresentation::DataRepresentation(const DataRepresentation& rhs)
    : dataFormatBase_(rhs.dataFormatBase_) {}

DataRepresentation& DataRepresentation::operator=(const DataRepresentation& that) {
    if (this != &that)
        dataFormatBase_ = that.dataFormatBase_;

    return *this;
}

DataRepresentation::~DataRepresentation() { }

std::string DataRepresentation::getClassName() const {
    return "DataRepresentation";
}

const DataFormatBase* DataRepresentation::getDataFormat() const {
    return dataFormatBase_;
}

std::string DataRepresentation::getDataFormatString() const {
    return std::string(dataFormatBase_->getString());
}

DataFormatId DataRepresentation::getDataFormatId() const {
    return dataFormatBase_->getId();
}

void DataRepresentation::setDataFormat(const DataFormatBase* format) {
    dataFormatBase_ = format;
}





} // namespace
