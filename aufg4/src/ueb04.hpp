#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

namespace ueb04
{
    /**
     * Extrahiert das Matching-Template aus dem Bild
     * 
     * @param[in] cv::Mat, Bild
     * @param[in] std::vector<int>, enthaelt Koordinaten und Dimensionen des Templates
     * @return cv::Mat, Matching-Template 
    */
    cv::Mat extractTemplate(cv::Mat, std::vector<int>);

    /**
     * Der Filter verfolgt mittels Template Matching einen Bildbereich durch das Video und blendet einen Text ein
     * 
     * @param[in] cv::Mat&, Bild, in dem getracked wird
     * @param[in] std::string, Text, der eingeblendet wird
     * @param[in] std::vector<cv::Mat>&, Matching-Templates der letzten Frames -> iterative Anpassung
     * @param[in] std::vector<cv::Point>&, Matching-Locations der letzten Frames -> zeitliche Anpassung
     * @param[in] std::vector<float>, RGB-Farbwerte des eingeblendeten Textes
     * 
    */
    void track(cv::Mat&, std::string, std::vector<cv::Mat> &, std::vector<cv::Point>&, std::vector<float>);
}