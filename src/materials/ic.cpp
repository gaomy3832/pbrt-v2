
/*
    pbrt source code Copyright(c) 1998-2012 Matt Pharr and Greg Humphreys.

    This file is part of pbrt.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */


// materials/plastic.cpp*
#include "stdafx.h"
#include "materials/ic.h"
#include "spectrum.h"
#include "reflection.h"
#include "paramset.h"
#include "texture.h"

// PlasticMaterial Method Definitions
BSDF *IcMaterial::GetBSDF(const DifferentialGeometry &dgGeom,
                               const DifferentialGeometry &dgShading,
                               MemoryArena &arena) const {
    // Allocate _BSDF_, possibly doing bump mapping with _bumpMap_
    DifferentialGeometry dgs;
    dgs = dgShading;
    BSDF *bsdf = BSDF_ALLOC(arena, BSDF)(dgs, dgGeom.nn);
    Spectrum kd = Kd->Evaluate(dgs).Clamp();
    if (!kd.IsBlack()) {
        BxDF *diff = BSDF_ALLOC(arena, Lambertian)(kd);
        bsdf->Add(diff);
    }
    Spectrum r = R->Evaluate(dgs).Clamp();
    if (!r.IsBlack()) {
        BxDF *strap = BSDF_ALLOC(arena, Strap)
                       (r, D, Deg, H);
        bsdf->Add(strap);
    }
    return bsdf;
}


IcMaterial *CreateIcMaterial(const Transform &xform,
        const TextureParams &mp) {
    Reference<Texture<Spectrum> > Kd = mp.GetSpectrumTexture("Kd", Spectrum(0.25f));
    Reference<Texture<Spectrum> > R = mp.GetSpectrumTexture("R", Spectrum(0.25f));
    Reference<Texture<float> > D = mp.GetFloatTexture("d", 1000.f);
    Reference<Texture<float> > Deg = mp.GetFloatTexture("deg", 0.f);
    Reference<Texture<float> > H = mp.GetFloatTexture("h", 1000.f);
    return new IcMaterial(Kd, R, D, Deg, H);
}


