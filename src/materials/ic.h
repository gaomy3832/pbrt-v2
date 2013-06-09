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
	   Reference<Texture<Spectrum> > r,
	   Reference<Texture<float> > d,
	   Reference<Texture<float> > deg,
	   Reference<Texture<float> > h
    )
    : Kd(kd), R(r), D(d), H(h), Deg(deg) {
	printf("Debug: construct IC material.\n");
    }
    BSDF *GetBSDF(const DifferentialGeometry &dgGeom,
                  const DifferentialGeometry &dgShading,
                  MemoryArena &arena) const;
private:
    // PlasticMaterial Private Data
    Reference<Texture<Spectrum> > Kd, R;
    Reference<Texture<float> > D, H, Deg;
};


IcMaterial *CreateIcMaterial(const Transform &xform,
			     const TextureParams &mp);

#endif // PBRT_MATERIALS_IC_H
