/*
 * Copyright 2026 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "include/core/SkCanvas.h"
#include "include/core/SkPaint.h"
#include "include/core/SkSurface.h"
#include "src/core/SkReadBuffer.h"

void FuzzPaintDeserialize(const uint8_t* data, size_t size) {
    SkReadBuffer buffer(data, size);
    SkPaint paint = buffer.readPaint();
    if (!buffer.isValid()) {
        return;
    }

    auto surface = SkSurfaces::Raster(SkImageInfo::MakeN32Premul(128, 128));
    if (!surface) {
        // May return nullptr in memory-constrained fuzzing environments
        return;
    }

    SkCanvas* canvas = surface->getCanvas();
    canvas->drawRect(SkRect::MakeXYWH(1, 1, 126, 126), paint);
    canvas->drawCircle(64, 64, 42, paint);
}

#if defined(SK_BUILD_FOR_LIBFUZZER)
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size > 4096) {
        return 0;
    }
    FuzzPaintDeserialize(data, size);
    return 0;
}
#endif
