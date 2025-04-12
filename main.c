#include <pthread.h>
#include <stdlib.h>
#include <threads.h>

struct hm
{
    int h;
    int m;
};

unsigned char streq(const char *const a, const char *const b)
{
    if (!a || !b)
    {
        return 0;
    }

    // We use an unsigned char because this function is only called with
    // "--start" and "--stop", so we know the max number will never be greater
    // than 7
    unsigned char i = 0;
    while (a[i] && b[i])
    {
        if (a[i] != b[i])
        {
            return 0;
        }
        ++i;
    }
    return 1;
}

struct hm strtohm(const char *const s)
{
    if (!s)
    {
        return (struct hm){ 0 };
    }

    unsigned char i = 0;
    int h           = 0;
    int m           = 0;
    char is_in_h    = 1;
    char c          = 0;
    while ((c = s[i++]))
    {
        switch (c)
        {
        case ':':
            is_in_h = 0;
            break;

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            if (is_in_h)
            {
                h *= 10;
                h += c - '0';
            }
            else
            {
                m *= 10;
                m += c - '0';
            }
            break;
        }

        // In case the given time is not in the form 'HH:MM'
        if (i > 5)
        {
            break;
        }
    }
    if (h > 24 || h < 0)
    {
        h = 0;
    }
    if (m > 60 || m < 0)
    {
        m = 0;
    }
    return (struct hm){ .h = h, .m = m };
}

void *launch_sunset(void *arg)
{
    system("hyprsunset -t 3500");
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 5 || !argv)
    {
        return 1;
    }

    // Set default values
    struct hm start = (struct hm){ .h = 18, .m = 0 };
    struct hm stop  = (struct hm){ .h = 8, .m = 0 };
    for (int i = 1; i < argc; ++i)
    {
        const char *const arg = argv[i];
        if (!arg)
        {
            continue;
        }

        if (streq("--start", arg) && i + 1 < argc)
        {
            start = strtohm(argv[++i]);
        }
        else if (streq("--stop", arg) && i + 1 < argc)
        {
            stop = strtohm(argv[++i]);
        }
    }

    // If the stop time is after the start time
    if (stop.h > start.h || (stop.h == start.h && stop.m >= start.m))
    {
        return 2;
    }

    time_t rawtime;
    struct tm *timeinfo;

    char sunset = 0;
    system("pkill hyprsunset");

    while (1)
    {
        time(&rawtime);
        timeinfo = localtime(&rawtime);

        int h = timeinfo->tm_hour;
        int m = timeinfo->tm_min;

        if (sunset
            && ((h > stop.h && h < start.h) || (h == stop.h && m >= stop.m)
                || (h == start.h && m < start.m)))
        {
            sunset = 0;
            system("pkill hyprsunset");
        }
        else if (!sunset
                 && (h > start.h || (h == start.h && m >= start.m) || h < stop.h
                     || (h == stop.h && m < stop.m)))
        {
            sunset = 1;
            pthread_t thread_id;
            pthread_create(&thread_id, NULL, launch_sunset, NULL);
        }

        thrd_sleep(&(struct timespec){ .tv_sec = 300 }, NULL); // sleep 5 min
    }
    return 0;
}
