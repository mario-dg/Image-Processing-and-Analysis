#pragma once
#include <opencv2/opencv.hpp>

#define CLAMP(v, lo, hi) (((v) < (lo)) ? (lo) : ((hi) < (v)) ? (hi) : (v))

namespace ueb01
{
    /**
    * Dieser Filter ist die Verallgemeinerung von brightness und factor. Er fuehrt eine lineare Histogrammtransformation durch.
    * 
    * @param[in/out] cv::Mat &, Matrix auf der der Filter angewendet werden soll
    * @param[in] float, Werte mit denen die Intensitaet angepasst wird
    */
    cv::Mat filterLinear(cv::Mat, float, float);

    /**
    * Dieser Filter manipuliert die Helligkeit des gesamten Bildes.
    * 
    * @param[in/out] cv::Mat &, Matrix auf der der Filter angewendet werden soll
    * @param[in] float, Wert mit dem die Intensitaet angepasst wird
    */
    cv::Mat filterBrightness(cv::Mat, float);

    /**
    * Dieser Filter manipuliert die Intensitaeten des gesamten Bildes anhand eines Faktors.
    * 
    * @param[in/out] cv::Mat &, Matrix auf der der Filter angewendet werden soll
    * @param[in] float, Wert mit dem die Intensitaet angepasst wird
    */
    cv::Mat filterFactor(cv::Mat, float);

} // namespace ueb01
