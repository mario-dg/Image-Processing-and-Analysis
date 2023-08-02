#include "ueb04.hpp"
#include "util.hpp"

cv::Mat ueb04::extractTemplate(cv::Mat img, std::vector<int> i_values)
{
    std::vector<int> coords = {i_values.at(0), i_values.at(1)};
    int templDim = i_values.at(2);

    return img(cv::Rect(coords.at(0) - (templDim / 2), coords.at(1) - (templDim / 2), templDim, templDim));
}

/**
 * Bildet das durchschnittliche Matching-Template -> ältere Templates stärker gewichtet
 * 
 * @param[in] std::vector<cv::Mat>, Matching-Templates der letzten Frames
 * @return cv::Mat, Durchschnitts Matching-Template
 * 
*/
cv::Mat calcAvrgTemplate(std::vector<cv::Mat> templs)
{
    cv::Mat result = cv::Mat(templs.back().cols, templs.back().rows, CV_32FC1);
    
    cv::addWeighted(templs.at(0), 0.5, templs.at(1), 0.5, 0, templs.at(0), -1);
    cv::addWeighted(templs.at(2), 0.5, templs.at(3), 0.5, 0, templs.at(2), -1);
    cv::addWeighted(templs.at(4), 0.5, templs.at(5), 0.5, 0, templs.at(4), -1);
    cv::addWeighted(templs.at(6), 0.5, templs.at(7), 0.5, 0, templs.at(6), -1);
    cv::addWeighted(templs.at(8), 0.5, templs.at(9), 0.5, 0, templs.at(8), -1);
    cv::addWeighted(templs.at(10), 0.5, templs.at(11), 0.5, 0, templs.at(10), -1);
    cv::addWeighted(templs.at(12), 0.5, templs.at(13), 0.5, 0, templs.at(12), -1);
    cv::addWeighted(templs.at(14), 0.5, templs.at(15), 0.5, 0, templs.at(14), -1);

    cv::addWeighted(templs.at(0), 0.5, templs.at(2), 0.5, 0, templs.at(0), -1);
    cv::addWeighted(templs.at(4), 0.5, templs.at(6), 0.5, 0, templs.at(4), -1);
    cv::addWeighted(templs.at(8), 0.5, templs.at(10), 0.5, 0, templs.at(8), -1);
    cv::addWeighted(templs.at(12), 0.5, templs.at(14), 0.5, 0, templs.at(12), -1);

    cv::addWeighted(templs.at(0), 0.5, templs.at(4), 0.5, 0, templs.at(0), -1);
    cv::addWeighted(templs.at(8), 0.5, templs.at(12), 0.5, 0, templs.at(8), -1);

    cv::addWeighted(templs.at(0), 0.5, templs.at(8), 0.5, 0, result, -1);
    
    return result;
}

void ueb04::track(cv::Mat &img, std::string trackingStr, std::vector<cv::Mat> &templVec, std::vector<cv::Point> &matchLocVec, std::vector<float> f_values)
{
    //Farbe des Textes
    cv::Scalar_<float> color = cv::Scalar(f_values.at(2), f_values.at(1), f_values.at(0));

    //Ergebnis vom Template-Matching
    cv::Mat result = cv::Mat(img.cols - templVec.back().cols + 1, img.rows - templVec.back().rows + 1, CV_32FC1);
    //Aktuelles Template
    cv::Mat currTempl = calcAvrgTemplate(templVec);

    //Fuer iterative Anpassung neues Template hinzufuegen
    templVec.erase(templVec.begin());
    templVec.push_back(currTempl);

    ///Template-Matching
    cv::matchTemplate(img, currTempl, result, cv::TM_SQDIFF);
    cv::normalize(result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

    double minVal, maxVal, sum = 0.0;
    cv::Point maxLoc, matchLoc, avrgLoc;

    //Suchen des niedrigsten Wertes -> Bestes Matching Ergebnis
    cv::minMaxLoc(result, &minVal, &maxVal, &matchLoc, &maxLoc, cv::Mat());

    //Fuer zeitliche Stabilisierung neue Match-Location hinzufuegen
    matchLocVec.erase(matchLocVec.begin());
    matchLocVec.push_back(matchLoc);

    //Gewichtung der Match-Location
    for (unsigned int i = 0; i < matchLocVec.size(); i++)
    {
        avrgLoc += (1.0 / std::pow(2, i)) * matchLocVec.at(i);
        sum += 1.0 / std::pow(2, i);
    }
    avrgLoc /= sum;

    //Anzeigen des Textes an der getrackten Stelle
    cv::putText(img, trackingStr, cv::Point(avrgLoc.x + (currTempl.cols / 2), avrgLoc.y + (currTempl.rows / 2)), cv::FONT_ITALIC, 4, color, 12, 8, false);
}
