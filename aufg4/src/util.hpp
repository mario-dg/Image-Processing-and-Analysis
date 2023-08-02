/** Ein Modul fuer ungebundene Hilfsfunktionen
 * 	@author Leonhard Brandes, Mario da Graca
*/

#pragma once

#include <opencv2/opencv.hpp>
#include <functional>

namespace util
{
	/**
 	* Durchlaeuft jeden Pixel eines Bildes und wendet die uebergebene Funktion an
 	* 
 	* @param[in] cv::Mat, Bild
 	* @param[in] std::function, Funktion
 	*/
	void for_each_pixel(cv::Mat3f &m, std::function<void(cv::Mat3f &, int, int)> func);
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
}
