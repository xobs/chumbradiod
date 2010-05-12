/*
 * chumbyradio.cpp
 *
 * (c) Copyright Chumby Industries, 2007
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "crad_interface.h"

#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static void usage(char *s);
static int debug = 0;

extern "C" void dump_radio_registers(crad_t *p_crad);
extern "C" void set_radio_led(crad_t *p_crad,int value);
extern "C" int tune_radio(crad_t *p_crad, int station);
extern "C" int seek_radio(crad_t *p_crad, int up, int strength);
extern "C" void set_radio_volume(crad_t *p_crad,int volume);

/*! dump current radio status XML */
static void dump_radio_xml(crad_t *p_crad);

/*! command line program entry point */
int main(int argc, char **argv) {
    crad_t *p_crad = 0;
    int c;
    int dump_xml = 0;
    char *hiddev_path = CRAD_DEFAULT_DEVICE_PATH;
    float station = -1.0;
    char *my_name = argv[0];
    int seek = 0;
    int up = 0;
    int strength = 20;
    int volume = -1;
    int led = -1;

    while ((c=getopt(argc,argv,"p:t:Dhxuds:v:l:"))!=-1) {
        switch (c) {
            case 'p':
                hiddev_path = optarg;
                break;
            case 't':
                sscanf(optarg,"%g",&station);
                break;
            case 'D':
                debug++;
                break;
            case 'x':
                dump_xml = 1;
                break;
            case 'v':
                sscanf(optarg,"%d",&volume);
                break;
            case 'h':
                usage(argv[0]);
                exit(0);
                break;
            case 'u':
                seek = 1;
                up = 1;
                break;
            case 'd':
                seek = 1;
                up = 0;
                break;
            case 's':
                sscanf(optarg,"%d",&strength);
                break;
            case 'l':
                sscanf(optarg,"%d",&led);
                break;
            case '?':
                if (isprint(optopt))
                    fprintf(stderr,"Unknown option '-%c'.\n",optopt);
                else
                    fprintf(stderr,"Unknown option character '\\x%x'.\n",optopt);
                usage(argv[0]);
                exit(1);
            default:
                usage(argv[0]);
                exit(1);
        }
    }

    /*! create chumby radio interface instance */
    {
        crad_info_t crad_info = { 0 };

        int ret = crad_create(&crad_info, &p_crad);

        if(CRAD_FAILED(ret)) 
        { 
            fprintf(stderr, "Error: crad_create failed (%s)\n", CRAD_RETURN_CODE_LOOKUP[ret]);
            exit(1);
        }
    }

    /*! initialize Chumby Radio instance */
    {
        int ret = crad_refresh(p_crad, CRAD_DEFAULT_DEVICE_PATH);

        if(CRAD_FAILED(ret))
        {
            fprintf(stderr, "Error: crad_refresh failed (%s)\n", CRAD_RETURN_CODE_LOOKUP[ret]);
            exit(1);
        }
    }

    if (station>=87.5 && station<=108.0) {
        if (debug) dump_radio_registers(p_crad);
        tune_radio(p_crad,station*100);
        if (debug) dump_radio_registers(p_crad);
    } else if (seek) {
        if (debug) dump_radio_registers(p_crad);
        seek_radio(p_crad,up,strength);
        if (debug) dump_radio_registers(p_crad);
    }
    if (volume>=0 && volume<=15) {
        set_radio_volume(p_crad,volume);
    }
    if (led>=0 && led<=7) {
        set_radio_led(p_crad,1<<led);
    }
    if (dump_xml) dump_radio_xml(p_crad);

cleanup:

    /*! cleanup Chumby Radio instance */
    if(p_crad != 0)
    {
        int ret = crad_close(p_crad);

        if(CRAD_FAILED(ret))
        {
            fprintf(stderr, "Warning: crad_close failed (0x%.08X)\n", ret);
            return 1;
        }
    }

    return 0;
}

static void usage(char *s) {
    printf("usage: %s\n",s);
    printf(
        "\t-t <station> (tune to station)\n"
        "\t-u (seek up)\n"
        "\t-d (seek down)\n"
        "\t-s <seek strength> (set minumum signal strength for seek [20])\n"
        "\t-v <volume> (set volume 0..15)\n"
        "\t-x (output status xml)\n"
        "\t-p <path> (set path to device directory, [/dev])\n"
        "\t-l <value> (set the LED color/behavior 0..7)\n"
        "\t-h (print this message)\n"
        "\t-D (turn on debug output)\n"
    );
    return;
}

static void dump_radio_xml(crad_t *p_crad) {
    char buff[4096];
    int ret = crad_get_status_xml(p_crad, buff, sizeof(buff));
    if(CRAD_SUCCESS(ret))
    {
        printf("%s", buff);
    }
}

