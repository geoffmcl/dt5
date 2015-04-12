// dt5.cxx : Defines the entry point for the console application.
//
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <sys/types.h>
#include "dt5.hxx"
#include <time.h>
#ifdef _MSC_VER
#include <Windows.h>
#include <sys/timeb.h>
#else
#include <stdlib.h> // for exit, ...
#include <sys/time.h> // for gettimeofday, ...
#endif
#include <string.h>
#include <stdint.h>

static const char *time_format = "%Y/%m/%d %H:%M:%S";
/* ===================
   The time that is returned represents the number of seconds elapsed 
   since 00:00 hours, Jan 1, 1970 UTC. It’s also called UNIX EPOCH time.

   Examples:
   date "+%m/%d/%y"  = 7/4/06
   date "+%Y%m%d"    = 20060704

   The formatting codes for strftime are listed below: 
    %a  Abbreviated weekday name
    %A  Full weekday name
    %b  Abbreviated month name
    %B  Full month name
    %c  Date and time representation appropriate for locale
    %d  Day of month as decimal number (01 – 31)
    %H  Hour in 24-hour format (00 – 23)
    %I  Hour in 12-hour format (01 – 12)
    %j  Day of year as decimal number (001 – 366)
    %m  Month as decimal number (01 – 12)
    %M  Minute as decimal number (00 – 59)
    %p  Current locale's A.M./P.M. indicator for 12-hour clock
    %S  Second as decimal number (00 – 59)
    %U  Week of year as decimal number, with Sunday as first day of week (00 – 53)
    %w  Weekday as decimal number (0 – 6; Sunday is 0)
    %W  Week of year as decimal number, with Monday as first day of week (00 – 53)
    %x  Date representation for current locale
    %X  Time representation for current locale
    %y  Year without century, as decimal number (00 – 99)
    %Y  Year with century, as decimal number
    %z, %Z Either the time-zone name or time zone abbreviation, depending on registry settings; 
    no characters if time zone is unknown
    %%  Percent sign
   ==================== */
typedef struct tagFORM {
    const char *form;
    const char *desc;
    const char *example;
}FORM, *PFORM;

#ifdef WIN32
static FORM sForm[] = {
    { "%a", "Abbreviated weekday name", "" },
    { "%A", "Full weekday name", "" },
    { "%b", "Abbreviated month name", "" },
    { "%B", "Full month name", "" },
    { "%c", "Date and time representation appropriate for locale", "" },
    { "%d", "Day of month as decimal number (01 - 31)", "" },
    { "%H", "Hour in 24-hour format (00 - 23)", "" },
    { "%I", "Hour in 12-hour format (01 - 12)", "" },
    { "%j", "Day of year as decimal number (001 - 366)", "" },
    { "%m", "Month as decimal number (01 - 12)", "" },
    { "%M", "Minute as decimal number (00 - 59)", "" },
    { "%p", "Current locale's A.M./P.M. indicator for 12-hour clock", "" },
    { "%S", "Second as decimal number (00 - 59)", "" },
    { "%U", "Week of year as decimal number, with Sunday as first day of week (00 - 53)", "" },
    { "%w", "Weekday as decimal number (0 - 6; Sunday is 0)", "" },
    { "%W", "Week of year as decimal number, with Monday as first day of week (00 - 53)", "" },
    { "%x", "Date representation for current locale", "" },
    { "%X", "Time representation for current locale", "" },
    { "%y", "Year without century, as decimal number (00 - 99)", "" },
    { "%Y", "Year with century, as decimal number", "" },
    { "%z, %Z", "Either the time-zone name or time zone abbreviation, depending on registry settings;\n        no characters if time zone is unknown", "" },
    { "%%", "Percent sign", "" },

    // MUST BE LAST
    { 0, 0, 0 }
};

#else

