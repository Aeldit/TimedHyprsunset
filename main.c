#include <pthread.h>
#include <stdlib.h>
#include <threads.h>
#include <time.h>

struct hm
{
    int h;
    int m;
};

char streq(char *a, char *b)
{
    if (!a || !b)
    {
        return 0;
    }

    unsigned long i = 0;
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

struct hm strtohm(char *s)
{
    if (!s)
    {
        return (struct hm){ .h = 0, .m = 0 };
    }

    unsigned long i = 0;
    int h = 0;
    int m = 0;
    char is_in_h = 1;
    char c = 0;
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

    struct hm start = (struct hm){ .h = 18, .m = 0 };
    struct hm stop = (struct hm){ .h = 8, .m = 0 };
    for (int i = 1; i < argc; ++i)
    {
        char *arg = argv[i];
        if (!arg)
        {
            continue;
        }

        if (streq(arg, "--start") && i + 1 < argc)
        {
            start = strtohm(argv[++i]);
        }
        else if (streq(arg, "--stop") && i + 1 < argc)
        {
            stop = strtohm(argv[++i]);
        }
    }

    time_t rawtime;
    struct tm *timeinfo;

    char sunset = 0;

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
                 && ((h > start.h && h < stop.h)
                     || (h == start.h && m >= start.m)
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
