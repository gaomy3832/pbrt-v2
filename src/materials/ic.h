#if defined(_MSC_VER)
#pragma once
#endif

#ifndef PBRT_MATERIALS_IC_H
#define PBRT_MATERIALS_IC_H

// materials/plastic.h*
#include "pbrt.h"
#include "material.h"

// PlasticMaterial Declarations
class IcMaterial : public Material {
public:
    // PlasticMaterial Public Methods
IcMaterial(Reference<Texture<Spectrum> > kd,
	   Reference<Texture<Spectrum> > ks,
	   Reference<Texture<Spectrum> > r,
	   Reference<Texture<float> > d,
	   Reference<Texture<float> > a,
	   Reference<Texture<float> > deg,
	   Reference<Texture<float> > h
    )
    : Kd(kd), Ks(ks), R(r), D(d), A(a), H(h), Deg(deg) {
    }
    BSDF *GetBSDF(const DifferentialGeometry &dgGeom,
                  const DifferentialGeometry &dgShading,
                  MemoryArena &arena) const;
private:
    // PlasticMaterial Private Data
    Reference<Texture<Spectrum> > Kd, Ks, R;
    Reference<Texture<float> > D, A, H, Deg;
};


IcMaterial *CreateIcMaterial(const Transform &xform,
			     const TextureParams &mp);

#endif // PBRT_MATERIALS_IC_H
