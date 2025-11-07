#include <stdio.h>
#include <string.h>

void get_author_and_track_name(const char *filename, char *author, char *track) {
    char *start_author = strstr(filename, " - ");
    if (start_author == NULL) {
        printf("Invalid format: could not find ' - '.\n");
        return;
    }
    start_author += 3; // Move the pointer right after first ' - '

    char *start_trackname = strstr(start_author, " - ");
    if (start_trackname == NULL) {
        printf("Invalid format: could not find the second ' - '.\n");
        return;
    }

    // Copy author to the author buffer
    strncpy(author, start_author, start_trackname - start_author);
    author[start_trackname - start_author] = '\0'; // Add null terminator

    // Check if trackname has a .flac extension
    char *ext = strstr(start_trackname + 3, ".flac");
    if (ext == NULL) {
        printf("Invalid format: could not find '.flac' extension.\n");
        return;
    }

    // Copy trackname to the trackname buffer without the extension
    strncpy(track, start_trackname + 3, ext - (start_trackname + 3));
    track[ext - (start_trackname + 3)] = '\0'; // Add null terminator
}
