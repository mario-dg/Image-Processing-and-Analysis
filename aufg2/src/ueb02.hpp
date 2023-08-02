#pragma once
#include <opencv2/opencv.hpp>
#include "argparse.hpp"

namespace ueb02
{

    /* Aufzaehlungstyp fuer die Bildanalyse */
    enum discriminatorEvaluation
    {
        EVAL_FOREST,
        EVAL_STAIRS,
        EVAL_NONE
    };

    /**
    * Liefert eine neue Matrix
    * 
    * @param[in] cv::Mat &, Matrix die kopiert werden soll
    * @return cv::Mat, kopierte Matrix
    */
    cv::Mat values(cv::Mat);

    /**
    * Faltet 2 Matrizen miteinander, Kantenbehandlung wird beachtet
    * 
    * @param[in] cv::Mat, Matrizen die gefaltet werden(1=Bild, 2=Filter)
    * @param[in] argp::EDGE. Kantenbehandlungsmethode
    * @param[in] float, Wert der für die Kantenbehandlungsmethode "values" und "null" verwendet wird
    * @return cv::Mat, Ergebnis der Faltung, Groesse entspricht der des Bildes
    */
    cv::Mat fold(cv::Mat, cv::Mat, argp::EDGE, float);

    /**
    * Der Median Filter wählt den mittleren Wert aus. 
    * Hat das Bild eine gerade Anzahl an Bildpunkten, wird der kleinere Wert (Untermedian) ausgewählt
    * 
    * @param[in] cv::Mat, Bild, auf das der Median-Filter angewendet werden soll
    * @return cv::Mat, Ergebnis
    */
    cv::Mat filterMedian(cv::Mat);

    /**
    * Die Erosion wählt das einfache Minimum aus den neun (3 x 3 Pixel Quadrat) umliegenden Werten aus
    * 
    * @param[in] cv::Mat, Bild, auf das der Erosions-Filter angewendet werden soll
    * @return cv::Mat, Ergebnis
    */
    cv::Mat filterErode(cv::Mat);

    /**
    * Die Dilatation wählt das einfache Maximum aus den neun (3 x 3 Pixel Quadrat) umliegenden Werten aus
    * 
    * @param[in] cv::Mat, Bild, auf das der Dilatations-Filter angewendet werden soll
    * @return cv::Mat, Ergebnis
    */
    cv::Mat filterDilate(cv::Mat);

    /**
    * Dieser Pseudo-Filter bestimmt, ob es sich bei dem Bild um eine Treppe oder einen Wald handelt
    * 
    * @param[in] cv::Mat, Bild, das bewertet wird
    * @return discriminatorEvaluation, Bewertungsergebnis
    */
    ueb02::discriminatorEvaluation discriminator(cv::Mat);
} // namespace ueb02