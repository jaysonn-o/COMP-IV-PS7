//   Copyright [2024] Jason Ossai
#include <iostream>
#include <fstream>
#include <regex>
#include <boost/regex.hpp>
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

using boost::posix_time::ptime;
using boost::posix_time::time_duration;
using boost::gregorian::date;
using boost::gregorian::from_simple_string;
using boost::smatch;

int ret_int(const std::string& s) {
    return std::stoi(s);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::ifstream input(argv[1]);
    if (!input.is_open()) {
        std::cerr << "Error opening log file" << std::endl;
        return -1;
    }

    std::string logFileName(argv[1]);
    std::string output_name = logFileName + ".rpt";
    std::ofstream outputFile(output_name.c_str());
    outputFile << "InTouch log file: " << logFileName << std::endl;

    // Regex patterns to match boot start and boot completion
    std::string prefix = "([0-9]{4})-([0-9]{1,2})-([0-9]{1,2}) ([0-9]{2}):([0-9]{2}):([0-9]{2})";
    std::string boot_suffix = "log.c.166";  // Boot start identifier
    std::string done_suffix = "oejs.AbstractConnector:Started SelectChannelConnector";  // Completion identifier

    std::string boot = prefix + ".*" + boot_suffix + ".*";
    std::string done = prefix + ".*" + done_suffix + ".*";

    boost::regex boot_regex(boot);
    boost::regex finish_regex(done);
    smatch m;
    ptime pt1, pt2;
    std::string line;
    int line_num = 0;
    bool is_boot = false;  // Flag to track if we are in a boot sequence

    try {
        while (getline(input, line)) {
            line_num++;

            // Check for boot record (Boot Start)
            if (regex_match(line, m, boot_regex)) {
                // If there was a previous boot without a completion, mark it incomplete
                if (is_boot) {
                    outputFile << "**** Incomplete boot **** at line " << line_num - 1 << "\n";
                }

                // Parse the timestamp for boot start
                date d(from_simple_string(m[1].str() + "-" + m[2].str() + "-" + m[3].str()));
                ptime temp(d, time_duration(ret_int(m[4]), ret_int(m[5]), ret_int(m[6])));
                pt1 = temp;

                outputFile << "Device Boot Report" << std::endl;
                outputFile << line_num << "(" << argv[1] << "): ";
                outputFile << m[1] << "-" << m[2] << "-" << m[3] << " ";
                outputFile << m[4] << ":" << m[5] << ":" << m[6] << " ";
                outputFile << "Boot Start" << std::endl;

                is_boot = true;  // Mark that a boot process has started
            }

            // Check for the completion record (Boot Done)
            if (is_boot && regex_match(line, m, finish_regex)) {
                // Parse the timestamp for boot end
                date d(from_simple_string(m[1].str() + "-" + m[2].str() + "-" + m[3].str()));
                ptime temp(d, time_duration(ret_int(m[4]), ret_int(m[5]), ret_int(m[6])));
                pt2 = temp;

                // Calculate the elapsed time for the boot
                time_duration td = pt2 - pt1;

                // Write to output if boot completed successfully
                outputFile << line_num << "(" << argv[1] << "): ";
                outputFile << m[1] << "-" << m[2] << "-" << m[3] << " ";
                outputFile << m[4] << ":" << m[5] << ":" << m[6] << " ";
                outputFile << "Boot Completed" << std::endl;
                outputFile << "\t" << "Boot time: " << td.total_milliseconds() << "ms \n" << std::endl;

                is_boot = false;  // Reset the boot flag
            }

            // If there's no completion record but a boot start was found, mark it as incomplete
            if (is_boot && line_num == 0) {
                outputFile << "**** Incomplete boot **** at line " << line_num << "\n";
            }
        }

        // Check for any unfinished boot sequences at the end of the file
        if (is_boot) {
            outputFile << "**** Incomplete boot **** at the end of the file \n";
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    std::cout << "Complete!" << std::endl;
    outputFile.close();
    return 0;
}
