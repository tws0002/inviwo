#ifndef IVW_ATTRIBUTES_H
#define IVW_ATTRIBUTES_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <vector>

namespace inviwo {

enum AttributeType{
    COLOR,
    CURVATURE,
    INDEX,
    NORMAL,
    POSITION,
    TEXCOORD
};

class IVW_CORE_API AttributesBase {
public:
    AttributesBase(){}
    virtual ~AttributesBase(){}

    virtual const void* getAttributes() const = 0;  
    virtual AttributeType getAttributeType() const = 0;
    virtual unsigned int getElementSize() const = 0;
    virtual unsigned int getNumberOfAttributes() const = 0;
    virtual unsigned int getDataSize() const = 0;
};

template<typename T, size_t B, AttributeType A>
class IVW_CORE_API Attributes : public AttributesBase {

public:
    Attributes() : AttributesBase() {}
    virtual ~Attributes(){}

    void add(T&);
    
    const void* getAttributes() const;
    AttributeType getAttributeType() const;
    unsigned int getElementSize() const;
    unsigned int getNumberOfAttributes() const;
    unsigned int getDataSize() const;
    
private:
    std::vector<T> attributes_;
};

template<typename T, size_t B, AttributeType A>
void Attributes<T,B,A>::add(T& v){
    attributes_.push_back(v);
}

template<typename T, size_t B, AttributeType A>
const void* Attributes<T,B,A>::getAttributes() const{
    return static_cast<const void*>(&attributes_[0]);
}

template<typename T, size_t B, AttributeType A>
AttributeType Attributes<T,B,A>::getAttributeType() const{
    return A;
}

template<typename T, size_t B, AttributeType A>
unsigned int Attributes<T,B,A>::getElementSize() const{
    return sizeof(T);
}

template<typename T, size_t B, AttributeType A>
unsigned int Attributes<T,B,A>::getNumberOfAttributes() const{
    return attributes_.size();
}

template<typename T, size_t B, AttributeType A>
unsigned int Attributes<T,B,A>::getDataSize() const{
    return getElementSize()*getNumberOfAttributes();
}

#define DataFormatAttribute(D, A) Attributes<D::type, D::bits, A>

typedef DataFormatAttribute(DataVec4FLOAT32, COLOR) ColorAttributes;
typedef DataFormatAttribute(DataFLOAT32, CURVATURE) CurvatureAttributes;
typedef DataFormatAttribute(DataUINT32, INDEX) IndexAttributes;
typedef DataFormatAttribute(DataVec3FLOAT32, NORMAL) NormalAttributes;
typedef DataFormatAttribute(DataVec3FLOAT32, POSITION) PositionAttributes;
typedef DataFormatAttribute(DataVec3FLOAT32, TEXCOORD) TexCoordAttributes;

} // namespace

#endif // IVW_ATTRIBUTES_H
