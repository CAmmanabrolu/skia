/*
 * Copyright 2026 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "fuzz/oss_fuzz/FuzzLimits.h"
#include "include/core/SkBitmap.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkPaint.h"
#include "include/core/SkPath.h"
#include "include/core/SkPathEffect.h"

void FuzzPathEffectDeserialize(const uint8_t* data, size_t size) {
    auto pathEffect = SkPathEffect::Deserialize(data, size);
    if (!pathEffect) {
        return;
    }

    SkBitmap bitmap;
    bitmap.allocN32Pixels(SK_FUZZ_RASTER_DIMENSION, SK_FUZZ_RASTER_DIMENSION);
    SkCanvas canvas(bitmap);
    canvas.clear(SK_ColorTRANSPARENT);

    SkPath path;
    path.moveTo(0, 0);
    path.lineTo(SK_FUZZ_RASTER_DIMENSION - 1, SK_FUZZ_RASTER_DIMENSION - 1);
    path.lineTo(SK_FUZZ_RASTER_DIMENSION - 1, 0);
    path.close();

    SkPaint paint;
    paint.setPathEffect(pathEffect);
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setStrokeWidth(2.0f);
    canvas.drawPath(path, paint);
}

#if defined(SK_BUILD_FOR_LIBFUZZER)
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size > SK_FUZZ_MAX_DESERIALIZE_INPUT_SIZE) {
        return 0;
    }
    FuzzPathEffectDeserialize(data, size);
    return 0;
}
#endif