FORM sForm[] = {
    { "%a", "weekday, abbreviated", "Mon" },
    { "%A", "weekday, full", "Monday" },
    { "%d", "day of the month (dd), zero filled", "08" },
    { "%e", "day of the month (dd)", "8" },
    { "%j", "day of year, zero filled", "001-366" },
    { "%u", "day of week starting with Monday (1), i.e. mtwtfss, 7 for Sunday","7" },
    { "%w", "day of week starting with Sunday (0), i.e. smtwtfs, 0 for Sunday","0" },
    // Week
    { "%U", "week number Sunday as first day of week","00–53" },
    { "%W", "week number Monday as first day of week","01–53" },
    { "%V", "week of the year", "01–53" },
    // Month
    { "%m", "mm two digit month", "10" },
    { "%h", "Mon", "Oct" },
    { "%b", "Mon, locale's abbreviated", "Oct" },
    { "%B", "locale's full month, variable length", "October" },
    // Year
    { "%y", "yy two digit year", "00–99" },
    { "%Y", "ccyy year", "2012" },
    { "%g", "2-digit year corresponding to the %V week number", "?" },
    { "%G", "4-digit year corresponding to the %V week number", "?" },
    // Century
    { "%C", "cc century", "00–99" },
    // Date
    { "%D", "mm/dd/yy", "10/22/12" },
    { "%x", "locale's date representation (mm/dd/yy)", "10/22/2012" },
    { "%F", "%Y-%m-%d", "2012-10-22" },
    // Hours
    { "%l", "hour (12 hour )", "2" },
    { "%I", "hour (12 hour ) zero filled", "02" },
    { "%k", "hour (24 hour )", "14" },
    { "%H", "hour (24 hour ) zero padded", "14" },
    { "%p", "locale's upper case AM or PM (blank in many locales)", "PM" },
    { "%P", "locale's lower case am or pm", "pm" },
    // Minutes
    { "%M", "MM minutes", "15" },
    // Seconds
    { "%s", "seconds since 00:00:00 1970-01-01 UTC (Unix epoch)", "1350915344" },
    { "%S", "SS second", "00–60" }, // (The 60 is necessary to accommodate a leap second)
    { "%N", "nanoseconds", "000000000–999999999" },
    // Time
    { "%r", "hours, minutes, seconds (12-hour clock)", "02:15:44 PM" },
    { "%R", "hours, minutes (24 hour clock)", "14:15" },
    { "%T", "hours, minutes, seconds (24-hour clock)", "14:15:44" },
    { "%X", "locale's time representation", "11:07:26 AM" },
    // Date and Time
    { "%c", "locale's date and time", "Sat Nov 04 12:02:33 EST 1989" },
    // Time zone
    { "%z", "-zzzz RFC-822 style numeric timezone", "-0500" },
    { "%Z", "time zone (e.g., EDT) null if no time zone is determinable", "EST" },

    // must be last
    { 0, 0, 0 }
};

#endif // WIN32 y/n

int is_leap_year (int year)
{
    if ((((year % 4) == 0 ) && ((year % 100) != 0)) || ((year % 400) == 0))
        return 1;
    return 0;
}

int show_leap_year()
{
    time_t raw_time;
    struct tm *ptr_ts;
    int year;
    time ( &raw_time );
    ptr_ts = gmtime ( &raw_time );
    year = ptr_ts->tm_year + 1900;

    printf("Leap Year %d %s\n",
        year,
        (is_leap_year(year) ? "Yes" : "No") );

    return 0;
}

 #define PST (-8)
 #define CET (1)
int show_gmtime()
{
    time_t raw_time;
    struct tm *ptr_ts;

    time ( &raw_time );
    ptr_ts = gmtime ( &raw_time );

    printf ("Time Los Angeles: %2d:%02d\n",
            ptr_ts->tm_hour+PST, ptr_ts->tm_min);
    printf ("Time Amsterdam: %2d:%02d\n",
            ptr_ts->tm_hour+CET, ptr_ts->tm_min);
    return 0;
}

int show_mktime(void)
{
	struct tm str_time;
	time_t time_of_day;
    struct tm *ptm;
    // to create a 'fake' time
	str_time.tm_year = 2012-1900;
	str_time.tm_mon = 6;
	str_time.tm_mday = 5;
	str_time.tm_hour = 10;
	str_time.tm_min = 3;
	str_time.tm_sec = 5;
	str_time.tm_isdst = 0;
	time_of_day = mktime(&str_time);
    // test using today
    time( &time_of_day );
    ptm = gmtime ( &time_of_day );
	time_of_day = mktime(ptm);
	printf("%s",ctime(&time_of_day));

	return 0;
}

int show_difftime(void)
{
	time_t start,end;
	volatile long unsigned counter;

	start = time(NULL);

	for(counter = 0; counter < 500000000; counter++)
		; /* Do nothing, just loop */

	end = time(NULL);
	printf("The loop used %f seconds.\n", difftime(end, start));
	return 0;
}

