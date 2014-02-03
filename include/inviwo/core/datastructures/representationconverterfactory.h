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

#ifndef IVW_REPRESENTATIONCONVERTERFACTORY_H
#define IVW_REPRESENTATIONCONVERTERFACTORY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/representationconverter.h>
#include <inviwo/core/util/factory.h>
#include <inviwo/core/util/singleton.h>

namespace inviwo {

class IVW_CORE_API RepresentationConverterFactory : public Singleton<RepresentationConverterFactory>  {

public:
    RepresentationConverterFactory();
    virtual ~RepresentationConverterFactory();

    void registerObject(RepresentationConverter* representationConverter);

    template <typename T>
    RepresentationConverter* getRepresentationConverter(DataRepresentation* source) {
        // TODO: optimize performance, e.g., by using a hash table
        for (size_t i=0; i<representationConverters_.size(); i++) {
            RepresentationConverterType<T>* repConverterTyped = dynamic_cast<RepresentationConverterType<T>*>(representationConverters_[i]);

            if (repConverterTyped) {
                if (repConverterTyped->canConvertFrom(source))
                    return representationConverters_[i];
            }
        }

        return NULL;
    }

    template <typename T>
    RepresentationConverterPackage<T>* getRepresentationConverterPackage(DataRepresentation* source) {
        // TODO: optimize performance, e.g., by using a hash table
        RepresentationConverterPackage<T>* currentConverterPackage = NULL;

        for (size_t i=0; i<representationConverters_.size(); i++) {
            RepresentationConverterPackage<T>* repConverterPackage = dynamic_cast<RepresentationConverterPackage<T>*>(representationConverters_[i]);

            if (repConverterPackage) {
                if (repConverterPackage->canConvertFrom(source)) {
                    if (currentConverterPackage)
                        currentConverterPackage = (repConverterPackage->getNumberOfConverters() < currentConverterPackage->getNumberOfConverters() ?
                                                   repConverterPackage : currentConverterPackage);
                    else
                        currentConverterPackage = repConverterPackage;
                }
            }
        }

        return currentConverterPackage;
    }

private:
    std::vector<RepresentationConverter*> representationConverters_;
};

} // namespace

#endif // IVW_REPRESENTATIONCONVERTERFACTORY_H
