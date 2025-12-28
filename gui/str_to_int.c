
// Store integer value of the given string in out
// Returns 0 on success, -1 if the given string doesn't state a integer value
// Returns 1 if the given string is empty
// '-' is supported for negative numbers
int str_to_int (const char* str, int* out) {
    int val =0;
    int negative =0;

    // Ignore spaces on front
    while (*str == ' ')
        str++;

    // Check if the string is empty
    if (!(*str)) return 1;


    if (*str == '-') {
        str++;
        negative =1;
        // Check if the only character is '-'
        if (!(*str)) return -1;
    }

    for (const char *ch = str; *ch; ch++) {
        // Check if the character isn't an integer
        if (*ch < '0' || *ch > '9') {
            // Check if there is a character after spaces
            // if so, return -1
            if (*ch == ' ') {
                while (*ch) {
                    if (*ch != ' ') return -1;
                    ch++;
                }
            }
            // Returns -1 if the character isn't space
            else return -1;
        }


        val *= 10;
        val += (*ch - '0');
    }

    if (negative)
        val *= -1;
    *out = val;
    return 0;
}

