/*
 * Copyright 2026 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "include/core/SkCanvas.h"
#include "include/core/SkImage.h"
#include "include/core/SkSurface.h"
#include "src/core/SkReadBuffer.h"

void FuzzImageDeserialize(const uint8_t* data, size_t size) {
    SkReadBuffer buffer(data, size);
    sk_sp<SkImage> image = buffer.readImage();
    if (!buffer.isValid() || !image) {
        return;
    }

    auto surface = SkSurfaces::Raster(SkImageInfo::MakeN32Premul(128, 128));
    if (!surface) {
        // May return nullptr in memory-constrained fuzzing environments
        return;
    }

    SkCanvas* canvas = surface->getCanvas();
    canvas->drawImage(image, 0, 0);
    canvas->drawImageRect(image, SkRect::MakeWH(64, 64), SkSamplingOptions(), nullptr);
}

#if defined(SK_BUILD_FOR_LIBFUZZER)
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size > 16384) {
        return 0;
    }
    FuzzImageDeserialize(data, size);
    return 0;
}
#endif
