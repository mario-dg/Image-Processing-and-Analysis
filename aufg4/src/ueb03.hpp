#pragma once
#include <opencv2/opencv.hpp>
#include "argparse.hpp"

namespace ueb03
{

    /**
    * Liefert die homogene Rotationsmatrix zum uebergebenen Winkel
    * 
    * @param[in] std::vector<float>, Floatvektor der nur den Winkel enthaelt(Grad)
    * @return cv::Matx33f, korrespondierende Rotationsmatrix
    */
    cv::Matx33f calcRotMatrix(std::vector<float>);

    /**
    * Liefert die homogene Skalierungsmatrix zu den uebergebenen Skalierungsfaktoren
    * 
    * @param[in] std::vector<float>, Floatvektor mit den Skalierungsfaktoren
    * @return cv::Matx33f, korrespondierende Skalierungsmatrix
    */
    cv::Matx33f calcScaleMatrix(std::vector<float>);

    /**
    * Liefert die homogene Scherungsmatrix zum uebergebenen Winkel
    * 
    * @param[in] std::vector<float>, Floatvektor der nur den Scherfaktor enthaelt
    * @return cv::Matx33f, korrespondierende Scherungsmatrix
    */
    cv::Matx33f calcShearMatrix(std::vector<float>);

    /**
    * Liefert die homogene Transformationsmatrix zu den uergebenen Werten
    * 
    * @param[in] std::vector<float>, Floatvektor mit den Werten der Transformationsmatrix
    * @return cv::Matx33f, korrespondierende Transformationsmatrix
    */
    cv::Matx33f calcTransformMatrix(std::vector<float>);

    /**
    * Berechnet mit Hilfe der Passpunktmethode eine homogene Transformationsmatrix
    * 
    * @param[in] std::vector<float>, Floatvektor mit der Zielgroesse und den 3 Passpunkten
    * @param[in] cv::Mat, Ursprungsbild
    * @return cv::Matx33f, korrespondierende Transformationsmatrix
    */
    cv::Matx33f calcUprightMatrix(std::vector<int>, cv::Mat);

    /**
    * Berechnet die Groesse des Zielbildes anhand des Urspungsbildes und der Transformationsmatrix
    * 
    * @param[in] cv::Mat, Ursprungsbild
    * @param[in] cv::Matx33f, Transformationsmatrix
    * @return std::vector<int>, Groesse des Zielbildes
    */
    std::vector<int> calcNewImageSize(cv::Mat, cv::Matx33f);

    /**
    * Fuehrt die Transformation auf dem Bild aus
    * 
    * @param[in] cv::Matx33f, Transformationsmatrix
    * @param[in] std::vector<int>, Groesse des Zielbildes
    * @param[in] cv::Mat, Ursprungsbild
    * @param[in] argp::INTERPOLATION, Interpolationsmethode (Nearest | Bilinear)
    * @param[in] argp::EDGE, Randbehandlungsmethode (NULL | VALUES | REPLICATE | REFLECT11 | REFLECT101)
    * @param[in] float, Fuer Randbehandlungsmethode (VALUES | NULL) relevanter Randwert 
    * @return cv::Mat, Transformiertes Ursprungsbild
    */
    cv::Mat transform(cv::Matx33f, std::vector<int>, cv::Mat, argp::INTERPOLATION, argp::EDGE, float);
} // namespace ueb03