#ifdef WIN32
void wait ( int sec )
{
	clock_t end_wait;
	end_wait = clock () + sec * CLK_TCK ;

	while (clock() < end_wait) {}
}

int show_wait5()
{
    time_t mytime;
    mytime = time(NULL);
	printf("Start the Wait! %s", ctime(&mytime));

	wait (5);	/* Wait for 5 seconds */

    mytime = time(NULL);
	printf ("End Wait!       %s", ctime(&mytime));
	return 0;
}

int gettimeofday(struct timeval *tp, void *tzp)
{
#ifdef WIN32
    struct _timeb timebuffer;
    _ftime(&timebuffer);
    tp->tv_sec = (long)timebuffer.time;
    tp->tv_usec = timebuffer.millitm * 1000;
#else
    tp->tv_sec = time(NULL);
    tp->tv_usec = 0;
#endif
    return 0;
}
#endif

#ifdef _MSC_VER
#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif
 
struct timezone 
{
  int  tz_minuteswest; /* minutes W of Greenwich */
  int  tz_dsttime;     /* type of dst correction */
};


void gettimeofday_FAILS(struct timeval *tv, struct timezone *tz)
{
    FILETIME ft;
    unsigned __int64 tmpres = 0;
    static int tzflag;

    if (NULL != tv)
    {
        GetSystemTimeAsFileTime(&ft);
        tmpres |= ft.dwHighDateTime;
        tmpres <<= 32;
        tmpres |= ft.dwLowDateTime;
 
        /*converting file time to unix epoch*/
        tmpres -= DELTA_EPOCH_IN_MICROSECS; 
        tmpres /= 10;  /*convert into microseconds*/
        tv->tv_sec = (long)(tmpres / 1000000UL);
        tv->tv_usec = (long)(tmpres % 1000000UL);
    }
 
    if (NULL != tz)
    {
        if (!tzflag)
        {
            _tzset();
            tzflag++;
        }
        tz->tz_minuteswest = _timezone / 60;
        tz->tz_dsttime = _daylight;
    }
} 
#endif // _MSC_VER

void show_nanosecs()
{
    struct timeval tv;
    gettimeofday( (struct timeval *)&tv, (struct timezone *)0 );
    uint64_t tm;
    tm = tv.tv_sec * 1000000;
    tm += tv.tv_usec;
#ifdef WIN32
    printf("%I64u",tm);
#else
    printf("%llu",(long long)tm);
#endif
}

int get_timeofday(char *cp, int len, const char *form)
{
    struct timeval tv;
    time_t curtime;
    struct tm * ptm;
    gettimeofday( (struct timeval *)&tv, (struct timezone *)0 );
    curtime = (tv.tv_sec & 0xffffffff);
    ptm = localtime(&curtime);
    if (ptm) {
        // strftime(buffer,128,"%m-%d-%Y  %T.",ptm); !!! What is %T
        // strftime(buffer,128,"%m-%d-%Y %H:%M:%S.",ptm);
        strftime(cp,len,form,ptm);
    } else {
        strcpy(cp,"time not available!");
    }

    return 0;

}


int show_timeofday(void)
{
    char buffer[128];
    get_timeofday(buffer, 128, time_format);
    printf("%s\n",buffer);
    return 0;
}

