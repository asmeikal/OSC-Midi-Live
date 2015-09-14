/**
 * OSC-Midi-Live
 * Copyright (C) 2015 Michele Laurenti
 * email: asmeikal [at] me [dot] com
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You can find a copy of the GNU General Public License at 
 * http://www.gnu.org/licenses/gpl.html
 */

#include <CopyrightNotice.h>

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void copyright_notice(char *pname, char *years, char *author)
{
    printf("\n");
    printf("%s Copyright (C) %s %s\nThis program comes with ABSOLUTELY NO WARRANTY; for details type 'w'.\nThis is free software, and you are welcome to redistribute it\nunder certain conditions; type 'c' for details. Type 'q' to exit.\nType anything else to continue.\n", pname, years, author);

    char in[10];

    while(1) {
        printf("\n> ");
        fgets(in, 10, stdin);

        switch(in[0]) {
            case 'c':
                printf("\nPlease visit http://www.gnu.org/licenses/gpl.html for details.\n");
                break;

            case 'w':
                printf("\nTHERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY \nAPPLICABLE LAW. EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT \nHOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM “AS IS” WITHOUT WARRANTY \nOF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, \nTHE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR \nPURPOSE. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE \nPROGRAM IS WITH YOU. SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME \nTHE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION.\n");
                break;

            case 'q':
                raise(SIGINT);
                break;

            default:
                printf("\n");
                return;
        }

    }

}
