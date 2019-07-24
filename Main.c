#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "Life_generator.h"
#include "Make_png.h"
#include "Board.h"

void parse_range(char *string, int *from, int *to) {
    sscanf(string, "<%d,%d>", from, to);
}

int *parse_list(char *string) {
    unsigned int count = 0;
    char *tmp = string;
    char *token = NULL;
    int *result;
    while (*tmp) {
        if (*(tmp++) == ' ') count++;
    }

    result = malloc(count * sizeof(*result));

    if (result) {
        unsigned int i = 0;
        token = strtok(string, " ");
        while (token) {
            *(result + i++) = atoi(token);
            token = strtok(0, " ");
        }
	*(result + i) = -1;
        return result;
    }
}

char *usage =
        "Usage: %s -FILE file_path [ -FINAL file_path ] [ -STOP number_of_generations ] [ -PNG \"<a,b>\" | \"a b c ...\" ] [ -TXT \"<a,b>\" | \"a b c ...\" ]\n"
        "       %s -FILE file_path [ -SBS ]\n"
        "            if -FINAL file_path is given then\n"
        "               writes final configuration file to file_path\n"
        "            endif\n"
        "            if -STOP number_of_generations is given then\n"
        "               sets number of generations made to given value\n"
        "            endif\n"
        "            if -PNG \"<a,b>\" | \"a b c ...\" is given then\n"
        "               creates PNG files:\n"
        "               - \"<a,b>\" form range from a to b\n"
        "               - \"a b c ...\" from generations number a, b, c, ...\n"
        "            endif\n"
        "            if -TXT \"<a,b>\" | \"a b c ...\" is given then\n"
        "               creates TXT configuration files:\n"
        "               - \"<a,b>\" form range from a to b\n"
        "               - \"a b c ...\" from generations number a, b, c, ...\n"
        "            endif\n"
        "            if -SBS is given then\n"
        "               runs program in step by step mode and lets user decide what to do at every step\n";

int main(int argc, char **argv) {

    int opt;
    char *progname = argv[0];

    char *configuration_filename = NULL;
    char *final_configuration_filename = NULL;
    int generation_limit = 0;
    int png_from = 0;
    int png_to = 0;
    int txt_from = 0;
    int txt_to = 0;
    int SBS_state = 0;
    int *txt_generation_index_list = NULL;
    int *png_generation_index_list = NULL;

    while (1) {
        int option_index = 0;
        int optind = option_index ? option_index : 1;

        static struct option long_options[] = {
                {"FILE",  required_argument, 0, 0},
                {"STOP",  required_argument, 0, 0},
                {"PNG",   required_argument, 0, 0},
                {"TXT",   required_argument, 0, 0},
                {"FINAL", required_argument, 0, 0},
                {"SBS",   no_argument,       0, 0},
                {0,       0,                 0, 0}
        };

        opt = getopt_long_only(argc, argv, "", long_options, &option_index);
        if (opt == -1)
            break;


        switch (opt) {
            case 0:
                switch (option_index) {
                    case 0:
                        configuration_filename = optarg;
                        break;
                    case 1:
                        generation_limit = atoi(optarg);
                        break;
                    case 2:
                        if (*optarg == '<') {
                            parse_range(optarg, &png_from, &png_to);
                        } else {
                            png_generation_index_list = parse_list(optarg);
                        }
			break;
                    case 3:
                        if (*optarg == '<') {
                            parse_range(optarg, &txt_from, &txt_to);
                        } else {
                            txt_generation_index_list = parse_list(optarg);
                        }
                        break;
                    case 4:
                        final_configuration_filename = optarg;
                        break;
                    case 5:
                        SBS_state = 1;
                        break;
                    default:
                        break;
                }
                break;
            case '?':
                fprintf(stderr, usage, progname, progname);
                exit(EXIT_FAILURE);
            default:
                fprintf(stderr, usage, progname, progname);
                exit(EXIT_FAILURE);
        }

    }

    if (optind < argc) {
        fprintf(stderr, "Bad parameters!\n");
        while (optind < argc)
            fprintf(stderr, "\t\"%s\"\n", argv[optind++]);
        fprintf(stderr, "\n");
        fprintf(stderr, usage, progname, progname);
        exit(EXIT_FAILURE);
    }

    if (configuration_filename != NULL) {
        FILE *in = NULL;
        if ((in = fopen(configuration_filename, "r")) == NULL) {
            fprintf(stderr, "File error: can't open %s\n", configuration_filename);
            fprintf(stderr, usage, progname, progname);
            exit(EXIT_FAILURE);
        }

        Board *gra = read_config(in);
        char destination[40];
        if (SBS_state) {
            int i = 0, c;
            while (SBS_state) {
                system("clear");
                printf("Numer generacji: %d\n", i + 1);
                print(gra);
                printf("Wybierz opcje:\n"
                       "1 - Następna generacja\n"
                       "2 - Zapis bierzącej generacji jako tekstowy plik z konfiguracją\n"
                       "3 - Zapis bierzącej generacji jako obraz w formacie PNG\n"
                       "4 - Zakończ\n");
                c = getchar();
                switch (c) {
                    case '1':
                        generate(gra);
                        i++;
                        break;
                    case '2':
                        sprintf(destination, "Generations/generation_%d.txt", i + 1);
                        save_config(destination, gra);
                        break;
                    case '3':
                        sprintf(destination, "Images/generation_%d.png", i + 1);
                        process_file(gra);
                        write_png_file(destination);
                        break;
                    case '4':
                        SBS_state = 0;
                        break;
                    default:
                        break;
                }
            }

        } else {
            for (int i = 0; i < (generation_limit ? generation_limit : 20); i++) {

                generate(gra);

                if (i + 1 >= png_from && i + 1 <= png_to) {
                    process_file(gra);
                    sprintf(destination, "Images/generation_%d.png", 1000 + i + 1);
                    write_png_file(destination);
                }

		if (png_generation_index_list) {
		    int *tmppng = png_generation_index_list; 
		    while(*png_generation_index_list != -1) {
		        if ((i+1) == *png_generation_index_list) { 
			    process_file(gra);
                   	    sprintf(destination, "Images/generation_%d.png", 1000 + i + 1);
                   	    write_png_file(destination);
			}
                   	png_generation_index_list++;
		    }
		    png_generation_index_list = tmppng;
		}
		
		if (txt_generation_index_list) {
		    int *tmptxt = txt_generation_index_list; 
		    while(*txt_generation_index_list != -1) {
		        if ((i+1) == *txt_generation_index_list) { 
                    	    sprintf(destination, "Generations/generation_%d.txt", i + 1);
                    	    save_config(destination, gra);
			}
                   	txt_generation_index_list++;
		    }
		    txt_generation_index_list = tmptxt;
		}

                if (i + 1 >= txt_from && i + 1 <= txt_to) {
                    sprintf(destination, "Generations/generation_%d.txt", i + 1);
                    save_config(destination, gra);
                }
            }

            if (final_configuration_filename != NULL) {
                save_config(final_configuration_filename, gra);
            }
        }
        for (unsigned int i = 0; i < gra->numOfRows; ++i)
	    free(gra->matrix[i]);
        free(gra->matrix);
	free(gra);
        fclose(in);
    } else {
        fprintf(stderr, "File error: no configuration file specified\n");
        fprintf(stderr, usage, progname, progname);
        exit(EXIT_FAILURE);
    }
    free(txt_generation_index_list);
    free(png_generation_index_list);
    exit(EXIT_SUCCESS);
}
