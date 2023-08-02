#pragma once
#include <opencv2/opencv.hpp>

namespace ueb01
{

    /* Aufzaehlungstyp fuer die Histogramm Analyse */
    enum histogramEvaluation
    {
        EVAL_BRIGHT,
        EVAL_DARK,
        EVAL_CONTRAST_LOW,
        EVAL_CONTRAST_GOOD,
        EVAL_NONE
    };

    typedef enum histogramEvaluation histogramEvaluation;

    /**
    * Dieser Filter begrenzt alle Werte auf den Wertebereich [0, 1] 
    * Viele Filter koennen Werte erzeugen, welche ausserhalb des Wertebereichs liegen. 
    * Vor dem Speichern oder dem Anzeigen sollte daher der Filter clamp aufgerufen werden.
    * 
    * @param[in/out] cv::Mat &, Matrix auf der der Filter angewendet werden soll
    */
    void filterClamp(cv::Mat &);

    /**
    * Dieser Filter manipuliert die Helligkeit des gesamten Bildes.
    * 
    * @param[in/out] cv::Mat &, Matrix auf der der Filter angewendet werden soll
    * @param[in] float, Wert mit dem die Intensitaet angepasst wird
    */
    void filterBrightness(cv::Mat &, float);

    /**
    * Dieser Filter manipuliert die Intensitaeten des gesamten Bildes anhand eines Faktors.
    * 
    * @param[in/out] cv::Mat &, Matrix auf der der Filter angewendet werden soll
    * @param[in] float, Wert mit dem die Intensitaet angepasst wird
    */
    void filterFactor(cv::Mat &, float);

    /**
    * Dieser Filter ist die Verallgemeinerung von brightness und factor. Er fuehrt eine lineare Histogrammtransformation durch.
    * 
    * @param[in/out] cv::Mat &, Matrix auf der der Filter angewendet werden soll
    * @param[in] float, Werte mit denen die Intensitaet angepasst wird
    */
    void filterLinear(cv::Mat &, float, float);

    /**
    * Dieser Filter aendert den Kontrast des gesamten Bildes anhand einer linearen Abbildung.
    * 
    * @param[in/out] cv::Mat &, Matrix auf der der Filter angewendet werden soll
    * @param[in] float, Werte mit denen die Intensitaet angepasst wird
    */
    void filterContrast(cv::Mat &, float);

    /**
    * Dieser Filter fuehrt eine Gamma-Anpassung durch.
    * 
    * @param[in/out] cv::Mat &, Matrix auf der der Filter angewendet werden soll
    * @param[in] float, Werte mit denen die Intensitaet angepasst wird
    */
    void filterGamma(cv::Mat &, float);

    /**
    * Dieser Filter summiert zwei Bilder pixelweise.
    * 
    * @param[in] cv::Mat, Matrizen, die addiert werden
    * @return cv::Mat, Ergebnis der Addtion (Neue Matrix)
    */
    cv::Mat filterAdd(cv::Mat, cv::Mat);

    /**
    * Dieser Filter subtrahiert zwei Bilder pixelweise.
    * 
    * @param[in] cv::Mat, Matrizen, die subtrahiert werden
    * @return cv::Mat, Ergebnis der Subtraktion (Neue Matrix)
    */
    cv::Mat filterSub(cv::Mat, cv::Mat);

    /**
    * Dieser Filter bestimmt pixelweise die absolute Differenz zweier Bilder.
    * 
    * @param[in] cv::Mat, Matrizen, deren absolut Differenz gebildet wird
    * @return cv::Mat, Differenzbild (Neue Matrix)
    */
    cv::Mat filterADiff(cv::Mat, cv::Mat);

    /**
    * Dieser Filter fuehrt eine "automatische Kontrastanpassung" des gesamten Bildes anhand einer linearen Abbildung durch.
    * Der kleinste im konkreten Bild vorkommende Wert wird auf 0 gesetzt, der groesste im konkreten Bild vorkommende Wert wird auf 1 gesetzt. 
    * Die Werte dazwischen werden linear Abgebildet.
    * 
    * @param[in/out] cv::Mat &, Matrix, an der die Normalisierung vorgenommen wird
    */
    void filterNormalize(cv::Mat &);

    /**
     * Erstellt ein Histogramm der relativen Haeufigkeiten aus dem uebergebenen Bild 
     * 
     * @param[in] cv::Mat, Bild dessen Histogramm erstellt wird
     * @param[out] cv::Mat &, Histogramm
     * @param[in] int, "BINS", Anzahl der Klassen des Histogramms
     */
    void histogram(cv::Mat, cv::Mat &, int);

    /**
     * Erstellt ein kumuliertes Histogramm der relativen Haeufigkeiten aus dem uebergebenen Bild 
     * 
     * @param[in] cv::Mat, Bild dessen Histogramm erstellt wird
     * @param[out] cv::Mat &, Histogramm
     * @param[in] int, "BINS", Anzahl der Klassen des kumulierten Histogramms
     */
    void sumHistogram(cv::Mat, cv::Mat &, int);

    /**
    * Dieser Pseudo-Filter bewertet die Qualitaet eines Bildes anhand seines Histogramms.
    * 
    * @param[in] cv::Mat, Bild, das bewertet wird
    * @return histogramEvaluation, Bewertungsergebnis
    */
    ueb01::histogramEvaluation histogramCheck(cv::Mat);
} // namespace ueb01
