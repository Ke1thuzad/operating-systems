#include "error_handler.h"

int throw_err(errcode err) {
    char* err_msg;
    switch(err) {
        case OUT_OF_BOUNDS:
            err_msg = "Option is out of allowed bounds.";
            break;
        case NOT_A_NUMBER:
            err_msg = "Number was expected, not a number got instead.";
            break;
        case INCORRECT_ARGUMENTS:
            err_msg = "Entered arguments are not valid.";
            break;
        case INCORRECT_OPTION:
            err_msg = "Entered option is not supported.";
            break;
        case MEMORY_NOT_ALLOCATED:
            err_msg = "Memory was not allocated";
            break;
        case FILE_ERROR:
            err_msg = "Failed to open file.";
            break;
        case PIPE_ERROR:
            err_msg = "Failed to open pipe.";
            break;
        case FORK_ERROR:
            err_msg = "Failed to fork a process.";
            break;
        default:
            err_msg = "An unknown message has occurred.";
            break;
    }
    write(STDERR_FILENO, err_msg, len(err_msg));
    write(STDERR_FILENO, "\n", 1);
    return (err);
}