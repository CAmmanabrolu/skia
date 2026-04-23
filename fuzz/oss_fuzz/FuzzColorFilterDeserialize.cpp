/*
 * Copyright 2026 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "fuzz/oss_fuzz/FuzzLimits.h"
#include "include/core/SkBitmap.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkColorFilter.h"
#include "include/core/SkPaint.h"

void FuzzColorFilterDeserialize(const uint8_t* data, size_t size) {
    auto colorFilter = SkColorFilter::Deserialize(data, size);
    if (!colorFilter) {
        return;
    }

    SkBitmap bitmap;
    bitmap.allocN32Pixels(SK_FUZZ_RASTER_DIMENSION, SK_FUZZ_RASTER_DIMENSION);
    SkCanvas canvas(bitmap);
    canvas.clear(SK_ColorTRANSPARENT);

    SkPaint paint;
    paint.setColorFilter(colorFilter);
    paint.setColor(SK_ColorMAGENTA);
    canvas.drawPaint(paint);
}

#if defined(SK_BUILD_FOR_LIBFUZZER)
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size > SK_FUZZ_MAX_DESERIALIZE_INPUT_SIZE) {
        return 0;
    }
    FuzzColorFilterDeserialize(data, size);
    return 0;
}
#endif
