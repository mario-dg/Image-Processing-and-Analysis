#include "argparse.hpp"
#include "filter.hpp"

#include <stdexcept>
#include <iostream>
#include <functional>

#define AMOUNT_F_TRACK_PARAMETERS (3)
#define AMOUNT_I_TRACK_PARAMETERS (3)

argp::Operations argp::parse_arguments(int argc, char *argv[])
{
    argp::Operations operations;
    std::vector<std::string> arguments(argv + 1, argv + argc);
    std::vector<std::string>::const_iterator it = arguments.cbegin(), end = arguments.cend();
    std::function<std::string(void)> next_arg = [&it, &end] {
        if (it == end)
        {
            throw std::runtime_error("Nicht genügend Argumente.");
        }
        return *it++;
    };

    while (it != end)
    {
        const std::string &action = next_arg();
        if (action == "--input")
        {
            argp::Operation op;
            op.action = argp::ACTION_INPUT;
            op.filename = next_arg();
            operations.push_back(op);
        }
        else if (action == "--output")
        {
            argp::Operation op;
            op.action = argp::ACTION_OUTPUT;
            op.filename = next_arg();
            operations.push_back(op);
        }
        else if (action == "--videoin")
        {
            argp::Operation op;
            op.action = argp::ACTION_VIDEOIN;
            op.filename = next_arg();
            operations.push_back(op);
        }
        else if (action == "--videoout")
        {
            argp::Operation op;
            op.action = argp::ACTION_VIDEOOUT;
            op.filename = next_arg();
            operations.push_back(op);
        }
        else if (action == "--print")
        {
            argp::Operation op;
            op.action = argp::ACTION_PRINT;
            operations.push_back(op);
        }
        else if (action == "--framerate")
        {
            argp::Operation op;
            op.action = argp::ACTION_FRAMERATE;
            op.f_filter_parameter.push_back(std::stof(next_arg()));
            operations.push_back(op);
        }
        else if (action == "--track")
        {
            argp::Operation op;
            op.action = argp::ACTION_TRACK;
            for (int i = 0; i < AMOUNT_I_TRACK_PARAMETERS; i++)
            {
                op.i_filter_parameter.push_back(std::stoi(next_arg()));
            }
            for (int i = 0; i < AMOUNT_F_TRACK_PARAMETERS; i++)
            {
                op.f_filter_parameter.push_back(std::stof(next_arg()) * 255);
            }
            op.trackingStr = next_arg();
            operations.push_back(op);
        }
        else if (action == "--track-gui")
        {
            argp::Operation op;
            op.action = argp::ACTION_TRACK_GUI;
            op.i_filter_parameter.push_back(std::stoi(next_arg()));
            for (int i = 0; i < AMOUNT_F_TRACK_PARAMETERS; i++)
            {
                op.f_filter_parameter.push_back(std::stof(next_arg()) * 255);
            }
            op.trackingStr = next_arg();
            operations.push_back(op);
        }
        else if (action == "--identity")
        {
            Operation op;
            op.action = argp::ACTION_IDENTITY_FILTER;
            operations.push_back(op);
        }
        else if (action == "--show")
        {
            Operation op;
            op.action = argp::ACTION_SHOW;
            operations.push_back(op);
        }
        else if (action == "--brightness")
        {
            Operation op;
            op.action = argp::ACTION_BRIGTHNESS;
            op.f_filter_parameter.push_back(std::stof(next_arg()));
            operations.push_back(op);
        }
        else if (action == "--factor")
        {
            Operation op;
            op.action = argp::ACTION_FACTOR;
            op.f_filter_parameter.push_back(std::stof(next_arg()));
            operations.push_back(op);
        }
        else if (action == "--linear")
        {
            Operation op;
            op.action = argp::ACTION_LINEAR;
            op.f_filter_parameter.push_back(std::stof(next_arg()));
            op.f_filter_parameter.push_back(std::stof(next_arg()));
            operations.push_back(op);
        }
        else if (action == "--values")
        {
            Operation op;
            op.action = argp::ACTION_VALUES;
            op.i_filter_parameter.push_back(std::stoi(next_arg()));
            op.i_filter_parameter.push_back(std::stoi(next_arg()));
            op.filter = cv::Mat::zeros(op.i_filter_parameter.at(1), op.i_filter_parameter.at(0), CV_32F);
            int i = 0;
            int k = 0;
            for (i = 0; i < op.i_filter_parameter.at(1); i++)
            {
                for (k = 0; k < op.i_filter_parameter.at(0); k++)
                {
                    op.filter.at<float>(i, k) = std::stof(next_arg());
                }
            }
            operations.push_back(op);
        }
        else if (action == "--edge")
        {
            Operation op;
            op.action = argp::ACTION_EDGE;
            std::string temp = next_arg();
            if (temp == "null")
            {
                op.edgeOpt = EDGE_NULL;
                op.f_filter_parameter.push_back(0.0f);
            }
            else if (temp == "replicate")
            {
                op.edgeOpt = EDGE_REPLICATE;
                op.f_filter_parameter.push_back(-1.0f);
            }
            else if (temp == "reflect11")
            {
                op.edgeOpt = EDGE_REFLECT_11;
                op.f_filter_parameter.push_back(-1.0f);
            }
            else if (temp == "reflect101")
            {
                op.edgeOpt = EDGE_REFLECT_101;
                op.f_filter_parameter.push_back(-1.0f);
            }
            else
            {
                op.edgeOpt = EDGE_VALUE;
                op.f_filter_parameter.push_back(std::stof(temp));
            }
            operations.push_back(op);
        }
        else if (action == "--fold")
        {
            Operation op;
            op.action = argp::ACTION_FOLD;
            operations.push_back(op);
        }
        else if (action == "--box")
        {
            Operation op;
            op.action = argp::ACTION_BOX;
            op.i_filter_parameter.push_back(3);
            op.i_filter_parameter.push_back(3);
            op.filter = filter::boxFilter;
            operations.push_back(op);
        }
        else if (action == "--gaussian")
        {
            Operation op;
            op.action = argp::ACTION_GAUSSIAN;
            op.i_filter_parameter.push_back(5);
            op.i_filter_parameter.push_back(5);
            op.filter = filter::gaussianFilter;

            operations.push_back(op);
        }
        else if (action == "--sobel-x")
        {
            Operation op;
            op.action = argp::ACTION_SOBEL_X;
            op.i_filter_parameter.push_back(3);
            op.i_filter_parameter.push_back(3);
            op.filter = filter::sobelXFilter;

            operations.push_back(op);
        }
        else if (action == "--sobel-y")
        {
            Operation op;
            op.action = argp::ACTION_SOBEL_Y;
            op.i_filter_parameter.push_back(3);
            op.i_filter_parameter.push_back(3);
            op.filter = filter::sobelYFilter;

            operations.push_back(op);
        }
        else if (action == "--laplace")
        {
            Operation op;
            op.action = argp::ACTION_LAPLACE;
            op.i_filter_parameter.push_back(3);
            op.i_filter_parameter.push_back(3);
            op.filter = filter::laplaceFilter;

            operations.push_back(op);
        }
        /*------------------------------------------------------------*/
        /*                 Ab hier Uebung 03                          */
        /*------------------------------------------------------------*/

        else if (action == "--rotate")
        {
            Operation op;
            op.action = argp::ACTION_ROTATE;
            //der Wert ist die Rotation in Grad
            op.f_filter_parameter.push_back(std::stof(next_arg()));
            operations.push_back(op);
        }
        else if (action == "--scale")
        {
            Operation op;
            op.action = argp::ACTION_SCALE;
            //Skalierung in x- und in y-Richtung
            op.f_filter_parameter.push_back(std::stof(next_arg()));
            op.f_filter_parameter.push_back(std::stof(next_arg()));
            operations.push_back(op);
        }
        else if (action == "--shear")
        {
            Operation op;
            op.action = argp::ACTION_SHEAR;
            //der Wert ist der Scherfaktor
            op.f_filter_parameter.push_back(std::stof(next_arg()));
            operations.push_back(op);
        }
        else if (action == "--transform")
        {

            Operation op;
            op.action = argp::ACTION_TRANSFORM;
            int m = 0;
            //4, da 2x2 Matrix fuer Transformationen, Schleife lohnt eigentlich nicht wirklich
            for (; m < 4; m++)
            {
                op.f_filter_parameter.push_back(std::stof(next_arg()));
            }

            operations.push_back(op);
        }
        //erwartet 8 Werte: 2 fuer die Zielbildgroesse und 3 Koordinaten
        else if (action == "--upright")
        {
            int i = 0;
            Operation op;
            op.action = argp::ACTION_UPRIGHT;
            for (; i < 8; i++)
            {
                int val = std::stoi(next_arg());
                op.i_filter_parameter.push_back(val);
            }
            operations.push_back(op);
        }
        else if (action == "--interpolation")
        {
            Operation op;
            op.action = argp::ACTION_INTERPOLATION;
            std::string temp = next_arg();
            if (temp == "nearest")
            {
                op.interpolOpt = INTERPOLATION_NEAREST;
            }
            else if (temp == "bilinear")
            {
                op.interpolOpt = INTERPOLATION_BILINEAR;
            }
            else
            { //unbekannte Interpolationsmethode
                throw std::runtime_error("Unbekannter Parameter.");
            }
            operations.push_back(op);
        }
        else
        {
            throw std::runtime_error(std::string("Unbekannter Parameter ") + action);
        }
    }
    return operations;
}