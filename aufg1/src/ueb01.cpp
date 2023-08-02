#include "ueb01.hpp"
#include "util.hpp"

void ueb01::filterClamp(cv::Mat &m)
{
    util::for_each_pixel(m, [](cv::Mat &m, int y, int x) -> void {
        if (m.at<float>(y, x) <= 0.0f)
        {
            m.at<float>(y, x) = 0.0f;
        }
        else if (m.at<float>(y, x) >= 1.0f)
        {
            m.at<float>(y, x) = 1.0f;
        }
    });
}

void ueb01::filterBrightness(cv::Mat &m, float a)
{
    util::for_each_pixel(m, [a](cv::Mat &m, int y, int x) -> void {
        m.at<float>(y, x) += a;
    });
}

void ueb01::filterFactor(cv::Mat &m, float a)
{
    util::for_each_pixel(m, [a](cv::Mat &m, int y, int x) -> void {
        m.at<float>(y, x) *= a;
    });
}

void ueb01::filterLinear(cv::Mat &m, float a, float b)
{
    util::for_each_pixel(m, [a, b](cv::Mat &m, int y, int x) -> void {
        m.at<float>(y, x) = m.at<float>(y, x) * a + b;
    });
}

void ueb01::filterContrast(cv::Mat &m, float a)
{
    util::for_each_pixel(m, [a](cv::Mat &m, int y, int x) -> void {
        m.at<float>(y, x) = (m.at<float>(y, x) - 0.5f) * a + 0.5f;
    });
}

void ueb01::filterGamma(cv::Mat &m, float a)
{
    util::for_each_pixel(m, [a](cv::Mat &m, int y, int x) -> void {
        m.at<float>(y, x) = std::pow(m.at<float>(y, x), a);
    });
}

cv::Mat ueb01::filterAdd(cv::Mat m1, cv::Mat m2)
{
    cv::Mat returnMat(m1.rows, m1.cols, CV_32F);
    util::for_each_pixel(returnMat, [m1, m2](cv::Mat &returnMat, int y, int x) -> void {
        returnMat.at<float>(y, x) = m2.at<float>(y, x) + m1.at<float>(y, x);
    });

    return returnMat;
}

cv::Mat ueb01::filterSub(cv::Mat m1, cv::Mat m2)
{
    cv::Mat returnMat(m1.rows, m1.cols, CV_32F);
    util::for_each_pixel(returnMat, [m1, m2](cv::Mat &returnMat, int y, int x) -> void {
        returnMat.at<float>(y, x) = m1.at<float>(y, x) - m2.at<float>(y, x);
    });

    return returnMat;
}

cv::Mat ueb01::filterADiff(cv::Mat m1, cv::Mat m2)
{
    cv::Mat returnMat(m1.rows, m1.cols, CV_32F);
    util::for_each_pixel(returnMat, [m1, m2](cv::Mat &returnMat, int y, int x) -> void {
        returnMat.at<float>(y, x) = std::abs(m1.at<float>(y, x) - m2.at<float>(y, x));
    });

    return returnMat;
}

void ueb01::filterNormalize(cv::Mat &m)
{
    double low, high;
    cv::minMaxLoc(m, &low, &high);
    if (fabs(high - low) >= std::numeric_limits<float>::epsilon())
    {
        util::for_each_pixel(m, [low, high](cv::Mat &m, int y, int x) -> void {
            m.at<float>(y, x) = (m.at<float>(y, x) - low) * (1 / (high - low));
        });
    }
}

void ueb01::histogram(cv::Mat m, cv::Mat &histo, int size)
{
    // Obere Grenze nicht inklusive -> setzen der Grenze auf den kleinstmoeglichen Groesseren Wert als 1
    float range[] = {0.0f, 1.0f + std::numeric_limits<float>::epsilon()};
    const float *histRange = range;

    /*
        m: Liste der Matrizen, deren Histogramm erstellt werden soll (Bei uns nur 1)
        1: Anzahl der Matrizen
        {0}: Anzahl der Kanaele, bei Graustufenbildern 0
        cv::Mat(): Maske, die benutzt werden soll (Keine)
        histo: Ausgabe
        &size: Liste der Klassen des Histogramms
        &histRange: Liste der Intensitaetsbereiche
        true: gleichgrosse Klassen
        false: Histogramm wird vorm Befuellen geleert
    */
    cv::calcHist(&m, 1, {0}, cv::Mat(), histo, 1, &size, &histRange, true, false);

    for (int i = 0; i < histo.rows; i++)
    {
        // CalcHist berechnet absoluten Haeufigkeiten -> Relativ berechnen
        histo.at<float>(i, 0) /= (m.rows * m.cols);
    }
}

void ueb01::sumHistogram(cv::Mat m, cv::Mat &histo, int size)
{
    // Obere Grenze nicht inklusive -> setzen der Grenze auf den kleinstmoeglichen Groesseren Wert als 1
    float range[] = {0.0f, 1.0f + std::numeric_limits<float>::epsilon()};
    const float *histRange = range;
    cv::calcHist(&m, 1, {0}, cv::Mat(), histo, 1, &size, &histRange, true, false);

    for (int i = 0; i < histo.rows; i++)
    {
        histo.at<float>(i, 0) /= (m.rows * m.cols);
        // Kumulierte Werte berechnen
        histo.at<float>(i, 0) += histo.at<float>(i - 1, 0);
    }
}

ueb01::histogramEvaluation ueb01::histogramCheck(cv::Mat m)
{
    ueb01::histogramEvaluation result = ueb01::EVAL_NONE;
    cv::Mat histo(3, 1, CV_32S);
    int min, max;
    int i = 0;

    ueb01::histogram(m, histo, 3);

    // Pruefen, ob die Helligkeit des Bildes nach unseren Massstaeben zu dunkel ist
    if ((histo.at<float>(2, 0) < 0.15f) && (histo.at<float>(0, 0) > 1.0f / 3.0f))
    {
        result = ueb01::EVAL_DARK;
    }
    // Ob es zu hell ist
    else if ((histo.at<float>(2, 0) > (1.0f / 3.0f)) && (histo.at<float>(0, 0) < 0.15f))
    {
        result = ueb01::EVAL_BRIGHT;
    }
    else
    {
        // Sonst Kontrast einschaetzen
        cv::Mat sumHisto(32, 1, CV_32S);
        ueb01::sumHistogram(m, sumHisto, 32);

        // Bereich in dem 90% der Intensitaeten liegen bestimmen
        // Um weniger anfaellig fuers Rauschen zu sein
        while (sumHisto.at<float>(i, 0) < 0.05f)
        {
            i++;
        }
        min = i;

        i = 31;

        while (sumHisto.at<float>(i, 0) > 0.95f)
        {
            i--;
        }
        max = i;

        // Nach unseren Massstaeben beurteilen, ob der Kontrast zu niedrig oder gut ist
        if ((max - min) < 16)
        {
            result = ueb01::EVAL_CONTRAST_LOW;
        }
        else
        {
            result = ueb01::EVAL_CONTRAST_GOOD;
        }
    }
    return result;
}
