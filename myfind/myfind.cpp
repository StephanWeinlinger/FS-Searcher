#include <iostream>
#include <filesystem>
#include <string>

#include <getopt.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/*
    After preparing the input parameters and flags a for-loop forks a child process for each file to search. Depending on the
    -R flag it gets searched recursively or not. With the usage of directory_iterator or recursive_directory_iterator, all files and folders
    except . and .. get looped through and checked if they match with the input file (folders get excluded using is_regular_file). Depending
    on the -i flag the name gets matched case sensitive or insensitive, where strcmp handles the first case and strcasecmp the second one. If
    a file gets matched the process id, name and absolute path gets send to stdout and the stream gets flushed using endl. The search continous
    as long as there are files or directories left to check, so a second match is possible. After the search the child process terminates. During
    this time the parent process waits for the termination of any child process using waitpid with -1 as its first parameter. As soon as a child
    terminates waitpid returns its pid and a while loop causes waitpid to run again and wait for the next child. If no childs remain waitpid returns
    -1, which causes the while loop to run and break since an if condition gets satisfied.
*/

namespace fs = std::filesystem;

const char* program_name = nullptr;
int flag_R = 0;
int flag_i = 0;

// function for usage output
void print_usage() {
    std::cerr << "Usage: " << program_name << " [-R][-i] searchpath filename1[filename2] …[filenameN]" << std::endl;
    exit(EXIT_FAILURE);
}

// function for searching file recursively
void searchFileRecursive(const char* start, const char* name) {
    for (auto const& entry : fs::recursive_directory_iterator(start)) {
        std::string entryname = entry.path().filename();
        if (entry.is_regular_file() && ((flag_i && !strcasecmp(entryname.c_str(), name)) || (!flag_i && !strcmp(entryname.c_str(), name)))) {
            std::cout << getpid() << ": " << name << ": " << fs::canonical(entry.path()) << std::endl;
        }
    }
}

// function for searching file
void searchFile(const char* start, const char* name) {
    for (auto const& entry : fs::directory_iterator(start)) {
        std::string entryname = entry.path().filename();
        if (entry.is_regular_file() && ((flag_i && !strcasecmp(entryname.c_str(), name)) || (!flag_i && !strcmp(entryname.c_str(), name)))) {
            std::cout << getpid() << ": " << name << ": " << fs::canonical(entry.path()) << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    program_name = argv[0];
    int input = 0;
    int error = 0;

    while ((input = getopt(argc, argv, "Ri")) != EOF) {
        switch (input) {
            case 'R': // recursive search
                if (flag_R) { // multiple usage
                    error = 1;
                    break;
                }
                flag_R = 1;
                break;
            case 'i': // case insensitive search
                if (flag_i) { // multiple usage
                    error = 1;
                    break;
                }
                flag_i = 1;
                break;
            case '?': // invalid argument
                error = 1;
                break;
            default: // not possible
                assert(0);
        }
    }
    if (error) {
        print_usage();
    }
    if (argc < optind + 1) {
        print_usage();
    }

    // fork child process for each file
    for (int i = optind + 1; i <= argc - 1; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            if (flag_R) {
                searchFileRecursive(argv[optind], argv[i]);
            } else {
                searchFile(argv[optind], argv[i]);
            }
            return EXIT_SUCCESS;
        }
    }

    // wait for all childs to finish execution
    pid_t childpid;
    while (childpid = waitpid(-1, NULL, 0)) {
        if ((childpid == -1) && (errno != EINTR)) {
            break;
        }
    }

    return EXIT_SUCCESS;
}