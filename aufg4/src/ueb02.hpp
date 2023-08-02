#pragma once
#include <opencv2/opencv.hpp>
#include "argparse.hpp"

namespace ueb02
{

    /**
    * Liefert eine neue Matrix
    * 
    * @param[in] cv::Mat &, Matrix die kopiert werden soll
    * @return cv::Mat, kopierte Matrix
    */
    cv::Mat values(cv::Mat);

    int clampToEdge(int, int, int);

    cv::Vec3b reflect(argp::EDGE, cv::Mat, cv::Mat, int, int, int, int);
    
    /**
    * Faltet 2 Matrizen miteinander, Kantenbehandlung wird beachtet
    * 
    * @param[in] cv::Mat, Matrizen die gefaltet werden(1=Bild, 2=Filter)
    * @param[in] argp::EDGE. Kantenbehandlungsmethode
    * @param[in] float, Wert der für die Kantenbehandlungsmethode "values" und "null" verwendet wird
    * @return cv::Mat, Ergebnis der Faltung, Groesse entspricht der des Bildes
    */
    cv::Mat fold(cv::Mat, cv::Mat, argp::EDGE, float);

} // namespace ueb02