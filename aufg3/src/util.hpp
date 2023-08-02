/** Ein Modul fuer ungebundene Hilfsfunktionen
 * 	@author Leonhard Brandes, Mario da Graca
*/

#pragma once

#include <opencv2/opencv.hpp>
#include <functional>
#include "ueb01.hpp"
#include "ueb02.hpp"

namespace util
{
	/**
 	* Durchlaeuft jeden Pixel eines Bildes und wendet die uebergebene Funktion an
 	* 
 	* @param[in] cv::Mat, Bild
 	* @param[in] std::function, Funktion
 	*/
	void for_each_pixel(cv::Mat &m, std::function<void(cv::Mat &, int, int)> func);

	/**
	 * Schreibt die Daten eines Bildes auf stdout
	 * 
	 * @param[in] cv::Mat, Bild
	 * @param[in] std::string, Dateiformat
	 */
	void writeImgStdOut(cv::Mat m, std::string fileFormat);

	/**
	 * Zeigt ein Bild solange an, bis eine Taste gedrueckt wird
	 * 
	 * @param[in] cv::Mat, Bild
	 */
	void showImage(cv::Mat m);

	/**
	 * Gibt das uebergebene Histogramm auf der Standard-Ausgabe aus
	 * 
	 * @param[in] cv::Mat, Histogramm 
	*/
	void printHistogram(cv::Mat);

	/**
	 * Gibt das Resultat der Bild-Bewertung aus
	 * 
	 * @param[in] histogramEvaluation, Resultat
	*/
	void printHistogramCheck(ueb01::histogramEvaluation);

	/**
	 * Gibt das Resultat der Analyse auf Treppe oder Wald aus
	 * 
	 * @param[in] discriminatorEvaluation, Resultat
	*/
	void printDiscriminator(ueb02::discriminatorEvaluation);
}
