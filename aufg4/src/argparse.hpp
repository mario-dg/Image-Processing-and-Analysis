/**
 * Ein Modul zum Verarbeiten von Kommandozeilen Parametern
 * 
 * 	@author Leonhard Brandes, Mario da Graca
*/

#pragma once

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

namespace argp
{
    enum Action
    {
        ACTION_NONE,
        ACTION_INPUT,
        ACTION_OUTPUT,
        ACTION_VIDEOIN,
        ACTION_VIDEOOUT,
        ACTION_PRINT,
        ACTION_FRAMERATE,
        ACTION_TRACK,
        ACTION_TRACK_GUI,
        ACTION_IDENTITY_FILTER,
        ACTION_SHOW,
        ACTION_BRIGTHNESS,
        ACTION_FACTOR,
        ACTION_LINEAR,
        ACTION_VALUES,
        ACTION_EDGE,
        ACTION_FOLD,
        ACTION_BOX,
        ACTION_GAUSSIAN,
        ACTION_SOBEL_X,
        ACTION_SOBEL_Y,
        ACTION_LAPLACE,
        ACTION_ROTATE,
        ACTION_SCALE,
        ACTION_SHEAR,
        ACTION_TRANSFORM,
        ACTION_UPRIGHT,
        ACTION_INTERPOLATION,
    };

    enum EDGE
    {
        EDGE_NULL,
        EDGE_VALUE,
        EDGE_REPLICATE,
        EDGE_REFLECT_11,
        EDGE_REFLECT_101
    };

    enum INTERPOLATION
    {
        INTERPOLATION_NEAREST,
        INTERPOLATION_BILINEAR
    };

    struct Operation
    {
        // Aktion
        Action action;
        // Dateiname
        std::string filename;
        // Faktor fuer zB linear, edgeValue, verschiedene Transformationen
        std::vector<float> f_filter_parameter;
        // Anzahl an Klassen beim Histogramm, Dimensionen einer zu erstellenden Matrix, upright
        std::vector<int> i_filter_parameter;
        //Filter-Matrix
        cv::Mat filter;
        // Werte fuer die Randbehandlung
        EDGE edgeOpt;
        //fuer Interpolation
        INTERPOLATION interpolOpt;
        //Tracking-String
        std::string trackingStr;
    };

    typedef std::vector<Operation> Operations;

    Operations parse_arguments(int argc, char *argv[]);
} // namespace argp