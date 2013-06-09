
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


// materials/ic.cpp*
#include "stdafx.h"
#include "materials/ic.h"
#include "spectrum.h"
#include "reflection.h"
#include "paramset.h"
#include "texture.h"

// IcMaterial Method Definitions
BSDF *IcMaterial::GetBSDF(const DifferentialGeometry &dgGeom,
                               const DifferentialGeometry &dgShading,
                               MemoryArena &arena) const {
    // Allocate _BSDF_
    DifferentialGeometry dgs;
    dgs = dgShading;
    BSDF *bsdf = BSDF_ALLOC(arena, BSDF)(dgs, dgGeom.nn);

    Spectrum kd = Kd->Evaluate(dgs).Clamp();
    if (!kd.IsBlack()) {
        BxDF *diff = BSDF_ALLOC(arena, Lambertian)(kd);
        bsdf->Add(diff);
    }

    Spectrum ks = Ks->Evaluate(dgs).Clamp();
    if (!ks.IsBlack()) {
        Fresnel *fresnel = BSDF_ALLOC(arena, FresnelDielectric)(1.5f, 1.f);
        BxDF *spec = BSDF_ALLOC(arena, Microfacet)
                       (ks, fresnel, BSDF_ALLOC(arena, Blinn)(INFINITY));
        bsdf->Add(spec);
    }

    Spectrum r = R->Evaluate(dgs).Clamp();
    if (!r.IsBlack()) {
        float d = D->Evaluate(dgs);
        float a = A->Evaluate(dgs);
        float deg = Deg->Evaluate(dgs);
        float h = H->Evaluate(dgs);
        BxDF *strap = BSDF_ALLOC(arena, Strap)(r, d, a, deg, h);
        bsdf->Add(strap);
    }
    return bsdf;
}


IcMaterial *CreateIcMaterial(const Transform &xform,
        const TextureParams &mp) {
    Reference<Texture<Spectrum> > Kd = mp.GetSpectrumTexture("Kd", Spectrum(0.25f));
    Reference<Texture<Spectrum> > Ks = mp.GetSpectrumTexture("Ks", Spectrum(0.25f));
    Reference<Texture<Spectrum> > R = mp.GetSpectrumTexture("R", Spectrum(0.25f));
    Reference<Texture<float> > D = mp.GetFloatTexture("d", 1000.0f);
    Reference<Texture<float> > A = mp.GetFloatTexture("a", 250.0f);
    Reference<Texture<float> > Deg = mp.GetFloatTexture("deg", 0.0f);
    Reference<Texture<float> > H = mp.GetFloatTexture("h", 1000.0f);
    return new IcMaterial(Kd, Ks, R, D, A, Deg, H);
}


