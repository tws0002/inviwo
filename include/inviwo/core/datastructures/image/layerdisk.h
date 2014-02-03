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

#ifndef IVW_LAYERDISK_H
#define IVW_LAYERDISK_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/diskrepresentation.h>
#include <inviwo/core/datastructures/image/layerrepresentation.h>

namespace inviwo {

class IVW_CORE_API LayerDisk : public LayerRepresentation, public DiskRepresentation {

public:
    LayerDisk(LayerType type = COLOR_LAYER);
    LayerDisk(std::string url, LayerType type = COLOR_LAYER);
    LayerDisk(const LayerDisk& rhs);
    LayerDisk& operator=(const LayerDisk& that);
    virtual LayerDisk* clone() const;
    virtual ~LayerDisk();
    virtual void initialize();
    virtual void deinitialize();
    virtual std::string getClassName() const;
    virtual bool copyAndResizeLayer(DataRepresentation*) const;
    virtual void resize(uvec2);
    /**
     * \brief loads data from url.
     *
     * @param void* the destination of the raw data
     * @param uvec2 dimension of the loaded image
     * @param DataFormatId the loaded format
     *
     */
    void* loadFileData(void*, uvec2&, DataFormatId&) const;
    /**
     * \brief loads and rescales data from url.
     *
     * @param void* the destination of the raw data
     * @param uvec2 dimension to scale image to
     * @param DataFormatId the loaded format
     *
     */
    void* loadFileDataAndRescale(void*, uvec2, DataFormatId&) const;
};

} // namespace

#endif // IVW_LAYERDISK_H
