#include <iostream>
#include <string>
#include <limits>
#include <iomanip>

inline void Calendar() noexcept;
inline bool InvalidateInput(const char * const) noexcept;
inline void CaptureInput(
        unsigned char &month,
        unsigned long long &year,
        unsigned char &day
                        ) noexcept;
inline void PrintCalendar(
        unsigned char &month,
        unsigned long long &year,
        unsigned char &day
                         ) noexcept;

int main()
{
    Calendar();

    return 0;
}

void Calendar() noexcept
{
    unsigned char month, day;
    unsigned long long year;

    // Capture the calendar parameters
    CaptureInput(month, year, day);

    // Print the calendar
    PrintCalendar(month,year,day);
}

inline void CaptureInput(
        unsigned char &month,
        unsigned long long &year,
        unsigned char &day
                        ) noexcept
{
    bool inputSafe = false;
    short monthBuffer, dayBuffer;

    while (!inputSafe)
    {
        // Base case for input safety
        inputSafe = true;

        // Collect input
        std::cout << "Please enter the Date in M/Y/D where D is the"
                     "\nday of the week for the calendar to start on: ";
        std::cin >> monthBuffer >> year >> dayBuffer;

        month = monthBuffer - 1;
        day = dayBuffer;

        // If the input contained invalid data for any of the variables
        if (std::cin.fail())
        {
            // Clear error state
            std::cin.clear();
            // Flush the buffer
            std::cin.ignore(std::numeric_limits<long long>::max());

            inputSafe = InvalidateInput( "\nInvalid input, please use"
                                         " the correct format.\n");
        }
            // If the month is invalid
        else if (month < 0 || month > 11)
            inputSafe = InvalidateInput( "\nMonth must be from 1 to 12.\n");

            // If the day of the week is invalid
        else if (day < 0 || day > 6)
            inputSafe = InvalidateInput( "\nDay of the week must"
                                         " be from 0 to 6.\n");

        // The year cannot be invalid as it does not have any limits placed on
        // its value other than those based upon its type; therefore, its error
        // cases are covered by std::cin.fail()
    }
}

void PrintCalendar(unsigned char &month, unsigned long long int &year, unsigned char &day) noexcept
{
    // A container for month related data; used to avoid magic numbers.
    struct Month
    {
        const char * const NAME;
        const unsigned char LENGTH;

        constexpr Month(const char * const NAME, unsigned char LEN)
                : NAME(NAME), LENGTH(LEN) {}
    };

    // A table containing all months; used to avoid magic numbers.
    constexpr Month months[12] = {
            {"January", 31}, {"February", 28}, {"March",     31},
            {"April",   30}, {"May",      31}, {"June",      30},
            {"July",    31}, {"August",   31}, {"September", 30},
            {"October", 31}, {"November", 30}, {"December",  31}
    };

    // isComplete / null / null / null / null / null / null / isPadded
    unsigned char calFlags = false;
    unsigned char currentDay = 1, totalDaysPadding = 0;

    // Print calendar header
    std::cout << months[month].NAME << " " << year
              << "\n--------------------"
              << "\nSu Mo Tu We Th Fr Sa\n";

    // While the printing is incomplete
    while ((calFlags & 0b10000000) == false)
    {
        // If it has not printed the first day
        if ((calFlags & 0b00000001) != true)
        {
            // If the current day is the correct day to start printing
            if (day == totalDaysPadding)
            {
                // Set padding flag to true
                calFlags |= 0b00000001;
                std::cout << " 1 ";
                totalDaysPadding = day;
                if (day == 6)
                    std::cout << "\n";
            }
            // If not then it should pad before the first day
            else
            {
                // Print padding
                std::cout << "   ";
                // Increment total padding
                totalDaysPadding++;
            }
        }
        // If not then it should be treated as any other day
        else
        {

            // Increment and print the current day
            std::cout << std::setw(2) << std::right
                      << static_cast<short>(++currentDay);

            // Wrap text
            if ((totalDaysPadding + currentDay) % 7 == 0)
                std::cout << "\n";
                // Print spacer
            else
                std::cout << " ";

            // If it's the end of the month, february, and a leap year
            if (std::strcmp(months[month].NAME, "February") == 0 &&
                currentDay == months[month].LENGTH &&
                year % 4 == 0 &&
                (year % 100 != 0 ||
                (year % 100 == 0 && year % 400 == 0)))
                std::cout << "29";
            // If it's the end of the month
            else if (currentDay == months[month].LENGTH)
                calFlags |= 0b10000000;
        }
    }
}

inline bool InvalidateInput(const char * const str) noexcept
{
    std::cout << str << "\n\n";

    return false;
}
