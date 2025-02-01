# PS7: Kronos Log Parsing

## Contact
Name: Jason Ossai
Section:201
Time to Complete:3 and aa half hours


## Description
The goal of this project is to track the beginning and ending times of device boot operations by scanning Kronos log files. In order to provide a report that contains both successful and unsuccessful boots, as well as the boot duration in milliseconds, the log file must be processed, timestamps extracted for each boot process, and boot durations computed.

### Features
The major decisions in this project included:

Using Boost Libraries: I handled date and time parsing with Boost Date/Time and boot start and completion timestamp matching with Boost Regex. Boost offers a dependable and effective method for managing date-time manipulation with regular expressions.
Error Handling: To ensure that the program ends gracefully and with a useful error message in the event that the log file cannot be opened or read, I implemented error handling.
Output Format: A report that explicitly states whether a boot was successful or not is the format of the output. Additionally, the application determines how long successful booting take in milliseconds.

### Approach
To solve this problem, I approached it step by step:
Log Parsing: I started by going through each line of the input log file.
Regular Expressions: To match the lines that corresponded to the boot start and boot finish events, I employed regular expressions.
Boost's Date/Time library was used to extract and parse the timestamps from the lines that matched.
Elapsed Time Calculation: I determined the boot process's duration by calculating the elapsed time between two timestamps after a boot start and completion were matched.
Handling unfinished Boots: I labeled a boot as unfinished if it began without a matching completion line.
A comprehensive report that includes the start and end times of each boot procedure as well as its duration in milliseconds is produced by the application.

### Regex
Boot Start Regex:
([0-9]{4})-([0-9]{1,2})-([0-9]{1,2}) ([0-9]{2}):([0-9]{2}):([0-9]{2}) .*log.c.166.*

This regular expression is used to match log lines where a boot process begins. It captures the timestamp in the format YYYY-MM-DD HH:MM:SS and looks for the "log.c.166" string to identify a boot start event.

Boot Done Regex:
([0-9]{4})-([0-9]{1,2})-([0-9]{1,2}) ([0-9]{2}):([0-9]{2}):([0-9]{2}) .*oejs.AbstractConnector:Started SelectChannelConnector.*

This regular expression matches lines that indicate the completion of a boot process. It also captures the timestamp in the same format and looks for the string "oejs.AbstractConnector:Started SelectChannelConnector" to identify a successful boot completion.

### Issues
I had trouble with the timestamp parsing, which It was difficult to make the date-time format in log files consistent.  Boost Regex: I learned how to use Boost Regex to efficiently extract patterns from text files. This was very helpful for processing the log files.

Time Calculation: I became more comfortable using Boost's Date/Time library to handle time and date calculations, which was necessary for calculating boot durations.

### Extra Credit
Incomplete Boot Marking: I made sure that the report made it easy for users to grasp the state of each boot process by clearly marking incomplete boot events.

## Acknowledgements
Online resources given to us for boost:  http://www.boost.org/doc/libs/1_58_0/libs/regex/doc/html/index.
html
