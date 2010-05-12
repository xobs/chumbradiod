/*
 * main.cpp
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

#include <stdlib.h>
#include <stdio.h>
#include <csignal>
#include <iostream>
#include <chumby_httpd/chumby_http_server.h>

#include "crad_content_handler.h"
#include "crad_crossdomain_handler.h"
#include "crad_interface.h"

using namespace std;
extern crad_t *p_crad;

/*! print program usage screen */
static void show_usage();




static void sigterm_handler( int sig )
{
    FILE *inp_file = popen("service_control chumbradioplay stop", "r");
    pclose( inp_file );
    exit( 1 );
}


/*! daemon program entry point */
int main(int argc, char **argv) 
{
    /*! HTTP server instance */
    chumby::HTTPServer *p_server = 0;
    
    /*! port number for HTTP server */
    uint16_t port_number = 8081;

    /*! options for stdout */
    int print_usage = 0;

    signal( SIGTERM, sigterm_handler );
    signal( SIGPIPE, SIG_IGN );

    /*! parse command line */
    {
        int cur_arg = 0;

        for(cur_arg = 1; cur_arg < argc; cur_arg++)
        {
            /*! expect all options to begin with '-' character */
            if(argv[cur_arg][0] != '-') 
            { 
                fprintf(stderr, "Warning: Unrecognized option \"%s\"\n", argv[cur_arg]);
                continue; 
            }

            /*! process command options */
            switch(argv[cur_arg][1])
            {
                case 'p':
                {
                    /*! skip over to filename */
                    if(++cur_arg >= argc) { break; }

                    /*! parse port number */
                    sscanf(argv[cur_arg], "%hu", &port_number);
                }
                break;

                case '-':
                    print_usage = 1;
                    break;

                default:
                    fprintf(stderr, "Warning: Unrecognized option \"%s\"\n", argv[cur_arg]);
                    break;
            }
        }
    }

    /*! optionally, print usage */
    if(print_usage)
    {
        show_usage();
        goto cleanup;
    }


    /*! create chumby radio interface instance */
    if(!p_crad) {
        crad_info_t crad_info = { 0 };

        int ret = crad_create(&crad_info, &p_crad);

        if(CRAD_FAILED(ret))
        {
            fprintf(stderr, "Error: crad_create failed (%s)\n", CRAD_RETURN_CODE_LOOKUP[ret]);
            goto cleanup;
        }
    }


    p_server = new chumby::HTTPServer(port_number, 1);

    if(p_server == 0) { goto cleanup; }

    p_server->addContentHandler(new chumby::HTTPSimpleFileContentHandler("/", "/usr/widgets/chumbradiod.html", "text/html"));
    p_server->addContentHandler(new chumby::HTTPSimpleFileContentHandler("/m.swf", "/usr/widgets/chumbradiod.swf", "application/x-shockwave-flash"));
    p_server->addContentHandler(new chumby::HTTPSimpleFileContentHandler("/user", "/mnt/usb/chumbradiod.html", "text/html"));
    p_server->addContentHandler(new chumby::HTTPSimpleFileContentHandler("/user/m.swf", "/mnt/usb/chumbradiod.swf", "application/x-shockwave-flash"));
    p_server->addContentHandler(new ChumbRadioContentHandler());
    p_server->addContentHandler(new ChumbRadioCrossDomainHandler());
    p_server->start();

cleanup:

    /*! cleanup HTTP server instance */
    if(p_server != 0)
    {
        delete p_server;
        p_server = 0;
    }

    return 0;
}

static void show_usage()
{
    printf("chumbradiod 1.0 [caustik@chumby.com]\n");
    printf("\n");
    printf("Usage : chumbradiod [-p PORT]\n");
    printf("\n");
    printf("Chumby Radio HTTP daemon\n");
    printf("\n");
    printf("Options:\n");
    printf("\n");
    printf("    --help      Display this help screen\n");
    printf("\n");
    printf("    -p <PORT>   Serve using the specified port number\n");
    printf("\n");
    return;
}