#ifdef WIN32
int show_ms_example()
{
    char tmpbuf[128], timebuf[26], ampm[] = "AM";
    time_t ltime;
    struct _timeb tstruct;
    struct tm today, gmt, xmas = { 0, 0, 12, 25, 11, 93 };
    errno_t err;

    // Set time zone from TZ environment variable. If TZ is not set,
    // the operating system is queried to obtain the default value 
    // for the variable. 
    //
    _tzset();

    // Display operating system-style date and time. 
    _strtime_s( tmpbuf, 128 );
    printf( "OS time:\t\t\t\t%s\n", tmpbuf );
    _strdate_s( tmpbuf, 128 );
    printf( "OS date:\t\t\t\t%s\n", tmpbuf );

    // Get UNIX-style time and display as number and string. 
    time( &ltime );
    printf( "Time in seconds since UTC 1/1/70:\t%ld\n", ltime );
    err = ctime_s(timebuf, 26, &ltime);
    if (err)
    {
       printf("ctime_s failed due to an invalid argument.");
       exit(1);
    }
    printf( "UNIX time and date:\t\t\t%s", timebuf );

    // Display UTC. 
    err = _gmtime64_s( &gmt, &ltime );
    if (err)
    {
       printf("_gmtime64_s failed due to an invalid argument.");
    }
    err = asctime_s(timebuf, 26, &gmt);
    if (err)
    {
       printf("asctime_s failed due to an invalid argument.");
       exit(1);
    }
    printf( "Coordinated universal time:\t\t%s", timebuf );

    // Convert to time structure and adjust for PM if necessary. 
    err = _localtime64_s( &today, &ltime );
    if (err)
    {
       printf("_localtime64_s failed due to an invalid argument.");
       exit(1);
    }

    if( today.tm_hour >= 12 )
    {
        strcpy_s( ampm, sizeof(ampm), "PM" );
        today.tm_hour -= 12;
    }

    if( today.tm_hour == 0 )  // Adjust if midnight hour.
        today.tm_hour = 12;

    // Convert today into an ASCII string 
    err = asctime_s(timebuf, 26, &today);
    if (err)
    {
       printf("asctime_s failed due to an invalid argument.");
       exit(1);
    }

    // Note how pointer addition is used to skip the first 11 
    // characters and printf is used to trim off terminating 
    // characters.
    //
    printf( "12-hour time:\t\t\t\t%.8s %s\n",
       timebuf + 11, ampm );

    // Print additional time information. 
    _ftime( &tstruct ); // C4996
    // Note: _ftime is deprecated; consider using _ftime_s instead
    printf( "Plus milliseconds:\t\t\t%u\n", tstruct.millitm );
    printf( "Zone difference in hours from UTC:\t%u\n", 
             tstruct.timezone/60 );
    printf( "Time zone name:\t\t\t\t%s\n", _tzname[0] ); //C4996
    // Note: _tzname is deprecated; consider using _get_tzname
    printf( "Daylight savings:\t\t\t%s\n", 
             tstruct.dstflag ? "YES" : "NO" );

    // Make time for noon on Christmas, 1993. 
    if( mktime( &xmas ) != (time_t)-1 )
    {
       err = asctime_s(timebuf, 26, &xmas);
       if (err)
       {
          printf("asctime_s failed due to an invalid argument.");
          exit(1);
       }
       printf( "Christmas\t\t\t\t%s", timebuf );
    }

    // Use time structure to build a customized time string. 
    err = _localtime64_s( &today, &ltime );
    if (err)
    {
        printf(" _localtime64_s failed due to invalid arguments.");
        exit(1);
    }

    // Use strftime to build a customized time string. 
    strftime( tmpbuf, 128,
         "Today is %A, day %d of %B in the year %Y.\n", &today );
    printf( tmpbuf );

    return 0;
}
#endif

void give_help( char * name )
{
    char buffer[128];
    printf("Usage: %s [options] [+time format]\n", name );
    printf(" The time format, if given is a string commencing with a '+' char,\n");
    printf(" followed by one or more of the following... other chars are printed as is\n");
    PFORM pf = sForm;
    while( pf->form ) {
        if (strlen(pf->example)) {
            fprintf(stdout,"  %s - %s - %s\n", pf->form, pf->desc, pf->example);
        } else {
            get_timeofday(buffer,128,pf->form);
            fprintf(stdout,"  %s - %s - %s\n", pf->form, pf->desc, buffer);
        }
        pf++;
    }
    fprintf(stdout, "The default format is \"+%s\"\n", time_format);
}

// 20121226 - add a +%N switch to output nanosecs
// and +%s switch to output epoch seconds only
int main(int argc, char **argv )
{
    //time_t mytime;
    //mytime = time(NULL);
    int c, i;
    char *arg, *sarg;
    for (i = 1; i < argc; i++) {
        arg = argv[i];
        c = *arg;
        if (c == '+') {
            arg++;
            time_format = strdup(arg);
        } else {
            if (c == '-') {
                sarg = &arg[1];
                while (*sarg == '-') sarg++;
                c = *sarg;
                if ((c == 'h')||(c == '?')) {
                    give_help(argv[0]);
                    return 0;
                } else {
                    printf("Unknown commad '%s'!\n", arg );
                    return 1;
                }
            } else {
                printf("Unknown commad '%s'!\n", arg );
                return 1;
            }
        }
    }
    //printf(ctime(&mytime));
    //show_ms_example();
    show_timeofday();
    //show_mktime();
    //show_difftime();
    //show_gmtime();
    //show_wait5();
    //show_leap_year();
    return 0;
}

// eof - dt5.cxx

