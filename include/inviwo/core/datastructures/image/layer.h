 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2014 Inviwo Foundation
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
 * Main file author: Erik Sund�n
 *
 *********************************************************************************/

#ifndef IVW_LAYER_H
#define IVW_LAYER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/data.h>
#include <inviwo/core/datastructures/spatialdata.h>
#include <inviwo/core/datastructures/image/imagetypes.h>

namespace inviwo {

class LayerRepresentation;

class IVW_CORE_API Layer : public Data, public StructuredGridEntity<2> {
public:
    Layer(uvec2 dimensions = uvec2(256,256), const DataFormatBase* format = DataVec4UINT8::get(), LayerType type = COLOR_LAYER);
    Layer(LayerRepresentation*);
    Layer(const Layer&);
    Layer& operator=(const Layer& that);
    virtual Layer* clone() const;
    virtual ~Layer();


    void resize(uvec2 dimensions);

    void resizeRepresentations(Layer* targetLayer, uvec2 targetDim);

    uvec2 getDimension() const;
    void setDimension(const uvec2& dim);

    LayerType getLayerType() const;

protected:
    virtual DataRepresentation* createDefaultRepresentation();

private:
    LayerType layerType_;

};

} // namespace

#endif // IVW_LAYER_H
