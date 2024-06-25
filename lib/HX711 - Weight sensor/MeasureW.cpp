#include "HX711.h"
#include "MeasureW.h"

// Define the calibration factor
const float calibration_factor = 476.53;

float normalizeWeight(float weight) {
    // Normalize weight to nearest whole gram
    if (abs(weight) < 0.1) {
        return 0.0;
    } else {
        return round(weight);
    }
}
