#ifndef _MESSAGEUTILITIES_H
#define _MESSAGEUTILITIES_H

template <typename data>
void parseString(char string_in[], const char delimiter[], data output_array[])
{
    char *pch;
    pch = strtok(string_in, delimiter);
    byte i = 0;
    while (pch != NULL)
    {
        output_array[i] = atof(pch);
        pch = strtok(NULL, delimiter);
        i++;
    }
}

template <typename data>
void buildString(data data_to_string[], unsigned char number_of_elements, char string_out[], const char format[], const char delimiter[] = "")
{
    char submessage[12];
    unsigned char element;
    strcpy(string_out, "");
    for (element = 0; element < number_of_elements; element++)
    {
        snprintf(submessage, sizeof(submessage), format, data_to_string[element]);
        strcat(string_out, submessage);
        if (element < number_of_elements - 1)
            strcat(string_out, delimiter);
    }
}

#endif // _MESSAGEUTILITIES_H