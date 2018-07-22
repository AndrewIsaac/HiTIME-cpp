#include <iostream>
#include <iterator>
#include "constants.h"
#include "options.h"
#include "cxxopts.h"

using namespace std;

Options::Options(int argc, char* argv[])
{
    intensity_ratio = default_intensity_ratio;
    rt_width = default_rt_width;
    rt_sigma = default_rt_sigma;
    ppm = default_ppm;
    mz_width = default_fwhm;
    mz_sigma = default_mz_sigma;
    mz_delta = default_mz_delta;
    min_sample = default_min_sample;
    in_file = "";
    out_file = "";
    debug = false;
    num_threads = 1;
    int num_args;

    string iratio_str = "Ratio of doublet intensities (isotope / parent). Defaults to " + to_string(default_intensity_ratio);
    string rtwidth_str = "Full width at half maximum for retention time in number of scans. Defaults to " + to_string(default_rt_width);
    string rtwindow_str = "Retention time width boundary in standard deviations. Defaults to " + to_string(default_rt_sigma);
    string ppm_str = "M/Z tolerance in parts per million. Defaults to " + to_string(default_ppm);
    string mzwidth_str = "M/Z full width at half maximum in parts per million. Defaults to " + to_string(default_fwhm);
    string mzsigma_str = "M/Z window boundary in standard deviations. Defaults to " + to_string(default_mz_sigma);
    string mzdelta_str = "M/Z delta for doublets. Defaults to " + to_string(default_mz_delta);
    string minsample_str = "Minimum number of data points required in each sample region. Defaults to " + to_string(default_min_sample);
    string threads_str = "Number of threads to use. Defaults to "  + to_string(num_threads);
    string desc = "Detect twin ion signal in Mass Spectrometry data";

    try {
        cxxopts::Options options("HiTIME-CPP", desc);
        options.add_options()
            ("h,help", "Show this help information.")
            ("a,iratio", iratio_str, cxxopts::value<double>())
            ("r,rtwidth", rtwidth_str, cxxopts::value<double>())
            ("t,rtwindow", rtwindow_str, cxxopts::value<double>())
            ("p,ppm", ppm_str, cxxopts::value<double>())
            ("m,mzwidth", mzwidth_str, cxxopts::value<double>())
            ("z,mzwindow", mzsigma_str, cxxopts::value<double>())
            ("d,mzdelta", mzdelta_str, cxxopts::value<double>())
            ("n,mindata", minsample_str, cxxopts::value<int>())
            ("debug", "Generate debugging output")
            ("j,threads", threads_str, cxxopts::value<int>())
            ("i,infile", "Input mzML file", cxxopts::value<string>())
            ("o,outfile", "Output mzML file", cxxopts::value<string>());

        num_args = argc;
        auto result = options.parse(argc, argv);

        if (num_args <= 1)
        {
            cout << program_name << " insufficient command line arguments." << endl;
            cout << options.help() << endl;
            exit(0);
        }

        if (result.count("help")) {
            cout << options.help() << endl;
            exit(0);
        }

        if (result.count("iratio")) {
            intensity_ratio = result["iratio"].as<double>();
        }
        if (result.count("rtwidth")) {
            rt_width = result["rtwidth"].as<double>();
        }
        if (result.count("rtwindow")) {
            rt_sigma = result["rtwindow"].as<double>();
        }
        if (result.count("ppm")) {
            ppm = result["ppm"].as<double>();
        }
        if (result.count("mzwidth")) {
            mz_width = result["mzwidth"].as<double>();
        }
        if (result.count("mzwindow")) {
            mz_sigma = result["mzwindow"].as<double>();
        }
        if (result.count("mzdelta")) {
            mz_delta = result["mzdelta"].as<double>();
        }
        if (result.count("mindata")) {
            min_sample = result["mindata"].as<double>();
        }
        if (result.count("infile")) {
            in_file = result["infile"].as<string>();
        }
        if (result.count("outfile")) {
            out_file = result["outfile"].as<string>();
        }
        if (result.count("debug")) {
            debug = true;
        }
        if (result.count("threads")) {
            int requested_threads = result["threads"].as<int>();
            if (requested_threads < 1)
            {
                cerr << program_name << " ERROR: number of requested threads may not be less than 1";
                exit(-1);
            }
            num_threads = requested_threads;
        }
    }

    catch (const cxxopts::OptionException& e)
    {
        std::cout << "error parsing options: " << e.what() << std::endl;
        exit(1);
    }
}
